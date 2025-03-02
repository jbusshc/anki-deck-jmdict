# Variables
CXX = g++
EXEC = bin/anki-gen

CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -O3 -Iinclude `xml2-config --cflags`
LDFLAGS = `xml2-config --libs`

SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

# Encontrar todos los archivos .cpp en el directorio src
SRC = $(wildcard $(SRC_DIR)/*.cpp)

# Convertir las rutas de archivos .cpp a rutas de archivos .o
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Regla por defecto
all: $(BIN_DIR) $(EXEC)

# Regla para crear el ejecutable
$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Regla para compilar archivos .cpp a .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para crear el directorio bin si no existe
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Regla para limpiar archivos generados
clean:
	rm -f $(OBJ_DIR)/*.o $(EXEC)

# Regla para eliminar los archivos de objeto
clean-obj:
	rm -rf $(OBJ_DIR)

# Regla para ejecutar el programa
run: $(EXEC)
	cd $(BIN_DIR) && ./$(notdir $(EXEC))

