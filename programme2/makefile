PWD=$(shell pwd)
REP=$(shell basename $(PWD))
CIBLE=$(REP).exe
SOURCE=$(REP).c
CFLAGS=-Wall

all: 
	@echo "Le programme $(CIBLE) a été produit dans le répertoire $(REP)"
	@echo -n "Production de $(CIBLE) \t"
	gcc $(CFLAGS) $(SOURCE) -o $(CIBLE)

clean: 
	@echo "Nettoyage de $(CIBLE)"
	@rm -rf $(CIBLE)
