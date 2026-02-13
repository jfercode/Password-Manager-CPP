# 🏗️ Arquitectura y Diagramas del Password Manager

## Tabla de Contenidos
1. [Arquitectura de Capas](#arquitectura-de-capas)
2. [Flujo de Autenticación](#flujo-de-autenticación)
3. [Flujo de Registro](#flujo-de-registro)
4. [Árbol de Dependencias](#árbol-de-dependencias)
5. [Modelo de Datos](#modelo-de-datos)
6. [Flujo de Criptografía](#flujo-de-criptografía)
7. [Interacciones de Clases](#interacciones-de-clases)

---

## Arquitectura de Capas

### Vista General Horizontal

```
┌──────────────────────────────────────────────────────────────────────┐
│                     INTERFAZ DE USUARIO (UI)                         │
│  ┌─────────────┬──────────────────┬────────────────┐                 │
│  │ LoginDialog │ MainWindow       │ NewUserDialog  │                 │
│  │             │ (no implementado) │ (esqueleto)    │                 │
│  └──────┬──────┴────────┬─────────┴────────┬───────┘                 │
│         │ emit signals  │ procesa eventos  │                          │
│         └──────┬────────┴─────────────────┬──────────┘               │
│                │ utiliza                  │                           │
│                ↓                          ↓                           │
├────────────────────────────────────────────────────────────────────────┤
│                   CAPA DE LÓGICA DE NEGOCIO (APP)                      │
│              ┌────────────────────────────────┐                        │
│              │  AuthenticationManager         │                        │
│              │  - registerNewUser()           │                        │
│              │  - authenticateUser()          │                        │
│              └────────┬──────────┬────────────┘                        │
│                       │ requiere │                                     │
│            ┌──────────┘          └─────────────┐                      │
│            │                                   │                      │
│            ↓                                   ↓                      │
├───────────────────────┬──────────────────┬─────────────────────────────┤
│  CAPA CRIPTOGRÁFICA   │                  │  CAPA DE PERSISTENCIA       │
│      (CRYPTO)         │                  │      (STORAGE)              │
│  ┌──────────────────┐ │                  │  ┌─────────────────────┐   │
│  │ CryptoManager    │ │                  │  │ SQLiteCipherDB      │   │
│  │ ┌──────────────┐ │ │                  │  │ ┌─────────────────┐ │   │
│  │ │hashPassword()│ │ │                  │  │ │createUser()    │ │   │
│  │ │verifyPasswd()│ │ │                  │  │ │getUserHash()   │ │   │
│  │ │generateBytes│ │ │                  │  │ │userExists()    │ │   │
│  │ └──────────────┘ │ │                  │  │ └─────────────────┘ │   │
│  └──────────────────┘ │                  │  └─────────────────────┘   │
│                       │                  │                            │
│  OpenSSL (PBKDF2)     │                  │  SQLite3                   │
└───────────────────────┴──────────────────┴─────────────────────────────┘
                                │
                                ↓
            ┌───────────────────────────────────┐
            │     CAPA DE UTILIDADES (CORE)     │
            │  ┌──────────────┬────────────────┐│
            │  │ PrintLog()   │createDirectory()││
            │  │ (Logging)    │ (Filesystem)   ││
            │  └──────────────┴────────────────┘│
            └───────────────────────────────────┘
```

### Características de Cada Capa

#### 🖥️ Capa de Presentación (UI)
**Responsabilidad:** Interacción con el usuario
- Captura entrada del usuario
- Muestra mensajes
- Emite señales Qt
- No contiene lógica de negocio
- No accede directamente a base de datos

**Clases:**
- `LoginDialog` - Diálogo de autenticación
- `MainWindow` - Ventana principal
- `NewUserDialog` - Diálogo de registro

---

#### 🎯 Capa de Lógica de Negocio (APP)
**Responsabilidad:** Orquestación de procesos
- Coordina entre UI, Crypto y Storage
- Válida reglas de negocio
- No manipula datos criptográficos
- No ejecuta queries SQL

**Clases:**
- `AuthenticationManager` - Gestor de autenticación

---

#### 🔐 Capa Criptográfica (CRYPTO)
**Responsabilidad:** Operaciones criptográficas
- Genera números aleatorios
- Hashea contraseñas
- Verifica contraseñas
- Conversiones hex<->bytes
- No accede a UI
- No accede a base de datos

**Clases:**
- `CryptoManager` - Gestor criptográfico

---

#### 💾 Capa de Persistencia (STORAGE)
**Responsabilidad:** Acceso a datos
- Crea/obtiene usuarios de BD
- Ejecuta queries SQL
- Gestiona conexión SQLite
- Maneja schema de datos
- No realiza criptografía
- No contains lógica de negocio

**Clases:**
- `SQLiteCipherDB` - Gestor de base de datos

---

#### 🔧 Capa de Utilidades (CORE)
**Responsabilidad:** Funciones transversales
- Logging con colores
- Creación de directorios
- Constantes globales
- Inclusiones comunes

**Funciones:**
- `PrintLog()` - Sistema de logging
- `createDirectory()` - Manejo de directorios

---

## Flujo de Autenticación

### Diagrama de Secuencia

```
Usuario          LoginDialog         AuthMgr         CryptoMgr       DB
   │                  │                  │               │             │
   ├─ Ingresa user ──→│                  │               │             │
   │ e password       │                  │               │             │
   │                  │                  │               │             │
   ├─ Presiona Login─→│                  │               │             │
   │                  │                  │               │             │
   │                  ├─ authenticateUser()               │             │
   │                  │                  │               │             │
   │                  │  getUserHash()────────────────────────────────→│
   │                  │   (busca en BD)  │               │             │
   │                  │                  │  ← hash+salt─│             │
   │                  │                  │               │             │
   │                  │  verifyPassword()───────────────→│             │
   │                  │  (compara hashes)│               │             │
   │                  │                  │ ← resultado ─│             │
   │                  │ ← true/false ────│               │             │
   │                  │                  │               │             │
   ├─ Si true:       │                  │               │             │
   │  MainWindow     │                  │               │             │
   │                  │                  │               │             │
   └─ Si false:      │                  │               │             │
      Error Box      │                  │               │             │
```

### Pasos Detallados

1. **Usuario abre aplicación**
   - Se muestra `LoginDialog`

2. **Usuario ingresa credenciales**
   - username → `userEdit`
   - password → `passEdit`

3. **Usuario presiona "Login"**
   - Señal `clicked()` emitida
   - Slot `onLoginClicked()` ejecutado

4. **LoginDialog obtiene credenciales**
   - `QString user = userEdit->text()`
   - `QString pass = passEdit->text()`
   - Convierte a `std::string`

5. **LoginDialog invoca AuthenticationManager**
   - `authM->authenticateUser(user, pass)`

6. **AuthenticationManager busca usuario**
   - Llama `db->getUserHash(username, hash, salt)`
   - Obtiene hash y salt almacenados

7. **AuthenticationManager verifica contraseña**
   - Llama `crypto->verifyPassword(password, hash, salt)`
   - PBKDF2 rehashea la contraseña
   - Compara hashes

8. **Resultado de verificación**
   - `true` → `accept()` → `MainWindow`
   - `false` → `QMessageBox::warning()` → Reintenta login

---

## Flujo de Registro

### Diagrama de Secuencia

```
Usuario           UI              AuthMgr         CryptoMgr        DB
   │               │                 │               │              │
   ├─ Presiona ───→│                 │               │              │
   │ "Registrar"   │                 │               │              │
   │               │                 │               │              │
   ├─ Ingresa user │                 │               │              │
   │ y password   │                 │               │              │
   │               │                 │               │              │
   ├─ Presiona ───→│                 │               │              │
   │ "Crear"      │                 │               │              │
   │               │                 │               │              │
   │               ├─ registerNewUser()               │              │
   │               │                 │               │              │
   │               │  userExists()─────────────────────────────────→│
   │               │  (verifica)      │               │              │
   │               │                  │  ← count ────│              │
   │               │                  │               │              │
   │               │  Si existe: retorna false       │              │
   │               │                  │               │              │
   │               │  Si no existe:    │               │              │
   │               │                   │               │              │
   │               │  hashPassword()────────────────→│              │
   │               │  (genera salt)    │               │              │
   │               │                   │  ← hash+salt ─│              │
   │               │                   │               │              │
   │               │  createUser()─────────────────────────────────→│
   │               │                   │               │              │
   │               │                   │  ← success ───│              │
   │               │                   │               │              │
   ├─ Éxito:      │                   │               │              │
   │ Usuario      │                   │               │              │
   │ creado       │                   │               │              │
   │               │                   │               │              │
   └─ Puede hacer └──────────────────────────────────────────────────
      login
```

### Pasos Detallados

1. **Usuario presiona "Registrar"**
   - Se muestra `NewUserDialog`

2. **Usuario ingresa nuevo usuario y contraseña**
   - Completa formulario

3. **Usuario presiona "Crear"**
   - Se invoca `registerNewUser()`

4. **Validar no duplicación**
   - `db->userExists(username)` → si true, error
   - Retorna false

5. **Generar hash y salt**
   - `crypto->hashPassword(password)`
   - Genera 16 bytes aleatorios = salt
   - Aplica PBKDF2-SHA256 10000 veces
   - Retorna {hash_hex, salt_hex}

6. **Almacenar usuario**
   - `db->createUser(username, hash, salt)`
   - INSERT en tabla users
   - Retorna true si exitoso

7. **Resultado**
   - `true` → Mensaje "Usuario creado"
   - `false` → Mensaje "Error creando usuario"

8. **Siguiente paso**
   - Usuario puede hacer login

---

## Árbol de Dependencias

### Dependencias de Módulos

```
┌──────────────────────────────────────────────┐
│                  Aplicación                  │
│               (main.cpp)                     │
└──────────────────┬─────────────────────────┘
                   │ crea
                   ↓
        ┌──────────────────────┐
        │  LoginDialog (UI)    │
        └─────────┬────────────┘
                  │ requiere
                  ↓
        ┌──────────────────────────┐
        │ AuthenticationManager    │
        │ (APP) - Lógica           │
        └──────┬──────────┬────────┘
               │ usa      │ usa
        ┌──────↓────┐  ┌──↓─────────────┐
        │ Crypto    │  │ SQLiteDB      │
        │ Manager   │  │ (STORAGE)     │
        │ (CRYPTO)  │  │               │
        └─────┬─────┘  └────┬──────────┘
              │             │
        ┌─────↓─────┐  ┌────↓──────────┐
        │ OpenSSL   │  │ SQLite3      │
        │ - rand    │  │ - sqlite3.h  │
        │ - evp     │  └──────────────┘
        └───────────┘

        Todos ↓ usan
        
        ┌──────────────────────────┐
        │   CORE (Logging, etc.)   │
        │ - PrintLog()             │
        │ - createDirectory()      │
        │ - library.hpp            │
        └──────────────────────────┘
```

### Matriz de Dependencias

```
         │ UI │ APP │ CRYPTO │ STORAGE │ CORE │ OpenSSL │ SQLite │ Qt │
─────────┼────┼─────┼────────┼─────────┼──────┼─────────┼────────┼────┤
UI       │ X  │  ✓  │        │         │  ✓   │         │        │ ✓  │
APP      │    │ X   │  ✓     │   ✓     │  ✓   │         │        │    │
CRYPTO   │    │     │  X     │         │  ✓   │   ✓     │        │    │
STORAGE  │    │     │        │  X      │  ✓   │         │   ✓    │    │
CORE     │    │     │        │         │  X   │         │        │ ✓  │
─────────┼────┼─────┼────────┼─────────┼──────┼─────────┼────────┼────┤
✓ = Depende
X = Es el mismo módulo
```

**Lectura:** "UI depende de APP" = ✓ en (UI, APP)

---

## Modelo de Datos

### Esquema de Base de Datos

```
┌─────────────────────────────────────────────────────────────────┐
│                         TABLA: users                            │
├──────┬──────────────┬──────────────────┬─────────────┬──────────┤
│ id   │ username     │ password_hash    │ password_   │ created_ │
│      │              │                  │ salt        │ at       │
├──────┼──────────────┼──────────────────┼─────────────┼──────────┤
│ KEY  │ UNIQUE NOT   │ TEXT NOT NULL    │ TEXT NOT    │ TIMESTAMP│
│ AUTO │ NULL         │ (256 bits HEX)   │ NULL        │ DEFAULT  │
│ INC  │              │                  │ (128 bits   │ CURRENT  │
│      │              │                  │ HEX)        │ TIMESTAMP│
├──────┼──────────────┼──────────────────┼─────────────┼──────────┤
│ 1    │ john         │ a1b2c3d4e5f6...  │ x1y2z3w4... │ 2024-02- │
│      │              │ (64 caracteres)  │ (32 caráct.)│ 13 10:30 │
├──────┼──────────────┼──────────────────┼─────────────┼──────────┤
│ 2    │ jane         │ q0p1o2n3m4l5...  │ s8r9q0p1... │ 2024-02- │
│      │              │ (64 caracteres)  │ (32 caráct.)│ 13 10:32 │
└──────┴──────────────┴──────────────────┴─────────────┴──────────┘
```

### Restricciones SQL

```sql
CREATE TABLE IF NOT EXISTS users(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    ↓
    Clave primaria, auto-incrementada
    
    username TEXT UNIQUE NOT NULL,
    ↓
    - UNIQUE: no permite duplicados
    - NOT NULL: siempre debe tener valor
    
    password_hash TEXT NOT NULL,
    password_salt TEXT NOT NULL,
    ↓
    - NOT NULL: ambos siempre presentes
    
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    ↓
    - Fecha automática de creación
)
```

### Ejemplo de Datos Almacenados

```
username: john_doe
password: MySecurePassword123!

Después de hashPassword():
- salt (16 bytes aleatorios): 
  x1y2z3w4v5u6t7s8r9q0p1o2n3m4l5k6
  
- hash (PBKDF2-SHA256 de "MySecurePassword123!"):
  a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6
  q0p1o2n3m4l5k6j7i8h9g0f1e2d3c4b5

Almacenados en BD:
username      = "john_doe"
password_hash = "a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6q0p1o2n3m4l5k6j7i8h9g0f1e2d3c4b5"
password_salt = "x1y2z3w4v5u6t7s8r9q0p1o2n3m4l5k6"
```

---

## Flujo de Criptografía

### Proceso de Hasheo (Registro)

```
password: "MyPassword123"
   │
   ├─→ CryptoManager::generateRandomBytes(16)
   │      │
   │      ├─→ OpenSSL RAND_bytes()
   │      │      │
   │      │      └─→ /dev/urandom (Linux)
   │      │
   │      └─→ salt = [random 16 bytes]
   │
   ├─→ CryptoManager::hashPassword()
   │      │
   │      ├─→ PBKDF2_HMAC(
   │      │      password = "MyPassword123"
   │      │      salt = [16 random bytes]
   │      │      iterations = 10,000
   │      │      algorithm = EVP_sha256()
   │      │   )
   │      │
   │      ├─→ Itera HMAC-SHA256 10,000 veces
   │      │      │
   │      │      ├─ Iteración 1: HMAC_SHA256(key, password+salt)
   │      │      ├─ Iteración 2: HMAC_SHA256(resultado_1, ...)
   │      │      ├─ ...
   │      │      └─ Iteración 10,000: resultado final
   │      │
   │      └─→ hash = [256 bits]
   │
   ├─→ CryptoManager::bytesToHex()
   │      │
   │      ├─→ hash_hex = "a1b2c3d4e5f6..." (64 caracteres)
   │      └─→ salt_hex = "x1y2z3w4v5u6..." (32 caracteres)
   │
   ├─→ SQLiteCipherDB::createUser()
   │      │
   │      ├─→ INSERT INTO users (username, password_hash, password_salt)
   │      │   VALUES (
   │      │      "john",
   │      │      "a1b2c3d4e5f6...",
   │      │      "x1y2z3w4v5u6..."
   │      │   )
   │      │
   │      └─→ Almacenado en BD
   │
   └─→ Retorna: {hash_hex, salt_hex}
```

### Proceso de Verificación (Login)

```
password_user: "MyPassword123"
password_stored_hash: "a1b2c3d4e5f6..."
password_stored_salt: "x1y2z3w4v5u6..."
   │
   ├─→ CryptoManager::verifyPassword()
   │      │
   │      ├─→ CryptoManager::hexToBytes(salt)
   │      │      │
   │      │      └─→ salt_bytes = [16 bytes]
   │      │
   │      ├─→ PBKDF2_HMAC(
   │      │      password = "MyPassword123"
   │      │      salt = salt_bytes
   │      │      iterations = 10,000
   │      │      algorithm = EVP_sha256()
   │      │   )
   │      │
   │      ├─→ computed_hash = [256 bits]
   │      │
   │      ├─→ CryptoManager::bytesToHex()
   │      │      │
   │      │      └─→ computed_hex = "a1b2c3d4e5f6..."
   │      │
   │      ├─→ Comparación:
   │      │      computed_hex == password_stored_hash
   │      │
   │      └─→ Retorna: true (coincide) o false (no coincide)
   │
   └─→ Resultado de autenticación
```

### Seguridad del Algoritmo PBKDF2

```
Iteración 1:
┌─────────────────────────────┐
│ HMAC_SHA256(key, input)     │
│ key = ""                    │
│ input = password + salt     │
│ output = hash_1             │
└─────────────────────────────┘
              │
              ↓
Iteración 2:
┌─────────────────────────────┐
│ HMAC_SHA256(key, input)     │
│ key = hash_1                │
│ input = password + salt     │
│ output = hash_2             │
└─────────────────────────────┘
              │
              ↓
Iteración 3:
┌─────────────────────────────┐
│ HMAC_SHA256(...)            │
│ output = hash_3             │
└─────────────────────────────┘
              │
              ↓
            ...
              │
              ↓
Iteración 10,000:
┌─────────────────────────────┐
│ HMAC_SHA256(...)            │
│ output = hash_final         │
└─────────────────────────────┘
              │
              ↓
      hash_final (256 bits)
         ↓ bytesToHex()
      "a1b2c3d4e5f6..." (64 chars)

Ventajas:
- 10,000 iteraciones = ~1 segundo de cálculo
- Ataque de fuerza bruta: 1 segundo por intento
- Diccionario de 1,000,000 contraseñas = 277 horas
- Salt único previene tablas rainbow
```

---

## Interacciones de Clases

### Diagrama de Clases Simplificado

```
┌──────────────────────┐
│    LoginDialog       │
│  (QDialog)           │
├──────────────────────┤
│ - authM: AuthMgr*   │
│ - userEdit: QLine*  │
│ - passEdit: QLine*  │
├──────────────────────┤
│ + setupUi()          │
│ + onLoginClicked()   │
│ + onCancelClicked()  │
└─────────────┬────────┘
              │ invoca
              │
              ↓
┌──────────────────────────────┐
│  AuthenticationManager        │
├──────────────────────────────┤
│ - crypto: CryptoMgr*         │
│ - db: SQLiteCipherDB*        │
├──────────────────────────────┤
│ + authenticateUser()          │
│ + registerNewUser()           │
└──────┬─────────────┬──────────┘
       │ usa         │ usa
       │             │
       ↓             ↓
┌────────────────┐ ┌──────────────────┐
│  CryptoManager │ │ SQLiteCipherDB   │
├────────────────┤ ├──────────────────┤
│ - db: sqlite3* │ │ - db: sqlite3*   │
├────────────────┤ │ - dbPath: string │
│ + hashPassword │ ├──────────────────┤
│ + verifyPass() │ │ + createUser()   │
│ - genRandom()  │ │ + getUserHash()  │
│ - bytesToHex() │ │ + userExists()   │
│ - hexToBytes() │ │ - setupDB()      │
└────────────────┘ │ - findDBPath()   │
                   └──────────────────┘
```

### Secuencia de Creación de Objetos

```
main()
├─ QApplication app(argc, argv)
│
├─ LoginDialog login
│  ├─ Constructor QDialog(nullptr)
│  ├─ setWindowTitle(...)
│  ├─ setupUi()
│  │  ├─ new QLineEdit()
│  │  ├─ new QPushButton()
│  │  ├─ new QLabel()
│  │  ├─ new QVBoxLayout()
│  │  └─ new QHBoxLayout()
│  └─ connect(...) señales a slots
│
└─ login.exec()
   ├─ Muestra diálogo modal
   ├─ Espera resultado
   │
   └─ Si aceptado:
      ├─ MainWindow window
      │  └─ Constructor QMainWindow()
      │     ├─ setWindowTitle(...)
      │     ├─ setupUI()
      │     └─ connect(...) señales
      │
      └─ app.exec()
         └─ Inicia event loop
```

### Interacción Login-Authentication-Crypto-DB

```
LoginDialog::onLoginClicked()
   │
   ├─ user = userEdit->text()
   ├─ pass = passEdit->text()
   │
   ├─ authM->authenticateUser(user, pass)
   │  │
   │  ├─ db->getUserHash(user, hash, salt)
   │  │  │
   │  │  └─→ [SQLite Query]
   │  │     SELECT password_hash, password_salt
   │  │     FROM users WHERE username = ?
   │  │
   │  │  ← hash, salt obtenidos
   │  │
   │  ├─ crypto->verifyPassword(pass, hash, salt)
   │  │  │
   │  │  ├─ hexToBytes(salt)
   │  │  ├─ PBKDF2_HMAC(pass, salt, 10000)
   │  │  ├─ bytesToHex(resultado)
   │  │  └─ Comparar con hash
   │  │
   │  │  ← true o false
   │  │
   │  └─ ← resultado autenticación
   │
   ├─ Si true:
   │  ├─ accept()
   │  └─ MainWindow abierta
   │
   └─ Si false:
      └─ QMessageBox error
```

---

## Resumen Arquitectónico

### Principios Implementados

1. **Layered Architecture**
   - Separación clara entre UI, Lógica, Datos
   - Cada capa tiene responsabilidad específica

2. **Dependency Injection**
   - AuthenticationManager recibe sus dependencias
   - Facilita testing y mantenimiento

3. **Single Responsibility**
   - CryptoManager = solo criptografía
   - SQLiteCipherDB = solo datos
   - AuthenticationManager = orquestación

4. **High Cohesion, Low Coupling**
   - Módulos cohesivos internamente
   - Débil acoplamiento entre módulos

5. **Separation of Concerns**
   - UI no conoce detalles criptográficos
   - Criptografía no conoce estructura de BD
   - BD no conoce lógica de negocio

### Beneficios

✅ **Mantenibilidad:** Cambios aislados a módulos específicos
✅ **Testabilidad:** Cada módulo puede testearse independientemente
✅ **Escalabilidad:** Fácil añadir nuevas características
✅ **Claridad:** Estructura lógica y evidente
✅ **Seguridad:** Responsabilidades claras para auditoría

---

**Última actualización:** 13 de febrero de 2026
