# 💻 Fase 2 - Plantillas de Código (Code Templates)

Este archivo contiene plantillas listas para usar en tu Fase 2. Cópialas y úsalas como base.

---

## 📄 Template 1: CryptoManager.hpp Completo

```cpp
#ifndef CRYPTOMANAGER_HPP
#define CRYPTOMANAGER_HPP

#include "library.hpp"
#include <string>
#include <vector>
#include <utility>  // para std::pair

// OpenSSL includes
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/err.h>

/**
 * @class CryptoManager
 * @brief Gestiona todas las operaciones criptográficas de la aplicación
 * 
 * Proporciona:
 * - Hashing seguro de contraseñas (PBKDF2)
 * - Verificación de contraseñas
 * - Derivación de claves para cifrado
 * - Generación de números aleatorios
 * 
 * NOTA: Esta clase NO realiza cifrado. Ver Fase 3 para AES-256.
 */
class CryptoManager
{
public:
    // ===== Constructor/Destructor =====
    /**
     * @brief Constructor - Inicializa OpenSSL
     */
    CryptoManager();
    
    /**
     * @brief Destructor
     */
    ~CryptoManager();

    // ===== Password Hashing (PBKDF2-SHA256) =====
    
    /**
     * @brief Genera un hash seguro de una contraseña usando PBKDF2
     * 
     * @param password Contraseña en texto plano
     * @param iterations Número de iteraciones PBKDF2 (default: 100000)
     *                   Mayor = más seguro pero más lento
     * 
     * @return std::pair<hash_hex, salt_hex>
     *         - hash_hex: Hash en formato hexadecimal (64 caracteres para SHA256)
     *         - salt_hex: Salt en formato hexadecimal (32 caracteres para 16 bytes)
     * 
     * @throw std::runtime_error si PBKDF2 falla
     * 
     * @example
     * auto [hash, salt] = crypto.hashPassword("miPassword123");
     * std::cout << "Hash: " << hash << std::endl;
     * std::cout << "Salt: " << salt << std::endl;
     */
    std::pair<std::string, std::string> hashPassword(
        const std::string& password,
        int iterations = 100000
    );

    /**
     * @brief Verifica si una contraseña coincide con su hash almacenado
     * 
     * @param password Contraseña a verificar (texto plano)
     * @param storedHash Hash almacenado en BD (formato hexadecimal)
     * @param salt Salt usado al crear el hash (formato hexadecimal)
     * @param iterations Número de iteraciones usado (debe ser igual al hash)
     * 
     * @return true si contraseña es correcta, false si no
     * 
     * @note Esta función es segura contra timing attacks
     * 
     * @example
     * bool isValid = crypto.verifyPassword("miPassword123", hash, salt);
     * if (isValid) {
     *     std::cout << "Contraseña correcta" << std::endl;
     * }
     */
    bool verifyPassword(
        const std::string& password,
        const std::string& storedHash,
        const std::string& salt,
        int iterations = 100000
    );

    // ===== Key Derivation (para Fase 3) =====
    
    /**
     * @brief Deriva una clave de cifrado AES-256 a partir de contraseña
     * 
     * @param password Contraseña maestra (texto plano)
     * @param salt Salt para la derivación
     * @param iterations Número de iteraciones (default: 100000)
     * 
     * @return Clave de 32 bytes (256 bits) en formato hexadecimal (64 chars)
     * 
     * @note Similar a hashPassword pero diseñado para cifrado, no almacenamiento
     * 
     * @example
     * std::string encryptionKey = crypto.deriveKey(masterPassword, salt);
     * // Usar encryptionKey para AES-256 en Fase 3
     */
    std::string deriveKey(
        const std::string& password,
        const std::string& salt,
        int iterations = 100000
    );

private:
    // ===== Private Utility Functions =====
    
    /**
     * @brief Genera bytes aleatorios criptográficamente seguros
     * 
     * @param length Cantidad de bytes a generar
     * @return std::vector<unsigned char> con bytes aleatorios
     * 
     * @throw std::runtime_error si RAND_bytes falla
     */
    std::vector<unsigned char> generateRandomBytes(size_t length);

    /**
     * @brief Convierte un vector de bytes a string hexadecimal
     * 
     * @param bytes Vector de bytes a convertir
     * @return String en formato hexadecimal (lowercase)
     * 
     * @example
     * bytes: [0x48, 0x65, 0x6C, 0x6C, 0x6F]
     * retorna: "48656c6c6f"
     */
    std::string bytesToHex(const std::vector<unsigned char>& bytes);

    /**
     * @brief Convierte string hexadecimal a vector de bytes
     * 
     * @param hex String en formato hexadecimal (puede ser upper o lowercase)
     * @return std::vector<unsigned char> con los bytes
     * 
     * @throw std::runtime_error si hex inválido
     * 
     * @example
     * hex: "48656c6c6f"
     * retorna: [0x48, 0x65, 0x6C, 0x6C, 0x6F]
     */
    std::vector<unsigned char> hexToBytes(const std::string& hex);
};

#endif // CRYPTOMANAGER_HPP
```

---

## 📄 Template 2: CryptoManager.cpp - Funciones Clave

```cpp
#include "../../include/CryptoManager.hpp"
#include <cstring>   // para snprintf
#include <stdexcept> // para std::runtime_error
#include <cctype>    // para tolower

// ============================================================================
// CONSTRUCTOR & DESTRUCTOR
// ============================================================================

CryptoManager::CryptoManager()
{
    // Inicializar OpenSSL (requerido para RAND_bytes y EVP)
    OpenSSL_add_all_algorithms();
    PrintLog(std::cout, BLUE "CryptoManager" RESET " - Initialized with OpenSSL");
}

CryptoManager::~CryptoManager()
{
    // Cleanup (opcional pero recomendado)
    EVP_cleanup();
}

// ============================================================================
// GENERACIÓN DE BYTES ALEATORIOS
// ============================================================================

std::vector<unsigned char> CryptoManager::generateRandomBytes(size_t length)
{
    std::vector<unsigned char> buffer(length);
    
    // RAND_bytes() rellena 'buffer' con 'length' bytes aleatorios
    // Retorna 1 si éxito, 0 si falla
    if (RAND_bytes(buffer.data(), length) != 1) {
        unsigned long err = ERR_get_error();
        throw std::runtime_error(
            std::string("RAND_bytes failed: ") + 
            std::string(ERR_error_string(err, nullptr))
        );
    }
    
    return buffer;
}

// ============================================================================
// CONVERSIONES HEX <-> BYTES
// ============================================================================

std::string CryptoManager::bytesToHex(const std::vector<unsigned char>& bytes)
{
    std::string hex;
    hex.reserve(bytes.size() * 2);  // Optimización: 1 byte = 2 caracteres hex
    
    for (unsigned char byte : bytes) {
        char buf[3];  // "FF\0"
        snprintf(buf, sizeof(buf), "%02x", byte);
        hex += buf;
    }
    
    return hex;
}

std::vector<unsigned char> CryptoManager::hexToBytes(const std::string& hex)
{
    if (hex.length() % 2 != 0) {
        throw std::runtime_error("hexToBytes: hex string length must be even");
    }
    
    std::vector<unsigned char> bytes;
    bytes.reserve(hex.length() / 2);
    
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteStr = hex.substr(i, 2);
        try {
            unsigned char byte = static_cast<unsigned char>(
                std::stoi(byteStr, nullptr, 16)
            );
            bytes.push_back(byte);
        } catch (...) {
            throw std::runtime_error("hexToBytes: invalid hex character");
        }
    }
    
    return bytes;
}

// ============================================================================
// HASHING DE CONTRASEÑA (PBKDF2-SHA256)
// ============================================================================

std::pair<std::string, std::string> CryptoManager::hashPassword(
    const std::string& password,
    int iterations)
{
    try {
        // 1. Generar salt aleatorio (16 bytes = 128 bits)
        auto salt_bytes = generateRandomBytes(16);
        
        // 2. Aplicar PBKDF2-SHA256
        // OUTPUT_LENGTH = 32 bytes (256 bits para AES-256)
        unsigned char hash[32];
        
        int result = PKCS5_PBKDF2_HMAC(
            password.c_str(),           // contraseña a hashear
            password.length(),          // longitud de contraseña
            salt_bytes.data(),          // salt bytes
            salt_bytes.size(),          // longitud del salt
            iterations,                 // iteraciones (ralentiza ataques)
            EVP_sha256(),               // función hash a usar
            32,                         // longitud de salida (bytes)
            hash                        // buffer de salida
        );
        
        // Verificar que PBKDF2 tuvo éxito
        if (result != 1) {
            throw std::runtime_error("PBKDF2 hashing failed");
        }
        
        // 3. Convertir resultado a hexadecimal para almacenamiento
        std::vector<unsigned char> hash_vec(hash, hash + 32);
        std::string hash_hex = bytesToHex(hash_vec);
        std::string salt_hex = bytesToHex(salt_bytes);
        
        PrintLog(std::cout, GREEN "CryptoManager" RESET " - Password hashed successfully");
        
        return {hash_hex, salt_hex};
        
    } catch (const std::exception& e) {
        PrintLog(std::cout, RED "CryptoManager ERROR" RESET " - " + std::string(e.what()));
        throw;
    }
}

// ============================================================================
// VERIFICACIÓN DE CONTRASEÑA
// ============================================================================

bool CryptoManager::verifyPassword(
    const std::string& password,
    const std::string& storedHash,
    const std::string& salt,
    int iterations)
{
    try {
        // 1. Convertir salt de hex a bytes
        auto salt_bytes = hexToBytes(salt);
        
        // 2. Convertir hash almacenado de hex a bytes
        auto stored_hash_bytes = hexToBytes(storedHash);
        
        // 3. Hashear la contraseña ingresada con el MISMO salt
        unsigned char computed_hash[32];
        
        int result = PKCS5_PBKDF2_HMAC(
            password.c_str(),
            password.length(),
            salt_bytes.data(),
            salt_bytes.size(),
            iterations,
            EVP_sha256(),
            32,
            computed_hash
        );
        
        if (result != 1) {
            return false;
        }
        
        // 4. Comparar bytes directamente (time-constant comparison)
        // CRYPTO_memcmp compara sin optimizaciones que filtraban tiempo
        int comparison = CRYPTO_memcmp(
            computed_hash,              // hash calculado
            stored_hash_bytes.data(),   // hash almacenado
            32                          // longitud (siempre 32 para SHA256)
        );
        
        return (comparison == 0);
        
    } catch (const std::exception& e) {
        PrintLog(std::cout, RED "CryptoManager ERROR" RESET " - " + std::string(e.what()));
        return false;
    }
}

// ============================================================================
// DERIVACIÓN DE CLAVE (para Fase 3 - Cifrado AES)
// ============================================================================

std::string CryptoManager::deriveKey(
    const std::string& password,
    const std::string& salt,
    int iterations)
{
    try {
        // Similar a hashPassword pero:
        // - No genera nuevo salt (usa el existente)
        // - No almacena en BD (se usa para cifrado)
        
        auto salt_bytes = hexToBytes(salt);
        unsigned char derived_key[32];  // 256 bits para AES-256
        
        int result = PKCS5_PBKDF2_HMAC(
            password.c_str(),
            password.length(),
            salt_bytes.data(),
            salt_bytes.size(),
            iterations,
            EVP_sha256(),
            32,
            derived_key
        );
        
        if (result != 1) {
            throw std::runtime_error("Key derivation failed");
        }
        
        std::vector<unsigned char> key_vec(derived_key, derived_key + 32);
        std::string key_hex = bytesToHex(key_vec);
        
        // IMPORTANTE: Limpiar el buffer que contiene la clave
        // Esto es más importante para claves que para hashes
        memset(derived_key, 0, 32);
        
        PrintLog(std::cout, GREEN "CryptoManager" RESET " - Key derived successfully");
        
        return key_hex;
        
    } catch (const std::exception& e) {
        PrintLog(std::cout, RED "CryptoManager ERROR" RESET " - " + std::string(e.what()));
        throw;
    }
}
```

---

## 📄 Template 3: SQLiteCipherDB.hpp

```cpp
#ifndef SQLITECIPHERDB_HPP
#define SQLITECIPHERDB_HPP

#include "library.hpp"
#include <sqlite3.h>
#include <string>
#include <memory>

/**
 * @class SQLiteCipherDB
 * @brief Gestor de base de datos SQLite con cierre seguro
 * 
 * Maneja:
 * - Conexión a BD
 * - Tabla de usuarios (username, password_hash, password_salt)
 * - CRUD operations
 */
class SQLiteCipherDB
{
public:
    // ===== Constructor/Destructor =====
    /**
     * @brief Constructor - Abre/crea la base de datos
     * @param dbPath Ruta al archivo de BD (ej: "~/.pm/passwords.db")
     */
    explicit SQLiteCipherDB(const std::string& dbPath);
    
    /**
     * @brief Destructor - Cierra conexión
     */
    ~SQLiteCipherDB();

    // ===== User Management =====
    
    /**
     * @brief Crea un nuevo usuario en la BD
     * @param username Usuario único
     * @param passwordHash Hash PBKDF2 (hexadecimal)
     * @param passwordSalt Salt usado (hexadecimal)
     * @return true si se creó, false si error (ej: usuario duplicado)
     */
    bool createUser(
        const std::string& username,
        const std::string& passwordHash,
        const std::string& passwordSalt
    );

    /**
     * @brief Obtiene el hash y salt de un usuario
     * @param username Usuario a buscar
     * @param outHash [out] Se llena con el hash almacenado
     * @param outSalt [out] Se llena con el salt almacenado
     * @return true si usuario existe, false si no
     */
    bool getUserHash(
        const std::string& username,
        std::string& outHash,
        std::string& outSalt
    );

    /**
     * @brief Verifica si un usuario existe
     * @param username Usuario a buscar
     * @return true si existe, false si no
     */
    bool userExists(const std::string& username);

    /**
     * @brief Actualiza la contraseña de un usuario existente
     * @param username Usuario
     * @param newHash Nuevo hash PBKDF2
     * @param newSalt Nuevo salt
     * @return true si se actualizó, false si error
     */
    bool updatePassword(
        const std::string& username,
        const std::string& newHash,
        const std::string& newSalt
    );

    /**
     * @brief Elimina un usuario
     * @param username Usuario a eliminar
     * @return true si se eliminó, false si no existe
     */
    bool deleteUser(const std::string& username);

private:
    sqlite3* db;
    std::string dbPath;

    /**
     * @brief Inicializa la tabla de usuarios si no existe
     */
    void initializeDatabase();

    /**
     * @brief Helper para ejecutar SQL sin resultados
     */
    bool executeSQL(const std::string& sql);

    /**
     * @brief Helper para queries que retornan resultados
     */
    sqlite3_stmt* prepareStatement(const std::string& sql);
};

#endif // SQLITECIPHERDB_HPP
```

---

## 📄 Template 4: AuthenticationManager.hpp

```cpp
#ifndef AUTHENTICATIONMANAGER_HPP
#define AUTHENTICATIONMANAGER_HPP

#include "library.hpp"
#include "CryptoManager.hpp"
#include "SQLiteCipherDB.hpp"
#include <string>
#include <memory>

/**
 * @class AuthenticationManager
 * @brief Orquesta la lógica de autenticación
 * 
 * Combina:
 * - CryptoManager (hashing/verificación)
 * - SQLiteCipherDB (persistencia)
 * 
 * Proporciona interfaz simple para login/registro
 */
class AuthenticationManager
{
public:
    // ===== Constructor/Destructor =====
    /**
     * @brief Constructor
     * @param database Referencia a la BD de usuarios
     */
    explicit AuthenticationManager(SQLiteCipherDB& database);
    
    /**
     * @brief Destructor
     */
    ~AuthenticationManager();

    // ===== Authentication =====
    
    /**
     * @brief Autentica un usuario con su contraseña
     * 
     * @param username Usuario
     * @param password Contraseña en texto plano
     * 
     * @return true si credenciales son correctas
     * @return false si usuario no existe o contraseña es incorrecta
     * 
     * @note NO diferencia entre "usuario no existe" y "contraseña incorrecta"
     *       por razones de seguridad (user enumeration)
     * 
     * @example
     * AuthenticationManager auth(database);
     * if (auth.authenticateUser("admin", "password123")) {
     *     std::cout << "Login successful" << std::endl;
     * } else {
     *     std::cout << "Invalid credentials" << std::endl;
     * }
     */
    bool authenticateUser(
        const std::string& username,
        const std::string& password
    );

    /**
     * @brief Registra un nuevo usuario
     * 
     * @param username Usuario nuevo (debe ser único)
     * @param password Contraseña en texto plano
     * 
     * @return true si se registró exitosamente
     * @return false si error (ej: usuario ya existe)
     * 
     * @note Después del registro, el usuario puede hacer login
     * 
     * @example
     * if (auth.registerNewUser("john", "password123")) {
     *     std::cout << "User registered" << std::endl;
     * } else {
     *     std::cout << "Registration failed" << std::endl;
     * }
     */
    bool registerNewUser(
        const std::string& username,
        const std::string& password
    );

    /**
     * @brief Cambia la contraseña de un usuario autenticado
     * 
     * @param username Usuario (debe existir)
     * @param oldPassword Contraseña actual (para verificación)
     * @param newPassword Nueva contraseña
     * 
     * @return true si se cambió exitosamente
     * @return false si error (vieja contraseña incorrecta, etc)
     */
    bool changePassword(
        const std::string& username,
        const std::string& oldPassword,
        const std::string& newPassword
    );

private:
    CryptoManager crypto;
    SQLiteCipherDB& database;  // Referencia a BD compartida
};

#endif // AUTHENTICATIONMANAGER_HPP
```

---

## 📄 Template 5: AuthenticationManager.cpp - Lógica Clave

```cpp
#include "../../include/AuthenticationManager.hpp"

AuthenticationManager::AuthenticationManager(SQLiteCipherDB& database)
    : database(database)
{
    PrintLog(std::cout, BLUE "AuthenticationManager" RESET " - Initialized");
}

AuthenticationManager::~AuthenticationManager()
{
}

bool AuthenticationManager::authenticateUser(
    const std::string& username,
    const std::string& password)
{
    try {
        // 1. Buscar usuario en BD
        std::string storedHash, storedSalt;
        if (!database.getUserHash(username, storedHash, storedSalt)) {
            // Usuario no existe
            PrintLog(std::cout, YELLOW "Auth" RESET " - User not found: " + username);
            return false;
        }

        // 2. Verificar contraseña
        bool isValid = crypto.verifyPassword(
            password,
            storedHash,
            storedSalt,
            100000  // debe coincidir con iteraciones usadas al crear
        );

        if (isValid) {
            PrintLog(std::cout, GREEN "Auth" RESET " - Authentication successful for: " + username);
        } else {
            PrintLog(std::cout, YELLOW "Auth" RESET " - Authentication failed for: " + username);
        }

        return isValid;

    } catch (const std::exception& e) {
        PrintLog(std::cout, RED "Auth ERROR" RESET " - " + std::string(e.what()));
        return false;
    }
}

bool AuthenticationManager::registerNewUser(
    const std::string& username,
    const std::string& password)
{
    try {
        // 1. Verificar que usuario no exista
        if (database.userExists(username)) {
            PrintLog(std::cout, YELLOW "Auth" RESET " - User already exists: " + username);
            return false;
        }

        // 2. Validar contraseña (opcional pero recomendado)
        if (password.length() < 6) {
            PrintLog(std::cout, YELLOW "Auth" RESET " - Password too short");
            return false;
        }

        // 3. Hashear contraseña
        auto [hash, salt] = crypto.hashPassword(password);

        // 4. Guardar en BD
        bool success = database.createUser(username, hash, salt);

        if (success) {
            PrintLog(std::cout, GREEN "Auth" RESET " - User registered: " + username);
        } else {
            PrintLog(std::cout, RED "Auth ERROR" RESET " - Failed to create user in DB");
        }

        return success;

    } catch (const std::exception& e) {
        PrintLog(std::cout, RED "Auth ERROR" RESET " - " + std::string(e.what()));
        return false;
    }
}

bool AuthenticationManager::changePassword(
    const std::string& username,
    const std::string& oldPassword,
    const std::string& newPassword)
{
    try {
        // 1. Autenticar con contraseña vieja
        if (!authenticateUser(username, oldPassword)) {
            PrintLog(std::cout, YELLOW "Auth" RESET " - Old password incorrect for: " + username);
            return false;
        }

        // 2. Validar nueva contraseña
        if (newPassword.length() < 6) {
            PrintLog(std::cout, YELLOW "Auth" RESET " - New password too short");
            return false;
        }

        // 3. Hashear nueva contraseña
        auto [hash, salt] = crypto.hashPassword(newPassword);

        // 4. Actualizar en BD
        bool success = database.updatePassword(username, hash, salt);

        if (success) {
            PrintLog(std::cout, GREEN "Auth" RESET " - Password changed for: " + username);
        }

        return success;

    } catch (const std::exception& e) {
        PrintLog(std::cout, RED "Auth ERROR" RESET " - " + std::string(e.what()));
        return false;
    }
}
```

---

## 📄 Template 6: Actualizar LoginDialog.cpp

```cpp
// En el constructor de LoginDialog, agregar:

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Login Password Manager");
    PrintLog(std::cout, YELLOW "Login Dialog" RESET " - Initializing UI...");    
    setupUi();
    
    PrintLog(std::cout, YELLOW "Login Dialog" RESET " - Establishing button connections...");    
    connect(loginBttn, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(cancelBttn, &QPushButton::clicked, this, &LoginDialog::onCancelClicked);
}

// En onLoginClicked(), cambiar:

void LoginDialog::onLoginClicked()
{
    QString userQt = userEdit->text();
    QString passQt = passEdit->text();

    // Convertir de QString a std::string
    std::string username = userQt.toStdString();
    std::string password = passQt.toStdString();

    // TODO: Obtener referencia a BD (pasa como parámetro o miembro)
    // SQLiteCipherDB& database = ...
    
    // Autenticar
    AuthenticationManager auth(database);
    if (auth.authenticateUser(username, password)) {
        accept();  // Cierra dialog y retorna QDialog::Accepted
    } else {
        QMessageBox::warning(
            this,
            "Error",
            "Usuario o contraseña incorrectos"
        );
        
        // Limpiar campos
        userEdit->clear();
        passEdit->clear();
        userEdit->setFocus();
    }
}
```

---

## 🔑 Parámetros Recomendados PBKDF2

```cpp
// ITERACIONES: Número de veces que se aplica la función hash
// Más iteraciones = más seguro pero más lento

2024 Recomendaciones:
- Interactivo (login): 100,000 - 500,000
- Derivación de claves: 100,000
- Almacenamiento de claves maestras: 1,000,000+

Nuestro proyecto (Fase 2):
- Usar: 100,000 iteraciones
- En Fase 5 (refactor), considerar aumentar a 500,000

// SALT LENGTH:
- Mínimo recomendado: 128 bits (16 bytes)
- Nuestro proyecto: 16 bytes ✅

// OUTPUT LENGTH:
- Para AES-256: 32 bytes (256 bits)
- Para AES-128: 16 bytes (128 bits)
- Nuestro proyecto: 32 bytes ✅

// FUNCIÓN HASH:
- SHA-256 es segura (estándar NIST)
- SHA-512 es aún más segura pero más lenta
- Nuestro proyecto: SHA-256 ✅
```

---

## 🐛 Códigos de Error Comunes OpenSSL

```cpp
// Si RAND_bytes falla:
// Causas: No hay suficiente entropía, problema del SO
// Solución: Usar /dev/urandom en Linux

// Si PKCS5_PBKDF2_HMAC retorna 0:
// Causas: Parámetros inválidos, memoria insuficiente
// Solución: Verificar parámetros, aumentar memoria

// Si ERR_get_error() retorna error:
char error_str[256];
ERR_error_string_n(ERR_get_error(), error_str, sizeof(error_str));
std::cerr << "OpenSSL Error: " << error_str << std::endl;
```

---

## 📖 Recursos Útiles

- OpenSSL Documentation: https://www.openssl.org/docs/
- PBKDF2 RFC: https://tools.ietf.org/html/rfc2898
- SQLite C API: https://www.sqlite.org/c3ref/intro.html
- OWASP Password Storage: https://cheatsheetseries.owasp.org/cheatsheets/Password_Storage_Cheat_Sheet.html

¡Usa estos templates como punto de partida! Personalízalos según tu estilo de código. 🚀
