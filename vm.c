/**
*	vm.c - Base functions of Ekwa virtual machine declared
*	here.
*/
#include "ekwa.h"
/*******************************************************/
void
ekwa_startexec(struct instruction *list)
{
	struct instruction *node = list;
	struct var buff;

	if (!list || list == NULL) {
		ekwa_exit(ERROR_BCODE);
	}

	ekwa_set_flags(list);
	buff.ptr = NULL;

	while (node && node != NULL) {
		switch (node->token) {
		case EKWA_BUFF:
			ekwa_token_buffer(node, &buff);
			break;

		case EKWA_VAR:
			ekwa_token_var(node);
			break;

		case EKWA_VAL:
			ekwa_token_setval(node);
			break;

		case EKWA_WRT:
			ekwa_token_wrt(node, &buff);
			break;

		case EKWA_PBUF:
			ekwa_token_buffptr(node, &buff);
			break;

		case EKWA_JMP:
			ekwa_token_jump(&node);
			break;

		case EKWA_RMV:
			ekwa_token_remove_var(node);
			break;

		case EKWA_IFB:
			ekwa_token_ifsmall_big(&node, false);
			break;

		case EKWA_IFS:
			ekwa_token_ifsmall_big(&node, true);
			break;

		case EKWA_IFE:
			ekwa_token_equal(&node, true);
			break;

		case EKWA_IFNE:
			ekwa_token_equal(&node, false);
			break;

		case EKWA_SHOW:
			ekwa_token_show(node);
			break;

		case EKWA_CAT:
			ekwa_token_concat(node, &buff);
			break;

		case EKWA_INFO:
			printf(RUNTIME_INFO);
			break;

		case EKWA_EXIT:
			ekwa_exit(SUCCESS);
		}

		node = node->next;
	}
}
/*******************************************************/
struct var *
ekwa_var_find(char *name)
{
	struct var *ptr = ekwa_vars;

	if (!name || strlen(name) == 0) {
		ekwa_exit(VAR_NAME);
	}

	while (ptr && ptr != NULL) {
		if (strcmp(ptr->name, name) != 0) {
			ptr = ptr->next;
			continue;
		}

		return ptr;
	}

	return NULL;
}
/*******************************************************/
bool
ekwa_var_exists(char *name)
{
	struct var *ptr = ekwa_vars;
	bool status = false;

	if (!name || strlen(name) == 0) {
		ekwa_exit(VAR_NAME);
	}

	while (ptr && ptr != NULL) {
		if (strcmp(ptr->name, name) != 0) {
			ptr = ptr->next;
			continue;
		}

		status = true;
		break;
	}

	return status;
}
/*******************************************************/
void
ekwa_new_var(struct var new)
{
	size_t size = sizeof(struct var);
	struct var *one;

	if (ekwa_var_exists(new.name)) {
		ekwa_exit(VAR_AEXISTS);
	}

	one = (struct var *)malloc(size);
	memcpy(one, &new, size);

	one->next = ekwa_vars;
	ekwa_vars = one;
}
/*******************************************************/
void
ekwa_set_flags(struct instruction *list)
{
	size_t size = sizeof(struct flag);
	struct instruction *ptr = list;
	struct flag *new;

	while (ptr && ptr != NULL) {
		if (ptr->token != EKWA_FSET) {
			ptr = ptr->next;
			continue;
		}

		if (ptr->len1 == 0) {
			ekwa_exit(TOKEN_ARGS);
		}

		new = (struct flag *)malloc(size);

		if (!new) {
			ekwa_exit(MEM_ALLOC);
		}

		strncpy(new->name, ptr->arg1,
				ptr->len1);

		new->next = ekwa_flags;
		new->point = ptr;
		ekwa_flags = new;

		ptr = ptr->next;
	}
}
/*******************************************************/
struct flag *
ekwa_find_flag(char *name)
{
	struct flag *ptr = ekwa_flags;

	if (!name || strlen(name) == 0) {
		return NULL;
	}

	while (ptr && ptr != NULL) {
		if (strcmp(name, ptr->name) != 0) {
			ptr = ptr->next;
			continue;
		}

		return ptr;
	}

	return NULL;
}
/*******************************************************/
enum var_types
ekwa_detect_type(byte *buffer)
{
	if (!buffer || buffer[0] > EKWA_INT) {
		return EKWA_BYTES;
	}

	return (enum var_types)buffer[0];
}
/*******************************************************/
void
ekwa_var_remove(char *name)
{
	struct var *ptr = ekwa_vars, *buff;

	if (!name || name == NULL) {
		ekwa_exit(VAR_NAME);
	}

	if (!ekwa_vars || ekwa_vars == NULL) {
		return;
	}

	if (strcmp(name, ptr->name) == 0) {
		buff = ekwa_vars;
		ekwa_vars = ekwa_vars->next;

		free(buff);
		return;
	}

	while (ptr->next && ptr->next != NULL) {
		if (strcmp(ptr->next->name, name) != 0) {
			ptr = ptr->next;
			continue;
		}

		buff = ptr->next;
		ptr->next = ptr->next->next;

		free(buff);
		return;
	}

	ekwa_exit(VAR_NAME);
}