.PHONY: conan cmake build clean

conan:
	conan install . --output-folder=build --build=missing --profile=conanprofile

cmake: conan
	cmake -B build -S . -G "Ninja" -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug

build:
	cmake --build build

clean:
	@git clean -f -d -X