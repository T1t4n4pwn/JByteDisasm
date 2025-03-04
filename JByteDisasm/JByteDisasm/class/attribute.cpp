#include "attribute.hpp"

attribute::attribute(const std::vector<cp_info_t>& cp_infos, attribute_info_t info)
	: _cp_infos(cp_infos), _attribute_info(info)
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
	return _cp_infos[_attribute_info.attribute_name_index - 1].utf8_info.bytes;
}

size_t attribute::get_attribute_length()
{
	return _attribute_info.attribute_length;
}

std::vector<uint8_t> attribute::get_attribute_data()
{
	return _attribute_info.bytes;
}
