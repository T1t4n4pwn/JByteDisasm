#include "byte_buffer.hpp"

byte_buffer::byte_buffer() 
	: _cur_pos(0), _endian_mode(0)
{

}

byte_buffer::byte_buffer(const std::vector<uint8_t>& data)
	: _buffer(data), _cur_pos(0), _endian_mode(0)
{
	
}

byte_buffer::~byte_buffer()
{
	_cur_pos = ~(0);
}

byte_buffer::byte_buffer(const byte_buffer& rhs)
{
	_buffer = rhs._buffer;
	_cur_pos = rhs._cur_pos;
	_endian_mode = rhs._endian_mode;
}

byte_buffer& byte_buffer::operator=(const byte_buffer& rhs)
{
	_buffer = rhs._buffer;
	_cur_pos = rhs._cur_pos;
	_endian_mode = rhs._endian_mode;
	return *this;
}

byte_buffer& byte_buffer::operator=(const std::vector<uint8_t> data)
{
	_buffer = data;
	_cur_pos = 0;
	return *this;
}

void byte_buffer::set_endian(int mode)
{
	_endian_mode = mode;
}

uint8_t* byte_buffer::raw_data()
{
	return _buffer.data();
}

uint8_t* byte_buffer::get_cur_pos_data()
{
	return &_buffer[_cur_pos];
}

void byte_buffer::copy_buffer(void* buffer, size_t size)
{
	memcpy(buffer, &_buffer[_cur_pos], size);
	_cur_pos += size;
}

void byte_buffer::copy_buffer_current(void* buffer, size_t size)
{
	memcpy(buffer, &_buffer[_cur_pos], size);
}

void byte_buffer::to_big_endian(void* data, size_t bytes_count)
{
	int max_index = bytes_count - 1;

	uint8_t* start_ptr = (uint8_t*)data;

	for (size_t i = 0; i < (bytes_count / 2); i++)
	{
		int end_index = max_index - i;

		uint8_t tmp = start_ptr[i];

		start_ptr[i] = start_ptr[end_index];

		start_ptr[end_index] = tmp;
	}
}

size_t byte_buffer::get_cur_pos()
{
	return _cur_pos;
}

void byte_buffer::set_cur_pos(size_t pos)
{
	if (pos < _buffer.size())
	{
		_cur_pos = pos;
	}
}
