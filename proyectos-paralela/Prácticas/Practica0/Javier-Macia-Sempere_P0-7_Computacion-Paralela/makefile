BIN_DIR = .# Localización del programa
SRC_DIR = ./src# Localización de los ficheros .c
OBJ_DIR = ./obj# Localización de los ficheros .o

FILE1 = fichero1.bin
FILE2 = fichero2.bin
PROGRAM = $(BIN_DIR)/prog
CC = gcc
CFLAGS = -lm 
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
	@echo Ejecutando el programa con $(FILE1)...
	@$(PROGRAM) $(FILE1)
	@echo
	@echo Ejecutando el programa con $(FILE2)...
	@$(PROGRAM) $(FILE2)

.PHONY: clean

clean: 
	rm -r $(OBJ_DIR)
	rm $(PROGRAM)