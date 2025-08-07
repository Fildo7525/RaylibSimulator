BUILD_DIR := $(PWD)/build

# Make use of the bash shell for better compatibility
# and to ensure that commands are executed in the same shell context.
.ONESHELL:
SHELL := /bin/bash

.PHONY: setup compile run test clean

# Do not print the commands themselves, only their output.
.SILENT:

setup:
	NPROC=$$(( $(shell nproc) * 2 ))
	cmake -S . -B $(BUILD_DIR) -G Ninja

compile: setup
	cmake --build $(BUILD_DIR) --target simulator -j$$NPROC

run: setup compile
	cd $(BUILD_DIR)
	./simulator

test: setup compile
	cmake --build $(BUILD_DIR) --target test -j$$NPROC
	if [[ $? -ne 0 ]]; then
		echo "Test compilation failed."
		exit 1
	if

	clear
	cd $(BUILD_DIR)/test
	./test

clean:
	rm -rf $(BUILD_DIR)
