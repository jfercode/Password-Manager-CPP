# 🚀 SQLite Implementation - Step-by-Step Walkthrough

## Overview

This guide walks you through implementing SQLiteCipherDB in small, testable steps.

**Key principle:** Implement one thing at a time, compile, and verify it works before moving to the next.

---

## Phase 1: Header File Setup

### Step 1.1: Add SQLite3 Include

**What to do:**
In `include/SQLiteCipherDB.hpp`, at the top of the file where other includes are:

```
#include <string>
// Add here:
```

**Why?**
- `<sqlite3.h>` gives you access to all SQLite3 functions
- It defines `sqlite3` type, all the function names, constants like `SQLITE_OK`

**How to check:**
After adding, try to compile. You should NOT get errors about sqlite3_open, sqlite3_close, etc. being undefined.

---

### Step 1.2: Add Private Members

**Current state of private section:**
```cpp
private:
    // TODO: Add private members here
    // HINT: You need a pointer to the database handle
    // HINT: You need a constant string for the database file path
```

**What to add:**

```
A variable to hold the database connection (pointer)
A constant string for the path
```

**Things to think about:**
1. Type for database pointer?
   - Look at sqlite3 documentation
   - It will be `sqlite3 *`
   - Name it something clear like `db`

2. Type for path?
   - It's a file path, so `std::string`
   - It never changes, so `const`
   - Value: `"/tmp/password_manager.db"`

**Result should look like:**
```cpp
private:
    // Your members here (not the answers!)
```

---

### Step 1.3: Add Private Helper Method Declaration

**What to add:**
A declaration for a method that sets up the database schema.

**Method signature:**
- Returns: `void` (no return value needed)
- Name: `initializeDatabase` or similar
- Parameters: none

**Why private?**
- Only called by constructor
- Users don't need to call it directly

---

### Step 1.4: Compile Header Only

**Command:**
```bash
cd /path/to/project && g++ -c include/SQLiteCipherDB.hpp
```

**What you're checking:**
- No syntax errors in header
- All includes are available

**If errors:**
- Check your syntax (did you close all braces?)
- Check includes are spelled correctly

---

## Phase 2: Implementation File - Constructor

### Step 2.1: Write Constructor Skeleton

**In `src/storage/SQLiteCipherDB.cpp`:**

```cpp
#include "SQLiteCipherDB.hpp"
#include <iostream>

SQLiteCipherDB::SQLiteCipherDB() {
    // TODO: Implement
}
```

**What you need to add:**

1. **Initialize db member**
   - In the initializer list (after the `:`)
   - Set it to `nullptr`

   **Why initializer list?**
   - It's the C++ way to initialize members
   - Runs before the function body

2. **Add debug log**
   - Print something like "Opening database"
   - Use the PrintLog function you see in other files

3. **Open the database file**
   - Call `sqlite3_open(filepath, &db)`
   - Store result in an `int rc` variable
   - This opens OR CREATES the database file

4. **Check for errors**
   - If `rc != SQLITE_OK`, something went wrong
   - Print the error: `sqlite3_errmsg(db)`
   - Throw an exception: `throw std::runtime_error("message")`

5. **Call helper to initialize schema**
   - Call: `this->initializeDatabase()`

---

### Step 2.2: Implementation Pattern

**Pattern for constructor:**

```cpp
ClassName::ClassName() : memberVar(initial_value), otherVar(value) {
    // Step 1: Log
    std::cout << "Starting..." << std::endl;
    
    // Step 2: Do main work
    int result = someFunction();
    
    // Step 3: Check for errors
    if (result != SUCCESS_CODE) {
        // Handle error
        throw std::runtime_error("Error message");
    }
    
    // Step 4: Call helpers
    helperFunction();
    
    std::cout << "Done!" << std::endl;
}
```

---

### Step 2.3: Compile and Test Constructor

**After writing constructor:**

```bash
cd /path/to/project && make compile
```

**What should happen:**
- No errors about the constructor
- The program might not run yet (depends on main), but should compile

---

## Phase 3: Implementation File - Destructor

### Step 3.1: Write Destructor

**In destructor:**

1. **Check if db is open**
   - If `db != nullptr`, it's open
   - If `db == nullptr`, it's already closed

2. **Close the database**
   - Call `sqlite3_close(db)`

3. **Set to nullptr**
   - After closing: `db = nullptr`

**Why check and set to nullptr?**
- Safety: prevents accidentally using closed database
- Prevents double-close (closing already-closed pointer)

**Important:**
- Destructors should NOT throw exceptions
- Just log and continue

---

### Step 3.2: Compile

```bash
make compile
```

---

## Phase 4: Helper Method - Initialize Database

### Step 4.1: Understand the Goal

Your database needs a `users` table. This method creates it when the database opens.

It uses `CREATE TABLE IF NOT EXISTS` so:
- First run: creates the table
- Later runs: does nothing (table already exists)

### Step 4.2: Write the SQL

**In the method:**

1. Create a `const char *sql` variable
2. Set it to your CREATE TABLE statement

**SQL structure:**
```sql
CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password_hash TEXT NOT NULL,
    password_salt TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

**Breaking it down:**
- `CREATE TABLE IF NOT EXISTS users` - Create table named "users" if not there
- `id INTEGER PRIMARY KEY AUTOINCREMENT` - Auto-incrementing unique ID (1, 2, 3...)
- `username TEXT UNIQUE NOT NULL` - Username must be unique and present
- `password_hash TEXT NOT NULL` - Hash must be present
- `password_salt TEXT NOT NULL` - Salt must be present
- `created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP` - Auto-fill with current time

### Step 4.3: Execute the SQL

**Use `sqlite3_exec`:**

```cpp
char *errMsg = nullptr;
int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
```

**What this does:**
- Executes the SQL statement
- If error, fills `errMsg` with message
- Returns code in `rc`

### Step 4.4: Handle Errors

```cpp
if (rc != SQLITE_OK) {
    // Print error
    std::cerr << sqlite3_errmsg(db) << std::endl;
    
    // Free error message
    sqlite3_free(errMsg);
    
    // Throw exception
    throw std::runtime_error("Failed to create table");
}
```

### Step 4.5: Compile

```bash
make compile
```

---

## Phase 5: Main Method - createUser()

### Step 5.1: Understand Parameters

```cpp
bool createUser(
    const std::string &username,      // Person's username
    const std::string &passwordHash,  // PBKDF2 hash from CryptoManager
    const std::string &salt           // Salt used for hashing
) const;
```

**Purpose:** Insert a new user into the database

### Step 5.2: Prepare SQL Statement

**SQL with placeholders:**
```sql
INSERT INTO users (username, password_hash, password_salt) VALUES (?, ?, ?);
```

**Why `?` instead of actual values?**
- Security: prevents SQL injection
- Flexibility: reuse same statement for different values

**Steps:**

1. Create `const char *sql` with the SQL string
2. Create `sqlite3_stmt *stmt = nullptr`
3. Call `sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr)`
4. Check return code: `if (rc != SQLITE_OK) { handle error }`

### Step 5.3: Bind Parameters

**After preparing, bind the values:**

```cpp
sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 2, passwordHash.c_str(), -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 3, salt.c_str(), -1, SQLITE_STATIC);
```

**Breaking it down:**
- `stmt` - the prepared statement
- First number - parameter index (starts at 1, not 0!)
  - 1 = first `?`
  - 2 = second `?`
  - 3 = third `?`
- `.c_str()` - converts std::string to C string
- `-1` - tell SQLite to calculate string length
- `SQLITE_STATIC` - string stays valid (don't copy it)

### Step 5.4: Execute

```cpp
int rc = sqlite3_step(stmt);
if (rc != SQLITE_DONE) {
    // Error: insertion failed
    sqlite3_finalize(stmt);
    return false;
}
```

**Why check for SQLITE_DONE?**
- `SQLITE_DONE` = operation completed successfully
- Other codes = error (user might already exist, etc.)

### Step 5.5: Cleanup and Return

```cpp
sqlite3_finalize(stmt);  // CRITICAL: always do this
return true;
```

**Why finalize?**
- Releases memory allocated by prepare_v2
- If you forget, memory leak

---

## Phase 6: Main Method - getUserHash()

### Step 6.1: Understand Purpose

**Goal:** Look up a user and return their stored hash and salt

**Parameters:**
- `username` - user to search for
- `hash` - reference to string (will be filled with hash)
- `salt` - reference to string (will be filled with salt)

**What are references?**
```cpp
std::string h, s;
db.getUserHash("alice", h, s);
// After call, h and s contain the data
```

### Step 6.2: Write SELECT SQL

```sql
SELECT password_hash, password_salt FROM users WHERE username = ?;
```

**Breaking it down:**
- `SELECT password_hash, password_salt` - Get these two columns
- `FROM users` - From the users table
- `WHERE username = ?` - Where username matches the parameter

### Step 6.3: Prepare and Bind

**Same pattern as createUser:**

```cpp
const char *sql = "SELECT password_hash, password_salt FROM users WHERE username = ?";
sqlite3_stmt *stmt = nullptr;
int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
// Check rc

sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
```

### Step 6.4: Step and Check for Row

```cpp
int rc = sqlite3_step(stmt);
if (rc != SQLITE_ROW) {
    // User not found
    sqlite3_finalize(stmt);
    return false;
}
```

**Why SQLITE_ROW?**
- `SQLITE_ROW` = found a row
- `SQLITE_DONE` = no rows found
- Other = error

### Step 6.5: Extract Column Data

```cpp
const unsigned char *hash_ptr = sqlite3_column_text(stmt, 0);  // Column 0 = hash
const unsigned char *salt_ptr = sqlite3_column_text(stmt, 1);  // Column 1 = salt

if (hash_ptr && salt_ptr) {
    hash = std::string(reinterpret_cast<const char*>(hash_ptr));
    salt = std::string(reinterpret_cast<const char*>(salt_ptr));
} else {
    sqlite3_finalize(stmt);
    return false;
}
```

**Important notes:**
- Columns are indexed starting at 0 (different from parameters!)
- `reinterpret_cast` converts pointer types
- You can only safely read column data BEFORE finalizing

### Step 6.6: Finalize and Return

```cpp
sqlite3_finalize(stmt);
return true;
```

---

## Phase 7: Main Method - userExists()

### Step 7.1: Understand Purpose

**Goal:** Check if username already exists (prevent duplicates)

**Return value:**
- `true` if user exists
- `false` if user doesn't exist

### Step 7.2: Write COUNT SQL

```sql
SELECT COUNT(*) FROM users WHERE username = ?;
```

**Why COUNT?**
- Returns number of matching rows
- Will be 0 (doesn't exist) or 1 (exists)

### Step 7.3: Prepare, Bind, Step

**Same pattern:**

```cpp
sqlite3_prepare_v2(...);
// Check

sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

int rc = sqlite3_step(stmt);
if (rc != SQLITE_ROW) {
    sqlite3_finalize(stmt);
    return false;
}
```

### Step 7.4: Get Integer Result

```cpp
int count = sqlite3_column_int(stmt, 0);  // Column 0, get as integer
sqlite3_finalize(stmt);
return (count > 0);  // true if count > 0, false otherwise
```

**Key differences from getUserHash:**
- `sqlite3_column_int()` instead of `sqlite3_column_text()`
- Only one column to read
- Simple true/false return

---

## 🧪 Testing Each Step

### After implementing each method, test it:

```cpp
// In main() or a test function:

SQLiteCipherDB db;  // Test constructor
std::cout << "Database opened successfully!" << std::endl;

// Test createUser
bool ok = db.createUser("testuser", "hash123", "salt456");
if (ok) std::cout << "User created!" << std::endl;
else std::cout << "User creation failed!" << std::endl;

// Test userExists
bool exists = db.userExists("testuser");
std::cout << "User exists: " << (exists ? "YES" : "NO") << std::endl;

// Test getUserHash
std::string h, s;
bool found = db.getUserHash("testuser", h, s);
if (found) {
    std::cout << "Hash: " << h << std::endl;
    std::cout << "Salt: " << s << std::endl;
}
```

---

## ✅ Completion Checklist

- [ ] Header file has sqlite3.h included
- [ ] Private db pointer added
- [ ] Private DB_PATH constant added
- [ ] initializeDatabase() method declared
- [ ] Constructor implemented (opens DB, calls init)
- [ ] Destructor implemented (closes DB)
- [ ] initializeDatabase() implemented (CREATE TABLE)
- [ ] createUser() implemented (INSERT)
- [ ] getUserHash() implemented (SELECT)
- [ ] userExists() implemented (COUNT)
- [ ] Code compiles without errors
- [ ] Database file is created at /tmp/password_manager.db
- [ ] Can create users without errors
- [ ] Can retrieve user data correctly
- [ ] Can detect duplicates

---

## 🐛 Debugging Tips

**If sqlite3_open fails:**
- Check file path is correct
- Check /tmp directory exists and is writable

**If prepare fails:**
- Check SQL syntax (use sqlite3 command line to test)
- Check parameter count matches ?'s in SQL

**If bind fails:**
- Check parameter index (starts at 1)
- Check value is not nullptr

**If step returns wrong code:**
- Check SQL carefully (typos in table/column names)
- Check data types match (TEXT vs INTEGER)

**Memory issues:**
- Always finalize prepared statements
- Always close database in destructor

---

Now you're ready to implement! Start with Step 1.1 and work through each step. Good luck! 🚀

