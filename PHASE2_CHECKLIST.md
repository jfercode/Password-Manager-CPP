# 📊 Fase 2 - Checklist Interactivo

## 🎓 Donde Estamos en el Proyecto

```
FASE 1 ✅ COMPLETADA
├─ [✅] Setup proyecto CMake
├─ [✅] Qt integrado
├─ [✅] Estructura de carpetas
├─ [✅] LoginDialog UI básica
└─ [✅] MainWindow UI básica

FASE 2 🔄 EN PROGRESO (TÚ ESTÁS AQUÍ)
├─ [ ] CryptoManager implementation (PBKDF2)
├─ [ ] AuthenticationManager logic
├─ [ ] SQLiteCipherDB para usuarios
├─ [ ] LoginDialog integrado con Auth
└─ [ ] NewUserDialog para registro

FASE 3 ⏳ PRÓXIMA
├─ [ ] AES-256 encryption
├─ [ ] Password table en BD
└─ [ ] Cipher/decipher data

FASE 4 ⏳ FUTURA
└─ [ ] CRUD completo de contraseñas

FASE 5 ⏳ FUTURA
└─ [ ] Refactor y calidad

FASE 6 ⏳ OPCIONAL
└─ [ ] Extras (generador, auto-lock, tests)
```

---

## 🛣️ Mapa de Tareas Fase 2

### Orden Recomendado de Implementación:

```
┌─────────────────────────────────────────┐
│ 1. CryptoManager.hpp (30 min)          │
│    └─ Definir interfaz con OpenSSL    │
└────────────┬────────────────────────────┘
             ↓
┌─────────────────────────────────────────┐
│ 2. CryptoManager.cpp (2 horas)         │
│    ├─ PBKDF2 hashing                   │
│    ├─ Random bytes generation          │
│    └─ Hex conversion utilities         │
└────────────┬────────────────────────────┘
             ↓
┌─────────────────────────────────────────┐
│ 3. SQLiteCipherDB.hpp/cpp (1.5 horas)  │
│    ├─ CREATE TABLE users               │
│    ├─ CRUD operations                  │
│    └─ Connection management            │
└────────────┬────────────────────────────┘
             ↓
┌─────────────────────────────────────────┐
│ 4. AuthenticationManager (1 hora)       │
│    ├─ Login logic                       │
│    ├─ Register logic                    │
│    └─ User validation                  │
└────────────┬────────────────────────────┘
             ↓
┌─────────────────────────────────────────┐
│ 5. LoginDialog integration (30 min)     │
│    ├─ Remove hardcoded auth            │
│    ├─ Conectar con AuthenticationMgr   │
│    └─ Add registration option          │
└────────────┬────────────────────────────┘
             ↓
┌─────────────────────────────────────────┐
│ 6. Test & Validation (1 hora)          │
│    ├─ Manual tests                      │
│    ├─ DB verification                   │
│    └─ Security review                  │
└─────────────────────────────────────────┘
```

---

## 📋 Checklist Detallado

### TAREA 1: CryptoManager.hpp
- [ ] Crear archivo o limpiar existente
- [ ] Incluir headers OpenSSL (rand.h, evp.h, err.h)
- [ ] Definir constructor y destructor
- [ ] Definir hashPassword() method
- [ ] Definir verifyPassword() method
- [ ] Definir deriveKey() method
- [ ] Definir métodos privados auxiliares
- [ ] Documentar todos los métodos con comentarios
- [ ] Compilar y verificar (sin errores de sintaxis)

**Comando para compilar y verificar**:
```bash
cd /home/jaferna2/Desktop/Password-Manager/build
cmake ..
make
```

### TAREA 2: CryptoManager.cpp
- [ ] Incluir CryptoManager.hpp
- [ ] Incluir headers OpenSSL necesarios
- [ ] Implementar constructor (OpenSSL_add_all_algorithms)
- [ ] Implementar generateRandomBytes()
  - [ ] Usar RAND_bytes()
  - [ ] Manejar errores
- [ ] Implementar bytesToHex()
  - [ ] Loop sobre bytes
  - [ ] snprintf para conversión
- [ ] Implementar hexToBytes()
  - [ ] Parse string hex
  - [ ] Crear vector de bytes
- [ ] Implementar hashPassword()
  - [ ] Generar salt con generateRandomBytes()
  - [ ] Usar PKCS5_PBKDF2_HMAC
  - [ ] Convertir resultado a hex
  - [ ] Retornar pair<hash, salt>
- [ ] Implementar verifyPassword()
  - [ ] Convertir hex a bytes
  - [ ] Hashear contraseña ingresada
  - [ ] Comparar con stored hash
  - [ ] Retornar bool
- [ ] Implementar deriveKey()
  - [ ] Similar a hashPassword pero para claves de cifrado
- [ ] Test: Compilar sin errores
- [ ] Test: Verificar que PBKDF2 produce hashes diferentes para mismo password

### TAREA 3: SQLiteCipherDB.hpp
- [ ] Definir clase SQLiteCipherDB
- [ ] Incluir headers sqlite3
- [ ] Definir miembros privados (db connection)
- [ ] Definir métodos:
  - [ ] initializeDatabase() / openDatabase()
  - [ ] createUser()
  - [ ] getUserHash()
  - [ ] userExists()
  - [ ] updatePassword()
  - [ ] deleteUser()
- [ ] Documentar métodos

### TAREA 4: SQLiteCipherDB.cpp
- [ ] Crear tabla 'users' en constructor/inicialización
- [ ] Implementar createUser()
  - [ ] Prepared statement INSERT
  - [ ] Manejar duplicados
  - [ ] Retornar success/error
- [ ] Implementar getUserHash()
  - [ ] SELECT password_hash, password_salt WHERE username
  - [ ] Llenar parámetros referencia
  - [ ] Manejar usuario no encontrado
- [ ] Implementar userExists()
  - [ ] SELECT COUNT(*) WHERE username
  - [ ] Retornar bool
- [ ] Test: Compilar sin errores

### TAREA 5: AuthenticationManager.hpp
- [ ] Definir clase AuthenticationManager
- [ ] Miembros privados:
  - [ ] CryptoManager crypto
  - [ ] Referencia a SQLiteCipherDB
- [ ] Métodos públicos:
  - [ ] Constructor (recibir BD)
  - [ ] authenticateUser()
  - [ ] registerNewUser()
  - [ ] changePassword() (opcional)
- [ ] Documentar bien

### TAREA 6: AuthenticationManager.cpp
- [ ] Implementar constructor
- [ ] Implementar authenticateUser()
  - [ ] Buscar usuario en BD
  - [ ] Verificar contraseña con CryptoManager
  - [ ] Retornar bool
  - [ ] NO loguear la contraseña
- [ ] Implementar registerNewUser()
  - [ ] Verificar usuario no existe
  - [ ] Hashear contraseña
  - [ ] Guardar en BD
  - [ ] Retornar bool
- [ ] Manejo de errores
- [ ] Test: Compilar

### TAREA 7: Actualizar LoginDialog.cpp
- [ ] Incluir AuthenticationManager.hpp
- [ ] En onLoginClicked():
  - [ ] Cambiar hardcoded auth por AuthenticationManager
  - [ ] Pasar referencia a BD
  - [ ] Llamar authenticateUser()
  - [ ] Mostrar error si falla
- [ ] BONUS: Agregar botón "Registrarse"
  - [ ] Abrir NewUserDialog
- [ ] Test: Compilar

### TAREA 8: Tests Manuales
- [ ] Test 1: Compilación exitosa
  ```bash
  cd build && cmake .. && make
  ```
- [ ] Test 2: Crear nuevo usuario
  - [ ] Ejecutar app
  - [ ] Registro: "testuser" / "password123"
  - [ ] Verificar en BD
- [ ] Test 3: Login correcto
  - [ ] Ingresar credenciales correctas
  - [ ] Debe entrar a MainWindow
- [ ] Test 4: Login incorrecto
  - [ ] Ingresar contraseña incorrecta
  - [ ] Mostrar error
- [ ] Test 5: Usuario no existe
  - [ ] Intentar login con usuario inexistente
  - [ ] Mostrar error
- [ ] Test 6: Verificar BD directamente
  ```bash
  sqlite3 /path/to/database.db
  SELECT * FROM users;
  ```
  - [ ] password_hash está en hex (no es texto plano)
  - [ ] password_salt está presente
  - [ ] Dos usuarios con mismo password tienen hashes diferentes

---

## 🎓 Conceptos a Dominar Antes de Pasar

Antes de continuar a Fase 3, debes entender:

- [ ] ¿Por qué PBKDF2 es seguro?
  - Respuesta: Usa iteraciones que ralentizan ataques por fuerza bruta
  
- [ ] ¿Qué es un "salt"?
  - Respuesta: Random bytes agregados a la contraseña antes de hashear
  
- [ ] ¿Por qué almacenamos salt en la BD?
  - Respuesta: Se necesita para verificar durante login
  
- [ ] ¿Puede alguien descifrar una contraseña hashada?
  - Respuesta: No, hashes son unidireccionales. Solo se puede verificar
  
- [ ] Diferencia entre hashPassword() y deriveKey()?
  - Respuesta: hashPassword() es para almacenar; deriveKey() es para cifrado AES
  
- [ ] ¿Qué sucede si el parámetro "iterations" es muy bajo?
  - Respuesta: Los ataques por fuerza bruta se vuelven más rápidos
  
- [ ] ¿Cómo evitas que un atacante vea las contraseñas en memoria?
  - Respuesta: Limpiar con memset() o usar clases especializadas
  
- [ ] ¿Qué sucede si reutilizas el mismo salt?
  - Respuesta: Dos usuarios con mismo password tendrían mismo hash (vulnerable)

---

## 🐛 Problemas Comunes y Soluciones

### Problema: "Undefined reference to PKCS5_PBKDF2_HMAC"
**Solución**: 
- Verificar que OpenSSL está linkado en CMakeLists.txt
- ```cmake
  target_link_libraries(PasswordManager OpenSSL::Crypto)
  ```

### Problema: "sqlite3 not found"
**Solución**:
- Verificar que SQLCipher está instalado
- ```bash
  pkg-config --cflags --libs sqlite3
  ```

### Problema: "random password hashes cada vez"
**Esperado**: ✅ BIEN - Por eso usamos salt aleatorio

### Problema: "Mi verificación de contraseña siempre falla"
**Debug**:
1. Verifica que el salt se almacena y se recupera correctamente
2. Imprime (en hex) el salt que usaste
3. Comprueba que hexToBytes() invierte correctamente bytesToHex()
4. Verifica iteraciones = iteraciones usadas al hashear

---

## 📚 Archivos a Crear/Editar

```
✏️ Editar:
├── include/
│   ├── CryptoManager.hpp          (Vacío → Interfaz completa)
│   ├── AuthenticationManager.hpp  (Minimal → Interfaz completa)
│   └── SQLiteCipherDB.hpp         (Vacío → Interfaz completa)
├── src/
│   ├── crypto/CryptoManager.cpp         (Vacío → Implementación)
│   ├── app/AuthenticationManager.cpp    (Vacío → Implementación)
│   ├── storage/SQLiteCipherDB.cpp       (Vacío → Implementación)
│   └── ui/LoginDialog.cpp               (Modificar lógica de login)
└── CMakeLists.txt                       (Verificar que está correctamente configurado)
```

---

## 🚀 Comandos Útiles

### Compilar durante el desarrollo:
```bash
cd /home/jaferna2/Desktop/Password-Manager/build
cmake ..
make -j$(nproc)  # Compilar en paralelo
```

### Ver errores de compilación:
```bash
make 2>&1 | grep error
```

### Limpiar build:
```bash
rm -rf /home/jaferna2/Desktop/Password-Manager/build/*
```

### Inspeccionar BD:
```bash
sqlite3 ~/.pm/database.db
.tables
.schema users
SELECT * FROM users;
```

---

## ✅ Criterios de Aceptación para Fase 2

Tu Fase 2 está COMPLETA cuando:

1. **Compilación**: El proyecto compila sin errores ni warnings
2. **PBKDF2**: Implementado y funcional con OpenSSL
3. **BD**: Tabla 'users' creada con usuarios almacenados
4. **Login**: Funciona con hash verificación (no hardcoded)
5. **Registro**: Puede crear nuevos usuarios
6. **Tests Manuales**: Todos pasan sin errores
7. **Seguridad**: Contraseñas no están en texto plano en la BD
8. **Documentación**: Código documentado con comentarios

¡Una vez cumplas TODO esto, estarás listo para Fase 3! 🎉
