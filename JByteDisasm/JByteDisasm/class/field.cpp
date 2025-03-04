#include "field.hpp"

field::field(const std::vector<cp_info_t>& cp_infos, field_info_t info)
	: _cp_infos(cp_infos), _field_info(info)
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
	return _cp_infos[_field_info.name_index].utf8_info.bytes;
}

std::string field::get_field_descript()
{
	return _cp_infos[_field_info.descript_index].utf8_info.bytes;
}
