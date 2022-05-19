#pragma once
#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <queue>
#include <cmath>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

float WinW = 1280.0;
float WinH = 720.0;
float R = 0.0;
const int maxSize = 20;
int** helpinter;//Вспомогательный массив для оценок пути
int* result;//Массив для пути
int*** inter;//Матрица смежности
int n;


int amount_vertex, amount_rib, source_vertex, target_vertex, rib_weight;
vector<vector<int>> double_adjM_K;


void drawButton(); // Кнопка, предназначенная для выполнения задачи Коммивояжера
void drawCircle(int x, int y, int R); // Построение круга в заданных коорданатах
void drawText(int nom, int x1, int y1); // Вес ребра между двумя вершинами
void MenuText(string text, int x1, int y1);
void drawLine(int text, int x0, int y0, int x1, int y1); // Построение ребра
void drawVertex(int n); // построение вершины
void setCoords(int i, int n); // Постановка координат вершины в зависимости от размера окна
void mouseClick(int button, int state, int x, int y); // Отслеживание действия кнопки
void reshape(int w, int h); // Новый размер окна
void display();

void metod_vetvei_i_granits(int*** inter, int n, int** helpinter, int* path);//Метод ветвей и границ
void preparation(int***& inter, int& n, int**& helpinter, int*& result);
void Min_Path(int*** inter, int n, int** helpinter, int* result);


class Graph {
public:
	int adjMatrix[maxSize][maxSize] = { 0 };
	Graph();
	~Graph() {}
	bool IsEmpty(); // Проверка на пустоту
	bool IsFull(); // проверка на полноту
	void InsertVertex(const int& vertex); // для вставки вершины
	void deleteVertex();
	void InsertEdge(const int& vertex1, const int& vertex2, int weight); // для вставки ребра
	int GetVertPos(const int& vertex); // для возвращения позиции вершины
	void Print(); // Вывод матрицы смежности
	void drawGraph(); // Построение графа
private:
	vector<bool> visit; // вектор посещений
	vector<int> min_ways; // вектор минимальных путей
	vector<int> vert_list; // вектор вершин
};
Graph gr;



struct vertC // Координаты вершин
{
	int x, y;
};
vertC vC[20];

Graph::Graph()
{
	for (int i = 0; i < maxSize; i++)
	{
		for (int j = 0; j < maxSize; j++)
		{
			adjMatrix[i][j] = 0;
		}
	}
}

bool Graph::IsEmpty()
{
	if (vert_list.size() != 0)
		return false;
	else
		return true;
}

bool Graph::IsFull()
{
	return(vert_list.size() == maxSize);
}

void Graph::InsertVertex(const int& vertex)
{
	if (!IsFull())
		vert_list.push_back(vertex);
	else
	{
		cout << "Граф уже заполнен." << endl;
		return;
	}
}
void Graph::deleteVertex() {
	this->vert_list.pop_back();
}

int Graph::GetVertPos(const int& vertex)
{
	for (size_t i = 0; i < vert_list.size(); i++)
	{
		if (vert_list[i] == vertex)
			return i;
	}
	return -1;
}


void Graph::InsertEdge(const int& vertex1, const int& vertex2, int weight)
{
	if (GetVertPos(vertex1) != (-1) && GetVertPos(vertex2) != (-1))
	{
		int vertPos1 = GetVertPos(vertex1);
		int vertPos2 = GetVertPos(vertex2);
		if (adjMatrix[vertPos1][vertPos2] != 0 && adjMatrix[vertPos2][vertPos1] != 0)
		{
			cout << "Ребро между вершинами уже есть" << endl;
			return;
		}
		else
		{
			adjMatrix[vertPos1][vertPos2] = weight;
			adjMatrix[vertPos2][vertPos1] = weight;
		}
	}
	else
	{
		cout << "Обеих вершин(или одной их них) нет в графе" << endl;
		return;
	}
}


void Graph::Print()  // Вывод смежности графа
{
	if (!IsEmpty())
	{
		cout << "Матрица смежности графа: " << endl;
		for (size_t i = 0; i < vert_list.size(); i++)
		{
			cout << vert_list[i] << " ";
			for (size_t j = 0; j < vert_list.size(); j++)
				cout << setw(4) << adjMatrix[i][j];
			cout << endl;
		}
	}
	else
		cout << "Граф пуст" << endl;
}

void metod_vetvei_i_granits(int*** inter, int n, int** helpinter, int* path)//Метод ветвей и границ
{
	for (int l = 0; l < n; l++)//Циклы от кол-ва вершин
	{
		for (int i = 0; i < n; i++)
		{
			int min = 1000000;//Здесь происходит оценка путей по длине и ширине
			for (int j = 0; j < n; j++)
				if (inter[i][j] && min > *inter[i][j])//Сравнивание минимума и длины пути
					min = *inter[i][j];
			for (int j = 0; j < n; j++)
				if (inter[i][j])
					*inter[i][j] -= min;//Декремент длины пути до 0
		}
		for (int j = 0; j < n; j++)//То же самое только для глубины
		{
			int min = 1000000;
			for (int i = 0; i < n; i++)
				if (inter[i][j] && min > *inter[i][j])
					min = *inter[i][j];
			for (int i = 0; i < n; i++)
				if (inter[i][j])
					*inter[i][j] -= min;
		}
		for (int i = 0; i < n; i++)//Цикл занулирования вспомогательного массива
			for (int j = 0; j < n; j++)
				helpinter[i][j] = 0;

		for (int i = 0; i < n; i++)//Цикл для заполнения массива "ценами" путей
			for (int j = 0; j < n; j++)
			{
				if (inter[i][j] && !*inter[i][j])//Если не указатель на массив
				{
					int hmin = 1000000;
					int vmin = 1000000;
					for (int l = 0; l < n; l++)//Цикл от кол-ва вершин
						if (l != i && inter[l][j] && hmin > *inter[l][j])//Если длина пути меньше минимальной по высоте и i не равно l
							hmin = *inter[l][j];//Присваивание указателя на массив со значением пути

					for (int l = 0; l < n; l++)//Цикл от кол-ва вершин
						if (l != j && inter[i][l] && vmin > *inter[i][l])//Если длина пути меньше минимальной по ширине и i не равно l
							vmin = *inter[i][l];//Присваивание указателя на массив со значением пути

					helpinter[i][j] = hmin + vmin;//Заполнение вспомогательного массива "ценами" путей
				}
			}
		int mcost = 0, mi = 0, mj = 0;
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				if (inter[i][j] && mcost < helpinter[i][j])//Если минимальный путь меньше значения в массиве
				{
					mcost = helpinter[i][j];//Присваивание цены пути
					mi = i;//Присваивание индексов для минимального пути
					mj = j;
				}
		path[mi] = mj;//Составление массива мин-го пути
		//Редукция строк и столбцов
		for (int i = 0; i < n; i++)
			inter[i][mj] = nullptr;//Присваивание нулевого указателя по ширине
		for (int i = 0; i < n; i++)
			inter[mi][i] = nullptr;//Присваивание нулевого указателя по высоте
		inter[mj][mi] = nullptr;//Присваивание нулевого указателя по ширине и высоте
	}
}

void preparation(int***& inter, int& n, int**& helpinter, int*& result)//Функция подготовки
{
	n = amount_vertex;//Присваивание кол-ва вершин
	helpinter = new int* [n];//Дин-й вспомогательный массив
	result = new int[n];//Дин-й массив для пути
	inter = new int** [n];//Дин-я матрица смежности
	for (int i = 0; i <= n; i++)//Цикл для инициализации вспомогательного дин-го массива
	{
		helpinter[i] = new int[n];
	}
	for (int i = 0; i < n; i++)//Цикл для инициализации дин-й матрицы
	{
		inter[i] = new int* [n];
		for (int j = 0; j < n; j++)
		{
			if (gr.adjMatrix[i][j] == 0)//Проверка массива на отсутствие дорог и присваиние нулевого указателя
			{
				inter[i][j] = nullptr;
				continue;
			}
			inter[i][j] = new int(gr.adjMatrix[i][j]);//Цикл для инициализации дин-й матрицы
		}
	}
}

void Min_Path(int*** inter, int n, int** helpinter, int* result)//Функция нахождения минимального пути
{
	preparation(inter, n, helpinter, result);//Функция подготовки
	int s = 0;//Переменная для хранения длины мин-го пути
	metod_vetvei_i_granits(inter, n, helpinter, result);//Функция для метода ветвей и границ
	for (int i = 0, j = 0; i < n; i++)
	{
		j = result[i];//Присваивание j номера следующего номера вершины пути
		s += gr.adjMatrix[i][j];//Добавление длины пути
	}
	cout << "\nМинимальный путь: ";//Минимальный путь
	int temp = 0;
	for (int l = 0; l < n;)
	{
		for (int i = 0, j = 0; i < n; i++)
		{
			if (temp == 0 || i + 1 == temp)
			{
				if (temp == 0) cout << i + 1;//Вывод номера начальной вершины
				j = result[i];
				temp = j + 1;
				if (temp > 0)	cout << " -> " << temp;//Вывод пути
				l++;//Инкремент подсчёта кол-ва вершин
			}
		}
	}
	cout << " = " << s;//Вывод длины мин-го пути
	cout << endl;
}
void MenuText(string text, int x1, int y1)//Функция отрисовки текста в меню
{
	string s = text;
	glRasterPos2i(x1 + 5, y1 - 20);
	for (int j = 0; j < s.length(); j++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[j]);
}

void drawButton() { // Поле для кнопки, предназначенной для выполнения алгоритма 
	
	glColor3f(1.9f, 1.9f, 0.7f);

	glBegin(GL_QUADS);
	glVertex2i(150, 150);
	glVertex2i(0, 150);
	glVertex2i(0, 30);
	glVertex2i(150, 30);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2i(150, 30);
	glVertex2i(150, 150);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2i(0, 30);
	glVertex2i(150, 30);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2i(0, 60);
	glVertex2i(150, 60);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2i(0, 90);
	glVertex2i(150, 90);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2i(0, 120);
	glVertex2i(150, 120);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2i(0, 150);
	glVertex2i(150, 150);
	glEnd();

	glColor3d(0, 0, 0);
	MenuText("Dobavit vershinu", 0, 150);
	glColor3d(0, 0, 0);
	MenuText("Udalit vershinu", 0, 120);
	glColor3d(0, 0, 0);
	MenuText("Matrix", 0, 90);
	glColor3d(0, 0, 0);
	MenuText("Task solution", 0, 60);
}

void drawCircle(int x, int y, int R) // Прорисовка круга
{
	glColor3f(1, 0, 0);
	float x1, y1;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(360);
		y1 = R * cos(theta) + y;
		x1 = R * sin(theta) + x;;
		glVertex2f(x1, y1);
	}
	glEnd();

	glColor3f(0.0f, 0.1f, 0.1f);
	float x2, y2;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(360);
		y2 = R * cos(theta) + y;
		x2 = R * sin(theta) + x;
		glVertex2f(x2, y2);
	}
	glEnd();
}

void drawText(int nom, int x1, int y1) // Текст
{
	GLvoid* font = GLUT_BITMAP_HELVETICA_18;
	string s = to_string(nom);
	glRasterPos2i(x1 - 5, y1 - 5);
	for (size_t j = 0; j < s.length(); j++)
		glutBitmapCharacter(font, s[j]);
}

void drawLine(int text, int x0, int y0, int x1, int y1) // Линии
{
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2i(x0, y0);
	glVertex2i(x1, y1);
	glEnd();

	drawText(text, (x0 + x1) / 2 + 10, (y0 + y1) / 2 + 10);
}

void drawVertex(int n) // Построение вершины
{
	for (int i = 0; i < n; i++)
	{
		drawCircle(vC[i].x, vC[i].y, R);
		drawText(i + 1, vC[i].x, vC[i].y);
	}
}

void setCoords(int i, int n) // Установка координат
{
	int R_;
	int x0 = WinW / 2;
	int y0 = WinH / 2;
	if (WinW > WinH)
	{
		R = 5 * (WinH / 13) / n;
		R_ = WinH / 2 - R - 10;
	}
	else
	{
		R = 5 * (WinW / 13) / n;
		R_ = WinW / 2 - R - 10;
	}
	float theta = 2.0f * 3.1415926f * i / n;
	int y1 = R_ * cos(theta) + y0;
	int x1 = R_ * sin(theta) + x0;

	vC[i].x = x1;
	vC[i].y = y1;
}

void Graph::drawGraph() // Рисование созданного заранее графа
{
	int n = vert_list.size();
	for (int i = 0; i < n; i++)
	{
		setCoords(i, n); // Установка координат
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			int a = adjMatrix[i][j];
			if (a != 0)
			{
				drawLine(a, vC[i].x, vC[i].y, vC[j].x, vC[j].y); // Ребро
			}
		}
	}
	drawVertex(n); // вершина
}

void mouseClick(int btn, int stat, int x, int y)//Функция для действий прик клике мыши
{
	if (stat == GLUT_DOWN) //Проверка на нажатие
	{
		if (x > 0 && x < 150 && y > 650 && y < 680)//Проверка на координаты положения мыши
		{
			int sourceVertex;
			int targetVertex;
			int edgeWeight;


			cout << "Начальная вершина: "; cin >> sourceVertex; cout << endl;//Ввод начальной вершины
			int* sourceVertPtr = &sourceVertex;//Присваивание указателю адреса начальной вершины
			cout << "Конечная вершина: "; cin >> targetVertex; cout << endl;//Ввод конечной вершины
			int* targetVertPtr = &targetVertex;//Присваивание указателю адреса конечной вершины

			if (sourceVertex > amount_vertex || targetVertex > amount_vertex) //Вставка новой вершины если номер конечной или начальной больше кол-ва вершин
			{
				amount_vertex++;//Инкремент переменной для кол-ва вершин
				int* vertPtr = &amount_vertex;//Присваивание указателю адреса новой вершины
				gr.InsertVertex(*vertPtr);//Функция вставки вершины
			}

			cout << "Вес ребра: "; cin >> edgeWeight; cout << endl;//Ввод веса ребра
			gr.InsertEdge(*sourceVertPtr, *targetVertPtr, edgeWeight); //Вызов функции для вставки ребра
		}
		if (x > 0 && x < 150 && y > 680 && y < 710)//Проверка на координаты положения мыши
		{
			int sourceVertex;

			cout << "Номер вершины: "; cin >> sourceVertex; cout << endl;//Ввод номера вершины для удаления
			int* sourceVertPtr = &sourceVertex;//Присваивание указателю адреса удаляемой вершины
			if (sourceVertex == amount_vertex)//Если удаляемая вершина = кол-ву вершин
			{
				amount_vertex--;//Декремент кол-ва вершинё
				gr.deleteVertex();//Функция удаления вершины
			}
			else cout << "Невозможно удалить вершину \n";//Невозможно удалить вершину
		}
		if (x > 0 && x < 150 && y >  710 && y < 740)
			gr.Print();
		if (x > 0 && x < 150 && y >  740 && y < 770)//Проверка на координаты положения мыши
		{
			Min_Path(inter, n, helpinter, result);
		}
	}
	glutPostRedisplay();
}

void setGraph() { // Создание графа
	cout << "Количество вершин: "; cin >> amount_vertex; cout << endl;
	cout << "Количество рёбер: "; cin >> amount_rib; cout << endl;
	for (int i = 1; i <= amount_vertex; i++)
	{
		int* vertPtr = &i;
		gr.InsertVertex(*vertPtr);
	}
	for (int i = 0; i < amount_rib; i++)
	{
		cout << "Начальная вершина: "; cin >> source_vertex; cout << endl;
		int* source_vertPtr = &source_vertex;
		cout << "Конечная вершина: "; cin >> target_vertex; cout << endl;
		int* target_vertPtr = &target_vertex;
		cout << "Вес: "; cin >> rib_weight; cout << endl;
		gr.InsertEdge(*source_vertPtr, *target_vertPtr, rib_weight);
	}
	cout << endl;
	gr.Print();
}

void change(int w, int h)// Установка новых размеров окна
{
	WinW = w;
	WinH = h;

	glViewport(0, 0, (GLsizei)WinW, (GLsizei)WinH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, (GLdouble)WinW, 0, (GLdouble)WinH);
	glutPostRedisplay();
}

void display() // Установка новых систем координат
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WinW, 0, WinH);
	glViewport(0, 0, WinW, WinH);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	drawButton();
	gr.drawGraph();

	glutSwapBuffers();
}
