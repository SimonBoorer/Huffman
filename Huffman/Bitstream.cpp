#include "Bitstream.h"

bool ibitstream::read_bit()
{
	if (pos_ == 8)
	{
		cur_byte_ = get();
		pos_ = 0;
	}
	int result = ((cur_byte_ & (1 << pos_)) != 0);
	pos_++;
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
	return pos_;
}

bool ibitstream::is_open()
{
	return true;
}

void obitstream::write_bit(bool bit)
{
	if (pos_ == 8)
	{
		put(cur_byte_);
		cur_byte_ = 0;
		pos_ = 0;
	}

	if (bit)
	{
		cur_byte_ |= (1 << pos_);
	}

	pos_++;
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
	if (pos_ > 0)
	{
		put(cur_byte_);
	}
}

bool obitstream::is_open()
{
	return true;
}

ifbitstream::ifbitstream()
{
	init(&fb_);
}

ifbitstream::ifbitstream(const char* filename)
{
	init(&fb_);
	open(filename);
}

ifbitstream::ifbitstream(const std::string& filename)
{
	init(&fb_);
	open(filename);
}

void ifbitstream::open(const char* filename)
{
	if (!fb_.open(filename, std::ios::in | std::ios::binary))
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
	return fb_.is_open();
}

void ifbitstream::close()
{
	if (!fb_.close())
	{
		setstate(std::ios::failbit);
	}
}

ofbitstream::ofbitstream()
{
	init(&fb_);
}

ofbitstream::~ofbitstream()
{
	flush_bits();
}

ofbitstream::ofbitstream(const char* filename)
{
	init(&fb_);
	open(filename);
}

ofbitstream::ofbitstream(const std::string& filename)
{
	init(&fb_);
	open(filename);
}

void ofbitstream::open(const char* filename)
{
	if (!fb_.open(filename, std::ios::out | std::ios::binary))
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
	return fb_.is_open();
}

void ofbitstream::close()
{
	flush_bits();
	if (!fb_.close())
	{
		setstate(std::ios::failbit);
	}
}

