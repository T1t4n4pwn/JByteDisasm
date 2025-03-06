#pragma once
#include <iostream>
#include <vector>
#include <optional>

#include "utils/byte_buffer.hpp"
#include "struct/class_struct.h"

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

	void init(byte_buffer* constant_pool_buffer, size_t constant_count);
	bool parse();

	cp_info_t get_cp_info(size_t i) const;
	const std::vector<cp_info_t>& get_cp_infos() const;

	size_t cp_count() const;
	size_t cp_info_count() const;

	std::string get_utf8_value(int index) const;
	cp_utf8_info_t get_utf8_info(int index) const;

	int get_int_value(int index) const;
	cp_integer_info_t get_int_info(int index) const;

	float get_float_value(int index) const;
	cp_float_info_t get_float_info(int index) const;

	long long get_long_value(int index) const;
	cp_long_info_t get_long_info(int index) const;

	double get_double_value(int index) const;
	cp_double_info_t get_double_info(int index) const;

	std::string get_class_value(int index) const;
	cp_class_info_t get_class_info(int index) const;

	std::string get_string_value(int index) const;
	cp_string_info_t get_string_info(int index) const;

	cp_fieldref_info_t get_fieldref_info(int index) const;
	cp_methodref_info_t get_methodref_info(int index) const;
	cp_interface_methodref_info_t get_interface_methodref_info(int index) const;
	cp_nameandtype_info_t get_nameandtype_info(int index) const;
	cp_methodhandle_info_t get_methodhandle_info(int index) const;
	cp_methodtype_info_t get_methodtype_info(int index) const;
	cp_invokedynamic_info_t get_invokedynamic_info(int index) const;
	
private:
	
	bool parse_constant_data(cp_info_t& info);

private:
	byte_buffer* _buffer;
	size_t _cp_count;

	std::vector<cp_info_t> _cp_infos;

};