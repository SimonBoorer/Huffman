# The recommended way to collect sources in variable 
# HUFFMAN_SOURCES by explicitly specifying the source files
set  (HUFFMAN_SOURCES
      "Bitstream.h"
      "Bitstream.cpp"
      "Huffman.h"
      "Huffman.cpp"
      "main.cpp")

# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("" FILES ${HUFFMAN_SOURCES})

# Set Properties->General->Configuration Type to Application(.exe)
# Creates Huffman.exe with the listed sources collected in the variables
# Adds sources to the Solution Explorer
add_executable (Huffman ${HUFFMAN_SOURCES})
 
# Creates a folder "executables" and adds target 
# project (Huffman.vcproj) under it
set_property(TARGET Huffman PROPERTY FOLDER "executables")
 
# Adds logic to INSTALL.vcproj to copy Huffman.exe to destination directory
install (TARGETS Huffman
         RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)