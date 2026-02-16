################################################################################
# CMAKE & BUILD COMMANDS
# ============================================================================

# Detectar si estamos dentro del contenedor
INSIDE_CONTAINER := $(shell [ -f /.dockerenv ] && echo 1 || echo 0)

ifeq ($(INSIDE_CONTAINER),1)
# DENTRO DEL CONTENEDOR - ejecutar directamente
cmake:
	@echo "$(BLUE)📦 Ejecutando cmake (dentro del contenedor)...$(NC)"
	cd /app && mkdir -p build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug

compile:
	@echo "$(BLUE)🔨 Compilando (dentro del contenedor)...$(NC)"
	cd /app/build && make -j$$(nproc)
	@echo "$(GREEN)✓ Compilación exitosa$(NC)"

else
# FUERA DEL CONTENEDOR - usar docker-compose
cmake:
	@echo "$(BLUE)📦 Ejecutando cmake...$(NC)"
	docker-compose exec app bash -c "cd /app && mkdir -p build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug"

compile:
	@echo "$(BLUE)🔨 Compilando...$(NC)"
	docker-compose exec app bash -c "cd /app/build && make -j$$(nproc)"
	@echo "$(GREEN)✓ Compilación exitosa$(NC)"

endif

run: build-all
	@echo "$(BLUE)▶️  Ejecutando aplicación...$(NC)"
	@if [ -f ./run-app.sh ]; then \
		./run-app.sh; \
	else \
        echo "$(RED)✗ No se ha encontrado el script, por favor ejecuta primero$(NC)make build-all"; \
	fi

################################################################################

# Variables
PROJECT_NAME := PasswordManager
DOCKER_IMAGE := password-manager:dev
DOCKER_CONTAINER := password-manager-dev
BUILD_DIR := build
BIN_PATH := $(BUILD_DIR)/$(PROJECT_NAME)

# Colores para output
GREEN := \033[0;32m
BLUE := \033[0;34m
YELLOW := \033[0;33m
RED := \033[0;31m
NC := \033[0m # No Color

# ============================================================================
# PHONY TARGETS (no son archivos reales)
# ============================================================================

.PHONY: help docker-build docker-up docker-down docker-bash docker-clean \
        cmake compile run setup dev clean distclean clean-db reset-db status logs

# ============================================================================
# INFORMACIÓN Y AYUDA
# ============================================================================

help:
	@echo "$(BLUE)╔════════════════════════════════════════════════════════════════╗$(NC)"
	@echo "$(BLUE)║         PASSWORD MANAGER C++ - Makefile Commands               ║$(NC)"
	@echo "$(BLUE)╚════════════════════════════════════════════════════════════════╝$(NC)"
	@echo ""
	@echo "$(GREEN)DOCKER COMMANDS:$(NC)"
	@echo "  $(YELLOW)make docker-build$(NC)     - Construir imagen Docker"
	@echo "  $(YELLOW)make docker-up$(NC)        - Iniciar contenedor en background"
	@echo "  $(YELLOW)make docker-bash$(NC)      - Entrar a terminal del contenedor"
	@echo "  $(YELLOW)make docker-down$(NC)      - Detener contenedor"
	@echo "  $(YELLOW)make docker-clean$(NC)     - Eliminar todo (imagen, contenedor, volúmenes)"
	@echo ""
	@echo "$(GREEN)BUILD COMMANDS (dentro del contenedor):$(NC)"
	@echo "  $(YELLOW)make cmake$(NC)            - Ejecutar cmake -B build"
	@echo "  $(YELLOW)make compile$(NC)          - Compilar con make -j\$$(nproc)"
	@echo "  $(YELLOW)make run$(NC)              - Ejecutar ./build/PasswordManager"
	@echo ""
	@echo "$(GREEN)COMBINED COMMANDS:$(NC)"
	@echo "  $(YELLOW)make setup$(NC)            - docker-build + docker-up (primera vez)"
	@echo "  $(YELLOW)make dev$(NC)              - docker-up + docker-bash (desarrollo)"
	@echo "  $(YELLOW)make build-all$(NC)        - cmake + compile (dentro del contenedor)"
	@echo ""
	@echo "$(GREEN)CLEANUP COMMANDS:$(NC)"
	@echo "  $(YELLOW)make clean$(NC)            - Limpiar build/ (artifacts)"
	@echo "  $(YELLOW)make clean-db$(NC)         - Eliminar base de datos (reset total)"
	@echo "  $(YELLOW)make reset-db$(NC)         - Alias para clean-db"
	@echo "  $(YELLOW)make distclean$(NC)        - Limpiar todo (build + docker)"
	@echo ""
	@echo "$(GREEN)DEBUG & STATUS COMMANDS:$(NC)"
	@echo "  $(YELLOW)make status$(NC)           - Ver estado del contenedor y base de datos"
	@echo "  $(YELLOW)make logs$(NC)             - Ver logs en tiempo real"
	@echo ""
	@echo "$(GREEN)EXAMPLE WORKFLOW:$(NC)"
	@echo "  1. $(YELLOW)make setup$(NC)         - Primera vez (construir + iniciar)"
	@echo "  2. $(YELLOW)make dev$(NC)           - Entrar al contenedor"
	@echo "  3. $(YELLOW)make build-all$(NC)     - Compilar dentro del contenedor"
	@echo "  4. $(YELLOW)make run$(NC)           - Ejecutar la app"
	@echo ""

# ============================================================================
# DOCKER COMMANDS
# ============================================================================

docker-build:
	@echo "$(BLUE)🐳 Construyendo imagen Docker...$(NC)"
	docker-compose build
	@echo "$(GREEN)✓ Imagen construida$(NC)"

docker-up:
	@echo "$(BLUE)🐳 Iniciando contenedor...$(NC)"
	docker-compose up -d
	@echo "$(GREEN)✓ Contenedor en ejecución$(NC)"
	@docker-compose ps

docker-bash:
	@echo "$(BLUE)🐳 Entrando al contenedor...$(NC)"
	docker-compose exec app bash

docker-down:
	@echo "$(BLUE)🐳 Deteniendo contenedor...$(NC)"
	docker-compose down
	@echo "$(GREEN)✓ Contenedor detenido$(NC)"

docker-clean:
	@echo "$(RED)🐳 Limpiando Docker (imagen, contenedor, volúmenes)...$(NC)"
	docker-compose down -v
	docker image rm $(DOCKER_IMAGE) 2>/dev/null || true
	@echo "$(GREEN)✓ Docker limpio$(NC)"

# ============================================================================
# COMBINED COMMANDS
# ============================================================================

setup: docker-build docker-up
	@echo "$(GREEN)✓ Setup completado$(NC)"
	@echo "$(YELLOW)Próximo paso: make dev$(NC)"

dev: docker-up docker-bash

build-all: cmake compile 
	@echo "$(GREEN)✓ Build completado$(NC)"
	@echo "$(YELLOW)Próximo paso: make run$(NC)"

quick-build:
	@echo "$(BLUE)🚀 Build rápido (cmake + compile)...$(NC)"
	docker-compose exec app bash -c "cd /app/$(BUILD_DIR) && cmake .. && make -j$$(nproc)"

# ============================================================================
# CLEANUP COMMANDS
# ============================================================================

clean:
	@echo "$(RED)🧹 Limpiando artifacts de build...$(NC)"
	rm -rf $(BUILD_DIR)
	@echo "$(GREEN)✓ Limpieza completa$(NC)"

# ============================================================================
# DATABASE COMMANDS
# ============================================================================

clean-db:
	@echo "$(RED)🗑️  Eliminando base de datos...$(NC)"
	@rm -f ~/.local/share/passman/passman.db && echo "$(GREEN)✓ Base de datos eliminada$(NC)" || echo "$(YELLOW)⚠️  Base de datos no encontrada (ya estaba limpia)$(NC)"

reset-db: clean-db
	@echo "$(GREEN)✓ Reset de base de datos completado$(NC)"
	@echo "$(YELLOW)La aplicación creará una nueva base de datos en la próxima ejecución$(NC)"

distclean: clean docker-clean
	@echo "$(GREEN)✓ Distclean completado (proyecto limpio)$(NC)"

# ============================================================================
# DEBUG & STATUS
# ============================================================================

status:
	@echo "$(BLUE)╔════════════════════════════════════════════════════════════════╗$(NC)"
	@echo "$(BLUE)║              ESTADO DEL PROYECTO PASSWORD MANAGER             ║$(NC)"
	@echo "$(BLUE)╚════════════════════════════════════════════════════════════════╝$(NC)"
	@echo ""
	@echo "$(GREEN)► ESTADO DEL CONTENEDOR DOCKER:$(NC)"
	@docker-compose ps 2>/dev/null || echo "$(YELLOW)  ⚠️  Contenedor no está corriendo$(NC)"
	@echo ""
	@echo "$(GREEN)► ESTADO DE LA BASE DE DATOS:$(NC)"
	@bash -c 'RED="\033[0;31m"; GREEN="\033[0;32m"; YELLOW="\033[0;33m"; NC="\033[0m"; \
	if [ -f ~/.local/share/passman/passman.db ]; then \
		echo -e "  ✅ Base de datos: $${GREEN}PRESENTE$${NC}"; \
		echo "  📁 Ubicación: ~/.local/share/passman/passman.db"; \
		FILESIZE=$$(stat -f%z ~/.local/share/passman/passman.db 2>/dev/null || stat -c%s ~/.local/share/passman/passman.db 2>/dev/null); \
		if [ -n "$$FILESIZE" ]; then \
			echo "  💾 Tamaño: $$FILESIZE bytes"; \
		fi; \
		TIMESTAMP=$$(stat -f%Sm -t "%Y-%m-%d %H:%M:%S" ~/.local/share/passman/passman.db 2>/dev/null || stat -c%y ~/.local/share/passman/passman.db 2>/dev/null | cut -d" " -f1-2); \
		if [ -n "$$TIMESTAMP" ]; then \
			echo "  🕐 Última modificación: $$TIMESTAMP"; \
		fi; \
		echo ""; \
		echo -e "  $${YELLOW}📊 ESTADÍSTICAS DE LA BASE DE DATOS:$${NC}"; \
		if command -v sqlite3 &> /dev/null; then \
			USERS_COUNT=$$(sqlite3 ~/.local/share/passman/passman.db "SELECT COUNT(*) FROM users;" 2>/dev/null); \
			ADMIN_COUNT=$$(sqlite3 ~/.local/share/passman/passman.db "SELECT COUNT(*) FROM users WHERE is_admin = 1;" 2>/dev/null); \
			NORMAL_USERS=$$(sqlite3 ~/.local/share/passman/passman.db "SELECT COUNT(*) FROM users WHERE is_admin = 0;" 2>/dev/null); \
			if [ -n "$$USERS_COUNT" ]; then \
				echo "    👥 Total de usuarios: $$USERS_COUNT"; \
				echo "    👑 Usuarios admin: $$ADMIN_COUNT"; \
				echo "    👤 Usuarios normales: $$NORMAL_USERS"; \
				if [ "$$USERS_COUNT" -eq 0 ]; then \
					echo -e "    $${YELLOW}⚠️  Sistema requiere inicialización (crear primer admin)$${NC}"; \
				elif [ "$$ADMIN_COUNT" -eq 0 ]; then \
					echo -e "    $${RED}⚠️  ERROR: No hay usuario administrador$${NC}"; \
				fi; \
			else \
				echo -e "    $${RED}✗ No se pudo consultar la base de datos$${NC}"; \
			fi; \
		else \
			echo -e "    $${YELLOW}⚠️  sqlite3 no está instalado (no se puede consultar la BD)$${NC}"; \
		fi; \
	else \
		echo -e "  ❌ Base de datos: $${RED}NO PRESENTE$${NC}"; \
		echo "  📁 Ubicación esperada: ~/.local/share/passman/passman.db"; \
		echo -e "  $${YELLOW}ℹ️  Se creará automáticamente en el primer inicio de la aplicación$${NC}"; \
	fi'
	@echo ""
	@echo "$(GREEN)► ARCHIVOS DEL PROYECTO:$(NC)"
	@if [ -f ./CMakeLists.txt ]; then echo "  ✅ CMakeLists.txt"; else echo "  ❌ CMakeLists.txt"; fi
	@if [ -f ./Makefile ]; then echo "  ✅ Makefile"; else echo "  ❌ Makefile"; fi
	@if [ -d ./src ]; then echo "  ✅ Directorio src/"; else echo "  ❌ Directorio src/"; fi
	@if [ -d ./include ]; then echo "  ✅ Directorio include/"; else echo "  ❌ Directorio include/"; fi
	@if [ -d ./doc ]; then echo "  ✅ Directorio doc/"; else echo "  ❌ Directorio doc/"; fi
	@if [ -d ./build ]; then echo "  ✅ Directorio build/ (compilado)"; else echo "  ⚠️  Directorio build/ (no compilado aún)"; fi
	@echo ""

logs:
	@docker-compose logs -f app

# ============================================================================
# DEFAULT TARGET
# ============================================================================

.DEFAULT_GOAL := help
