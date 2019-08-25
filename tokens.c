/**
*	tokens.c - Here declared functions which will be called
*	by current byte code.
*/
#include "ekwa.h"
/*******************************************************/
void
ekwa_token_var(struct instruction *ptr)
{
	struct var new;

	if (!ptr || ptr == NULL || ptr->len1 == 0
		|| ptr->len2 != 1) {
		ekwa_exit(TOKEN_ARGS);
	}

	memset(new.name, 0x00, BUFFLEN + 1);

	strncpy(new.name, ptr->arg1, ptr->len1);
	new.type = ekwa_detect_type(ptr->arg2);
	new.ptr = NULL;

	ekwa_new_var(new);
}
/*******************************************************/
void
ekwa_token_setval(struct instruction *ptr)
{
	char name[BUFFLEN + 1];
	struct var *c_var;

	if (!ptr || ptr == NULL || ptr->len1 == 0
		|| ptr->len2 == 0) {
		ekwa_exit(TOKEN_ARGS);
	}

	memset(name, 0x00, BUFFLEN + 1);

	strncpy(name, ptr->arg1, ptr->len1);
	c_var = ekwa_var_find(name);

	if (c_var == NULL) {
		ekwa_exit(VAR_SEARCH);
	}

	if (c_var->ptr != NULL) {
		free(c_var->ptr);
	}

	if (!(c_var->ptr = malloc(ptr->len2 + 1))) {
		ekwa_exit(MEM_ALLOC);
	}

	memset(c_var->ptr, 0x00, ptr->len2  + 1);
	memcpy(c_var->ptr, ptr->arg2, ptr->len2);
	c_var->length = ptr->len2;
}
/*******************************************************/
void
ekwa_token_buffer(struct instruction *ptr,
				struct var *buff)
{
	char name[BUFFLEN + 1];
	struct var *c_var;

	if (!ptr || ptr == NULL || ptr->len1 == 0
		|| !buff) {
		ekwa_exit(TOKEN_ARGS);
	}

	memset(name, 0x00, BUFFLEN + 1);

	strncpy(name, ptr->arg1, ptr->len1);
	c_var = ekwa_var_find(name);

	if (c_var == NULL) {
		ekwa_exit(VAR_SEARCH);
	}

	if (buff->ptr != NULL) {
		free(buff->ptr);
	}

	if (!(buff->ptr = malloc(c_var->length + 1))) {
		ekwa_exit(MEM_ALLOC);
	}

	memcpy(buff->ptr, c_var->ptr, c_var->length);
	buff->length = c_var->length;
}
/*******************************************************/
void
ekwa_token_wrt(struct instruction *ptr,
				struct var *buff)
{
	char name[BUFFLEN + 1];
	struct var *c_var;

	if (!ptr || ptr == NULL || ptr->len1 == 0
		|| !buff) {
		ekwa_exit(TOKEN_ARGS);
	}

	memset(name, 0x00, BUFFLEN + 1);

	strncpy(name, ptr->arg1, ptr->len1);
	c_var = ekwa_var_find(name);

	if (c_var == NULL) {
		ekwa_exit(VAR_SEARCH);
	}

	if (c_var->ptr != NULL) {
		free(c_var->ptr);
	}

	c_var->length = buff->length;
	c_var->ptr = buff->ptr;

	buff->length = 0;
	buff->ptr = NULL;
}
/*******************************************************/
void
ekwa_token_buffptr(struct instruction *ptr,
				struct var *buff)
{
	char name[BUFFLEN + 1];
	struct var *c_var;

	if (!ptr || ptr == NULL || ptr->len1 == 0
		|| !buff) {
		ekwa_exit(TOKEN_ARGS);
	}

	memset(name, 0x00, BUFFLEN + 1);

	strncpy(name, ptr->arg1, ptr->len1);
	c_var = ekwa_var_find(name);

	if (c_var == NULL) {
		ekwa_exit(VAR_SEARCH);
	}

	buff->length = c_var->length;
	buff->ptr = c_var->ptr;
}