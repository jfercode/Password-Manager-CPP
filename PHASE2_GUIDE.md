# 🎓 Fase 2 - Guía de Desarrollo: Autenticación y Seguridad Base

## 📍 Resumen del Estado Actual

Felicidades, has completado **Fase 1** exitosamente:
- ✅ Proyecto compilable
- ✅ Interfaz UI básica (LoginDialog, MainWindow)
- ✅ CMake configurado con Qt5 y OpenSSL
- ✅ Estructura modular correcta

**AHORA ESTÁS EN FASE 2**, donde implementaremos:
1. **Sistema de autenticación seguro**
2. **Hash de contraseña maestra (PBKDF2)**
3. **Manejo seguro de memoria sensible**
4. **Validación de login con seguridad real**

---

## 🎯 Objetivos de Fase 2

| Objetivo | Descripción | Prioridad |
|----------|-------------|-----------|
| Hash seguro | PBKDF2 para la contraseña maestra | 🔴 CRÍTICA |
| Almacenamiento | Guardar hash en archivo/BD | 🔴 CRÍTICA |
| Validación | Comparar contraseñas correctamente | 🔴 CRÍTICA |
| Seguridad de memoria | Limpiar datos sensibles | 🟠 ALTA |

---

## 📚 Conceptos Clave para Estudiar

### 1. **¿Por qué NO usar contraseña en texto plano?**
```
❌ MALO:
  contraseña = "1234"
  guardar en archivo: "1234"
  
✅ BIEN:
  hash(contraseña) = "a1b2c3d4e5f6..."
  guardar en archivo: "a1b2c3d4e5f6..."
  
Cuando el usuario ingresa "1234":
  hash(1234) == almacenado → ✅ Login OK
```

### 2. **PBKDF2 vs otros métodos**
```
PBKDF2:
  - Password-Based Key Derivation Function 2
  - Estándar NIST
  - Iteraciones configurables
  - Disponible en OpenSSL
  - Buena relación seguridad/complejidad

Otras opciones (más fuertes pero complejas):
  - bcrypt (mejor para aplicaciones web)
  - Argon2 (moderna, requiere librería externa)
```

### 3. **Salt (sal criptográfica)**
```
❌ MALO:
  hash("1234") = "a1b2c3d4..."  ← Determinístico
  
✅ BIEN:
  salt = generar_random_16bytes()
  hash("1234" + salt) = "x9y8z7w6..."
  almacenar: (hash, salt)
  
Beneficio: Dos usuarios con "1234" tendrán hashes diferentes
```

---

## 🛠️ Plan de Implementación Paso a Paso

### PASO 1: Mejorar CryptoManager.hpp
**Objetivo**: Definir la interfaz para operaciones criptográficas

Tu `CryptoManager.hpp` está vacío. Necesitas:
- Función para hashear contraseña con PBKDF2
- Función para verificar contraseña
- Función para generar salt aleatorio
- Manejo de errores

**Archivo a editar**: `include/CryptoManager.hpp`

```cpp
#ifndef CRYPTOMANAGER_HPP
#define CRYPTOMANAGER_HPP

#include "library.hpp"
#include <string>
#include <vector>

class CryptoManager
{
public:
    // ===== Constructor/Destructor =====
    CryptoManager();
    ~CryptoManager();

    // ===== Password Hashing (PBKDF2) =====
    /**
     * @brief Genera un hash seguro de una contraseña
     * @param password Contraseña en texto plano
     * @param iterations Número de iteraciones PBKDF2 (por defecto 100000)
     * @return Pair<hash_hex, salt_hex> - ambos en formato hexadecimal
     */
    std::pair<std::string, std::string> hashPassword(
        const std::string& password,
        int iterations = 100000
    );

    /**
     * @brief Verifica si una contraseña coincide con su hash
     * @param password Contraseña a verificar (texto plano)
     * @param storedHash Hash almacenado (formato hexadecimal)
     * @param salt Salt usado (formato hexadecimal)
     * @param iterations Número de iteraciones usado
     * @return true si coinciden, false si no
     */
    bool verifyPassword(
        const std::string& password,
        const std::string& storedHash,
        const std::string& salt,
        int iterations = 100000
    );

    // ===== Key Derivation =====
    /**
     * @brief Deriva una clave de cifrado a partir de contraseña
     * @param password Contraseña maestra (texto plano)
     * @param salt Salt para KDF
     * @return Clave de 32 bytes para AES-256 (en formato hexadecimal)
     */
    std::string deriveKey(
        const std::string& password,
        const std::string& salt,
        int iterations = 100000
    );

private:
    // ===== Utilidades privadas =====
    /**
     * @brief Genera random bytes
     * @param length Cantidad de bytes a generar
     * @return Vector de bytes aleatorios
     */
    std::vector<unsigned char> generateRandomBytes(size_t length);

    /**
     * @brief Convierte bytes a string hexadecimal
     * @param bytes Vector de bytes
     * @return String en formato hexadecimal (lowercase)
     */
    std::string bytesToHex(const std::vector<unsigned char>& bytes);

    /**
     * @brief Convierte string hexadecimal a bytes
     * @param hex String en formato hexadecimal
     * @return Vector de bytes
     */
    std::vector<unsigned char> hexToBytes(const std::string& hex);
};

#endif
```

### PASO 2: Implementar CryptoManager.cpp
**Archivo**: `src/crypto/CryptoManager.cpp`

Aquí usarás OpenSSL para implementar PBKDF2.

### PASO 3: AuthenticationManager para lógica de negocio
**Archivo**: `include/AuthenticationManager.hpp`

Este manager maneja:
- Verificación de usuario en base de datos
- Creación de nuevos usuarios
- Cambio de contraseña

```cpp
class AuthenticationManager
{
public:
    AuthenticationManager(/* database connection */);
    ~AuthenticationManager();

    /**
     * @brief Autentica un usuario con su contraseña
     * @return true si credenciales correctas
     */
    bool authenticateUser(const std::string& username, const std::string& password);

    /**
     * @brief Registra un nuevo usuario
     * @return true si se registró exitosamente
     */
    bool registerNewUser(const std::string& username, const std::string& password);

private:
    CryptoManager crypto;
    // database reference
};
```

### PASO 4: LoginDialog vinculado con AuthenticationManager
**Archivo**: `src/ui/LoginDialog.cpp`

Cambiar de:
```cpp
if (user == "admin" && pass == "1234")  // ❌ Hardcoded
```

A:
```cpp
AuthenticationManager auth;  // ✅ Sistema real
if (auth.authenticateUser(user.toStdString(), pass.toStdString()))
```

### PASO 5: SQLiteCipherDB para persistencia
**Objetivo**: Guardar hashes de usuarios

Estructura de tabla:
```sql
CREATE TABLE users (
    id INTEGER PRIMARY KEY,
    username TEXT UNIQUE NOT NULL,
    password_hash TEXT NOT NULL,
    password_salt TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

---

## 📋 Tareas en Orden de Prioridad

### Tarea 1: Implement CryptoManager Header
**Dificultad**: ⭐⭐⭐⭐
**Tiempo estimado**: 30 minutos

**Qué hacer**:
1. Abre `include/CryptoManager.hpp`
2. Reemplaza el contenido con la definición de clase mejorada (ver PASO 1 arriba)
3. Incluye todos los headers necesarios de OpenSSL

**Headers OpenSSL necesarios**:
```cpp
#include <openssl/rand.h>    // Para generación de random
#include <openssl/evp.h>     // Para PBKDF2
#include <openssl/err.h>     // Para manejo de errores
```

**Puntos de aprendizaje**:
- Interfaz clara y documentada
- Separación entre hash (seguridad) y derivación de claves (cifrado)
- Métodos privados para utilidades

---

### Tarea 2: Implement CryptoManager.cpp
**Dificultad**: ⭐⭐⭐⭐⭐
**Tiempo estimado**: 2 horas

**Qué implementar**:

```cpp
// Constructor
CryptoManager::CryptoManager() {
    OpenSSL_add_all_algorithms();  // Inicializar OpenSSL
}

// Generar bytes aleatorios
std::vector<unsigned char> CryptoManager::generateRandomBytes(size_t length) {
    std::vector<unsigned char> buffer(length);
    if (RAND_bytes(buffer.data(), length) != 1) {
        throw std::runtime_error("RAND_bytes failed");
    }
    return buffer;
}

// Convertir a hexadecimal
std::string CryptoManager::bytesToHex(const std::vector<unsigned char>& bytes) {
    std::string hex;
    for (unsigned char byte : bytes) {
        char buf[3];
        snprintf(buf, sizeof(buf), "%02x", byte);
        hex += buf;
    }
    return hex;
}

// PBKDF2 Hashing
std::pair<std::string, std::string> CryptoManager::hashPassword(
    const std::string& password,
    int iterations) 
{
    // 1. Generar salt aleatorio (16 bytes)
    auto salt_bytes = generateRandomBytes(16);
    
    // 2. Aplicar PBKDF2-SHA256
    // EVP_BytesToKey o PKCS5_PBKDF2_HMAC es el equivalente moderno
    unsigned char hash[32];  // SHA256 = 32 bytes
    
    int success = PKCS5_PBKDF2_HMAC(
        password.c_str(),           // entrada
        password.length(),
        salt_bytes.data(),          // salt
        salt_bytes.size(),
        iterations,
        EVP_sha256(),               // función hash
        32,                         // longitud de salida
        hash                        // buffer de salida
    );
    
    if (success != 1) {
        throw std::runtime_error("PBKDF2 hashing failed");
    }
    
    // 3. Convertir a hexadecimal
    std::vector<unsigned char> hash_vec(hash, hash + 32);
    std::string hash_hex = bytesToHex(hash_vec);
    std::string salt_hex = bytesToHex(salt_bytes);
    
    return {hash_hex, salt_hex};
}

// Verificar contraseña
bool CryptoManager::verifyPassword(
    const std::string& password,
    const std::string& storedHash,
    const std::string& salt,
    int iterations)
{
    try {
        // 1. Convertir salt de hex a bytes
        auto salt_bytes = hexToBytes(salt);
        
        // 2. Hashear la contraseña ingresada con el salt almacenado
        unsigned char computed_hash[32];
        int success = PKCS5_PBKDF2_HMAC(
            password.c_str(),
            password.length(),
            salt_bytes.data(),
            salt_bytes.size(),
            iterations,
            EVP_sha256(),
            32,
            computed_hash
        );
        
        if (success != 1) return false;
        
        // 3. Comparar con el hash almacenado
        std::vector<unsigned char> computed_vec(computed_hash, computed_hash + 32);
        std::string computed_hex = bytesToHex(computed_vec);
        
        // Usar comparación segura (time-constant)
        return computed_hex == storedHash;
        
    } catch (...) {
        return false;
    }
}
```

**Puntos de aprendizaje**:
- Cómo usar OpenSSL PBKDF2
- Generación segura de números aleatorios
- Conversiones hex/binario
- Manejo de errores de OpenSSL

---

### Tarea 3: Mejorar SQLiteCipherDB
**Dificultad**: ⭐⭐⭐
**Tiempo estimado**: 1.5 horas

**Qué hacer**:
1. Definir estructura de tabla de usuarios
2. Métodos CRUD para usuarios:
   ```cpp
   bool createUser(const std::string& username, const std::string& passwordHash, const std::string& salt);
   bool getUserHash(const std::string& username, std::string& hash, std::string& salt);
   bool userExists(const std::string& username);
   ```
3. Conectar la BD al iniciar la aplicación

---

### Tarea 4: Implementar AuthenticationManager
**Dificultad**: ⭐⭐⭐⭐
**Tiempo estimado**: 1 hora

```cpp
class AuthenticationManager {
private:
    CryptoManager crypto;
    SQLiteCipherDB& database;
    
public:
    bool authenticateUser(const std::string& username, const std::string& password) {
        // 1. Buscar usuario en BD
        std::string stored_hash, stored_salt;
        if (!database.getUserHash(username, stored_hash, stored_salt)) {
            return false;  // Usuario no existe
        }
        
        // 2. Verificar contraseña
        return crypto.verifyPassword(password, stored_hash, stored_salt);
    }
    
    bool registerNewUser(const std::string& username, const std::string& password) {
        // 1. Verificar que no exista
        if (database.userExists(username)) {
            return false;
        }
        
        // 2. Hashear contraseña
        auto [hash, salt] = crypto.hashPassword(password);
        
        // 3. Guardar en BD
        return database.createUser(username, hash, salt);
    }
};
```

---

### Tarea 5: Actualizar LoginDialog
**Dificultad**: ⭐⭐
**Tiempo estimado**: 30 minutos

Cambiar:
```cpp
// ANTES:
if (user == "admin" && pass == "1234")
    accept();

// DESPUÉS:
AuthenticationManager auth(database);  // Pasar referencia a BD
if (auth.authenticateUser(user.toStdString(), pass.toStdString())) {
    accept();
} else {
    QMessageBox::warning(this, "Error", "Usuario o contraseña incorrectos");
}
```

También agregar opción para nuevo usuario (NewUserDialog).

---

## 🔒 Mejores Prácticas de Seguridad en Fase 2

### 1. **Limpiar memoria sensible**
```cpp
// ANTES: ❌
std::string password = userInput;  // Queda en memoria

// DESPUÉS: ✅ (usar después de PBKDF2)
memset(password.data(), 0, password.length());  // Sobrescribir
// O mejor aún: usar clases especializadas
```

### 2. **No loguear credenciales**
```cpp
// ❌ NUNCA HACER:
PrintLog(std::cout, "Usuario login: " + username);  // OK
PrintLog(std::cout, "Contraseña: " + password);     // ❌ CRÍTICO

// ✅ BIEN:
PrintLog(std::cout, "Usuario login intentado para: " + username);
PrintLog(std::cout, "Autenticación completada");
```

### 3. **Usar comparación time-constant**
```cpp
// ❌ VULNERABLE A TIMING ATTACKS:
if (computed_hash == stored_hash) { }

// ✅ MEJOR (aunque en C++ es difícil sin librería):
// OpenSSL proporciona: CRYPTO_memcmp
if (CRYPTO_memcmp(computed, stored, 32) == 0) { }
```

---

## 🧪 Cómo Probar Fase 2

### Test Manual 1: Crear Usuario
1. Ejecuta aplicación
2. Login → Mostrar error (usuario no existe)
3. Opción "Nuevo usuario" → crear "admin" / "password123"
4. Verificar que se guarda en BD

### Test Manual 2: Login Correcto
1. Ingresar "admin" / "password123" → ✅ Login OK
2. Abrir MainWindow

### Test Manual 3: Login Incorrecto
1. Ingresar "admin" / "wrong" → ❌ Error
2. Ingresar "wrong_user" / "password123" → ❌ Error

### Test Manual 4: Verificar almacenamiento
1. Abre la BD directamente con sqlite3
2. Verifica que `password_hash` NO sea texto plano
3. Verifica que exista `password_salt`

---

## 📚 Recursos Útiles

### OpenSSL PBKDF2 Documentation
```bash
man PKCS5_PBKDF2_HMAC
# o en línea: https://www.openssl.org/docs/man1.1.1/man3/PKCS5_PBKDF2_HMAC.html
```

### Parámetros PBKDF2 Recomendados
```cpp
// Seguridad básica (2024)
iterations: 100,000 a 500,000
hash_function: SHA-256
salt_length: 16 bytes (128 bits)
output_length: 32 bytes (256 bits) para AES-256
```

### Ejemplo de Test de PBKDF2
```cpp
// Hash conocido (para verificar implementación)
password: "test"
salt: "0102030405060708"
iterations: 1000
esperado: "d1daa78615f287e6a1c8375f9c11efc0"
```

---

## ❓ Preguntas de Auto-Evaluación

Responde estas preguntas para verificar que entiendes Fase 2:

1. ¿Por qué es importante usar un salt en PBKDF2?
2. ¿Cuál es la diferencia entre hashPassword() y deriveKey()?
3. ¿Qué sucede si un atacante roba tu base de datos?
4. ¿Cómo evitas ataques de timing en comparación de contraseñas?
5. ¿Qué pasa si cambias iterations de 100,000 a 1,000?

**Respuestas correctas te permiten pasar a Fase 3**

---

## 🚀 Próximas Fases (Preview)

Después de completar Fase 2:
- **Fase 3**: AES-256 para cifrar credenciales almacenadas
- **Fase 4**: CRUD completo de passwords
- **Fase 5**: Refactor y mejora de código
- **Fase 6**: Features extras (generador, auto-lock, tests)

---

## 📝 Notas Finales

- 🎯 **Mantén el código modular**: Cada clase debe tener una responsabilidad
- 🔐 **La seguridad no es negociable**: Usa librerías probadas, no criptografía casera
- 📚 **Documenta tu código**: Especialmente funciones criptográficas
- ✅ **Prueba constantemente**: Compila frecuentemente

¡Éxito en Fase 2! 🚀
