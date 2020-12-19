PWD=$(shell pwd)
REP=$(shell basename $(PWD))
SOURCE=$(REP).c
CIBLE=$(SOURCE:.c=.o)
CFLAGS=-Wall

# makefile générique pour produire un code source 
# dont le nom correspond au nom du répertoire qui le contient

all: $(CIBLE)
	@echo "Le programme $(CIBLE) a été produit dans le répertoire $(REP)"

$(CIBLE) : adjacence_list.c
	@echo "Production de $(CIBLE)"
	gcc $(CFLAGS) $< -o $@

clean: 
	@echo "Nettoyage de $(CIBLE)"
	@rm -rf $(CIBLE)
