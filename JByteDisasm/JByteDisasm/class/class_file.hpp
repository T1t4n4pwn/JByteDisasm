#include <iostream>
#include <vector>
#include "utils/byte_buffer.hpp"
#include "constant_pool.hpp"
#include "method.hpp"
#include "field.hpp"

class class_file
{
public:
	class_file(unsigned char* data, size_t length);
	class_file(const std::vector<uint8_t>& data);
	~class_file();

	bool parse();
	bool is_valid();

	constant_pool& cp_pool();

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
};