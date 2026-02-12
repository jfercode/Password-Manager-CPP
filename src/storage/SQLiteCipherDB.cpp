#include "SQLiteCipherDB.hpp"

// TODO: Implement all methods here
// Start with: Constructor -> Helper -> Destructor -> Main Methods
SQLiteCipherDB::SQLiteCipherDB() : db(nullptr), dbPath("") // TODO: pensar en un path para la db
{
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - Initializing db...");

    // Trying to open or create the db
    int dbRes = sqlite3_open(dbPath, &db);
    if (dbRes != 1)
        throw std::runtime_error(RED "Error" RESET " while trying to initialize the db");

    // Set up db
    setupDB();

    PrintLog(std::cout, CYAN "SQLiteCipherDB" GREEN " - db running!");
}

void SQLiteCipherDB::setupDB(void)
{
    // sql variable (SQL execution)
    const char *sql = "CREATE TABLE IF NOT EXISTS users("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "username TEXT UNIQUE NOT NULL,"
                      "password_hash TEXT NOT NULL,"
                      "password_salt TEXT NOT NULL,"
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
                                const std::string &salt) const
{
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - Adding new user %s...", username);

    // sql line
    const char *sql = "INSERT INTO users (username, password_hash, password_salt) VALUES (?, ?, ?);";

    // prepare order to db
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    // Binding parameters values
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, passwordHash.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, salt.c_str(), -1, SQLITE_STATIC);

    // Send the order to the spql
    int rSql = sqlite3_step(stmt);
    if (rSql != SQLITE_DONE)
    {
        sqlite3_finalize(stmt); // finalize db order
        PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - " RED " Can´t add user %s to the db" RESET, username);
        return false;
    }
    sqlite3_finalize(stmt); // finalize db order
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " -  user %s added to the db" RESET, username);
    return true;
}

// Look up a user and return their stored hash and salt
bool SQLiteCipherDB::getUserHash(
    const std::string &username,
    std::string &hash,
    std::string &salt) const
{
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - Obtaining user %s hash and salt...", username);

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
        PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - " RED " Can´t find %s in the db" RESET, username);
        return false;
    }

    // Extract column data
    const unsigned char *hash_ptr = sqlite3_column_text(stmt, 0); // Column 0 hash
    const unsigned char *salt_ptr = sqlite3_column_text(stmt, 2); // Column 1 salt

    if (hash_ptr != nullptr && salt_ptr != nullptr)
    {
        hash = std::string(reinterpret_cast<const char *>(hash_ptr));
        salt = std::string(reinterpret_cast<const char *>(salt_ptr));
    }
    else
    {
        sqlite3_finalize(stmt); // finalize db order
        if (hash_ptr == nullptr)
            PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - " RED " Can´t find %s hash in the db" RESET, username);
        if (salt_ptr == nullptr)
            PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - " RED " Can´t find %s salt in the db" RESET, username);
        return false;
    }
    sqlite3_finalize(stmt); // finalize db order
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - %s hash and salt obtaining", username);
    return true;
}

// Check if username already exists (prevent duplicates)
bool SQLiteCipherDB::userExists(const std::string &username) const
{
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - Looking for %s user...", username);

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
        PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - " RED " Can´t find %s user in the db" RESET, username);
        return false;
    }
    // Get column 0 (username) as integer
    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);  // finalize db order
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " -  %s user appears %d times", username, count);
    return (count > 0);
}
