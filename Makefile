PROJECT 	:= test-cpp
PROJ_DIR 	:= $(shell pwd)

SOURCE_DIR	:=  crimson \
				lcf_json \
				mini_std \
				simplified-csapp \
				test-c
BUILD_DIR 	:= $(PROJ_DIR)/build

BUILDER 	:= cmake
BUILD_TYPE 	:= Debug
GENERATOR 	:= Ninja
BUILD_FLAG 	:= -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
							-DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE

default : run

reload : 
	$(BUILDER) $(BUILD_FLAG) -G Ninja -S $(PROJ_DIR) -B $(BUILD_DIR)

build: FORCE
	$(BUILDER) --build $(BUILD_DIR) --target all

clean:
	rm -rf $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)

format: .clang-format
	find $(SOURCE_DIR) -name "*.c" -name "*.cpp" -o -name "*.h" | xargs clang-format -i


FORCE : ;

.PHONY : FORCE reload build run clean format
