# Huffman
A utility for compressing and decompressing files using Huffman coding

# Build Instructions
1. Install [CMake](https://cmake.org/) 3.1.0 or later
2. Install [Boost](http://www.boost.org/)
3. Clone a copy of the repository:

    ```
    git clone https://github.com/SimonBoorer/Huffman.git
    ```
4. Build with CMake:

    ```
    cd Huffman
    mkdir build
    cd build
    cmake ..
    ```

# Usage
```
Usage: Huffman [options] input-file output-file
Allowed options:
  -h [ --help ]         produce help message
  -c [ --compress ]     compress file
  -x [ --decompress ]   decompress file
```

