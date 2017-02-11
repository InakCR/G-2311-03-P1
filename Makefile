#Carpetas
DOC = ./doc/
LIB = ./lib/
MAN = ./man/
OBJ = ./obj/
SRCLIB = ./srclib/
SRC = ./src/
#Compilador
CC = gcc
#Librerias
LDFLAGS = -lm
CFLAGS = -ggdb -Wall
#Ficheros
EXEC = serv
SOURCE_FILES =

all:
	@echo Compilando y enlazando $(EXEC)
	$(EXEC)

$(EXEC): $(OBJ)*.o
clean:
	rm -f *.o $(EXEC)

tar:
		@ echo "Comprimiendo el archivo"
		tar czvf G-2311-03-P1.tar.gz ../G-2311-03-P1 --exclude G-2311-03-P1.tar.gz
		@ echo "Archivo comprimido ..."
