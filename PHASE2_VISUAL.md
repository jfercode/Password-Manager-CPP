# 📊 Fase 2 - Resumen Visual Rápido

## 🎯 ¿Dónde Estás?

```
┌────────────────────────────────────────────┐
│                                            │
│  FASE 1: ✅ Completada                     │
│  └─ Setup, CMake, Qt UI básica            │
│                                            │
│  FASE 2: 🔴 ACTUAL - Autenticación        │
│  └─ Hashing, BD, Login seguro             │
│                                            │
│  FASE 3: ⏳ Próxima - Cifrado AES-256      │
│  └─ Encryptar contraseñas almacenadas    │
│                                            │
│  FASES 4-6: ⏳ Futuras                     │
│  └─ CRUD, Refactor, Extras                │
│                                            │
└────────────────────────────────────────────┘
```

---

## 🔐 Flujo de Autenticación (Fase 2)

### Registro (Primera vez)

```
Usuario ingresa: "john" / "password123"
          ↓
[CryptoManager]
    hashPassword("password123")
          ↓
    Genera salt aleatorio (16 bytes)
    Aplica PBKDF2-SHA256 con 100,000 iteraciones
          ↓
    Retorna: {hash_hex, salt_hex}
          ↓
[SQLiteCipherDB]
    INSERT INTO users (john, hash, salt)
          ↓
✅ Usuario registrado

BD ahora contiene:
┌─────────┬──────────────────────────────┬──────────────────────────────┐
│ username│ password_hash (64 chars hex)  │ password_salt (32 chars hex) │
├─────────┼──────────────────────────────┼──────────────────────────────┤
│ john    │ a1b2c3d4e5f6...              │ f6e5d4c3b2a1...              │
└─────────┴──────────────────────────────┴──────────────────────────────┘
```

### Login (Posteriores)

```
Usuario ingresa: "john" / "password123"
          ↓
[SQLiteCipherDB]
    SELECT password_hash, password_salt WHERE username='john'
          ↓
    Retorna: {hash: "a1b2c3d4...", salt: "f6e5d4c3..."}
          ↓
[CryptoManager]
    verifyPassword("password123", hash, salt)
          ↓
    Convierte salt de hex a bytes
    Aplica PBKDF2-SHA256 con MISMO salt y 100,000 iteraciones
          ↓
    Compara resultado con hash almacenado
          ↓
    ¿Coinciden? → ✅ LOGIN OK o ❌ ERROR
```

---

## 📁 Archivos a Editar en Orden

### 1️⃣ `include/CryptoManager.hpp` (30 min)
```
Estado: 🔴 Vacío
Acción: Agregar interfaz PBKDF2
Código: Ver PHASE2_TEMPLATES.md - Template 1
```

### 2️⃣ `src/crypto/CryptoManager.cpp` (2 hours)
```
Estado: 🔴 Vacío
Acción: Implementar PBKDF2, random, hex conversions
Código: Ver PHASE2_TEMPLATES.md - Template 2
```

### 3️⃣ `include/SQLiteCipherDB.hpp` (30 min)
```
Estado: 🔴 Vacío
Acción: Definir tabla users y métodos CRUD
Código: Ver PHASE2_TEMPLATES.md - Template 3
```

### 4️⃣ `src/storage/SQLiteCipherDB.cpp` (1.5 hours)
```
Estado: 🔴 Vacío
Acción: Implementar CREATE TABLE, INSERT, SELECT, DELETE
Skill: SQL + sqlite3 C API
```

### 5️⃣ `include/AuthenticationManager.hpp` (30 min)
```
Estado: 🔴 Casi vacío
Acción: Definir métodos authenticate/register
Código: Ver PHASE2_TEMPLATES.md - Template 4
```

### 6️⃣ `src/app/AuthenticationManager.cpp` (1 hour)
```
Estado: 🔴 Vacío
Acción: Lógica de login y registro
Código: Ver PHASE2_TEMPLATES.md - Template 5
```

### 7️⃣ `src/ui/LoginDialog.cpp` (30 min)
```
Estado: 🟡 Parcial
Acción: Cambiar hardcoded auth por AuthenticationManager
Código: Ver PHASE2_TEMPLATES.md - Template 6
```

---

## 🔄 Ciclo de Vida PBKDF2

```
┌─────────────────────────────────────────────────────┐
│                PBKDF2-SHA256                        │
└─────────────────────────────────────────────────────┘

ENTRADA:
  password: "myPassword123"     (variable length)
  salt:     <16 random bytes>   (128 bits)
  iterations: 100,000           (ralentiza ataques)
  hash_fn:  SHA-256             (probado y seguro)

PROCESO:
  result = PBKDF2(password, salt, iterations, SHA-256, 32 bytes)
  
  ↓ (internamente en OpenSSL)
  
  U1 = HMAC-SHA256(password, salt)
  U2 = HMAC-SHA256(password, U1)
  U3 = HMAC-SHA256(password, U2)
  ...
  U100000 = HMAC-SHA256(password, U99999)
  
  result = U1 XOR U2 XOR ... XOR U100000

SALIDA:
  hash: <32 bytes>
  
  En hex: "a1b2c3d4e5f6..." (64 caracteres)
```

---

## 🎓 Concepto Clave: ¿Por qué es Seguro?

```
❌ INSEGURO: hash("password") = "5f4dcc3b5aa765d61d8327deb882cf99"
   Problema: Mismo password → Mismo hash (Rainbow tables)

✅ SEGURO: PBKDF2("password", salt="random", iterations=100000)
   Ventaja: Mismo password + diferentes salts = diferentes hashes
   Ventaja: 100,000 iteraciones ralentizan fuerza bruta
   
Ejemplo:
  Usuario 1: PBKDF2("password", "random1") = "a1b2c3d4..."
  Usuario 2: PBKDF2("password", "random2") = "x9y8z7w6..."
  
  Aunque ambos usan "password", los hashes son DISTINTOS
```

---

## 📊 Comparación: Fases 1, 2, 3

```
┌──────────┬────────────────────┬────────────────────┬────────────────────┐
│ Fase     │ FASE 1             │ FASE 2 (AHORA)     │ FASE 3 (PRÓXIMA)   │
├──────────┼────────────────────┼────────────────────┼────────────────────┤
│ Enfoque  │ UI + Setup         │ Autenticación      │ Cifrado de datos   │
├──────────┼────────────────────┼────────────────────┼────────────────────┤
│ Crypto   │ -                  │ PBKDF2-SHA256      │ AES-256-CBC        │
├──────────┼────────────────────┼────────────────────┼────────────────────┤
│ Storage  │ -                  │ Tabla users        │ Tabla passwords    │
├──────────┼────────────────────┼────────────────────┼────────────────────┤
│ Security │ Basic              │ Hash seguro        │ Cipher completo    │
├──────────┼────────────────────┼────────────────────┼────────────────────┤
│ Tiempo   │ 1 semana           │ 1 semana (2-3 h)   │ 1 semana           │
└──────────┴────────────────────┴────────────────────┴────────────────────┘
```

---

## 🧪 Tests Rápidos

### Test 1: ¿Compila?
```bash
cd /home/jaferna2/Desktop/Password-Manager/build
cmake ..
make -j$(nproc)
# Esperar a que complete sin errores
```

### Test 2: ¿Se registra usuario?
```bash
./PasswordManager
# Ir a "Register"
# Crear: "testuser" / "pass123"
# Verificar en BD
```

### Test 3: ¿Login funciona?
```bash
./PasswordManager
# Ingresar: "testuser" / "pass123"
# Debe mostrar MainWindow
```

### Test 4: ¿Hashes diferentes?
```bash
sqlite3 ~/.pm/passwords.db
SELECT username, password_hash FROM users;
# Los dos primeros caracteres del hash ¿son diferentes?
```

---

## 💡 Trucos y Tips

### Compilación Rápida
```bash
cd /home/jaferna2/Desktop/Password-Manager/build
cmake .. && make -j4
```

### Ver Errores OpenSSL
```cpp
if (result != 1) {
    char err_str[256];
    ERR_error_string_n(ERR_get_error(), err_str, sizeof(err_str));
    std::cerr << err_str << std::endl;
}
```

### Debug BD
```bash
sqlite3 ~/.pm/passwords.db
.mode column
.headers on
SELECT * FROM users;
```

### Limpiar memoria sensible
```cpp
memset(password.data(), 0, password.length());  // Sobrescribir
```

---

## ⚠️ Errores Comunes

| Error | Causa | Solución |
|-------|-------|----------|
| "Undefined reference to PBKDF2" | OpenSSL no linkado | Agregar en CMakeLists: `target_link_libraries(...OpenSSL::Crypto)` |
| "sqlite3 not found" | SQLite/SQLCipher no instalado | `sudo apt install sqlite3 libsqlite3-dev` |
| Login siempre falla | Salt no se recupera correctamente | Debug: imprimir salt en hex antes/después de converso |
| Hashes iguales cada vez | No se usa salt aleatorio | Verificar generateRandomBytes() |
| Compilador error en snprintf | C++ estricto | Incluir `<cstdio>` |

---

## 🎯 Criterios de Éxito Fase 2

Cuando termines Fase 2:

- ✅ **Compilación**: `make` sin errores ni warnings
- ✅ **PBKDF2**: Funciona con OpenSSL, genera hashes diferentes
- ✅ **BD**: Tabla users existe con hash y salt
- ✅ **Registro**: Puede crear nuevos usuarios
- ✅ **Login**: Verifica contraseñas correctamente
- ✅ **Seguridad**: Hashes en hexadecimal, no texto plano
- ✅ **Tests**: Todos pasan sin excepciones

---

## 📚 Documentos Guía

En tu carpeta del proyecto tienes:

1. **PHASE2_GUIDE.md** ← Lee primero (conceptos + explicaciones)
2. **PHASE2_CHECKLIST.md** ← Usa para seguimiento (tareas)
3. **PHASE2_TEMPLATES.md** ← Copia código (implementación)
4. **PHASE2_VISUAL.md** ← Este archivo (resumen rápido)

---

## 🚀 ¿Listo para Empezar?

### Paso 1: Lee
```
Abre PHASE2_GUIDE.md
Lee los conceptos clave (PBKDF2, salt, etc)
Dedica 30 minutos a entender "por qué"
```

### Paso 2: Planifica
```
Abre PHASE2_CHECKLIST.md
Marca tareas conforme avances
Compila frecuentemente
```

### Paso 3: Código
```
Abre PHASE2_TEMPLATES.md
Copia templates en tus archivos
Personaliza según necesites
Compila y prueba después de cada tarea
```

### Paso 4: Prueba
```
Ejecuta tests manuales
Verifica BD
Asegúrate que hashing funciona correctamente
```

### Paso 5: Documenta
```
Agrega comentarios al código
Actualiza README si es necesario
Commit a Git
```

---

## ❓ Si te Atascas...

1. **Error de compilación**: 
   - Verifica que todos los headers existan
   - Busca en Google el error (OpenSSL es bien documentado)

2. **Login no funciona**:
   - Imprime pasos: usuario encontrado? → contraseña verificada?
   - Debug paso a paso en onLoginClicked()

3. **PBKDF2 produce resultados raros**:
   - Verifica parámetros (iterations, salt length)
   - Prueba con un ejemplo conocido de PBKDF2

4. **BD no se crea**:
   - Verifica permisos de carpeta
   - Prueba crear directorio manualmente
   - Usa absolute paths, no ~

---

## 📞 Próximos Pasos (Preview)

Después de completar Fase 2:
- Fase 3: AES-256 para cifrar passwords almacenados
- Agregar tabla `passwords` con campos cifrados
- Implementar encrypt/decrypt en CryptoManager

¡Vamos! 🚀 Eres capaz de completar Fase 2. 💪
