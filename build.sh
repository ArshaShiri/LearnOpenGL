#!/bin/sh

# sudo apt install libxcb-keysyms1-dev
# sudo apt install libxcb-randr0-dev
# sudo apt install libxcb-shape0-dev
# sudo apt install libxcb-sync-dev
# sudo apt install libxcb-xfixes0-dev
# sudo apt install libxcb-xinerama0-dev
# sudo apt install libxcb-dri3-dev
# sudo apt install libxcb-cursor-dev
# sudo apt install libxcb-util-dev

# TODO Add more options to the build: Debug etc.
printManual() {
    echo -c "\t clean build"
    exit 1
}

while getopts ":c" opt; do
    case $opt in
    c) clean_build=true ;;
    i) install_conan=true ;;
    \?)
        printManual
        exit
        ;;
    esac
done

build_folder_name="build"

if [ $install_conan ]; then
    conan create lib/glad/.
    conan install . --build=missing
    # conan install . --output-folder=$build_folder_name --build=missing
fi

cd build/Release/generators
if [ $clean_build ]; then
    rm -rf $build_folder_name
    cmake ../../.. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
fi

cmake --build .
