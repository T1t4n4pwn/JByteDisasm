#include "constant_pool.hpp"
#include "struct/class_struct.h"

#include <algorithm>

constant_pool::constant_pool() 
	: _cp_count(0)
{

}

constant_pool::~constant_pool()
{

}

void constant_pool::init(byte_buffer* constant_pool_buffer, size_t constant_count)
{
	_buffer = constant_pool_buffer;
	_cp_count = constant_count;
}

bool constant_pool::parse()
{
	_cp_infos.clear();

	size_t cp_cnt = _cp_count - 1;

	for (size_t i = 0; i < cp_cnt; i++)
	{
		cp_info_t pool{};

		if (!parse_constant_data(pool))
		{
			break;
		}

		_cp_infos.push_back(pool);
	}

	return true;
}

cp_info_t constant_pool::get_cp_info(size_t i) const
{
	return _cp_infos[i];
}

const std::vector<cp_info_t>& constant_pool::get_cp_infos() const
{
	return _cp_infos;
}

size_t constant_pool::cp_count() const
{
	return _cp_count;
}

size_t constant_pool::cp_info_count() const
{
	return _cp_infos.size();
}

std::string constant_pool::get_utf8_value(int index) const
{
	return get_utf8_info(index).bytes;
}

cp_utf8_info_t constant_pool::get_utf8_info(int index) const
{
	return _cp_infos[index].utf8_info;
}

int constant_pool::get_int_value(int index) const
{
	return get_int_info(index).value;
}

cp_integer_info_t constant_pool::get_int_info(int index) const
{
	return _cp_infos[index].int_info;
}

float constant_pool::get_float_value(int index) const
{
	uint32_t value = get_float_info(index).value;
	return *(float*)&value;
}

cp_float_info_t constant_pool::get_float_info(int index) const
{
	return _cp_infos[index].float_info;
}

long long constant_pool::get_long_value(int index) const
{
	cp_long_info_t value = get_long_info(index);
	return *(long long*)&value;
}

cp_long_info_t constant_pool::get_long_info(int index) const
{
	return _cp_infos[index].long_info;
}

double constant_pool::get_double_value(int index) const
{
	cp_double_info_t value = get_double_info(index);
	return *(double*)&value;
}

cp_double_info_t constant_pool::get_double_info(int index) const
{
	return _cp_infos[index].double_info;
}

std::string constant_pool::get_class_value(int index) const
{
	return get_utf8_value(get_class_info(index).name_index - 1);
}

cp_class_info_t constant_pool::get_class_info(int index) const
{
	return _cp_infos[index].class_info;
}

std::string constant_pool::get_string_value(int index) const
{
	return get_utf8_value(get_string_info(index).string_index - 1);
}

cp_string_info_t constant_pool::get_string_info(int index) const
{
	return _cp_infos[index].string_info;
}

cp_fieldref_info_t constant_pool::get_fieldref_info(int index) const
{
	return _cp_infos[index].fieldref_info;
}

cp_methodref_info_t constant_pool::get_methodref_info(int index) const
{
	return _cp_infos[index].methodref_info;
}

cp_interface_methodref_info_t constant_pool::get_interface_methodref_info(int index) const
{
	return _cp_infos[index].interface_methodref_info;
}

cp_nameandtype_info_t constant_pool::get_nameandtype_info(int index) const
{
	return _cp_infos[index].name_and_type_info;
}

cp_methodhandle_info_t constant_pool::get_methodhandle_info(int index) const
{
	return _cp_infos[index].method_handle_info;
}

cp_methodtype_info_t constant_pool::get_methodtype_info(int index) const
{
	return _cp_infos[index].method_type_info;
}

cp_invokedynamic_info_t constant_pool::get_invokedynamic_info(int index) const
{
	return _cp_infos[index].invokedynamic_info;
}

bool constant_pool::parse_constant_data(cp_info_t& info)
{

	CP_CONST_TYPE tag = (CP_CONST_TYPE)_buffer->read<unsigned char>();

	info.tag = tag;
	info.index = _cp_infos.size();

	switch (tag)
	{
	case CONSTANT_Utf8:
	{
		uint16_t length = _buffer->read<uint16_t>();
		info.utf8_info.bytes.resize(length);

		_buffer->copy_buffer(&info.utf8_info.bytes[0], length);
		break;
	}
	case CONSTANT_Integer:
	{
		info.int_info.value = _buffer->read<uint32_t>();
		break;
	}
	case CONSTANT_Float:
	{
		info.float_info.value = _buffer->read<uint32_t>();
		break;
	}
	case CONSTANT_Long:
	{
		info.long_info.low_value = _buffer->read<uint32_t>();
		info.long_info.high_value = _buffer->read<uint32_t>();

		break;
	}
	case CONSTANT_Double:
	{
		info.double_info.low_value = _buffer->read<uint32_t>();
		info.double_info.high_value = _buffer->read<uint32_t>();

		break;
	}
	case CONSTANT_Class:
	{
		info.class_info.name_index = _buffer->read<uint16_t>();
		break;
	}
	case CONSTANT_String:
	{
		info.string_info.string_index = _buffer->read<uint16_t>();
		break;
	}
	case CONSTANT_Fieldref:
	{
		info.fieldref_info.class_index = _buffer->read<uint16_t>();
		info.fieldref_info.name_and_type_index = _buffer->read<uint16_t>();
		break;
	}
	case CONSTANT_Methodref:
	{
		info.methodref_info.class_index = _buffer->read<uint16_t>();
		info.methodref_info.name_and_type_index = _buffer->read<uint16_t>();
		break;
	}
	case CONSTANT_InterfaceMethodref:
	{
		info.interface_methodref_info.class_index = _buffer->read<uint16_t>();
		info.interface_methodref_info.name_and_type_index = _buffer->read<uint16_t>();
		break;
	}
	case CONSTANT_NameAndType:
	{
		info.name_and_type_info.name_index = _buffer->read<uint16_t>();
		info.name_and_type_info.descriptor_index = _buffer->read<uint16_t>();
		break;
	}
	case CONSTANT_MethodHandle_info:
	{
		info.method_handle_info.reference_kind = _buffer->read<uint8_t>();
		info.method_handle_info.reference_index = _buffer->read<uint16_t>();
		break;
	}
	case CONSTANT_MethodType:
	{
		info.method_type_info.descriptor_index = _buffer->read<uint16_t>();
		break;
	}
	case CONSTANT_InvokeDynamic:
	{
		info.invokedynamic_info.bootstrap_method_attr_index = _buffer->read<uint16_t>();
		info.invokedynamic_info.name_and_type_index = _buffer->read<uint16_t>();
		break;
	}
	default:
	{
		_buffer->set_cur_pos(_buffer->get_cur_pos() - 1);
		return false;
	}
	}

	if (tag == CONSTANT_Long || tag == CONSTANT_Double)
	{
		_cp_infos.push_back(info);
	}

	return true;
}


