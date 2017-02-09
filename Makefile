EXEC =
SOURCE_FILES =
LDFLAGS = -lm
CFLAGS = -ggdb -Wall

all:
	@echo Compilando y enlazando $(EXEC)
	gcc  $(CFLAGS) -o $(EXEC) $(SOURCE_FILES) $(LDFLAGS)

clean:
	rm -f *.o $(EXEC)
