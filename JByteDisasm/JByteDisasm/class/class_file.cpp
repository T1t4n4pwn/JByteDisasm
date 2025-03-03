#include "class_file.hpp"

class_file::class_file(unsigned char* data, size_t length)
	: _is_valid(false)
{

	_class_data.resize(length);
	memcpy(&_class_data[0], data, length);

	_buffer = _class_data;

	_buffer.set_endian(1);
	
}

class_file::class_file(const std::vector<uint8_t>& data)
	: _class_data(data), _buffer(data), _is_valid(false)
{

}

class_file::~class_file()
{
	_is_valid = false;
}

bool class_file::parse()
{
	uint32_t magic = _buffer.read<uint32_t>();
	if (magic != 0xCAFEBABE)
	{
		return false;
	}

	_minor_version = _buffer.read<uint16_t>();
	_major_version = _buffer.read<uint16_t>();
	size_t constant_pool_count = _buffer.read<uint16_t>();
	
	_constant_pool.init(_buffer, constant_pool_count);
	if (!_constant_pool.parse())
	{
		return false;
	}

	_access_flags = _buffer.read<uint16_t>();
	_this_class_index = _buffer.read<uint16_t>();
	_super_class_index = _buffer.read<uint16_t>();
	_interface_count = _buffer.read<uint16_t>();

	for (size_t i = 0; i < _interface_count; i++)
	{
		_interface_indexes.push_back(_buffer.read<uint16_t>());
	}

	_field_count = _buffer.read<uint16_t>();


	_is_valid = true;
	return true;
}

bool class_file::is_valid()
{
	return _is_valid;
}
