#Carpetas make -f
DOC = ./doc/
LIB = ./lib/
MAN = ./man/
OBJ = ./obj/
SRCLIB = ./srclib/
SRC = ./src/
DIRS = obj lib
GROUP = G-2311-03-P1
#Compilador
CC = gcc -pthread
#Librerias
LDFLAGS = -lm
CCLAGS = -ggdb -Wall
#Ficheros
EXEC = servidor
SOURCE_FILES =

all: dirs $(EXEC)

dirs:
	mkdir -p $(DIRS)

libreria: $(LIB)lib.a

$(LIB)lib.a: $(OBJ)lib.o
	ar -rv $@ $^
	@echo "Libreria lib.a generada"

$(EXEC): $(OBJ)server.o $(OBJ)commands.o $(OBJ)canal.o $(OBJ)user.o $(OBJ)utilidadesTAD.o $(LIB)lib.a
	$(CC) $(CCFLAGS) $^ -o  $@ -lircredes -lirctad
	@echo "Ejecutable servidor creado"

$(OBJ)server.o: $(SRC)$(GROUP)server.c
	$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)commands.o: $(SRC)$(GROUP)commands.c
	$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)canal.o: $(SRC)$(GROUP)canal.c
	$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)user.o: $(SRC)$(GROUP)user.c
	$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)utilidadesTAD.o: $(SRC)$(GROUP)utilidadesTAD.c
	$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)lib.o: $(SRCLIB)lib.c
	$(CC) $(CCFLAGS) -o $@ -c $^

clean:
		rm -f lib/lib.a obj/*.o includes/*.gch $(GROUP).tar.gz $(EXEC)
dox:
		doxygen Doxyfile


tar:
		@ echo "Comprimiendo el archivo"
		tar --exclude .git -czvf $(GROUP).tar.gz ../$(GROUP) --exclude $(GROUP).tar.gz
		@ echo "Archivo comprimido ..."
