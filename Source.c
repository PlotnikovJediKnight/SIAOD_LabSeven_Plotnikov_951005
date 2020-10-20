#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>

//СИАОД, Лабораторная работа №7
//Плотников, 951005

#define RUSSIAN_LETTERS 1251	//русская кодировка
#define MAX_STRING_LENGTH 50	//максимальная вводимая длина строки

int N = -1;				//кол-во вершин в графе
int M = -1;				//кол-во дуг в графе
int s = -1;				//номер стартовой вершины
int v = -1;				//номер конечной вершины
int** a = NULL;			//матрица смежности
char* visited = NULL;	//массив посещений

//функция для очистки буфера вывода
void clear_buffer() {
	char ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
}

//функция для проверки корректности вводимого кол-ва вершин
int checkVertexNumber(const char* p) {
	int sum = 0, N = strlen(p);

	for (int i = 0; i < N; i++)
		if (p[i] >= '0' && p[i] <= '9') {
			sum *= 10;
			sum += (p[i] - '0');
		}
		else
			return -1;

	if (sum == 0 || sum == 1) return -1;

	return sum;
}

//запрос количества вершин
int getVertexRequest(void) {
	printf("Введите кол-во вершин ориентированного графа: ");
	char str[MAX_STRING_LENGTH];
	gets(str);
	
	int res = checkVertexNumber(str);
	return res;
}

//функция для проверки коррекности вводимого кол-ва дуг
int checkEdgeNumber(const char* p) {
	int sum = 0, K = strlen(p);

	for (int i = 0; i < K; i++)
		if (p[i] >= '0' && p[i] <= '9') {
			sum *= 10;
			sum += (p[i] - '0');
		}
		else
			return -1;


	if (sum == 0 || sum > N * (N - 1) ) return -1;

	return sum;
}

//запрос количества дуг
int getEdgeRequest(void) {
	printf("Введите кол-во дуг в ориентированном графе: ");
	char str[MAX_STRING_LENGTH];
	gets(str);

	int res = checkEdgeNumber(str);
	return res;
}

//запрос и проверка самих дуг
int getEdgesRequest(void) {
	int from, to, weight, res;
	for (int i = 0; i < M; i++) {
		printf("Введите дугу в формате \"номер_вершины_1 номер_вершины_2 вес_дуги\":\n");
		res = scanf("%d%d%d", &from, &to, &weight);

		if (res != 3) return -1;
		if (from <= 0 || from > N) return -1;
		if (to <= 0 || to > N) return -1;
		if (weight <= 0) return -1;

		a[from - 1][to - 1] = weight;
	}
	return 0;
}

//выделение памяти под матрицу смежности и массив посещений
int allocateMemoryForAdjacentMatrix(void) {
	a = (int**) malloc(sizeof(int*) * N);
	if (a == NULL) return -1;

	for (int i = 0; i < N; i++) {
		a[i] = (int*)malloc(sizeof(int) * N);
		if (a[i] == NULL) return -1;

		for (int j = 0; j < N; j++)
			a[i][j] = -1;	//-1 символизирует отсутствие ребра как такового
	}

	visited = (char*)malloc(sizeof(char) * N);
	for (int i = 0; i < N; i++)
		visited[i] = 0;

	return 0;
}

//очистка памяти под матрицу смежности и матрицу посещений
void deallocateMemoryForAdjacentMatrix(void) {
	for (int i = 0; i < N; i++)
		free(a[i]);
	free(a);
	free(visited);
}

//вспомогательный метод для вывода сообщения об ошибке
void finalMessage(const char* msg) {
	printf(msg);
	deallocateMemoryForAdjacentMatrix();
}

//запрос на ввод стартовой вершины и проверка
int getSRequest(void) {
	printf("Введите стартовую вершину: ");
	int res = scanf("%d", &s);

	if (res != 1) return -1;
	if (s <= 0 || s > N) return -1;

	return s;
}

//запрос на ввод конечной вершины и проверка
int getFRequest(void) {
	printf("Введите финальную вершину: ");
	int res = scanf("%d", &v);

	if (res != 1) return -1;
	if (v <= 0 || v > N) return -1;

	return v;
}


#define MAX_PATH_LENGTH 100		//максимальная длина пути от стартовой вершины до конечной
int path[MAX_PATH_LENGTH];		//массив, содержащий путь от стартовой вершины до конечной
int pInd = 0;					//индекс для заполнения пути

int* pointers[MAX_PATH_LENGTH];	//массив указатель, каждый указатель будет указывать на область памяти,
								//в которой будет храниться длина пути и сам непосредственно путь
int pp = 0;						//индекс для заполнения массива указателей

//Рекурсивная функция для поиска в глубину
void DFS(int u, int dist) {
	visited[u] = 1;		//Посещаем u-ую вершину и заносим её в путь
	path[pInd] = u;
	pInd++;
	if (u == v - 1) {	//Посещенная вершина - конечная
		pointers[pp] = (int*) malloc(sizeof(int) * (pInd + 2));	//Выделяем память для указателя
		pointers[pp][0] = dist;									//Запоминаем накопившееся растояние
		for (int i = 0; i < pInd; i++)
			pointers[pp][i + 1] = path[i] + 1;					//Копируем весь пройденный путь
		pointers[pp][pInd + 1] = -1;							//Признак окончания данных

		pp++;
	}
	else {
		for (int i = 0; i < N; i++)
			if (i != u && !visited[i] && a[u][i] != -1) {		//Вершина не идет сама в себя? Мы ее не посещали? Путь в нее существует?
				DFS(i, dist + a[u][i]);							//Тогда переходим в нее
			}
	}

	pInd--;														//Снимаем вершину со стека
	visited[u] = 0;												//Возвращаем ей её "непосещенность"
}

//Функция для сортировки всех путей
void mySort(void) {
	for (int i = pp - 1; i >= 0; i--)
		for (int j = 0; j < i; j++)
			if (pointers[j][0] > pointers[j + 1][0]) {
				int* buf = pointers[j];
				pointers[j] = pointers[j + 1];
				pointers[j + 1] = buf;
			}
}

//Освобождаем память под все пути
void deallocateArrays(void) {
	for (int i = 0; i < pp; i++)
		free(pointers[i]);
}

#define INFINITY 1000 //Какое-то очень большое число (для нашей задачи)
//Алгоритм Флойда
void FloydAlgorithm(void) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			if (a[i][j] == -1) a[i][j] = INFINITY; //Отствутвие связи помечаем бесконечностью
	}

	for (int k = 0; k < N; k++)
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++) {
				if (a[i][k] + a[k][j] < a[i][j]) {
					a[i][j] = a[i][k] + a[k][j];
				}
			}
}

int main(void) {

	SetConsoleCP(RUSSIAN_LETTERS);
	SetConsoleOutputCP(RUSSIAN_LETTERS);

	//Ввод данных
	////////////////////////////////////////////////////////////////////////////////////////////
	{
		N = getVertexRequest();
		if (N == -1) {
			printf("Введено некорректное число вершин!\n");
			return 0;
		}

		if (allocateMemoryForAdjacentMatrix() == -1) {
			printf("Не удалось выделить память под матрицу смежности!\n");
			return 0;
		}

		M = getEdgeRequest();
		if (M == -1) {
			finalMessage("Введено некорректное число дуг!\n");
			return 0;
		}

		if (getEdgesRequest() == -1) {
			finalMessage("Одна из дуг была введена некорректно!\n");
			return 0;
		}

		clear_buffer();

		s = getSRequest();
		if (s == -1) {
			finalMessage("Введена некорректная стартовая вершина!\n");
			return 0;
		}

		clear_buffer();

		v = getFRequest();
		if (v == -1) {
			finalMessage("Введена некорректная финальная вершина!\n");
			return 0;
		}

		clear_buffer();
	}

	//Нахождение и вывод всех путей
	////////////////////////////////////////////////////////////////////////////////////////////
	{
		DFS(s - 1, 0);
		mySort();

		if (pp == 0) printf("Не существует пути между заданными вершинами!");
		else {
			printf("Все возможные пути из стартовой вершины в конечную:\n");
			for (int i = 0; i < pp; i++) {
				printf("%d: ", pointers[i][0]);
				for (int j = 1; pointers[i][j] != -1; j++)
					printf("%d ", pointers[i][j]);
				printf("\n");
			}
		}
	}

	//Применение алгоритма Флойда для нахождения центра орграфа
	////////////////////////////////////////////////////////////////////////////////////////////
	{
		FloydAlgorithm();

		int minEccentricity = INFINITY;
		int ind = -1;

		for (int j = 0; j < N; j++) {
			int eccentricity = -1;
			for (int i = 0; i < N; i++)
				if (a[i][j] != INFINITY && a[i][j] > eccentricity)
					eccentricity = a[i][j];
			if (eccentricity < minEccentricity && eccentricity != -1) {
				minEccentricity = eccentricity;
				ind = j;
			}

		}

		printf("Центр орграфа - вершина %d с эксцентриситетом %d", ind + 1, minEccentricity);

		deallocateMemoryForAdjacentMatrix();
		deallocateArrays();
	}

	return 0;
}