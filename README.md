# 🔐 Password Manager - Gestor Seguro de Contraseñas

Una aplicación de escritorio desarrollada en **C++** que proporciona un gestor seguro de contraseñas con autenticación, cifrado y almacenamiento protegido.

## ✨ Características Principales

- 🔒 **Autenticación Segura** - Sistema de login con PBKDF2-SHA256
- 📝 **Registro de Usuarios** - Creación de cuentas con primer usuario administrador
- 🗝️ **Almacenamiento Seguro** - Contraseñas cifradas con hash y salt único
- 🎨 **Interfaz Gráfica** - Interfaz moderna con Qt
- 🔐 **Criptografía Robusta** - OpenSSL para operaciones criptográficas
- 💾 **Base de Datos Embebida** - SQLite local sin servidor externo
- 📊 **Logs Coloreados** - Sistema de logging con códigos ANSI para debugging

## 📋 Índice de Contenidos

1. [Flujo de la Aplicación](#flujo-de-la-aplicación)
2. [Estructura del Proyecto](#estructura-del-proyecto)
3. [Documentación](#documentación)
4. [Guía de Instalación](#guía-de-instalación)
5. [Guía de Uso](#guía-de-uso)

---

## 🔄 Flujo de la Aplicación

```
┌─────────────────────────────────────────────────────────────────┐
│              USUARIO ABRE LA APLICACIÓN                         │
└──────────────────────────┬──────────────────────────────────────┘
                           │
                           ↓
        ┌──────────────────────────────────────┐
        │  ¿Existe usuario administrador?      │
        └──────────┬──────────────────┬────────┘
                   │                  │
        NO         │                  │         SÍ
        ┌──────────↓─────┐     ┌──────↓──────────┐
        │                │     │                 │
        ↓                ↓     ↓                 ↓
    ┌────────────┐  ┌────────────────────────────┐
    │PRIMER INICIO│  │    USUARIO EXISTENTE      │
    │             │  │                            │
    │ NewUserDialog  │    LoginDialog             │
    │ (Registrar)   │    (Autenticarse)          │
    └────┬─────────┘  └────────┬─────────────────┘
         │                      │
         │                      │
         ↓                      ↓
    ┌────────────────────────────────────┐
    │  ¿Credenciales válidas?            │
    └────┬──────────────────────┬────────┘
         │                      │
      SÍ │                      │ NO
         ↓                      ↓
    ┌────────────┐         ┌────────────┐
    │  MainWindow │         │   Error    │
    │  (Abierto)  │         │ (Reintentar)
    └────────────┘         └────────────┘
```

### Detalles del Flujo

1. **Primer Inicio (Sistema Vacío)**
   - No existe usuario administrador
   - Se muestra `NewUserDialog`
   - Usuario crea su primera cuenta (administrador)
   - Se abre `MainWindow` automáticamente

2. **Usuario Existente**
   - Existe usuario administrador en BD
   - Se muestra `LoginDialog`
   - Usuario ingresa credenciales
   - Si son válidas → `MainWindow`
   - Si son inválidas → Mostrar error y reintentar

---

## 🏗️ Estructura del Proyecto

```
Password-Manager/
├── src/                          # Código fuente
│   ├── main.cpp                 # Punto de entrada
│   ├── core/                    # Módulo de utilidades
│   │   ├── Debug.cpp
│   │   └── Filesystem.cpp
│   ├── crypto/                  # Módulo criptográfico
│   │   └── CryptoManager.cpp
│   ├── storage/                 # Módulo de BD
│   │   └── SQLiteCipherDB.cpp
│   ├── app/                     # Módulo de lógica
│   │   ├── AuthenticationManager.cpp
│   │   └── InitializationManager.cpp
│   └── ui/                      # Módulo de interfaz
│       ├── LoginDialog.cpp
│       ├── MainWindow.cpp
│       └── NewUserDialog.cpp
│
├── include/                      # Headers (.hpp)
│   ├── library.hpp
│   ├── AuthenticationManager.hpp
│   ├── InitializationManager.hpp
│   ├── CryptoManager.hpp
│   ├── SQLiteCipherDB.hpp
│   ├── LoginDialog.hpp
│   ├── MainWindow.hpp
│   └── NewUserDialog.hpp
│
├── doc/                          # 📖 DOCUMENTACIÓN (IMPORTANTE)
│   ├── INDEX.md                 # 👈 COMIENZA AQUÍ
│   ├── README.md                # Documentación general
│   ├── SUMMARY.md               # Resumen ejecutivo
│   ├── ARCHITECTURE.md          # Arquitectura detallada
│   ├── APP_MODULE.md            # Módulo APP
│   ├── CRYPTO_MODULE.md         # Módulo CRYPTO
│   ├── STORAGE_MODULE.md        # Módulo STORAGE
│   ├── CORE_MODULE.md           # Módulo CORE
│   ├── UI_MODULE.md             # Módulo UI
│   └── LIBRARIES_REFERENCE.md   # Referencia de librerías
│
├── CMakeLists.txt               # Configuración de compilación
├── Makefile                     # Automatización
├── run-app.sh                   # Script de ejecución
├── docker-compose.yml           # Configuración Docker
├── Dockerfile                   # Imagen Docker
├── LICENSE                      # Licencia del proyecto
└── README.md                    # Este archivo

```

---

## 📖 Documentación

La documentación completa se encuentra en el directorio `/doc/`. 

### 🎯 Comienza Aquí

**[`doc/INDEX.md`](doc/INDEX.md)** - Índice completo y guía de navegación

### Documentos Principales

| Documento | Contenido |
|-----------|-----------|
| **[`doc/README.md`](doc/README.md)** | Introducción y características del proyecto |
| **[`doc/ARCHITECTURE.md`](doc/ARCHITECTURE.md)** | Arquitectura general, diagramas y patrones |
| **[`doc/SUMMARY.md`](doc/SUMMARY.md)** | Resumen ejecutivo de la documentación |

### Documentación por Módulo

| Módulo | Documento |
|--------|-----------|
| **APP** (Autenticación) | [`doc/APP_MODULE.md`](doc/APP_MODULE.md) |
| **CRYPTO** (Criptografía) | [`doc/CRYPTO_MODULE.md`](doc/CRYPTO_MODULE.md) |
| **STORAGE** (Base de Datos) | [`doc/STORAGE_MODULE.md`](doc/STORAGE_MODULE.md) |
| **CORE** (Utilidades) | [`doc/CORE_MODULE.md`](doc/CORE_MODULE.md) |
| **UI** (Interfaz Gráfica) | [`doc/UI_MODULE.md`](doc/UI_MODULE.md) |
| **Librerías** | [`doc/LIBRARIES_REFERENCE.md`](doc/LIBRARIES_REFERENCE.md) |

---

## ⚙️ Guía de Instalación

### Requisitos del Sistema

- **SO:** Linux/Unix (compilación específica a POSIX)
- **Compilador:** GCC/Clang con soporte C++17
- **Qt:** Qt 6.x
- **OpenSSL:** 1.1.x o superior
- **SQLite3:** Headers de desarrollo

### Instalación de Dependencias

**Debian/Ubuntu:**
```bash
sudo apt-get install build-essential cmake qt6-base-dev libssl-dev libsqlite3-dev
```

**Fedora:**
```bash
sudo dnf install gcc-c++ cmake qt6-base-devel openssl-devel sqlite-devel
```

### Compilación

```bash
# Clonar o descargar el proyecto
cd Password-Manager

# Crear y entrar al directorio de compilación
mkdir -p build && cd build

# Ejecutar CMake
cmake ..

# Compilar (usa todos los procesadores disponibles)
make -j$(nproc)

# El ejecutable estará en ./build/PasswordManager
```

---

## 🚀 Guía de Uso

### Ejecución de la Aplicación

```bash
# Desde el directorio build
./PasswordManager

# O usar el script de ejecución
../run-app.sh
```

### Primer Inicio (Primera Vez)

1. Abre la aplicación
2. Se mostrará el diálogo de registro (`NewUserDialog`)
3. Ingresa un nombre de usuario y contraseña
4. Verifica la contraseña (debe coincidir)
5. Presiona "Login" para crear la cuenta
6. Se abrirá automáticamente la ventana principal

### Inicios Posteriores (Usuarios Existentes)

1. Abre la aplicación
2. Se mostrará el diálogo de login (`LoginDialog`)
3. Ingresa tu nombre de usuario y contraseña
4. Presiona "Login"
5. Si las credenciales son válidas, se abre la ventana principal

### Ubicación de Datos

La base de datos se crea automáticamente en:
```
~/.local/share/passman/passman.db
```

Para limpiar la base de datos (resetear a estado inicial):
```bash
make clean-db
# o manualmente:
rm -f ~/.local/share/passman/passman.db
```

---

## 🔒 Seguridad

### Características de Seguridad Implementadas

✅ **PBKDF2-SHA256** - Hashing de contraseñas con 10,000 iteraciones
✅ **Salt Único** - 16 bytes aleatorios por usuario
✅ **Prepared Statements** - Prevención de SQL injection
✅ **OpenSSL** - Generación criptográficamente segura de números aleatorios
✅ **Almacenamiento Local** - Base de datos embebida sin servidor

### Limitaciones de Seguridad Conocidas

⚠️ Base de datos sin cifrado (solución futura: SQLCipher)
⚠️ Sin timeout de sesión (futura: timeout de inactividad)
⚠️ Sin auditoría de intentos fallidos (futura: tabla de logs)

---

## 📚 Términos Técnicos

| Término | Significado |
|---------|------------|
| **PBKDF2** | Password-Based Key Derivation Function 2 |
| **SHA256** | Secure Hash Algorithm 256-bit |
| **Salt** | Valor aleatorio único añadido al hash |
| **SQL Injection** | Ataque insertando código SQL malicioso |
| **Prepared Statements** | Consultas SQL parametrizadas seguras |
| **OpenSSL** | Librería de criptografía estándar |
| **SQLite** | Base de datos SQL embebida |
| **Qt** | Framework para interfaces gráficas |

---

## 🤝 Contribución

Este proyecto es de código abierto. Para contribuir:

1. Fork el repositorio
2. Crea una rama para tu feature
3. Commit tus cambios
4. Push a la rama
5. Abre un Pull Request

---

## 📄 Licencia

Este proyecto está bajo licencia [Ver LICENSE](LICENSE)

---

## ❓ Preguntas Frecuentes

**¿Dónde se almacenan mis contraseñas?**
Se almacenan en `~/.local/share/passman/passman.db` en forma de hash con salt único.

**¿Puedo exportar mis contraseñas?**
Actualmente no, pero es una característica futura planificada.

**¿Es seguro este password manager?**
Implementa estándares de seguridad modernos (PBKDF2-SHA256, salt único, prepared statements).

**¿Cómo reseteo el sistema si olvido mi contraseña?**
Ejecuta `make clean-db` para eliminar la base de datos y vuelve a ejecutar la aplicación.

---

## 📞 Soporte

Para reportar problemas o sugerencias, consulta la documentación en `/doc/` o contacta al desarrollador.

---

**Última actualización:** 16 de febrero de 2026
