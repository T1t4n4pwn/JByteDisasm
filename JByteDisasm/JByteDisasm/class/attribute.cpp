#include "attribute.hpp"

attribute::attribute(const constant_pool& cp, attribute_info_t info)
	: _cp(cp), _attribute_info(info)
{

}

attribute::~attribute()
{

}

attribute_info_t attribute::attribute_info()
{
	return _attribute_info;
}

std::string attribute::get_attribute_name()
{
	return _cp.get_utf8_value(_attribute_info.attribute_name_index - 1);
}

size_t attribute::get_attribute_length()
{
	return _attribute_info.attribute_length;
}

std::vector<uint8_t> attribute::get_attribute_data()
{
	return _attribute_info.bytes;
}
