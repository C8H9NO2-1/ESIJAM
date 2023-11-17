CC = gcc
SRC= $(notdir $(wildcard src/*.c))

ifeq ($(OS), Windows_NT)
	SE = win
	FLAGS_C = -I include -L lib -lmingw32 -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2 -pthread
	FLAGS_E = -I include -L lib -lmingw32 -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2 -pthread
	clr = echo Non implementer sur windows
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		FLAGS_C = -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2
		FLAGS_L = -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2
		
		SE = linux
	endif
	ifeq ($(UNAME_S), Darwin)
		FLAGS_C = -I/opt/homebrew/include
		FLAGS_E = -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_mixer

		SE=mac
	endif
	clr = rm bin/$(SE)/*.o && echo Tout les fichiers binaires ont ete supprimes
endif

OBJ= $(SRC:.c=.o)

%.o : src/%.c
	@echo Compilation de $<
	@$(CC) -c $< -Wall $(FLAGS_C) -o bin/$(SE)/$*.o

all : $(OBJ)
	@echo Edition des liens...
	@$(CC) $(addprefix bin/$(SE)/, $^) $(FLAGS_E) -o exe/$(SE)/prog
	@echo Terminer !

clean : 
	@$(clr)
