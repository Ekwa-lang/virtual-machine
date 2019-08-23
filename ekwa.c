/**
*	ekwa.c - Start point for launching virtual
*	machine.
*/
#include "ekwa.h"
/*******************************************************/
void
ekwa_exit(unsigned char type)
{
#ifdef EKWA_DEBUG
	printf("Completion of execution.\n"
			"Error code: %x\n", type);
#endif
	exit(type);
}
/*******************************************************/
void
ekwa_setposition(FILE *fp)
{
	if (!fp) {
		ekwa_exit(BCODE_OPEN);
	}

	fseek(fp, 0,  SEEK_END);
	fseek(fp, -1, SEEK_CUR);
}
/*******************************************************/
int
main(int argc, char *args[])
{
	FILE *fp;

	if (argc == 0) {
		ekwa_exit(ENVIROMENT);
	}

	if (!(fp = fopen(args[0], "rb"))) {
		ekwa_exit(BCODE_OPEN);
	}

	ekwa_setposition(fp);
	ekwa_bcode = NULL;
	ekwa_flags = NULL;
	ekwa_vars  = NULL;

	ekwa_readbcode(fp, &ekwa_bcode);
	fclose(fp);

	ekwa_startexec(ekwa_bcode);
	ekwa_freeinstructions(&ekwa_bcode);
}