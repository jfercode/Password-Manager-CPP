# 🎓 SQLite Database Implementation - Complete Teaching Guide

## 📚 Overview

You are going to implement a SQLite database wrapper that manages user credentials. This guide teaches you **how** to think about the problem, not just **what** to code.

---

## 🧠 Part 1: Understanding SQLite3 Fundamentals

### What is SQLite?

SQLite is an **embedded SQL database engine** - it's a library, not a separate server.

**Key Concepts:**
- File-based database (runs on disk, one file = entire DB)
- Zero configuration needed
- Perfect for desktop applications
- Used by Android, iOS, Chrome, Firefox, etc.

### SQLite3 C API

The C API is how we interact with SQLite from C++. Key components:

```
sqlite3 *db           ← Database connection handle (like a file pointer)
sqlite3_stmt *stmt    ← Prepared statement (like a template for a query)
int rc                ← Return code (SQLITE_OK, SQLITE_ERROR, etc.)
```

### Your Database File

In your case: `/tmp/password_manager.db`

This is a **binary file** containing your entire database.

---

## 🏗️ Part 2: Architecture of SQLiteCipherDB

Your class needs to manage the **lifecycle** of database operations:

```
┌──────────────────────────────────────────────┐
│     SQLiteCipherDB Instance (Object)         │
├──────────────────────────────────────────────┤
│                                              │
│  Private Members:                            │
│  ┌────────────────────────────────────────┐ │
│  │ sqlite3 *db                            │ │
│  │   ↓ Points to actual database file     │ │
│  │   /tmp/password_manager.db             │ │
│  └────────────────────────────────────────┘ │
│                                              │
│  ┌────────────────────────────────────────┐ │
│  │ const std::string DB_PATH =            │ │
│  │   "/tmp/password_manager.db"           │ │
│  │   ↓ Where database file lives          │ │
│  └────────────────────────────────────────┘ │
│                                              │
│  Private Methods:                            │
│  ┌────────────────────────────────────────┐ │
│  │ void initializeDatabase()              │ │
│  │   ↓ Called by constructor              │ │
│  │   ↓ Creates 'users' table if needed    │ │
│  └────────────────────────────────────────┘ │
│                                              │
│  Public Methods:                             │
│  ├─ createUser()                            │
│  ├─ getUserHash()                           │
│  └─ userExists()                            │
│                                              │
└──────────────────────────────────────────────┘
```

---

## 🛠️ Part 3: Step-by-Step Implementation Plan

### STEP 1: Add Private Members to Header

**What to add:**
1. A pointer to `sqlite3` named `db`
   - Type: `sqlite3 *`
   - Initialization: initialize to `nullptr` in constructor
   - Why pointer? Because SQLite returns a pointer when you open a database

2. A constant string for database path
   - Type: `const std::string`
   - Value: `"/tmp/password_manager.db"`
   - Why constant? The path never changes

3. A private helper method declaration
   - Name: `void initializeDatabase()`
   - Purpose: Create the 'users' table when database opens

**Header Question:** Where should these go in the class?
- Members go in the `private:` section
- Method declarations go below members

---

### STEP 2: Add Includes to .cpp File

**What includes do you need?**

Think about what you're using:
- `sqlite3.h` - For SQLite3 C API functions
- `iostream` - For error messages (optional, for debugging)
- Any other debugging/logging you want

**Pro Tip:** Look at your existing code - what library do you use for logging?

---

### STEP 3: Implement the Constructor

**Constructor Goal:** Open the database file and prepare the schema

**Your Constructor Should:**

1. **Initialize the db pointer**
   - Set `db = nullptr` in initializer list
   - Why? Safety - null pointer is "not open yet"

2. **Open (or create) the database file**
   - Use: `sqlite3_open()`
   - Parameters: file path (string), pointer to db variable
   - Returns: error code (int)
   - Check return code - if not `SQLITE_OK`, something went wrong

3. **Handle errors**
   - If `sqlite3_open()` fails, what should you do?
   - Option A: Print error and throw exception
   - Option B: Print error and return
   - **Best practice:** Throw exception (constructor failure = object failure)
   - Use: `sqlite3_errmsg(db)` to get error message

4. **Initialize the database schema**
   - Call your helper method: `initializeDatabase()`
   - This creates tables if they don't exist

**Return Code Checking Pattern:**
```
call sqlite3 function
if (return_code != expected_code) {
    handle error
    return false or throw
}
continue
```

---

### STEP 4: Implement Destructor

**Destructor Goal:** Clean up and close the database

**Your Destructor Should:**

1. **Check if db is open**
   - If `db != nullptr`, close it
   - Why check? Avoid trying to close nullptr

2. **Close the database**
   - Use: `sqlite3_close(db)`
   - After: Set `db = nullptr`
   - Why set to nullptr? Safety - prevents double-close

3. **No error handling needed**
   - Destructors shouldn't throw exceptions
   - Print warnings if needed, but don't fail

---

### STEP 5: Implement initializeDatabase() Helper

**Helper Goal:** Create the 'users' table on first run

**Your Helper Should:**

1. **Write CREATE TABLE SQL**
   - Create a `const char *sql` with your SQL statement
   - Table name: `users`
   - Columns needed:
     - `id` - INTEGER PRIMARY KEY AUTOINCREMENT
     - `username` - TEXT, UNIQUE, NOT NULL
     - `password_hash` - TEXT, NOT NULL
     - `password_salt` - TEXT, NOT NULL
     - `created_at` - TIMESTAMP, DEFAULT CURRENT_TIMESTAMP

   **Why each column?**
   - `id`: Auto-generated unique identifier (not required but best practice)
   - `username`: User's login name (UNIQUE prevents duplicate usernames)
   - `password_hash`: PBKDF2 hash (from CryptoManager)
   - `password_salt`: Salt used for hashing (from CryptoManager)
   - `created_at`: When user registered (audit trail)

2. **Execute SQL with sqlite3_exec()**
   - Parameters: db, sql string, callback, arg, error message pointer
   - For CREATE TABLE, callbacks are null
   - Capture error message pointer
   - Check return code

3. **Handle errors**
   - If error, free the error message with `sqlite3_free(errMsg)`
   - Throw exception (table creation failure = fatal)

**SQL Pattern:**
```sql
CREATE TABLE IF NOT EXISTS tablename (
  col1 type constraints,
  col2 type constraints
);
```

---

### STEP 6: Implement createUser()

**Method Goal:** Insert a new user into the users table

**Your Method Should:**

1. **Prepare SQL Statement**
   - Write INSERT SQL: `"INSERT INTO users (username, password_hash, password_salt) VALUES (?, ?, ?)"`
   - The `?` are **placeholders** (prevents SQL injection)
   - Use: `sqlite3_prepare_v2()`
   - This creates a `sqlite3_stmt *stmt`

2. **Bind Parameters**
   - Match placeholders to actual values
   - Use: `sqlite3_bind_text(stmt, parameter_index, value.c_str(), -1, SQLITE_STATIC)`
   - Parameter indices start at 1 (not 0!)
   - `-1` means: calculate string length automatically
   - `SQLITE_STATIC`: SQLite doesn't copy the string (it's still valid)

   **Three parameters to bind:**
   1. First `?` (index 1) ← username
   2. Second `?` (index 2) ← passwordHash  
   3. Third `?` (index 3) ← salt

3. **Execute the statement**
   - Use: `sqlite3_step(stmt)`
   - Return code for success: `SQLITE_DONE` (operation completed)
   - Return code for failure: anything else

4. **Finalize the statement**
   - Use: `sqlite3_finalize(stmt)`
   - **IMPORTANT:** Always call this, even on error
   - Why? Releases memory allocated by `sqlite3_prepare_v2()`

5. **Error Handling**
   - If prepare fails: print error, return false
   - If step fails: print error (username might already exist!), finalize, return false
   - If success: return true

**Prepared Statement Pattern:**
```
1. Prepare (compile SQL)
2. Bind parameters (supply values)
3. Step (execute)
4. Finalize (cleanup)
```

---

### STEP 7: Implement getUserHash()

**Method Goal:** Find user and return their password hash and salt

**Your Method Should:**

1. **Prepare SELECT SQL**
   - Write: `"SELECT password_hash, password_salt FROM users WHERE username = ?"`
   - This returns TWO columns (hash, salt)

2. **Bind the username parameter**
   - Same pattern as createUser()
   - One parameter: the username to search for

3. **Execute and check for row**
   - `sqlite3_step(stmt)` returns:
     - `SQLITE_ROW` = found a row
     - `SQLITE_DONE` = no rows found
     - Other = error

4. **Extract column values**
   - Use: `sqlite3_column_text(stmt, column_index)`
   - Column indices start at 0
   - Column 0 = password_hash
   - Column 1 = password_salt
   - Returns pointer to text (const unsigned char *)

5. **Convert to std::string**
   - Cast pointer: `reinterpret_cast<const char*>(ptr)`
   - Create string: `std::string(ptr)`
   - Assign to the reference parameters: `hash = ...`, `salt = ...`

6. **Finalize and return**
   - If found and extracted: return true
   - If not found: return false
   - Always finalize the statement

**Return by Reference:**
The hash and salt parameters are references (`std::string &`).
- You ASSIGN values to them (not return them)
- The caller gets the values back through these references
```cpp
std::string h, s;
db.getUserHash("jaferna", h, s);
// Now h contains the hash, s contains the salt
```

---

### STEP 8: Implement userExists()

**Method Goal:** Check if a user already exists (prevent duplicates)

**Your Method Should:**

1. **Prepare COUNT query**
   - Write: `"SELECT COUNT(*) FROM users WHERE username = ?"`
   - COUNT(*) returns the number of matching rows (0 or 1 in your case)

2. **Bind username**
   - Same pattern

3. **Execute**
   - `sqlite3_step()` will return `SQLITE_ROW`

4. **Get the count**
   - Use: `sqlite3_column_int(stmt, 0)`
   - Returns an integer

5. **Determine existence**
   - If count > 0: user exists, return true
   - If count == 0: user doesn't exist, return false

6. **Finalize and return**

**Simple Pattern:**
```
SELECT COUNT(*) returns one integer
If that integer > 0: found
If that integer == 0: not found
```

---

## 📋 Implementation Checklist

### Header File (SQLiteCipherDB.hpp):
- [ ] Add `#include <sqlite3.h>`
- [ ] Add `sqlite3 *db` member
- [ ] Add `const std::string DB_PATH` member
- [ ] Add `void initializeDatabase()` private method declaration

### Constructor (SQLiteCipherDB.cpp):
- [ ] Initialize db to nullptr in initializer list
- [ ] Call `sqlite3_open()`
- [ ] Check return code
- [ ] Handle errors with logging/exception
- [ ] Call `initializeDatabase()`

### Destructor:
- [ ] Check if db is not nullptr
- [ ] Call `sqlite3_close(db)`
- [ ] Set db to nullptr

### initializeDatabase():
- [ ] Write CREATE TABLE IF NOT EXISTS SQL
- [ ] Call `sqlite3_exec()`
- [ ] Check return code
- [ ] Free error message if error
- [ ] Throw exception on error

### createUser():
- [ ] Write INSERT SQL with ? placeholders
- [ ] Prepare statement
- [ ] Bind 3 parameters (username, hash, salt)
- [ ] Step to execute
- [ ] Check for SQLITE_DONE
- [ ] Finalize statement
- [ ] Return success/failure

### getUserHash():
- [ ] Write SELECT SQL with ? placeholder
- [ ] Prepare statement
- [ ] Bind username
- [ ] Step and check for SQLITE_ROW
- [ ] Get column 0 (hash) and column 1 (salt)
- [ ] Convert to strings and assign to references
- [ ] Finalize statement
- [ ] Return success/failure

### userExists():
- [ ] Write SELECT COUNT(*) SQL
- [ ] Prepare statement
- [ ] Bind username
- [ ] Step
- [ ] Get integer count
- [ ] Finalize statement
- [ ] Return (count > 0)

---

## 🎯 Key Concepts to Remember

### 1. **Prepared Statements**
- Always use `?` placeholders
- Prevents SQL injection attacks
- Better performance for repeated queries

### 2. **Return Codes Matter**
```cpp
SQLITE_OK       ✓ Success
SQLITE_DONE     ✓ Operation completed
SQLITE_ROW      ✓ Row available
SQLITE_ERROR    ✗ Generic error
SQLITE_NOTFOUND ✗ Not found
```

### 3. **Resource Management**
- Always finalize statements
- Close database in destructor
- Check for nullptrs

### 4. **Error Messages**
- Use `sqlite3_errmsg(db)` to get human-readable error
- Print them for debugging
- Don't ignore them

### 5. **Column Indexing**
- Prepared statements: parameter indices start at 1
- Result columns: indices start at 0

---

## 🧪 Testing Your Implementation

After you implement, you should be able to:

1. **Create an instance**
   ```cpp
   SQLiteCipherDB db;  // Should open database file
   ```

2. **Create a user**
   ```cpp
   bool success = db.createUser("testuser", "abc123hash", "salt123");
   ```

3. **Check if user exists**
   ```cpp
   bool exists = db.userExists("testuser");  // Should be true
   ```

4. **Get user's hash**
   ```cpp
   std::string hash, salt;
   bool found = db.getUserHash("testuser", hash, salt);
   // hash should equal "abc123hash"
   // salt should equal "salt123"
   ```

5. **Prevent duplicates**
   ```cpp
   bool success2 = db.createUser("testuser", "newhash", "newsalt");
   // Should fail (return false) - username already exists
   ```

---

## 🚀 Next Steps After Implementation

1. Compile without errors
2. Manually test each function
3. Check database file exists at `/tmp/password_manager.db`
4. Verify data was actually stored

---

## 📚 SQLite3 C API Quick Reference

**Opening/Closing:**
- `sqlite3_open(filename, &db)` - Open/create database
- `sqlite3_close(db)` - Close database

**Execution:**
- `sqlite3_exec(db, sql, callback, arg, &errMsg)` - Execute simple SQL
- `sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr)` - Compile SQL
- `sqlite3_step(stmt)` - Execute prepared statement
- `sqlite3_finalize(stmt)` - Cleanup prepared statement

**Binding:**
- `sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_STATIC)` - Bind string

**Results:**
- `sqlite3_column_text(stmt, index)` - Get column as text
- `sqlite3_column_int(stmt, index)` - Get column as integer

**Errors:**
- `sqlite3_errmsg(db)` - Get error message
- `sqlite3_free(ptr)` - Free allocated memory

---

## 💡 Pro Tips

1. **Test one method at a time** - Implement constructor first, test it compiles, then destructor, then helpers, then main methods

2. **Check SQL syntax** - If `sqlite3_prepare_v2()` fails, it's usually an SQL syntax error

3. **Parameter binding order matters** - First `?` is index 1, second is index 2, etc.

4. **Always finalize** - Forgetting to finalize leaks memory

5. **Use prepared statements** - Never use string concatenation for SQL

6. **Column indices vs Parameter indices** - Different starting points!
   - Parameters: 1, 2, 3...
   - Columns: 0, 1, 2...

---

## 🎓 Summary

You now understand:
1. ✅ How SQLite3 works (file-based database)
2. ✅ Why we need sqlite3 pointers and statements
3. ✅ How to design a database class
4. ✅ How to open/close databases safely
5. ✅ How to create tables with CREATE TABLE
6. ✅ How to insert data with prepared statements
7. ✅ How to query data and extract results
8. ✅ How to handle errors properly

Now go implement it! 🚀

