#pragma once

enum kernel_func_param_type {
	KERNEL_TYPE_NONE,
	KERNEL_TYPE_INT,
	KERNEL_TYPE_CHAR,
	KERNEL_TYPE_STRING,
};

typedef struct {
	const char* func_name;
	kernel_func_param_type param_1_type;
	const char* param_1;
	kernel_func_param_type param_2_type;
	const char* param_2;
	kernel_func_param_type param_3_type;
	const char* param_3;
	kernel_func_param_type param_4_type;
	const char* param_4;
	kernel_func_param_type param_5_type;
	const char* param_5;
} kernel_func_t;

extern kernel_func_t kernel_a_funcs[];
extern kernel_func_t kernel_b_funcs[];
extern kernel_func_t kernel_c_funcs[];
