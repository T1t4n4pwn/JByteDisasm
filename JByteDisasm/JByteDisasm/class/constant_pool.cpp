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

	for (size_t i = 0; i < _cp_count - 1; i++)
	{
		cp_info_t pool{};

		if (!parse_constant_data(pool))
		{
			continue;
		}

		_cp_infos.push_back(pool);

	}

	std::for_each(_cp_infos.begin(), _cp_infos.end(),
		[&](const cp_info_t& e)
		{
			switch (e.tag)
			{
			case CONSTANT_Utf8:
				_cp_utf8_infos.push_back(e.utf8_info);
				break;
			case CONSTANT_Integer:
				_cp_int_infos.push_back(e.int_info);
				break;
			case CONSTANT_Float:
				_cp_float_infos.push_back(e.float_info);
				break;
			case CONSTANT_Long:
				_cp_long_infos.push_back(e.long_info);
				break;
			case CONSTANT_Double:
				_cp_double_infos.push_back(e.double_info);
				break;
			case CONSTANT_Class:
				_cp_class_infos.push_back(e.class_info);
				break;
			case CONSTANT_String:
				_cp_string_infos.push_back(e.string_info);
				break;
			case CONSTANT_Fieldref:
				_cp_fieldref_infos.push_back(e.fieldref_info);
				break;
			case CONSTANT_Methodref:
				_cp_methodref_infos.push_back(e.methodref_info);
				break;
			case CONSTANT_InterfaceMethodref:
				_cp_interface_methodref_infos.push_back(e.interface_methodref_info);
				break;
			case CONSTANT_NameAndType:
				_cp_name_and_type_infos.push_back(e.name_and_type_info);
				break;
			case CONSTANT_MethodHandle_info:
				_cp_methodhandle_infos.push_back(e.method_handle_info);
				break;
			case CONSTANT_MethodType:
				_cp_methodtype_infos.push_back(e.method_type_info);
				break;
			case CONSTANT_InvokeDynamic:
				_cp_invokedynamic_infos.push_back(e.invokedynamic_info);
				break;
			default:
				break;
			}
			
		}
	);

	return true;
}

cp_info_t constant_pool::get_cp_info(size_t i)
{
	return _cp_infos[i];
}

const std::vector<cp_info_t>& constant_pool::get_cp_infos()
{
	return _cp_infos;
}

size_t constant_pool::cp_count()
{
	return _cp_count;
}

size_t constant_pool::cp_info_count()
{
	return _cp_infos.size();
}

bool constant_pool::parse_constant_data(cp_info_t& info)
{

	CP_CONST_TYPE tag = (CP_CONST_TYPE)_buffer->read<char>();

	info.tag = tag;
	info.index = _cp_infos.size();

	switch (tag)
	{
	case CONSTANT_Utf8:
	{
		uint16_t length = _buffer->read<uint16_t>();
		info.utf8_info.bytes.resize(length + 1);

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
		info.long_info.high_value = _buffer->read<uint32_t>();
		info.long_info.low_value = _buffer->read<uint32_t>();
		break;
	}
	case CONSTANT_Double:
	{
		info.double_info.high_value = _buffer->read<uint32_t>();
		info.double_info.low_value = _buffer->read<uint32_t>();
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
		return false;
	}
	}

	return true;
}


