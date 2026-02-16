# 📚 Documentación del Password Manager - Índice y Guía de Navegación

## 🗂️ Estructura de Documentación

```
/doc/
├── README.md                    ← COMIENZA AQUÍ (Documentación General)
├── APP_MODULE.md                ← Módulo de Autenticación
├── CRYPTO_MODULE.md             ← Módulo Criptográfico
├── STORAGE_MODULE.md            ← Módulo de Base de Datos
├── UI_MODULE.md                 ← Módulo de Interfaz Gráfica
├── CORE_MODULE.md               ← Módulo de Utilidades
├── LIBRARIES_REFERENCE.md       ← Referencia de Librerías
└── INDEX.md                     ← Este archivo
```

---

## 📖 Guía Rápida de Lectura

### Para Principiantes (Primera vez leyendo la documentación)

1. **Comienza aquí:** [`README.md`](README.md)
   - Introducción al proyecto
   - Arquitectura general
   - Flujos principales
   - Requisitos del sistema

2. **Luego lee por módulo:**
   - [`UI_MODULE.md`](UI_MODULE.md) - Entiende la interfaz primero
   - [`APP_MODULE.md`](APP_MODULE.md) - Lógica de autenticación
   - [`CRYPTO_MODULE.md`](CRYPTO_MODULE.md) - Criptografía
   - [`STORAGE_MODULE.md`](STORAGE_MODULE.md) - Base de datos
   - [`CORE_MODULE.md`](CORE_MODULE.md) - Utilidades

3. **Consulta cuando necesites:**
   - [`LIBRARIES_REFERENCE.md`](LIBRARIES_REFERENCE.md) - Detalles de librerías

---

### Para Desarrolladores (Modificando código)

**Si modificas el módulo UI:**
→ Lee [`UI_MODULE.md`](UI_MODULE.md)
- Sistema de Señales y Slots
- Estructura de widgets
- Flujo de eventos

**Si modificas criptografía:**
→ Lee [`CRYPTO_MODULE.md`](CRYPTO_MODULE.md)
- PBKDF2-SHA256
- Generación de números aleatorios
- Manejo de errores OpenSSL

**Si modificas base de datos:**
→ Lee [`STORAGE_MODULE.md`](STORAGE_MODULE.md)
- Esquema SQL
- Prepared statements
- Ubicación de datos

**Si añades nuevas funciones:**
→ Lee [`CORE_MODULE.md`](CORE_MODULE.md)
- Sistema de logging
- Manejo de directorios
- Librerías compartidas

---

### Para Arquitectos de Software (Diseñando cambios)

1. **Entiende la arquitectura completa:**
   - [`README.md`](README.md) - Sección "Arquitectura General"

2. **Revisa patrones de diseño:**
   - Inyección de dependencias
   - Separación de responsabilidades
   - Composición sobre herencia

3. **Consulta las dependencias:**
   - [`LIBRARIES_REFERENCE.md`](LIBRARIES_REFERENCE.md)

---

## 🎯 Búsqueda Rápida por Tema

### Autenticación
- **¿Cómo funciona la inicialización del sistema?** → [`APP_MODULE.md#flujo-de-inicialización-del-sistema-nuevo`](APP_MODULE.md#flujo-de-inicialización-del-sistema-nuevo)
- **¿Cómo funciona el login?** → [`README.md#flujo-2-autenticación-de-usuario`](README.md#flujo-2-autenticación-de-usuario)
- **¿Cómo se registra un usuario?** → [`README.md#flujo-1-registro-de-nuevo-usuario`](README.md#flujo-1-registro-de-nuevo-usuario)
- **¿Qué métodos tiene AuthenticationManager?** → [`APP_MODULE.md#clase-authenticationmanager`](APP_MODULE.md#clase-authenticationmanager)
- **¿Qué es InitializationManager?** → [`APP_MODULE.md#clase-initializationmanager`](APP_MODULE.md#clase-initializationmanager)

### Seguridad y Criptografía
- **¿Cómo se hashean las contraseñas?** → [`CRYPTO_MODULE.md#algoritmo-pbkdf2-sha256`](CRYPTO_MODULE.md#algoritmo-pbkdf2-sha256)
- **¿Qué es PBKDF2-SHA256?** → [`CRYPTO_MODULE.md#algoritmo-pbkdf2-sha256-password-based-key-derivation-function-2`](CRYPTO_MODULE.md#algoritmo-pbkdf2-sha256-password-based-key-derivation-function-2)
- **Consideraciones de seguridad** → [`README.md#consideraciones-de-seguridad`](README.md#consideraciones-de-seguridad)
- **Librerías criptográficas** → [`LIBRARIES_REFERENCE.md#criptografía`](LIBRARIES_REFERENCE.md#criptografía)

### Base de Datos
- **¿Dónde se almacenan los datos?** → [`STORAGE_MODULE.md#ubicación-de-almacenamiento`](STORAGE_MODULE.md#ubicación-de-almacenamiento)
- **¿Cuál es el esquema SQL?** → [`STORAGE_MODULE.md#esquema-de-base-de-datos`](STORAGE_MODULE.md#esquema-de-base-de-datos)
- **SQLite3 API referencia** → [`LIBRARIES_REFERENCE.md#sqlite3`](LIBRARIES_REFERENCE.md#sqlite3)

### Interfaz Gráfica
- **¿Cómo funciona el diálogo de login?** → [`UI_MODULE.md#logindialoghpp`](UI_MODULE.md#logindialoghpp)
- **Sistema de Señales y Slots** → [`UI_MODULE.md#sistema-de-señales-y-slots`](UI_MODULE.md#sistema-de-señales-y-slots)
- **Qt Framework referencia** → [`LIBRARIES_REFERENCE.md#qt-framework`](LIBRARIES_REFERENCE.md#qt-framework)

### Compilación y Ejecución
- **¿Cómo compilar?** → [`README.md#guía-de-compilación-y-ejecución`](README.md#guía-de-compilación-y-ejecución)
- **Requisitos del sistema** → [`README.md#requisitos-del-sistema`](README.md#requisitos-del-sistema)

### Librerías Utilizadas
- **Lista de todas las librerías** → [`README.md#librerías-utilizadas`](README.md#librerías-utilizadas)
- **Detalles de OpenSSL** → [`LIBRARIES_REFERENCE.md#openssl`](LIBRARIES_REFERENCE.md#openssl)
- **Detalles de SQLite3** → [`LIBRARIES_REFERENCE.md#sqlite3`](LIBRARIES_REFERENCE.md#sqlite3)
- **Detalles de Qt** → [`LIBRARIES_REFERENCE.md#qt-framework`](LIBRARIES_REFERENCE.md#qt-framework)
- **Detalles de STL** → [`LIBRARIES_REFERENCE.md#stl-standard-template-library`](LIBRARIES_REFERENCE.md#stl-standard-template-library)

---

## 📊 Diagrama de Relaciones entre Módulos

```
                    ┌─────────────────────────────┐
                    │   UI (LoginDialog)          │
                    │   UI (MainWindow)           │
                    └────────────┬────────────────┘
                                 │ usa
                                 ↓
                    ┌─────────────────────────────┐
                    │   APP (AuthenticationMgr)   │
                    └────┬────────────────┬───────┘
                         │ usa            │ usa
                    ┌────↓────┐      ┌────↓──────────┐
                    │   CRYPTO │      │   STORAGE     │
                    │ (CryptoM)│      │ (SQLiteCipher)│
                    └────┬────┘      └────┬──────────┘
                         │                │
                    ┌────↓────────────────↓────┐
                    │   CORE (Logging, Files)  │
                    └──────────────────────────┘
                         ↓
                    Librerías Externas:
                    - OpenSSL (Crypto)
                    - SQLite3 (DB)
                    - Qt (UI)
                    - STL (Contenedores)
```

---

## 🔍 Búsqueda por Clase/Función

### Clases

| Clase | Módulo | Ubicación | Documentación |
|-------|--------|-----------|---------------|
| `LoginDialog` | UI | `/include/LoginDialog.hpp` | [`UI_MODULE.md#3-logindialughpp`](UI_MODULE.md#3-logindialughpp) |
| `MainWindow` | UI | `/include/MainWindow.hpp` | [`UI_MODULE.md#5-mainwindowhpp`](UI_MODULE.md#5-mainwindowhpp) |
| `NewUserDialog` | UI | `/include/NewUserDialog.hpp` | [`UI_MODULE.md#3-newuserdialughpp`](UI_MODULE.md#3-newuserdialughpp) |
| `AuthenticationManager` | APP | `/include/AuthenticationManager.hpp` | [`APP_MODULE.md#clase-authenticationmanager`](APP_MODULE.md#clase-authenticationmanager) |
| `CryptoManager` | CRYPTO | `/include/CryptoManager.hpp` | [`CRYPTO_MODULE.md#clase-cryptomanager`](CRYPTO_MODULE.md#clase-cryptomanager) |
| `SQLiteCipherDB` | STORAGE | `/include/SQLiteCipherDB.hpp` | [`STORAGE_MODULE.md#clase-sqlitecipherdb`](STORAGE_MODULE.md#clase-sqlitecipherdb) |

### Funciones Públicas

| Función | Clase | Documentación |
|---------|-------|---------------|
| `authenticateUser()` | AuthenticationManager | [`APP_MODULE.md#authenticateuserconst-stdstring-username-const-stdstring-password-const`](APP_MODULE.md#authenticateuserconst-stdstring-username-const-stdstring-password-const) |
| `registerNewUser()` | AuthenticationManager | [`APP_MODULE.md#registernewuserconst-stdstring-username-const-stdstring-password-const`](APP_MODULE.md#registernewuserconst-stdstring-username-const-stdstring-password-const) |
| `hashPassword()` | CryptoManager | [`CRYPTO_MODULE.md#hashpasswordconst-stdstring-password-int-iterations--10000-const`](CRYPTO_MODULE.md#hashpasswordconst-stdstring-password-int-iterations--10000-const) |
| `verifyPassword()` | CryptoManager | [`CRYPTO_MODULE.md#verifypasswordconst-stdstring-password-const-stdstring-storedhash-const-stdstring-salt-int-iterations--10000-const`](CRYPTO_MODULE.md#verifypasswordconst-stdstring-password-const-stdstring-storedhash-const-stdstring-salt-int-iterations--10000-const) |
| `createUser()` | SQLiteCipherDB | [`STORAGE_MODULE.md#createuserconst-stdstring-username-const-stdstring-passwordhash-const-stdstring-salt-const`](STORAGE_MODULE.md#createuserconst-stdstring-username-const-stdstring-passwordhash-const-stdstring-salt-const) |
| `getUserHash()` | SQLiteCipherDB | [`STORAGE_MODULE.md#getuserhashconst-stdstring-username-stdstring-hash-stdstring-salt-const`](STORAGE_MODULE.md#getuserhashconst-stdstring-username-stdstring-hash-stdstring-salt-const) |
| `userExists()` | SQLiteCipherDB | [`STORAGE_MODULE.md#userexistsconst-stdstring-username-const`](STORAGE_MODULE.md#userexistsconst-stdstring-username-const) |
| `PrintLog()` | CORE | [`CORE_MODULE.md#funcionalidad-printlog`](CORE_MODULE.md#funcionalidad-printlog) |
| `createDirectory()` | CORE | [`CORE_MODULE.md#funcionalidad-createdirectory`](CORE_MODULE.md#funcionalidad-createdirectory) |

---

## 🔧 Flujos de Trabajo Comunes

### Quiero entender cómo funciona el login

1. Lee: [`UI_MODULE.md#logindialogcpp`](UI_MODULE.md#logindialogcpp) - Ver interfaz
2. Lee: [`APP_MODULE.md#authenticateuserconst-stdstring-username-const-stdstring-password-const`](APP_MODULE.md#authenticateuserconst-stdstring-username-const-stdstring-password-const) - Lógica
3. Lee: [`CRYPTO_MODULE.md#verifypasswordconst-stdstring-password-const-stdstring-storedhash-const-stdstring-salt-int-iterations--10000-const`](CRYPTO_MODULE.md#verifypasswordconst-stdstring-password-const-stdstring-storedhash-const-stdstring-salt-int-iterations--10000-const) - Verificación criptográfica
4. Lee: [`STORAGE_MODULE.md#getuserhashuserexists`](STORAGE_MODULE.md#getuserhashuserexists) - Acceso a BD

### Quiero mejorar la seguridad del hashing

1. Lee: [`CRYPTO_MODULE.md#algoritmo-pbkdf2-sha256-password-based-key-derivation-function-2`](CRYPTO_MODULE.md#algoritmo-pbkdf2-sha256-password-based-key-derivation-function-2)
2. Lee: [`README.md#consideraciones-de-seguridad`](README.md#consideraciones-de-seguridad)
3. Lee: [`LIBRARIES_REFERENCE.md#openssl`](LIBRARIES_REFERENCE.md#openssl)

### Quiero añadir una nueva característica a la UI

1. Lee: [`UI_MODULE.md`](UI_MODULE.md) - Entiende la estructura
2. Lee: [`LIBRARIES_REFERENCE.md#qt-framework`](LIBRARIES_REFERENCE.md#qt-framework) - Referencia de Qt
3. Consulta: [`APP_MODULE.md`](APP_MODULE.md) - Ver cómo conectar con lógica

### Quiero cambiar dónde se almacena la base de datos

1. Lee: [`STORAGE_MODULE.md#ubicación-de-almacenamiento`](STORAGE_MODULE.md#ubicación-de-almacenamiento)
2. Modifica: `/src/storage/SQLiteCipherDB.cpp` - Función `findDataBasePath()`
3. Consulta: [`LIBRARIES_REFERENCE.md#posix-portable-operating-system-interface`](LIBRARIES_REFERENCE.md#posix-portable-operating-system-interface)

---

## 💡 Consejos de Navegación

### Ctrl+F para Buscar
Usa la búsqueda del navegador para encontrar rápidamente términos:
- Busca "PBKDF2" para encontrar información sobre hashing
- Busca "sql" para encontrar información sobre base de datos
- Busca "signal" para encontrar información sobre Qt signals

### Headings y Anchors
Cada sección tiene un heading (encabezado) que puedes usar para navegar:
- `# Titulo Grande` - Sección principal
- `## Titulo Mediano` - Subsección
- `### Titulo Pequeño` - Subsubsección

### Códigos de Referencia
Los comandos y códigos están en bloques dedicados para fácil copia:
```cpp
// Puedes copiar directamente
std::string hash = hashPassword(password);
```

---

## 📌 Puntos Clave del Proyecto

### Seguridad
- ✅ PBKDF2-SHA256 con 10,000 iteraciones
- ✅ Salt único de 16 bytes por usuario
- ✅ Nunca almacena contraseñas en texto plano
- ✅ Prepared statements contra SQL injection

### Arquitectura
- ✅ Separación en capas (UI, Lógica, Datos)
- ✅ Inyección de dependencias
- ✅ Const correctness
- ✅ Manejo robusto de errores

### Tecnologías
- ✅ C++17 moderno
- ✅ OpenSSL para criptografía
- ✅ SQLite3 para persistencia
- ✅ Qt6 para interfaz gráfica

---

## 🔗 Enlaces Externos Útiles

### Documentación Oficial
- [OpenSSL Documentation](https://www.openssl.org/docs/)
- [SQLite3 C API](https://www.sqlite.org/c3ref/intro.html)
- [Qt6 Documentation](https://doc.qt.io/qt-6/)
- [C++ Standard Library](https://cppreference.com/)

### Estándares de Seguridad
- [NIST Password Guidelines](https://pages.nist.gov/800-63-3/sp800-63b.html)
- [RFC 8018 - PBKDF2](https://tools.ietf.org/html/rfc8018)
- [OWASP Password Storage](https://cheatsheetseries.owasp.org/cheatsheets/Password_Storage_Cheat_Sheet.html)

---

## ✅ Checklist de Lectura

### Para Entender el Proyecto Completo
- [ ] Leí el [`README.md`](README.md)
- [ ] Leí el [`UI_MODULE.md`](UI_MODULE.md)
- [ ] Leí el [`APP_MODULE.md`](APP_MODULE.md)
- [ ] Leí el [`CRYPTO_MODULE.md`](CRYPTO_MODULE.md)
- [ ] Leí el [`STORAGE_MODULE.md`](STORAGE_MODULE.md)
- [ ] Leí el [`CORE_MODULE.md`](CORE_MODULE.md)
- [ ] Consulté el [`LIBRARIES_REFERENCE.md`](LIBRARIES_REFERENCE.md)

### Para Modificar Código
- [ ] Entiendo la arquitectura general
- [ ] Sé qué módulo necesito modificar
- [ ] Leí la documentación de ese módulo
- [ ] Consulté la referencia de librerías si es necesario
- [ ] Verificaré que no rompo otras dependencias

---

## 📝 Notas Importantes

⚠️ **Esta documentación fue creada el 13 de febrero de 2026**
- Pueden haber cambios en el código después de esta fecha
- Siempre verifica el código fuente como referencia
- La documentación debe mantenerse actualizada con cambios de código

💡 **Si encuentras errores en la documentación:**
- Verifica el código fuente correspondiente
- Actualiza la documentación
- Reporta inconsistencias

🔐 **Recuerda:**
- La seguridad es crítica en un password manager
- Cualquier cambio en criptografía requiere revisión experta
- Prueba exhaustivamente después de modificaciones

---

**Última actualización:** 13 de febrero de 2026
**Creado por:** Sistema de Documentación Automática
**Licencia:** Mismo que el proyecto (Ver LICENSE.md)
