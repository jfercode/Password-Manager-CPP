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
        cmake compile run setup dev clean distclean

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
	@echo "  $(YELLOW)make distclean$(NC)        - Limpiar todo (build + docker)"
	@echo ""
	@echo "$(GREEN)DEBUG & STATUS COMMANDS:$(NC)"
	@echo "  $(YELLOW)make status$(NC)           - Ver estado del contenedor"
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

distclean: clean docker-clean
	@echo "$(GREEN)✓ Distclean completado (proyecto limpio)$(NC)"

# ============================================================================
# DEBUG & STATUS
# ============================================================================

status:
	@echo "$(BLUE)Estado del proyecto:$(NC)"
	@docker-compose ps || echo "$(YELLOW)Contenedor no está corriendo$(NC)"

logs:
	@docker-compose logs -f app

# ============================================================================
# DEFAULT TARGET
# ============================================================================

.DEFAULT_GOAL := help
