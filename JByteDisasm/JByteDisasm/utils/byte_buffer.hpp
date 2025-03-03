#pragma once
#include <vector>

class byte_buffer
{
public:
	byte_buffer();
	byte_buffer(const std::vector<uint8_t>& data);
	~byte_buffer();

	byte_buffer(const byte_buffer& rhs);
	byte_buffer& operator=(const byte_buffer& rhs);
	byte_buffer& operator=(const std::vector<uint8_t> data);

	// 0 little endian
	// 1 big endian
	void set_endian(int mode);

	template<typename T>
	/*Will move the pos*/
	T read()
	{
		T* ptr = (T*)&_buffer[_cur_pos];
		_cur_pos += sizeof(T);

		if (_endian_mode)
		{
			return big_endian<T>(*ptr);
		}

		return *ptr;
	}

	template<typename T>
	/*Will move the pos*/
	void write(T data)
	{
		T* ptr = (T*)&_buffer[_cur_pos];
		_cur_pos += sizeof(T);

		if (_endian_mode)
		{
			*ptr = big_endian<T>(data);
			return;
		}

		*ptr = data;
	}

	template<typename T>
	T read_current()
	{
		T* ptr = (T*)&_buffer[_cur_pos];

		if (_endian_mode)
		{
			return big_endian<T>(*ptr);
		}

		return *ptr;
	}

	template<typename T>
	void write_current(T data)
	{
		T* ptr = (T*)&_buffer[_cur_pos];

		if (_endian_mode)
		{
			*ptr = big_endian<T>(data);
			return;
		}

		*ptr = data;
	}

	template <typename T>
	T big_endian(T data)
	{

		to_big_endian(&data, sizeof(T));

		return data;
	}


	/*Will move the pos*/
	void copy_buffer(void* buffer, size_t size);
	void copy_buffer_current(void* buffer, size_t size);


	void to_big_endian(void* data, size_t bytes_count);

	size_t get_cur_pos();
	void set_cur_pos(size_t pos);

	uint8_t* raw_data();
	uint8_t* get_cur_pos_data();

private:
	std::vector<uint8_t> _buffer;
	size_t _cur_pos;
	int _endian_mode;
};