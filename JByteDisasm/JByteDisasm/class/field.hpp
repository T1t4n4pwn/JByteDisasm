#pragma once
#include <iostream>
#include "constant_pool.hpp"
#include "attribute.hpp"

typedef struct
{
	uint16_t access_flags;
	uint16_t name_index;
	uint16_t descript_index;
	uint16_t attribute_count;
	std::vector<attribute> attributes;
}field_info_t;

typedef struct attribute_constant_value
{
	uint16_t constant_value_index;

	void from_binary(const std::vector<uint8_t>& data)
	{
		byte_buffer buffer = data;
		buffer.set_endian(1);

		constant_value_index = buffer.read<uint16_t>();
	}

}attribute_constant_value_t;

class field
{
public:
	field(const std::vector<cp_info_t>& cp_infos, field_info_t info);
	~field();

	field_info_t field_info();
	std::string get_field_name();
	std::string get_field_descript();
	
	bool get_const_value_cp_info(cp_info_t& info)
	{
		if (!is_final())
		{
			return false;
		}

		if (_field_info.attribute_count == 0)
		{
			return false;
		}

		std::string attribute_name = _field_info.attributes[0].get_attribute_name();
		if (strstr(attribute_name.c_str(), "ConstantValue") == NULL)
		{
			return false;
		}

		std::vector<uint8_t> attri_data = _field_info.attributes[0].get_attribute_data();

		attribute_constant_value_t attr_constant_value;
		attr_constant_value.from_binary(attri_data);

		info = _cp_infos[attr_constant_value.constant_value_index - 1];

		return true;
	}

	CP_CONST_TYPE get_const_value_type()
	{
		cp_info_t cp;
		if (!get_const_value_cp_info(cp))
		{
			return CP_CONST_TYPE::CONSTANT_NONE;
		}

		return cp.tag;
	}

	template<typename T>
	/*
		You must make sure field's type is correct
	*/
	bool get_const_value(T* value)
	{
		cp_info_t cp;
		if (!get_const_value_cp_info(cp))
		{
			return false;
		}

		if constexpr (std::is_same_v<T, int>)
		{
			*value = (int)cp.int_info.value;
		}
		else if constexpr (std::is_same_v<T, float>)
		{
			*value = *(float*)&cp.float_info.value;
		}
		else if constexpr (std::is_same_v<T, long long>)
		{
			*value = *(int64_t*)&cp.long_info;
		}
		else if constexpr (std::is_same_v<T, double>)
		{
			*value = *(double*)&cp.double_info;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			*value = _cp_infos[cp.string_info.string_index - 1].utf8_info.bytes;
		}
		else
		{
			static_assert(std::is_same_v<T, void>, "Unsupported type!");
			return false;
		}

		return true;
	}

	bool is_public();
	bool is_protected();
	bool is_private();
	bool is_static();
	bool is_final();
	bool is_interface();

private:


private:
	const std::vector<cp_info_t>& _cp_infos;
	field_info_t _field_info;
};