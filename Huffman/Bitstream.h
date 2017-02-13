#ifndef BITSTREAM_H_
#define BITSTREAM_H_

#include <istream>
#include <ostream>
#include <fstream>
#include <vector>

class ibitstream : public std::istream
{
public:
	ibitstream() : std::istream(NULL), cur_byte(0), pos(8) {}

	bool read_bit();
	void read_bits(char& byte, int bits);
	char read_byte();

	void rewind();

	int tellbitg();

	virtual bool is_open();

private:
	unsigned char cur_byte;
	int pos;
};

class obitstream : public std::ostream
{
public:
	obitstream() : std::ostream(NULL), cur_byte(0), pos(0) {}

	void write_bit(bool bit);
	void write_bits(const std::vector<bool>& bits);
	void write_bits(char bytes, int bits);
	void write_byte(char byte);

	void flush_bits();

	virtual bool is_open();

private:
	unsigned char cur_byte;
	int pos;
};

class ifbitstream : public ibitstream
{
public:
	ifbitstream();

	ifbitstream(const char* filename);
	ifbitstream(const std::string& filename);

	void open(const char* filename);
	void open(const std::string &filename);

	bool is_open() override;

	void close();

private:
	std::filebuf fb;
};

class ofbitstream : public obitstream
{
public:
	ofbitstream();
	~ofbitstream();

	ofbitstream(const char* filename);
	ofbitstream(const std::string& filename);

	void open(const char* filename);
	void open(const std::string &filename);

	bool is_open() override;

	void close();

private:
	std::filebuf fb;
};

#endif // BITSTREAM_H_