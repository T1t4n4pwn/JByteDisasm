#pragma once
#include <iostream>

#include "utils/byte_buffer.hpp"
#include "struct/class_struct.h"

typedef struct
{
	uint16_t access_flags;
	uint16_t name_index;
	uint16_t descript_index;
	uint16_t attribute_count;
	std::vector<attribute_info_t> attributes;
}method_info_t;

class method
{
public:
	method()
	{
		
	}

	~method()
	{
		
	}

	void init(const byte_buffer& method_buffer, size_t method_count);
	bool parse();

private:
	byte_buffer _buffer;
	size_t _method_count;

	std::vector<method_info_t>
};