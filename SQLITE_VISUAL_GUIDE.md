# 🎨 SQLite Implementation - Visual Guide & Flowcharts

## 📊 Flow Diagrams

### Diagram 1: Program Flow - User Registration

```
User clicks "Register" in UI
           ↓
LoginDialog gets username + password
           ↓
AuthenticationManager::registerNewUser()
           ↓
        ┌──────────────────────────┐
        │ Check: Does user exist?  │
        └──────────────────────────┘
                 ↓        ↓
            YES (fail)   NO (continue)
             ↓              ↓
          Return          Hash password with PBKDF2
          False              ↓
             ↓           Get: hash + salt
             ✗               ↓
                    db->createUser(user, hash, salt)
                            ↓
                    ┌─────────────────┐
                    │ Insert into DB  │
                    │ users table     │
                    └─────────────────┘
                      ↓            ↓
                   Success      Failure
                     ↓            ↓
                   TRUE        FALSE
                     ✓            ✗
```

### Diagram 2: Program Flow - User Login

```
User clicks "Login"
        ↓
LoginDialog gets username + password
        ↓
AuthenticationManager::authenticateUser()
        ↓
    ┌─────────────────────────────┐
    │ Find user in database       │
    │ db->getUserHash(user,...)   │
    └─────────────────────────────┘
         ↓               ↓
    Found          Not Found
      ↓               ↓
  Get hash         Return FALSE
  Get salt            ↓
      ↓              ✗ (Login fails)
Hash provided password with stored salt
      ↓
Compare hashes
      ↓         ↓
   Match    No Match
     ↓         ↓
   TRUE     FALSE
     ✓         ✗
```

---

## 🗄️ Database Schema Visualization

### Your "users" Table Structure

```
┌─────────────────────────────────────────────────────────┐
│                    users TABLE                          │
├────┬──────────┬─────────────┬──────────────┬──────────┤
│ id │ username │ password_hash│ password_salt│created_at│
├────┼──────────┼─────────────┼──────────────┼──────────┤
│ 1  │ jaferna  │ a1b2c3d4... │ xyz789...    │ 2026-02-11│
├────┼──────────┼─────────────┼──────────────┼──────────┤
│ 2  │ admin    │ e5f6g7h8... │ abc123...    │ 2026-02-11│
├────┼──────────┼─────────────┼──────────────┼──────────┤
│    │          │             │              │          │
└────┴──────────┴─────────────┴──────────────┴──────────┘

Each row = One user
Each column = One piece of information
```

### Why This Schema?

```
id:
   └─ Auto-increment, unique identifier
      Purpose: Database indexing, relationships in future

username:
   └─ UNIQUE NOT NULL
      Purpose: No duplicate users, login identifier

password_hash:
   └─ NOT NULL
      Purpose: Compare passwords without storing plaintext

password_salt:
   └─ NOT NULL
      Purpose: Randomness for hashing

created_at:
   └─ DEFAULT CURRENT_TIMESTAMP
      Purpose: Audit trail (when was account created?)
```

---

## 🔄 SQLite3 C API Call Sequence

### Opening Database

```
Program Start
    ↓
sqlite3 *db = nullptr;
    ↓
sqlite3_open("/tmp/password_manager.db", &db)
    ↓
    ┌─────────────────────────────┐
    │ Does /tmp/password_manager │
    │      .db file exist?        │
    └─────────────────────────────┘
       ↓                    ↓
     YES                  NO
      ↓                    ↓
   Open it         Create new empty DB
      ↓                    ↓
   Set db pointer to file handle
      ↓
  Now db points to actual database
      ↓
  Ready to use!
```

### Creating Table

```
Constructor called
    ↓
initializeDatabase()
    ↓
Write SQL: "CREATE TABLE IF NOT EXISTS users (...)"
    ↓
sqlite3_exec(db, sql, NULL, NULL, &errMsg)
    ↓
    ┌──────────────────────────────┐
    │ Does 'users' table exist?    │
    └──────────────────────────────┘
       ↓                    ↓
     YES                  NO
      ↓                    ↓
   Do nothing         Create table
      ↓                    ↓
   Both result in table being ready
      ↓
  Continue...
```

### Inserting User (createUser Flow)

```
createUser("jaferna", "hash123", "salt456")
    ↓
Write SQL with placeholders: "INSERT INTO users (...) VALUES (?, ?, ?)"
    ↓
sqlite3_prepare_v2() → stmt pointer
    ↓
sqlite3_bind_text(stmt, 1, "jaferna", ...)      ← Username
sqlite3_bind_text(stmt, 2, "hash123", ...)      ← Hash
sqlite3_bind_text(stmt, 3, "salt456", ...)      ← Salt
    ↓
sqlite3_step(stmt)
    ↓
    ┌─────────────────────────┐
    │ Insert successful?      │
    └─────────────────────────┘
       ↓            ↓
    YES (SQLITE_DONE)    NO (error)
      ↓            ↓
    Continue    Handle error
      ↓            ↓
sqlite3_finalize(stmt)
    ↓
Return true/false
```

### Querying User (getUserHash Flow)

```
getUserHash("jaferna", hash, salt)
    ↓
Write SQL: "SELECT password_hash, password_salt FROM users WHERE username = ?"
    ↓
sqlite3_prepare_v2() → stmt pointer
    ↓
sqlite3_bind_text(stmt, 1, "jaferna", ...)
    ↓
sqlite3_step(stmt)
    ↓
    ┌─────────────────────────────────┐
    │ Did we find a matching row?     │
    └─────────────────────────────────┘
       ↓                        ↓
    SQLITE_ROW              SQLITE_DONE or error
      ↓                        ↓
  Extract data            User not found
      ↓                        ↓
sqlite3_column_text(stmt, 0)  sqlite3_finalize(stmt)
     → password_hash          return false
sqlite3_column_text(stmt, 1)
     → password_salt
      ↓
  Copy to reference parameters
      ↓
sqlite3_finalize(stmt)
    ↓
Return true
```

---

## 📝 SQL Command Reference for Your Use Case

### CREATE TABLE

```sql
CREATE TABLE IF NOT EXISTS users (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  username TEXT UNIQUE NOT NULL,
  password_hash TEXT NOT NULL,
  password_salt TEXT NOT NULL,
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

**What each part means:**
- `CREATE TABLE IF NOT EXISTS` = Create if not already there
- `id INTEGER PRIMARY KEY AUTOINCREMENT` = Auto-increment ID (1, 2, 3...)
- `username TEXT UNIQUE NOT NULL` = Text, must be unique, can't be empty
- `password_hash TEXT NOT NULL` = Text, can't be empty
- `password_salt TEXT NOT NULL` = Text, can't be empty
- `created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP` = Auto-fill with current time

### INSERT (used in createUser)

```sql
INSERT INTO users (username, password_hash, password_salt) 
VALUES (?, ?, ?);
```

**Parameters:**
1. ? → username (e.g., "jaferna")
2. ? → password_hash (e.g., "a1b2c3d4e5f6...")
3. ? → password_salt (e.g., "xyz789...")

**Result:** New row added to users table

### SELECT (used in getUserHash)

```sql
SELECT password_hash, password_salt 
FROM users 
WHERE username = ?;
```

**Parameters:**
1. ? → username to search for

**Result:** Returns 0 or 1 rows with 2 columns

### SELECT COUNT (used in userExists)

```sql
SELECT COUNT(*) 
FROM users 
WHERE username = ?;
```

**Parameters:**
1. ? → username to search for

**Result:** Returns 1 row with 1 column (integer count)

---

## 🔍 Data Flow - Concrete Example

### Scenario: Register new user "alice" with password "secret123"

```
Step 1: Generate Hash
        ┌─────────────────────────┐
        │ PBKDF2-SHA256           │
        │ Input: "secret123"      │
        │ Generate: 16-byte salt  │
        │ 10,000 iterations       │
        │ Output: 32-byte hash    │
        └─────────────────────────┘
        │
        hash = "a1b2c3d4e5f6g7h8..." (64 hex chars)
        salt = "xyz123abc456def..." (32 hex chars)
                        ↓

Step 2: Insert into Database
        ┌──────────────────────────────┐
        │ INSERT INTO users            │
        │ VALUES                       │
        │ username="alice"             │
        │ password_hash="a1b2c3d4..."  │
        │ password_salt="xyz123abc..." │
        └──────────────────────────────┘
                        ↓

Step 3: Database File Updated
        /tmp/password_manager.db (binary file)
                        ↓
        File now contains:
        ┌─────────────────────────────────────────┐
        │ alice | a1b2c3d4... | xyz123abc... | ts │
        └─────────────────────────────────────────┘
                        ↓

Later - User Logs In with "secret123"
        ┌──────────────────────────────┐
        │ 1. Query DB for "alice"      │
        │ 2. Get salt "xyz123abc..."   │
        │ 3. Get stored hash "a1b2..." │
        └──────────────────────────────┘
                        ↓
        ┌──────────────────────────────┐
        │ Hash provided password       │
        │ Input: "secret123"           │
        │ With salt: "xyz123abc..."    │
        │ Output: "a1b2c3d4..." (same!)│
        └──────────────────────────────┘
                        ↓
        ┌──────────────────────────────┐
        │ Compare:                     │
        │ Computed: "a1b2c3d4..."      │
        │ Stored:   "a1b2c3d4..."      │
        │ Match! ✓                     │
        └──────────────────────────────┘
                        ↓
        Login successful!
```

---

## 🎯 Implementation Order - Visual

```
┌──────────────────────────────────────────────┐
│ 1. Header File (.hpp)                        │
│    ├─ Add includes                           │
│    ├─ Add private members                    │
│    ├─ Add private helper method              │
│    └─ Declare public methods                 │
└──────────────────────────────────────────────┘
                        ↓
┌──────────────────────────────────────────────┐
│ 2. Implementation File (.cpp)                │
│    ├─ Constructor                            │
│    │  └─ Open DB, call initializeDatabase   │
│    ├─ Destructor                             │
│    │  └─ Close DB                            │
│    ├─ initializeDatabase() helper            │
│    │  └─ Create table                        │
│    ├─ createUser()                           │
│    │  └─ INSERT query                        │
│    ├─ getUserHash()                          │
│    │  └─ SELECT query                        │
│    └─ userExists()                           │
│       └─ SELECT COUNT query                  │
└──────────────────────────────────────────────┘
                        ↓
┌──────────────────────────────────────────────┐
│ 3. Compile & Test                            │
│    ├─ Should compile without errors          │
│    ├─ Database file should be created        │
│    └─ Test each method manually              │
└──────────────────────────────────────────────┘
```

---

## 🧩 Code Structure Template

### Header File Pattern

```
#ifndef CLASSNAME_HPP
#define CLASSNAME_HPP

#include <necessary-headers>

class MyClass {
private:
    // Member variables
    SomeType member1;
    
    // Helper methods
    void helperMethod();
    
public:
    // Constructor/Destructor
    MyClass();
    ~MyClass();
    
    // Public methods
    bool publicMethod(parameters);
};

#endif
```

### Implementation Pattern

```
#include "ClassName.hpp"

// Constructor
ClassName::ClassName() : member1(initial_value) {
    // Initialize
    // Call helpers
}

// Destructor
ClassName::~ClassName() {
    // Cleanup
}

// Helper
void ClassName::helperMethod() {
    // Setup
}

// Public methods
bool ClassName::publicMethod(parameters) {
    // Prepare
    // Execute
    // Check
    // Cleanup
    // Return
}
```

---

## 🐛 Common Mistakes to Avoid

```
❌ MISTAKE 1: Forget to finalize
   sqlite3_prepare_v2(...);
   sqlite3_bind_text(...);
   sqlite3_step(...);
   return result;  // ← MEMORY LEAK! Forgot finalize

✅ CORRECT:
   sqlite3_prepare_v2(...);
   sqlite3_bind_text(...);
   sqlite3_step(...);
   sqlite3_finalize(stmt);  // ← Always do this
   return result;


❌ MISTAKE 2: Wrong parameter indices
   sqlite3_bind_text(stmt, 0, ...);  // ← Indices start at 1!
   
✅ CORRECT:
   sqlite3_bind_text(stmt, 1, ...);  // ← Index 1, 2, 3...


❌ MISTAKE 3: Wrong column indices
   int id = sqlite3_column_int(stmt, 1);  // ← Columns start at 0
   
✅ CORRECT:
   int id = sqlite3_column_int(stmt, 0);  // ← Index 0, 1, 2...


❌ MISTAKE 4: Ignore return codes
   sqlite3_open(path, &db);
   // What if it failed?
   
✅ CORRECT:
   int rc = sqlite3_open(path, &db);
   if (rc != SQLITE_OK) {
       // Handle error
   }


❌ MISTAKE 5: Don't check for nullptr
   sqlite3_close(db);  // What if db is nullptr?
   
✅ CORRECT:
   if (db != nullptr) {
       sqlite3_close(db);
   }
```

---

## 📊 Memory Management

```
Constructor:
  db = nullptr
          ↓
  sqlite3_open() 
          ↓
  db points to open database
          ↓
  Use database throughout program life
          ↓
  Destructor:
    sqlite3_close(db)
          ↓
    db = nullptr


Prepared Statement:
  sqlite3_prepare_v2()
          ↓
  stmt points to compiled statement
          ↓
  Bind and execute
          ↓
  Extract results
          ↓
  sqlite3_finalize(stmt)
          ↓
  stmt is freed
```

---

## 🎓 Checklist for Understanding

Before implementing, you should understand:

- [ ] What SQLite is (file-based database engine)
- [ ] How sqlite3 *db works (pointer to database handle)
- [ ] How prepared statements prevent SQL injection
- [ ] Why we use ? placeholders
- [ ] Parameter binding (index 1, 2, 3...)
- [ ] Column extraction (index 0, 1, 2...)
- [ ] Return codes (SQLITE_OK, SQLITE_DONE, SQLITE_ROW)
- [ ] Why we must finalize statements
- [ ] Why we must close database in destructor
- [ ] The difference between CREATE TABLE IF NOT EXISTS
- [ ] How PRIMARY KEY AUTOINCREMENT works
- [ ] Why UNIQUE constraints matter
- [ ] Reference parameters (&) vs return values

---

This visual guide should help you understand the big picture before diving into code! 🚀

