PWD=$(shell pwd)
REP=$(shell basename $(PWD))
SOURCES=$(shell cat makefile_sources)
CIBLE=$(REP).exe
CFLAGS=-Wall

all: $(CIBLE)
	@echo "Le programme $(CIBLE) a été produit dans le répertoire $(REP)"

$(CIBLE) : $(SOURCES)
	@echo Production de $(CIBLE)
	@echo à partir des fichiers : $(SOURCES)
	gcc $(CFLAGS) $(SOURCES) -o $@

clean: 
	@echo "Nettoyage de $(CIBLE)"
	@rm -rf $(CIBLE)