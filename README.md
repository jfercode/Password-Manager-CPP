# Password Manager - Documentación Completa del Proyecto

## Índice General

1. [Introducción](#introducción)
2. [Arquitectura General](#arquitectura-general)
3. [Módulos del Proyecto](#módulos-del-proyecto)
4. [Flujos Principales](#flujos-principales)
5. [Librerías Utilizadas](#librerías-utilizadas)
6. [Consideraciones de Seguridad](#consideraciones-de-seguridad)
7. [Guía de Compilación y Ejecución](#guía-de-compilación-y-ejecución)

---

## Introducción

**Password Manager** es una aplicación de escritorio desarrollada en **C++** que proporciona un gestor seguro de contraseñas. La aplicación permite a los usuarios registrarse, autenticarse y almacenar contraseñas de forma segura utilizando criptografía moderna (PBKDF2-SHA256) y bases de datos SQLite.

### Características Principales:
- ✅ Autenticación segura de usuarios
- ✅ Registro de nuevos usuarios
- ✅ Almacenamiento seguro de contraseñas con hash y salt
- ✅ Interfaz gráfica moderna con Qt
- ✅ Criptografía robusta con OpenSSL
- ✅ Base de datos SQLite embebida
- ✅ Logs coloreados en terminal para debugging

### Requisitos del Sistema:
- **Sistema Operativo:** Linux/Unix (compilación específica a POSIX)
- **Compilador:** GCC/Clang con soporte C++17
- **Qt Framework:** Qt 6.x
- **OpenSSL:** OpenSSL 1.1.x o superior
- **SQLite3:** Desarrollo headers incluidos

---

## Arquitectura General

### Patrón de Arquitectura: Capas Verticales

La aplicación está dividida en capas independientes que se comunican entre sí:

```
┌────────────────────────────────────────────────┐
│           CAPA DE PRESENTACIÓN (UI)            │
│  - LoginDialog                                 │
│  - MainWindow                                  │
│  - NewUserDialog                               │
└────────────────────────────────────────────────┘
              ↓ (Interacción del Usuario)
┌────────────────────────────────────────────────┐
│        CAPA DE LÓGICA DE NEGOCIO (APP)         │
│  - AuthenticationManager                       │
│    ├─ Registra usuarios                        │
│    └─ Autentica usuarios                       │
└────────────────────────────────────────────────┘
         ↓ (Requiere)         ↓ (Requiere)
┌──────────────────────┐  ┌──────────────────────┐
│  CAPA CRIPTOGRÁFICA  │  │  CAPA DE DATOS       │
│  (CRYPTO)            │  │  (STORAGE)           │
│  - CryptoManager     │  │  - SQLiteCipherDB    │
│    ├─ hashPassword() │  │    ├─ createUser()   │
│    └─ verifyPass()   │  │    ├─ getUserHash()  │
│                      │  │    └─ userExists()   │
└──────────────────────┘  └──────────────────────┘
                    ↓
        ┌──────────────────────┐
        │   CAPA UTILIDADES    │
        │   (CORE)             │
        │   - PrintLog()       │
        │   - createDirectory()│
        └──────────────────────┘
```

### Principios de Diseño:

1. **Separación de Responsabilidades:**
   - Cada módulo tiene una responsabilidad clara
   - UI no maneja criptografía
   - CryptoManager no accede a base de datos

2. **Composición sobre Herencia:**
   - AuthenticationManager contiene referencias a CryptoManager y SQLiteCipherDB
   - UI contiene referencias a AuthenticationManager

3. **Inyección de Dependencias:**
   - Los objetos reciben sus dependencias en el constructor
   - Favorece testabilidad y desacoplamiento

4. **Const Correctness:**
   - Métodos que no modifican estado son `const`
   - Punteros const garantizan no modificación

---

## Módulos del Proyecto

### 1. **Módulo CORE** (`/src/core/`)
**Responsabilidad:** Utilidades fundamentales y logging

**Archivos:**
- `Debug.cpp` - Sistema de logging con colores
- `Filesystem.cpp` - Operaciones del sistema de archivos
- `library.hpp` - Header master con todas las inclusiones

**Funciones Clave:**
- `PrintLog()` - Logging formateado con códigos ANSI
- `createDirectory()` - Crea directorios recursivamente

**Documentación:** Consultar `doc/CORE_MODULE.md`

---

### 2. **Módulo CRYPTO** (`/src/crypto/`)
**Responsabilidad:** Operaciones criptográficas y hashing

**Archivos:**
- `CryptoManager.cpp` - Implementación de gestor criptográfico
- `CryptoManager.hpp` - Interfaz pública

**Clases:**
- `CryptoManager` - Gestor de operaciones criptográficas

**Funciones Clave:**
- `hashPassword()` - Genera hash PBKDF2-SHA256 + salt
- `verifyPassword()` - Verifica contraseña contra hash
- `generateRandomBytes()` (privada) - Genera bytes aleatorios
- `bytesToHex()` (privada) - Convierte bytes a hexadecimal
- `hexToBytes()` (privada) - Convierte hexadecimal a bytes

**Librerías Utilizadas:**
- `<openssl/rand.h>` - Generación aleatoria
- `<openssl/evp.h>` - PBKDF2-SHA256
- `<openssl/err.h>` - Manejo de errores

**Algoritmo:** PBKDF2-SHA256 con 10000 iteraciones

**Documentación:** Consultar `doc/CRYPTO_MODULE.md`

---

### 3. **Módulo STORAGE** (`/src/storage/`)
**Responsabilidad:** Persistencia de datos en SQLite

**Archivos:**
- `SQLiteCipherDB.cpp` - Implementación de acceso a BD
- `SQLiteCipherDB.hpp` - Interfaz pública

**Clases:**
- `SQLiteCipherDB` - Gestor de base de datos SQLite

**Funciones Clave:**
- `createUser()` - Inserta nuevo usuario
- `getUserHash()` - Obtiene hash y salt de usuario
- `userExists()` - Verifica existencia de usuario
- `setupDB()` (privada) - Inicializa estructura de tabla
- `findDataBasePath()` (privada) - Localiza/crea ruta de BD

**Librerías Utilizadas:**
- `<sqlite3.h>` - API de SQLite3

**Base de Datos:** Archivo SQLite en `~/.local/share/passman/passman.db`

**Esquema:** Tabla `users` con columnas: id, username, password_hash, password_salt, created_at

**Documentación:** Consultar `doc/STORAGE_MODULE.md`

---

### 4. **Módulo APP** (`/src/app/`)
**Responsabilidad:** Lógica de negocio y autenticación

**Archivos:**
- `AuthenticationManager.cpp` - Implementación
- `AuthenticationManager.hpp` - Interfaz pública

**Clases:**
- `AuthenticationManager` - Gestor centralizado de autenticación

**Funciones Clave:**
- `registerNewUser()` - Registra nuevo usuario en sistema
- `authenticateUser()` - Autentica usuario existente

**Responsabilidades:**
- Coordina entre UI, Crypto y Storage
- Valida no duplicación de usuarios
- Hashea contraseñas al registrar
- Verifica contraseñas al autenticar

**Documentación:** Consultar `doc/APP_MODULE.md`

---

### 5. **Módulo UI** (`/src/ui/`)
**Responsabilidad:** Interfaz gráfica de usuario

**Archivos:**
- `LoginDialog.cpp` - Diálogo de inicio de sesión
- `LoginDialog.hpp` - Interfaz del diálogo de login
- `MainWindow.cpp` - Ventana principal
- `MainWindow.hpp` - Interfaz de ventana principal
- `NewUserDialog.cpp` - Diálogo de registro (esqueleto)
- `NewUserDialog.hpp` - Interfaz de diálogo de registro

**Clases:**
- `LoginDialog : public QDialog` - Diálogo modal para autenticación
- `MainWindow : public QMainWindow` - Ventana principal post-login
- `NewUserDialog : public QDialog` - Diálogo para registro (no implementado)

**Librerías Utilizadas:**
- Qt Core, Qt Gui, Qt Widgets
- Sistema de Señales y Slots

**Documentación:** Consultar `doc/UI_MODULE.md`

---

## Flujos Principales

### Flujo 1: Registro de Nuevo Usuario

```
Usuario abre aplicación
    ↓
LoginDialog::exec() - Muestra diálogo de login
    ↓
Usuario presiona "Registrar" / "Nuevo Usuario"
    ↓
NewUserDialog::exec() - Muestra diálogo de registro
    ↓
Usuario ingresa usuario y contraseña
    ↓
LoginDialog::registerNewUser()
    ↓
AuthenticationManager::registerNewUser(username, password)
    ├─ SQLiteCipherDB::userExists(username)
    │  └─ Si existe: retorna false
    ├─ CryptoManager::hashPassword(password)
    │  ├─ generateRandomBytes(16) → salt
    │  ├─ PBKDF2_HMAC(password, salt, 10000 iteraciones)
    │  └─ bytesToHex(hash, salt) → {hash_hex, salt_hex}
    ├─ SQLiteCipherDB::createUser(username, hash_hex, salt_hex)
    │  ├─ sqlite3_prepare_v2() - Prepara INSERT
    │  ├─ sqlite3_bind_text() - Vincula parámetros
    │  ├─ sqlite3_step() - Ejecuta inserción
    │  └─ sqlite3_finalize() - Limpia
    └─ Retorna true si exitoso
    ↓
QMessageBox "Usuario creado exitosamente"
    ↓
Usuario puede hacer login con nuevas credenciales
```

### Flujo 2: Autenticación de Usuario

```
Usuario abre aplicación
    ↓
LoginDialog::exec() - Muestra diálogo de login
    ↓
Usuario ingresa username y password
    ↓
Usuario presiona botón "Login"
    ↓
LoginDialog::onLoginClicked()
    ├─ QString user = userEdit->text()
    ├─ QString pass = passEdit->text()
    ↓
AuthenticationManager::authenticateUser(user, pass)
    ├─ SQLiteCipherDB::getUserHash(username, hash, salt)
    │  ├─ sqlite3_prepare_v2() - Prepara SELECT
    │  ├─ sqlite3_bind_text() - Vincula username
    │  ├─ sqlite3_step() - Ejecuta consulta
    │  ├─ sqlite3_column_text() - Extrae hash y salt
    │  └─ sqlite3_finalize() - Limpia
    ├─ CryptoManager::verifyPassword(password, hash, salt)
    │  ├─ hexToBytes(salt) → salt_bytes
    │  ├─ PBKDF2_HMAC(password, salt_bytes, 10000) → computed_hash
    │  ├─ bytesToHex(computed_hash) → computed_hex
    │  ├─ Compara computed_hex == hash
    │  └─ Retorna true si coinciden
    └─ Retorna resultado de verificación
    ↓
Si LoginDialog::accept()
    └─ MainWindow::exec() - Abre ventana principal
Else
    └─ QMessageBox::warning() "Credenciales incorrectas"
```

### Flujo 3: Inicialización de Base de Datos

```
SQLiteCipherDB constructor
    ↓
findDataBasePath()
    ├─ std::getenv("HOME") → /home/javier
    ├─ Construir rutas:
    │  ├─ baseDir = /home/javier/.local
    │  ├─ shareDir = /home/javier/.local/share
    │  └─ appDir = /home/javier/.local/share/passman
    ├─ createDirectory(baseDir)
    │  ├─ stat() - Verifica si existe
    │  └─ mkdir(0755) - Crea si no existe
    ├─ createDirectory(shareDir)
    ├─ createDirectory(appDir)
    └─ dbPath = /home/javier/.local/share/passman/passman.db
    ↓
sqlite3_open(dbPath, &db)
    └─ Crea archivo si no existe
    ↓
setupDB()
    ├─ sqlite3_exec() - Ejecuta CREATE TABLE IF NOT EXISTS
    └─ Tabla users con columnas: id, username, password_hash, password_salt, created_at
    ↓
PrintLog() - "SQLiteCipherDB - db running!"
```

---

## Librerías Utilizadas

### Librerías de Terceros (Externas)

#### 1. **OpenSSL** (Criptografía)
- **Versión:** 1.1.x
- **Módulos Utilizados:**
  - `<openssl/rand.h>` - Generación de números aleatorios
  - `<openssl/evp.h>` - Funciones EVP para PBKDF2
  - `<openssl/err.h>` - Manejo de errores
- **Funciones Clave:**
  - `RAND_bytes()` - Generación de bytes aleatorios
  - `PKCS5_PBKDF2_HMAC()` - Derivación de clave con PBKDF2
  - `EVP_sha256()` - Obtiene descriptor SHA256
- **Propósito:** Operaciones criptográficas seguras

#### 2. **SQLite3** (Base de Datos)
- **Versión:** 3.x
- **Header:** `<sqlite3.h>`
- **Funciones Clave:**
  - `sqlite3_open()` - Abre conexión a BD
  - `sqlite3_exec()` - Ejecuta SQL directo
  - `sqlite3_prepare_v2()` - Prepara statement
  - `sqlite3_bind_text()` - Vincula parámetros
  - `sqlite3_step()` - Ejecuta statement
  - `sqlite3_column_text()` - Extrae datos
  - `sqlite3_close()` - Cierra conexión
- **Propósito:** Persistencia de datos de usuarios

#### 3. **Qt Framework** (Interfaz Gráfica)
- **Versión:** Qt 6.x
- **Módulos Utilizados:**
  - **QtCore:** Clases base (QObject, QString, etc.)
  - **QtGui:** Componentes visuales básicos
  - **QtWidgets:** Widgets de alto nivel
- **Clases Clave:**
  - `QApplication` - Aplicación principal Qt
  - `QMainWindow` - Ventana principal
  - `QDialog` - Diálogos modales
  - `QLineEdit` - Campos de entrada
  - `QPushButton` - Botones
  - `QLabel` - Etiquetas
  - `QVBoxLayout`/`QHBoxLayout` - Gestores de diseño
  - `QTableWidget` - Tabla de datos
  - `QMessageBox` - Cuadros de diálogo
- **Propósito:** Creación de interfaz gráfica multiplataforma

### Librerías Estándar de C++ (STL)

```cpp
#include <iostream>      // I/O: std::cout, std::cerr
#include <string>        // std::string
#include <vector>        // std::vector<T>
#include <cstring>       // Funciones de cadena C
#include <cstdio>        // printf, sprintf
#include <exception>     // std::exception
#include <stdexcept>     // std::runtime_error
#include <utility>       // std::pair, structured binding (C++17)
#include <functional>    // std::function
#include <algorithm>     // Algoritmos (sort, find, etc.)
#include <cstdlib>       // Funciones del sistema
#include <sys/stat.h>    // Funciones de sistema de archivos (POSIX)
```

### Tabla de Dependencias por Módulo

| Módulo | OpenSSL | SQLite3 | Qt | STL |
|--------|---------|---------|----|----|
| CORE | - | - | ✓ | ✓ |
| CRYPTO | ✓ | - | - | ✓ |
| STORAGE | - | ✓ | - | ✓ |
| APP | - | - | - | ✓ |
| UI | - | - | ✓ | ✓ |

---

## Consideraciones de Seguridad

### 1. **Hashing de Contraseñas**

✅ **Implementado:** PBKDF2-SHA256 con 10000 iteraciones
- Algoritmo resistente a ataques de fuerza bruta
- Cada usuario tiene salt único (16 bytes aleatorios)
- Iteraciones hacen computacionalmente costoso probar muchas contraseñas
- Conforme a estándares NIST

### 2. **Generación de Números Aleatorios**

✅ **Implementado:** OpenSSL RAND_bytes()
- Usa `/dev/urandom` en sistemas Unix/Linux
- Criptográficamente seguro
- No predecible
- Adecuado para generar salts

### 3. **Prepared Statements SQL**

✅ **Implementado:** Parametrización de consultas
```cpp
sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
```
- Previene SQL injection
- Separa datos de estructura SQL
- Validación automática

### 4. **Almacenamiento de Credenciales**

✅ **Implementado:** Base de datos SQLite local
- Contraseñas nunca en texto plano
- Solo se almacenan hash y salt
- Ubicación segura: `~/.local/share/passman/`
- Permisos restrictivos: 0755

### 5. **Validación de Entrada**

✅ **Implementado:** En cada capa
- AuthenticationManager valida no duplicación
- SQLiteCipherDB usa UNIQUE constraint
- Qt acepta cualquier entrada (validación posterior)

### 6. **Manejo de Errores**

✅ **Implementado:** Try-catch y códigos de retorno
- Excepciones para condiciones críticas
- Logs de errores para auditoría
- No expone detalles internos en mensajes públicos

### ⚠️ **Limitaciones de Seguridad Conocidas**

1. **Sin Cifrado de Base de Datos:** SQLite almacena datos en texto plano
   - **Solución Futura:** Integrar SQLCipher
   
2. **Sin Timeout de Sesión:** Usuario logineado indefinidamente
   - **Solución Futura:** Implementar timeout de inactividad
   
3. **Sin Auditoría:** No se registran intentos de login fallidos
   - **Solución Futura:** Tabla de logs de autenticación
   
4. **Single-threaded:** SQLite tiene limitaciones de concurrencia
   - **Solución Futura:** Usar SQLite con WAL mode

---

## Guía de Compilación y Ejecución

### Requisitos Previos

```bash
# En Debian/Ubuntu
sudo apt-get install build-essential cmake qt6-base-dev libssl-dev libsqlite3-dev

# En Fedora
sudo dnf install gcc-c++ cmake qt6-base-devel openssl-devel sqlite-devel
```

### Compilación

```bash
# Desde raíz del proyecto
cd /home/javier/Escritorio/Password-Manager-CPP

# Crear directorio de construcción
mkdir -p build
cd build

# Ejecutar CMake
cmake ..

# Compilar
make -j$(nproc)

# Ejecutable resultante
./PasswordManager
```

### Ejecución

```bash
# Desde directorio de build
./PasswordManager

# O usando script de ejecución
../run-app.sh
```

### Estructura de Directorios Post-Compilación

```
build/
├── PasswordManager          (Ejecutable)
├── CMakeFiles/
├── CMakeCache.txt
├── cmake_install.cmake
├── Makefile
└── PasswordManager_autogen/
```

### Variables de Entorno

```bash
# Base de datos se crea automáticamente en:
~/.local/share/passman/passman.db

# Logs se imprimen en stdout/stderr
# Códigos ANSI requieren terminal que soporte colores
```

---

## Conclusión

Password Manager es una aplicación bien estructurada que demuestra:
- ✅ Arquitectura en capas
- ✅ Separación de responsabilidades
- ✅ Criptografía moderna
- ✅ Interfaz gráfica moderna
- ✅ Buenas prácticas de C++
- ✅ Logging y debugging efectivo

El proyecto es un excelente punto de partida para un gestor de contraseñas seguro y puede extenderse con características adicionales como sincronización en la nube, generador de contraseñas, auditoría, y más.

---

## Documentación Adicional

Para información detallada sobre cada módulo, consultar:

- **Módulo APP:** `doc/APP_MODULE.md`
- **Módulo CRYPTO:** `doc/CRYPTO_MODULE.md`
- **Módulo STORAGE:** `doc/STORAGE_MODULE.md`
- **Módulo UI:** `doc/UI_MODULE.md`
- **Módulo CORE:** `doc/CORE_MODULE.md`

---

**Última actualización:** 13 de febrero de 2026
**Autor:** Javier - Password Manager Development
**Licencia:** Ver LICENSE.md en raíz del proyecto
