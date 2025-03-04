#include "method.hpp"
#include <algorithm>

method::method(const std::vector<cp_info_t>& cp_infos, method_info_t method_info)
	: _cp_infos(cp_infos), _method_info(method_info)
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
			_attr_code.from_binary(attr_data, cp_infos);

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
	return _cp_infos[_method_info.name_index - 1].utf8_info.bytes;
}

std::string method::get_method_descript()
{
	return _cp_infos[_method_info.descript_index - 1].utf8_info.bytes;
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
