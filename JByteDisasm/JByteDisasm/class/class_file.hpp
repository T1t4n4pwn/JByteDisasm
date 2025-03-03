#include <iostream>
#include <vector>
#include "utils/byte_buffer.hpp"
#include "constant_pool.hpp"

class class_file
{
public:
	class_file(unsigned char* data, size_t length);
	class_file(const std::vector<uint8_t>& data);
	~class_file();

	bool parse();
	bool is_valid();

	constant_pool& cp_pool()
	{
		return _constant_pool;
	}

private:
	std::vector<uint8_t> _class_data;
	byte_buffer _buffer;
	bool _is_valid;

	uint16_t _minor_version;
	uint16_t _major_version;
	constant_pool _constant_pool;
	uint16_t _access_flags;
	uint16_t _this_class_index;
	uint16_t _super_class_index;
	uint16_t _interface_count;
	std::vector<uint16_t> _interface_indexes;
	uint16_t _field_count;

};