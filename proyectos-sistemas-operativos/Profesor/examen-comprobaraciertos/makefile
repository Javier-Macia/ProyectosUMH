BIN_DIR = .# Localización del programa
SRC_DIR = ./src# Localización de los ficheros .c
INC_DIR = ./inc# Localización de los ficheros .h
OBJ_DIR = ./obj# Localización de los ficheros .o


PROGRAM = $(BIN_DIR)/comprobaraciertos# Nombre del programa

CC = gcc
CFLAGS = -I$(INC_DIR)
OBJS = $(OBJ_DIR)/shared.o \
	   $(OBJ_DIR)/func.o \
	   $(OBJ_DIR)/sem.o \

INCS = $(INC_DIR)/func.h \
	   $(INC_DIR)/sem.h \

$(PROGRAM) : $(OBJS)
	@mkdir -p $(BIN_DIR)
	@$(CC) $(CFLAGS) $(OBJS) -o $(PROGRAM) -lm

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) -c -MD $(CFLAGS) $< -o $@
	@echo \'$<\' se ha compilado.

-include $(OBJ_DIR)/*.d

.PHONY: clean

clean: 
	rm -r $(OBJ_DIR) $(BIN_DIR)
