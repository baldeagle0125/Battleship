#include <iostream>
//#include <Windows.h>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <ctype.h>
#include <cctype>
#include <string.h>
#include <math.h>
#include <string>
#include <time.h>
#include <algorithm>
#include <stack>
#include <stdlib.h>
#include <random>
#include <locale.h>
#include <fstream>
#include <sstream>
#include <cstring>
//#include <Config.h>
//#pragma warning(disable : 4996)
//#define _CRT_SECURE_NO_WARNINGS
using namespace std;

const int BOARD_WIDTH = 10; // ширина поля
const int BOARD_HEIGHT = 10; // высота поля
const int SHIP_TYPES = 4; // типы кораблей

const char isWATER = '*'; // если вода - *
const char isHIT = 'X'; // если попадание - Х
const char isSHIP = 'S'; // если корабль - S
const char isMISS = '0'; // промах - 0

struct POINT {
	// координаты
	int X; // x - горизонталь
	int Y; // y - вертикаль
};

struct SHIP {
	// название корабля
	string name;
	// количество точек на поле
	int length;
	// координаты точек
	POINT onGrid[5]; // максимальная длина корабля 4
	// попали ли в эту точку
	bool hitFlag[5];
}ship[SHIP_TYPES];

struct PLAYER {
	char grid[BOARD_WIDTH][BOARD_HEIGHT]; // поле для определенного игрока
}player[3]; // только 2 игрока

enum DIRECTION {HORIZONTAL, VERTICAL}; // перечисляем все точки
struct PLACESHIPS {
	DIRECTION direction;
	SHIP shipType;
};

bool gameRunning = false; // идет ли еще игра (проверка - остались ли не взорваные корабли на поле)

// проверка на проигрыш
bool GameOverCheck(int enemyPLAYER) {
	bool winner = true;
	// проверяем вражеское поле
	for (int w = 0; w < BOARD_WIDTH; w++) {
		for (int h = 0; h < BOARD_HEIGHT; h++) {
			// если еще остались корабли - игра продолжается
			if (player[enemyPLAYER].grid[w][h] = isSHIP) {
        winner = false;
        return winner;
			}
		}
  }
	// если нет - конец игры
	return winner;
}

// ввод координат игрока
bool UserInputAttack(int& x, int& y, int theplayer) {
	cout << "\nPlayer " << theplayer << ", enter coordinates to attack (x, y): ";
	// проверка на правильность ввода
	bool goodInput = false;
	cin >> x >> y;
	if (x < 0 || x >= BOARD_WIDTH) {
    return goodInput;
  }
	if (y < 0 || y >= BOARD_HEIGHT) {
   return goodInput;
  }
  goodInput = true;
	return goodInput;
}

// расставление кораблей игроком
PLACESHIPS UserInputShipPlacement() {
	// d - направление (вертикальное (1) или горизонтальное (0))
	// x - горизонталь
	// y - вертикаль
	int d, x, y;
	PLACESHIPS tmp;
	// если ввод координат неправельный
	tmp.shipType.onGrid[0].X = -1;
	// получение трех координат
	cin >> d >> x >> y;
	if (d != 0 && d != 1) {
    return tmp;
  }
	if (x < 0 || x >= BOARD_WIDTH) {
    return tmp;
  }
  if (y < 0 || y >= BOARD_HEIGHT) {
    return tmp;
  }
	// если ввод правильный
	tmp.direction = (DIRECTION)d;
	tmp.shipType.onGrid[0].X = x;
	tmp.shipType.onGrid[0].Y = y;
	return tmp;
}

// корабли!
void LoadShips() {
	ship[0].name = "4-deck";
	ship[0].length = 4; // 4-палубный

	ship[1].name = "3-deck";
	ship[1].length = 3; // 3-палубный

	ship[2].name = "2-deck";
	ship[2].length = 2; // 2-палубный

	ship[3].name = "1-deck";
	ship[3].length = 1; // 1-палубный
}

// для каждого игрока свое поле
void ResetBoard() {
	// цикл для проверки каждого игрока
	for (int plyr = 1; plyr < 3; plyr++) {
	   // пустые клетки заполняются водой
		for (int w = 0; w < BOARD_WIDTH; w++) {
			for (int h = 0; h < BOARD_HEIGHT; h++) {
				player[plyr].grid[w][h] = isWATER;
		  }
    }
		// переход к следующему игроку
	}
}

// вывод игрового поля
void DrawBoard(int thisPlayer) {
	// Created by Igor
	cout << "Created by Igor" << "\n\n";

	// показывает поле игрока
	cout << "\t";
	cout << "Player " << thisPlayer << "'s game board\n";
  cout << "\n";

	// цикл для верхнего ряда и количества столбцов
	cout << "   ";
	for (int w = 0; w < BOARD_WIDTH; w++) {
		if (w < 10) {
			// длинна в 1 символ и 2 пробела
			cout << w << "  ";
    }
		else if (w >= 10) {
			// длинна в 2 символа и 1 пробел
		  cout << w << " ";
    }
	}
	cout << "\n";

	// отображение поля
	for (int h = 0; h < BOARD_HEIGHT; h++) {
		for (int w = 0; w < BOARD_WIDTH; w++) {
			// если символ длинною в 1 - мы его нумеруем
			if (w == 0) {
        cout << h << " ";
      }
			// если символ длинною в 1 - дописываем пробел чтобы было красиво (setw(2) - можно было использовать)
			if (w < 10 && w == 0) {
				cout << " ";
			}
			// отображение поля (если игра еще не начилась, расставляем корабли)
			if (gameRunning == false) {
				cout << player[thisPlayer].grid[w][h] << "  ";
			}
			// при попадании показываем соответствующий знак
			if (gameRunning == true && player[thisPlayer].grid[w][h] != isSHIP) {
				cout << player[thisPlayer].grid[w][h] << "  ";
			}
			else if (gameRunning == true && player[thisPlayer].grid[w][h] == isSHIP) {
				cout << isWATER << "  ";
			}
			// при достижения краю поля, переносим на следующую строку
			if (w == BOARD_WIDTH - 1) {
				cout << "\n";
			}
		}
	}
}

int main() {
	LoadShips();
	ResetBoard();

	// расстановка кораблей

	// цикл для 2ох игроков
	for (int aplyr = 1; aplyr < 3; aplyr++) {
		// цикл для каждого типа корабля
		for (int thisShip = 0; thisShip < SHIP_TYPES; thisShip++) {
			// отображение своего поля
			system("cls");
			DrawBoard(aplyr);
			// инструкции
			cout << "\n";
			cout << "Instructions for Player " << aplyr << "\n\n";
			cout << "Coords format should be 0 5 4" << endl;
			cout << "First number is directon of the ship (0 - horizontal, 1 - vertical)" << endl;
			cout << "Second number is X (horizontal)" << endl;
			cout << "Thrid number is Y (vertical)" << "\n\n";
			cout << "Place " << ship[thisShip].name << " ship which has a length of " << ship[thisShip].length  << "\n";
			cout << "Where do you want it placed? " << endl;

			cout << "Coords: ";
			// пользователь вводит координаты
			PLACESHIPS aShip;
			aShip.shipType.onGrid[0].X = -1;
			while (aShip.shipType.onGrid[0].X == -1) {
				aShip = UserInputShipPlacement();
			}

			// прравниваем ввод пользователя к нашим кораблям
			aShip.shipType.length = ship[thisShip].length;
			aShip.shipType.name = ship[thisShip].name;

			// первая точка корабля (которую вводит игрок)
			player[aplyr].grid[aShip.shipType.onGrid[0].X][aShip.shipType.onGrid[0].Y] = isSHIP;

			// добавляем корабли в зависимости от ввода игрока
			for (int i = 1; i < aShip.shipType.length; i++)	{
				if (aShip.direction == HORIZONTAL) {
					aShip.shipType.onGrid[i].X = aShip.shipType.onGrid[i-1].X + 1;
					aShip.shipType.onGrid[i].Y = aShip.shipType.onGrid[i-1].Y;
				}
				if (aShip.direction == VERTICAL) {
					aShip.shipType.onGrid[i].Y = aShip.shipType.onGrid[i-1].Y + 1;
					aShip.shipType.onGrid[i].X = aShip.shipType.onGrid[i-1].X;
				}

				// добавляем оставшиеся точки (если на них не расположен корабль)
				player[aplyr].grid[aShip.shipType.onGrid[i].X][aShip.shipType.onGrid[i].Y] = isSHIP;
			}
			// цикл для каждого типа корабля
		}
		// цикл для каждого игрока
	}

	// атака

	// конец ли игры
	gameRunning = true;
	int thisPlayer = 1;
	do {
		// отображение поля противника
		int enemyPlayer;
		if (thisPlayer == 1) {
			enemyPlayer = 2;
		}
		if (thisPlayer == 2) {
			enemyPlayer = 1;
		}
		system("cls");
		DrawBoard(enemyPlayer);

		// получаем координаты выстрела
		bool goodInput = false;
		int x, y;
		while (goodInput == false) {
			goodInput = UserInputAttack(x, y, thisPlayer);
		}

		// проверяем клетку
		// есть ли попадание или нет
		if (player[enemyPlayer].grid[x][y] == isSHIP) {
	 		player[enemyPlayer].grid[x][y] = isHIT;
		}
		if (player[enemyPlayer].grid[x][y] == isWATER) {
			player[enemyPlayer].grid[x][y] = isMISS;
		}

		// проверяем конец ли игры
		int aWin = GameOverCheck(enemyPlayer);
		if (aWin != 0) {
			gameRunning = false;
			break;
		}
		// чередование игроков (тернарный оператор)
		thisPlayer = (thisPlayer == 1) ? 2 : 1;
	} while (gameRunning);

	system("cls");
	cout << "\n\nPlayer " << thisPlayer << " has won the game!\n\n\n";

  cout << "\n";
  system("pause");
  return 0;
}
