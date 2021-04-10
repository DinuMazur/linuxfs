// MAZUR DINU 313 CB
#include <stdio.h>

#ifndef _TEMA3
#define _TEMA3

typedef struct TNode {
	char c;
	struct TNode *pr;
	struct TNode *next;
	struct TNode *ch;
} TNode, *ANode;

void print_err(int my_errno, char *str);
ANode get_last_matching_node(ANode root, ANode cd, char *path_name, char **ch);
ANode nodealloc(char c);
int remove_back(ANode node);
int make_file(ANode root, ANode cd, int argc, char **argv,  char suffix);
void rem_files(ANode cd);
int remove_d(ANode root, ANode cd, int argc, char **argv, int mode);
int move_file(ANode root, ANode cd, int argc, char **argv, int remove);
void pwd(ANode root, ANode cd);
void pwd2(ANode cd);
int change_directory(ANode root, ANode *acd, int argc, char **argv);
void free_mem(ANode node);
void get_files(ANode cd, int f);
int list_directory(ANode root, ANode cd, int argc, char **argv);
int get_args(char *str, char ***aargv);
#endif