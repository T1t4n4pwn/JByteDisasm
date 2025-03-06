#include "method.hpp"
#include <algorithm>

method::method(const constant_pool& cp, method_info_t method_info)
	: _cp(cp), _method_info(method_info)
{
	
	if (method_info.attributes.empty())
	{
		return;
	}

	for (auto& attr : method_info.attributes)
	{
		if (attr.get_attribute_name() == "Code")
		{
			auto attr_data = attr.get_attribute_data();
			_attr_code.from_binary(attr_data, cp);

			_has_code_attr = true;

			break;
		}

	}

	if (!_has_code_attr)
	{
		return;
	}

	for (auto& attr : _attr_code.attributes)
	{
		if (attr.get_attribute_name() == "LineNumberTable")
		{
			auto attr_data = attr.get_attribute_data();
			_attr_line_number.from_binary(attr_data);

			_has_line_number_attr = true;
		}

		if (attr.get_attribute_name() == "LocalVariableTable")
		{
			auto attr_data = attr.get_attribute_data();
			_attr_local_variable.from_binary(attr_data);

			_has_local_variable_attr = true;
		}

		if (attr.get_attribute_name() == "LocalVariableTypeTable")
		{
			auto attr_data = attr.get_attribute_data();
			_attr_local_vartype.from_binary(attr_data);

			_has_local_vartype_attr = true;
		}

	}

}

method::~method()
{

}

method_info_t method::method_info()
{
	return _method_info;
}

std::string method::get_method_name()
{
	return _cp.get_utf8_value(_method_info.name_index - 1);
}

std::string method::get_method_descript()
{
	return _cp.get_utf8_value(_method_info.descript_index - 1);
}

bool method::has_code_attr()
{
	return _has_code_attr;
}

bool method::has_line_number_attr()
{
	return _has_line_number_attr;
}

bool method::has_local_variable_attr()
{
	return _has_local_variable_attr;
}

bool method::has_local_vartype_attr()
{
	return _has_local_vartype_attr;
}

attribute_code_t method::attribute_code()
{
	return _attr_code;
}

attribute_line_number_t method::attribute_line_number()
{
	return _attr_line_number;
}

attribute_local_variable_t method::attribute_local_variable()
{
	return _attr_local_variable;
}

attribute_local_vartype_t method::attribute_local_vartype()
{
	return _attr_local_vartype;
}

std::vector<uint8_t> method::get_byte_code()
{
	return _attr_code.code;
}

std::vector<uint8_t>& method::get_byte_code_ref()
{
	return _attr_code.code;
}

const constant_pool& method::cp()
{
	return _cp;
}

bool method::is_public()
{
	return _method_info.access_flags & ACCESS_FLAGS::ACC_PUBLIC;
}

bool method::is_protected()
{
	return _method_info.access_flags & ACCESS_FLAGS::ACC_PROTECTED;
}

bool method::is_private()
{
	return _method_info.access_flags & ACCESS_FLAGS::ACC_PRIVATE;
}

bool method::is_static()
{
	return _method_info.access_flags & ACCESS_FLAGS::ACC_STATIC;
}

bool method::is_final()
{
	return _method_info.access_flags & ACCESS_FLAGS::ACC_FINAL;
}

bool method::is_interface()
{
	return _method_info.access_flags & ACCESS_FLAGS::ACC_INTERFACE;
}


void attribute_code::from_binary(const std::vector<uint8_t>& data, const constant_pool& cp)
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

		attribute attri(cp, info);

		attributes.push_back(attri);
	}

}

void attribute_line_number::from_binary(const std::vector<uint8_t>& data)
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

void attribute_local_variable::from_binary(const std::vector<uint8_t>& data)
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

void attribute_local_vartype::from_binary(const std::vector<uint8_t>& data)
{
	byte_buffer buffer = data;
	buffer.set_endian(1);

	local_variable_type_table_length = buffer.read<uint16_t>();

	for (size_t i = 0; i < local_variable_type_table_length; i++)
	{
		local_vartype_table_t vartype{};

		vartype.start_pc = buffer.read<uint16_t>();
		vartype.length = buffer.read<uint16_t>();
		vartype.name_index = buffer.read<uint16_t>();
		vartype.signature_index = buffer.read<uint16_t>();
		vartype.index = buffer.read<uint16_t>();

		local_vartype_tables.push_back(vartype);
	}

}
