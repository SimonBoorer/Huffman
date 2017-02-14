#include "Bitstream.h"

bool ibitstream::read_bit()
{
	if (pos == 8)
	{
		cur_byte = get();
		pos = 0;
	}
	int result = ((cur_byte & (1 << pos)) != 0);
	pos++;
	return result == 1;
}

void ibitstream::read_bits(char& byte, int bits)
{
	for (int i = 0; i < bits; i++)
	{
		bool bit = read_bit();
		if (bit)
		{
			byte |= (1 << i);
		}
	}
}

char ibitstream::read_byte()
{
	char byte = 0;
	for (int i = 0; i < 8; i++)
	{
		bool bit = read_bit();
		if (bit)
		{
			byte |= (1 << i);
		}
	}

	return byte;
}

void ibitstream::rewind()
{
	clear();
	seekg(0, std::ios::beg);
}

int ibitstream::tellbitg()
{
	return pos;
}

bool ibitstream::is_open()
{
	return true;
}

void obitstream::write_bit(bool bit)
{
	if (pos == 8)
	{
		put(cur_byte);
		cur_byte = 0;
		pos = 0;
	}

	if (bit)
	{
		cur_byte |= (1 << pos);
	}

	pos++;
}

void obitstream::write_bits(const std::vector<bool>& bits)
{
	for (const auto it : bits)
	{
		write_bit(it);
	}
}

void obitstream::write_bits(char byte, int bits)
{
	for (int i = 0; i < bits; i++)
	{
		int bit = ((byte & (1 << i)) != 0);
		write_bit(bit == 1);
	}
}

void obitstream::write_byte(char byte)
{
	for (int i = 0; i < 8; i++)
	{
		int bit = ((byte & (1 << i)) != 0);
		write_bit(bit == 1);
	}
}

void obitstream::flush_bits()
{
	if (pos > 0)
	{
		put(cur_byte);
	}
}

bool obitstream::is_open()
{
	return true;
}

ifbitstream::ifbitstream()
{
	init(&fb);
}

ifbitstream::ifbitstream(const char* filename)
{
	init(&fb);
	open(filename);
}

ifbitstream::ifbitstream(const std::string& filename)
{
	init(&fb);
	open(filename);
}

void ifbitstream::open(const char* filename)
{
	if (!fb.open(filename, std::ios::in | std::ios::binary))
	{
		setstate(std::ios::failbit);
	}
}

void ifbitstream::open(const std::string& filename)
{
	open(filename.c_str());
}

bool ifbitstream::is_open()
{
	return fb.is_open();
}

void ifbitstream::close()
{
	if (!fb.close())
	{
		setstate(std::ios::failbit);
	}
}

ofbitstream::ofbitstream()
{
	init(&fb);
}

ofbitstream::~ofbitstream()
{
	flush_bits();
}

ofbitstream::ofbitstream(const char* filename)
{
	init(&fb);
	open(filename);
}

ofbitstream::ofbitstream(const std::string& filename)
{
	init(&fb);
	open(filename);
}

void ofbitstream::open(const char* filename)
{
	if (!fb.open(filename, std::ios::out | std::ios::binary))
	{
		setstate(std::ios::failbit);
	}
}

void ofbitstream::open(const std::string& filename)
{
	open(filename.c_str());
}

bool ofbitstream::is_open()
{
	return fb.is_open();
}

void ofbitstream::close()
{
	flush_bits();
	if (!fb.close())
	{
		setstate(std::ios::failbit);
	}
}

