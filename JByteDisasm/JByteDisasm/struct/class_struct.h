#pragma once
#include <stdint.h>
#include <vector>

enum ACCESS_FLAGS : uint16_t
{
	ACC_PUBLIC = 0x0001,
	ACC_PRIVATE = 0x0002,
	ACC_PROTECTED = 0x0004,
	ACC_STATIC = 0x0008,
	ACC_FINAL = 0x0010,
	ACC_SUPER = 0x0020,
	ACC_VOLATILE = 0x0040,
	ACC_TRANSIENT = 0x0080,
	ACC_NATIVE = 0x0100,
	ACC_INTERFACE = 0x0200,
	ACC_ABSTRACT = 0x0400,
	ACC_STRICT = 0x0800
};

enum CP_CONST_TYPE : char
{
	CONSTANT_NONE = 0,
	CONSTANT_Utf8 = 1,
	CONSTANT_Integer = 3,
	CONSTANT_Float = 4,
	CONSTANT_Long = 5,
	CONSTANT_Double = 6,
	CONSTANT_Class = 7,
	CONSTANT_String = 8,
	CONSTANT_Fieldref = 9,
	CONSTANT_Methodref = 10,
	CONSTANT_InterfaceMethodref = 11,
	CONSTANT_NameAndType = 12,
	CONSTANT_MethodHandle_info = 15,
	CONSTANT_MethodType = 16,
	CONSTANT_InvokeDynamic = 18
};

typedef struct
{
	uint16_t length;
	std::string bytes;
}cp_utf8_info_t;

typedef struct
{
	uint32_t value;
}cp_integer_info_t;

typedef struct
{
	uint32_t value;
}cp_float_info_t;

typedef struct
{
	uint32_t high_value;
	uint32_t low_value;
}cp_long_info_t;

typedef struct
{
	uint32_t high_value;
	uint32_t low_value;
}cp_double_info_t;

typedef struct
{
	uint16_t name_index;
}cp_class_info_t;

typedef struct
{
	uint16_t string_index;
}cp_string_info_t;

typedef struct
{
	uint16_t class_index;
	uint16_t name_and_type_index;
}cp_fieldref_info_t;

typedef struct
{
	uint16_t class_index;
	uint16_t name_and_type_index;
}cp_methodref_info_t;

typedef struct
{
	uint16_t class_index;
	uint16_t name_and_type_index;
}cp_interface_methodref_info_t;

typedef struct
{
	uint16_t name_index;
	uint16_t descriptor_index;
}cp_nameandtype_info_t;

typedef struct
{
	uint8_t reference_kind;
	uint16_t reference_index;
}cp_methodhandle_info_t;

typedef struct
{
	uint16_t descriptor_index;
}cp_methodtype_info_t;

typedef struct
{
	uint16_t bootstrap_method_attr_index;
	uint16_t name_and_type_index;
}cp_invokedynamic_info_t;

//typedef struct
//{
//	uint16_t access_flags;
//	uint16_t name_index;
//	uint16_t descript_index;
//	uint16_t attribute_count;
//	attribute_info_t attributes[1];
//}field_info_t;

//typedef struct
//{
//	uint16_t access_flags;
//	uint16_t name_index;
//	uint16_t descript_index;
//	uint16_t attribute_count;
//	attribute_info_t attributes[1];
//}method_info_t;

//typedef struct
//{
//	uint32_t magic;
//	uint16_t minor_version;
//	uint16_t major_version;
//	uint16_t constant_count;
//	constant_pool_t constant_pool[1];
//	uint16_t access_flags;
//	uint16_t this_class_index;
//	uint16_t super_class_index;
//	uint16_t interface_count;
//	uint16_t interface_indexes[1];
//	uint16_t field_count;
//	field_info_t fields[1];
//	uint16_t method_count;
//	method_info_t methods[1];
//	uint16_t attribute_count;
//	attribute_info_t attributes[1];
//}class_file_t;