#include "method.hpp"

method::method(const std::vector<cp_info_t>& cp_infos, method_info_t method_info)
	: _cp_infos(cp_infos), _method_info(method_info)
{

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
	return _cp_infos[_method_info.descript_index].utf8_info.bytes;
}