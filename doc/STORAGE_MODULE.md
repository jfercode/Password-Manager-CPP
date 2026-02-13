# Módulo STORAGE - Gestor de Base de Datos SQLite

## Descripción General
El módulo `storage/` proporciona acceso persistente a la base de datos SQLite, responsable del almacenamiento seguro de credenciales de usuarios. Gestiona la creación de tablas, inserción de registros y consultas de verificación. SQLite es una base de datos relacional embebida que permite almacenar datos sin necesidad de un servidor externo, lo que es perfecto para aplicaciones de escritorio.

---

## Archivos Incluidos

### 1. **SQLiteCipherDB.hpp**
Archivo de encabezado que define la clase `SQLiteCipherDB`.

**Ubicación:** `/include/SQLiteCipherDB.hpp`

#### Incluye Dependencias:
```cpp
#include <string>
#include <sqlite3.h>      // SQLite3 C API
#include "library.hpp"
```

#### Clase: `SQLiteCipherDB`
Gestor de base de datos SQLite para persistencia de usuarios.

**Miembros Privados:**
- `sqlite3 *db` - Puntero a la conexión abierta de SQLite
- `std::string dbPath` - Ruta del archivo de base de datos en el filesystem

**Métodos Privados (Funciones Auxiliares):**

1. **`setupDB()`**
   - **Propósito:** Inicializa la estructura de la base de datos creando tablas
   - **Implementación:** Ejecuta sentencia CREATE TABLE IF NOT EXISTS
   - **Tabla Creada:** `users` con columnas para usuario, hash, salt y timestamp

2. **`findDataBasePath()`**
   - **Propósito:** Localiza o crea la ruta de almacenamiento de la base de datos
   - **Retorno:** `bool` - true si es exitoso
   - **Ubicación:** `~/.local/share/passman/passman.db`
   - **Crea directorios:** Si no existen, crea `~/.local/share/passman/`

**Constructor:**
```cpp
SQLiteCipherDB();
```
- Inicializa la conexión a SQLite
- Busca la ruta de la base de datos
- Abre (o crea) el archivo de base de datos
- Llama a `setupDB()` para crear tablas si no existen
- Lanza excepción si algo falla

**Destructor:**
```cpp
~SQLiteCipherDB();
```
- Cierra la conexión a SQLite
- Libera recursos

**Métodos Públicos:**

1. **`createUser(const std::string &username, const std::string &passwordHash, const std::string &salt) const`**
   - **Propósito:** Inserta un nuevo usuario en la base de datos
   - **Parámetros:**
     - `username`: Nombre único del usuario
     - `passwordHash`: Hash de la contraseña (formato hexadecimal)
     - `salt`: Salt usado en el hash (formato hexadecimal)
   - **Retorno:** `bool` - true si se inserta exitosamente
   - **Proceso:**
     1. Prepara sentencia SQL INSERT
     2. Vincula parámetros para evitar SQL injection
     3. Ejecuta la sentencia
     4. Finaliza y limpia el statement

2. **`getUserHash(const std::string &username, std::string &hash, std::string &salt) const`**
   - **Propósito:** Obtiene el hash y salt de un usuario existente
   - **Parámetros:**
     - `username`: Nombre del usuario a buscar
     - `hash`: Referencia a string para recibir el hash
     - `salt`: Referencia a string para recibir el salt
   - **Retorno:** `bool` - true si el usuario existe y se obtienen los datos
   - **Proceso:**
     1. Prepara sentencia SELECT
     2. Vincula el parámetro username
     3. Ejecuta y obtiene la fila si existe
     4. Extrae columnas 0 (hash) y 1 (salt)
     5. Convierte a strings C++

3. **`userExists(const std::string &username) const`**
   - **Propósito:** Verifica si un usuario ya existe en la base de datos
   - **Parámetros:**
     - `username`: Nombre del usuario a verificar
   - **Retorno:** `bool` - true si el usuario existe
   - **Proceso:**
     1. Ejecuta SELECT COUNT(*) WHERE username = ?
     2. Si retorna > 0, el usuario existe
     3. Evita duplicados al registrar

---

### 2. **SQLiteCipherDB.cpp**
Implementación completa del gestor de base de datos.

**Ubicación:** `/src/storage/SQLiteCipherDB.cpp`

#### Implementación Detallada

**Constructor:**
```cpp
SQLiteCipherDB::SQLiteCipherDB() : db(nullptr), dbPath("")
```
- Inicializa miembros en lista de inicialización
- Busca la ruta usando `findDataBasePath()`
- Abre la conexión con `sqlite3_open()`
- Si falla, lanza excepción con `sqlite3_errmsg()`
- Llama a `setupDB()` para crear tablas

**`findDataBasePath()`:**
```cpp
bool SQLiteCipherDB::findDataBasePath(void)
```
- Obtiene la ruta HOME del usuario
- Construye ruta: `$HOME/.local/share/passman/`
- Crea directorios con `createDirectory()` (definida en Filesystem.cpp)
- Asigna `dbPath = appDir + "/passman.db"`
- Retorna true si todo es exitoso

**`setupDB()`:**
```cpp
void SQLiteCipherDB::setupDB(void)
```
SQL ejecutada:
```sql
CREATE TABLE IF NOT EXISTS users(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password_hash TEXT NOT NULL,
    password_salt TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
)
```

**Columnas:**
- `id`: Identificador único auto-incrementado (clave primaria)
- `username`: Nombre único del usuario (UNIQUE previene duplicados)
- `password_hash`: Hash PBKDF2-SHA256 en formato hexadecimal
- `password_salt`: Salt usado en la derivación en formato hexadecimal
- `created_at`: Timestamp de creación automático

**Destructor:**
```cpp
SQLiteCipherDB::~SQLiteCipherDB()
```
- Verifica si `db != nullptr`
- Llama a `sqlite3_close(db)` para cerrar conexión
- Establece `db = nullptr`

**`createUser()`:**
```cpp
bool SQLiteCipherDB::createUser(const std::string &username,
                                const std::string &passwordHash,
                                const std::string &salt) const
```

**SQL:** `INSERT INTO users (username, password_hash, password_salt) VALUES (?, ?, ?);`

**Proceso:**
1. `sqlite3_prepare_v2()` - Compila la sentencia SQL
2. `sqlite3_bind_text()` - Vincula parámetros:
   - Posición 1: username
   - Posición 2: passwordHash
   - Posición 3: salt
   - `SQLITE_STATIC` indica que las cadenas persisten durante la ejecución
3. `sqlite3_step()` - Ejecuta la sentencia
4. Verifica si retorna `SQLITE_DONE` (éxito)
5. `sqlite3_finalize()` - Limpia el statement

**Manejo de Errores:**
- Si falla, retorna false y registra log en rojo
- UNIQUE constraint viola si username ya existe

**`getUserHash()`:**
```cpp
bool SQLiteCipherDB::getUserHash(const std::string &username,
                                  std::string &hash,
                                  std::string &salt) const
```

**SQL:** `SELECT password_hash, password_salt FROM users WHERE username = ?`

**Proceso:**
1. Prepara sentencia SELECT
2. Vincula username
3. Ejecuta y espera `SQLITE_ROW` (fila encontrada)
4. Si no encuentra fila, retorna false
5. Extrae columnas con `sqlite3_column_text()`:
   - Columna 0: password_hash
   - Columna 1: password_salt
6. Convierte a `std::string` usando `reinterpret_cast`
7. Finaliza y retorna true

**`userExists()`:**
```cpp
bool SQLiteCipherDB::userExists(const std::string &username) const
```

**SQL:** `SELECT COUNT(*) FROM users WHERE username = ?`

**Proceso:**
1. Prepara sentencia COUNT
2. Vincula username
3. Ejecuta y obtiene la fila
4. Extrae el COUNT como entero con `sqlite3_column_int()`
5. Finaliza statement
6. Retorna `count > 0`

---

## Librería SQLite3 (https://www.sqlite.org/)

SQLite es una base de datos SQL relacional de código abierto, embebida, que requiere una configuración mínima.

### Características:
- **Sin Servidor:** Acceso directo a archivo, no requiere demonio
- **Portátil:** Base de datos única en archivo (passman.db)
- **Segura:** Soporta transacciones ACID
- **Rápida:** Compilada en lenguaje C para máximo rendimiento

### Funciones C API Utilizadas:

#### 1. **Conexión y Gestión**
- **`sqlite3_open(const char *filename, sqlite3 **ppDb)`**
  - Abre conexión a archivo SQLite
  - Crea archivo si no existe
  - Retorna `SQLITE_OK` si es exitoso
  - `ppDb` es doble puntero para retornar la conexión

- **`sqlite3_close(sqlite3 *db)`**
  - Cierra conexión y libera recursos
  - Debe llamarse antes de terminar programa

- **`sqlite3_errmsg(sqlite3 *db)`**
  - Retorna mensaje de error en formato C string
  - Útil para debugging

#### 2. **Ejecución de SQL**
- **`sqlite3_exec(sqlite3 *db, const char *sql, sqlite3_callback callback, void *arg, char **errmsg)`**
  - Ejecuta sentencia SQL directa
  - Usado para CREATE TABLE en setupDB()
  - Si callback es NULL, solo ejecuta sin procesar resultados
  - Retorna `SQLITE_OK` si es exitoso

#### 3. **Prepared Statements (Consultas Parametrizadas)**
- **`sqlite3_prepare_v2(sqlite3 *db, const char *sql, int nbyte, sqlite3_stmt **ppStmt, const char **pzTail)`**
  - Compila sentencia SQL en bytecode
  - `nbyte = -1` significa leer hasta null terminator
  - Retorna `SQLITE_OK` si es exitoso
  - Retorna `sqlite3_stmt*` para ejecutar después

- **`sqlite3_bind_text(sqlite3_stmt *pStmt, int index, const char *value, int n, void(*destructor)(void*))`**
  - Vincula parámetro string a placeholder (?)
  - `index` comienza en 1 (no 0)
  - `n = -1` calcula longitud automáticamente
  - `SQLITE_STATIC` indica que cadena persiste

- **`sqlite3_step(sqlite3_stmt *pStmt)`**
  - Ejecuta statement preparado
  - Retorna `SQLITE_ROW` si hay fila siguiente
  - Retorna `SQLITE_DONE` si no hay más filas
  - Retorna `SQLITE_ERROR` si falla

- **`sqlite3_column_text(sqlite3_stmt *pStmt, int iCol)`**
  - Obtiene valor TEXT de columna
  - `iCol` comienza en 0
  - Retorna `const unsigned char*` (C string)

- **`sqlite3_column_int(sqlite3_stmt *pStmt, int iCol)`**
  - Obtiene valor INTEGER de columna

- **`sqlite3_finalize(sqlite3_stmt *pStmt)`**
  - Libera resources del statement
  - Debe llamarse después de terminar con statement
  - Retorna código de error final si existe

#### 4. **Códigos de Retorno Principales**
```
SQLITE_OK        = 0   // Éxito
SQLITE_ERROR     = 1   // Error SQL genérico
SQLITE_ROW       = 100 // Hay una fila disponible
SQLITE_DONE      = 101 // Ejecución completada
SQLITE_BUSY      = 5   // Base de datos bloqueada
```

---

## Esquema de Base de Datos

### Tabla: `users`

```
+----+----------+------------------------------------------+----------------------------------+---------------------+
| id | username | password_hash                            | password_salt                    | created_at          |
+----+----------+------------------------------------------+----------------------------------+---------------------+
| 1  | john     | a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6q... | x1y2z3w4v5u6t7s8r9q0p1o2n3m4... | 2024-02-13 10:30:45 |
| 2  | jane     | q0p1o2n3m4l5k6j7i8h9g0f1e2d3c4b5... | s8r9q0p1o2n3m4l5k6j7i8h9g0f1... | 2024-02-13 10:32:12 |
+----+----------+------------------------------------------+----------------------------------+---------------------+
```

**Restricciones:**
- `id`: PRIMARY KEY, AUTOINCREMENT
- `username`: UNIQUE (previene duplicados), NOT NULL
- `password_hash`: NOT NULL
- `password_salt`: NOT NULL
- `created_at`: Timestamp automático

---

## Flujo de Operaciones de Base de Datos

### Creación de Usuario:
```
AuthenticationManager::registerNewUser()
    ↓
SQLiteCipherDB::userExists(username)
    ↓ (false - usuario no existe)
SQLiteCipherDB::createUser(username, hash, salt)
    ├─ sqlite3_prepare_v2() - Compila INSERT
    ├─ sqlite3_bind_text() - Vincula parámetros
    ├─ sqlite3_step() - Ejecuta inserción
    ├─ sqlite3_finalize() - Limpia
    ↓
Usuario almacenado en tabla users
```

### Autenticación de Usuario:
```
AuthenticationManager::authenticateUser()
    ↓
SQLiteCipherDB::getUserHash(username, hash, salt)
    ├─ sqlite3_prepare_v2() - Compila SELECT
    ├─ sqlite3_bind_text() - Vincula username
    ├─ sqlite3_step() - Ejecuta consulta
    ├─ sqlite3_column_text() - Extrae hash y salt
    ├─ sqlite3_finalize() - Limpia
    ↓
Hash y salt obtenidos
    ↓
CryptoManager::verifyPassword(password, hash, salt)
```

---

## Ubicación de Almacenamiento

La base de datos se almacena en:
```
~/.local/share/passman/passman.db
```

**Ventajas de esta ubicación:**
- Sigue estándar XDG de Linux
- Hidden directory (comienza con .)
- Ruta relativa a HOME (portable)
- `.local/share` es para datos de aplicación
- Automáticamente excluido de backups del escritorio en algunos sistemas

**Directorios creados automáticamente:**
```
~/.local/              - Datos locales del usuario
~/.local/share/        - Datos de aplicaciones
~/.local/share/passman/ - Datos de Password Manager
```

---

## Consideraciones de Seguridad

1. **Prepared Statements:** Usa parametrización para evitar SQL injection
2. **UNIQUE Constraint:** Previene duplicados de usuarios
3. **Hash Almacenado:** No almacena contraseñas en texto plano
4. **Ubicación Segura:** Datos en directorio privado del usuario
5. **Errores Detallados:** Registra errores para debugging
6. **Transacciones ACID:** SQLite garantiza integridad de datos

---

## Patrones de Código

### Pattern: Prepared Statements
```cpp
sqlite3_stmt *stmt = nullptr;
sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_STATIC);
int result = sqlite3_step(stmt);
// ... procesar resultado
sqlite3_finalize(stmt);
```

Este patrón previene SQL injection y es más eficiente que ejecutar SQL directo.

---

## Notas Técnicas

- **Const Correctness:** Los métodos que leen datos son `const`
- **Resource Management:** Todos los statements se finalizan correctamente
- **Memory Safety:** No hay memory leaks, SQLite maneja buffers internos
- **Thread Safety:** SQLite es single-threaded por diseño (cada thread su conexión)
- **Forward Compatibility:** La API C de SQLite es muy estable

---

## Limitaciones Conocidas

1. **Single Thread:** SQLite está optimizado para single-threaded access
2. **Tamaño Archivo:** Aunque es poco probable, hay límite teórico
3. **Concurrencia:** Si múltiples procesos acceden simultáneamente, puede haber contención
4. **Sin Cifrado Integrado:** Los datos están almacenados en texto plano (SQLCipher es extensión)

---

## Extensiones Futuras

- **SQLCipher:** Wrapper que añade cifrado AES-256 a SQLite
- **Migrations:** Sistema de versiones de esquema para actualizaciones
- **Índices:** Añadir índices en username para búsquedas más rápidas
- **Auditoría:** Registrar intentos de login fallidos
