/**
*	ekwa.h - Here declared all structc and
*	public functions of virtual machine.
*/
#ifndef EKWA_LANGUAGE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
/*
*	Custom types.
*/
#define u16_t	unsigned short int
#define bool	unsigned char
#define byte	unsigned char
//#define size_t	unsigned int
/*
*	Boolean values.
*/
#define false	0
#define true	1
/**
*	Constant vm debug enabling.
*/
#define EKWA_DEBUG
/**
*	Max size of byte code buffer.
*/
#define BUFFLEN 65535
/**
*	Runtime current version.
*/
#define RUNTIME_INFO "Ekwa runtime 0.2\n"
/**
*	Struct of error table.
*/
enum status_codes {
	VAR_AEXISTS	= 0x11,
	VAR_NAME	= 0x10,
	VAR_NVALUE	= 0x12,
	VAR_CMPTYPE	= 0x13,

	BUFFER_SIZE	= 0x09,
	BCODE_SIZE1	= 0x08,
	BCODE_SIZE2	= 0x07,
	ERROR_BCODE	= 0x06,
	ENVIROMENT	= 0x05,
	TOKEN_ARGS	= 0x04,
	VAR_SEARCH	= 0x03,
	FLAG_ERROR	= 0x02,
	BCODE_OPEN	= 0x10,
	MEM_ALLOC	= 0x01,
	SUCCESS		= 0x00
};
/**
*	Available instructions.
*/
enum tokens {
	EKWA_VAR	= 0x01, // New var.
	EKWA_BUFF	= 0x02, // Write to buffer.
	EKWA_ARG	= 0x03, // Argument for function.
	EKWA_CALL	= 0x04, // Call function from lib.
	EKWA_JMP	= 0x05, // Jump to flag.
	EKWA_FSET	= 0x06, // Set flag for jumping.
	EKWA_WRT	= 0x07, // Write from buffer to var.
	EKWA_IFE	= 0x08, // If args are equal.
	EKWA_IFNE	= 0x09, // If args aren't equal.
	EKWA_IFS	= 0x0a, // If arg is smoller.
	EKWA_IFB	= 0x0b, // If arg is bigger.
	EKWA_INFO	= 0x0c, // Info about VM.
	EKWA_SHOW	= 0x0d, // Print var value as a string.
	EKWA_RMV	= 0x0e, // Remove var.
	EKWA_VAL	= 0x0f, // Set value of var.
	EKWA_CAT	= 0x10, // Concatenation.
	EKWA_OPT	= 0x11, // Set/reset vm options.
	EKWA_EXIT	= 0x12, // Stop script.
	EKWA_PBUF	= 0x13, // Write pointer to buffer.
	EKWA_ARGL	= 0x14, // Argument for function as a link.
	EKWA_STD	= 0x15, // Call standard function.

	/* Arithmetic operations */
	EKWA_ADD	= 0x16,
	EKWA_SUB	= 0x17,
	EKWA_DIV	= 0x18,
	EKWA_MOD	= 0x19,
	EKWA_MUL	= 0x20,
	EKWA_SAL	= 0x21,
	EKWA_SAR	= 0x22,

	EKWA_END	= 0x23
};
/**
*	Available types of vars.
*/
enum var_types {
	EKWA_BYTES	= 0x00,
	EKWA_FLOAT	= 0x01,
	EKWA_INT	= 0x02
};
/**
*	Struct of instructions list.
*/
struct instruction {
	byte arg1[BUFFLEN], arg2[BUFFLEN];
	struct instruction *next;
	enum tokens token;
	u16_t len1, len2;
};
/**
*	Struct of all flags for jumping.
*/
struct flag {
	struct instruction *point;
	char name[BUFFLEN + 1];
	struct flag *next;
};
/**
*	Struct of all program's vars.
*/
struct var {
	char name[BUFFLEN + 1];
	enum var_types type;
	struct var *next;
	size_t length;
	void *ptr;
};

struct arg {
	enum var_types type;
	size_t length;
	void *ptr;
	bool link;
	struct arg *next;
};
/**
*	All global structs.
*/
struct instruction *ekwa_bcode;
struct flag *ekwa_flags;
struct var *ekwa_vars;
struct arg *ekwa_args;

void
ekwa_readbcode(FILE *, struct instruction **);

void
ekwa_freeinstructions(struct instruction **);

void
ekwa_startexec(struct instruction *);

void
ekwa_exit(unsigned char, char *);

void
ekwa_set_flags(struct instruction *);

void
ekwa_new_var(struct var);

bool
ekwa_var_exists(char *);

struct var *
ekwa_var_find(char *);

struct flag *
ekwa_find_flag(char *);

enum var_types
ekwa_detect_type(byte *);

void
ekwa_token_var(struct instruction *);

void
ekwa_token_setval(struct instruction *);

void
ekwa_token_wrt(struct instruction *,
				struct var *);

void
ekwa_token_buffer(struct instruction *,
				struct var *);

void
ekwa_token_buffptr(struct instruction *,
				struct var *);

void
ekwa_token_jump(struct instruction **);

void
ekwa_var_remove(char *);

void
ekwa_token_remove_var(struct instruction *);

void
ekwa_token_equal(struct instruction **, bool);

void
ekwa_token_ifsmall_big(struct instruction **,
					bool);

void
ekwa_token_show(struct instruction *);

void
ekwa_token_concat(struct instruction *,
				struct var *);

void
ekwa_token_sal_sar(struct instruction *, bool);

void
ekwa_token_call(struct instruction *,
				struct var *);

void
ekwa_args_add(struct arg);

void
ekwa_token_add_argument(struct instruction *);

void
ekwa_token_add_linkedarg(struct instruction *);

#endif