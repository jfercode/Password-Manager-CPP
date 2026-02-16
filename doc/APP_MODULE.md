# Módulo APP - Gestor de Autenticación

## Descripción General
El módulo `app/` contiene la lógica de autenticación y gestión de usuarios del sistema de Password Manager. Es responsable de coordinar las operaciones de login y registro, actuando como intermediario entre la interfaz gráfica (UI) y los servicios de base de datos y criptografía.

---

## Archivos Incluidos

### 1. **InitializationManager.hpp** (NEW)
Archivo de encabezado que define la clase `InitializationManager`.

**Ubicación:** `/include/InitializationManager.hpp`

#### Clase: `InitializationManager`
Gestor centralizado de inicialización del sistema que verifica si la aplicación es una instalación nueva o existente.

**Miembros Privados:**
- `const SQLiteCipherDB *db` - Puntero a instancia de base de datos para verificar estado
- `const AuthenticationManager *authM` - Puntero a gestor de autenticación

**Constructor:**
```cpp
InitializationManager(const SQLiteCipherDB *database, const AuthenticationManager *auth);
```
- Inicializa el gestor con referencias a la base de datos y autenticación
- Registra logs de inicialización

**Destructor:**
```cpp
~InitializationManager();
```
- Realiza limpieza de recursos

**Métodos Públicos:**

1. **`isSystemInitialized() const`**
   - **Propósito:** Verifica si el sistema ha sido inicializado (si existe un usuario admin)
   - **Retorno:** `bool` - `true` si existe un usuario admin, `false` en caso contrario
   - **Proceso:**
     1. Llama a `SQLiteCipherDB::hasMasterUser()`
     2. Registra logs del estado del sistema
     3. Retorna el resultado

2. **`getRequiredDialogType() const`**
   - **Propósito:** Determina qué diálogo mostrar al usuario
   - **Retorno:** `int` - 0 para NewUserDialog (primer setup), 1 para LoginDialog (usuario existente)
   - **Proceso:**
     1. Verifica si sistema está inicializado
     2. Retorna tipo de diálogo apropiado

---

### 2. **InitializationManager.cpp**
Implementación de la clase `InitializationManager`.

**Ubicación:** `/src/app/InitializationManager.cpp`

#### Detalles de Implementación

**Constructor:**
```cpp
InitializationManager::InitializationManager(const SQLiteCipherDB *database, const AuthenticationManager *auth)
    : db(database), authM(auth)
```
- Asigna los punteros a las dependencias
- Registra log de inicialización con color CYAN

**`isSystemInitialized()`:**
- Verifica que `db` no sea null
- Llama a `db->hasMasterUser()`
- Registra log con color GREEN si inicializado, YELLOW si no
- Retorna el resultado

**`getRequiredDialogType()`:**
- Utiliza `isSystemInitialized()` para determinar estado
- Retorna 0 para NewUserDialog (sistema nuevo)
- Retorna 1 para LoginDialog (sistema existente)
- Registra logs descriptivos del tipo de diálogo

---

### 1. **AuthenticationManager.hpp**
Archivo de encabezado que define la clase `AuthenticationManager`.

**Ubicación:** `/include/AuthenticationManager.hpp`

#### Clase: `AuthenticationManager`
Gestor centralizado de autenticación que coordina la verificación de usuarios y el registro de nuevos usuarios.

**Miembros Privados:**
- `const CryptoManager *crypto` - Puntero a instancia de gestor criptográfico para hash de contraseñas
- `const SQLiteCipherDB *db` - Puntero a instancia de base de datos SQLite para almacenamiento de usuarios

**Constructor:**
```cpp
AuthenticationManager(const CryptoManager *cry, const SQLiteCipherDB *dB);
```
- Inicializa el gestor con referencias a los servicios de criptografía y base de datos
- Parámetros:
  - `cry`: Puntero al gestor criptográfico
  - `dB`: Puntero a la base de datos SQLite

**Destructor:**
```cpp
~AuthenticationManager();
```
- Realiza la limpieza de recursos

**Métodos Públicos:**

1. **`authenticateUser(const std::string &username, const std::string &password) const`**
   - **Propósito:** Autentica a un usuario existente verificando sus credenciales
   - **Parámetros:**
     - `username`: Nombre de usuario a autenticar
     - `password`: Contraseña en texto plano
   - **Retorno:** `bool` - `true` si la autenticación es exitosa, `false` en caso contrario
   - **Proceso:**
     1. Busca el usuario en la base de datos
     2. Obtiene el hash y salt almacenados
     3. Verifica la contraseña contra el hash usando CryptoManager
     4. Registra logs en consola durante el proceso

2. **`registerNewUser(const std::string &username, const std::string &password, bool isMaster) const`**
   - **Propósito:** Registra un nuevo usuario en el sistema
   - **Parámetros:**
     - `username`: Nombre de usuario para la nueva cuenta
     - `password`: Contraseña en texto plano a proteger
     - `isMaster`: Boolean que indica si el usuario es administrador (true) o usuario normal (false)
   - **Retorno:** `bool` - `true` si el registro es exitoso, `false` en caso contrario
   - **Proceso:**
     1. Verifica que el usuario no exista ya en la base de datos
     2. Genera hash y salt usando CryptoManager
     3. Crea el usuario en la base de datos con los datos criptografrados y el flag is_admin
     4. Registra logs del proceso

---

### 2. **AuthenticationManager.cpp**
Implementación de la clase `AuthenticationManager`.

**Ubicación:** `/src/app/AuthenticationManager.cpp`

#### Detalles de Implementación

**Constructor:**
```cpp
AuthenticationManager::AuthenticationManager(const CryptoManager *cry, const SQLiteCipherDB *dB)
```
- Inicializa los punteros a los servicios
- Registra un log de inicialización

**`authenticateUser()`:**
- Registra un log indicando que inicia la autenticación
- Obtiene el hash y salt almacenados del usuario
- Retorna `false` si el usuario no existe
- Utiliza `CryptoManager::verifyPassword()` para verificar
- Registra logs de éxito o fallo

**`registerNewUser()`:**
- Verifica que el usuario no exista (previene duplicados)
- Genera el par hash-salt usando `CryptoManager::hashPassword()`
- Utiliza structured binding `auto [hash, salt] = ...` (C++17)
- Llama a `SQLiteCipherDB::createUser()` con el parámetro `isMaster` para persistir los datos
- Registra logs detallados del proceso
- El parámetro `isMaster` se convierte a `1` o `0` en la base de datos (is_admin field)

---

## Flujo de Inicialización del Sistema (NUEVO)

```
Aplicación Inicia
    ↓
QApplication creado
    ↓
InitializationManager::isSystemInitialized()
    ↓
    SQLiteCipherDB::hasMasterUser()
    ↓
    SELECT COUNT(*) FROM users WHERE is_admin = 1
    ↓
    ┌─ Retorna 0 (No existe admin)
    │   ↓
    │   Mostrar NewUserDialog
    │   (Crear primer usuario admin)
    │
    └─ Retorna > 0 (Existe admin)
        ↓
        Mostrar LoginDialog
        (Usuario existente hace login)
    ↓
Dialog::exec()
    ↓
    ┌─ Dialog aceptado (Accepted)
    │   ↓
    │   MainWindow abre
    │
    └─ Dialog rechazado (Rejected)
        ↓
        Aplicación cierra
```

---

## Flujo de Autenticación

```
Usuario -> UI (LoginDialog)
    ↓
LoginDialog::onLoginClicked()
    ↓
AuthenticationManager::authenticateUser()
    ↓
├─ SQLiteCipherDB::getUserHash() - Obtiene hash y salt
├─ CryptoManager::verifyPassword() - Verifica contraseña
    ↓
MainWindow (Si autenticación exitosa)
```

## Flujo de Registro (Nuevo Admin)

```
Usuario -> UI (NewUserDialog - Solo en primer inicio)
    ↓
NewUserDialog::onLoginClicked()
    ↓
AuthenticationManager::registerNewUser(..., isMaster=true)
    ↓
├─ SQLiteCipherDB::userExists() - Verifica duplicados
├─ CryptoManager::hashPassword() - Genera hash y salt
├─ SQLiteCipherDB::createUser(..., is_admin=1) - Almacena como admin
    ↓
MainWindow (Se abre directamente después del registro)
```

## Flujo de Registro (Usuario Existente)

```
Usuario -> UI (NewUserDialog desde MainWindow - Futuro)
    ↓
AuthenticationManager::registerNewUser(..., isMaster=false)
    ↓
├─ SQLiteCipherDB::userExists() - Verifica duplicados
├─ CryptoManager::hashPassword() - Genera hash y salt
├─ SQLiteCipherDB::createUser(..., is_admin=0) - Almacena como usuario normal
    ↓
LoginDialog (Usuario puede hacer login después)
```

---

## Dependencias

### Librerías Utilizadas:
- **STL (Standard Template Library):**
  - `<string>` - Manejo de cadenas

### Módulos Internos:
- **CryptoManager** - Para operaciones criptográficas
- **SQLiteCipherDB** - Para acceso a base de datos

---

## Logs y Debugging

El módulo utiliza la función `PrintLog()` para registrar:
- Inicialización del gestor: `"Authentication Manager - initialized"`
- Autenticación en proceso: `"Authentication Manager - authenticating user %s..."`
- Usuario autenticado: `"Authentication Manager - user %s authenticated"`
- Fallo de autenticación: `"Authentication Manager - user %s not authenticated"`
- Usuario ya existe: `"Authentication Manager - user %s already exist"`
- Usuario creado: `"Authentication Manager - user %s created"`
- Fallo al crear usuario: `"Authentication Manager - fails to create user %s"`

Los logs utilizan códigos de color ANSI para mejor visualización en terminal (CYAN para información, GREEN para éxito, RED para errores).

---

## Consideraciones de Seguridad

1. **No Almacenamiento de Contraseñas:** Las contraseñas nunca se almacenan en texto plano
2. **PBKDF2-SHA256:** Utiliza este algoritmo de derivación de clave para mayor seguridad
3. **Salt Único:** Cada usuario tiene su propio salt generado aleatoriamente
4. **Validación de Entrada:** Verifica que los usuarios no se dupliquen antes de crear
5. **Gestión de Recursos:** Libera correctamente los punteros en el destructor

---

## Patrones de Diseño

- **Strategy Pattern:** Delega la criptografía a `CryptoManager` y persistencia a `SQLiteCipherDB`
- **Composition:** Utiliza composición sobre herencia para agregar funcionalidad
- **Const Correctness:** Métodos marcados como `const` cuando no modifican estado

---

## Notas Técnicas

- Utiliza C++17 features como structured binding
- Los métodos que modifican estado son `const` porque no cambian el estado interno del objeto
- Los punteros son `const` para garantizar que no se modifican las dependencias
- Sigue patrón de inyección de dependencias en el constructor
