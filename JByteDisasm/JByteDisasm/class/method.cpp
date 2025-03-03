#include "method.hpp"

void method::init(const byte_buffer& method_buffer, size_t method_count)
{
	_buffer = method_buffer;
	_method_count = method_count;
}

bool method::parse()
{

	method_info_t info;

	info.access_flags = _buffer.read<uint16_t>();
	info.name_index = _buffer.read<uint16_t>();
	info.descript_index = _buffer.read<uint16_t>();
	info.attribute_count = _buffer.read<uint16_t>();
	
	for (size_t i = 0; i < info.attribute_count; i++)
	{
		attribute_info_t attri{ 0 };

		attri.attribute_name_index = _buffer.read<uint16_t>();
		attri.attribute_length = _buffer.read<uint16_t>();

		attri.bytes.resize(attri.attribute_length);

		_buffer.copy_buffer(&attri.bytes[0], attri.attribute_length);

		info.attributes.push_back(attri);
	}

	return true;
}
