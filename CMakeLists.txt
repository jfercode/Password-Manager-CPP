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

# SQLCipher - SQLite3 con cifrado transparente
# TO DO: Descomentar para primera compilación (printf test)
# pkg_check_modules(SQLCIPHER REQUIRED sqlcipher)
# Nota: SQLCipher = SQLite3 + AES-256 integrado
# Proporciona:
# - ${SQLCIPHER_LIBRARIES}: librerías a linkear
# - ${SQLCIPHER_INCLUDE_DIRS}: headers a incluir

# ============================================================================
# SECCIÓN 3: Definir Archivos Fuente y Headers
# ============================================================================

# --- Crypto Module (Operaciones criptográficas) ---
set(CRYPTO_SOURCES
    # src/crypto/.cpp
)

set(CRYPTO_HEADERS
    # include/crypto/.h
)

# --- Storage Module (Persistencia en BD) ---
set(STORAGE_SOURCES
    # src/storage/.cpp
)

set(STORAGE_HEADERS
    # include/storage/.h
)

# --- Core Module (Lógica de aplicación) ---
set(CORE_SOURCES
    # src/core/.cpp
)

set(CORE_HEADERS
    # include/core/.h
)

# --- UI Module (Interfaz gráfica Qt5) ---
set(UI_SOURCES
    # src/ui/.cpp
    src/ui/MainWindow.cpp
)

set(UI_HEADERS
    # include/ui/.h
    include/MainWindow.hpp
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
    ${MAIN_SOURCES}
    ${CRYPTO_SOURCES}
    ${STORAGE_SOURCES}
    ${CORE_SOURCES}
    ${UI_SOURCES}
)

set(HEADERS
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
    
    # Base de datos (COMENTADO TEMPORALMENTE para prueba inicial)
    # ${SQLCIPHER_LIBRARIES}
)

# Incluir directorios de headers
target_include_directories(${PROJECT_NAME} PRIVATE
    include/                    # Headers propios
    # ${SQLCIPHER_INCLUDE_DIRS}   # COMENTADO TEMPORALMENTE para prueba inicial
)
