PWD=$(shell pwd)
REP=$(shell basename $(PWD))
SOURCES=$(shell cat makefile_sources)
CIBLE=$(REP).exe
CFLAGS=-Wall

all: $(CIBLE)
	@echo "Le programme $(CIBLE) a bien été produit dans le répertoire $(REP) \n"

$(CIBLE) : $(SOURCES)
	@echo Production de $(CIBLE)  à partir des fichiers : $(SOURCES)
	gcc $(CFLAGS) $(SOURCES) -o $@

clean: 
	@echo "Nettoyage de $(CIBLE)"
	@rm -rf $(CIBLE)