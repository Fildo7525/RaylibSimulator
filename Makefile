BUILD_DIR=$(PWD)/build

setup:
	cmake -S . -B ${BUILD_DIR}

compile: setup
	cmake --build ${BUILD_DIR} --target simulator -j32

run: setup compile
	cd ${BUILD_DIR} && ./simulator

test: setup compile
	cd ${BUILD_DIR}/test && make -j16 && clear && ./test

clean:
	rm -rf ${BUILD_DIR}
