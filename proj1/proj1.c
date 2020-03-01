/*
Projekt c. 1
Autor: Jan Vavra (xvavra20)
*/

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

//prototypy funkci
int myStrcmp(char *str1, char *str2);
char checkEscape();
int numCheck(char *str);
void xFunction();
void sFunction(int size);
void rFunction();
void basicFunction(int skip, int lenght);


int main(int argc, char *argv[])
{
	// inicializace promennych
	int sFlag = 0;
	int rFlag = 0;
	int xFlag = 0;
	int basicFlag = 0;
	int skipValue = 0;
	int lenghtValue = 0;
	int help = 0;

	for(int i = 1;i < argc; i += 2)				//opakuje se dokud neprecte vsechny liche argumenty (-r,-S-x ... atd)
	{
		if(myStrcmp(argv[i], "-r") && basicFlag == 0)			//pokud obsahuje argument -r
		{
        	rFlag = 1;

			if (argc > 2)				//"Pokud je program spusten s libovolnymi dalsimi neprazdnymi argumenty, vypise svuj kratky popis (napovedu) a uspesne skonci"
				help = 1;

        	break;
		}
		else if (myStrcmp(argv[i], "-S") && basicFlag == 0)		//pokud obsahuje argument -S
		{
			sFlag = 1;
			if (argc == 2)			//pokud je tam pouze argument -S
			{
				printf("Nezadal jste ciselný argument\n");
				return (-1);
			}
			else if (argc == 3)			//pokud obsahuje ciselnou hodnotu
			{
				if (numCheck(argv[i + 1]) == 0)				//kontrola jestli je argument číslo
				{
					lenghtValue = strtol(argv[i + 1], NULL, 10);
					if (lenghtValue <= 0 || lenghtValue >= 200)		//kontrola jesli ciselny argument lezi v zadanem intervalu
					{
						printf("Ciselný argument musi byt z intervalu (0,200)");
						return (-1);
					}
				}
				else
				{
					printf("Argument, ktery jste zadali není cislo\n");
					return (-1);
				}
			}
		}
		else if (myStrcmp(argv[i], "-x") && basicFlag == 0)		//pokud obsahuje argument -x
		{
        	xFlag = 1;

			if (argc > 2)
				help = 1;

        	break;
		}
		else if(myStrcmp(argv[i], "-s"))					//pokud obsahuje argument -s
		{
        	basicFlag = 1;
			if (argc == 2 || argc == 4)			//je bez ciselneho argumentu?
			{
				printf("Musite jeste zadat ciselny argument\n");
				return (-1);
			}
			else if (argc == 3 || argc == 5)		//je ciselny argument?
			{
				if (numCheck(argv[i + 1]) == 0)
					skipValue = strtol(argv[i + 1], NULL, 10);
				else
				{
					printf("Argument, ktery jste zadali není cislo\n");
					return (-1);
				}
			}
		}

		else if (myStrcmp(argv[i], "-n"))					//pokud obsahuje argument -n
		{
			basicFlag = 1;
			if (argc == 2 || argc == 4)		//je bez ciselneho argumentu?
			{
				printf("Musite jeste zadat ciselny argument\n");
				return (-1);
			}
			else if (argc == 3 || argc == 5)			//je zadan ciselny argument?
			{
				if (numCheck(argv[i + 1]) == 0)
				{
					lenghtValue = strtol(argv[i + 1], NULL, 10);
					if (lenghtValue <= 0)
					{
						printf("Argument musi byt kladné cislo\n");
						return (-1);
					}
				}
				else
				{
					printf("Argument, ktery jste zadali není cislo\n");
					return (-1);
				}
			}
		}
		else
			help = 1;
	}
	if (help)
	{
		printf("Nespravne argumenty zadany\n"
			"Ocekavane argumenty\n"
			"-s M -n N = s preskoci M znaku n precte N znaku a skonci\n"
			"-x = vypise vstup hexadecimalne na jeden radek\n"
			"-S N = vypise takove retezce, ktere jsou dlouhe N znaku\n"
			"-r = prevede vstup z hexa na znak(y) na radek\n"
			);
		return(-1);
	}
	if(!xFlag && !rFlag && !sFlag)			//pokud nebyl zadny z predchozich argumenu x,S,r tak to bude basic funkce
		basicFlag = 1;


	// vypis hexadecimalne na 1 řádek
	if (xFlag)
		xFunction();

	//vypis retezec dlouhy N znaků
	else if (sFlag)
			sFunction(lenghtValue);

	//arguument reverse
	else if (rFlag)
			rFunction();
	
	//bez argumentu nebo s skip nebo i lenght
	else if (basicFlag)
			basicFunction(skipValue, lenghtValue);

	return 0;
}


int myStrcmp (char *str1, char *str2)
{
    for(int i = 0; str1[i] != '\0' || str2[i] != '\0';i++)	//dokud nejsme na konci retezce 
    {
		if(str1[i] != str2[i])		
			return 0;
    }
	return 1;
}


int numCheck(char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] >= '0' && str[i] <= '9')		//pokud je hodnota znaku mezi '0' a '9'
			;
		else
			return 1;
	}
	return 0;
}


void xFunction()
{
	int znak;
	while ((znak = getchar()) != EOF)	//bude nacitat znak dokud neni End Of File (-1)
	{
		printf("%02x", znak);
	}
	putchar('\n');
	return;
}


void sFunction(int size)
{
	char vystup[200];
	int counter = 0;
	int znak;
	int overload = 0;

	while ((znak = getchar()) != EOF)
	{
		if (counter == 200)				//pokud jsme zaplnili cely retezec
		{
			for (int i = 0; i < counter; i++)   //vypis retezce
			{
				printf("%c", vystup[i]);
				vystup[i] = '0';            //nahrazeni pismena nulou
			}
			counter = 0;
		}
		else if (znak == '\n' || znak == '\0')		//dokud nenarazi na konec radku nebo stringu
		{
			if ((counter + overload*200) >= size)
			{
				for (int i = 0; i < counter; i++)   //vypis retezce
				{
					printf("%c", vystup[i]);
					vystup[i] = '0';            //nahrazeni pismena nulou
				}
				printf("\n");
				counter = 0;
			}
			else
			{
				counter = 0;
			}
		}
		else 
		{
			vystup[counter] = znak;		//jinak nacita do retezce
			counter++;
		}

	}

	return;
}


void rFunction()
{
	int hex = 0;
	int i = 0;
	int znak;

	while ((znak = getchar()) != EOF)	//dokud neni EOF
	{

		if (znak == ' ' || znak == '\t' || znak == '\0' || znak == '\n')	//preskoci prazdne znaky
			continue;
		else if (znak >= '0' && znak <= '9')		//pokud je znak cislo, odecte hodnotu 0 z ASCII
			hex += znak - '0';
		else if (znak >= 'a' && znak <= 'f')    //pokud je znak pismeno od a-f
			hex += znak - 'a' + 10;
		else
		{
			fprintf(stderr, "Nespravny argument\n");
			return;
		}
		
		if (i == 0)			//pokud je toto 1. iterace
		{
			hex *= 16;
			i++;
		}
		else if (i == 1)	//pokud je toto 2. iterace
		{
			printf("%c", hex);
			i = 0;
			hex = 0;
		}

	}

	if (hex != 0)		//pokud v hex neco zustalo, znamena to, ze je pocet znaku lichy
	{
		hex = hex / 16;
		printf("%c", hex);
	}
	return;
}


void basicFunction(int skip, int lenght)
{
	int znak;
	int pocet = skip;

	for (int i = 0; i < pocet; i++)	//pokud je argument -s zadan, preskoci n znaku
		znak = getchar();

	while (znak != EOF)
	{
		char vystup[16];		
		printf("%08x ", pocet);		//vypis adresy

		for (int i = 0; i < 16; i++)
		{
			if (i == 8)				//pokud jsme vytiskli 8 bajtů
				putchar(' ');

			znak = getchar();

			if (lenght != 0)		//pokud je zadan -n argument
				if (((skip + lenght) - (pocet + i)) <= 0)
					znak = EOF;
				

			if (znak != EOF)			//zjistuje jestli jsme na konci souboru
			{
				printf(" %02x", znak);

				if (isprint(znak))		//je znak tisknutelny?
					vystup[i] = znak;
				else
					vystup[i] = '.';
			}
			else
			{
				printf("   ");		//pokud ano, vyplni radek prazdnymi znaky
				vystup[i] = ' ';
			}

		}
		//vypis retezce (16 znaků)
		printf("  |");
		for (int j = 0; j < 16; j++)
			printf("%c", vystup[j]);

		printf("|\n");

		pocet += 16;

		if (lenght != 0)		//pokud je zadan -n argument
			if (((skip + lenght) - pocet) <= 0)
				znak = EOF;
	}

	return;
}
