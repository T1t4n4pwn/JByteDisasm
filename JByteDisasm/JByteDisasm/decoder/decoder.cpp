#include "decoder/decoder.hpp"

decoder::decoder()
{

}

decoder::~decoder()
{

}

void decoder::setup(method* me)
{
	_method = me;
	_code_bytes = me->get_byte_code();
	_cp = &me->cp();
	_buffer = _code_bytes;
	_buffer.set_endian(1);
}

void decoder::process_ldc_opstr(char* opstr_buf, size_t size, const cp_info_t& cp)
{
	switch (cp.tag)
	{
	case CONSTANT_Integer:
	{
		sprintf_s(opstr_buf, size, "%d", cp.int_info.value);
		break;
	}
	case CONSTANT_Float:
	{
		float value = *(float*)&cp.float_info.value;
		sprintf_s(opstr_buf, size, "%f", value);
		break;
	}
	case CONSTANT_String:
	{
		std::string str = _cp->get_utf8_value(cp.string_info.string_index - 1);
		memcpy(opstr_buf, &str[0], str.length());
		break;
	}
	case CONSTANT_Class:
	{
		std::string str = _cp->get_utf8_value(cp.class_info.name_index - 1);
		memcpy(opstr_buf, &str[0],  str.length());
		break;
	}
	case CONSTANT_Long:
	{
		uint64_t value = *(uint64_t*)&cp.long_info;
		sprintf_s(opstr_buf, size, "%lld", value);
		break;
	}
	case CONSTANT_Double:
	{
		double value = *(double*)&cp.double_info;
		sprintf_s(opstr_buf, size, "%lf", value);
		break;
	}
	}
}

template<typename T>
T calc_abs_offset(T current, T target, size_t insn_size)
{
	return current + target - insn_size;
}

void decoder::process_opcode_operands(bytecode_info_t& insn)
{

	char opstr_buf[512]{ 0 };

	switch (insn.mnemonic)
	{
	case BIPUSH:
	{
		insn.op_count = 1;

		opcode_operand_t op{};

		op.type = OPERAND_TYPE_IMM;
		op.imm.bit_width = 8;
		op.imm.imm = _buffer.read<uint8_t>();

		insn.oprands.push_back(op);

		break;
	}
	case SIPUSH:
	{
		insn.op_count = 1;

		opcode_operand_t op{};

		op.type = OPERAND_TYPE_IMM;
		op.imm.bit_width = 16;
		op.imm.imm = _buffer.read<uint16_t>();

		insn.oprands.push_back(op);

		break;
	}
	case LDC:
	{
		insn.op_count = 1;
		
		opcode_operand_t op{};

		op.type = OPERAND_TYPE_INDEX;
		op.index.bit_width = 8;
		op.index.index = _buffer.read<uint8_t>();

		insn.oprands.push_back(op);

		cp_info_t cp = _cp->get_cp_info(op.index.index - 1);

		process_ldc_opstr(opstr_buf, sizeof(opstr_buf), cp);

		break;
	}
	case LDC_W:
	case LDC2_W:
	{
		insn.op_count = 1;

		opcode_operand_t op{};

		op.type = OPERAND_TYPE_INDEX;
		op.index.bit_width = 16;
		op.index.index = _buffer.read<uint16_t>();

		insn.oprands.push_back(op);

		cp_info_t cp = _cp->get_cp_info(op.index.index - 1);

		process_ldc_opstr(opstr_buf, sizeof(opstr_buf), cp);

		break;
	}
	case ILOAD:
	case LLOAD:
	case FLOAD:
	case DLOAD:
	case ALOAD:
	case ISTORE:
	case LSTORE:
	case FSTORE:
	case DSTORE:
	case ASTORE:
	case RET:
	{
		insn.op_count = 1;

		opcode_operand_t op{};

		op.type = OPERAND_TYPE_INDEX;
		op.index.bit_width = 8;
		op.index.index = _buffer.read<uint8_t>();
		
		insn.oprands.push_back(op);

		sprintf_s(opstr_buf, sizeof(opstr_buf), "local.%d", op.index.index);

		break;
	}
	case IINC:
	{
		insn.op_count = 1;

		opcode_operand_t op{};

		op.type = OPERAND_TYPE_IINC;
		op.iinc.table_index = _buffer.read<uint8_t>();
		op.iinc.const_value = _buffer.read<uint8_t>();

		insn.oprands.push_back(op);

		sprintf_s(opstr_buf, sizeof(opstr_buf), "local.%d + %d", op.iinc.table_index, op.iinc.const_value);

		break;
	}
	case IFEQ:
	case IFNE:
	case IFLT:
	case IFGE:
	case IFGT:
	case IFLE:
	case IF_ICMPEQ:
	case IF_ICMPNE:
	case IF_ICMPLT:
	case IF_ICMPGE:
	case IF_ICMPGT:
	case IF_ICMPLE:
	case IF_ACMPEQ:
	case IF_ACMPNE:
	case GOTO:
	case JSR:
	{
		insn.op_count = 1;

		opcode_operand_t op{};

		op.type = OPERAND_TYPE_OFFSET;
		op.offset.bit_width = 16;
		op.offset.offset = _buffer.read<uint16_t>();

		insn.oprands.push_back(op);

		uint16_t off = calc_abs_offset<uint16_t>(_buffer.get_cur_pos(), op.offset.offset, op.offset.bit_width / 8 + 1);

		sprintf_s(opstr_buf, sizeof(opstr_buf), "L%X", off);

		break;
	}
	case TABLESWITCH:
	{
		//TODO FIX
		insn.op_count = 1;
		opcode_operand_t op{};

		op.type = OPERAND_TYPE_TABLESWITCH;

		//process pad
		uint8_t pad = 0;
		do
		{
			pad = _buffer.read<uint8_t>();
		} while (pad == 0);

		//move back the buffer pos
		_buffer.set_cur_pos(_buffer.get_cur_pos() - 1);

		op.tableswitch.default_offset = _buffer.read<uint32_t>();
		op.tableswitch.low_byte = _buffer.read<uint32_t>();
		op.tableswitch.high_byte = _buffer.read<uint32_t>();

		size_t size_of_jumps = op.tableswitch.high_byte - op.tableswitch.low_byte + 1;

		for (size_t i = 0; i < size_of_jumps; i++)
		{
			op.tableswitch.jumps.push_back(_buffer.read<uint32_t>());
		}

		insn.oprands.push_back(op);


		std::string str = "Dflt: ";

		char buf[64]{ 0 };
		sprintf_s(buf, sizeof(buf), "L%X", op.tableswitch.default_offset);

		str.append(buf);

		for (const auto& e : op.tableswitch.jumps)
		{
			memset(buf, 0, sizeof(buf));
			sprintf_s(buf, sizeof(buf), "|L%X", e);
			str.append(buf);
		}

		memcpy(opstr_buf, &str[0], str.length());

		break;
	}
	case LOOKUPSWITCH:
	{
		//TODO FIX
		insn.op_count = 1;
		opcode_operand_t op{};

		op.type = OPERAND_TYPE_LOOKUPSWITCH;

		//process pad
		uint8_t pad = 0;
		do
		{
			pad = _buffer.read<uint8_t>();
		} while (pad == 0);

		//move back the buffer pos
		_buffer.set_cur_pos(_buffer.get_cur_pos() - 1);

		op.lookupswitch.default_offset = _buffer.read<uint32_t>();
		op.lookupswitch.pairs_count = _buffer.read<uint32_t>();

		for (size_t i = 0; i < op.lookupswitch.pairs_count; i++)
		{
			match_pair_t pair{};
			pair.match = _buffer.read<uint32_t>();
			pair.offset = _buffer.read<uint32_t>();

			op.lookupswitch.match_pairs.push_back(pair);
		}

		insn.oprands.push_back(op);

		std::string str = "Dflt: ";

		char buf[64]{ 0 };
		sprintf_s(buf, sizeof(buf), "L%X", op.offset.offset);

		str.append(buf);

		for (const auto& pair : op.lookupswitch.match_pairs)
		{
			memset(buf, 0, sizeof(buf));
			sprintf_s(buf, sizeof(buf), " [%d|L%X]", pair.match, pair.offset);
			str.append(buf);
		}

		memcpy(opstr_buf, &str[0], str.length());

		break;
	}
	case WIDE:
	{
		//TODO

		break;
	}
	case MULTIANEWARRAY:
	{
		//TODO
		insn.op_count = 1;
		opcode_operand_t op{};

		op.type = OPERAND_TYPE_MULTIANEWARRAY;
		op.multianewarray.index = _buffer.read<uint16_t>();
		op.multianewarray.dimensions = _buffer.read<uint8_t>();

		insn.oprands.push_back(op);

		std::string str = _cp->get_class_value(op.multianewarray.index - 1);
		memcpy(opstr_buf, &str[0], str.length());

		break;
	}
	case GOTO_W:
	case JSR_W:
	{
		//TODO
		insn.op_count = 1;

		opcode_operand_t op{};

		op.type = OPERAND_TYPE_OFFSET;
		op.offset.bit_width = 32;
		op.offset.offset = _buffer.read<uint32_t>();

		insn.oprands.push_back(op);

		sprintf_s(opstr_buf, sizeof(opstr_buf), "L%X", op.offset.offset);

		break;
	}
	case GETFIELD:
	case PUTFIELD:
	case GETSTATIC:
	case PUTSTATIC:
	{
		//Fieldref 
		insn.op_count = 1;

		opcode_operand_t op{};

		op.type = OPERAND_TYPE_INDEX;

		op.index.bit_width = 16;
		op.index.index = _buffer.read<uint16_t>();

		insn.oprands.push_back(op);

		cp_fieldref_info_t field_info = _cp->get_fieldref_info(op.index.index - 1);
		cp_nameandtype_info_t name_and_type = _cp->get_nameandtype_info(field_info.name_and_type_index - 1);

		std::string class_name = _cp->get_class_value(field_info.class_index - 1);

		std::string field_name = _cp->get_utf8_value(name_and_type.name_index - 1);
		std::string field_desc = _cp->get_utf8_value(name_and_type.descriptor_index - 1);

		std::string full_name = class_name + "." + field_name + " " + field_desc;

		memcpy(opstr_buf, &full_name[0], full_name.length());

		break;
	}
	case INVOKEVIRTUAL:
	case INVOKESPECIAL:
	case INVOKESTATIC:
	{
		//Methodref 
		insn.op_count = 1;

		opcode_operand_t op{};

		op.type = OPERAND_TYPE_INDEX;

		op.index.bit_width = 16;
		op.index.index = _buffer.read<uint16_t>();

		insn.oprands.push_back(op);

		cp_methodref_info_t methodref_info = _cp->get_methodref_info(op.index.index - 1);
		cp_nameandtype_info_t name_and_type = _cp->get_nameandtype_info(methodref_info.name_and_type_index - 1);

		std::string class_name = _cp->get_class_value(methodref_info.class_index - 1);
		std::string method_name = _cp->get_utf8_value(name_and_type.name_index - 1);
		std::string method_desc = _cp->get_utf8_value(name_and_type.descriptor_index - 1);

		std::string full_name = class_name + "." + method_name + " " + method_desc;

		memcpy(opstr_buf, &full_name[0], full_name.length());

		break;
	}
	case INVOKEINTERFACE:
	{
		//InterfaceMethodref
		opcode_operand_t op{};

		op.type = OPERAND_TYPE_INVOKEINTERFACE;
		
		op.invokeinterface.index = _buffer.read<uint16_t>();
		op.invokeinterface.arg_count = _buffer.read<uint8_t>();

		_buffer.read<uint8_t>(); //pad

		insn.oprands.push_back(op);

		cp_interface_methodref_info_t interface_info = _cp->get_interface_methodref_info(op.invokeinterface.index - 1);

		cp_nameandtype_info_t name_and_type = _cp->get_nameandtype_info(interface_info.name_and_type_index - 1);

		std::string class_name = _cp->get_class_value(interface_info.class_index - 1);
		std::string method_name = _cp->get_utf8_value(name_and_type.name_index - 1);
		std::string method_desc = _cp->get_utf8_value(name_and_type.descriptor_index - 1);

		std::string full_name = class_name + "." + method_name + " " + method_desc;

		memcpy(opstr_buf, &full_name[0], full_name.length());

		break;
	}
	case INVOKEDYNAMIC:
	{
		//InvokeDynamic
		insn.op_count = 1;

		opcode_operand_t op{};

		op.type = OPERAND_TYPE_INDEX;

		op.index.bit_width = 16;
		op.index.index = _buffer.read<uint16_t>();

		_buffer.read<uint16_t>(); //pad

		insn.oprands.push_back(op);

		cp_invokedynamic_info_t invoke_info = _cp->get_invokedynamic_info(op.index.index - 1);
		cp_nameandtype_info_t name_and_type = _cp->get_nameandtype_info(invoke_info.name_and_type_index - 1);

		std::string method_name = _cp->get_utf8_value(name_and_type.name_index - 1);
		std::string method_desc = _cp->get_utf8_value(name_and_type.descriptor_index - 1);

		std::string full_name = method_name + " " + method_desc;

		memcpy(opstr_buf, &full_name[0], full_name.length());

		break;
	}
	case NEW:
	case ANEWARRAY:
	case CHECKCAST:
	case INSTANCEOF:
	{
		//Class
		insn.op_count = 1;

		opcode_operand_t op{};

		op.type = OPERAND_TYPE_INDEX;
		op.index.bit_width = 16;
		op.index.index = _buffer.read<uint16_t>();

		insn.oprands.push_back(op);

		std::string class_name = _cp->get_class_value(op.index.index - 1);

		memcpy(opstr_buf, &class_name[0], class_name.length());

		break;
	}
	case NEWARRAY:
	{
		insn.op_count = 1;

		opcode_operand_t op{};

		op.type = OPERAND_TYPE_NEWARRAY;

		op.newarray.array_type = (array_type_t)_buffer.read<uint8_t>();

		insn.oprands.push_back(op);

		std::string type_str = get_array_type_str(op.newarray.array_type);

		memcpy(opstr_buf, &type_str[0], type_str.length());

		break;
	}
	default:
		break;
	}

	insn.op_str = opstr_buf;

}

size_t decoder::decode_all(std::vector<bytecode_info_t>& infos, bool is_skip_invalid)
{

	size_t offset = 0;
	size_t byte_size = _code_bytes.size();
	
	infos.clear();

	bytecode_info_t insn;

	while (offset < byte_size)
	{
		opcode_t opcode = (opcode_t)_buffer.read<uint8_t>();
		insn.mnemonic = opcode;

		auto mnem_str_opt = get_opcode_str(opcode);

		insn.valid = mnem_str_opt.has_value();

		if (!insn.valid && !is_skip_invalid)
		{
			break;
		}

		insn.mnemonic_str = get_opcode_str(opcode).value_or("");

		size_t before_cur_pos = _buffer.get_cur_pos();

		process_opcode_operands(insn);

		size_t after_cur_pos = _buffer.get_cur_pos();

		size_t op_size = after_cur_pos - before_cur_pos;

		insn.bytes.resize(op_size + 1);

		insn.bytes[0] = opcode;

		if (after_cur_pos > before_cur_pos)
		{
			_buffer.set_cur_pos(before_cur_pos);
			_buffer.copy_buffer_current(&insn.bytes[1], op_size);
			_buffer.set_cur_pos(after_cur_pos);
		}

		infos.push_back(insn);

		offset += insn.bytes.size();
	}

	return size_t();
}
