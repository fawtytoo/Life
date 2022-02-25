#   life

TARGET = life

CC = gcc

CFLAGS = -Wall
LDFLAGS = -lSDL2

SOURCE = main.o system.o world.o

all:	$(SOURCE)
	$(CC) $(SOURCE) -o $(TARGET) $(LDFLAGS)

%.o:	%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)

install: all
	cp $(TARGET) ~/.local/bin/

uninstall:
	rm ~/.local/bin/$(TARGET)

#   life
