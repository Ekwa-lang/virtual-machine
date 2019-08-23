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
		|| ptr->len2 == 0) {
		ekwa_exit(TOKEN_ARGS);
	}

	strncpy(new.name, ptr->arg1, ptr->len1);
	new.type = ekwa_detect_type(ptr->arg2);
	new.ptr = NULL;

	ekwa_new_var(new);
}