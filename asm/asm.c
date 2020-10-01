/*Authors   Adi Polak and Or Lopata.
Project – Assembler
Description-The program derives the assembler code we wrote from the "program.txt" and creates a text file called "memin.txt" containing 
a picture of the memory. 
argv inputs- "asm.exe program.asm mem.txt" - program is filled with one of the 3 programs -pascal, div, bubble
*/
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct label
{
	char label_name[50];
	int index;
	struct label * next;
}label;
typedef struct wordstruct
{
	int dest;
	int num;
	struct wordstruct * next;
}wordstruct;

struct wordstruct *sortwordbyindex(struct wordstruct*  wordi);
void print_w(char * line, int compare, int place, char *bitsline);
void add_words_to_print(FILE *memory, int printed, struct wordstruct* wordi);
int final_print(char *bitsline, FILE* memory, int imm, char *line, struct label* first);
void swap(struct wordstruct* a, struct wordstruct* b);
void decimal_to_hex(char* hex, int decimal, int neg);
void make_word(char* line, struct wordstruct* last, int *wor);
void free_word(struct  wordstruct* wordi);
void free_label(struct  label* first);
void free_line(char **line);
int my_strcmp(char *line, char *(opps[]), char *(registers[]), int place, int line_place);
int check_label(char *line, int a, struct label* first, char* bitsline);
int str_cmp(char * arr, char * opps);
int check_num(char *line, struct label* first);
int check_double_dest(int dest, int num, struct wordstruct* wordi);


int main(int argc, char * argv[])
{

	int i = 0, check_double = 0, neg = 0, new_lines = 0, gg = 0, wordsnum = 0, checkw = 0, h = 0, labels_num = 0, imm_check = 0, m = 0, skip = 0, lab = -1, printed = 0, j = 0, t = 0, e = 0, num_of_lines = 0, max_chars_in_line = 100, a = 0, compare = -1, line_num = 0, immid_commands = 0, place = 0, line_place = 0;
	int mem[100];			//array of final memory
	int wor[2];
	char** line;		//a matrix for each line
	char * EE;
	char temp[100];
	char word[100];				//array for next word
	char bitsline[17];			//line in memory in bits;
	struct wordstruct* wordi = NULL;
	struct wordstruct* wordf = NULL;
	struct wordstruct* tempword = NULL;
	struct label* last = NULL;			// Labels.		
	struct label* first = NULL;
	FILE *assembly;   		//open assembly file
	char *opps[] = { "add","sub","and", "or", "sll", "sra","reserved ","beq", "bgt", "ble","bne","jal","lw", "sw", "lhi","halt" };				//OPS NAMES
	char *registers[] = { "zero","imm","v0","a0","a1","t0","t1","t2","t3","s0","s1","s2","gp","sp","fp","ra" }; //registers names
	FILE *memory;	//open assembly file
	memory =fopen( argv[2], "w");
	assembly = fopen(argv[1], "r");
	line = (char**)(malloc(sizeof(char*) * 500));
	line[0] = (char*)(malloc(sizeof(char) * 500));
	first = (label *)(malloc(sizeof(label)));		// initiating first lable	
	if ((first == NULL) || (line == NULL) || (line[0] == NULL))
	{
		printf("Error! memory not allocated.");
		exit(0);
	}
	last = first;
	last->next = NULL;
	if (&assembly == NULL)
	{
		perror("Error opening file");
		return(-1);
	}
	j = 0, i = 0;
	while (1<2)  //switching the \n in with \0 fgets +reading to an array
	{
		if (feof(assembly))
			break;
		EE = fgets(temp, 1000, assembly);
		if (EE == NULL)
			break;
		if (temp == NULL)
			break;
		skip = strlen(temp);
		if (skip<3)
		{
			i++;
			continue;
		}
		temp[strlen(temp)] = '\0';
		if (temp[0] == '\n')
			continue;
		line[j] = (char*)(malloc(sizeof(char) * 100));
		if (line[j] == NULL)
		{
			printf("Error! memory not allocated.");
			exit(0);
		}
		strcpy(line[j], temp);

		i++;
		j++;


	}
	line[j] = "END";
	num_of_lines = j;
	j = 0;
	skip = 0;
	for (i = 0; i<num_of_lines; i++)				//	 deleting comments+finding labels names and indexes
	{

		a = 0;
		if (strlen(line[i])<1)
			continue;
		for (j = 0; j<max_chars_in_line; j++)
		{
			if ((line[i][j] == '.') && (line[i][j + 1] == 'w') && (line[i][j + 2] == 'o') && (line[i][j + 3] == 'r') && (line[i][j + 4] == 'd'))					// marking words
			{
				wordsnum++;
				make_word(line[i], wordf, wor);
				if (h != 0)
				{
					check_double = check_double_dest(wor[1], wor[0], wordi);
					if (check_double == 1)
						check_double = 1;
				}
				if ((check_double == 0) || (h == 0))
				{
					tempword = (struct wordstruct*)(malloc)(sizeof(wordstruct));
					if (tempword == NULL)
					{
						printf("Error! memory not allocated.");
						exit(0);
					}

					tempword->num = wor[0];
					tempword->dest = wor[1] + 2;
					tempword->next = NULL;
				}

				if ((h == 1) && (check_double == 0))
				{
					wordf->next = tempword;
					wordf = wordf->next;
				}
				if ((h == 0) && (check_double == 0))
				{
					h = 1;
					wordf = tempword;
					wordi = wordf;
				}
				check_double = 0;
				tempword = NULL;
				break;
			}
			if (line[i][j] == '#')				// deleting comments
			{
				a = 1;
			}
			if (a == 1)
				line[i][j] = '\0';


			if (line[i][j] == ':')							// marking lables
			{
				if (line[i][0] == 'a')
					(line[i][0] = 'a');

				line[i][j + 1] = '\0';
				strcpy(last->label_name, line[i]);
				if (gg == 0)
				{
					last->index = i + immid_commands - wordsnum;
					gg = 1;
				}
				else
				{
					last->index = i + immid_commands - wordsnum - 1 - labels_num;
					labels_num++;
				}
				// initiating index, not final
				t++;

				last->next = (label *)(malloc(sizeof(label)));
				if (last->next == NULL)
				{
					printf("Error! memory not allocated.");
					exit(0);
				}
				last = last->next;
				last->index = 0;
				last->next = NULL;
				break;
			}
			if ((line[i][j] == '$') && (line[i][j + 1] == 'i') && (line[i][j + 2] == 'm') && (line[i][j + 3] == 'm'))
				immid_commands++;						// found immidiate command, labels index is increasing

		}
	}
	for (i = 0; i <= num_of_lines; i++)	// printing line by line
	{
		imm_check = 0;
		if ((line[i][0] == 'E') && (line[i][1] == 'N') && (line[i][2] == 'D'))
		{
			break;
		}
		for (j = 0; line[i][j] != 0; j++)		//checking if there is an imm or its a comment
		{
			if ((line[i][j] == '$') && (line[i][j + 1] == 'i') && (line[i][j + 2] == 'm') && (line[i][j + 3] == 'm'))
				imm_check = 1;
			if (line[i][j] == ':')			// skiping comments
			{
				skip = 1;
				break;
			}
			if ((line[i][j] == '.') && (line[i][j + 1] == 'w') && (line[i][j + 2] == 'o') && (line[i][j + 3] == 'r') && (line[i][j + 4] == 'd')) // checking if the line is a word line
				checkw = 1;
		}
		if (skip == 1)
		{
			skip = 0;
			continue;
		}
		if (checkw == 1)
		{
			checkw = 0;
			continue;
		}

		imm_check = 0;
		for (j = 0; line[i][j] != 0; j++)		//checking if there is an imm or its a comment
		{
			if ((line[i][j] == '$') && (line[i][j + 1] == 'i') && (line[i][j + 2] == 'm') && (line[i][j + 3] == 'm'))
				imm_check = 1;
			if (line[i][j] == ':')			// skiping comments
			{
				skip = 1;
				break;
			}
		}
		if (skip == 1)
		{
			skip = 0;
			continue;
		}
		a = 0;
		while (a<16)
		{
			//	if (line[i][0]=='a')
			//	line[i][0] = 'a';
			compare = my_strcmp(line[i], opps, registers, a, line_place);		//found opp or register- compare is num of opp

			if (compare != -1)				//found opp or register							
			{
				print_w(line[i], compare, a, bitsline);
				a = a + 4;
				if (a >= 15)
					break;
				line_place++;
				while (1<2)
				{
					if (line[i][line_place] == '$')
					{
						if ((line[i][line_place - 1] == 32) || (line[i][line_place - 1] == '\t') || (line[i][line_place - 1] == ','))
						{
							line_place++;
							break;
						}
					}

					line_place++;
				}

				continue;
			}
			if (compare == -1)						//found label or immidiate
			{

				if ((line[i][0] == '.') && (line[i][1] == 'w') && (line[i][2] == 'o'))
					break;
				compare = check_label(line[i], a, first, bitsline);
				print_w(line[i], compare, a, bitsline);
				a = a + 4;
				if (a >= 15)
					break;
				line_place++;
				while (1<2)
				{
					if ((line[i][0] == '.') || (line[i][0] == '\0'))
						break;

					if (line[i][line_place] == '$')
					{
						if (line[i][line_place - 1] == 32)
						{
							line_place++;
							break;
						}
					}

					line_place++;
				}

				continue;
			}
		}

		bitsline[16] = '\n';
		a = 0;
		printed = printed + final_print(bitsline, memory, imm_check, line[i], first);
		imm_check = 0;
		line_place = 0;
	}
	sortwordbyindex(wordi);
	add_words_to_print(memory, printed, wordi);
	fclose(assembly);
	fclose(memory);
	free_line(line);
	free_word(wordi);
	free_label(first);
	return 0;
}


int my_strcmp(char *line, char *(opps[]), char *(registers[]), int place, int line_place)		//checks if next word an opp and returns its num
{																			//(place- in the line)
	int i = 0, e = 0, j = 0, a = 0;
	char *arr;
	char word[100];				// next word in line
	arr = word;
	while ((line[line_place] == ',') || (line[line_place] == '$') || (line[line_place] == 0) || (line[line_place] == 32) || (line[line_place] == 9))
	{
		line_place++;
	}
	for (i = line_place; (line[i] != 32); i++)
	{
		if ((line[i] == ',') || (line[i] == '$') || (line[i] == 0) || (line[i] == 9))
		{
			word[j] = -52;
			break;
		}
		word[j] = line[i];
		j++;
	}
	a = 10;
	j = 10;
	for (i = 0; i<16; i++)					// compare between all registeres and opps and return its number if found
	{									// else returns -1
		j = str_cmp(arr, opps[i]);
		a = str_cmp(arr, registers[i]);
		if (a == 0)
			return i;
		if (j == 0)
			return i;
	}
	return -1;
}
int final_print(char *bitsline, FILE* memory, int imm, char *line, struct label* first)
{
	int i = 0, a = 0, neg = 0, decimal = 0, j = 0, printed = 0;
	char hex[5];
	for (i = 0; i<16; i++)				//binary string to Decimal 
	{
		if (bitsline[i] == '1')
		{
			decimal = decimal + (int)pow((double)2, (double)(16 - i - 1));
		}
	}
	decimal_to_hex(hex, decimal, neg);
	neg = 0;
	hex[4] = '\0';
	for (j = 0; j <= 3; j++)
		fprintf(memory, "%c", hex[j]);
	fprintf(memory, "%c", '\n');
	printed++;
	if (imm == 1)
	{
		printed++;

		i = check_num(line, first);

		if (i<0)
			decimal_to_hex(hex, i, 1);
		if (i>0)
			decimal_to_hex(hex, i, 0);
		if (i == 0)
		{
			hex[0] = '0';
			hex[1] = '0';
			hex[2] = '0';
			hex[3] = '0';
		}

		for (j = 0; j <= 3; j++)
			fprintf(memory, "%c", hex[j]);
		fprintf(memory, "%c", '\n');

	}
	hex[4] = '\0';
	return printed;
}
void decimal_to_hex(char* hex, int decimal, int neg)
{
	int  i = 0, j = 0, n = 0;
	if (neg == 1)
		decimal = decimal*(-1);
	for (i = 0; i<4; i++)
	{
		hex[i] = '0';
	}
	for (i = 0; i<4; i++)
	{
		j = decimal % 16;				//Decimal string to hex 
		if (j>9)
		{
			hex[3 - i] = 55 + j;
		}
		else
			hex[3 - i] = 48 + j;
		decimal = decimal / 16;
		if (decimal == 0)
			break;
	}
	hex[4] = '\0';
	if (neg == 1)
	{
		for (i = 0; i<4; i++)
		{
			if ((hex[i]>47) && (hex[i]<54))
				hex[i] = 70 - (hex[i]) + '0';
			else
				hex[i] = '9' - (hex[i]) + '0';
		}
		hex[3] = hex[3] + 1;
	}

}
int check_label(char *line, int place, struct label* first, char* bitsline)		// returns label index
{
	int p = -1;
	int i = 0, j = 0;
	struct label* fir;
	fir = first;
	char word[100];				// next word in line
	for (i = place; (line[i] != 32); i++)
	{
		if ((line[i] == ',') || (line[i] == '$') || (line[i] == 0))
		{
			word[j] = '\0';
			break;
		}
		word[j] = line[i];
		j++;
	}
	while (fir != NULL)
	{
		p = strcmp(word, fir->label_name);
		if (p == 0)
		{

			return fir->index;
		}
		fir = fir->next;
	}
	return -1;
}
int str_cmp(char * arr, char * opps)
{
	int i = 0;
	if ((arr == NULL) || (opps == NULL))
		return 1;
	for (i = 0; arr[i] != -52; i++)
	{
		if ((arr[i] == '\n') || (opps[i] == '/0'))
			return 0;
		if (arr[i] != opps[i])
			return 1;
		if ((arr[i] == NULL) || (opps[i] == NULL))
			return 1;
	}
	return 0;
}
int check_num(char *line, struct label* first)
{
	int i = 0, n = 0, len = 0, label_or_num = 0, j = 0, a = 0, neg = 0, s = 0;
	char word[50];
	if (line[i] == 'l')
		s++;
	for (i = 0; a<3; i++)
	{
		if (line[i] == ',')
			a++;
	}
	while ((line[i] == ',') || (line[i] == 9) || (line[i] == 10) || (line[i] == 32))
		i++;
	if (line[i] == 45)
		neg = 1;
	for (i; ((line[i] != 9) || (line[i] == 10)); i++)
	{
		if ((line[i]<48) || (line[i]>57))
		{
			if ((line[i] == '\0') || (line[i] == ',') || (line[i] == '\n'))
			{
				break;
			}
			if (line[i] == 45)
			{
				neg = 1;
				continue;
			}
			label_or_num = 1;
		}
		word[j] = line[i];
		j++;
	}
	word[j] = '\n';
	for (i = 0; i<5; i++)
	{
		if ((word[i] != '0') && (word[i] != 32) && (word[i] != '\n'))
		{
			break;
		}
	}
	if (i >= 4)
		return 0;
	if (label_or_num == 0)
	{
		if (neg == 1)
			return (-1)*atoi(word);
		else
			return atoi(word);


	}
	else
	{
		j = 0;
		while (1<3)
		{
			if (first == NULL)
				return -1;
			if (first->label_name[j] == ':')
				return first->index;
			if (first->label_name[j] != word[j])
			{
				first = first->next;
				j = 0;
				continue;
			}
			else
				j++;
		}

	}
	return first->index;

}
void print_w(char * line, int compare, int place, char *bitsline)
{
	int i = 0;
	if (compare % 2 == 1)
		bitsline[place + 3] = '1';
	else
		bitsline[place + 3] = '0';
	compare = compare / 2;
	if (compare % 2 == 1)
		bitsline[place + 2] = '1';
	else
		bitsline[place + 2] = '0';
	compare = compare / 2;
	if (compare % 2 == 1)
		bitsline[place + 1] = '1';
	else
		bitsline[place + 1] = '0';
	compare = compare / 2;
	if (compare % 2 == 1)
		bitsline[place] = '1';
	else
		bitsline[place] = '0';
}
void make_word(char* line, struct wordstruct* wordf, int *wor)
{
	int i = 0, j = 0, neg = 0, numint = 0, numdest = 0, hexd = 0, hexn = 0;
	char dest[50];
	char num[50];
	while (1<2)
	{
		if ((47<line[i]) && (line[i]<58))
		{
			break;
		}
		i++;
	}

	while (1<2)
	{
		if ((line[i] == '0') && ((line[i + 1] == 'X') || (line[i + 1] == 'x')))
		{
			i = i + 2;
			hexd = 1;
			break;

		}
		if ((line[i]<47) || (line[i]>58))
			break;
		dest[j] = line[i];
		i++;
		j++;
	}
	if (hexd == 1)
	{
		j = 0;
		while ((line[i] != ' ') && (line[i] != '\n'))
		{
			dest[j] = line[i];
			i++;
			j++;
		}

	}
	dest[j] = '\0';
	j = 0;
	while (1<2)
	{
		if (line[i] == '-')
		{
			neg = 1;
		}
		if ((47<line[i]) && (line[i]<58))
			break;
		i++;
	}
	while (1<2)
	{
		if ((line[i] == '0') && ((line[i + 1] == 'X') || (line[i + 1] == 'x')))
		{
			i = i + 2;
			hexn = 1;
			break;
		}
		if (line[i] == '-')
		{
			neg = 1;
		}
		if ((line[i]<47) || (line[i]>58))
			break;
		num[j] = line[i];
		i++;
		j++;
	}
	if (hexn == 1)
	{
		while ((line[i] != ' ') && (line[i] != '\n') && (line[i] != -52) && (line[i] != '\0'))
		{
			num[j] = line[i];
			i++;
			j++;
		}

	}

	if (hexn == 1)
		numint = strtol(num, NULL, 16);
	if (hexd == 1)
		numdest = strtol(dest, NULL, 16);
	if (hexn == 0)
		numint = atoi(num);
	if (hexd == 0)
		numdest = atoi(dest);
	wor[0] = numint;
	wor[1] = numdest;
	if (neg == 1)
		wor[0] = wor[0] * (-1);

}
void add_words_to_print(FILE *memory, int printed, struct wordstruct* wordi)
{
	int i = 0, h = 0, words = 0;
	while (wordi != NULL)
	{
		if (h == 0)
		{
			for (i = printed; i < wordi->dest - 2; i++)
			{
				fprintf(memory, "%s", "0000\n");
				words++;
			}
			h = 1;
		}
		if (wordi->num < 0)
		{
			(wordi->num) = (wordi->num) - 0xFFFF0000;
		}
		fprintf(memory, "%04X\n", wordi->num);
		i++;
		wordi = wordi->next;
		if (wordi == NULL)
			break;
		while (i < wordi->dest - 2)
		{
			fprintf(memory, "%s", "0000\n");
			i++;
		}
	}


}
void free_word(struct wordstruct* wordi)
{
	struct wordstruct* temp = wordi;
	while (wordi != NULL)
	{
		temp = wordi->next;
		free(wordi);
		wordi = temp;
		if (wordi == NULL)
			return;
	}


}
void free_line(char **line)
{
	int i = 0;
	for (i = 0; 0<1; i++)
	{
		if ((line[i][0] == 'E') && (line[i][1] == 'N') && (line[i][2] == 'D'))
		{
			break;
		}
		free(line[i]);
	}
	free(line);

}
void free_label(struct label * first)
{
	struct label *temp = NULL;
	while (2>1)
	{
		if (first == NULL)
			return;
		temp = first->next;
		free(first);
		first = temp;

	}
}
wordstruct *sortwordbyindex(struct wordstruct*  wordi)
{
	int swapped, i;
	struct wordstruct * ptr1;
	struct wordstruct * lptr = NULL;
	/* Checking for empty list */
	if (wordi == NULL)
		return;
	do
	{
		swapped = 0;
		ptr1 = wordi;

		while (ptr1->next != lptr)
		{
			if (ptr1->dest > ptr1->next->dest)
			{
				swap(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);
}
void swap(struct wordstruct* a, struct wordstruct* b)
{
	int num = 0, dest = 0;
	num = a->num;
	dest = a->dest;
	a->num = b->num;
	a->dest = b->dest;
	b->num = num;
	b->dest = dest;
}
int check_double_dest(int dest, int num, struct wordstruct* wordi)	// checking if there is a word in this destination
{
	struct wordstruct* temp = wordi;
	while (temp != NULL)
	{
		if (temp->dest == dest + 2)
		{
			temp->num = num;
			return 1;
		}
		else
			temp = temp->next;
	}
	return 0;
}