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

	void from_binary(const std::vector<uint8_t>& data, const std::vector<cp_info_t>& cp_infos)
	{
		byte_buffer buffer = data;
		buffer.set_endian(1);

		max_stack = buffer.read<uint16_t>();
		max_locals = buffer.read<uint16_t>();

		code_length = buffer.read<uint32_t>();
		
		code.resize(code_length);

		buffer.copy_buffer(&code[0], code_length);
		
		exception_table_length = buffer.read<uint16_t>();
		for (size_t i = 0; i < exception_table_length; i++)
		{
			exception_table_t except_table{};
			except_table.start_pc = buffer.read<uint16_t>();
			except_table.end_pc = buffer.read<uint16_t>();
			except_table.handler_pc = buffer.read<uint16_t>();
			except_table.catch_type = buffer.read<uint16_t>();

			exception_tables.push_back(except_table);
		}

		attribute_count = buffer.read<uint16_t>();

		for (size_t i = 0; i < attribute_count; i++)
		{
			attribute_info_t info;

			info.attribute_name_index = buffer.read<uint16_t>();
			info.attribute_length = buffer.read<uint32_t>();
			
			info.bytes.resize(info.attribute_length);

			buffer.copy_buffer(&info.bytes[0], info.attribute_length);

			attribute attri(cp_infos, info);
			
			attributes.push_back(attri);
		}

	}

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

	void from_binary(const std::vector<uint8_t>& data)
	{
		byte_buffer buffer = data;
		buffer.set_endian(1);

		line_number_table_length = buffer.read<uint16_t>();

		for (size_t i = 0; i < line_number_table_length; i++)
		{
			line_number_table_t line{};

			line.start_pc = buffer.read<uint16_t>();
			line.line_number = buffer.read<uint16_t>();
			
			line_number_tables.push_back(line);
		}

	}

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

	void from_binary(const std::vector<uint8_t>& data)
	{
		byte_buffer buffer = data;
		buffer.set_endian(1);

		local_variable_length = buffer.read<uint16_t>();

		for (size_t i = 0; i < local_variable_length; i++)
		{
			local_variable var{ 0 };

			var.start_pc = buffer.read<uint16_t>();
			var.length = buffer.read<uint16_t>();
			var.name_index = buffer.read<uint16_t>();
			var.descriptor_index = buffer.read<uint16_t>();
			var.index = buffer.read<uint16_t>();

			local_variable_tables.push_back(var);
		}

	}

}attribute_local_variable_t;

class method
{
public:

	method(const std::vector<cp_info_t>& cp_infos, method_info_t method_info);
	~method();

	method_info_t method_info();
	std::string get_method_name();
	std::string get_method_descript();

	bool has_code_attr();
	bool has_line_number_attr();
	bool has_local_variable_attr();

	attribute_code_t attribute_code();
	attribute_line_number_t attribute_line_number();
	attribute_local_variable_t attribute_local_variable();

	std::vector<uint8_t> get_byte_code();
	std::vector<uint8_t>& get_byte_code_ref();
	
	bool is_public();
	bool is_protected();
	bool is_private();
	bool is_static();
	bool is_final();
	bool is_interface();

private:

	const std::vector<cp_info_t>& _cp_infos;
	method_info_t _method_info;

	bool _has_code_attr = false;
	attribute_code_t _attr_code;

	bool _has_line_number_attr = false;
	attribute_line_number_t _attr_line_number;

	bool _has_local_variable_attr = false;
	attribute_local_variable_t _attr_local_variable;

};