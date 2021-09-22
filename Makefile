# Pour afficher beaucoup de warnings (et les traiter commes des erreurs)
CFLAGS=-Wall -Werror -std=c99 -pedantic
# Pour compiler en mode "debug"
CFLAGS+=-g
# Pour optimiser "à fond"
CFLAGS+=-O3
# Pour utiliser la librairie "math" (cos, sin, acos, asin...)
CFLAGS+=-lm

all: be1

be1: be1.c
	gcc $< ${CFLAGS} -o $@

clean:
	rm be1
