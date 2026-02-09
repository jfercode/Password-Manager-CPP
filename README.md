# Password Manager C++

Un gestor de contraseñas seguro desarrollado en C++ moderno con Qt para la interfaz gráfica y OpenSSL para criptografía.

## Características

- Interfaz gráfica con Qt
- Cifrado de datos con AES-256
- Almacenamiento seguro de credenciales
- Contraseña maestra para acceso

## Instalación

### Prerrequisitos

- CMake 3.16+
- Qt5
- OpenSSL
- Compilador C++17 (GCC, Clang, MSVC)

### Compilación

```bash
mkdir build
cd build
cmake ..
make
```

### Ejecución

```bash
./PasswordManager
```

## Uso

1. Ejecuta la aplicación
2. Crea una contraseña maestra
3. Agrega tus credenciales
4. Los datos se almacenan cifrados

## Seguridad

Este proyecto sigue las mejores prácticas de seguridad:
- Cifrado AES-256 para datos
- Hash PBKDF2 para contraseñas
- Manejo seguro de memoria

Ver [SECURITY.md](SECURITY.md) para más detalles.

## Licencia

Ver [LICENSE](LICENSE)
