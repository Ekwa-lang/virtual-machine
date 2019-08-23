all:
	gcc ekwa.c parser.c tokens.c vm.c -o vm
	php compiler.php
