################################################################################
#                    PASSWORD MANAGER C++ - Dockerfile
#                   Debian Slim - Desarrollo sin GUI
################################################################################

# Base image: Debian slim (ligero, ~60MB)
FROM debian:bookworm-slim

# Metadatos
LABEL maintainer="jfercode"
LABEL description="Password Manager C++ Development Environment"

# ============================================================================
# SECCIÓN 1: Actualizar repositorios e instalar herramientas base
# ============================================================================

RUN apt-get update && apt-get install -y \
    # Build tools
    build-essential \
    cmake \
    git \
    curl \
    wget \
    pkg-config \
    # Necesario para compilar
    && rm -rf /var/lib/apt/lists/*

# ============================================================================
# SECCIÓN 2: Instalar Qt5 (sin interfaz gráfica)
# ============================================================================

RUN apt-get update && apt-get install -y \
    qt5-qmake \
    qtbase5-dev \
    libqt5core5a \
    libqt5gui5 \
    libqt5widgets5 \
    # Clean up
    && rm -rf /var/lib/apt/lists/*

# ============================================================================
# SECCIÓN 3: Instalar OpenSSL (criptografía)
# ============================================================================

RUN apt-get update && apt-get install -y \
    libssl-dev \
    libcrypto++-dev \
    openssl \
    # Clean up
    && rm -rf /var/lib/apt/lists/*

# ============================================================================
# SECCIÓN 4: Instalar SQLite3
# ============================================================================

RUN apt-get update && apt-get install -y \
    sqlite3 \
    libsqlite3-dev \
    # Clean up
    && rm -rf /var/lib/apt/lists/*

# ============================================================================
# SECCIÓN 5: Compilar SQLCipher desde fuente
# ============================================================================

RUN apt-get update && apt-get install -y \
    autoconf \
    libtool \
    && rm -rf /var/lib/apt/lists/*

# Descargar y compilar SQLCipher
RUN cd /tmp && \
    git clone https://github.com/sqlcipher/sqlcipher.git && \
    cd sqlcipher && \
    ./configure --prefix=/usr/local \
                CFLAGS="-DSQLITE_HAS_CODEC -DSQLITE_EXTRA_INIT=sqlcipher_extra_init -DSQLITE_EXTRA_SHUTDOWN=sqlcipher_extra_shutdown -DSQLITE_TEMP_STORE=3" \
                LDFLAGS="-lcrypto" && \
    make && \
    make install && \
    ldconfig && \
    rm -rf /tmp/sqlcipher

# ============================================================================
# SECCIÓN 6: Setup del contenedor
# ============================================================================

# Directorio de trabajo
WORKDIR /app

# Crear usuario no-root (opcional, aquí usamos root para desarrollo)
# RUN useradd -m -s /bin/bash developer

# ============================================================================
# ENTRYPOINT: Bash interactivo
# ============================================================================

CMD ["/bin/bash"]
