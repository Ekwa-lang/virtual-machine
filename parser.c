/**
*	parser.c - Function of byte code parsing from binary
*	sector.
*/
#include "ekwa.h"
/*******************************************************/
void
ekwa_addinstructin(struct instruction **list,
					struct instruction new)
{
	size_t size = sizeof(struct instruction);
	struct instruction *one, *ptr = (*list);

	one = (struct instruction *)malloc(size);

	if (!one) {
		ekwa_exit(MEM_ALLOC, "addinstructin");
	}

	memcpy(one, &new, size);
	one->next = NULL;

	if ((*list) == NULL || !(*list)) {
		*list = one;
		return;
	}

	while (ptr->next && ptr->next != NULL) {
		ptr = ptr->next;
	}

	ptr->next = one;
}
/*******************************************************/
void
ekwa_freeinstructions(struct instruction **list)
{
	struct instruction *ptr = *list, *next;

	if (!list || list == NULL) {
		return;
	}

	while (ptr && ptr != NULL) {
		next = ptr->next;
		free(ptr);

		ptr = next;
	}

	*list = NULL;
}
/*******************************************************/
void
ekwa_readbcode(FILE *fp, struct instruction **list)
{
	size_t size = sizeof(struct instruction);
	struct instruction line;
	u16_t len = 0;
	byte token;

	if (!fp || (*list) != NULL) {
		ekwa_exit(ERROR_BCODE, "readbcode");
	}

	while (true) {
		fread(&token, sizeof(byte), 1, fp);
		memset(&line, 0x00, size);

		if (token == 0x00 || token >= EKWA_END) {
			return;
		}

		line.token = token;

		fseek(fp, -3L, SEEK_CUR);
		fread(&len, sizeof(u16_t), 1, fp);

		len >>= 8; // It will be removed.
		line.len1 = len;

		if (len > BUFFLEN) {
			ekwa_exit(BCODE_SIZE1, "readbcode");
		}

		if (len == 0x00) {
			ekwa_addinstructin(list, line);
			fseek(fp, -3L, SEEK_CUR);
			continue;
		}

		fseek(fp, -2L - len, SEEK_CUR);
		fread(&line.arg1, 1, len, fp);

		fseek(fp, -1 * len - 2, SEEK_CUR);
		fread(&len, sizeof(u16_t), 1, fp);

		len >>= 8; // It will be removed.
		line.len2 = len;

		if (len > BUFFLEN) {
			ekwa_exit(BCODE_SIZE2, "readbcode");
		}
		
		if (len == 0x00) {
			ekwa_addinstructin(list, line);
			fseek(fp, -3L, SEEK_CUR);
			continue;
		}

		fseek(fp, -2L - len, SEEK_CUR);
		fread(&line.arg2, 1, len, fp);
		fseek(fp, -1 * len - 1, SEEK_CUR);

		ekwa_addinstructin(list, line);
	}
}