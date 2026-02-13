################################################################################
#                      PASSWORD MANAGER C++ - CMakeLists.txt
#                           Fase 1: Build Configuration
################################################################################

# ============================================================================
# SECCIÓN 1: Configuración Básica
# ============================================================================

cmake_minimum_required(VERSION 3.16)    # Versión mínima de CMake requerida
project(PasswordManager)                # Nombre del proyecto

# C++ Standard Configuration
set(CMAKE_CXX_STANDARD 17)              # Usar C++17 (smart pointers, optional, etc.)
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # Obligatorio: C++17 o error

# Qt5 Automation (Muy importante para Qt)
set(CMAKE_AUTOMOC ON)                   # Meta-Object Compiler automático (signals/slots)
set(CMAKE_AUTORCC ON)                   # Resource Compiler automático (.qrc files)
set(CMAKE_AUTOUIC ON)                   # User Interface Compiler automático (.ui files)

# Add compiler flags
add_compile_options(-Wall -Wextra -Werror -I include/)

# ============================================================================
# SECCIÓN 2: Buscar Dependencias Externas
# ============================================================================

# Qt5 - GUI Framework
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
# - Core: funcionalidad base de Qt5
# - Gui: manejo de gráficos y eventos
# - Widgets: componentes UI (botones, ventanas, tablas)

# OpenSSL - Criptografía
find_package(OpenSSL REQUIRED)
# Necesario para:
# - PBKDF2 (derivación de contraseña maestra)
# - AES-256 (cifrado de credenciales)
# - SHA-256 (hashing)

# PkgConfig - Herramienta para encontrar librerías
find_package(PkgConfig REQUIRED)

# Set PKG_CONFIG_PATH to include local installations
set(ENV{PKG_CONFIG_PATH} "/usr/local/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")

# SQLCipher - SQLite3 con cifrado transparente
# Note: After compilation, SQLCipher registers as 'sqlite3' in pkg-config
pkg_check_modules(SQLCIPHER REQUIRED sqlite3)
# Nota: SQLCipher = SQLite3 + AES-256 integrado
# Proporciona:
# - ${SQLCIPHER_LIBRARIES}: librerías a linkear
# - ${SQLCIPHER_INCLUDE_DIRS}: headers a incluir

# ============================================================================
# SECCIÓN 3: Definir Archivos Fuente y Headers
# ============================================================================

set (APP_SOURCES
    src/app/AuthenticationManager.cpp
)

set (APP_HEADERS
    include/AuthenticationManager.hpp
)


# --- Crypto Module (Operaciones criptográficas) ---
set(CRYPTO_SOURCES
    src/crypto/CryptoManager.cpp
)

set(CRYPTO_HEADERS
    include/CryptoManager.hpp
)

# --- Storage Module (Persistencia en BD) ---
set(STORAGE_SOURCES
    src/storage/SQLiteCipherDB.cpp
)

set(STORAGE_HEADERS
    include/SQLiteCipherDB.hpp
)

# --- Core Module (Lógica de aplicación) ---
set(CORE_SOURCES
    # src/core/.cpp
    src/core/Debug.cpp
    src/core/Filesystem.cpp

)

set(CORE_HEADERS
    # include/core/.h
)

# --- UI Module (Interfaz gráfica Qt5) ---
set(UI_SOURCES
    src/ui/MainWindow.cpp
    src/ui/LoginDialog.cpp
    src/ui/NewUserDialog.cpp
)

set(UI_HEADERS
    include/MainWindow.hpp
    include/LoginDialog.hpp
    include/NewUserDialog.hpp
)

# --- Qt Designer UI Files ---
set(UI_FORMS
    # src/ui/.ui
)

# --- Main Entry Point ---
set(MAIN_SOURCES
    src/main.cpp
)

# --- Combine All Files ---
set(SOURCES
    ${APP_SOURCES}
    ${MAIN_SOURCES}
    ${CRYPTO_SOURCES}
    ${STORAGE_SOURCES}
    ${CORE_SOURCES}
    ${UI_SOURCES}
)

set(HEADERS
    ${APP_HEADERS}
    ${CRYPTO_HEADERS}
    ${STORAGE_HEADERS}
    ${CORE_HEADERS}
    ${UI_HEADERS}
)

set(ALL_FILES ${SOURCES} ${HEADERS} ${UI_FORMS})

# ============================================================================
# SECCIÓN 4: Crear Ejecutable y Linkear Librerías
# ============================================================================

# Crear ejecutable
add_executable(${PROJECT_NAME} ${ALL_FILES})

# Linkear librerías necesarias
target_link_libraries(${PROJECT_NAME} PRIVATE
    # Qt5 Libraries
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    
    # Criptografía
    OpenSSL::Crypto
    
    # Base de datos
    ${SQLCIPHER_LIBRARIES}
)

# Incluir directorios de headers
target_include_directories(${PROJECT_NAME} PRIVATE
    include/                    # Headers propios
    ${SQLCIPHER_INCLUDE_DIRS}   # SQLite Cipher headers
)
