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
	_constant_pool_count = _buffer.read<uint16_t>();
	
	_constant_pool.init(&_buffer, _constant_pool_count);
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
	parse_fields();

	_method_count = _buffer.read<uint16_t>();
	parse_methods();

	_attribute_count = _buffer.read<uint16_t>();
	parse_attributes();

	_is_valid = true;
	return true;
}

bool class_file::is_valid()
{
	return _is_valid;
}

constant_pool& class_file::cp_pool()
{
	return _constant_pool;
}

void class_file::parse_fields()
{
	auto cp_infos = _constant_pool.get_cp_infos();

	for (size_t i = 0; i < _field_count; i++)
	{
		field_info_t info{};

		info.access_flags = _buffer.read<uint16_t>();
		info.name_index = _buffer.read<uint16_t>();
		info.descript_index = _buffer.read<uint16_t>();
		info.attribute_count = _buffer.read<uint16_t>();

		for (size_t j = 0; j < info.attribute_count; j++)
		{
			attribute_info_t attri{ 0 };

			attri.attribute_name_index = _buffer.read<uint16_t>();
			attri.attribute_length = _buffer.read<uint32_t>();

			attri.bytes.resize(attri.attribute_length);

			_buffer.copy_buffer(&attri.bytes[0], attri.attribute_length);



			info.attributes.push_back(attribute(cp_infos, attri));
		}

		_fields.push_back(field(cp_infos, info));
	}

}

void class_file::parse_methods()
{
	auto cp_infos = _constant_pool.get_cp_infos();

	for (size_t i = 0; i < _method_count; i++)
	{
		method_info_t info;

		info.access_flags = _buffer.read<uint16_t>();
		info.name_index = _buffer.read<uint16_t>();
		info.descript_index = _buffer.read<uint16_t>();
		info.attribute_count = _buffer.read<uint16_t>();

		for (size_t j = 0; j < info.attribute_count; j++)
		{
			attribute_info_t attri{ 0 };

			attri.attribute_name_index = _buffer.read<uint16_t>();
			attri.attribute_length = _buffer.read<uint32_t>();

			attri.bytes.resize(attri.attribute_length);

			_buffer.copy_buffer(&attri.bytes[0], attri.attribute_length);

			info.attributes.push_back(attribute(cp_infos, attri));
		}

		_methods.push_back(method(cp_infos, info));
	}
}

void class_file::parse_attributes()
{
	auto cp_infos = _constant_pool.get_cp_infos();

	for (size_t i = 0; i < _attribute_count; i++)
	{
		attribute_info_t attri{ 0 };

		attri.attribute_name_index = _buffer.read<uint16_t>();
		attri.attribute_length = _buffer.read<uint32_t>();

		attri.bytes.resize(attri.attribute_length);

		_buffer.copy_buffer(&attri.bytes[0], attri.attribute_length);

		_attributes.push_back(attribute(cp_infos, attri));
	}
}
