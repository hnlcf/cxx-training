PROJECT 	:= cxx-training
PROJ_DIR 	:= $(shell pwd)

BUILDER 	:= xmake
BUILD_TYPE 	:= debug

JOBS		:= $(shell nproc)

default : build

reload : clean format
	$(BUILDER) config --mode=$(BUILD_TYPE)
	$(BUILDER) project --kind=compile_commands --lsp=clangd --outputdir=$(PROJ_DIR)

build : FORCE
	$(BUILDER) build --all --warning --jobs=$(JOBS)

clean :
	rm -rf compile_commands.json
	$(BUILDER) clean

format : .clang-format
	$(BUILDER) format


FORCE : ;

.PHONY : FORCE reload build clean format
