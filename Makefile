ifeq ($(OS),Windows_NT)
    RM = del /Q /F
    NULL = NUL
    EXE = .exe
    RUN_CMD = .\\$(TARGET)
else
    RM = rm -f
    NULL = /dev/null
    EXE =
    RUN_CMD = ./$(TARGET)
endif

# Impostazioni del compilatore
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
INCLUDES = -I./headers
LDFLAGS =

SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:.c=.o)
TARGET = Christian_Quarto_CardGame$(EXE)

# Build di default
all: debug

# Build di debug
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)


# Assegna il target
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

# Compila i file
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Avvia il programma
run: debug
	$(RUN_CMD)

# Genera la documentazione
docs:
	doxygen Doxyfile

# Rimuove i file della build
clean:
	$(RM) $(OBJ) $(TARGET) > $(NULL) 2>&1 || exit 0

# Rimuove tutti i file compresa la documentazione
distclean: clean
	$(RM) -r docs > $(NULL) 2>&1 || exit 0

# Help
help:
	@echo "Opzioni possibili"
	@echo "  all/debug - Versione debug (default)"
	@echo "  run       - Compila e avvia la versione di debug"
	@echo "  docs      - Genera la documentazione"
	@echo "  clean     - Rimuove i file object"
	@echo "  distclean - Rimuove tutti i file"
	@echo "  help      - Mostra le opzioni possibili"

.PHONY: all debug release run docs clean distclean help
