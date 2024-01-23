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

progname="$(basename "$0")"

# TODO Add more options to the build: Debug etc.
printManual() {
    cat <<EOF
usage: $progname

    echo -c "\t clean build"
    echo -d "\t clean build"
EOF
    exit "${1-0}"
}

while getopts ci opt; do
    case $opt in
    c)
        clean_build=true
        ;;
    i)
        install_conan=true
        ;;
    *)
        printManual
        exit 64 # EX_USAGE
        ;;
    esac
done

build_folder_name="build"

if [ $clean_build ]; then
    rm -rf $build_folder_name
fi

if [ "$install_conan" ] || [ "$clean_build" ]; then
    conan create lib/glad/.
    conan install . --build=missing
fi

cd build/Release/generators
if [ $clean_build ]; then
    cmake ../../.. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
fi

cmake --build .
