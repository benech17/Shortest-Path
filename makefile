TARGETS=$(shell ls -d programme* tests)
CLEANTARGETS=$(foreach dir,$(TARGETS),clean_$(dir))
MRPROPERTARGETS=$(foreach dir,$(TARGETS),mrproper_$(dir))
MAKEFLAGS += --no-print-directory

all: $(TARGETS)	

.PHONY: $(TARGETS) $(CLEANTARGETS)
$(TARGETS) :
	@echo "Execution de make $@ :"
	@cd $@; make

$(CLEANTARGETS) :
	@echo -n "Execution de clean $(subst clean_,,$@) :   " 
	@cd $(subst clean_,,$@); make clean

$(MRPROPERTARGETS) :
	@echo -n "Execution de mrproper $(subst mrproper_,,$@) :   " 
	@cd $(subst mrproper_,,$@); make mrproper

clean: $(CLEANTARGETS)

mrproper: $(MRPROPERTARGETS)