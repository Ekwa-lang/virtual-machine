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
/*******************************************************/
void
ekwa_token_jump(struct instruction **ptr)
{
	char name[BUFFLEN + 1];
	struct flag *c_flag;

	if (!(*ptr) || (*ptr) == NULL
		|| (*ptr)->len1 == 0) {
		ekwa_exit(TOKEN_ARGS);
	}

	memset(name, 0x00, BUFFLEN + 1);

	strncpy(name, (*ptr)->arg1, (*ptr)->len1);
	c_flag = ekwa_find_flag(name);

	if (c_flag == NULL) {
		return;
	}

	*ptr = c_flag->point;
}
/*******************************************************/
void
ekwa_token_remove_var(struct instruction *ptr)
{
	char name[BUFFLEN + 1];

	if (!ptr || ptr == NULL || ptr->len1 == 0) {
		ekwa_exit(TOKEN_ARGS);
	}

	memset(name, 0x00, BUFFLEN + 1);

	strncpy(name, ptr->arg1, ptr->len1);
	ekwa_var_remove(name);
}
/*******************************************************/
bool
ekwa_equal_int(struct var *v1, struct var *v2)
{
	int var_1, var_2;

	if (v1 == NULL || v2 == NULL) {
		ekwa_exit(VAR_SEARCH);
	}

	memcpy(&var_1, v1->ptr, sizeof(int));
	memcpy(&var_2, v2->ptr, sizeof(int));

	return var_1 == var_2;
}
/*******************************************************/
bool
ekwa_equal_float(struct var *v1, struct var *v2)
{
	float var_1, var_2;

	if (v1 == NULL || v2 == NULL) {
		ekwa_exit(VAR_SEARCH);
	}

	memcpy(&var_1, v1->ptr, sizeof(float));
	memcpy(&var_2, v2->ptr, sizeof(float));

	return var_1 == var_2;
}
/*******************************************************/
bool
ekwa_equal_bytes(struct var *v1, struct var *v2)
{
	char *var_1, *var_2;

	if (v1 == NULL || v2 == NULL) {
		ekwa_exit(VAR_SEARCH);
	}

	if (v1->length != v2->length) {
		return false;
	}

	var_1 = (char *)v1->ptr;
	var_2 = (char *)v2->ptr;

	return (strcmp(var_1, var_2) == 0) ? true : false;
}
/*******************************************************/
void
ekwa_token_equal(struct instruction **node, bool eq)
{
	char name1[BUFFLEN + 1], name2[BUFFLEN + 1];
	struct instruction *ptr = *node;
	struct var *v1, *v2;
	bool status;

	if (!ptr || ptr == NULL || ptr->len1 == 0
		|| ptr->len2 == 0) {
		ekwa_exit(TOKEN_ARGS);
	}

	memset(name1, 0x00, BUFFLEN + 1);
	memset(name2, 0x00, BUFFLEN + 1);

	strncpy(name1, ptr->arg1, ptr->len1);
	strncpy(name2, ptr->arg2, ptr->len2);

	v1 = ekwa_var_find(name1);
	v2 = ekwa_var_find(name2);

	if (v1 == NULL || v2 == NULL) {
		ekwa_exit(VAR_SEARCH);
	}

	if (v1->ptr == NULL || v2->ptr == NULL) {
		ekwa_exit(VAR_NVALUE);
	}

	if (v1->type != v2->type) {
		*node = (eq) ? (*node)->next : *node;
		return;
	}

	switch (v1->type) {
	case EKWA_INT:
		status = ekwa_equal_int(v1, v2);
		break;

	case EKWA_FLOAT:
		status = ekwa_equal_float(v1, v2);
		break;

	case EKWA_BYTES:
		status = ekwa_equal_bytes(v1, v2);
		break;
	}

	if (eq) {
		if (!status) {
			*node = (*node)->next;
			return;
		}

		return;
	}

	if (status) {
		*node = (*node)->next;
		return;
	}
}
/*******************************************************/
bool
ekwa_smaller_int(struct var *v1, struct var *v2)
{
	int var_1, var_2;

	if (v1 == NULL || v2 == NULL) {
		ekwa_exit(VAR_SEARCH);
	}

	memcpy(&var_1, v1->ptr, sizeof(int));
	memcpy(&var_2, v2->ptr, sizeof(int));

	return var_1 < var_2;
}
/*******************************************************/
bool
ekwa_smaller_float(struct var *v1, struct var *v2)
{
	float var_1, var_2;

	if (v1 == NULL || v2 == NULL) {
		ekwa_exit(VAR_SEARCH);
	}

	memcpy(&var_1, v1->ptr, sizeof(float));
	memcpy(&var_2, v2->ptr, sizeof(float));

	return var_1 < var_2;
}
/*******************************************************/
bool
ekwa_smaller_bytes(struct var *v1, struct var *v2)
{
	if (v1 == NULL || v2 == NULL) {
		ekwa_exit(VAR_SEARCH);
	}

	return v1->length < v2->length;
}
/*******************************************************/
void
ekwa_token_ifsmall_big(struct instruction **node,
					bool sm)
{
	char name1[BUFFLEN + 1], name2[BUFFLEN + 1];
	struct instruction *ptr = *node;
	struct var *v1, *v2;
	bool status;

	if (!ptr || ptr == NULL || ptr->len1 == 0
		|| ptr->len2 == 0) {
		ekwa_exit(TOKEN_ARGS);
	}

	memset(name1, 0x00, BUFFLEN + 1);
	memset(name2, 0x00, BUFFLEN + 1);

	strncpy(name1, ptr->arg1, ptr->len1);
	strncpy(name2, ptr->arg2, ptr->len2);

	v1 = ekwa_var_find(name1);
	v2 = ekwa_var_find(name2);

	if (v1 == NULL || v2 == NULL) {
		ekwa_exit(VAR_SEARCH);
	}

	if (v1->ptr == NULL || v2->ptr == NULL) {
		ekwa_exit(VAR_NVALUE);
	}

	if (v1->type != v2->type) {
		ekwa_exit(VAR_CMPTYPE);
		return;
	}

	switch (v1->type) {
	case EKWA_INT:
		status = ekwa_smaller_int(v1, v2);
		break;

	case EKWA_FLOAT:
		status = ekwa_smaller_float(v1, v2);
		break;

	case EKWA_BYTES:
		status = ekwa_smaller_bytes(v1, v2);
		break;
	}

	if (sm) {
		if (!status) {
			*node = (*node)->next;
			return;
		}

		return;
	}

	if (status) {
		*node = (*node)->next;
		return;
	}
}
/*******************************************************/
void
ekwa_print_intfloat(struct var *c_var)
{
	size_t size = sizeof(float);
	char buffer[BUFFLEN + 1];
	float v_float;
	int v_int;

	if (c_var == NULL) {
		ekwa_exit(VAR_SEARCH);
	}

	memset(buffer, 0x00, BUFFLEN + 1);

	if (c_var->type = EKWA_INT) {
		memcpy(&v_int, c_var->ptr, sizeof(int));
		sprintf(buffer, "%d", v_int);
	}
	else {
		memcpy(&v_float, c_var->ptr, size);
		sprintf(buffer, "%f", v_float);
	}

	printf("%s\n", buffer);
}
/*******************************************************/
void
ekwa_token_show(struct instruction *ptr)
{
	char name[BUFFLEN + 1], *buff;
	struct var *c_var;

	if (!ptr || ptr == NULL || ptr->len1 == 0) {
		ekwa_exit(TOKEN_ARGS);
	}

	memset(name, 0x00, BUFFLEN + 1);

	strncpy(name, ptr->arg1, ptr->len1);
	c_var = ekwa_var_find(name);

	if (c_var == NULL) {
		ekwa_exit(VAR_SEARCH);
	}

	if (c_var->length == 0) {
		return;
	}

	if (c_var->type != EKWA_BYTES) {
		ekwa_print_intfloat(c_var);
		return;
	}

	buff = (char *)malloc(c_var->length + 1);

	if (!buff) {
		ekwa_exit(MEM_ALLOC);
	}

	memset(buff, 0x00, c_var->length + 1);
	memcpy(buff, c_var->ptr, c_var->length);

	printf("%s\n", buff);
	free(buff);
}
/*******************************************************/
void
ekwa_token_concat(struct instruction *ptr,
				struct var *buff)
{
	char name1[BUFFLEN + 1], name2[BUFFLEN + 1];
	struct var *var_1, *var_2;
	size_t len;

	if (!ptr || ptr == NULL || ptr->len1 == 0
		|| ptr->len2 == 0 || !buff) {
		ekwa_exit(TOKEN_ARGS);
	}

	memset(name1, 0x00, BUFFLEN + 1);
	memset(name2, 0x00, BUFFLEN + 1);

	strncpy(name1, ptr->arg1, ptr->len1);
	strncpy(name2, ptr->arg2, ptr->len2);

	var_1 = ekwa_var_find(name1);
	var_2 = ekwa_var_find(name2);

	if (buff->ptr != NULL) {
		free(buff->ptr);
	}

	len = var_1->length + var_2->length;
	buff->ptr = malloc(len + 1);

	if (!buff->ptr) {
		ekwa_exit(MEM_ALLOC);
	}

	memset(buff->ptr, 0x00, len + 1);

	if (var_1 == NULL || var_2 == NULL) {
		ekwa_exit(VAR_SEARCH);
	}

	memcpy(buff->ptr, var_1->ptr, var_1->length);
	memcpy(buff->ptr + var_1->length, var_2->ptr,
			var_2->length);

	buff->length = len;
}
