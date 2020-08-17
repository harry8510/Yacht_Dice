#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<Windows.h>
#include<algorithm>

const int UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, USE = 4, SELECT = 5;
const int Aces = 1, Deuces = 2, Threes = 3, Fours = 4, Fives = 5, Sixes = 6,
          Choice = 7, Four_of_a_Kind = 8, Full_House = 9, Small_Straight = 10, Large_Straight = 11, Yacht = 12;

CONSOLE_SCREEN_BUFFER_INFO presentCur, presentCur2;

void gotoxy(int x, int y)
{
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void pressKey()
{
	char buffer;
	buffer = getch();
}

int getKey()
{
	char buffer;
	buffer = getch();

	if (buffer == 'w' || buffer == 'W')
		return UP;
	else if (buffer == 's' || buffer == 'S')
		return DOWN;
	else if (buffer == 'a' || buffer == 'A')
		return LEFT;
	else if (buffer == 'd' || buffer == 'D')
		return RIGHT;
	else if (buffer == 'f' || buffer == 'F')
		return SELECT;
	else if (buffer == 'e' || buffer == 'E')
		return USE;
	return 0;
}

class YachtPlayer
{
	int dice[6] = { 0 };
	int Score[13] = { 0 };
	int nowScore[13] = { 0 };
	int checkScore[13] = { 0 };
	int isReroll[6] = { 0 };
	int subtotal = 0;
	int subtotalFlag = 0;

public:

	int total = 0;
	void rollDiceReset();
	void rollDice();
	void showDice();
	void rerollDice();
	void calcExpectedScore();
	void showExpectedScore();
	int  isAbleCategories(int input);
	void selectCategories(int input);

} player;

void YachtPlayer::rollDiceReset()
{
	std::fill_n(isReroll, 6, 1);
}

void YachtPlayer::rollDice()
{
	gotoxy(0, presentCur.dwCursorPosition.Y);
	printf("| Dice 1 | Dice 2 | Dice 3 | Dice 4 | Dice 5 \n");
	for (int i = 1; i <= 10; i++)
	{
		gotoxy(0, presentCur.dwCursorPosition.Y + 1);
		for (int j = 1; j <= 5; j++)
		{
			printf("| %6d ", isReroll[j] ? (rand() % 6 + 1) : dice[j]);
		}
		Sleep(100+10*i);
	}
	for (int i = 1; i <= 5; i++)
	{
		if (isReroll[i])
			dice[i] = rand() % 6 + 1;
	}
	std::fill_n(isReroll, 6, 0);
}

void YachtPlayer::showDice()
{
	gotoxy(0, presentCur.dwCursorPosition.Y);
	printf("| Dice 1 | Dice 2 | Dice 3 | Dice 4 | Dice 5 \n");
	printf("| %6d | %6d | %6d | %6d | %6d\n\n", dice[1], dice[2], dice[3], dice[4], dice[5]);
	Sleep(500);
}

void YachtPlayer::rerollDice()
{
	int input, position = 1;

	printf(" Press F to select, press E to Confirm\n");
	while (1)
	{
		gotoxy(0, presentCur.dwCursorPosition.Y+2);
		for (int i = 1; i <= 5; i++)
		{
			if (position==i)
			{
				printf("| ¡ã     ");
			}
			else
			{
				printf("| ¡â     ");
			}
		}
		for (int i = 1; i <= 5; i++)
		{
			gotoxy(9 * (i - 1) + 2, presentCur.dwCursorPosition.Y + 1);
			if (isReroll[i])
			{
				printf("¡Ü");
			}
			else
			{
				printf("¡Û");
			}
		}
		input = getKey();
		if (input == USE)
			break;
		if (input == SELECT)
		{
			isReroll[position] = isReroll[position] ? 0 : 1;
		}
		if (input == RIGHT)
		{
			position++;
		}
		if (input == LEFT)
		{
			position--;
		}
		position += 5;
		position = position % 5;
		position = !(position) ? 5 : position;
	}
	rollDice();
	std::fill_n(isReroll, 6, 0);
}

void YachtPlayer::calcExpectedScore()
{
	int sumDice[7] = { 0 }, sumCnt[6] = { 0 };
	std::fill_n(nowScore, 13, 0);

	for (int i = 1; i <= 5; i++)
	{
		sumDice[dice[i]]++;
	}
	for (int i = 1; i <= 6; i++)
	{
		sumCnt[sumDice[i]]++;
	}

	nowScore[Aces]   = sumDice[1] * 1;
	nowScore[Deuces] = sumDice[2] * 2;
	nowScore[Threes] = sumDice[3] * 3;
	nowScore[Fours]  = sumDice[4] * 4;
	nowScore[Fives]  = sumDice[5] * 5;
	nowScore[Sixes]  = sumDice[6] * 6;
	nowScore[Choice] = nowScore[Aces] + nowScore[Deuces] + nowScore[Threes] + nowScore[Fours] + nowScore[Fives] + nowScore[Sixes];
	if (sumCnt[5] == 1)
		nowScore[Yacht] = 50;
	if (sumCnt[4] == 1 || sumCnt[5] == 1)
		nowScore[Four_of_a_Kind] = nowScore[Choice];
	if (sumCnt[3] == 1 && sumCnt[2] == 1)
		nowScore[Full_House] = nowScore[Choice];
	if (sumDice[1] * sumDice[2] * sumDice[3] * sumDice[4] != 0 || sumDice[2] * sumDice[3] * sumDice[4] * sumDice[5] != 0 || sumDice[3] * sumDice[4] * sumDice[5] * sumDice[6] != 0)
		nowScore[Small_Straight] = 15;
	if (sumDice[1] * sumDice[2] * sumDice[3] * sumDice[4] * sumDice[5] == 1 || sumDice[2] * sumDice[3] * sumDice[4] * sumDice[5] * sumDice[6] == 1)
		nowScore[Large_Straight] = 30;

	subtotal = Score[Aces] + Score[Deuces] + Score[Threes] + Score[Fours] + Score[Fives] + Score[Sixes];
	if (subtotal >= 63 && subtotalFlag == 0)
	{
		subtotalFlag = 1;
		total += 35;
	}
}

void YachtPlayer::showExpectedScore()
{
	char buf[13][10];
	for (int i = 1; i <= 12; i++)
	{
		if (!isAbleCategories(i))
		{
			itoa(Score[i], buf[i], 10);
			continue;
		}
		buf[i][0] = '-';
		buf[i][1] = '\0';
	}
	printf(" %-19s | %10s | %10s |\n", "Categories", "Your Score", "able Score");
	printf(" %-19s | %10s | %10s |\n", " ", " ", " ");
	printf(" %-19s | %10s | %10d |\n", "01. Aces", buf[Aces], nowScore[Aces]);
	printf(" %-19s | %10s | %10d |\n", "02. Deuces", buf[Deuces], nowScore[Deuces]);
	printf(" %-19s | %10s | %10d |\n", "03. Threes", buf[Threes], nowScore[Threes]);
	printf(" %-19s | %10s | %10d |\n", "04. Fours", buf[Fours], nowScore[Fours]);
	printf(" %-19s | %10s | %10d |\n", "05. Fives", buf[Fives], nowScore[Fives]);
	printf(" %-19s | %10s | %10d |\n", "06. Sixes", buf[Sixes], nowScore[Sixes]);
	printf(" %-19s | %10d / 63    \n", "SubTotal", subtotal);
	printf(" %-19s | %10s | %10d |\n", "07. Choice", buf[Choice], nowScore[Choice]);
	printf(" %-19s | %10s | %10d |\n", "08. Four of a Kind", buf[Four_of_a_Kind], nowScore[Four_of_a_Kind]);
	printf(" %-19s | %10s | %10d |\n", "09. Full House", buf[Full_House], nowScore[Full_House]);
	printf(" %-19s | %10s | %10d |\n", "10. Small Straight", buf[Small_Straight], nowScore[Small_Straight]);
	printf(" %-19s | %10s | %10d |\n", "11. Large Straight", buf[Large_Straight], nowScore[Large_Straight]);
	printf(" %-19s | %10s | %10d |\n", "12. Yacht", buf[Yacht], nowScore[Yacht]);
	printf(" %-19s | %10d         \n", "TotalScore", total);
}

int YachtPlayer::isAbleCategories(int input)
{
	if (checkScore[input])
		return 0;
	return 1;
}

void YachtPlayer::selectCategories(int input)
{
	Score[input] = nowScore[input];
	total += nowScore[input];
	checkScore[input] = 1;
}

void rollFirst(int turn)
{
	printf(" %d / 12 Turn \n\n", turn);
	printf("First Roll\n");
	printf("Press any key to Roll\n\n");
	pressKey();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);
	player.rollDiceReset();
	player.rollDice();
	player.showDice();
	player.calcExpectedScore();
	player.showExpectedScore();
	printf("\n Press any key to Continue \n\n");
	pressKey();
}

void rollSecond(int turn)
{
	printf(" %d / 12 Turn \n\n", turn);
	printf("Second Roll\n");
	printf("Press any key to Reroll\n\n");
	pressKey();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);
	player.showDice();
	player.rerollDice();
	player.showDice();
	player.calcExpectedScore();
	player.showExpectedScore();
	printf("\n Press any key to Continue \n\n");
	pressKey();
}

void rollThird(int turn)
{
	printf(" %d / 12 Turn \n\n", turn);
	printf("Third Roll\n");
	printf("Press any key to Reroll\n\n");
	pressKey();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);
	player.showDice();
	player.rerollDice();
	player.showDice();
	player.calcExpectedScore();
	player.showExpectedScore();
	printf("\n Press any key to Continue \n\n");
	pressKey();
}

void select(int turn)
{
	int input, position = 1;

	printf(" %d / 12 Turn \n\n", turn);
	printf("Time to Select\n");
	printf("Press any key to Select\n\n");
	pressKey();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);
	player.showDice();
	player.calcExpectedScore();
	player.showExpectedScore();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur2);

	printf(" Press F to select\n");
	while (1)
	{
		for (int i = 1; i <= 12; i++)
		{
			gotoxy(50, presentCur.dwCursorPosition.Y + 4 + i + (i>=7));
			if (position == i)
			{
				printf("¢¸");
			}
			else
			{
				printf(" ");
			}
		}
		input = getKey();
		if (input == SELECT)
		{
			if (!player.isAbleCategories(position))
			{
				gotoxy(0, presentCur2.dwCursorPosition.Y+1);
				printf("Already Selected\n");
				continue;
			}
			player.selectCategories(position);
			break;
		}
		if (input == DOWN)
		{
			position++;
		}
		if (input == UP)
		{
			position--;
		}
		position += 12;
		position = position % 12;
		position = !(position) ? 12 : position;
	}
	system("cls");
	printf(" %d / 12 Turn \n\n", turn);
	printf("Time to Select\n");
	printf("Press any key to Select\n\n");
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);
	player.showDice();
	player.calcExpectedScore();
	player.showExpectedScore();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur2);
	printf("\n Press any key to Continue \n\n");
	pressKey();
}

void playYacht()
{
	for (int i = 1; i <= 12; i++)
	{
		system("cls");
		rollFirst(i);
		system("cls");
		rollSecond(i);
		system("cls");
		rollThird(i);
		system("cls");
		select(i);
	}
	printf("YOUR FINAL SCORE is %10d", player.total);
}

void printMainMenu()
{
	system("cls");
	printf("\n\n");
	printf(" ===== YACHT single by metroa =====\n");
	printf(" =====     version 0.0.1      =====\n\n");
	printf("     Press any key to continue     ");
	pressKey();
	playYacht();
}

int main()
{
	srand(time(NULL));
	printMainMenu();
	return 0;
}