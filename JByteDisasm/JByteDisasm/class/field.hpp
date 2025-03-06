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

	void from_binary(const std::vector<uint8_t>& data);

}attribute_constant_value_t;

class field
{
public:
	field(const constant_pool& cp, field_info_t info);
	~field();

	field_info_t field_info();
	std::string get_field_name();
	std::string get_field_descript();
	
	bool get_const_value_cp_info(cp_info_t& info);

	CP_CONST_TYPE get_const_value_type();

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
			*value = _cp.get_utf8_value(cp.string_info.string_index - 1);
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
	const constant_pool& _cp;
	field_info_t _field_info;

};