BIN_DIR = ./bin# Localización del programa
SRC_DIR = ./src# Localización de los ficheros .c
INC_DIR = ./inc# Localización de los ficheros .h
OBJ_DIR = ./obj# Localización de los ficheros .o


PROGRAM = $(BIN_DIR)/sorteo

CC = gcc
CFLAGS = -Wall -I$(INC_DIR)
OBJS = $(OBJ_DIR)/main.o \
	   $(OBJ_DIR)/funciones.o

INCS = $(INC_DIR)/funciones.h

$(PROGRAM) : $(OBJS)
	@mkdir -p $(BIN_DIR)
	@$(CC) $(CFLAGS) $(OBJS) -o $(PROGRAM)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) -c -MD $(CFLAGS) $< -o $@
	@echo \'$<\' se ha compilado.

-include $(OBJ_DIR)/*.d

.PHONY: clean

clean: 
	rm -r $(OBJ_DIR) $(BIN_DIR)