#!/bin/bash

# Set the source file and output binary name
SRC_FILE="icmp_packet.c"
OUTPUT_BINARY="icmp_packet"

# Function to compile on Linux
compile_linux() {
    echo "Compiling for Linux..."
    gcc -o $OUTPUT_BINARY $SRC_FILE
}

# Function to compile on Windows using MinGW
compile_windows() {
    echo "Compiling for Windows (MinGW)..."
    x86_64-w64-mingw32-gcc -o $OUTPUT_BINARY.exe $SRC_FILE
}

# Function to compile on macOS
compile_mac() {
    echo "Compiling for macOS..."
    clang -o $OUTPUT_BINARY $SRC_FILE
}

# Check the platform and call the appropriate compiler
case "$(uname)" in
    "Linux")
        compile_linux
        ;;
    "Darwin")
        compile_mac
        ;;
    "MINGW64_NT"*)
        compile_windows
        ;;
    *)
        echo "Unsupported platform!"
        exit 1
        ;;
esac

echo "Compilation completed for $(uname)."
