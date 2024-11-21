# Nom du fichier executable final
EXEC = program

# Compilateur et ses arguments
CC = gcc
CFLAGS = -Wall

# Répertoires type
SRC_DIR = sources
OBJ_DIR = objects
BIN_DIR = binaries

# Fichiers
SRCS = $(wildcard $(SRC_DIR)/*.c) 
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/$(EXEC)


# Règle résolvant l'executables cible (règle par défaut !)
all: $(TARGET)
	@echo "Création de l'executable terminée."

# Créée les dossiers
setup:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)
	@echo "Création des dossiers terminée."

# Compile chaque fichier source .c en un fichier objet .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compilation terminée."

# Lie les fichiers objets .o ensemble en un executable avec le compilateur (résolue par "all" !)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lm
	@echo "Liaisons terminées."


# Supprime les fichiers compilés
clean:
	rm -r -f $(OBJ_DIR)/* $(BIN_DIR)/*
	@echo "Nettoyage terminé."

# Recompile
rebuild: clean all

.PHONY: all clean rebuild setup