#pragma once
#include <iostream>
#include "constant_pool.hpp"
#include "attribute.hpp"

typedef struct
{
	uint16_t access_flags;
	uint16_t name_index;
	uint16_t descript_index;
	uint16_t attribute_count;
	std::vector<attribute> attributes;
}method_info_t;

class method
{
public:

	method(const std::vector<cp_info_t>& cp_infos, method_info_t method_info);
	~method();

	method_info_t method_info();
	std::string get_method_name();
	std::string get_method_descript();

private:
	const std::vector<cp_info_t>& _cp_infos;
	method_info_t _method_info;
};