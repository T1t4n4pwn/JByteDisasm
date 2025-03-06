#include "field.hpp"

field::field(const constant_pool& cp, field_info_t info)
	: _cp(cp), _field_info(info)
{

}

field::~field()
{

}

field_info_t field::field_info()
{
	return _field_info;
}

std::string field::get_field_name()
{
	return _cp.get_utf8_value(_field_info.name_index - 1);
}

std::string field::get_field_descript()
{
	return _cp.get_utf8_value(_field_info.descript_index - 1);
}

bool field::get_const_value_cp_info(cp_info_t& info)
{
	if (!is_final())
	{
		return false;
	}

	if (_field_info.attribute_count == 0)
	{
		return false;
	}

	std::string attribute_name = _field_info.attributes[0].get_attribute_name();
	if (strstr(attribute_name.c_str(), "ConstantValue") == NULL)
	{
		return false;
	}

	std::vector<uint8_t> attri_data = _field_info.attributes[0].get_attribute_data();

	attribute_constant_value_t attr_constant_value;
	attr_constant_value.from_binary(attri_data);

	info = _cp.get_cp_info(attr_constant_value.constant_value_index - 1);

	return true;
}

CP_CONST_TYPE field::get_const_value_type()
{
	cp_info_t cp;
	if (!get_const_value_cp_info(cp))
	{
		return CP_CONST_TYPE::CONSTANT_NONE;
	}

	return cp.tag;
}

bool field::is_public()
{
	return _field_info.access_flags & ACCESS_FLAGS::ACC_PUBLIC;
}

bool field::is_protected()
{
	return _field_info.access_flags & ACCESS_FLAGS::ACC_PROTECTED;
}

bool field::is_private()
{
	return _field_info.access_flags & ACCESS_FLAGS::ACC_PRIVATE;
}

bool field::is_static()
{
	return _field_info.access_flags & ACCESS_FLAGS::ACC_STATIC;
}

bool field::is_final()
{
	return _field_info.access_flags & ACCESS_FLAGS::ACC_FINAL;
}

bool field::is_interface()
{
	return _field_info.access_flags & ACCESS_FLAGS::ACC_INTERFACE;
}

void attribute_constant_value::from_binary(const std::vector<uint8_t>& data)
{
	byte_buffer buffer = data;
	buffer.set_endian(1);

	constant_value_index = buffer.read<uint16_t>();
}
