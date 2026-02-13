#!/bin/bash

# Script para ejecutar PasswordManager evitando conflictos con Snap
# El problema: Snap inyecta librerías viejas que causan GLIBC_PRIVATE errors
# La solución: ejecutar con un entorno limpio que NO incluya Snap

# Ejecutar con env -i (entorno limpio) pero preservando variables esenciales
exec env -i \
    PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" \
    HOME="$HOME" \
    DISPLAY="${DISPLAY}" \
    XAUTHORITY="${XAUTHORITY}" \
    DBUS_SESSION_BUS_ADDRESS="${DBUS_SESSION_BUS_ADDRESS}" \
    /home/javier/Escritorio/Password-Manager-CPP/build/PasswordManager "$@"
