/**
*	vm.c - Base functions of Ekwa virtual machine declared
*	here.
*/
#include "ekwa.h"
/*******************************************************/
void
ekwa_startexec(struct instruction *list)
{
	struct instruction *c_list = list;

	if (!list || list == NULL) {
		ekwa_exit(ERROR_BCODE);
	}

	ekwa_set_flags(list);

	while (c_list && c_list != NULL) {
		switch (c_list->token) {
		case EKWA_VAR:
			ekwa_token_var(c_list);
			break;

		case EKWA_SHOW:
			//ekwa_token_show();
			break;

		}

		c_list = c_list->next;
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

	if (ekwa_var_exists(new.name))
		ekwa_exit(VAR_AEXISTS);

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

		new->point = ptr->next;
		new->next = ekwa_flags;
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
	if (buffer[0] > EKWA_PTR) {
		return EKWA_BYTES;
	}

	return (enum var_types)buffer[0];
}