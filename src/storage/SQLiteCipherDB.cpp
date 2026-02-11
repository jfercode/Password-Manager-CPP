#include "SQLiteCipherDB.hpp"

// TODO: Implement all methods here
// Start with: Constructor -> Helper -> Destructor -> Main Methods
SQLiteCipherDB::SQLiteCipherDB() : db(nullptr), dbPath("") // TODO: pensar en un path para la db
{
    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - Initializing db...");
    
    // Trying to open or create the db
    int dbRes = sqlite3_open(dbPath, &db);
    if (dbRes != 1)
        throw std::runtime_error(RED "Error" RESET" while trying to initialize the db");
    
    // Set up db
    setupDB();

    PrintLog(std::cout, CYAN "SQLiteCipherDB" GREEN" - db running!");
}

void    SQLiteCipherDB::setupDB(void) {

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
        throw std::runtime_error(RED "Error" RESET" Failed to create table");
    }
}

SQLiteCipherDB::~SQLiteCipherDB() {

    // Check for db state    
    if (db != nullptr)
        sqlite3_close(db);
    db = nullptr;

    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - db closed");
}

// Insert a new user into the db
bool    SQLiteCipherDB::createUser(const std::string &username,
            const std::string &passwordHash,
            const std::string &salt) const {

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
                    PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " - " RED" Can´t add user %s to the db" RESET, username);
                    return false;
                }
                sqlite3_finalize(stmt); // finalize db order
                PrintLog(std::cout, CYAN "SQLiteCipherDB" RESET " -  user %s added to the db" RESET, username);
                return true;
            }

// Look up a user and return their stored hash and salt
bool    SQLiteCipherDB::getUserHash(
            const std::string &username,
            std::string &hash,
            std::string &salt) const {

                
            }

bool    SQLiteCipherDB::userExists(const std::string &username) const {

}
