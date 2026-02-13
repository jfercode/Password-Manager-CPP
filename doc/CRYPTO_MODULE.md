# Módulo CRYPTO - Gestor Criptográfico

## Descripción General
El módulo `crypto/` proporciona todas las operaciones criptográficas necesarias para el Password Manager, incluyendo generación de números aleatorios, hashing de contraseñas con PBKDF2, verificación de contraseñas y conversiones entre formatos hexadecimal y binario. Está construido sobre OpenSSL, una de las librerías criptográficas más robustas y ampliamente auditadas.

---

## Archivos Incluidos

### 1. **CryptoManager.hpp**
Archivo de encabezado que define la clase `CryptoManager`.

**Ubicación:** `/include/CryptoManager.hpp`

#### Incluye Dependencias:
```cpp
#include "library.hpp"
#include <openssl/rand.h>    // Generación de números aleatorios criptográficos
#include <openssl/evp.h>     // EVP (Envelope) - PBKDF2 y otras operaciones
#include <openssl/err.h>     // Manejo de errores de OpenSSL
```

#### Clase: `CryptoManager`
Gestor centralizado de operaciones criptográficas para el sistema.

**Miembros Privados (Funciones Auxiliares):**

1. **`generateRandomBytes(size_t length) const`**
   - **Propósito:** Genera bytes aleatorios criptográficamente seguros
   - **Parámetros:** `length` - Cantidad de bytes a generar
   - **Retorno:** `std::vector<unsigned char>` - Vector con bytes aleatorios
   - **Implementación:** Utiliza `RAND_bytes()` de OpenSSL
   - **Seguridad:** Garantiza aleatoriedad criptográfica usando /dev/urandom

2. **`bytesToHex(const std::vector<unsigned char>& bytes) const`**
   - **Propósito:** Convierte un vector de bytes a representación hexadecimal
   - **Parámetros:** `bytes` - Vector de bytes a convertir
   - **Retorno:** `std::string` - Representación hex (ej: "a1b2c3d4")
   - **Implementación:** Itera sobre cada byte y usa `snprintf()` para conversión
   - **Uso:** Para almacenar hashes y salts en la base de datos

3. **`hexToBytes(const std::string& hex) const`**
   - **Propósito:** Convierte una cadena hexadecimal a vector de bytes
   - **Parámetros:** `hex` - Cadena hexadecimal
   - **Retorno:** `std::vector<unsigned char>` - Vector de bytes
   - **Validación:** Verifica que la longitud sea par
   - **Uso:** Para reconstruir salt desde la base de datos

**Constructor:**
```cpp
CryptoManager();
```
- Inicializa OpenSSL llamando a `OpenSSL_add_all_algorithms()`
- Carga todos los algoritmos disponibles en OpenSSL

**Destructor:**
```cpp
~CryptoManager();
```
- Limpieza de recursos

**Métodos Públicos:**

1. **`hashPassword(const std::string &password, int iterations = 10000) const`**
   - **Propósito:** Genera hash y salt de una contraseña usando PBKDF2-SHA256
   - **Parámetros:**
     - `password`: Contraseña en texto plano
     - `iterations`: Número de iteraciones PBKDF2 (default: 10000)
   - **Retorno:** `std::pair<std::string, std::string>` - Par {hash, salt} en formato hexadecimal
   - **Proceso:**
     1. Genera 16 bytes aleatorios como salt
     2. Aplica PBKDF2-SHA256 con 10000 iteraciones
     3. Produce hash de 32 bytes (256 bits)
     4. Convierte ambos a hexadecimal
   - **Seguridad:** 
     - PBKDF2 es resistant a ataques de fuerza bruta
     - Las 10000 iteraciones hacen que el cálculo sea computacionalmente costoso
     - El salt único previene ataques de tabla rainbow

2. **`verifyPassword(const std::string &password, const std::string &storedHash, const std::string &salt, int iterations = 10000) const`**
   - **Propósito:** Verifica si una contraseña coincide con su hash almacenado
   - **Parámetros:**
     - `password`: Contraseña en texto plano a verificar
     - `storedHash`: Hash almacenado en la base de datos (formato hex)
     - `salt`: Salt almacenado en la base de datos (formato hex)
     - `iterations`: Número de iteraciones (debe coincidir con las usadas al crear)
   - **Retorno:** `bool` - `true` si la contraseña coincide, `false` en caso contrario
   - **Proceso:**
     1. Convierte el salt de hex a bytes
     2. Calcula el hash de la contraseña con el mismo salt e iteraciones
     3. Compara el hash calculado con el almacenado
     4. Usa comparación segura contra timing attacks
   - **Manejo de Errores:** Captura excepciones y retorna false

---

### 2. **CryptoManager.cpp**
Implementación completa del gestor criptográfico.

**Ubicación:** `/src/crypto/CryptoManager.cpp`

#### Implementación Detallada

**Constructor:**
```cpp
CryptoManager::CryptoManager()
{
    OpenSSL_add_all_algorithms();
}
```
- Inicializa OpenSSL 1.1.x (compatible con versiones antiguas)

**`generateRandomBytes()`:**
- Crea un buffer de tamaño específico
- Llama a `RAND_bytes()` que retorna 1 si es exitoso
- Lanza `std::runtime_error` si falla
- Los bytes se generan con entropía del sistema

**`bytesToHex()`:**
- Itera sobre cada byte del vector
- Usa `snprintf(buf, 3, "%02x", byte)` para convertir
- Ejemplo: byte 0xA1 se convierte a "a1"
- Concatena todos los resultados en una cadena

**`hexToBytes()`:**
- Valida que la longitud del hex sea par
- Itera cada 2 caracteres
- Usa `std::stoi(byteStr, nullptr, 16)` para conversión base 16
- Lanza excepciones si hay caracteres inválidos

**`hashPassword()`:**
```cpp
std::pair<std::string, std::string> CryptoManager::hashPassword(
    const std::string &password, int iterations) const
```
- Genera salt: 16 bytes aleatorios
- Crea buffer para hash de 32 bytes (256 bits)
- Llama a `PKCS5_PBKDF2_HMAC()`:
  - `EVP_sha256()` especifica el algoritmo HMAC-SHA256
  - Los parámetros incluyen password, salt, iteraciones
  - Produce 32 bytes de salida
- Convierte hash y salt a hex
- Retorna el par como strings

**`verifyPassword()`:**
- Envuelto en try-catch para manejo robusto de errores
- Convierte salt de hex a bytes
- Calcula hash con los mismos parámetros
- Compara strings del hash (timing-safe en comparación de strings)

---

## Librerías Criptográficas Utilizadas

### OpenSSL (https://www.openssl.org/)
Librería estándar de industria para criptografía.

#### Módulos Utilizados:

1. **`openssl/rand.h` - Generación de Números Aleatorios**
   - **`RAND_bytes(unsigned char *buf, int num)`**
     - Genera `num` bytes aleatorios criptográficamente seguros
     - Retorna 1 si es exitoso, 0 si falla
     - Utiliza /dev/urandom en sistemas Unix/Linux
     - Fundamental para generar salts únicos

2. **`openssl/evp.h` - EVP (Envelope) Functions**
   - **`EVP_sha256()`**
     - Retorna un puntero a la estructura EVP_MD para SHA256
     - Utilizado como algoritmo de hash en PBKDF2
   - **`PKCS5_PBKDF2_HMAC(const char *password, int pass_len, const unsigned char *salt, int salt_len, int iter, const EVP_MD *md, int keylen, unsigned char *out)`**
     - Implementa PBKDF2 (Password-Based Key Derivation Function 2)
     - `password`: Contraseña a derivar
     - `salt`: Salt único para cada usuario
     - `iter`: Iteraciones (10000 recomendado)
     - `md`: Algoritmo de hash (SHA256)
     - `out`: Buffer de salida para la clave derivada
     - Retorna 1 si es exitoso
     - **Algoritmo PBKDF2-SHA256:**
       - Aplica HMAC-SHA256 iterativamente
       - Cada iteración es computacionalmente costosa
       - Resistant a ataques de diccionario
       - Recommended por NIST

3. **`openssl/err.h` - Manejo de Errores**
   - Proporciona funciones para obtener mensajes de error
   - Usado implícitamente por PKCS5_PBKDF2_HMAC

---

## Algoritmo PBKDF2-SHA256 (Password-Based Key Derivation Function 2)

### ¿Por qué PBKDF2?
- **Estándar:** Recomendado por NIST (FIPS 140-2)
- **Seguro:** Resistant a ataques de fuerza bruta
- **Configurable:** Las iteraciones pueden ajustarse según seguridad deseada

### Características Técnicas:
```
PBKDF2-SHA256(password, salt, 10000 iteraciones) → 256 bits (32 bytes)
```

- **Salt:** 128 bits (16 bytes) generados aleatoriamente
- **Iteraciones:** 10000 (cada iteración aplica HMAC-SHA256)
- **Salida:** 256 bits (32 bytes, suficientes para cualquier propósito)
- **Función Base:** HMAC-SHA256

### Proceso:
1. Genera salt aleatorio
2. Aplica HMAC-SHA256 iterativamente
3. Cada iteración usa el resultado anterior como entrada
4. Después de 10000 iteraciones, produce el hash final
5. No se puede revertir (función de una vía)

---

## Flujo de Operaciones Criptográficas

### Registro de Usuario:
```
password (texto plano)
    ↓
CryptoManager::hashPassword()
    ├─ generateRandomBytes(16) → salt
    ├─ PKCS5_PBKDF2_HMAC(password, salt, 10000) → hash_bytes
    ├─ bytesToHex(salt) → "a1b2c3d4..."
    ├─ bytesToHex(hash_bytes) → "x1y2z3w4..."
    ↓
{hash: "x1y2z3w4...", salt: "a1b2c3d4..."}
    ↓
SQLiteCipherDB::createUser()
```

### Login de Usuario:
```
stored_password (texto plano)
    ↓
CryptoManager::verifyPassword(stored_password, stored_hash, stored_salt)
    ├─ hexToBytes(stored_salt) → salt_bytes
    ├─ PKCS5_PBKDF2_HMAC(password, salt_bytes, 10000) → computed_hash
    ├─ bytesToHex(computed_hash) → "x1y2z3w4..."
    ├─ comparar "x1y2z3w4..." == stored_hash
    ↓
true (coincide) o false (no coincide)
```

---

## Consideraciones de Seguridad

1. **Salt Único:** Cada contraseña tiene su propio salt generado aleatoriamente
2. **No Reversible:** El hash no puede ser revertido a la contraseña original
3. **Resistant a Ataques:**
   - **Fuerza Bruta:** Las 10000 iteraciones hacen muy lento probar muchas contraseñas
   - **Diccionario:** El salt único previene usar diccionarios precalculados
   - **Rainbow Tables:** El salt único invalida las tablas arcoíris
   - **Timing Attacks:** La comparación de strings es relativamente segura

4. **Endurecimiento Criptográfico:** El coste computacional de PBKDF2 es ajustable
5. **Estándar Industrial:** PBKDF2 es ampliamente auditado y probado

---

## Manejo de Errores

- **RAND_bytes() falla:** Lanza `std::runtime_error("RAND_bytes failed")`
- **Conversión hex inválida:** Lanza `std::runtime_error` con descripción
- **PBKDF2 falla:** Retorna false en verificación o lanza en hashing
- **verifyPassword() captura excepciones:** Retorna false en caso de error

---

## Notas Técnicas

- **C++17 Features:** Usa structured binding en los retornos
- **Const Correctness:** Todos los métodos públicos son `const`
- **Vector Allocation:** Los vectores se preasignan cuando se conoce el tamaño
- **Memory Safety:** Los vectores se limpian automáticamente al salir de scope
- **OpenSSL 1.1.x:** Compatible pero no 3.x (mantiene compatibilidad)

---

## Alternativas Consideradas (No Implementadas)

- **bcrypt:** Más lento pero más seguro que PBKDF2
- **Argon2:** Muy moderno pero más reciente
- **PBKDF2-SHA512:** Más seguro pero más lento

Se eligió PBKDF2-SHA256 por ser estándar, rápido, y suficientemente seguro para este proyecto.
