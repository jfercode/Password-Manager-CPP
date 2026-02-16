#include "SQLiteCipherDB.hpp"

// Start with: Constructor -> Helper -> Destructor -> Main Methods
SQLiteCipherDB::SQLiteCipherDB() : db(nullptr), dbPath("")
{
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - Initializing db...");

    // Looking for the database path
    if (!findDataBasePath())
        throw std::runtime_error(RED "Error" RESET " failed to determinate database path");

    // Trying to open or create the db
    int dbRes = sqlite3_open(dbPath.c_str(), &db);
    if (dbRes != SQLITE_OK)
        throw std::runtime_error(std::string(RED "Error" RESET " opening DB: ") + sqlite3_errmsg(db));

    // Set up db
    setupDB();

    PrintLog(std::cout, CYAN "SQLiteCipherDB" GREEN " - db running!" RESET);
}

bool SQLiteCipherDB::findDataBasePath(void)
{
    // Get Home environment variable value to build and check the path
    const char *homeEnv = std::getenv("HOME");
    if (!homeEnv)
    {
        PrintLog(std::cerr, RED "Error: HOME environment variable not found" RESET);
        return false;
    }

    // Build the directory path in $HOMEDIR/.local/share/passman
    std::string baseDir = std::string(homeEnv) + "/.local";
    std::string shareDir = baseDir + "/share";
    std::string appDir = shareDir + "/passman";

    // Ensure that ~/.local/share/passman exits if not create it
    if (!createDirectory(baseDir) || !createDirectory(shareDir) || !createDirectory(appDir))
        return false;

    // Assign db path
    dbPath = appDir + "/passman.db";
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - DB Path: %s", dbPath.c_str());

    return true;
}

void SQLiteCipherDB::setupDB(void)
{
    // sql variable (SQL execution)
    const char *sql = "CREATE TABLE IF NOT EXISTS users("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "username TEXT UNIQUE NOT NULL,"
                      "password_hash TEXT NOT NULL,"
                      "password_salt TEXT NOT NULL,"
                      "is_admin INTEGER DEFAULT 0,"
                      "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)";

    // Try to mount sql db
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - Executing sql...");
    char *errMsg = nullptr;
    int rSql = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rSql != SQLITE_OK)
    {
        PrintLog(std::cerr, RED "%s\n", sqlite3_errmsg(db));
        sqlite3_free(errMsg);
        throw std::runtime_error(RED "Error" RESET " Failed to create table");
    }
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - Setup completed!");
}

SQLiteCipherDB::~SQLiteCipherDB()
{
    // Check for db state
    if (db != nullptr)
        sqlite3_close(db);
    db = nullptr;

    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - db closed");
}

// Insert a new user into the db
bool SQLiteCipherDB::createUser(const std::string &username,
                                const std::string &passwordHash,
                                const std::string &salt,
                                bool isMaster) const
{
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - Adding new user %s...", username.c_str());

    // sql line
    const char *sql = "INSERT INTO users (username, password_hash, password_salt, is_admin) VALUES (?, ?, ?, ?);";

    // prepare order to db
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    // Binding parameters values
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, passwordHash.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, salt.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, isMaster ? 1 : 0);

    // Send the order to the spql
    int rSql = sqlite3_step(stmt);
    if (rSql != SQLITE_DONE)
    {
        sqlite3_finalize(stmt); // finalize db order
        PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - " RED " Can´t add user %s to the db" RESET, username.c_str());
        return false;
    }
    sqlite3_finalize(stmt); // finalize db order
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " -  user %s added to the db" RESET, username.c_str());
    return true;
}

// Look up a user and return their stored hash and salt
bool SQLiteCipherDB::getUserHash(
    const std::string &username,
    std::string &hash,
    std::string &salt) const
{
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - Obtaining user %s hash and salt...", username.c_str());

    // sql line => Obtain password_hash and salt from the users table where username matches the parameter
    const char *sql = "SELECT password_hash, password_salt FROM users WHERE username = ?";

    // Prepare order to db
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    // Binding parameters values
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hash.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, salt.c_str(), -1, SQLITE_STATIC);

    // Send the order to the sql
    int rSql = sqlite3_step(stmt);
    if (rSql != SQLITE_ROW) // Handle sqlite order here (if not found a SQLITE ROW)
    {
        sqlite3_finalize(stmt); // finalize db order
        PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - " RED " Can´t find %s in the db" RESET, username.c_str());
        return false;
    }

    // Extract column data
    const unsigned char *hash_ptr = sqlite3_column_text(stmt, 0); // Column 0 hash
    const unsigned char *salt_ptr = sqlite3_column_text(stmt, 1); // Column 1 salt

    if (hash_ptr != nullptr && salt_ptr != nullptr)
    {
        hash = std::string(reinterpret_cast<const char *>(hash_ptr));
        salt = std::string(reinterpret_cast<const char *>(salt_ptr));
    }
    else
    {
        sqlite3_finalize(stmt); // finalize db order
        if (hash_ptr == nullptr)
            PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - " RED " Can´t find %s hash in the db" RESET, username.c_str());
        if (salt_ptr == nullptr)
            PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - " RED " Can´t find %s salt in the db" RESET, username.c_str());
        return false;
    }
    sqlite3_finalize(stmt); // finalize db order
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - %s hash and salt obtaining", username.c_str());
    return true;
}

// Check if username already exists (prevent duplicates)
bool SQLiteCipherDB::userExists(const std::string &username) const
{
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - Looking for %s user...", username.c_str());

    // sql line => Returns number of matching rows where username row matchs
    const char *sql = "SELECT COUNT(*) FROM users WHERE username = ?";

    // Prepare order to db
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    // Binding parameters values
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    // Send the order to the sql
    int rSql = sqlite3_step(stmt);
    if (rSql != SQLITE_ROW) // Handle sqlite order here (if not found a SQLITE ROW)
    {
        sqlite3_finalize(stmt); // finalize db order
        PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - " RED " Can´t find %s user in the db" RESET, username.c_str());
        return false;
    }
    // Get column 0 (username) as integer
    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt); // finalize db order
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " -  %s user appears %d times", username.c_str(), count);
    return (count > 0);
}

// Check if username given is master user
bool SQLiteCipherDB::isMasterUser(const std::string &username) const
{
    // Check for user existence
    if (!userExists(username))
    {
        PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - " RED "Master %s user not found" RESET, username.c_str());
        return (false);
    }

    // Check user count (is admin or not)
    const char *countSql = "SELECT COUNT(*) FROM users";
    sqlite3_stmt *countStmt = nullptr;
    sqlite3_prepare_v2(db, countSql, -1, &countStmt, nullptr);
    sqlite3_step(countStmt);
    int userCount = sqlite3_column_int(countStmt, 0);
    sqlite3_finalize(countStmt);

    // Decide is_admin
    int is_admin = (userCount == 0) ? 1 : 0;
    if (is_admin)
        PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " -  %s is Master user" RESET, username.c_str());
    else
        PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - " RED "%s is not a Master user" RESET, username.c_str());
    return (is_admin);
}

// Check if system has any master user (for first-time initialization)
bool SQLiteCipherDB::hasMasterUser() const
{
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - Checking if master user exists...");

    // sql line => Check if there's any admin user
    const char *sql = "SELECT COUNT(*) FROM users WHERE is_admin = 1";

    // Prepare order to db
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    // Send the order to the sql
    int rSql = sqlite3_step(stmt);
    if (rSql != SQLITE_ROW)
    {
        sqlite3_finalize(stmt);
        PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - " RED "Error checking for master user" RESET);
        return false;
    }

    // Get the count of admin users
    int adminCount = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    if (adminCount > 0)
    {
        PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - " GREEN "Master user exists" RESET);
        return true;
    }
    else
    {
        PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - " YELLOW "No master user found - system needs initialization" RESET);
        return false;
    }
}
