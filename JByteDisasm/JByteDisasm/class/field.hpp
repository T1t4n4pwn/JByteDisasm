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
}field_info_t;

class field
{
public:
	field(const std::vector<cp_info_t>& cp_infos, field_info_t info);
	~field();

	field_info_t field_info();
	std::string get_field_name();
	std::string get_field_descript();

private:
	const std::vector<cp_info_t>& _cp_infos;
	field_info_t _field_info;
};