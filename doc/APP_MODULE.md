# Módulo APP - Gestor de Autenticación

## Descripción General
El módulo `app/` contiene la lógica de autenticación y gestión de usuarios del sistema de Password Manager. Es responsable de coordinar las operaciones de login y registro, actuando como intermediario entre la interfaz gráfica (UI) y los servicios de base de datos y criptografía.

---

## Archivos Incluidos

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

2. **`registerNewUser(const std::string &username, const std::string &password) const`**
   - **Propósito:** Registra un nuevo usuario en el sistema
   - **Parámetros:**
     - `username`: Nombre de usuario para la nueva cuenta
     - `password`: Contraseña en texto plano a proteger
   - **Retorno:** `bool` - `true` si el registro es exitoso, `false` en caso contrario
   - **Proceso:**
     1. Verifica que el usuario no exista ya en la base de datos
     2. Genera hash y salt usando CryptoManager
     3. Crea el usuario en la base de datos con los datos criptografrados
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
- Llama a `SQLiteCipherDB::createUser()` para persistir los datos
- Registra logs detallados del proceso

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

## Flujo de Registro

```
Usuario -> UI (NewUserDialog)
    ↓
AuthenticationManager::registerNewUser()
    ↓
├─ SQLiteCipherDB::userExists() - Verifica duplicados
├─ CryptoManager::hashPassword() - Genera hash y salt
├─ SQLiteCipherDB::createUser() - Almacena usuario
    ↓
LoginDialog (Usuario creado, puede hacer login)
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
