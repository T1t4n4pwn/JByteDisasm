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
	return _cp_infos[_field_info.name_index - 1].utf8_info.bytes;
}

std::string field::get_field_descript()
{
	return _cp_infos[_field_info.descript_index - 1].utf8_info.bytes;
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