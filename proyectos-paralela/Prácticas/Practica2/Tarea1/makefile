BIN_DIR = .# Localización del programa
SRC_DIR = ./src# Localización de los ficheros .c
OBJ_DIR = ./obj# Localización de los ficheros .o

PROGRAM = $(BIN_DIR)/prog
FILE = ./img/lena4096x4096.raw
MODE = so
TAM_IMG = 4096
CC = gcc
EX = 
CFLAGS = -lm -fopenmp -Wall
OBJS = $(OBJ_DIR)/main.o \

$(PROGRAM) : $(OBJS)
	@mkdir -p $(BIN_DIR)
	@$(CC) $(OBJS) -o $(PROGRAM) $(CFLAGS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) -c $< -MD $(CFLAGS) -o $@ 
	@echo \'$<\' se ha compilado.

-include $(OBJ_DIR)/*.d

run:
	@make
	@echo
	@echo Ejecutando el programa.
	@$(EX) $(PROGRAM) $(FILE) $(MODE) $(TAM_IMG)

.PHONY: clean

clean: 
	rm -r $(OBJ_DIR)
	rm $(PROGRAM)
	rm imagen_filtrada.raw
	rm out.txt
	