#pragma once
#include <iostream>
#include <vector>
#include <optional>

#include "utils/byte_buffer.hpp"
#include "struct/class_struct.h"

typedef struct
{
	uint16_t length;
	std::string bytes;
}cp_utf8_info_t;

typedef struct
{
	uint32_t value;
}cp_integer_info_t;

typedef struct
{
	uint32_t value;
}cp_float_info_t;

typedef struct
{
	uint32_t high_value;
	uint32_t low_value;
}cp_long_info_t;

typedef struct
{
	uint32_t high_value;
	uint32_t low_value;
}cp_double_info_t;

typedef struct
{
	uint16_t name_index;
}cp_class_info_t;

typedef struct
{
	uint16_t string_index;
}cp_string_info_t;

typedef struct
{
	uint16_t class_index;
	uint16_t name_and_type_index;
}cp_fieldref_info_t;

typedef struct
{
	uint16_t class_index;
	uint16_t name_and_type_index;
}cp_methodref_info_t;

typedef struct
{
	uint16_t class_index;
	uint16_t name_and_type_index;
}cp_interface_methodref_info_t;

typedef struct
{
	uint16_t name_index;
	uint16_t descriptor_index;
}cp_nameandtype_info_t;

typedef struct
{
	uint8_t reference_kind;
	uint16_t reference_index;
}cp_methodhandle_info_t;

typedef struct
{
	uint16_t descriptor_index;
}cp_methodtype_info_t;

typedef struct
{
	uint16_t bootstrap_method_attr_index;
	uint16_t name_and_type_index;
}cp_invokedynamic_info_t;

typedef struct
{
	int index;
	CP_CONST_TYPE tag;

	cp_utf8_info_t utf8_info;
	cp_integer_info_t int_info;
	cp_float_info_t float_info;
	cp_long_info_t long_info;
	cp_double_info_t double_info;
	cp_class_info_t class_info;
	cp_string_info_t string_info;
	cp_fieldref_info_t fieldref_info;
	cp_methodref_info_t methodref_info;
	cp_interface_methodref_info_t interface_methodref_info;
	cp_nameandtype_info_t name_and_type_info;
	cp_methodhandle_info_t method_handle_info;
	cp_methodtype_info_t method_type_info;
	cp_invokedynamic_info_t invokedynamic_info;

}cp_info_t;

class constant_pool
{
public:
	constant_pool();
	~constant_pool();

	void init(const byte_buffer& constant_pool_buffer, size_t constant_count);
	bool parse();

	cp_info_t get_cp_info(size_t i);

	size_t cp_count();
	size_t cp_info_count();
	
private:
	
	bool parse_constant_data(cp_info_t& info);

private:
	byte_buffer _buffer;
	size_t _cp_count;

	std::vector<cp_info_t> _cp_infos;

	std::vector<cp_utf8_info_t> _cp_utf8_infos;
	std::vector<cp_integer_info_t> _cp_int_infos;
	std::vector<cp_float_info_t> _cp_float_infos;
	std::vector<cp_long_info_t> _cp_long_infos;
	std::vector<cp_double_info_t> _cp_double_infos;
	std::vector<cp_class_info_t> _cp_class_infos;
	std::vector<cp_string_info_t> _cp_string_infos;
	std::vector<cp_fieldref_info_t> _cp_fieldref_infos;
	std::vector<cp_methodref_info_t> _cp_methodref_infos;
	std::vector<cp_interface_methodref_info_t> _cp_interface_methodref_infos;
	std::vector<cp_nameandtype_info_t> _cp_name_and_type_infos;
	std::vector<cp_methodhandle_info_t> _cp_methodhandle_infos;
	std::vector<cp_methodtype_info_t> _cp_methodtype_infos;
	std::vector<cp_invokedynamic_info_t> _cp_invokedynamic_infos;

};