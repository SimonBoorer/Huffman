#include "Huffman.h"

int main()
{
	Huffman huffman;
	huffman.Compress("compressme.txt", "compressed.txt");
	huffman.Decompress("compressed.txt", "decompressed.txt");

	return 0;
}