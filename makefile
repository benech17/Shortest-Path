TARGETS=$(shell ls -d programme*)
CLEANTARGETS=$(foreach dir,$(TARGETS),clean_$(dir))
MAKEFLAGS += --no-print-directory

all: $(TARGETS)	

.PHONY: $(TARGETS) $(CLEANTARGETS)
$(TARGETS) :
	@echo "Execution de make $@ :"
	@cd $@; make

$(CLEANTARGETS) :
	@echo -n "Execution de clean $(subst clean_,,$@) :   " 
	@cd $(subst clean_,,$@); make clean

clean: $(CLEANTARGETS)


