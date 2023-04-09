BIN_DIR = .# Localización del programa
SRC_DIR = ./src# Localización de los ficheros .c
OBJ_DIR = ./obj# Localización de los ficheros .o

PROGRAM = $(BIN_DIR)/prog
FILE = ./ejemplo.bin
RAND_FILES = ./matriz*.bin
OUT = ./out.txt	
NUM_ITER = 4
CC = mpicc
EX = mpirun
NRECURSOS = 1
CFLAGS = -lm 
OBJS = $(OBJ_DIR)/main.o \
	$(OBJ_DIR)/lib.o \

$(PROGRAM) : $(OBJS)
	@mkdir -p $(BIN_DIR)
	@$(CC) $(OBJS) -o $(PROGRAM) $(CFLAGS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) -c $< -MD $(CFLAGS) -o $@ 
	@echo \'$<\' se ha compilado.

-include $(OBJ_DIR)/*.d

run:
	@clear
	@make
	@echo
	@echo Ejecutando el programa.
	@$(EX) -np $(NRECURSOS) $(PROGRAM) $(NUM_ITER)

.PHONY: clean

clean: 
	rm -r $(OBJ_DIR)
	rm $(PROGRAM)
	rm $(RAND_FILES)
	rm $(OUT)
	