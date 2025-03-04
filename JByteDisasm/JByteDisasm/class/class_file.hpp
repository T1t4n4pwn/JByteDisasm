#include <iostream>
#include <vector>
#include "utils/byte_buffer.hpp"
#include "constant_pool.hpp"
#include "method.hpp"
#include "field.hpp"

typedef struct attribute_sourcefile
{
	uint16_t sourcefile_index;

	void from_binary(const std::vector<uint8_t>& data)
	{
		byte_buffer buffer = data;
		buffer.set_endian(1);

		sourcefile_index = buffer.read<uint16_t>();
	}

}attribute_sourcefile_t;

typedef struct inner_class_info
{
	uint16_t inner_class_info_index;
	uint16_t outer_class_info_index;
	uint16_t inner_name_index;
	uint16_t inner_class_access_flags;
}inner_class_info_t;

typedef struct attribute_innerclass
{
	uint16_t number_of_classes;
	std::vector<inner_class_info_t> classes;

	void from_binary(const std::vector<uint8_t>& data)
	{
		byte_buffer buffer = data;
		buffer.set_endian(1);

		number_of_classes = buffer.read<uint16_t>();

		for (size_t i = 0; i < number_of_classes; i++)
		{
			inner_class_info_t inner{};

			inner.inner_class_info_index = buffer.read<uint16_t>();
			inner.outer_class_info_index = buffer.read<uint16_t>();
			inner.inner_name_index = buffer.read<uint16_t>();
			inner.inner_class_access_flags = buffer.read<uint16_t>();

			classes.push_back(inner);
		}

	}

}attribute_innerclass_t;

typedef struct bootstrap_method
{
	uint16_t bootstrap_method_ref;
	uint16_t num_bootstrap_arguments;
	std::vector<uint16_t> bootstrap_arguments;
}bootstrap_method_t;

typedef struct attribute_booststrap_methods
{
	uint16_t num_bootstrap_methods;
	std::vector<bootstrap_method_t> bootstrap_methods;

	void from_binary(const std::vector<uint8_t>& data)
	{
		byte_buffer buffer = data;
		buffer.set_endian(1);

		num_bootstrap_methods = buffer.read<uint16_t>();

		for (size_t i = 0; i < num_bootstrap_methods; i++)
		{
			bootstrap_method_t method{};

			method.bootstrap_method_ref = buffer.read<uint16_t>();
			method.num_bootstrap_arguments = buffer.read<uint16_t>();

			for (size_t j = 0; j < method.num_bootstrap_arguments; j++)
			{
				method.bootstrap_arguments.push_back(buffer.read<uint16_t>());
			}

			bootstrap_methods.push_back(method);
		}

	}

}attribute_bootstrap_methods_t;

class class_file
{
public:
	class_file(unsigned char* data, size_t length);
	class_file(const std::vector<uint8_t>& data);
	~class_file();

	bool parse();
	bool is_valid();

	constant_pool& cp_pool();
	std::vector<field> get_fields();
	std::vector<field>& get_fields_ref();
	std::vector<method> get_methods();
	std::vector<method>& get_methods_ref();

	std::string get_sourcefile_name();

	bool has_innerclass_attr();
	bool has_bootstrap_methods();

	attribute_innerclass_t attr_innerclass();
	attribute_bootstrap_methods_t attr_bootstrap_methods();

private:

	void parse_fields();
	void parse_methods();
	void parse_attributes();

private:
	std::vector<uint8_t> _class_data;
	byte_buffer _buffer;
	bool _is_valid = false;

	/*主要数据开始*/
	uint16_t _minor_version = 0;
	uint16_t _major_version = 0;
	uint16_t _constant_pool_count = 0;
	constant_pool _constant_pool;
	uint16_t _access_flags = 0;
	uint16_t _this_class_index = 0;
	uint16_t _super_class_index = 0;
	uint16_t _interface_count = 0;
	std::vector<uint16_t> _interface_indexes;
	uint16_t _field_count = 0;
	std::vector<field> _fields;
	uint16_t _method_count = 0;
	std::vector<method> _methods;
	uint16_t _attribute_count = 0;
	std::vector<attribute> _attributes;
	/*主要数据结束*/

	std::vector<cp_info_t> _cp_infos;

	bool _has_sourcefile_attr = false;
	attribute_sourcefile_t _attr_sourcefile;

	bool _has_innerclass_attr = false;
	attribute_innerclass_t _attr_innerclass;

	bool _has_bootstrap_method_attr = false;
	attribute_bootstrap_methods_t _attr_bootstrap_methods;
};