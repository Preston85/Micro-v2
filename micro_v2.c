
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#define TREE_SIZE (USHRT_MAX + 1)
#define MAX (LONG_MAX / 2 + 1)
#define SEPARATOR 0

/*** CFG - context-free grammar ***/

// Struktura gramatyki bezkontekstowej w postaci normalnej Chomsky'ego.

	unsigned last, act, size, freeNode, pos1, pos2, pos3, lastMax, DICT, N, M, BUFFSIZE; // 6 < N < MAXINT
	unsigned *gram, *next, *listNext, *lsop, *rsop, *hash, *item, *left, *right, *tab, *defined;
	unsigned char *buff;
	FILE *f1, *f2, *f3;


unsigned wczytano = 0;
// jesli para "ab" jest mniejsza od tej pod 'pos', return 1;
// jesli para "ab" jest wieksza lub rowna, return 0;
unsigned compare(a, b, pos)
{
	if(pos <= last)
	{
		if(a < lsop[pos])
			return 1;
		else if(a > lsop[pos])
			return 0;
		else
			return b < rsop[pos];
	}
	else
	{
		if(a < gram[next[pos]])
			return 1;
		else if(a > gram[next[pos]])
			return 0;
		else
			return b < gram[next[next[pos]]];
	}
}



/*
wyswietlSymbol(unsigned x)
{
	if(x < 32)
		printf("%5d", x);
	else if(x < 128)
		printf("%5c", x);
	else if(x < N)
		printf("%5d", x);
	else if(x == N)
		printf("    -");
	else
		printf("ERROR");
}




wezel(unsigned x)
{
	if(x != N)
	{
		wezel(left[x]);
		printf("%5d ", x);
		if(item[x] <= last)
		{
			wyswietlSymbol(lsop[item[x]]);
			printf(" ");
			wyswietlSymbol(rsop[item[x]]);
			printf(" ");
		}
		else
		{
			wyswietlSymbol(gram[next[item[x]]]);
			printf(" ");
			wyswietlSymbol(gram[next[next[item[x]]]]);
			printf(" ");
		}
		printf("%d %d, pos = %d\n", left[x], right[x], item[x]);
		printf("\n");
		wezel(right[x]);
	}
}

weryfikuj(unsigned x)
{
	unsigned a, b, pos;
	
	
	if(x != N)
	{
		if(item[x] <= last)
		{
			a = lsop[item[x]];
			b = rsop[item[x]];
		}
		else
		{
			a = gram[next[item[x]]];
			b = gram[next[next[item[x]]]];
		}
		
		weryfikuj(left[x]);
		if(left[x] != N)
		{
			pos = item[left[x]];
			if(compare(a, b, pos) == 0)
				;
			else
			{
				printf("FUCK! ab rodzica = %d %d\n", a, b);
				printf("\n");
				printf("node rodzica = %d\n", x);
				printf("left i right rodzica = %d %d\n", left[x], right[x]);
				system("pause");
			}	
		}
		if(right[x] != N)
		{
			pos = item[right[x]];
			if(compare(a, b, pos) == 1)
				;
			else
			{
				printf("FUCK! ab rodzica = %d %d\n", a, b);
				printf("\n");
				printf("node rodzica = %d\n", x);
				printf("left i right rodzica = %d %d\n", left[x], right[x]);
				system("pause");
			}	
		}
		weryfikuj(right[x]);
	}
}



wyswietlSop()
{
	unsigned i;
	printf("      LSOP RSOP\n");
	for(i = DICT; i < last; ++i)
	{
		printf("%5d", i);
		wyswietlSymbol(lsop[i]);
		wyswietlSymbol(rsop[i]);
		printf("\n");
	}
	printf("\n");
}


wyswietlGram()
{
	unsigned i, start = last;
	while(gram[start] == N)
		++start;
	printf("      GRAM NEXT\n");
	for(i = start; i < N; ++i)
	{
		printf("%5d", i);
		wyswietlSymbol(gram[i]);
		wyswietlSymbol(next[i]);
		printf("\n");
	}
	printf("\n");
	
}

wyswietlTree(unsigned x)
{
	if(hash[x] != N)
	{
		printf(" NODE TREE LEFT RGHT Korzen: %d\n", hash[x]);
		wezel(hash[x]);
		printf("\n");
	}
		
}


checkVirginity(unsigned x)
{
	if(x != N)
	{
		printf("DRAMAT! Proba ponownego zapisu\n");
	}
}

wyswietlNode()
{
	unsigned i;
	unsigned *parent = malloc(N * sizeof(unsigned));
	for(i = DICT; i < N; ++i)
	{
		parent[i] = N;
	}	
	for(i = DICT; i < N; ++i)
	{
		if(item[i] != N)
		{
			if(left[i] != N)
			{
				checkVirginity(parent[left[i]]);
				parent[left[i]] = i;
			}
		
			if(right[i] != N)
			{
				checkVirginity(parent[right[i]]);
				parent[right[i]] = i;
			}
		}
	}
	
	printf(" NODE ITEM LEFT RGHT PRNT    A    B\n");
	for(i = DICT; i < N; ++i)
	{
		if(item[i] != N)
		{
			printf("%5d%5d", i, item[i]);
			wyswietlSymbol(left[i]);
			wyswietlSymbol(right[i]);
			wyswietlSymbol(parent[i]);
			if(item[i] <= last)
			{
				wyswietlSymbol(lsop[item[i]]);
				wyswietlSymbol(rsop[item[i]]);
			}
			else
			{
				wyswietlSymbol(gram[next[item[i]]]);
				wyswietlSymbol(gram[next[next[item[i]]]]);
			}
			
			
			printf("\n");
		}
	}
	free(parent);
}

wyswietlStrukture()
{
	int i;
	printf("\n N = %d, DICT = %d, M = %d, BUFFSIZE = %d\n", N, DICT, M, BUFFSIZE);
	printf(" last = %d, act = %d, freeNode = %d, node = %d\n", last, act, freeNode, hash[0]);
	printf("\n");
	wyswietlSop();
	wyswietlGram();
	wyswietlTree(0);
	wyswietlNode();
}


wyswietlProd()
{
	unsigned i = N - 2;
	unsigned size = 0;
	printf("\n S ->");
	while(i != act)
	{	
		if(gram[i] != N)
			wyswietlSymbol(gram[i]);
		i = next[i];
		size++;
	}
	
	wyswietlSymbol(gram[act]);
	size++;
	
	wyswietlSymbol(gram[next[act]]);
	size++;
	

	printf("\n\nRozmiar prod = %d\n", size);
}

wyswietlTab()
{
	unsigned i;
	printf("tab: ");
	for(i = 0; i < size; ++i)
		wyswietlSymbol(tab[i]);
	printf("\n");
}

niezmiennik()
{
	if((item[15324257] == 14098120) && (item[14502858] == 14918962) && (left[15324257] == 14502858))
	{
		printf("Zaburzony niezmiennik! Wczytano = %d\n", wczytano);
		system("pause");
	}	
	
}
*/
///////////////////////////////////////////////////




// Przypisuje M wartosc, ktora jest liczb¹ pierwsz¹ o podobnej wielkosci co N.
// Liczba ta jest daleka od potêg dwójki.

setM()
{
	M = 24;
	unsigned counter = 5;

	
	while(M < 2 * N)
	{
		M *= 2;
		++counter;
	}
	
	switch(counter)
	{
		case 5	: M -= 1;	break; // M = 24 - 1 = liczba pierwsza 23
		case 6	: M -= 1;	break; // M = 48 - 1 = liczba pierwsza 47
		case 7	: M -= 7;	break; // M = 96 - 7 = liczba pierwsza 91
		case 8	: M -= 1;	break; // ...
		case 9	: M -= 1;	break;
		case 10	: M -= 7;	break;
		case 11	: M -= 5;	break;
		case 12	: M -= 5;	break;
		case 13	: M -= 1;	break;
		case 14	: M -= 7;	break;
		case 15	: M -= 5;	break;
		case 16	: M -= 13;	break;
		case 17	: M -= 5;	break;
		case 18	: M -= 11;	break;
		case 19	: M -= 7;	break;
		case 20	: M -= 1;	break;
		case 21	: M -= 11;	break;
		case 22	: M -= 7;	break;
		case 23	: M -= 7;	break;
		case 24	: M -= 19;	break;
		case 25	: M -= 11;	break;
		case 26	: M -= 49;	break;
		case 27	: M -= 5;	break;
		case 28	: M -= 35;	break;
		case 29	: M -= 13;	break;
		case 30	: M -= 11;	break;
		case 31	: M -= 25;	break;
		case 32	: M -= 11;	break;
		case 33	: M -= 5;	break;
		case 34	: M -= 11;	break;
		case 35	: M -= 25;	break;
		case 36	: M -= 1;	break;
		case 37	: M -= 17;	break;
		case 38	: M -= 25;	break;
		case 39	: M -= 29;	break;
		case 40	: M -= 1;	break;
		case 41	: M -= 13;	break;
		case 42	: M -= 19;	break;
		case 43	: M -= 25;	break;
		case 44	: M -= 13;	break;
		case 45	: M -= 1;	break;
		case 46	: M -= 71;	break;
		case 47	: M -= 7;	break;
		case 48	: M -= 23;	break;
		case 49	: M -= 31;	break;
		case 50	: M -= 5;	break;
		case 51	: M -= 35;	break;
		case 52	: M -= 11;	break;
		case 53	: M -= 13;	break;
		case 54	: M -= 5;	break;
		case 55	: M -= 37;	break;
		case 56	: M -= 83;	break;
		case 57	: M -= 1;	break;
		case 58	: M -= 35;	break;
		case 59	: M -= 55;	break;
		case 60	: M -= 25;	break;
		case 61	: M -= 31;	break;
		case 62	: M -= 137;	break;
		case 63	: M -= 119;	break;
	}
	
}


// Przywraca stan pocz¹tkowy struktury cfg.
void cfgReset()
{	
	unsigned i;
	
	for(i = DICT; i < N; ++i)
	{
		next[i] = i - 1;
		listNext[i] = i - 1;
		gram[i] = N;
	}	
	
	for(i = 0; i < M; ++i)
		hash[i] = N;	
	freeNode = N - 1;
	act  = N - 1;
	last = DICT;
	
	for(i = 0; i < N; ++i)  // do skasowania
	{
		item[i] = N;
		right[i] = N;
	}
	
}



// Przydziela pamiêc dla struktury cfg oraz inicjuje wartosci jej zmiennych.
cfgOpen()
{
	lastMax = 0;
	setM();
	hash = malloc(M * sizeof(unsigned));
	lsop = malloc(N * sizeof(unsigned));
	rsop = malloc(N * sizeof(unsigned));
	item = malloc(N * sizeof(unsigned));
	left = malloc(N * sizeof(unsigned));
	right = malloc(N * sizeof(unsigned));
	buff = malloc(BUFFSIZE * sizeof(unsigned char));
	gram = lsop;
	next = rsop;
	listNext = left;
	gram[N - 1] = N;
	gram[N - 2] = N;
	cfgReset();
}


// Zwalnia pamiêæ przydzielon¹ elementom struktury cfg.
void cfgClose()
{
	if(lsop)
		free(lsop);
	if(rsop)
		free(rsop);
	if(hash)
		free(hash);
	if(item)
		free(item);
	if(left)
		free(left);
	if(right)
		free(right);
	if(buff)
		free(buff);
}



// Pobiera dane w postaci bajtów z pliku wejsciowego do bufora,
// gdzie zapisuje je jako symbole. Zwraca liczbê pobranych bajtów.
unsigned bufferLoad(FILE *f1)
{
	unsigned i, n;
	//unsigned char *bytes;
	//bytes = malloc(BUFFSIZE * sizeof(unsigned char));
	n = fread(buff, sizeof(unsigned char), BUFFSIZE, f1);
	//for(i = 0; i < n; ++i)
	//	buff[i] = (unsigned)bytes[i];
	return n;
}	





hashAdd(unsigned pos)
{
	unsigned a, b, c = 1, k, node, parent = 0;
	
	a = gram[next[pos1]];
	b = gram[next[next[pos1]]];


	/*
	printf("hashAdd(%d),", pos);
	wyswietlSymbol(a);
	wyswietlSymbol(b);
	printf("\n");
	*/
	
	
	k = (a * b) % M;
	//k = 0;
	left[0] = hash[k];  
	node = left[0]; 
	while(node != N)
	{
		parent = node;
		c = compare(a, b, item[node]);  // c == 1, jesli para 'ab' jest mniejsza od pary na pozycji pos. Wpp c == 0.
		node = c ? left[node] : right[node];
	}
	
	node = freeNode;
	freeNode = listNext[freeNode];
	if(c)
	 	left[parent] = node;
	else
		right[parent] = node;
	
	
	left[node] = N;
	right[node] = N;
	item[node] = pos1;
	hash[k] = left[0];  // Je¿eli wlasnie dodalismy korzen, to w hash[k] zostanie przypisany jego indeks. Wpp. wartosc pozostanie jak poprzednio.
	
	

	
}



// Szuka wartosci pos w hashu, aby ja stamtad usunac.
unsigned hashDelete(unsigned pos)
{
	unsigned a, b, x, k, node, parent = 0;

	a = gram[pos];
	b = gram[next[pos]];

	/*
	printf("hashDel(%d),", pos);
	wyswietlSymbol(a);
	wyswietlSymbol(b);
	printf("\n");
	*/

	k = (a * b) % M;
	//k = 0;
	left[0] = hash[k];   
	node = left[0]; 
	
	
	while(item[node] <= last || next[item[node]] != pos)  // Zlokalizuj wêze³ do usuniêcia.
	{
		parent = node;
		node = compare(a, b, item[node]) ? left[node] : right[node];
	}
	
	pos = item[node];
	
	while((left[node] != N) && (right[node] != N))
	{
		
		parent = node;
		if(rand() % 2)
		{
			// szukaj poprzednika
			x = left[node];
			while(right[x] != N)
			{
				parent = x;
				x = right[x];
			}
			if((parent != node) && (gram[next[item[x]]] == gram[next[item[left[node]]]]) && (gram[next[next[item[x]]]] == gram[next[next[item[left[node]]]]]))
			{
				// szukaj jednak nastepnika, bo poprzednik zaburzylby drzewo BST.
				parent = node;
				x = right[node];
				while(left[x] != N)
				{
					parent = x;
					x = left[x];
				}
			}
		}
		else
		{
			// szukaj nastepnika
			x = right[node];
			while(left[x] != N)
			{
				parent = x;
				x = left[x];
			}
		}
		item[node] = item[x];  // zastap usuwany node wartoscia poprzednika/nastepnika (czyli wartoscia w wezle x).
		node = x;  // Bedziemy usuwac wezel x;
	}
	
	if(left[node] != N)
	{
		if(node == left[parent])
			left[parent] = left[node];
		else
			right[parent] = left[node];
	}
	else if(right[node] != N)
	{
		if(node == left[parent])
			left[parent] = right[node];
		else
			right[parent] = right[node];
	}
	else
	{
		if(node == left[parent])
			left[parent] = N;
		else
			right[parent] = N;
	}
	
	item[node] = N; // do skasowania
	listNext[node] = freeNode;
	freeNode = node;
	hash[k] = left[0];  // Je¿eli usunelismy korzen, to w hash[k] zostanie przypisane N. Wpp. wartosc pozostanie jak poprzednio.
	
	
	return pos; // ma zwraaæ wskazanie na poprzednika pozycji pos.
	
}



// Bada ostatni¹ parê w pg. Je¿eli wyst¹pi³a po raz pierwszy, return 0.
// Je¿eli wyst¹pi³a po raz drugi, utwórz dla niej symbol w s³owniku i zast¹p nim obie pary; return 1.
// Je¿eli mia³a ju¿ swój symbol w s³owniku, zast¹p nim tê parê; return 1.
unsigned checkPair()
{
	
	unsigned a, b, c = 1, k, node, parent = 0;
	
	a = gram[next[act]];
	b = gram[next[next[act]]];
	
	
	
	
	/*
	printf("\ncheckPair(),");
	wyswietlSymbol(a);
	wyswietlSymbol(b);
	printf("\n");
	wyswietlStrukture();
	*/
	k = (a * b) % M;
	//k = 0;
	left[0] = hash[k];   
	node = left[0]; 
	
	/*
	fprintf(f3, "k = %d\n", k);
	if(node == N)
		fprintf(f3, "Drzewo jest puste.\n");
	*/
	
	while(node != N)  // Przeszukuj kolejne wezly drzewa, a¿ dojdziesz do liscia.
	{                 // lub znajdziesz wskazanie na tak¹ sam¹ parê jak ostatnia w pg.
		
		pos1 = item[node];
		if((pos1 <= last) && (a == lsop[pos1]) && (b == rsop[pos1])) // W s³owniku wystêpuje symbol reprezentuj¹cy
		{                                                            // tak¹ sam¹ parê jak ostatnia w pg.
			//printf("Znaleziono powtorzenie - w slowniku pod pos1 = %d.\n", pos1);
			
			
			act = hashDelete(act); // Zast¹p ostatni¹ pare w pg symbolem ze slownika.
			gram[next[next[act]]] = pos1;

			return 1;
		}
		else if((pos1 > last) && (a == gram[next[pos1]]) && (b == gram[next[next[pos1]]]) && (next[pos1] != act) && (a != 32 || b == 32 || !SEPARATOR)) 
		{                                         // Taka para jak ostatnia w pg wyst¹pi³a w niej ju¿ wczesniej.
			//printf("Znaleziono powtorzenie - w gramatyce pod pos1 = %d.\n", pos1);
			item[node] = last;
			lsop[last] = a;
			rsop[last] = b;
			pos3 = next[act]; // Utwórz nowy symbol w s³owniku i zast¹p nim obie pary.
			pos2 = next[next[pos1]];	
			pos1 = hashDelete(pos1);
			pos2 = hashDelete(pos2);
			gram[pos2] = last;
			gram[next[pos2]] = last++;
			hashAdd(pos1);  // AX
			pos1 = next[pos1];
			if(act != next[pos2])
			{
				act = hashDelete(act);
				next[pos1] = next[pos2];
				next[next[act]] = pos2;
				next[pos2] = pos3;
				hashAdd(pos1);  // XB
			}
			else
				act = pos1;

			return 1;
		}
		else // Sprawdzimy w kolejnym wezle drzewa.
		{	
			//printf("Jeszcze sprawdzam...\n");
			parent = node;
			c = compare(a, b, pos1);
			node = c ? left[node] : right[node];
		}
	}
	
	//printf("To pierwsze wystapienie takiej pary.\n");
	
	node = freeNode;
	freeNode = listNext[freeNode];
	if(c)
	 	left[parent] = node;
	else
		right[parent] = node;
	
	item[node] = act;
	left[node] = N;
	right[node] = N;
	act = next[act];
	hash[k] = left[0];  // Je¿eli wlasnie dodalismy korzen, to w hash[k] zostanie przypisany jego indeks. Wpp. wartosc pozostanie jak poprzednio.
	return 0;
}






// Zakoduj 
void bufferEncode(FILE *f2, unsigned buffRead)
{
	unsigned i;
	
	i = 0;
	while(i < buffRead)
	{
		wczytano++;
		//if(wczytano % 1000000 == 0)
		//	printf("%d\n", wczytano);
		//printf("last/nna/act %d %d %d\n", last, next[next[act]], act);
		
		gram[next[next[act]]] = buff[i++];
		while(checkPair());
		if(gram[last + 2] != N)
		{
			cfgWrite();
			cfgReset();
		}
	}
}
	
	
	

// Zapisuje strukturê gramatyki bezkontekstowej w postaci tablicy.
unsigned cfgToTab()
{
	unsigned i, pos, stop, x, counter;
	
	
	tab = hash;
	defined = item;
	for(i = 0; i < DICT; ++i)
		defined[i] = 1;
	for(i = DICT; i < last; ++i)
		defined[i] = 0;
	
	size = 0;
	counter = 0;
	i = 0; 
	stop = next[next[act]];
	pos = next[N - 2];
	while(pos != stop) 
	{
		tab[i++] = gram[pos];
		while(counter < i)
		{
			x = tab[counter++];
			if(!defined[x])
			{
				tab[i++] = lsop[x];
				tab[i++] = rsop[x];
				defined[x] = 1;
			}
		}
		pos = next[pos];
	}
	size = i;
	
	/*
	printf("Przed przemianowaniem:\n");
	wyswietlTab();
	*/
	
	N = size - last + 2 * DICT;
	if(last > lastMax)
		lastMax = last;
	
	// Przemianuj symbole i ustaw N w miejsce pierwszych wyst¹pien.
	for(i = 0; i < DICT; ++i)
		defined[i] = i;
	
	for(i = DICT; i < last; ++i)
		defined[i] = 0;

	counter = DICT;
	for(i = 0; i < size; ++i)
	{
		x = tab[i];
		if(defined[x] || !x)
			tab[i] = defined[x];
		else
		{
			defined[x] = counter++;
			tab[i] = N;
		}
	}
	
	/*
	printf("Po przemianowaniu:\n");
	wyswietlTab();
	printf(" counter = %d, size = %d\n", counter, size);
	*/
	
	return size;
}





cfgWrite()
{
	size = cfgToTab();
	fwrite(&N,  sizeof(unsigned), 1, f2);
	fwrite(&size,  sizeof(unsigned), 1, f2);
	fwrite(tab,  sizeof(unsigned), size, f2);
	unsigned i;
}

// 560163149 = "Mic!"

// Kompresuje dane z pliku wejsciowego.
micro(char *fileName1, char *fileName2, unsigned n, unsigned dict, unsigned buffsize)
{ 
	unsigned buffRead, mic;
	
	f1 = fopen(fileName1, "rb");
	f2 = fopen(fileName2, "wb");

	mic = 560163149;
	N = n;
	DICT = dict;
	BUFFSIZE = buffsize;
	
	fseek(f1, 0, 2);
	size = ftell(f1);
	fseek(f1, 0, 0);
	if(N > size + 3 - last + 2 * DICT)
		N = size + 3 - last + 2 * DICT;
	
	fwrite(&mic,  sizeof(unsigned), 1, f2);
	fwrite(&DICT, sizeof(unsigned), 1, f2);
	fwrite(&lastMax, sizeof(unsigned), 1, f2); // Tutaj dowolna wartosc, bo i tak bedzie na koniec nadpisana - rezerwujemy w ten sposob miejsce na INT-a.
	fwrite(&N, sizeof(unsigned), 1, f2);  // Tutaj dowolna wartosc, bo i tak bedzie na koniec nadpisana - rezerwujemy w ten sposob miejsce na INT-a.
	
	cfgOpen();
	
	int i;
	for(i = 0; i < N; ++i)
		gram[i] = N;/////////// chyba do skasowania
	
		
	do
	{
		buffRead = bufferLoad(f1);  // Pobierz dane do bufora
		//printf(" buffRead = %d\n", buffRead); ////////////////
		bufferEncode(f2, buffRead);
		
	} while(buffRead);

	cfgWrite();
	fseek(f2, 8, 0);
	fwrite(&lastMax, sizeof(unsigned), 1, f2);
	
	cfgClose();
	fclose(f1);
	fclose(f2);
}




int main()
{
	clock_t start, finish;
	double duration = 0;
	start = clock();
	
	//f3 = fopen("gram.txt", "wt");
	micro("bible.txt", "bible.txt.mic", 16777216, 256, 32768);
	// demicro("bible.txt.mic", "wynik.txt");
	
	//fclose(f3);
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;   
	printf("\n\nCzas wykonywania %2.5f seconds\n", duration);
	
	return 0;
}
