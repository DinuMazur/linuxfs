// MAZUR DINU 313 CB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema3.h"

int main() {
	
	ANode root, cd;
	char str[64];
	char **argv;
	int argc;
	int count;
	
	cd = root = nodealloc('/');
	if (!cd) {
		return 1;
	}
	root->c = '/';
	root->pr = NULL;
	root->next = NULL;
	root->ch = NULL;
	
	scanf("%d", &count);
	while (count >= 0) {
		fgets(str, 64, stdin); // citirea unei linii
		argc = get_args(str, &argv); // extragerea argumentelor din string
		if (argv) { //alegerea functiei corespunzatoare dupa primul argument
			if (strcmp(argv[0], "mkdir") == 0) {
				make_file(root, cd, argc, argv, '/');
			} else if (strcmp(argv[0], "touch") == 0) {
				make_file(root, cd, argc, argv, '*');
			} else if (strcmp(argv[0], "ls") == 0) {
				list_directory(root, cd, argc, argv);
			} else if (strcmp(argv[0], "cd") == 0) {
				change_directory(root, &cd, argc, argv);
			} else if (strcmp(argv[0], "mv") == 0) {
				move_file(root, cd, argc, argv, 1);
			} else if (strcmp(argv[0], "cp") == 0) {
				move_file(root, cd, argc, argv, 0);
			} else if (strcmp(argv[0], "rmdir") == 0) {
				remove_d(root, cd, argc, argv, 1);
			} else if (strcmp(argv[0], "rm") == 0) {
				remove_d(root, cd, argc, argv, 0);
			} else if (strcmp(argv[0], "pwd") == 0) {
				if (argc == 1) {
					if (cd != root) {
						pwd(root, cd->pr);
					} else {
						printf("/");
					}
					printf("\n");
				} else {
					print_err(2, argv[0]);
				}
			} else {
				print_err(0, argv[0]);
			}
			free(argv); // eliberearea listei cu pointere
		}
		count--;
	}
	free_mem(root); //eliberearea memoriei (triei)
	return 0;
}