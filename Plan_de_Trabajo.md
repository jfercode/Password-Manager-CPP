# 📌 Plan de Trabajo – Password Manager en C++

Proyecto orientado a **portfolio profesional**, enfocado en C++ moderno, arquitectura limpia y fundamentos de seguridad.

**Estado Actual:** 🟨 **Fase 2 - Autenticación y Seguridad Base (EN PROGRESO)**

---

## 🎯 Objetivo del Proyecto

Desarrollar una aplicación de escritorio en C++ que permita:

* Almacenar credenciales (usuario + contraseña)
* Proteger el acceso mediante una **contraseña maestra**
* Cifrar todos los datos almacenados
* Ofrecer una interfaz gráfica clara y segura

El proyecto debe demostrar:

* Buen dominio de **C++ moderno (C++17/20)**
* Separación de responsabilidades
* Uso correcto de librerías externas
* Conciencia de seguridad (sin implementar criptografía casera)

---

## 🧱 Stack Tecnológico

* **Lenguaje:** C++17
* **GUI:** Qt5 (Qt Widgets)
* **Build System:** CMake 3.28
* **Cifrado:** OpenSSL 3.0.13
* **Persistencia:** SQLCipher (SQLite3 cifrado)
* **Containerización:** Docker + Docker Compose
* **Control de versiones:** Git + GitHub
* **Compiladores:** GCC 13.3.0

---

## 🗂️ Estructura del Proyecto

```
PasswordManagerCPP/
├── src/
│   ├── main.cpp
│   ├── app/
│   │   └── AuthenticationManager.cpp
│   ├── ui/
│   │   ├── LoginDialog.cpp
│   │   ├── MainWindow.cpp
│   │   └── NewUserDialog.cpp
│   ├── crypto/
│   │   └── CryptoManager.cpp
│   ├── storage/
│   │   └── SQLiteCipherDB.cpp
│   └── core/
│       ├── Debug.cpp
│       └── Filesystem.cpp
├── include/
│   ├── AuthenticationManager.hpp
│   ├── LoginDialog.hpp
│   ├── MainWindow.hpp
│   ├── NewUserDialog.hpp
│   ├── CryptoManager.hpp
│   ├── SQLiteCipherDB.hpp
│   └── library.hpp
├── build/
│   └── PasswordManager (binario compilado)
├── CMakeLists.txt
├── Dockerfile
├── docker-compose.yml
├── Makefile
├── README.md
├── Plan_de_Trabajo.md
├── SECURITY.md
└── run-app.sh (wrapper para evitar conflicto de librerías)
```

---

## 🗺️ Roadmap de Desarrollo

### ✅ Fase 1 – Diseño y Setup (Completada)

**Objetivos:**
* Definir arquitectura
* Preparar entorno de desarrollo

**Tareas Completadas:**
* ✅ Crear repositorio Git con estructura inicial
* ✅ Configurar CMake con soporte para Qt5, OpenSSL, SQLCipher
* ✅ Integrar Qt5 (Widgets)
* ✅ Definir módulos principales (app, ui, crypto, storage, core)
* ✅ Boceto de UI (LoginDialog, MainWindow, NewUserDialog)
* ✅ Configurar Docker con todas las dependencias
* ✅ Setup de Makefile para compilación y ejecución

**Entregables:**
* ✅ Proyecto compila sin errores
* ✅ Ventana básica Qt funcional
* ✅ Estructura modular clara

---

### 🟨 Fase 2 – Autenticación y Seguridad Base (EN PROGRESO)

**Objetivos:**
* Implementar contraseña maestra
* Evitar almacenamiento inseguro

**Tareas en Progreso:**
* 🔄 Hash de contraseña maestra (PBKDF2 - Implementado)
* 🔄 Validación de login (Parcialmente implementado)
* 🔄 Manejo seguro de memoria sensible (En desarrollo)
* 🔄 Autenticación entre formularios de login

**Entregables Esperados:**
* ✅ Login funcional con validación
* ✅ Hash almacenado de forma segura
* ✅ Interfaz de usuario fluida (login → new user → main)
* Gestión de sesión

**Problemas Resueltos en esta Fase:**
* ✅ Error GLIBC por conflicto con Snap (Docker + wrapper)
* ✅ Compilación consistente en múltiples máquinas
* ✅ Integración de SQLCipher con CMake

---

### 🟩 Fase 3 – Cifrado y Persistencia (Próxima)

**Objetivos:**
* Proteger los datos almacenados

**Tareas Planeadas:**
* Implementar cifrado simétrico (AES-256)
* Gestión de claves criptográficas
* Guardar y cargar datos cifrados
* Serialización de estructuras de datos

**Entregables:**
* Archivo cifrado funcional
* Datos ilegibles fuera de la app
* Interfaz para agregar/guardar contraseñas

---

### 🟩 Fase 4 – Gestión de Contraseñas (Próxima)

**Objetivos:**
* CRUD completo de credenciales

**Tareas Planeadas:**
* Agregar / editar / eliminar contraseñas
* Estructuras de datos eficientes
* Ocultar/mostrar contraseñas en UI
* Búsqueda y filtrado básico
* Auto-lock por inactividad

**Entregables:**
* Password manager completamente usable
* UI intuitiva y responsive

---

### 🟦 Fase 5 – Refactor y Calidad (Posterior)

**Objetivos:**
* Mejorar calidad del código

**Tareas Planeadas:**
* Refactor a C++ moderno (STL, smart pointers)
* Manejo robusto de errores
* Limpieza de secretos en memoria
* Comentarios técnicos claros
* Logging y debugging mejorado

**Entregables:**
* Código limpio y mantenible
* Fácil de auditar

---

### 🟦 Fase 6 – Extras de Portfolio (Opcional)

**Objetivos:**
* Diferenciar el proyecto

**Tareas Planeadas:**
* Generador de contraseñas seguras
* Auto-lock por inactividad
* Tests unitarios básicos
* GitHub Actions (CI/CD)
* Exportación de credenciales (CSV cifrado)
* Historial de cambios
* Sincronización básica

**Entregables:**
* Proyecto "portfolio-ready"

---

## 🧪 Testing

### Pruebas Realizadas (Fase 1-2)

* ✅ Compilación en Docker
* ✅ Ejecución en máquinas con Snap (error GLIBC solucionado)
* ✅ Interfaz gráfica básica (login, creación de usuario)
* ✅ Hash PBKDF2 de contraseña maestra

### Pruebas Planeadas (Fase 3+)

* Pruebas unitarias de:
  * Cifrado / descifrado (AES-256)
  * Lógica de negocio (CRUD de credenciales)
  * Hash de contraseñas
* Pruebas manuales de UI
* Pruebas de seguridad (intentos de extracción de secretos)

---

## 📘 Documentación

### README.md
* ✅ Descripción del proyecto
* ✅ Tecnologías usadas
* ✅ Arquitectura modular
* ✅ Instrucciones de compilación (Docker y local)
* ✅ Solución de problemas conocidos
* ✅ Comandos útiles (Makefile)

### SECURITY.md
* ✅ Amenazas cubiertas
* ✅ Limitaciones conocidas
* ✅ Detalles de implementación criptográfica
* ✅ Directrices de uso seguro
* ✅ Mejoras planeadas

### Plan_de_Trabajo.md
* ✅ Este documento
* ✅ Roadmap completo
* ✅ Estado de cada fase

---

## ✅ Criterios de Finalización

### Fase 2 Actual

* [x] El proyecto compila sin errores
* [x] No hay contraseñas en texto plano
* [x] Arquitectura clara y modular
* [x] README profesional
* [ ] Login completamente funcional
* [ ] Almacenamiento seguro de hash

### Fase 6 (Final)

* [ ] El proyecto compila sin errores
* [ ] No hay contraseñas en texto plano
* [ ] Arquitectura clara y modular
* [ ] README profesional
* [ ] Código entendible por terceros
* [ ] Tests unitarios básicos
* [ ] Documentación de seguridad completa
* [ ] CI/CD funcional

---

## 🚀 Resultado Esperado

Un proyecto que demuestre capacidad real en:

* Desarrollo en C++ moderno
* Diseño de software (arquitectura modular)
* Uso responsable de criptografía
* Aplicaciones de escritorio profesionales
* Containerización y DevOps básico
* Documentación técnica clara

Ideal para **portfolio junior o mid-level**.

---

## 📊 Métricas del Proyecto

| Métrica | Valor |
|---------|-------|
| **Líneas de Código** | ~2000 |
| **Módulos Principales** | 6 |
| **Dependencias Externas** | 5 (Qt5, OpenSSL, SQLCipher, etc.) |
| **Fases Completadas** | 1/6 |
| **Fases en Progreso** | 1/6 |
| **Compiladores Soportados** | GCC, Clang, MSVC |
| **Plataformas** | Linux, macOS, Windows (potencial) |

---

## 🔗 Enlaces Útiles

- [Documentación Qt5](https://doc.qt.io/qt-5/)
- [OpenSSL EVP](https://www.openssl.org/docs/manmaster/man3/EVP_BytesToKey.html)
- [SQLCipher Documentation](https://www.zetetic.net/sqlcipher/)
- [OWASP Password Storage](https://cheatsheetseries.owasp.org/cheatsheets/Password_Storage_Cheat_Sheet.html)

---

**Última actualización:** 13 de Febrero de 2026  
**Mantenedor:** jfercode  
**Estado:** En Desarrollo Activo

### 🟩 Fase 3 – Cifrado y Persistencia (Semana 3)

**Objetivos:**

* Proteger los datos almacenados

**Tareas:**

* Implementar cifrado simétrico (AES-256)
* Gestión de claves
* Guardar y cargar datos cifrados
* Serialización de estructuras

**Entregables:**

* Archivo cifrado funcional
* Datos ilegibles fuera de la app

---

### 🟩 Fase 4 – Gestión de Contraseñas (Semana 4)

**Objetivos:**

* CRUD completo de credenciales

**Tareas:**

* Agregar / editar / eliminar contraseñas
* Estructuras de datos eficientes
* Ocultar contraseñas en UI
* Búsqueda básica

**Entregables:**

* Password manager completamente usable

---

### 🟦 Fase 5 – Refactor y Calidad (Semana 5)

**Objetivos:**

* Mejorar calidad del código

**Tareas:**

* Refactor a C++ moderno
* Manejo de errores
* Limpieza de secretos en memoria
* Comentarios técnicos claros

**Entregables:**

* Código limpio y mantenible

---

### 🟦 Fase 6 – Extras de Portfolio (Semana 6 – Opcional)

**Objetivos:**

* Diferenciar el proyecto

**Tareas:**

* Generador de contraseñas
* Auto-lock por inactividad
* Tests unitarios básicos
* GitHub Actions (build)

**Entregables:**

* Proyecto "portfolio-ready"

---

## 🧪 Testing

* Pruebas unitarias de:

  * Cifrado / descifrado
  * Lógica de negocio
* Pruebas manuales de UI

---

## 📘 Documentación

### README.md

* Descripción del proyecto
* Tecnologías usadas
* Arquitectura
* Seguridad (qué se hace y por qué)
* Screenshots
* Instrucciones de compilación

### SECURITY.md

* Qué amenazas cubre
* Qué NO cubre
* Decisiones de diseño

---

## ✅ Criterios de Finalización

* El proyecto compila sin errores
* No hay contraseñas en texto plano
* Arquitectura clara y modular
* README profesional
* Código entendible por terceros

---

## 🚀 Resultado Esperado

Un proyecto que demuestre capacidad real en:

* Desarrollo en C++
* Diseño de software
* Uso responsable de criptografía
* Aplicaciones de escritorio profesionales

Ideal para **portfolio junior o mid-level**.
