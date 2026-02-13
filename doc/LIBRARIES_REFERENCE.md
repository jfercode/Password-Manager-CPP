# Librerías Utilizadas - Referencia Rápida

## 📚 Índice de Librerías

### Criptografía
- [OpenSSL](#openssl)

### Base de Datos
- [SQLite3](#sqlite3)

### Interfaz Gráfica
- [Qt Framework](#qt-framework)

### Estándar de C++
- [STL (Standard Template Library)](#stl)

### Sistema Operativo
- [POSIX](#posix)

---

## Criptografía

### OpenSSL
**Versión:** 1.1.x (Compatible con 3.x parcialmente)  
**Licencia:** Apache 2.0  
**Página Web:** https://www.openssl.org/

#### Módulos Utilizados

##### `<openssl/rand.h>` - Generación de Números Aleatorios

**Función Principal:**
```cpp
int RAND_bytes(unsigned char *buf, int num);
```

| Parámetro | Descripción |
|-----------|-------------|
| `buf` | Buffer donde almacenar bytes aleatorios |
| `num` | Cantidad de bytes a generar |
| **Retorno** | 1 si exitoso, 0 si falla |

**Fuente de Aleatoriedad:**
- Unix/Linux: `/dev/urandom`
- Windows: CryptGenRandom (API Windows)
- Criptográficamente seguro

**Uso en Proyecto:**
```cpp
// En CryptoManager::generateRandomBytes()
std::vector<unsigned char> buffer(length);
if (RAND_bytes(buffer.data(), length) != 1)
    throw std::runtime_error("RAND_bytes failed");
```

**Ejemplo Práctico:**
```cpp
// Generar 16 bytes para salt
unsigned char salt[16];
RAND_bytes(salt, 16);
// salt = [0xA3, 0x7F, 0x2E, 0xB1, ...] (aleatorio)
```

---

##### `<openssl/evp.h>` - EVP (Envelope) Functions

**Función Principal para Derivación de Clave:**
```cpp
int PKCS5_PBKDF2_HMAC(
    const char *password,
    int pass_len,
    const unsigned char *salt,
    int salt_len,
    int iter,
    const EVP_MD *md,
    int keylen,
    unsigned char *out
);
```

| Parámetro | Descripción |
|-----------|-------------|
| `password` | Contraseña en texto plano |
| `pass_len` | Longitud de la contraseña (-1 para null-terminated) |
| `salt` | Salt (mínimo 8 bytes, recomendado 16) |
| `salt_len` | Longitud del salt |
| `iter` | Número de iteraciones (10000 recomendado) |
| `md` | Algoritmo de hash (EVP_sha256()) |
| `keylen` | Longitud de salida (32 bytes = 256 bits) |
| `out` | Buffer de salida |
| **Retorno** | 1 si exitoso |

**Función Auxiliar:**
```cpp
const EVP_MD *EVP_sha256(void);
```
Retorna el descriptor MD (Message Digest) para SHA256.

**Algoritmo PBKDF2-SHA256:**
- **P**assword **B**ased **K**ey **D**erivation **F**unction 2
- Itera HMAC-SHA256 N veces
- Cada iteración: HMAC_SHA256(key, input) donde key es resultado anterior
- Resistente a ataques de fuerza bruta
- Conforme a RFC 8018 (PKCS #5)

**Uso en Proyecto:**
```cpp
// En CryptoManager::hashPassword()
unsigned char hash[32];
PKCS5_PBKDF2_HMAC(
    password.c_str(),
    password.length(),
    salt_bytes.data(),
    salt_bytes.size(),
    10000,              // Iteraciones
    EVP_sha256(),       // Algoritmo
    32,                 // Salida de 256 bits
    hash
);
```

**Cálculo de Tiempo:**
- Iteraciones: 10000
- Tiempo por iteración: ~0.1ms
- Tiempo total: ~1 segundo por operación
- Previene ataques de diccionario

---

##### `<openssl/err.h>` - Manejo de Errores

**Función Principal:**
```cpp
unsigned long ERR_get_error(void);
char *ERR_error_string(unsigned long e, char *buf);
```

**Uso en Proyecto:**
- Implícitamente utilizado por otras funciones OpenSSL
- Proporciona mensajes de error detallados
- Usado en manejo de excepciones

**Ejemplo:**
```cpp
if (RAND_bytes(buffer, length) != 1) {
    unsigned long err = ERR_get_error();
    char err_str[120];
    ERR_error_string(err, err_str);
    throw std::runtime_error(err_str);
}
```

---

## Base de Datos

### SQLite3
**Versión:** 3.x (Última disponible)  
**Licencia:** Public Domain  
**Página Web:** https://www.sqlite.org/

#### Características
- Base de datos SQL relacional embebida
- Archivo único (passman.db)
- Sin servidor (serverless)
- ACID compliant (Atomicity, Consistency, Isolation, Durability)
- Transacciones

#### Funciones Principales Utilizadas

##### Inicialización y Conexión

**`sqlite3_open()` / `sqlite3_open_v2()`**
```cpp
int sqlite3_open(const char *filename, sqlite3 **ppDb);
```

| Parámetro | Descripción |
|-----------|-------------|
| `filename` | Ruta del archivo BD (crea si no existe) |
| `ppDb` | Doble puntero para recibir conexión |
| **Retorno** | SQLITE_OK (0) si exitoso |

**Códigos de Retorno:**
```
SQLITE_OK       = 0   ✓ Éxito
SQLITE_ERROR    = 1   ✗ Error SQL genérico
SQLITE_BUSY     = 5   ⚠ BD bloqueada
SQLITE_READONLY = 8   ⚠ BD solo lectura
```

**Uso en Proyecto:**
```cpp
// En SQLiteCipherDB::SQLiteCipherDB()
sqlite3 *db;
int dbRes = sqlite3_open(dbPath.c_str(), &db);
if (dbRes != SQLITE_OK)
    throw std::runtime_error("Error opening DB");
```

---

##### Ejecución SQL Directo

**`sqlite3_exec()`**
```cpp
int sqlite3_exec(
    sqlite3 *db,
    const char *sql,
    sqlite3_callback callback,
    void *arg,
    char **errmsg
);
```

| Parámetro | Descripción |
|-----------|-------------|
| `db` | Conexión abierta |
| `sql` | Sentencia SQL a ejecutar |
| `callback` | Función para procesar filas (NULL para ignorar) |
| `arg` | Argumento para callback |
| `errmsg` | Puntero para mensaje de error |
| **Retorno** | SQLITE_OK si exitoso |

**Uso en Proyecto:**
```cpp
// En SQLiteCipherDB::setupDB()
const char *sql = "CREATE TABLE IF NOT EXISTS users(...)";
char *errMsg = nullptr;
int rSql = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
if (rSql != SQLITE_OK) {
    sqlite3_free(errMsg);
    throw std::runtime_error("Failed to create table");
}
```

---

##### Prepared Statements (Consultas Parametrizadas)

**`sqlite3_prepare_v2()`**
```cpp
int sqlite3_prepare_v2(
    sqlite3 *db,
    const char *sql,
    int nbyte,
    sqlite3_stmt **ppStmt,
    const char **pzTail
);
```

| Parámetro | Descripción |
|-----------|-------------|
| `db` | Conexión abierta |
| `sql` | Sentencia SQL con placeholders (?) |
| `nbyte` | Bytes a leer (-1 para null-terminated) |
| `ppStmt` | Recibe el statement compilado |
| `pzTail` | Recibe inicio de siguiente sentencia |
| **Retorno** | SQLITE_OK si compiló exitosamente |

**Ejemplo:**
```cpp
const char *sql = "SELECT password_hash FROM users WHERE username = ?";
sqlite3_stmt *stmt;
sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
// stmt es statement compilado listo para ejecutar
```

**Ventajas de Prepared Statements:**
- ✅ Previene SQL injection
- ✅ Reutilizable para múltiples consultas
- ✅ Más eficiente
- ✅ Separación datos-código

---

**`sqlite3_bind_text()`**
```cpp
int sqlite3_bind_text(
    sqlite3_stmt *pStmt,
    int index,
    const char *value,
    int n,
    void(*destructor)(void*)
);
```

| Parámetro | Descripción |
|-----------|-------------|
| `pStmt` | Statement preparado |
| `index` | Posición del placeholder (comienza en 1, no 0) |
| `value` | Valor a vincular (C string) |
| `n` | Longitud (-1 para null-terminated) |
| `destructor` | SQLITE_STATIC (no libera) o SQLITE_TRANSIENT |
| **Retorno** | SQLITE_OK si exitoso |

**Uso en Proyecto:**
```cpp
// Vinculación de parámetros
sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 2, password_hash.c_str(), -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 3, salt.c_str(), -1, SQLITE_STATIC);
```

---

**`sqlite3_step()`**
```cpp
int sqlite3_step(sqlite3_stmt *pStmt);
```

| Código | Significado |
|--------|-------------|
| SQLITE_ROW (100) | Hay una fila disponible |
| SQLITE_DONE (101) | Ejecución completada |
| SQLITE_ERROR | Error en ejecución |
| SQLITE_BUSY | BD bloqueada |

**Uso en Proyecto:**
```cpp
// Inserción
int result = sqlite3_step(stmt);
if (result == SQLITE_DONE) {
    // Inserción exitosa
}

// Consulta (seleccionar)
while (sqlite3_step(stmt) == SQLITE_ROW) {
    // Procesar fila
}
```

---

**`sqlite3_column_text()` / `sqlite3_column_int()`**
```cpp
const unsigned char *sqlite3_column_text(sqlite3_stmt *pStmt, int iCol);
int sqlite3_column_int(sqlite3_stmt *pStmt, int iCol);
```

| Parámetro | Descripción |
|-----------|-------------|
| `pStmt` | Statement |
| `iCol` | Índice de columna (comienza en 0) |
| **Retorno** | Valor de la columna |

**Uso en Proyecto:**
```cpp
// Extraer datos de columna
const unsigned char *hash_ptr = sqlite3_column_text(stmt, 0);
const unsigned char *salt_ptr = sqlite3_column_text(stmt, 1);
hash = std::string(reinterpret_cast<const char *>(hash_ptr));
salt = std::string(reinterpret_cast<const char *>(salt_ptr));
```

---

**`sqlite3_finalize()`**
```cpp
int sqlite3_finalize(sqlite3_stmt *pStmt);
```

**Propósito:** Libera resources del statement

**Uso en Proyecto:**
```cpp
// Siempre llamar al final
sqlite3_finalize(stmt);

// Buena práctica RAII
try {
    // ... usar statement
} catch (...) {
    // Finalizar incluso si hay error
    sqlite3_finalize(stmt);
    throw;
}
```

---

**`sqlite3_close()`**
```cpp
int sqlite3_close(sqlite3 *db);
```

**Propósito:** Cierra conexión y libera recursos

**Uso en Proyecto:**
```cpp
// En destructor de SQLiteCipherDB
if (db != nullptr)
    sqlite3_close(db);
db = nullptr;
```

---

#### Esquema de Base de Datos

```sql
CREATE TABLE IF NOT EXISTS users(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password_hash TEXT NOT NULL,
    password_salt TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

**Restricciones:**
- `PRIMARY KEY AUTOINCREMENT` - ID único, autoincrementado
- `UNIQUE NOT NULL` - Username único en toda la tabla
- `DEFAULT CURRENT_TIMESTAMP` - Fecha automática de creación

---

## Interfaz Gráfica

### Qt Framework
**Versión:** Qt 6.x  
**Licencia:** LGPL 3.0 / Comercial  
**Página Web:** https://www.qt.io/

#### Módulos Utilizados

##### QtCore
**Responsabilidad:** Funcionalidad fundamental

**Clases Clave:**

| Clase | Propósito |
|-------|-----------|
| `QObject` | Clase base para todos los objetos Qt con system de señales/slots |
| `QString` | Cadena Unicode para Qt |
| `QCoreApplication` | Manejo de aplicación sin UI |
| `QVariant` | Tipo de datos que puede contener varios tipos |

**Sistema de Señales y Slots:**
```cpp
// Conectar señal a slot
connect(button, &QPushButton::clicked, 
        this, &MainWindow::onButtonClicked);

// Cuando button emite clicked(), onButtonClicked() se ejecuta
```

---

##### QtGui
**Responsabilidad:** Componentes gráficos

**Clases Clave:**

| Clase | Propósito |
|-------|-----------|
| `QGuiApplication` | Aplicación con soporte gráfico |
| `QIcon` | Icono para ventanas/botones |
| `QFont` | Propiedades de fuente |
| `QColor` | Colores |
| `QPalette` | Esquema de colores |

**Uso en Proyecto:**
```cpp
// Cambiar tamaño de fuente
QFont titleFont = tittleLabel->font();
titleFont.setPointSize(16);
titleFont.setBold(true);
tittleLabel->setFont(titleFont);
```

---

##### QtWidgets
**Responsabilidad:** Widgets de alto nivel

**Clases Principales Utilizadas:**

###### **QApplication**
```cpp
class QApplication : public QGuiApplication
```

**Propósito:** Aplicación Qt con soporte de widgets

**Uso en Proyecto:**
```cpp
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // ... crear widgets
    return app.exec();  // Inicia event loop
}
```

---

###### **QMainWindow**
```cpp
class QMainWindow : public QWidget
```

**Propósito:** Ventana principal con menú, barras de herramientas

**Métodos Clave:**
- `setCentralWidget()` - Establece widget central
- `setWindowTitle()` - Título de ventana
- `resize()` - Tamaño de ventana
- `setMinimumSize()` - Tamaño mínimo

**Uso en Proyecto:**
```cpp
class MainWindow : public QMainWindow {
    MainWindow() {
        setWindowTitle("Password Manager - Secure Storage");
        setMinimumSize(600, 300);
        show();
    }
};
```

---

###### **QDialog**
```cpp
class QDialog : public QWidget
```

**Propósito:** Ventana modal para interacción del usuario

**Métodos Clave:**
- `exec()` - Muestra diálogo modal y espera resultado
- `accept()` - Cierra con código QDialog::Accepted (1)
- `reject()` - Cierra con código QDialog::Rejected (0)

**Códigos de Retorno:**
```cpp
enum DialogCode {
    Rejected = 0,
    Accepted = 1
};
```

**Uso en Proyecto:**
```cpp
LoginDialog login;
if (login.exec() == QDialog::Accepted) {
    // Usuario hizo login exitoso
    MainWindow window;
    return app.exec();
}
```

---

###### **QLineEdit**
```cpp
class QLineEdit : public QWidget
```

**Propósito:** Campo de entrada de texto de una línea

**Métodos Principales:**
| Método | Descripción |
|--------|-------------|
| `text()` | Obtiene texto actual |
| `setText()` | Establece texto |
| `setPlaceholderText()` | Texto de relleno (gris) |
| `setEchoMode()` | Modo de visualización |
| `clear()` | Borra contenido |

**Echo Modes:**
```cpp
enum EchoMode {
    Normal,                  // Muestra todos los caracteres
    NoEcho,                  // No muestra nada
    Password,                // Muestra puntos
    PasswordEchoOnEdit       // Muestra mientras escribe
};
```

**Uso en Proyecto:**
```cpp
userEdit = new QLineEdit(this);
userEdit->setPlaceholderText("Usuario");

passEdit = new QLineEdit(this);
passEdit->setPlaceholderText("Contraseña");
passEdit->setEchoMode(QLineEdit::Password);  // Oculta entrada
```

---

###### **QPushButton**
```cpp
class QPushButton : public QAbstractButton
```

**Propósito:** Botón presionable

**Señales:**
- `clicked(bool checked = false)` - Emitida cuando se hace clic

**Métodos:**
- `setText()` - Establece texto del botón
- `setIcon()` - Establece icono
- `setEnabled()` - Habilita/deshabilita

**Uso en Proyecto:**
```cpp
loginBttn = new QPushButton("Login", this);
connect(loginBttn, &QPushButton::clicked, 
        this, &LoginDialog::onLoginClicked);
```

---

###### **QLabel**
```cpp
class QLabel : public QWidget
```

**Propósito:** Mostrar texto o imagen

**Métodos:**
- `setText()` - Establece texto
- `setFont()` - Establece fuente
- `setAlignment()` - Alineación del texto

**Uso en Proyecto:**
```cpp
QLabel *titleLabel = new QLabel("Your Passwords", this);
QFont titleFont = titleLabel->font();
titleFont.setPointSize(16);
titleFont.setBold(true);
titleLabel->setFont(titleFont);
```

---

###### **QMessageBox**
```cpp
class QMessageBox : public QDialog
```

**Propósito:** Cuadros de diálogo para mensajes

**Métodos Estáticos:**
| Método | Propósito |
|--------|-----------|
| `warning()` | Muestra advertencia |
| `information()` | Muestra información |
| `question()` | Pregunta sí/no |
| `critical()` | Muestra error crítico |

**Uso en Proyecto:**
```cpp
if (!authM->authenticateUser(user, pass)) {
    QMessageBox::warning(this, "Error", "Credenciales incorrectas");
}
```

---

###### **QTableWidget**
```cpp
class QTableWidget : public QTableView
```

**Propósito:** Tabla editable y seleccionable

**Métodos Principales:**
| Método | Descripción |
|--------|-------------|
| `setColumnCount()` | Número de columnas |
| `setHorizontalHeaderLabels()` | Nombres de columnas |
| `insertRow()` | Añade fila |
| `setItem()` | Establece celda |
| `setColumnWidth()` | Ancho de columna |
| `setSelectionMode()` | Modo de selección |
| `setAlternatingRowColors()` | Filas coloreadas alternadas |

**Uso en Proyecto:**
```cpp
QTableWidget *table = new QTableWidget(this);
table->setColumnCount(4);
table->setHorizontalHeaderLabels({"Website", "Username", "Password", "Actions"});
table->setColumnWidth(0, 200);
table->insertRow(0);
table->setItem(0, 0, new QTableWidgetItem("Gmail"));
```

---

##### Gestores de Layout

###### **QVBoxLayout**
```cpp
class QVBoxLayout : public QLayout
```

**Propósito:** Distribuye widgets verticalmente

**Métodos:**
- `addWidget()` - Añade widget al layout
- `addLayout()` - Añade otro layout
- `setSpacing()` - Espacio entre widgets
- `setContentsMargins()` - Márgenes

**Uso en Proyecto:**
```cpp
QVBoxLayout *layout = new QVBoxLayout(this);
layout->addWidget(new QLabel("Usuario"));
layout->addWidget(userEdit);
layout->addWidget(new QLabel("Contraseña"));
layout->addWidget(passEdit);
```

---

###### **QHBoxLayout**
```cpp
class QHBoxLayout : public QLayout
```

**Propósito:** Distribuye widgets horizontalmente

**Uso en Proyecto:**
```cpp
QHBoxLayout *bttnLayout = new QHBoxLayout();
bttnLayout->addWidget(loginBttn);
bttnLayout->addWidget(cancelBttn);
layout->addLayout(bttnLayout);
```

---

#### Macro Q_OBJECT

```cpp
class LoginDialog : public QDialog
{
    Q_OBJECT  // <-- NECESARIO
    
private slots:
    void onLoginClicked();
    
signals:
    void userAuthenticated(const QString &username);
};
```

**Propósito:**
- Expande el sistema de meta-objeto
- Permite declarar slots y signals
- Procesado por Meta-Object Compiler (moc)

**Sin Q_OBJECT:**
- ❌ Los slots no funcionan
- ❌ Las señales no se emiten
- ❌ Errores en tiempo de compilación

---

## STL (Standard Template Library)

### Contenedores

#### `std::string`
**Cabecera:** `<string>`

```cpp
std::string password = "secure123";
std::string hashed_password = bytesToHex(hash_bytes);
```

**Métodos Clave:**
- `length()` / `size()` - Longitud
- `c_str()` - Obtiene C string (const char*)
- `substr()` - Subcadena
- `find()` - Busca subcadena
- `append()` / `+=` - Concatena

---

#### `std::vector<T>`
**Cabecera:** `<vector>`

```cpp
std::vector<unsigned char> salt = generateRandomBytes(16);
std::vector<unsigned char> bytes;
bytes.reserve(16);
bytes.push_back(0xA1);
```

**Métodos Clave:**
- `push_back()` - Añade elemento
- `data()` - Obtiene puntero a buffer
- `size()` - Cantidad de elementos
- `empty()` - ¿Está vacío?
- `clear()` - Vacía vector
- `reserve()` - Preasigna memoria

---

#### `std::pair<T, U>`
**Cabecera:** `<utility>`

```cpp
std::pair<std::string, std::string> result = hashPassword(password);
std::string hash = result.first;
std::string salt = result.second;

// Structured binding (C++17)
auto [hash, salt] = hashPassword(password);
```

**Acceso:**
- `.first` - Primer elemento
- `.second` - Segundo elemento

---

### Excepciones

#### `std::exception`
**Cabecera:** `<exception>`

```cpp
try {
    // Código que puede lanzar excepción
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

---

#### `std::runtime_error`
**Cabecera:** `<stdexcept>`

```cpp
if (RAND_bytes(buffer, length) != 1)
    throw std::runtime_error("RAND_bytes failed");
```

---

### I/O Streams

#### `std::cout` / `std::cerr`
**Cabecera:** `<iostream>`

```cpp
std::cout << "Mensaje a stdout" << std::endl;
std::cerr << "Error a stderr" << std::endl;
```

---

## POSIX (Portable Operating System Interface)

### Sistema de Archivos

#### `<sys/stat.h>`

**`mkdir()`**
```cpp
int mkdir(const char *path, mode_t mode);
```

| Parámetro | Descripción |
|-----------|-------------|
| `path` | Ruta del directorio a crear |
| `mode` | Permisos (ej: 0755) |
| **Retorno** | 0 si exitoso, -1 si falla |

**Modos (octal):**
- `0700` - rwx------ (solo propietario)
- `0755` - rwxr-xr-x (propietario rwx, otros r-x)
- `0777` - rwxrwxrwx (todos rwx)

**Uso en Proyecto:**
```cpp
// En createDirectory()
if (mkdir(path.c_str(), 0755) == 0 || errno == EEXIST) {
    return true;  // Exitoso o ya existe
}
```

---

**`stat()`**
```cpp
int stat(const char *path, struct stat *sb);
```

**Retorna información de archivo/directorio:**
```cpp
struct stat {
    mode_t st_mode;      // Tipo y permisos
    // ... otros campos
};
```

**Macros para verificar tipo:**
```cpp
S_ISDIR(st_mode)    // ¿Es directorio?
S_ISREG(st_mode)    // ¿Es archivo regular?
S_ISLNK(st_mode)    // ¿Es enlace simbólico?
```

**Uso en Proyecto:**
```cpp
struct stat sb;
if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
    // El directorio existe
}
```

---

## Tabla Resumen de Dependencias

| Librería | Módulo | Función |
|----------|--------|---------|
| OpenSSL rand.h | CRYPTO | Generación de números aleatorios |
| OpenSSL evp.h | CRYPTO | PBKDF2-SHA256 |
| SQLite3 | STORAGE | Persistencia de datos |
| Qt Core | CORE, APP, UI | Sistema de objetos y signals |
| Qt Gui | UI | Componentes visuales |
| Qt Widgets | UI | Widgets de alto nivel |
| STL | TODOS | Contenedores y algoritmos |
| POSIX | STORAGE, CORE | Sistema de archivos |

---

## Versiones Recomendadas

```
OpenSSL:  >= 1.1.1
SQLite3:  >= 3.30.0
Qt:       >= 6.2.0
GCC:      >= 11.0 (con C++17)
CMake:    >= 3.20
```

---

## Comandos de Instalación

### Debian/Ubuntu
```bash
sudo apt-get install \
    build-essential \
    cmake \
    qt6-base-dev \
    libssl-dev \
    libsqlite3-dev
```

### Fedora/RHEL
```bash
sudo dnf install \
    gcc-c++ \
    cmake \
    qt6-base-devel \
    openssl-devel \
    sqlite-devel
```

### macOS (con Homebrew)
```bash
brew install \
    cmake \
    qt6 \
    openssl \
    sqlite
```

---

## Licencias

| Librería | Licencia |
|----------|----------|
| OpenSSL | Apache 2.0 |
| SQLite3 | Public Domain |
| Qt | LGPL 3.0 (Free) o Comercial |
| C++ STL | Parte del compilador |

---

## Enlaces Útiles

- **OpenSSL:** https://www.openssl.org/
  - Documentación: https://www.openssl.org/docs/
  - PBKDF2: https://www.openssl.org/docs/man1.1.1/man3/PKCS5_PBKDF2_HMAC.html

- **SQLite:** https://www.sqlite.org/
  - Documentación C API: https://www.sqlite.org/c3ref/intro.html
  - Tutorial: https://www.sqlite.org/appfileformat.html

- **Qt:** https://www.qt.io/
  - Documentación: https://doc.qt.io/
  - Qt 6 API: https://doc.qt.io/qt-6/

- **RFC 8018 (PBKDF2):** https://tools.ietf.org/html/rfc8018
- **NIST Guidelines:** https://pages.nist.gov/800-63-3/sp800-63b.html

---

**Última actualización:** 13 de febrero de 2026
