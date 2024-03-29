CC	= g++
CFLAGS	= -g -Wall -Wextra -I.
LINKS	= -lmenu -lncursesw
DEPS	= screen.hpp widget.hpp widgetClock.hpp widgetANSI.hpp widgetDeco.hpp widgetManager.hpp widgetTextFile.hpp widgetDialog.hpp moduleDialog.hpp cursesAnsi.hpp
OBJ	= main.o screen.o widget.o widgetClock.o widgetANSI.o widgetDeco.o widgetManager.o widgetTextFile.o widgetDialog.o moduleDialog.o cursesAnsi.o

APP	= adash

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LINKS)

$(APP): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LINKS)

all: $(APP)

.PHONY: clean

clean:
	rm -f *.o

re: clean
	$(MAKE) all
