#pragma once
#include <iostream>
#include "constant_pool.hpp"
#include "utils/byte_buffer.hpp"
#include "struct/class_struct.h"

typedef struct
{
	uint16_t attribute_name_index;
	uint32_t attribute_length;
	std::vector<uint8_t> bytes;
}attribute_info_t;

class attribute
{
public:
	attribute(const constant_pool& cp, attribute_info_t info);
	~attribute();

	attribute_info_t attribute_info();
	std::string get_attribute_name();
	size_t get_attribute_length();
	std::vector<uint8_t> get_attribute_data();

private:
	const constant_pool& _cp;
	attribute_info_t _attribute_info;

};