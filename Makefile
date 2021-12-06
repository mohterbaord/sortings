all: clear build execute

clear:
	@rm -rf ./build/

build:
	@cmake -S . -B ./build/
	@cmake --build ./build/ --target main

execute:
	@cd build && ./main
