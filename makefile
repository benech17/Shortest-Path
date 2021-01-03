TARGETS=$(shell ls -d programme*)
CLEANTARGETS=$(foreach dir,$(TARGETS),clean_$(dir))


all: $(TARGETS)	

.PHONY: $(TARGETS) $(CLEANTARGETS)
$(TARGETS) :
	@echo "Execution de make $@ :\n"
	@cd $@; make

$(CLEANTARGETS) :
	@echo -n "Execution de clean $(subst clean_,,$@)\n" :
	@cd $(subst clean_,,$@); make clean

clean: $(CLEANTARGETS)


