#Carpetas make -f
DOC = ./doc/
LIB = ./lib/
MAN = ./man/
OBJ = ./obj/
SRCLIB = ./srclib/
SRC = ./src/
GROUP = G-2311-03-P1
#Compilador
CC = gcc
#Librerias
LDFLAGS = -lm
CCLAGS = -ggdb -Wall
#Ficheros
EXEC = serv
SOURCE_FILES =

all: servidor

libreria: $(LIB)lib.a

$(LIB)lib.a: $(OBJ)lib.o
	ar -rv $@ $^
	@echo "Libreria lib.a generada ..."

##############################
## EJECUTABLES
##############################
servidor: $(OBJ)server.o $(OBJ)commands.o $(OBJ)channels.o $(OBJ)clients.o  $(LIB)lib.a
	$(CC) $(CCFLAGS) $^ -o  $@ -lircredes
	@echo "Ejecutable servidor creado ..."

$(OBJ)server.o: $(SRC)server.c
	@echo "Generando objeto ..."
	@echo "servidor.o ..."
	$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)commands.o: $(SRC)commands.c
	@ echo "Generando objeto ..."
	@ echo "comandos.o ..."
	$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)lib.o: $(SRCLIB)lib.c
	@ echo "Generando objeto ..."
	@ echo "libs.o ..."
	$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)channels.o: $(SRC)channels.c
	@ echo "Generando objeto ..."
	@ echo "channels.o ..."
	$(CC) $(CCFLAGS) -o $@ -c $^

$(OBJ)clients.o: $(SRC)clients.c
	@ echo "Generando objeto ..."
	@ echo "client.o ..."
	$(CC) $(CCFLAGS) -o $@ -c $^

clean:
	rm -f *.o includes/*.gch $(EXEC)

tar:
		@ echo "Comprimiendo el archivo"
		tar --exclude .git -czvf $(GROUP).tar.gz ../$(GROUP) --exclude $(GROUP).tar.gz
		@ echo "Archivo comprimido ..."
