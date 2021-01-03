PWD=$(shell pwd)
REP=$(shell basename $(PWD))
CIBLE=$(REP).exe
SOURCE=$(REP).c
CFLAGS=-Wall

all: $(CIBLE)
	@echo "Le programme $(CIBLE) a bien été produit dans le répertoire $(REP) \n"

$(CIBLE): $(SOURCE)
	@echo "Production de $(CIBLE) à partir du fichier $(SOURCE)"
	gcc $(CFLAGS) $(SOURCE) -o $(CIBLE)

clean: 
	@echo "Nettoyage de $(CIBLE)"
	@rm -rf $(CIBLE)
