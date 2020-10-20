#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>

//�����, ������������ ������ �7
//���������, 951005

#define RUSSIAN_LETTERS 1251	//������� ���������
#define MAX_STRING_LENGTH 50	//������������ �������� ����� ������

int N = -1;				//���-�� ������ � �����
int M = -1;				//���-�� ��� � �����
int s = -1;				//����� ��������� �������
int v = -1;				//����� �������� �������
int** a = NULL;			//������� ���������
char* visited = NULL;	//������ ���������

//������� ��� ������� ������ ������
void clear_buffer() {
	char ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
}

//������� ��� �������� ������������ ��������� ���-�� ������
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

//������ ���������� ������
int getVertexRequest(void) {
	printf("������� ���-�� ������ ���������������� �����: ");
	char str[MAX_STRING_LENGTH];
	gets(str);
	
	int res = checkVertexNumber(str);
	return res;
}

//������� ��� �������� ����������� ��������� ���-�� ���
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

//������ ���������� ���
int getEdgeRequest(void) {
	printf("������� ���-�� ��� � ��������������� �����: ");
	char str[MAX_STRING_LENGTH];
	gets(str);

	int res = checkEdgeNumber(str);
	return res;
}

//������ � �������� ����� ���
int getEdgesRequest(void) {
	int from, to, weight, res;
	for (int i = 0; i < M; i++) {
		printf("������� ���� � ������� \"�����_�������_1 �����_�������_2 ���_����\":\n");
		res = scanf("%d%d%d", &from, &to, &weight);

		if (res != 3) return -1;
		if (from <= 0 || from > N) return -1;
		if (to <= 0 || to > N) return -1;
		if (weight <= 0) return -1;

		a[from - 1][to - 1] = weight;
	}
	return 0;
}

//��������� ������ ��� ������� ��������� � ������ ���������
int allocateMemoryForAdjacentMatrix(void) {
	a = (int**) malloc(sizeof(int*) * N);
	if (a == NULL) return -1;

	for (int i = 0; i < N; i++) {
		a[i] = (int*)malloc(sizeof(int) * N);
		if (a[i] == NULL) return -1;

		for (int j = 0; j < N; j++)
			a[i][j] = -1;	//-1 ������������� ���������� ����� ��� ��������
	}

	visited = (char*)malloc(sizeof(char) * N);
	for (int i = 0; i < N; i++)
		visited[i] = 0;

	return 0;
}

//������� ������ ��� ������� ��������� � ������� ���������
void deallocateMemoryForAdjacentMatrix(void) {
	for (int i = 0; i < N; i++)
		free(a[i]);
	free(a);
	free(visited);
}

//��������������� ����� ��� ������ ��������� �� ������
void finalMessage(const char* msg) {
	printf(msg);
	deallocateMemoryForAdjacentMatrix();
}

//������ �� ���� ��������� ������� � ��������
int getSRequest(void) {
	printf("������� ��������� �������: ");
	int res = scanf("%d", &s);

	if (res != 1) return -1;
	if (s <= 0 || s > N) return -1;

	return s;
}

//������ �� ���� �������� ������� � ��������
int getFRequest(void) {
	printf("������� ��������� �������: ");
	int res = scanf("%d", &v);

	if (res != 1) return -1;
	if (v <= 0 || v > N) return -1;

	return v;
}


#define MAX_PATH_LENGTH 100		//������������ ����� ���� �� ��������� ������� �� ��������
int path[MAX_PATH_LENGTH];		//������, ���������� ���� �� ��������� ������� �� ��������
int pInd = 0;					//������ ��� ���������� ����

int* pointers[MAX_PATH_LENGTH];	//������ ���������, ������ ��������� ����� ��������� �� ������� ������,
								//� ������� ����� ��������� ����� ���� � ��� ��������������� ����
int pp = 0;						//������ ��� ���������� ������� ����������

//����������� ������� ��� ������ � �������
void DFS(int u, int dist) {
	visited[u] = 1;		//�������� u-�� ������� � ������� � � ����
	path[pInd] = u;
	pInd++;
	if (u == v - 1) {	//���������� ������� - ��������
		pointers[pp] = (int*) malloc(sizeof(int) * (pInd + 2));	//�������� ������ ��� ���������
		pointers[pp][0] = dist;									//���������� ������������ ���������
		for (int i = 0; i < pInd; i++)
			pointers[pp][i + 1] = path[i] + 1;					//�������� ���� ���������� ����
		pointers[pp][pInd + 1] = -1;							//������� ��������� ������

		pp++;
	}
	else {
		for (int i = 0; i < N; i++)
			if (i != u && !visited[i] && a[u][i] != -1) {		//������� �� ���� ���� � ����? �� �� �� ��������? ���� � ��� ����������?
				DFS(i, dist + a[u][i]);							//����� ��������� � ���
			}
	}

	pInd--;														//������� ������� �� �����
	visited[u] = 0;												//���������� �� � "��������������"
}

//������� ��� ���������� ���� �����
void mySort(void) {
	for (int i = pp - 1; i >= 0; i--)
		for (int j = 0; j < i; j++)
			if (pointers[j][0] > pointers[j + 1][0]) {
				int* buf = pointers[j];
				pointers[j] = pointers[j + 1];
				pointers[j + 1] = buf;
			}
}

//����������� ������ ��� ��� ����
void deallocateArrays(void) {
	for (int i = 0; i < pp; i++)
		free(pointers[i]);
}

#define INFINITY 1000 //�����-�� ����� ������� ����� (��� ����� ������)
//�������� ������
void FloydAlgorithm(void) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			if (a[i][j] == -1) a[i][j] = INFINITY; //���������� ����� �������� ��������������
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

	//���� ������
	////////////////////////////////////////////////////////////////////////////////////////////
	{
		N = getVertexRequest();
		if (N == -1) {
			printf("������� ������������ ����� ������!\n");
			return 0;
		}

		if (allocateMemoryForAdjacentMatrix() == -1) {
			printf("�� ������� �������� ������ ��� ������� ���������!\n");
			return 0;
		}

		M = getEdgeRequest();
		if (M == -1) {
			finalMessage("������� ������������ ����� ���!\n");
			return 0;
		}

		if (getEdgesRequest() == -1) {
			finalMessage("���� �� ��� ���� ������� �����������!\n");
			return 0;
		}

		clear_buffer();

		s = getSRequest();
		if (s == -1) {
			finalMessage("������� ������������ ��������� �������!\n");
			return 0;
		}

		clear_buffer();

		v = getFRequest();
		if (v == -1) {
			finalMessage("������� ������������ ��������� �������!\n");
			return 0;
		}

		clear_buffer();
	}

	//���������� � ����� ���� �����
	////////////////////////////////////////////////////////////////////////////////////////////
	{
		DFS(s - 1, 0);
		mySort();

		if (pp == 0) printf("�� ���������� ���� ����� ��������� ���������!");
		else {
			printf("��� ��������� ���� �� ��������� ������� � ��������:\n");
			for (int i = 0; i < pp; i++) {
				printf("%d: ", pointers[i][0]);
				for (int j = 1; pointers[i][j] != -1; j++)
					printf("%d ", pointers[i][j]);
				printf("\n");
			}
		}
	}

	//���������� ��������� ������ ��� ���������� ������ �������
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

		printf("����� ������� - ������� %d � ���������������� %d", ind + 1, minEccentricity);

		deallocateMemoryForAdjacentMatrix();
		deallocateArrays();
	}

	return 0;
}