# Módulo CORE - Utilidades Centrales

## Descripción General
El módulo `core/` proporciona funciones utilitarias fundamentales para el funcionamiento del Password Manager, incluyendo logging con colores ANSI, creación de directorios y otras funciones auxiliares. Es el corazón técnico que soporta las operaciones básicas de la aplicación.

---

## Archivos Incluidos

### 1. **Debug.hpp y Debug.cpp**
Sistema de logging con soporte para colores en terminal.

**Ubicación:** `/include/library.hpp` (declaración) e `/src/core/Debug.cpp` (implementación)

#### Funcionalidad: `PrintLog()`

**Declaración (en library.hpp):**
```cpp
template<typename... Args>
void PrintLog(std::ostream& stream, const char* format, Args... args);
```

**Propósito:** 
Función variádica para imprimir logs formateados en terminal con soporte para códigos de color ANSI.

**Parámetros:**
- `stream`: Stream de salida (std::cout, std::cerr, etc.)
- `format`: Cadena de formato similar a printf con códigos de color
- `args...`: Argumentos variables para interpolar en el formato

**Ejemplo de Uso:**
```cpp
PrintLog(std::cout, CYAN "Crypto Manager" RESET" - Hashing password...");
PrintLog(std::cout, CYAN "SQLiteCipherDB" RED " - User %s not found" RESET, username.c_str());
```

#### Códigos de Color ANSI Utilizados

Se definen constantes para códigos de color ANSI:

```cpp
#define RESET   "\033[0m"    // Reinicia a color por defecto
#define RED     "\033[31m"   // Rojo
#define GREEN   "\033[32m"   // Verde
#define YELLOW  "\033[33m"   // Amarillo
#define CYAN    "\033[36m"   // Cian
```

**Estructura de Logs:**
```
[MODULO] - Descripción
```

Ejemplo:
```
Crypto Manager - Hashing password...
SQLiteCipherDB - db running!
Authentication Manager - user john authenticated
Login Dialog - Initialazing UI...
```

**Colores por Tipo de Módulo:**
- **CYAN:** Módulos principales (CryptoManager, SQLiteCipherDB, AuthenticationManager)
- **YELLOW:** UI (LoginDialog, MainWindow)
- **GREEN:** Mensajes de éxito
- **RED:** Mensajes de error

---

### 2. **Filesystem.hpp y Filesystem.cpp**
Funciones para operaciones del sistema de archivos.

**Ubicación:** `/include/library.hpp` (declaración) e `/src/core/Filesystem.cpp` (implementación)

#### Funcionalidad: `createDirectory()`

**Declaración (en library.hpp):**
```cpp
bool createDirectory(const std::string& path);
```

**Propósito:**
Crea un directorio en la ruta especificada si no existe.

**Parámetros:**
- `path`: Ruta del directorio a crear (ej: "/home/user/.local/share")

**Retorno:**
- `bool` - `true` si el directorio existe o se crea exitosamente
- `bool` - `false` si no se puede crear

**Implementación:**
Usa funciones del sistema POSIX (`<sys/stat.h>`):
- `stat()` - Verifica si directorio existe
- `mkdir()` - Crea directorio si no existe

**Ejemplo de Uso:**
```cpp
// Crear directorio de datos de aplicación
if (!createDirectory("/home/user/.local/share/passman"))
    throw std::runtime_error("Failed to create directory");
```

**Flujo en SQLiteCipherDB:**
```cpp
std::string baseDir = homeDir + "/.local";
std::string shareDir = baseDir + "/share";
std::string appDir = shareDir + "/passman";

createDirectory(baseDir);     // ~/.local
createDirectory(shareDir);    // ~/.local/share
createDirectory(appDir);      // ~/.local/share/passman
```

---

### 3. **library.hpp**
Archivo maestro de encabezados que consolida todas las inclusiones.

**Ubicación:** `/include/library.hpp`

**Propósito:**
Centralizar todas las inclusiones de librerías estándar, Qt y propias en un único lugar.

**Contenido Principal:**

#### Librerías Estándar de C++:
```cpp
#include <iostream>      // std::cout, std::cerr, std::endl
#include <string>        // std::string
#include <vector>        // std::vector
#include <cstring>       // strcpy, strlen, etc.
#include <cstdio>        // printf, sprintf
#include <exception>     // std::exception
#include <stdexcept>     // std::runtime_error
#include <utility>       // std::pair, structured binding
#include <functional>    // std::function
#include <algorithm>     // std::sort, std::find
#include <cstdlib>       // getenv, system functions
#include <sys/stat.h>    // mkdir, stat para filesystem
```

#### Librerías Qt:
```cpp
#include <QApplication>   // Aplicación Qt principal
#include <QMainWindow>    // Ventana principal
#include <QDialog>        // Diálogos modales
#include <QLineEdit>      // Campo de entrada
#include <QPushButton>    // Botones
#include <QVBoxLayout>    // Layout vertical
#include <QHBoxLayout>    // Layout horizontal
#include <QLabel>         // Etiquetas
#include <QMessageBox>    // Cuadros de diálogo
#include <QTableWidget>   // Tabla
#include <QFont>          // Propiedades de fuente
```

#### Definiciones de Proyecto:
```cpp
// Constantes de dimensiones de ventana
#define WIDTH   1200
#define HEIGHT  600

// Códigos de color ANSI
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
```

#### Declaraciones de Funciones Utilitarias:
```cpp
// Logging
template<typename... Args>
void PrintLog(std::ostream& stream, const char* format, Args... args);

// Filesystem
bool createDirectory(const std::string& path);
```

#### Declaraciones Forward de Clases:
```cpp
class MainWindow;
class LoginDialog;
class NewUserDialog;
class AuthenticationManager;
class CryptoManager;
class SQLiteCipherDB;
```

**Ventajas de este Enfoque:**
1. **Centralización:** Todas las inclusiones en un lugar
2. **Consistencia:** Mismo orden y organización
3. **Mantenibilidad:** Cambios globales fáciles
4. **Reducción de Conflictos:** Evita inclusiones múltiples
5. **Claridad:** Define constantes globales y colores

**Orden de Inclusiones:**
1. Librerías estándar C++ (iostream, string, vector, etc.)
2. Librerías del sistema (sys/stat.h)
3. Librerías de terceros (Qt)
4. Headers propios (implícitos mediante funciones)

---

## Flujo de Utilidades Centrales

### Sistema de Logging:
```
Cualquier módulo
    ↓
PrintLog(std::cout, CYAN "Módulo" RESET " - mensaje", args...)
    ↓
Formatea cadena con códigos ANSI
    ↓
Imprime en stdout/stderr con colores
    ↓
Terminal muestra con colores si soporta ANSI
```

### Creación de Directorios:
```
SQLiteCipherDB::findDataBasePath()
    ↓
createDirectory("$HOME/.local")
    ├─ stat() verifica si existe
    ├─ mkdir() crea si no existe
    ↓
createDirectory("$HOME/.local/share")
    ↓
createDirectory("$HOME/.local/share/passman")
    ↓
dbPath = "$HOME/.local/share/passman/passman.db"
```

---

## Librerías Utilizadas en CORE

### Librería Estándar de C++ (STL)
- **`<iostream>`** - Input/output streams (std::cout, std::cerr)
- **`<string>`** - Cadenas Unicode (std::string)
- **`<vector>`** - Vectores dinámicos (std::vector)
- **`<cstdio>`** - Funciones printf/sprintf para formateo
- **`<exception>`** - Manejo de excepciones
- **`<utility>`** - Pares y structured binding (C++17)

### Librerías del Sistema POSIX
- **`<sys/stat.h>`** - Funciones de sistema de archivos:
  - `mkdir(const char *path, mode_t mode)` - Crea directorio
  - `stat(const char *path, struct stat *sb)` - Obtiene información de archivo
  - `S_ISDIR(st_mode)` - Macro para verificar si es directorio

### Qt Framework
- **QtCore:** QCoreApplication, QObject, QString
- **QtGui:** QGuiApplication, QPalette
- **QtWidgets:** QApplication, QMainWindow, QDialog, QLineEdit, QPushButton, QLabel, QVBoxLayout, QHBoxLayout, QMessageBox, QTableWidget, QFont

---

## Variables de Entorno

### HOME
```cpp
const char *homeEnv = std::getenv("HOME");
```

**Propósito:** Obtener la ruta del directorio home del usuario
**Formato:** /home/username
**Uso:** Base para construir ruta de base de datos

**Ejemplo:**
```
USER: javier
HOME: /home/javier
Database: /home/javier/.local/share/passman/passman.db
```

---

## Modos de Permisos en mkdir()

```cpp
mkdir(path.c_str(), 0755);
```

**0755 en Octal:**
- **7 (rwx):** Propietario - lectura, escritura, ejecución
- **5 (r-x):** Grupo - lectura, ejecución
- **5 (r-x):** Otros - lectura, ejecución

**Resultado:** Directorio legible/ejecutable por todos, escribible solo por propietario

---

## Ejemplo Completo de Inicialización

```cpp
// En SQLiteCipherDB::findDataBasePath()

1. Obtener HOME:
   homeEnv = "/home/javier"

2. Construir rutas:
   baseDir = "/home/javier/.local"
   shareDir = "/home/javier/.local/share"
   appDir = "/home/javier/.local/share/passman"

3. Crear directorios:
   createDirectory(baseDir)
   - stat("/home/javier/.local") - No existe
   - mkdir("/home/javier/.local", 0755) - Crea
   
   createDirectory(shareDir)
   - stat("/home/javier/.local/share") - No existe
   - mkdir("/home/javier/.local/share", 0755) - Crea
   
   createDirectory(appDir)
   - stat("/home/javier/.local/share/passman") - No existe
   - mkdir("/home/javier/.local/share/passman", 0755) - Crea

4. Asignar ruta:
   dbPath = "/home/javier/.local/share/passman/passman.db"

5. sqlite3_open(dbPath, &db)
   - Crea archivo passman.db en el directorio
```

---

## Patrones de Programación

### 1. **Printf-style Formatting con Variadic Templates**
```cpp
template<typename... Args>
void PrintLog(std::ostream& stream, const char* format, Args... args) {
    // Expande los argumentos variádicos
    // Formatea similar a printf pero en C++ moderno
}
```

**Ventajas:**
- Type-safe (a diferencia de printf)
- Compatible con std::ostream
- Soporta colores ANSI

### 2. **Centralización de Headers**
```cpp
// library.hpp incluye todo lo necesario
#include "../include/library.hpp"

// Los .cpp files solo incluyen library.hpp
// Evita conflictos de inclusión múltiple
```

### 3. **RAII para Sistema de Archivos**
```cpp
// createDirectory() es RAII en espíritu
// Verifica condiciones antes de actuar
// No mantiene recursos abiertos
```

---

## Consideraciones de Seguridad

1. **Validación de HOME:** Verifica que GETENV retorna valor válido
2. **Permisos de Directorio:** 0755 permite lectura a otros pero solo propietario modifica
3. **Error Handling:** createDirectory() retorna bool, no lanza
4. **Path Traversal:** Las rutas se construyen seguramente sin manipulación

---

## Limitaciones Conocidas

1. **Específico a POSIX:** mkdir() es POSIX, no funciona en Windows sin adaptación
2. **Permisos Fijos:** No permite configurar permisos personalizados
3. **Colores ANSI:** No soporta Windows sin librerías adicionales
4. **Atomicidad:** crear "/a/b/c" requiere tres llamadas separadas

---

## Extensiones Futuras

1. **Portabilidad Windows:** Usar `_mkdir()` en Windows
2. **Configuración de Logging:** Nivel de verbosidad ajustable
3. **Logging a Archivo:** Escribir logs en archivo además de stdout
4. **Stack Traces:** Mostrar stack trace en errores
5. **Performance Monitoring:** Medir tiempos de operaciones
6. **Internacionalización:** Logs en múltiples idiomas

---

## Notas Técnicas

- **Header Guard:** library.hpp usa `#ifndef` para prevenir inclusiones múltiples
- **Inline Functions:** Las funciones template se expanden inline
- **Const Correctness:** Las funciones de lectura de entorno son const
- **Memory Efficiency:** PrintLog no copia argumentos innecesariamente
- **Compatibility:** Compatible con C++11 en adelante, optimizado para C++17

---

## Relación con Otros Módulos

```
┌─────────────────────────────────────┐
│         library.hpp (CORE)          │
├─────────────────────────────────────┤
│ - PrintLog() para todos             │
│ - createDirectory() para Storage    │
│ - Constantes globales               │
└─────────────────────────────────────┘
         ↓
├─── CryptoManager
├─── SQLiteCipherDB
├─── AuthenticationManager
├─── LoginDialog
├─── MainWindow
└─── NewUserDialog
```

Todos los módulos dependen de `library.hpp` para:
- Logging con colores
- Constantes (WIDTH, HEIGHT)
- Inclusiones de Qt
- Inclusiones de STL
