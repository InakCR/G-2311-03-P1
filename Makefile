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
CFLAGS = -ggdb -Wall
#Ficheros
EXEC = serv
SOURCE_FILES =

all: servidor
libreria: $(LIB)lib.a

	$(LIB)lib.a: $(OBJ)libs.o
		$(ADDLIB) -rv $@ $^
		@echo "Libreria lib.a generada ..."

	##############################
	## EJECUTABLES
	##############################
	servidor: $(OBJ)server.o $(OBJ)comandos.o $(OBJ)channels.o $(OBJ)client.o  $(LIB)lib.a
		$(CC) $(CCFLAGS) $^ $(FLAGS) -o $@ -lircredes
		@echo "Ejecutable servidor creado ..."

	##############################
	## OBJETOS
	##############################
	$(OBJ)server.o: $(SRC)server.c
		@ echo "Generando objeto ..."
		@ echo "servidor.o ..."
		$(CC) $(CCFLAGS) $(FLAGS) -o $@ -c $^

	$(OBJ)comandos.o: $(SRC)comandos.c
		@ echo "Generando objeto ..."
		@ echo "comandos.o ..."
		$(CC) $(CCFLAGS) $(FLAGS) -o $@ -c $^

	$(OBJ)libs.o: $(SRCLIB)libs.c
		@ echo "Generando objeto ..."
		@ echo "libs.o ..."
		$(CC) $(CCFLAGS) $(FLAGS) -o $@ -c $^

	$(OBJ)channels.o: $(SRC)channels.c
		@ echo "Generando objeto ..."
		@ echo "channels.o ..."
		$(CC) $(CCFLAGS) $(FLAGS) -o $@ -c $^

	$(OBJ)client.o: $(SRC)client.c
		@ echo "Generando objeto ..."
		@ echo "client.o ..."
		$(CC) $(CCFLAGS) $(FLAGS) -o $@ -c $^

$(EXEC): $(OBJ)*.o
clean:
	rm -f *.o $(EXEC)

tar:
		@ echo "Comprimiendo el archivo"
		tar --exclude .git -czvf $(GROUP).tar.gz ../$(GROUP) --exclude $(GROUP).tar.gz
		@ echo "Archivo comprimido ..."
