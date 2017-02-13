#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include "Bitstream.h"

#include <string>
#include <vector>
#include <map>

typedef std::vector<bool> HuffmanCode;
typedef std::map<char, std::vector<bool>> HuffmanCodeMap;

class Huffman
{
protected:
	class Node
	{
	public:
		virtual ~Node() { }

		unsigned int frequency_;

	protected:
		Node(unsigned int frequency)
			: frequency_(frequency) { }
	};

	class InternalNode : public Node
	{
	public:
		InternalNode(Node* left, Node* right)
			: Node(left->frequency_ + right->frequency_), left_(left), right_(right) { }

		InternalNode()
			: Node(0), left_(nullptr), right_(nullptr) { }

		InternalNode::~InternalNode()
		{
			delete left_;
			delete right_;
		}

		Node *left_, *right_;
	};

	class LeafNode : public Node
	{
	public:
		LeafNode(unsigned char symbol, unsigned int frequency = 0)
			: Node(frequency), symbol_(symbol) { }

		char symbol_;
	};

	class CompareNode
	{
	public:
		bool operator() (const Node* lhs, const Node* rhs) const
		{
			return lhs->frequency_ > rhs->frequency_;
		}
	};

public:
	void Compress(const std::string& infilename, const std::string& outfilename);

	void Decompress(const std::string& infilename, const std::string& outfilename);

protected:
	// Compress:
	void CreateFrequencyTable(ifbitstream& infile, unsigned int frequencies[256]);
	void PrintFrequencyTable(unsigned int frequencies[256]);

	Node* CreateBinaryTree(unsigned int frequencies[256]);

	void CreateCodeMap(Node* node, HuffmanCode& prefix, HuffmanCodeMap& code_map);
	void PrintCodeMap(HuffmanCodeMap& code_map);

	void WriteBinaryTree(ofbitstream& outfile, const Node *node);
	void WritePadding(ofbitstream& outfile, const int size, const unsigned int frequencies[256], HuffmanCodeMap& code_map);
	void WriteBits(ifbitstream& infile, HuffmanCodeMap& code_map, ofbitstream& outfile);

	// Decompress:
	Node* ReadBinaryTree(ifbitstream& infile);
	void ReadPadding(ifbitstream &infile);
	void ReadBits(ifbitstream& infile, Node* node, ofbitstream& outfile);
};

#endif // HUFFMAN_H_