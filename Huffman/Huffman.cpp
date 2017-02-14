#include "Huffman.h"

#include <fstream>
#include <iostream>
#include <queue>

void Huffman::CreateFrequencyTable(ifbitstream& infile, unsigned int frequencies[256])
{
	char c;
	while (infile.get(c))
	{
		frequencies[c]++;
	}
}

void Huffman::PrintFrequencyTable(unsigned int frequencies[256])
{
	std::cout << "Symbol\tFrequency\n";
	for (int i = 0; i < 256; ++i)
	{
		if (frequencies[i] == 0)
			continue;

		std::cout << (char)i << "\t" << frequencies[i] << "\n";
	}
	std::cout << "\n";
}

Huffman::Node *Huffman::CreateBinaryTree(unsigned int frequencies[256])
{
	std::priority_queue<Node*, std::vector<Node*>, CompareNode> trees;

	for (int i = 0; i < 256; ++i)
	{
		if (frequencies[i] != 0)
			trees.push(new LeafNode((char)i, frequencies[i]));
	}

	while (trees.size() > 1)
	{
		Node* left = trees.top();
		trees.pop();

		Node* right = trees.top();
		trees.pop();

		Node* parent = new InternalNode(left, right);
		trees.push(parent);
	}

	return trees.top();
}

void Huffman::CreateCodeMap(Node* node, HuffmanCode& prefix, HuffmanCodeMap& code_map)
{
	if (const LeafNode* ln = dynamic_cast<const LeafNode*>(node))
	{
		code_map[ln->symbol_] = prefix;
	}
	else if (const InternalNode *in = dynamic_cast<const InternalNode*>(node))
	{
		HuffmanCode left_prefix = prefix;
		left_prefix.push_back(false);
		CreateCodeMap(in->left_, left_prefix, code_map);

		HuffmanCode right_prefix = prefix;
		right_prefix.push_back(true);
		CreateCodeMap(in->right_, right_prefix, code_map);
	}
}

void Huffman::PrintCodeMap(HuffmanCodeMap& code_map)
{
	std::cout << "Symbol\tCode\n";
	for (const auto& kv : code_map)
	{
		std::cout << kv.first << "\t";
		for (const auto& it : kv.second)
		{
			std::cout << it;
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void Huffman::WriteBinaryTree(ofbitstream& outfile, const Node *node)
{
	if (const LeafNode* ln = dynamic_cast<const LeafNode*>(node))
	{
		outfile.write_bit(true);
		outfile.write_byte(ln->symbol_);
	}
	else if (const InternalNode *in = dynamic_cast<const InternalNode*>(node))
	{
		outfile.write_bit(false);
		WriteBinaryTree(outfile, in->left_);
		WriteBinaryTree(outfile, in->right_);
	}
}

void Huffman::WritePadding(ofbitstream& outfile, const int size, const unsigned int frequencies[256], HuffmanCodeMap& code_map)
{
	int tree_size = size * 10 - 1;
	int encoded_size = 0;
	for (int i = 0; i < 256; ++i)
	{
		encoded_size += frequencies[i] * code_map[i].size();
	}
	int total_size = tree_size + encoded_size + 3;
	
	char padding = 8 - (total_size % 8);
	outfile.write_bits(padding, 3);

	for (int i = 0; i < padding; i++)
	{
		outfile.write_bit(false);
	}
}

void Huffman::WriteBits(ifbitstream& infile, HuffmanCodeMap& code_map, ofbitstream& outfile)
{
	char c;
	while (infile.get(c))
	{
		HuffmanCode code = code_map[c];
		outfile.write_bits(code);
	}
}

void Huffman::Compress(const std::string& infilename, const std::string& outfilename)
{
	ifbitstream infile(infilename);
	if (!infile.is_open())
	{
		std::cout << "Could not load " << infilename << ".\n";
		return;
	}

	// Create frequency table from input
	unsigned int frequencies[256]{};
	CreateFrequencyTable(infile, frequencies);
	PrintFrequencyTable(frequencies);

	// Create binary tree from priority queue
	Node* tree = CreateBinaryTree(frequencies);

	// Create code map from binary tree
	HuffmanCode prefix;
	HuffmanCodeMap code_map;
	CreateCodeMap(tree, prefix, code_map);
	PrintCodeMap(code_map);

	infile.clear();
	int size = infile.tellg();
	infile.rewind();

	ofbitstream outfile(outfilename);
	if (!outfile.is_open())
	{
		std::cout << "Could not load " << outfilename << ".\n";
		return;
	}

	WriteBinaryTree(outfile, tree);
	WritePadding(outfile, size, frequencies, code_map);
	WriteBits(infile, code_map, outfile);

	infile.close();
	outfile.close();

	delete tree;
}

Huffman::Node* Huffman::ReadBinaryTree(ifbitstream &infile)
{
	bool bit = infile.read_bit();
	if (bit)
	{
		char symbol = infile.read_byte();
		return new LeafNode(symbol);
	}
	else
	{
		Node* left = ReadBinaryTree(infile);
		Node* right = ReadBinaryTree(infile);
		return new InternalNode(left, right);
	}
}

void Huffman::ReadBits(ifbitstream& infile, Node* tree, ofbitstream& outfile)
{
	Node* node = tree;
	while (!infile.eof())
	{
		if (const InternalNode *in = dynamic_cast<const InternalNode*>(node))
		{
			bool bit = infile.read_bit();
			node = bit ? in->right_ : in->left_;
		}
		else if (const LeafNode *ln = dynamic_cast<const LeafNode*>(node))
		{
			outfile.write_byte(ln->symbol_);
			node = tree;
		}
	}
}

void Huffman::ReadPadding(ifbitstream &infile)
{
	char padding = 0;
	infile.read_bits(padding, 3);

	for (int i = 0; i < padding; i++)
	{
		infile.read_bit();
	}
}

void Huffman::Decompress(const std::string& infilename, const std::string& outfilename)
{
	ifbitstream infile(infilename);
	if (!infile.is_open())
	{
		std::cout << "Could not load " << infilename << ".\n";
		return;
	}

	// Create binary tree from input
	Node* tree = ReadBinaryTree(infile);

	// Create code map from binary tree
	HuffmanCode prefix;
	HuffmanCodeMap code_map;
	CreateCodeMap(tree, prefix, code_map);
	PrintCodeMap(code_map);

	ofbitstream outfile(outfilename);
	if (!outfile.is_open())
	{
		std::cout << "Could not load " << outfilename << ".\n";
		return;
	}

	ReadPadding(infile);
	ReadBits(infile, tree, outfile);

	infile.close();
	outfile.close();

	delete tree;
}

