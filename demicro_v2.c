#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#define MAX (LONG_MAX / 2 + 1)

unsigned last, lastMax, buffRead, mic, size, stop, DICT, N, BUFFSIZE;
unsigned *lsop, *rsop, *tab, *gram, *map, *rank, *lead, *stat, *tree, *left, *right;
unsigned char *buff;
FILE *f1, *f2;

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


wyswietlTab()
{
	unsigned i;
	printf("tab: ");
	for(i = DICT; i < DICT + size; ++i)
		wyswietlSymbol(tab[i]);
	printf("\n");
}


cfgOpenDemicro()
{
	tab = malloc((2 * (lastMax - DICT) + N) * sizeof(unsigned));
	printf("Rozmiar gram = %d\n", N);
	gram = malloc(N * sizeof(unsigned));
	map  = malloc(lastMax * sizeof(unsigned));
	lead = malloc(lastMax * sizeof(unsigned));
	stat = malloc(lastMax * sizeof(unsigned));
	buff = malloc(BUFFSIZE * sizeof(unsigned char));
	rank = gram;
	lsop = gram;
	rsop = tab;
}


wyswietlSopDemicro()
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


wyswietlGramDemicro()
{
	unsigned i;
	printf("      GRAM\n");
	for(i = N - 1; i > N - size; --i)
	{
		printf("%5d", i);
		wyswietlSymbol(gram[i]);
		printf("\n");
	}
	printf("\n");
	
}


wyswietlStrukture()
{
	wyswietlSopDemicro();
	wyswietlGramDemicro();
}


cfgClose()
{
	if(tab)
		free(tab);
	if(gram)
		free(gram);
	if(map)
		free(map);
	if(lead)
		free(lead);
	if(stat)
		free(stat);
	if(buff)
		free(buff);
}


void tabToCfg()
{
	unsigned i, pos, counter;
	
	last = DICT;
	counter = DICT;
	i = DICT;
	pos = N - 1;
	stop = DICT + size;
	while(i < stop)
	{
		gram[pos] = tab[i++];
		if(gram[pos] == N)
		{
			gram[pos] = counter++;
			while(last < counter)
			{
				lsop[last] = tab[i++];
				if(lsop[last] == N)
					lsop[last] = counter++;
				rsop[last] = tab[i++];
				if(rsop[last] == N)
					rsop[last] = counter++;
				++last;
			}
		}
		--pos;
	}
	stop = pos;
}


cfgDecode()
{
	
	unsigned i, pos = N - 1, b = 0;
	printf("last = %d\n", last);
	
	while(pos > stop)
	{
		i = last + 1;
		tab[i] = gram[pos--];
		while(i != last)
		{
			if(tab[i] < DICT)
			{
				buff[b++] = (unsigned char)tab[i--];
				if(b == BUFFSIZE)
				{
					fwrite(buff, sizeof(unsigned char), b, f2);
					b = 0;
				}
			}
			else
			{
				tab[i+1] = lsop[tab[i]];
				tab[i] = rsop[tab[i]];
				i++;
			}
		}
	}
	fwrite(buff, sizeof(unsigned char), b, f2);
	
}


demicro(char *fileName1, char *fileName2, unsigned buffSize)
{
	f1 = fopen(fileName1, "rb");
	f2 = fopen(fileName2, "wb");
	fread(&mic , sizeof(unsigned), 1, f1);
	if(mic == 560163149)
		printf("Najprawdopodobniej jest to archiwum Micro.\n");
	else
		printf("To z pewnoscia nie jest archiwum Micro!\n");
	
	fread(&DICT, sizeof(unsigned), 1, f1);
	fread(&lastMax, sizeof(unsigned), 1, f1);
	fread(&N   , sizeof(unsigned), 1, f1);
	printf("N = %d, DICT = %d, lastMax = %d\n", N, DICT, lastMax);
	printf("Rozmiar tab = %d\n", (2 * (lastMax - DICT) + N));
	BUFFSIZE = buffSize;
	
	
	
	cfgOpenDemicro();
	
	
	int i;
	
	while(!feof(f1))  // Dopoki sa dane w pliku
	{
		
		if(fread(&N, sizeof(unsigned), 1, f1))
		{
			fread(&size, sizeof(unsigned), 1, f1);
			fread(&tab[DICT], sizeof(unsigned), size, f1);
			tabToCfg();
			cfgDecode();
		}
	}
	cfgClose();
	fclose(f1);
	fclose(f2);
	
}


int main()
{
	clock_t start, finish;
	double duration = 0;
	start = clock();
	
	demicro("bible.txt.mic", "wynik.txt", 1024*1024);
	
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;   
	printf("\n\nCzas wykonywania %2.5f seconds\n", duration);
	return 0;
}

