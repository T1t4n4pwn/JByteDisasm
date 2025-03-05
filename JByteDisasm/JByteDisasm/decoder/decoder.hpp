#pragma once
#include <iostream>
#include <vector>
#include "class/method.hpp"
#include "utils/byte_buffer.hpp"
#include "opcode.hpp"

class decoder
{
public:
	decoder();
	~decoder();

	void setup(method* me);
	void process_ldc_opstr(char* opstr_buf, size_t size, const cp_info_t& cp);
	size_t decode_all(std::vector<bytecode_info_t>& infos, bool is_skip_invalid = false);

private:

	void process_opcode_operands(bytecode_info_t& insn);

private:
	std::vector<uint8_t> _code_bytes;
	std::vector<cp_info_t> _cp_infos;
	byte_buffer _buffer;
	method* _method;
};