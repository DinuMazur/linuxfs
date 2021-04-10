// MAZUR DINU 313 CB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema3.h"

// functia pentru afisarea erorilor
void print_err(int my_errno, char *str) {
	if (my_errno == -1) {
		fprintf(stderr, "Error\n");
	} else if (my_errno == 0) {
		fprintf(stderr, "%s: command not found\n",str);
	} else if (my_errno == 1) {
		fprintf(stderr, "%s: the syntax of the command is incorrect\n",str);
	} else if (my_errno == 2) {
		fprintf(stderr, "%s: missing operand\n",str);
	} else if (my_errno == 3) {
		fprintf(stderr, "%s: too many arguments\n",str);
	} else if (my_errno == 4) {
		fprintf(stderr, "%s: Directory not empty\n",str);
	} else if (my_errno == 5) {
		fprintf(stderr, "%s: Not a directory\n",str);
	} else if (my_errno == 6) {
		fprintf(stderr, "%s: already exists\n",str);
	} else if (my_errno == 7) {
		fprintf(stderr, "%s: No such file or directory\n", str);
	}
}

//functia returneaza ultimul nod care corespunde cu calea si un pointer
// catre calea ramasa
ANode get_last_matching_node(ANode root, ANode cd, char *path_name, char **ch) {
	
	ANode node;
	int is_d_name = 0;	// variabila care arata daca la moment avem un nume de
						// fisier
	int count = 0;	// variabila care pastreaza numarul de puncte intalnite in
					// cale cuprins intre doua '/'
	
	if (*path_name == '/') { // daca primul caracter din cale este / atunci
								//returnam root-ul
		cd = root;// setam directorul curent sa fie in root
		path_name++;
		*ch = path_name; //scriem in pointer restul caii
	}
	while (cd && *path_name != '\0') { // pentru fiecare caracter din cale
		*ch = path_name;
		if (*path_name == '/' && count > 0) { // daca intalnim / e moment de
												// resetare a variabilelor
			is_d_name = 0;
			count = 0;
		} else {
			if (*path_name == '.' && !is_d_name) { // numaram punctele 
					// pentru a fi siguri daca plecam in urma
				if (count == 1) {
					if (cd == root) {
						// daca ne aflam in root si incercam sa mergem in urma
						// returnam mesaul de eroare NULL
						return NULL;
					} else {
						// muta directorul curent cu un pas in urma
						while (cd->pr->c != '/') {
							cd = cd->pr;
						}
						cd = cd->pr;
					}
				}
				count++;
			} else {
				// daca caracterul curent nu este punct sau avem un nume de
				// fisier sau director
				node = cd->ch;
				//verificam daca caraterul curent exista intre noduri
				while (node && *path_name != node->c) {
					node = node->next;
				}
				if (node) { // daca exista acest nod atunci setam nodul curent
					cd = node;
				} else {
					//in caz cotrar trebuie de verificat daca nu mai
					// sunt / in cale
					while (*path_name != '/' && *path_name != '\0') {
						path_name++;
					}
					if (*path_name == '\0') {
						//daca nu mai sunt returneaza nodul curent
						return cd;
					} else {
						// caz contrar , mesaj de eroare
						return NULL;
					}
				}
				// daca se ajunge aici se stie ca caracterul nu e . deci
				// nu e o cale relativa dar este un nume de fisier
				is_d_name = 1;
				count = 0;
			}
		}		
		// iteram prin caractere
		path_name++;
	}
	*ch = path_name;
	return cd;
}

// alocare de un nod cu caracterul corespunzator
ANode nodealloc(char c) {
	ANode node = malloc(sizeof(TNode));
	if (node) {
		node->c = c;
		node->pr = NULL;
		node->next = NULL;
		node->ch = NULL;
	}
	return node;
}

// stergerea direcoarelor si fisierelor in oridine inversa de la capat pana
// la nodul directorului curent
int remove_back(ANode node) {
	ANode node_next;
	ANode node_pr;
	int first_node = 1; // caz particular cand primul nod tot este /
	
	while (node->c != '/' || first_node) {
		first_node = 0;
		node_pr = node->pr;
		if (node_pr->ch != node || node->next) { // verificam daca nodul are
												// vecini
			if (node_pr->ch == node) { // daca nodul curent este copilul parin.
				node_pr->ch = node->next; // stabilim legaturi noi
			} else { // nodul curent nu este copilul direct al parintelui sau
				node_next = node_pr->ch;
				// cautam prin vecini nodul care arata spre nodul curent
				while (node_next->next != node) {
					node_next = node_next->next;
				}
				node_next->next = node->next;
			}
			free(node); // eliberam nodul
			break; // mai departe nu se poate de sters deoarece nodurile
			//in continuare intra in componenta numelor altor fisiere si dir.
		}
		// nu are vecini deci se poate de eliberat fara complicatii
		// si de continuat in aceeasi manera
		node_pr->ch = NULL;
		free(node);
		node = node_pr;
	}
	return 0;
}


//functia care creaza si fisiere si directoare
int make_file(ANode root, ANode cd, int argc, char **argv,  char suffix) {
	ANode node;
	ANode node_aux;
	ANode node_prev;
	ANode node_ch;
	int i;
	char *d_name;
	
	if (argc < 2) {
		print_err(2, argv[0]);
		return 1;
	}
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			node = get_last_matching_node(root, cd, argv[i], &d_name);
			node_ch = NULL;
			if (node) { // daca nu a fost returant nici un nod atunci calea
						// nu este corecta
				if (*d_name == '\0') { //daca nu mai exista caract. pentru cale
					node_ch = node->ch;
					//verificam daca nodul urmator contine caracterul suffix
					while (node_ch && node_ch->c != suffix) {
						node_ch = node_ch->next;
					}
				}
				if (node_ch) {
					//daca nu a fost gasit caracterul afisam messajul de eroare
					print_err(6, argv[i]);
				} else {
					// alocarea lantului de noduri care respecta structura de
					// trie
					while (*d_name != '\0' || suffix != '\0') {
						if (*d_name == '\0') {
							node_aux = nodealloc(suffix);
						} else {
							node_aux = nodealloc(*d_name);
						}
						if (!node_aux) {
							return 1;
						}
						node_aux->pr = node;
						node_prev = NULL;
						node_ch = node->ch;
						while (node_ch && node_ch->c < *d_name) {
							node_prev = node_ch;
							node_ch = node_ch->next;
						}
						if (node_prev) {
							node_aux->next = node_prev->next;
							node_prev->next = node_aux;
						} else {
							node_aux->next = node->ch;
							node->ch = node_aux;
						}
						if (*d_name != '\0') {
							d_name++;
							node = node_aux;
						} else {
							break;
						}
					}
				}
			} else {
				print_err(7, argv[i]);
			}
		} else {
			print_err(1, argv[0]);
			return 1;
		}
	}
	return 0;
}

// stergera fisierelor si directoarelor din directorul curent
void rem_files(ANode cd) {
	while (cd) {
		if (cd->c == '*' || cd->c == '/') {
			remove_back(cd);
		} else {
			rem_files(cd->ch);
		}
		cd = cd->next;
	}
}

//functia pentru rmdir si rm.
int remove_d(ANode root, ANode cd, int argc, char **argv, int mode) {
	// mode = [ 0 - rm; 1 - rmdir]
	ANode node;
	ANode node_ch;
	char *d_name;
	int i;
	if (argc == 1) {
		return 1;
	}
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			node = get_last_matching_node(root, cd, argv[i], &d_name);
			if (node && *d_name == '\0') { // daca calea e completa
				node_ch = node->ch;
				// verificam daca fisierul sau directorul din cale
				// este chair fisier sau director
				while (node_ch && ((mode && node_ch->c != '/') || (!mode &&
						node_ch->c != '/' && node->ch->c != '*'))) {
					node_ch = node_ch->next;
				}
				if (node_ch) { // daca fisierul din cale este fisier sau dir.
					if (mode) {// daca este modul rmdir
						if (node_ch->ch) {
							// daca nodul mai are copii atunci directorul
							// curent nu e gol
							print_err(4, argv[i]);
						} else {
							// sterge directorul curent
							remove_back(node_ch);
						}
					} else { // rm
						if (node_ch->c == '*') { // daca e fisier
							remove_back(node_ch);
						} else { // daca e director
							rem_files(node_ch->ch); // stergem elem. din inter.
							remove_back(node_ch); // stergem directorul curent
						}
					}
				} else {
					print_err(7, argv[i]);
				}
			} else {
				print_err(7, argv[i]);
			}
		} else {
			print_err(1, argv[i]);
		}
	}
	return 0;
}

// functia pentru cp si mv
int move_file(ANode root, ANode cd, int argc, char **argv, int remove) {
	// remove este variabila care spune daca trebuie de sters sau nu
	// fisierul copiat pentru mv - da pentru cp - nu
	ANode node;
	ANode node_ch;
	ANode src = NULL;
	char str[64];
	char *d_name;
	char *argv2[2];
	int i;
	
	if (argc == 1) {
		print_err(2, argv[0]);
		return 1;
	} else if (argc == 2) {
		strcpy(str, argv[0]);
		strcat(str, " ");
		strcat(str, argv[1]);
		print_err(2, str);
		return 1;
	}
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			node = get_last_matching_node(root, cd, argv[i], &d_name);
			if (node && *d_name == '\0') {
				if (!src) { // daca sursa nu a fost setata
					node_ch = node->ch;
				} else { // setam destinatia
					if (node->c == '/') {
						node_ch = node;
					} else {
						node_ch = node->ch;
					}
				} // verificam daca fisierul sau/si destinatia este valida
				while (node_ch && ((!src && node_ch->c != '*') ||
						(src && node_ch->c != '/'))) {
					node_ch = node_ch->next;
				}	
				if (node_ch) { // daca fisiereul sau/si destinatia este valida
					if (!src) { // setam nodul sursa
						src = node_ch;
					} else {
						if (remove) { // daca e functia mv
							remove_back(src); // stergem fisierul curent
						}
						d_name = argv[1];
						while (argv[1][i] != '\0') {
							if (argv[1][i] == '/') {
								d_name = &argv[1][i + 1];
							}
							i++;
						}
						strcpy(str, argv[2]);
						if (argv[2][1] != '\0') {
							strcat(str, "/");
						}
						strcat(str, d_name);
						argv2[1] = str;
						make_file(root, cd, 2, argv2, '*'); // creeam fisierul
					}
				} else {
					if (src) {
						print_err(5, argv[i]);
					} else {
						print_err(7, argv[i]);
					}
					return 1;
				}
			} else {
				print_err(7, argv[i]);
				return 1;
			}
		} else {
			print_err(1, argv[i]);
		}
	}
	return 0;
}

//functia care afiseaza directorul curent in mod recursiv
void pwd(ANode root, ANode cd) {
	if (cd != root) {
		pwd(root, cd->pr);
	}
	printf("%c", cd->c);
}

// un caz particular pentru pwd in care avem nevoie sa se afiseze pana
// la nodul primit ca parametru (de la copii spre parinte)
void pwd2(ANode cd) {
	if (cd) {
		if (cd->pr && cd->pr->c != '/') {
			pwd2(cd->pr);
		}
		printf("%c", cd->c);
	}
}

// functia pentru schimbarea directorului
int change_directory(ANode root, ANode *acd, int argc, char **argv) {
	ANode node;
	ANode node_ch;
	char *d_name;
	int i;
	
	if (argc != 2) {
		print_err(2, argv[0]);
		return 1;
	}
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			node = get_last_matching_node(root, *acd, argv[i], &d_name);
			if (node && *d_name == '\0') {
				if (node->c == '/') {
					*acd = node;
				} else {
					node_ch = node->ch;
					while (node_ch && node_ch->c != '/') {
						node_ch = node_ch->next;
					}
					if (node_ch) { // daca directorul este valid
						*acd = node_ch;
					} else {
						print_err(5, argv[i]);
						return 1;
					}
				}				
			} else {
				print_err(7, argv[i]);
				return 1;
			}
		} else {
			print_err(1, argv[i]);
		}
	}
	return 0;
}

// eliberarea memoriei
void free_mem(ANode node) {
	
	ANode node_next;
	while (node) {
		free_mem(node->ch);
		node_next = node->next;
		free(node);
		node = node_next;
	}
}

// functia care ajuta sa gasim toate fisierele din directorul curent
// si apoi sa le afisam
void get_files(ANode cd, int f) {
	while (cd) {
		if (cd->c == '*' || cd->c == '/') {
			if (f) {
				pwd2(cd);
			} else {
				pwd2(cd->pr);
			}
			printf(" ");
		} else {
			get_files(cd->ch, f);
		}
		cd = cd->next;
	}
}


// afiseaza fisierele si directoarele din directorul curent
int list_directory(ANode root, ANode cd, int argc, char **argv) {
	ANode node = cd;
	ANode node_ch;
	char *d_name;
	int f = 0;
	char str[64];
	int i;
	
	if (argc == 1) {
		get_files(node->ch, 0);
		printf("\n");
		return 1;
	} else if (argc == 2 && strcmp(argv[1], "-F") == 0) {
		get_files(node->ch, 1);
		printf("\n");
		return 1;
	} else if (argc > 3) {
		strcpy(str, argv[0]);
		for (i = 1; i < argc; i++) {
			strcat(str, " ");
			strcat(str, argv[i]);
		}
		print_err(3, str);
		return 1;
	} else {
		for (i = 1; i < argc; i++) {
			if (argv[i][0] == '-' && strcmp(argv[i], "-F") == 0) {
				// daca argumentul e -F atunci fisierele vor fi afisate in
				// alt mod
				f = 1;
			} else if (argv[i][0] == '-') {
				print_err(2, argv[0]);
			} else {
				node = get_last_matching_node(root, cd, argv[i], &d_name);
				if (node && *d_name == '\0') {
					if (node->c != '/') {
						node_ch = node->ch;
						while (node_ch && node_ch->c != '/') {
							node_ch = node_ch->next;
						}
						if (node_ch) { //daca nodul e valid
							get_files(node_ch->ch, f);
							printf("\n");
						} else {
							print_err(7, argv[i]);
						}
					} else { // daca nodul e valid doar ca nodul initial deja
					// avea caracterul /
						get_files(node->ch, f);
						printf("\n");
					}
				} else {
					print_err(7, argv[i]);
				}
			}
		}
	}
	return 0;
}

// functia care returneaza o lista cu pointeri spre argumentele extrase din
// stringul primit ca parametru
int get_args(char *str, char ***aargv) {
	int argc = 0;
	char **argv = NULL;
	char **aux;
	char *pch;
	pch = strtok (str, " \n");
	while (pch) {
		aux = (char **)realloc(argv, (argc + 1) * sizeof(char*));
		if (!aux) { // return whatever has been saved before
			*aargv = argv;
			return argc;
		}
		argv = aux;
		*(argv + argc) = pch;
		argc++;
		pch = strtok (NULL, " \n");
	}
	*aargv = argv;
	return argc;
}