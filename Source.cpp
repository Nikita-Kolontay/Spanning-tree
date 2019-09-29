#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>

using namespace std;

struct top
{
	short int key;
	int y1;
	int y2;
};

void fill_hand(int size, unsigned int**graf)
{
	for (int y = 0; y < size; y++)
	{
		graf[y][y] = 0;
		for (int x = y+1; x < size; x++)
		{
			cout << "Введите вес ребра между вершиной "<< y+1 <<" и "<< x+1 <<": ";
			int val = 0;
			cin >> val;
			while(val<0||val>32766)
			{
				cout << "Введено недопустимое значение, попробуйте ещё раз: ";
				cin >> val;
			}
			graf[x][y] = val;
			graf[y][x] = val;
		}
	}
}

void fill_rand(int size, unsigned int**graf)
{
	HCRYPTPROV hProv;
	unsigned char c;
	CryptAcquireContext(&hProv, 0, 0, PROV_RSA_FULL, 0);
	for (int y = 0; y < size; y++)
	{
		for (int x = y; x < size; x++)
		{
			CryptGenRandom(hProv, 1, &c);
			graf[x][y] = c;
			graf[y][x] = c;
		}
		graf[y][y] = 0;
	}
	CryptReleaseContext(hProv, 0);
}

int find_min(unsigned int**graf, top* tree, int size, int* x, int i)
{
	int KEYmin = 32767, Y1min=0, Y2min=0;
	for (int j = 0; j < i; j++)
	{
		for (int t = 0; t < size; t++)
		{
			if (graf[x[j]][t] < KEYmin && graf[x[j]][t] != 0 && graf[t][t] != 32767)
			{
				KEYmin = graf[x[j]][t];
				Y1min = x[j];
				Y2min = t;
			}
		}
	}
	graf[Y2min][Y2min] = 32767;
	graf[Y1min][Y2min] = 0;
	graf[Y2min][Y1min] = 0;
	tree[i].key = KEYmin;
	tree[i].y1 = Y1min;
	tree[i].y2 = Y2min;
	return Y2min;
}

void ost_tree(unsigned int**graf, int size)
{
	top* tree = new top[size];
	tree[0].key = 0;
	tree[0].y1 = 0;
	tree[0].y2 = 0;
	int *x = new int[size+1];
	x[0]=0;
	graf[0][0] = 32767;
	for (int i = 0; i < size; i++)
	{
		x[i+1] = find_min(graf, tree, size, x, i+1);
	}
	for (int k = 1; k < size; k++)
	{
		cout << tree[k].y1+1<<"-"<< tree[k].y2+1 << " " << tree[k].key << "\n";
	}
}

unsigned int** init_graf(unsigned int* &help)
{
	cout << "Введите кол-во вершин графа: ";
	unsigned int size = 0;
	cin >> size;
	while (size <= 0)
	{
		cout << "Введено недопустимое значение, попробуйте ещё раз: ";
		cin >> size;
	}
	*help = size;
	unsigned int **graf = new unsigned int*[size];
	for (int i = 0; i < size; i++)
	{
		graf[i] = new unsigned int[size];
	}
	cout << "Выберите вариант заполнения: ручной/рандом (0/1): ";
	int fl = 0;
	cin >> fl;
	while (fl < 0 || fl>1)
	{
		cout << "Введено недопустимое значение, попробуйте ещё раз: ";
		cin >> fl;
	}
	switch (fl)
	{
	case 0: fill_hand(size, graf); break;
	case 1: fill_rand(size, graf); break;
	}

	return graf;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	unsigned int size = 0;
	unsigned int* help = &size;
	unsigned int**graf=init_graf(help);
	double t = clock();
	ost_tree(graf, size);
	double t1 = clock();
	cout << "На нахождение минимального остовного дерево звтрачено " << (t1 - t) / CLOCKS_PER_SEC << " секунд(ы).";
	_getch();
}