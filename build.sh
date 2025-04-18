#!/usr/bin/env bash

set -e

# Configuration
BINARY_NAME="bullet_balance"

# Function to copy compile_commands.json to project root
copy_compile_commands() {
    if [ -f "build/compile_commands.json" ]; then
        cp build/compile_commands.json .
    fi
}

# Function to clean build artifacts
clean() {
    if [ -d "build" ]; then
        # Remove all build artifacts
        rm -rf build/*
        # Recreate .gitkeep
        touch build/.gitkeep
        echo "Build artifacts cleaned"
    fi
    if [ -f "compile_commands.json" ]; then
        rm compile_commands.json
        echo "compile_commands.json removed"
    fi
}

# Function to build native version
build_native() {
    mkdir -p build
    cd build
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug ..
    make
    cd ..
    copy_compile_commands
}

# Function to build web version
build_web() {
    mkdir -p build
    cd build
    emcmake cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Debug ..
    emmake make
    cd ..
    copy_compile_commands
}

# Function to build hotreload library
build_hot() {
    mkdir -p build
    cd build
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug ..
    make ${BINARY_NAME}_lib
    cd ..
    copy_compile_commands
}

# Function to run hotreload version
run_hot() {
    mkdir -p build
    cd build
    cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug ..
    make ${BINARY_NAME}_lib ${BINARY_NAME}_hotload
    cd ..
    copy_compile_commands
    ./build/${BINARY_NAME}_hotload
}

# Function to run native version
run_native() {
    build_native
    ./build/$BINARY_NAME
}

# Function to run web version
run_web() {
    build_web
    cd build
    emrun --port 8080 $BINARY_NAME.html
}

# Main script logic
case "$1" in
    "build")
        case "$2" in
            "web")
                build_web
                ;;
            "native"|"")
                build_native
                ;;
            "hot")
                build_hot
                ;;
            *)
                echo "Unknown build target: $2"
                exit 1
                ;;
        esac
        ;;
    "run")
        case "$2" in
            "web")
                run_web
                ;;
            "native"|"")
                run_native
                ;;
            "hot")
                run_hot
                ;;
            *)
                echo "Unknown run target: $2"
                exit 1
                ;;
        esac
        ;;
    "clean")
        clean
        ;;
    "")
        build_native
        ;;
    *)
        echo "Usage: $0 [build|run|clean] [native|web|hot]"
        exit 1
        ;;
esac


