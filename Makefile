#Carpetas make -f
DOC = ./doc/
LIB = ./lib/
MAN = ./man/
OBJ = ./obj/
SRCLIB = ./srclib/
SRC = ./src/
GROUP = G-2311-03-P1
#Compilador
CC = gcc -pthread
#Librerias
LDFLAGS = -lm
CCLAGS = -ggdb -Wall
#Ficheros
EXEC = servidor
SOURCE_FILES =

all: $(EXEC)

libreria: $(LIB)lib.a

$(LIB)lib.a: $(OBJ)lib.o
	ar -rv $@ $^
	@echo "Libreria lib.a generada"

$(EXEC): $(OBJ)server.o $(OBJ)commands.o $(OBJ)channels.o $(OBJ)clients.o  $(LIB)lib.a
	$(CC) $(CCFLAGS) $^ -o  $@ -lircredes -lirctad
	@echo "Ejecutable servidor creado"

$(OBJ)server.o: $(SRC)server.c
	$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)commands.o: $(SRC)commands.c
	$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)lib.o: $(SRCLIB)lib.c
	$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)channels.o: $(SRC)channels.c
	$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)clients.o: $(SRC)clients.c
	$(CC) $(CCFLAGS) -o $@ -c $^

clean:
		rm -f lib/lib.a obj/*.o includes/*.gch $(EXEC)
dox:
		doxygen Doxyfile

prueba:$(OBJ)prueba.o
	$(CC) $(CCFLAGS) $^ -o  $@ -lircredes -lirctad
	@echo "Ejecutable servidor creado"

$(OBJ)prueba.o:$(SRC)prueba.c
	$(CC) $(CCFLAGS) -o $@ -c $^

tar:
		@ echo "Comprimiendo el archivo"
		tar --exclude .git -czvf $(GROUP).tar.gz ../$(GROUP) --exclude $(GROUP).tar.gz
		@ echo "Archivo comprimido ..."
