#include <boost/program_options.hpp>
using namespace boost::program_options;

#include <iostream>
using namespace std;

#include "Huffman.h"

/* Auxiliary functions for checking input for validity. */

/* Function used to check that 'opt1' and 'opt2' are not specified
 *    at the same time. */
void conflicting_options(const variables_map& vm, 
	const char* opt1, const char* opt2)
{
	if (vm.count(opt1) && !vm[opt1].defaulted() 
		&& vm.count(opt2) && !vm[opt2].defaulted())
		throw logic_error(string("Conflicting options '") 
			+ opt1 + "' and '" + opt2 + "'.");
}

int main(int argc, char* argv[])
{
	try
	{
		options_description desc("Allowed options");
		desc.add_options()
			("help,h", "produce help message")
			("compress,c", "compress file")
			("decompress,x", "decompress file")
		;

		options_description hidden("Hidden options");
		hidden.add_options()
			("input-file", value<string>()->required(), "input file")
			("output-file", value<string>()->required(), "output file")
		;

		options_description all("Allowed options");
		all.add(desc).add(hidden);

		positional_options_description p;
		p.add("input-file", 1);
		p.add("output-file", 1);

		variables_map vm;
		store(command_line_parser(argc, argv).options(all)
			.positional(p).run(), vm);

		if (vm.count("help"))
		{
			cout << "Usage: " << argv[0] << " [options] input-file output-file" << endl;
			cout << desc << endl;
			return 0;
		}

		notify(vm);

		conflicting_options(vm, "compress", "decompress");

		string inputfile = vm["input-file"].as<string>();
		string outputfile = vm["output-file"].as<string>();

		Huffman huffman;
		if (vm.count("compress"))
		{
			huffman.Compress(inputfile, outputfile);
		}
		else if (vm.count("decompress"))
		{
			huffman.Decompress(inputfile, outputfile);
		}
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
	}
}

