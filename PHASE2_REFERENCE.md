# 🔧 Fase 2 - Comando y Referencia Rápida

## 🛠️ Comandos Básicos

### Compilación

```bash
# Ir a carpeta build
cd /home/jaferna2/Desktop/Password-Manager/build

# Configurar CMake (primera vez o si cambias CMakeLists.txt)
cmake ..

# Compilar
make -j$(nproc)        # Compilar en paralelo (recomendado)
make -j4               # Compilar con 4 threads
make                   # Compilar secuencial (lento)

# Compilar y mostrar solo errores
make 2>&1 | grep -i error

# Limpiar todo y recompilar
cd ..
rm -rf build
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### Base de Datos

```bash
# Ver usuario actual
whoami

# Ver donde está la BD
ls -la ~/.pm/

# Abrir BD con sqlite3
sqlite3 ~/.pm/passwords.db

# Dentro de sqlite3:
.tables                         # Ver tablas
.schema users                   # Ver estructura
SELECT * FROM users;            # Ver usuarios
SELECT username, password_hash FROM users;  # Ver hashes
DELETE FROM users;              # Limpiar (para tests)
.exit                           # Salir
```

### Git (Control de Versiones)

```bash
# Ver cambios
git status
git diff

# Agregar cambios
git add .

# Commit
git commit -m "Fase 2: Implementar PBKDF2 y autenticación"

# Ver historial
git log --oneline
```

### Ejecución

```bash
# Ejecutar la aplicación compilada
cd /home/jaferna2/Desktop/Password-Manager/build
./PasswordManager

# Con output de terminal visible
./PasswordManager 2>&1 | cat
```

---

## 📝 Include Necesarios (Cheat Sheet)

### Para CryptoManager.cpp
```cpp
#include "../../include/CryptoManager.hpp"
#include <cstring>              // memset, snprintf
#include <stdexcept>            // std::runtime_error
#include <cctype>               // tolower
#include <openssl/rand.h>       // RAND_bytes
#include <openssl/evp.h>        // PKCS5_PBKDF2_HMAC, EVP_sha256
#include <openssl/err.h>        // ERR_get_error
```

### Para SQLiteCipherDB.cpp
```cpp
#include "../../include/SQLiteCipherDB.hpp"
#include <sqlite3.h>
#include <iostream>
#include <stdexcept>
```

### Para AuthenticationManager.cpp
```cpp
#include "../../include/AuthenticationManager.hpp"
// Nota: Incluye los .hpp que ya incluyen sus dependencias
```

---

## 🔐 Funciones OpenSSL Clave

### PBKDF2-SHA256
```cpp
int PKCS5_PBKDF2_HMAC(
    const char *password,       // Contraseña
    int password_len,           // Longitud
    const unsigned char *salt,  // Salt bytes
    int salt_len,               // Longitud del salt
    int iter,                   // Iteraciones (100000 recomendado)
    const EVP_MD *md,          // EVP_sha256()
    int keylen,                 // 32 para SHA256
    unsigned char *out          // Buffer de salida
);
// Retorna: 1 si éxito, 0 si falla
```

### Generación de Random Bytes
```cpp
int RAND_bytes(unsigned char *buf, int num);
// Retorna: 1 si éxito, 0 si falla
// Rellena buf con num bytes aleatorios
```

### Comparación Segura de Memoria
```cpp
int CRYPTO_memcmp(const void *a, const void *b, size_t len);
// Retorna: 0 si iguales, no-zero si diferentes
// Toma tiempo constante (no vulnerable a timing attacks)
```

### Obtener Descripción de Error
```cpp
unsigned long ERR_get_error(void);
char *ERR_error_string(unsigned long e, char *buf);
int ERR_error_string_n(unsigned long e, char *buf, size_t len);
```

---

## 📊 Estructura de Base de Datos

### Tabla Users
```sql
CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password_hash TEXT NOT NULL,
    password_salt TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

### Consultas SQL Útiles
```sql
-- Crear usuario
INSERT INTO users (username, password_hash, password_salt) 
VALUES ('john', 'a1b2c3d4...', 'f6e5d4c3...');

-- Obtener hash y salt
SELECT password_hash, password_salt 
FROM users 
WHERE username = 'john';

-- Verificar si existe
SELECT COUNT(*) FROM users WHERE username = 'john';

-- Actualizar contraseña
UPDATE users 
SET password_hash = '...', password_salt = '...' 
WHERE username = 'john';

-- Eliminar usuario
DELETE FROM users WHERE username = 'john';

-- Ver todos
SELECT * FROM users;
```

---

## 🧪 Pruebas de Verificación

### Test: ¿PBKDF2 funciona?
```cpp
CryptoManager crypto;

// Generar hash
auto [hash1, salt1] = crypto.hashPassword("test123");
std::cout << "Hash: " << hash1 << std::endl;
std::cout << "Salt: " << salt1 << std::endl;

// Verificar que es correcto
bool verified = crypto.verifyPassword("test123", hash1, salt1);
std::cout << "Verified: " << (verified ? "YES" : "NO") << std::endl;

// Verificar que falla con contraseña incorrecta
bool wrong = crypto.verifyPassword("wrong", hash1, salt1);
std::cout << "Wrong password rejected: " << (wrong ? "FAIL" : "PASS") << std::endl;
```

### Test: ¿Salts son diferentes?
```cpp
CryptoManager crypto;

auto [hash1, salt1] = crypto.hashPassword("password");
auto [hash2, salt2] = crypto.hashPassword("password");

std::cout << "Salt1: " << salt1 << std::endl;
std::cout << "Salt2: " << salt2 << std::endl;
std::cout << "Diferentes? " << (salt1 != salt2 ? "YES" : "NO") << std::endl;
```

### Test: ¿BD guarda correctamente?
```bash
sqlite3 ~/.pm/passwords.db
SELECT username, length(password_hash), length(password_salt) FROM users;
-- password_hash debe tener ~64 caracteres (32 bytes en hex)
-- password_salt debe tener ~32 caracteres (16 bytes en hex)
```

---

## 🐛 Debug y Troubleshooting

### Ver Error OpenSSL Completo
```cpp
try {
    // código que falla
} catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    
    unsigned long err = ERR_get_error();
    char err_str[256];
    ERR_error_string_n(err, err_str, sizeof(err_str));
    std::cerr << "OpenSSL Error: " << err_str << std::endl;
}
```

### Ver Valores de Configuración
```cpp
std::cout << "PBKDF2 Iterations: " << 100000 << std::endl;
std::cout << "Salt Length: " << 16 << " bytes" << std::endl;
std::cout << "Hash Length: " << 32 << " bytes" << std::endl;
std::cout << "Hash Algorithm: SHA-256" << std::endl;
```

### Imprimir Hash en Partes
```cpp
std::string hash = "a1b2c3d4e5f6...";
std::cout << "Hash first 16 chars: " << hash.substr(0, 16) << std::endl;
std::cout << "Hash last 16 chars: " << hash.substr(hash.length()-16) << std::endl;
std::cout << "Hash length: " << hash.length() << std::endl;
```

---

## 📐 Parámetros de Referencia

### PBKDF2-SHA256 Estándares
```
RFC 2898 (PBKDF2):
- Iterations: 1,000 - 1,000,000 (más es más seguro)
- Salt length: >= 128 bits (16 bytes)
- Output length: >= 160 bits (20 bytes)

NIST 2024 Recomendaciones:
- Iterations: >= 100,000
- Salt length: 128 bits mínimo

Nuestro Proyecto:
- Iterations: 100,000 ✅
- Salt length: 16 bytes (128 bits) ✅
- Output length: 32 bytes (256 bits) ✅
- Hash: SHA-256 ✅
```

### Tiempo de Ejecución Esperado
```
Operación                  | Tiempo
---------------------------|----------
hashPassword()            | 100-500ms (depende CPU)
verifyPassword()          | 100-500ms (igual a hash)
Registro de usuario       | 200-700ms
Login de usuario          | 200-700ms

Si toma < 50ms:   Iteraciones son pocas (inseguro)
Si toma > 2s:     Iteraciones son muchas (lento)
```

---

## 💾 Persistencia de Datos

### Ubicación de BD Recomendada
```cpp
// Opción 1: Carpeta home del usuario
std::string dbPath = std::string(getenv("HOME")) + "/.pm/passwords.db";

// Opción 2: Carpeta actual
std::string dbPath = "./passwords.db";

// Opción 3: Ruta absoluta
std::string dbPath = "/home/jaferna2/.pm/passwords.db";
```

### Crear Carpeta si no Existe
```cpp
#include <sys/stat.h>
#include <sys/types.h>

std::string dbDir = std::string(getenv("HOME")) + "/.pm";
mkdir(dbDir.c_str(), 0700);  // Solo owner puede leer/escribir/ejecutar
```

---

## 🎯 Checklist Diario

### Cada día que trabajes en Fase 2:

```
[ ] Iniciaste la sesión y abriste el workspace
[ ] Compilaste el proyecto (make -j$(nproc))
[ ] No hay errores de compilación
[ ] Creaste un nuevo usuario en la BD
[ ] Verificaste login con contraseña correcta
[ ] Verificaste que login falla con contraseña incorrecta
[ ] Abriste la BD y verificaste que hash está en hexadecimal
[ ] Hiciste git commit de cambios
[ ] Terminaste sin dejar código en estado roto
```

---

## 🚀 Fase 2: Timeline Estimado

| Tarea | Tiempo | Acumulado |
|-------|--------|-----------|
| Leer guía conceptos | 30 min | 30 min |
| CryptoManager.hpp | 30 min | 1 h |
| CryptoManager.cpp | 2 h | 3 h |
| SQLiteCipherDB | 1.5 h | 4.5 h |
| AuthenticationManager | 1 h | 5.5 h |
| LoginDialog actualización | 30 min | 6 h |
| Tests y debugging | 1-2 h | 7-8 h |
| **TOTAL FASE 2** | **~8 horas** | |

Distribuido en:
- Día 1: Lectura + CryptoManager
- Día 2: SQLiteCipherDB + Auth
- Día 3: Integración + Tests

---

## 📖 Documentación Enlaces

### OpenSSL
- Man pages local: `man PKCS5_PBKDF2_HMAC`
- Online: https://www.openssl.org/docs/man1.1.1/man3/PKCS5_PBKDF2_HMAC.html

### SQLite
- C API: https://www.sqlite.org/c3ref/intro.html
- SQL Reference: https://www.sqlite.org/lang.html

### Qt
- Signal/Slots: https://doc.qt.io/qt-5/signalsandslots.html
- Dialogs: https://doc.qt.io/qt-5/qdialog.html

### OWASP Security
- Password Storage: https://cheatsheetseries.owasp.org/cheatsheets/Password_Storage_Cheat_Sheet.html

---

## 🎓 Recordatorios Importantes

```
🔴 NUNCA HACER:
  ❌ Loguear contraseñas
  ❌ Almacenar contraseñas en texto plano
  ❌ Usar < 100,000 iteraciones PBKDF2
  ❌ Reutilizar el mismo salt
  ❌ Comparación simple de strings para hashes

🟢 SIEMPRE HACER:
  ✅ Hashear PBKDF2 + salt aleatorio
  ✅ Almacenar hash + salt en BD
  ✅ Limpiar memoria sensible
  ✅ Usar CRYPTO_memcmp para comparación
  ✅ Compilar frecuentemente
  ✅ Hacer git commits
```

---

Este documento es tu referencia rápida. Guárdalo y consulta frecuentemente. 🚀
