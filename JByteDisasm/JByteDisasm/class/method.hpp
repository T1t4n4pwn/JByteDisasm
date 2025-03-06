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

typedef struct
{
	uint16_t start_pc;
	uint16_t end_pc;
	uint16_t handler_pc;
	uint16_t catch_type;
}exception_table_t;

typedef struct attribute_code
{
	uint16_t max_stack;
	uint16_t max_locals;
	uint32_t code_length;
	std::vector<uint8_t> code;
	uint16_t exception_table_length;
	std::vector<exception_table_t> exception_tables;
	uint16_t attribute_count;
	std::vector<attribute> attributes;

	void from_binary(const std::vector<uint8_t>& data, const constant_pool& cp);

}attribute_code_t;

typedef struct line_number_table
{
	uint16_t start_pc;
	uint16_t line_number;
}line_number_table_t;

typedef struct attribute_line_number
{
	uint16_t line_number_table_length;
	std::vector<line_number_table_t> line_number_tables;

	void from_binary(const std::vector<uint8_t>& data);

}attribute_line_number_t;

typedef struct local_variable
{
	uint16_t start_pc;
	uint16_t length;
	uint16_t name_index;
	uint16_t descriptor_index;
	uint16_t index;
}local_variable_t;

typedef struct attribute_local_variable
{
	uint16_t local_variable_length;
	std::vector<local_variable_t> local_variable_tables;

	void from_binary(const std::vector<uint8_t>& data);

}attribute_local_variable_t;

typedef struct local_vartype_table
{
	uint16_t start_pc;
	uint16_t length;
	uint16_t name_index;
	uint16_t signature_index;
	uint16_t index;
}local_vartype_table_t;

typedef struct attribute_local_vartype
{
	uint16_t local_variable_type_table_length;
	std::vector<local_vartype_table_t> local_vartype_tables;

	void from_binary(const std::vector<uint8_t>& data);

}attribute_local_vartype_t;

class method
{
public:

	method(const constant_pool& cp_infos, method_info_t method_info);
	~method();

	method_info_t method_info();
	std::string get_method_name();
	std::string get_method_descript();

	bool has_code_attr();
	bool has_line_number_attr();
	bool has_local_variable_attr();
	bool has_local_vartype_attr();

	attribute_code_t attribute_code();
	attribute_line_number_t attribute_line_number();
	attribute_local_variable_t attribute_local_variable();
	attribute_local_vartype_t attribute_local_vartype();

	std::vector<uint8_t> get_byte_code();
	std::vector<uint8_t>& get_byte_code_ref();

	const constant_pool& cp();
	
	bool is_public();
	bool is_protected();
	bool is_private();
	bool is_static();
	bool is_final();
	bool is_interface();

private:

	const constant_pool& _cp;
	method_info_t _method_info;

	bool _has_code_attr = false;
	attribute_code_t _attr_code;

	bool _has_line_number_attr = false;
	attribute_line_number_t _attr_line_number;

	bool _has_local_variable_attr = false;
	attribute_local_variable_t _attr_local_variable;

	bool _has_local_vartype_attr = false;
	attribute_local_vartype_t _attr_local_vartype;

};