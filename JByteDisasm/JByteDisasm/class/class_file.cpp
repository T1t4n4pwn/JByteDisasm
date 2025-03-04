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

	_cp_infos = _constant_pool.get_cp_infos();

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

std::vector<field> class_file::get_fields()
{
	return _fields;
}

std::vector<field>& class_file::get_fields_ref()
{
	return _fields;
}

std::vector<method> class_file::get_methods()
{
	return _methods;
}

std::vector<method>& class_file::get_methods_ref()
{
	return _methods;
}

std::string class_file::get_sourcefile_name()
{
	if (!_has_sourcefile_attr)
	{
		return "";
	}

	return _cp_infos[_attr_sourcefile.sourcefile_index - 1].utf8_info.bytes;
}

bool class_file::has_innerclass_attr()
{
	return _has_innerclass_attr;
}

bool class_file::has_bootstrap_methods()
{
	return _has_bootstrap_method_attr;
}

attribute_innerclass_t class_file::attr_innerclass()
{
	return _attr_innerclass;
}

attribute_bootstrap_methods_t class_file::attr_bootstrap_methods()
{
	return _attr_bootstrap_methods;
}

void class_file::parse_fields()
{

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



			info.attributes.push_back(attribute(_cp_infos, attri));
		}

		_fields.push_back(field(_cp_infos, info));
	}

}

void class_file::parse_methods()
{

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

			info.attributes.push_back(attribute(_cp_infos, attri));
		}

		_methods.push_back(method(_cp_infos, info));
	}
}

void class_file::parse_attributes()
{

	for (size_t i = 0; i < _attribute_count; i++)
	{
		attribute_info_t attri{ 0 };

		attri.attribute_name_index = _buffer.read<uint16_t>();
		attri.attribute_length = _buffer.read<uint32_t>();

		attri.bytes.resize(attri.attribute_length);

		_buffer.copy_buffer(&attri.bytes[0], attri.attribute_length);

		_attributes.push_back(attribute(_cp_infos, attri));
	}

	for (auto& attr : _attributes)
	{
		if (attr.get_attribute_name() == "SourceFile")
		{
			auto attr_data = attr.get_attribute_data();
			_attr_sourcefile.from_binary(attr_data);

			_has_sourcefile_attr = true;
		}

		if (attr.get_attribute_name() == "InnerClasses")
		{
			auto attr_data = attr.get_attribute_data();
			_attr_innerclass.from_binary(attr_data);

			_has_innerclass_attr = true;
		}

		if (attr.get_attribute_name() == "BootstrapMethods")
		{
			auto attr_data = attr.get_attribute_data();
			_attr_bootstrap_methods.from_binary(attr_data);

			_has_bootstrap_method_attr = true;
		}

	}
}
