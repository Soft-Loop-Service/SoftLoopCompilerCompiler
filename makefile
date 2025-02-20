
FLD_SRC = ./src
BIN_SRC = ./bin
SRCS_SRC = $(wildcard $(FLD_SRC)/*/*.cpp) $(wildcard $(FLD_SRC)/*/*/*.cpp) $(wildcard $(FLD_SRC)/*/*/*/*.cpp) $(wildcard $(FLD_SRC)/*/*/*/*/*.cpp)
ANALYSIS_SRC = ./src/analysis.cpp $(SRCS_SRC)
TABLE_SRC = ./src/table.cpp $(SRCS_SRC)

FILES = ""

define MAKEOBJ
   @echo $(subst cpp,o,$(1)) 
   g++ -std=c++20 -c $(1) -o ./bin/$(notdir $(subst cpp,o,$(1)) ) 


   $(foreach filename,$(ANALYSIS_SRC),$(call MAKEOBJ,$(filename)))

endef

 
buildparse:
	@echo "$(ANALYSIS_SRC)"
	rm -f $(wildcard $(BIN_SRC)/*.o)
	g++ -std=c++2a -w -o analysis.out $(wildcard $(ANALYSIS_SRC)) 


buildtable:
	@echo "$(TABLE_SRC)"
	rm -f $(wildcard $(BIN_SRC)/*.o)
	g++ -std=c++2a  -w -o table.out $(wildcard $(TABLE_SRC))



	


	
