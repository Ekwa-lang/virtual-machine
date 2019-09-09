all:
	gcc ekwa.c parser.c tokens.c vm.c stdlib.c -o vm -ldl
	php compiler.php
