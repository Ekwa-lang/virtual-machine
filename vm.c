
#include "ekwa.h"

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
		case EKWA_SHOW:
			//ekwa_token_show();
			break;

		}

		c_list = c_list->next;
	}
}

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