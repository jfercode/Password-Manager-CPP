# 📌 Plan de Trabajo – Password Manager en C++

Proyecto orientado a **portfolio profesional**, enfocado en C++ moderno, arquitectura limpia y fundamentos de seguridad.

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

* **Lenguaje:** C++17 o C++20
* **GUI:** Qt (Qt Widgets)
* **Build System:** CMake
* **Cifrado:** OpenSSL o Crypto++
* **Persistencia:** Archivo binario cifrado o SQLite cifrado
* **Control de versiones:** Git + GitHub

---

## 🗂️ Estructura del Proyecto

```
PasswordManagerCPP/
├── src/
│   ├── main.cpp
│   ├── app/
│   ├── ui/
│   ├── crypto/
│   ├── storage/
│   └── core/
├── include/
├── tests/
├── resources/
├── CMakeLists.txt
├── README.md
└── SECURITY.md
```

---

## 🗺️ Roadmap de Desarrollo

### 🟩 Fase 1 – Diseño y Setup (Semana 1)

**Objetivos:**

* Definir arquitectura
* Preparar entorno de desarrollo

**Tareas:**

* Crear repositorio Git
* Configurar CMake
* Integrar Qt
* Definir módulos principales
* Boceto simple de UI (login + vista principal)

**Entregables:**

* Proyecto compila
* Ventana básica Qt

---

### 🟩 Fase 2 – Autenticación y Seguridad Base (Semana 2)

**Objetivos:**

* Implementar contraseña maestra
* Evitar almacenamiento inseguro

**Tareas:**

* Hash de contraseña maestra (PBKDF2 / bcrypt / Argon2)
* Validación de login
* Manejo seguro de memoria sensible

**Entregables:**

* Login funcional
* Hash almacenado de forma segura

---

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
