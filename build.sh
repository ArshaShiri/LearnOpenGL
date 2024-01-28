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
    echo -d "\t debug build"
    echo -i "\t reconfigure project"
EOF
    exit "${1-0}"
}

clean_build=false
reconfigure=false
debug=false

while getopts cid opt; do
    case $opt in
    c)
        clean_build=true
        ;;
    i)
        reconfigure=true
        ;;
    d)
        debug=true
        ;;
    *)
        printManual
        exit 64 # EX_USAGE
        ;;
    esac
done

build_folder_name="build"

if [ $clean_build = true ]; then
    rm -rf $build_folder_name
fi

build_path=build/Release/generators
build_type=Release

if [ $debug = true]; then
    echo "Debug build"
    build_path=build/Debug/generators
    build_type=Debug
fi

if [ $reconfigure = true ] || [ "$clean_build" = true ]; then
    echo "Reconfiguring the project"
    conan create lib/glad/.
    conan install . --build=missing --settings:host=build_type=$build_type
fi

if [ $clean_build = true ]; then
    echo "Clean build"
    cmake -S . -DCMAKE_TOOLCHAIN_FILE=$build_path/conan_toolchain.cmake -B $build_path -DCMAKE_BUILD_TYPE=$build_type
fi

cmake --build $build_path
