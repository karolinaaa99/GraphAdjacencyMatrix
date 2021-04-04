#include <iostream>
#include <queue>
#include <stack>
#include <vector>

using namespace std;

struct edge //krawedz
{
	int s; //poczatek krawedzi
	int t; //koniec krawedzi
};

class Graph 
{
	private:
		int** adjMatrix; //macierz sasiedztwa, dwuwymiarowa
		int n;				//liczba węzłów
	public:
		Graph(int n, int m, edge edges[], bool directed = false);	//tworzy graf w oparciu o podaną listę krawędzi, konstruktor
		int edgeCnt();					//zwraca liczbę krawędzi
		int nodeCnt(); 					//zwraca liczbę węzłów
		void insertEdge(int u, int v);			//wstawia krawędź 
		void deleteEdge(int u, int v);			//usuwa krawędź 
		bool check(int u, int v);			//sprawdza czy istnieje krawędź
		void bfs(int s);				//wyświetla węzły w porządku odwiedzenia
		void bfs(int s, int* connectedComp, int components);
		void dfs(int s);				//wyświetla węzły w porządku odwiedzenia
		void visit(int *visited, int s);
		int* connectedComponents(); //metoda (działająca dla grafu nieskierowanego), która dla każdego węzła zwraca nr spójnej składowej, do której węzeł należy
		//int* stronglyConnectedComponents(); // metoda (działająca dla grafu skierowanego), która dla każdego węzła zwraca nr silnie spójnej składowej, do której węzeł należy
		int* Dijkstra(int s); //s-wezel zrodlowy, zwraca tabele odleglosci
		vector<int> path(int s, int e); //zwraca najkrotsza sciezke pomiedzy wybrana para wezlow
		//double** WarshallFloyd(); //zwraca macierz odleglosci
		friend ostream& operator<<(ostream& out, Graph& g); //wyswietla macierz sasiedztwa na ekranie
		//~Graph(); //destruktor
		//inne potrzebne/pomocnicze metody składowe
};

Graph::Graph(int n, int m, edge edges[], bool directed) //tu juz nie piszemy jeszcze raz true lub false
{
	this->n = n;

	adjMatrix = new int* [n]; //tworzymy zerowy wiersz
	for (int i = 0; i < n; ++i) //tworzymy pozostale wiersze
	{
		adjMatrix[i] = new int[n];
	}

	//cala macierz sasiedztwa wypelniam 0
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			adjMatrix[i][j] = 0;
		}
	}

	//wpisuje do macierzy 1, jesli krawedz laczaca wierzcholki istnieje
	//dla grafu nieskierowanego
	for (int i = 0; i < m; i++)
	{
		//dla grafu nieskierowanego, czyli macierz jest symetryczna wzgledem glownej przekatnej
		adjMatrix[edges[i].s][edges[i].t] = 1;
		adjMatrix[edges[i].t][edges[i].s] = 1;
	}

	//dla grafu skierowanego
	/**for (int i = 0; i < m; i++)
	{
		adjMatrix[edges[i].s][edges[i].t] = 1;
	}**/

}

int Graph::edgeCnt()
{
	int m = 0;

	//odczytuje z macierzy sasiedztwa ile jest krawedzi
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (adjMatrix[i][j] == 1)
			{
				m++;
			}
		}
	}
	//teraz m dziele przez 2, bo graf jest nieskierowany
	m = m / 2;

	return m;
}

int Graph::nodeCnt()
{
	return n;
}

void Graph::insertEdge(int u, int v)
{
	adjMatrix[u][v] = 1;
}

void Graph::deleteEdge(int u, int v)
{
	adjMatrix[u][v] = 0;
}

bool Graph::check(int u, int v)
{
	if (adjMatrix[u][v] == 1)
		return true;
	else
		return false;
}

void Graph::bfs(int s) //s-wezel, od ktorego rozpoczynamy przeszukiwanie grafu
{
	//int m = edgeCnt(); //obliczam ile jest krawedzi

	//tworze tablice przechowujaca odwiedzone wezly
	int* odwiedzone = new int[n];
	//wpisuje do tablicy odwiedzone 0
	for (int i = 0; i < n; ++i)
	{
		odwiedzone[i] = 0;
	}

	//wezel s ustawiam jak odwiedzony
	odwiedzone[s] = 1;

	//tworze kolejke intow, to jest kolejka fifo
	queue<int> kolejka;

	/**
	METODY DLA BIBLIOTEKI QUEUE
	push - umieszczenie nowego elementu na końcu kolejki
	pop - usunięcie istniejącego elementu z początku kolejki
	empty - informacja czy kolejka jest pusta
	size - zwraca ilość elementów umieszczonych w kolejce
	front - zwraca wartość pierwszego elementu w kolejce
	back - zwraca wartość ostatniego elementu w kolejce
	**/

	//dodaje do kolejki wezel s
	kolejka.push(s);

	int aktualny; //pamieta pierwszy element w kolejce fifo

	while (!kolejka.empty())
	{
		aktualny = kolejka.front();
		cout << aktualny << " ";

		for (int i = 0; i < n; ++i)
		{
			if ((adjMatrix[aktualny][i] == 1) && (odwiedzone[i] == 0))
			{
				odwiedzone[i] = 1;
				kolejka.push(i); //dodaje i na koniec kolejki
			}
		}
		kolejka.pop(); //usuniecie elementu z poczatku kolejki
	}
}

void Graph::bfs(int s, int* connectedComp, int components) 
{
	//int m = edgeCnt(); //obliczam ile jest krawedzi

	//tworze tablice przechowujaca odwiedzone wezly
	int* odwiedzone = new int[n];
	//wpisuje do tablicy odwiedzone 0
	for (int i = 0; i < n; ++i)
	{
		odwiedzone[i] = 0;
	}

	//wezel s ustawiam jak odwiedzony
	odwiedzone[s] = 1;

	//tworze kolejke intow, to jest kolejka fifo
	queue<int> kolejka;

	/**
	METODY DLA BIBLIOTEKI QUEUE
	push - umieszczenie nowego elementu na końcu kolejki
	pop - usunięcie istniejącego elementu z początku kolejki
	empty - informacja czy kolejka jest pusta
	size - zwraca ilość elementów umieszczonych w kolejce
	front - zwraca wartość pierwszego elementu w kolejce
	back - zwraca wartość ostatniego elementu w kolejce
	**/

	//dodaje do kolejki wezel s
	kolejka.push(s);

	int aktualny; //pamieta pierwszy element w kolejce fifo

	while (!kolejka.empty())
	{
		aktualny = kolejka.front();
		if (connectedComp[aktualny] == -1)
			connectedComp[aktualny] = components;

		for (int i = 0; i < n; ++i)
		{
			if ((adjMatrix[aktualny][i] == 1) && (odwiedzone[i] == 0))
			{
				odwiedzone[i] = 1;
				kolejka.push(i); //dodaje i na koniec kolejki
			}
		}
		kolejka.pop(); //usuniecie elementu z poczatku kolejki
	}
}

void Graph::dfs(int s)
{
	int* visited = new int[n];

	for(int i = 0; i < n; i++) {
		visited[i] = -1;
	}
	
	visit(visited, s);
	
	//tworze stos intow
	//stack<int> stos;

	/**
	METODY DLA BIBLIOTEKI <stack>
	push - umieszczenie nowego elementu na szczycie stosu;
	pop - zdjęcie istniejącego elementu ze szczytu stosu;
	empty - informacja czy stos jest pusty;
	size - zwraca ilość elementów umieszczonych na stosie;
	top - zwraca wartość szczytowego elementu na stosie.
	**/

	//tworze tablice odwiedzin typu bool
	//bool* V = new bool[n];

	//zeruje (false) tablice odwiedzin
	/**for (int i = 0; i < n; i++)
	{
		V[i] = false;
	}**/

	//wrzucamy startujacy wierzcholek na stos
	//stos.push(s);

	/**while (!stos.empty())
	{
		s = stos.top();
		//usuwamy odwiedzany element
		stos.pop();

		cout << s << " ";

		V[s] = true;

		for (int j = n - 1; j >= 0; --j)
		{
			if (adjMatrix[j][s] != 0 && V[j] == false)
			{
				//wrzucamy na stos jego sasiadow
				stos.push(j);
				V[j] = true;//Znaznaczamy ,że go odwiedzimy!(w niedalekiej przyszłości)
				//Inaczej będziemy mieli np taką sytuację w stosie 2,3,4,2 <-- 2x dwójka
			}
		}
	}**/
}

void Graph::visit(int *visited, int u) {
	visited[u] = 0;
	cout << u << " ";

	for(int v = 0; v < n; v++) {
//		if (adjMatrix[u][v] == 0) {
//			continue;
//		}
		if (visited[v] == -1)
		{
			visit(visited, v);
		}
	}
	visited[u] = 1;
}

int* Graph::connectedComponents()
{
	int* connectedComp = new int[n];

	for(int i = 0; i < n; i++) {
		connectedComp[i] = -1;
	}

	int components = 0;
	for(int i = 0; i < n; i++) {
		if(connectedComp[i] == -1) {
			bfs(i, connectedComp, components);
			components++;
		}
	}
	return connectedComp;
}

int* Graph::Dijkstra(int s)
{
	int* q = new int[n]; //zawiera wezly
	vector<int> v; //zawiera wezly, dla ktorych juz obliczono najkrotsza sciezke s
	int* d = new int[n]; //tablica kosztow
	int* p = new int[n]; //tablica poprzednikow
	int min, index, amount = n;
	//inicjalizacja tablic
	for (int i = 0; i < n; i++) 
	{
		q[i] = i;
		d[i] = INT_MAX - 200; //najpierw ustawiamy ja na nieskonczonosc
		p[i] = -1;
	}
	d[s] = 0; //koszt dla wierzcholka startowego ustawiamy na 0
	while (amount > 0) 
	{
		min = INT_MAX - 200;
		for (int i = 0; i < n; i++) 
		{
			if (q[i] != -1 && min > d[i]) 
			{
				min = d[i];
				index = i;
			}
		}
		v.push_back(q[index]);
		q[index] = -1;
		amount--;
		for (int i = 0; i < n; i++) 
		{
			if (check(index, i) && q[i] != -1) 
			{
				if (d[i] > d[index] + adjMatrix[index][i]) 
				{
					d[i] = d[index] + adjMatrix[index][i];
					p[i] = index;
				}
			}
		}
	}

	return d;

	/**for (int i = 0; i < n; i++) 
	{
		cout << i << "\t";
	}
	cout << endl;**/
	/**for (int i = 0; i < n; i++) 
	{
		cout << d[i] << "\t";
	}
	cout << endl;**/
	/**for (int i = 0; i < n; i++) 
	{
		cout << p[i] << "\t";
	}
	cout << endl;**/
	
	/**int* d = new int[nodeCnt()]; //tablica kosztow
	int* p = new int[nodeCnt()]; //tablica poprzednikow
	bool* QS = new bool[nodeCnt()]; //zawiera true, jesli dla wierzcholka obliczono najkrotsza sciezke od s
	int* S = new int[nodeCnt()]; //zawiera wezly dla, ktorych obliczono najkrotsza sciezke od s, stos
	int sptr = 0; //wskaznik stosu
	
	//inicjujemy tablice
	for (int i = 0; i < n; i++)
	{
		d[i] = INT_MAX; //ustawiamy na nieskonczonosc
		p[i] = -1;
		QS[i] = false;
	}

	d[s] = 0; //koszt dojscia s jest zerowy**/


}

/**vector<int> Graph::path(int s, int e)
{
	vector<int> wynik;
	vector<int> temp;

	int** dane;

	dane = Dijkstra(s);
	int x = e;
	temp.push_back(e);

	while (x != s) {
		x = dane[1][x];
		temp.push_back(x);
	}


	for (int i = 0, j = temp.size() - 1; i < temp.size(); i++, j--) {
		wynik.push_back(temp[j]);
	}

	cout << endl << "Najkrotsza droga wynosi " << dane[0][e] << endl;
	for (size_t i = 0; i < wynik.size(); i++)
	{
		cout << wynik[i] << " ";
	}

	return wynik;
}**/

ostream& operator<<(ostream& out, Graph& g)
{
	for (int i = 0; i < g.nodeCnt(); i++)
	{
		for (int j = 0; j < g.nodeCnt(); j++)
		{
			out << "[" << i << "]" << "[" << j << "] " << g.adjMatrix[i][j] << endl;
		}
	}

	return out;
}

int main(int argc, char** argv) 
{
	//WYDAJE MI SIE, ZE ZLE DZIALA DFS DLA GRAFU SKIEROWANEGO, ALE NIE WIEM
	
	//int n = 10, m = 14; //dla grafu nieskierowanego
	int n = 10, m = 13; //dla grafu skierowanego
	edge undirectedConnectedGraph[] = { {0,1}, {0,4}, {0,6}, {1,2}, {1,7}, {2,3}, {2,8}, {3,5}, {3,9}, {4,7}, {5,8}, {6,7}, {7,8}, {8,9} };
	//edge directedConnectedGraph[] = { {0,1}, {1,2}, {1,7}, {2,8}, {3,2}, {3,9}, {4,0}, {5,3}, {6,0}, {7,4}, {7,6}, {8,5}, {9,8} };
	Graph g(n, m, undirectedConnectedGraph);
	//Graph g(n, m, directedConnectedGraph);
	cout << g;

	g.deleteEdge(1, 2);
	g.deleteEdge(7, 8);

	g.bfs(0);
	g.bfs(2);

	int* tab;
	tab = g.connectedComponents();
	cout << endl;
	for (int i = 0; i < n; i++) {
		cout << tab[i] << " ";
	}
	cout << endl;
	//g.bfs(0);
	//cout << endl;
	//g.dfs(0);


	return 0;
}