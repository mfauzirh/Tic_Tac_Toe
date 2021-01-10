#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* =====================================*/
/*				UDT						*/
/* =====================================*/

typedef struct
{
	int row;
	int col;
} MoveFormat;

typedef struct
{
	char NameP1[12];
	char NameP2[12];
} PlayerName;

/* =====================================*/
/*				Deklarasi Modul			*/
/* =====================================*/
void Draw3x3Board(char**);
void Draw5x5Board(char**);
void Draw7x7Board(char**);
void DrawBoard(char **board, int boardSize);

char** CreateBoard(int boardSize);
int StreakRule(int boardSize);
int CheckHorizontal(char **board, int boardSize);
int CheckVertical(char **board, int boardSize);
int CheckMainDiagonal(char **board, int boardSize);
int CheckSecDiagonal(char **board, int boardSize);
int CheckWin(char **board, int boardSize);
void MakeMove(char **board, int boardSize, int *currentPlayer, int gameMode);
void GetUserInput(char **board, MoveFormat *playerMove,  int *currentPlayer, int boardSize);
void GetComputerInput(MoveFormat *computerMove, int *currentPlayer);
int isValidInput(char **board, MoveFormat *playerMove, int BoardSize);
void FillBoard(char **board, MoveFormat Move, int boardSize, int *currentPlayer);
void InputName(PlayerName *playerName);

/* =====================================*/
/*				Modul Utama				*/
/* =====================================*/
// ini merupakan modul utama program

int main()
{
	char **board;
	int gameMode = 1;
	int boardSize = 3, col, row, currentPlayer = 1, isWin = 0, maxRound, roundPlayed = 0;
	PlayerName playerName;
	
	/* Time variabel */
	clock_t elapsedTime, timer;
	int timeConsume;
	
	board = CreateBoard(boardSize);
	maxRound = boardSize * boardSize;
	
	InputName(&playerName);
	
	// Game Start
	elapsedTime = clock();
	while(roundPlayed != maxRound)
	{
		DrawBoard(board, boardSize);
		isWin = CheckWin(board, boardSize);
		if(isWin > 0)
			break;
		MakeMove(board, boardSize, &currentPlayer, gameMode);
		roundPlayed++;
	}
	
	elapsedTime = clock() - elapsedTime;
	timeConsume = elapsedTime / (CLOCKS_PER_SEC);
	// Game End
	
	DrawBoard(board, boardSize);
	if(isWin == 0)
		printf("there is no winner\n");
	else
	{
		if(currentPlayer == 2)
		{	
			printf("winner is player 1 : %s\n", playerName.NameP1);
			printf("with time to complete : %d", timeConsume);	
		}
		else if(currentPlayer == 1)
		{	
			printf("winner is player 2 : %s\n", playerName.NameP2);
			printf("with time to complete : %d", timeConsume);	
		}
	}
	
	return 0;
}

/* =====================================*/
/*				Modul Tampilan			*/
/* =====================================*/
// Dibawah ini merupakan kumpulan modul-modul untuk mengurus bagian tampilan

/* Modul untuk membuat tampilan papan 3x3 */
void Draw3x3Board(char**board)
{
	system("cls");
	printf("\n  %c | %c | %c \n",board[0][0],board[0][1],board[0][2]);
    printf(" ---+---+---\n");
    printf("  %c | %c | %c \n",board[1][0],board[1][1],board[1][2]);
    printf(" ---+---+---\n");
    printf("  %c | %c | %c \n",board[2][0],board[2][1],board[2][2]);
    printf("\n\n");
}

/* Modul untuk membuat tampilan papan 5x5 */
void Draw5x5Board(char**board)
{
	system("cls");
	printf("\n  %c | %c | %c | %c | %c \n",board[0][0],board[0][1],board[0][2],board[0][3],board[0][4]);
    printf(" ---+---+---+---+---\n");
    printf("  %c | %c | %c | %c | %c \n",board[1][0],board[1][1],board[1][2],board[1][3],board[1][4]);
    printf(" ---+---+---+---+---\n");
    printf("  %c | %c | %c | %c | %c \n",board[2][0],board[2][1],board[2][2],board[2][3],board[2][4]);
    printf(" ---+---+---+---+---\n");
    printf("  %c | %c | %c | %c | %c \n",board[3][0],board[3][1],board[3][2],board[3][3],board[3][4]);
    printf(" ---+---+---+---+---\n");
    printf("  %c | %c | %c | %c | %c \n",board[4][0],board[4][1],board[4][2],board[4][3],board[4][4]);
    
    printf("\n\n");
}

/* Modul untuk membuat tampilan papan 7x7 */
void Draw7x7Board(char**board)
{
	system("cls");
	printf("\n  %c | %c | %c | %c | %c | %c | %c \n",board[0][0],board[0][1],board[0][2],board[0][3],board[0][4], board[0][5],board[0][6]);
    printf(" ---+---+---+---+---+---+---\n");
    printf("  %c | %c | %c | %c | %c | %c | %c \n",board[1][0],board[1][1],board[1][2],board[1][3],board[1][4], board[1][5],board[1][6]);
    printf(" ---+---+---+---+---+---+---\n");
    printf("  %c | %c | %c | %c | %c | %c | %c \n",board[2][0],board[2][1],board[2][2],board[2][3],board[2][4], board[2][5],board[2][6]);
    printf(" ---+---+---+---+---+---+---\n");
    printf("  %c | %c | %c | %c | %c | %c | %c \n",board[3][0],board[3][1],board[3][2],board[3][3],board[3][4], board[3][5],board[3][6]);
    printf(" ---+---+---+---+---+---+---\n");
    printf("  %c | %c | %c | %c | %c | %c | %c \n",board[4][0],board[4][1],board[4][2],board[4][3],board[4][4], board[4][5],board[4][6]);
    printf(" ---+---+---+---+---+---+---\n");
    printf("  %c | %c | %c | %c | %c | %c | %c \n",board[5][0],board[5][1],board[5][2],board[5][3],board[5][4], board[5][5],board[5][6]);
    printf(" ---+---+---+---+---+---+---\n");
    printf("  %c | %c | %c | %c | %c | %c | %c \n",board[6][0],board[6][1],board[6][2],board[6][3],board[6][4], board[6][5],board[6][6]);
    
    printf("\n\n");
}

/* modul untuk menampilkan papan permainan */
void DrawBoard(char **board, int boardSize)
{
	if(boardSize == 3)
		Draw3x3Board(board);
	else if(boardSize == 5)
		Draw5x5Board(board);
	else if(boardSize == 7)
		Draw7x7Board(board);
}

/* =====================================*/
/*				Modul Logika			*/
/* =====================================*/
// dibawah ini merupakan kumpulan modul-modul untuk mengurus logika program

/* Ini adalah modul untuk membuat papan permainan */
char** CreateBoard(int boardSize)
{
	char **board;
	int i, j;
	
	/* Mengalokasikasin memori untuk papan permainan */
	board = malloc(sizeof(char*) * boardSize);
	for(i = 0; i < boardSize; i++)
	{
		board[i] = malloc(sizeof(char) * boardSize);
		/* menginisialisasikan papan kosong / berisi char spasi */
		for(j = 0; j < boardSize; j++)
		{
			board[i][j] = ' ';
		}
	}
	
	return board;
}

/* modul untuk menentukan syarat streak */
int StreakRule(int boardSize)
{
	int numberToStreak;
	if(boardSize == 3)
		numberToStreak = boardSize - 1;
	else if(boardSize == 5)
		numberToStreak = boardSize - 2;
	else if(boardSize == 7)
		numberToStreak = boardSize - 3;
	
	return numberToStreak;
}

/* Modul untuk memeriksa kemenangan secara horizontal */
int CheckHorizontal(char **board, int boardSize)
{
	int i, j, count = 0;
	int numberToStreak = StreakRule(boardSize);
	for(i = 0; i < boardSize; i++)
	{
		for(j = 1; j < boardSize; j++)
		{
			/* memeriksa apakah kotak tidak kosong */
			if(board[i][j-1] != ' ')
			{
				/* memeriksa apakah kotak memiliki simbol yang samas */
				if(board[i][j-1] == board[i][j])
					count++;
			}
		}
		
		/* jika simbol streak, maka menang */
		if(count >= numberToStreak)
			return 1;
		else
			count = 0;
	}
	/* jika tidak ada yang sama, belum ada pemenang */
	return 0;
}


/* Modul untuk memeriksa kemenangan secara vertical */
int CheckVertical(char **board, int boardSize)
{
	int i, j, count = 0;
	int numberToStreak = StreakRule(boardSize);
	for(i = 0; i < boardSize; i++)
	{
		for(j = 1; j < boardSize; j++)
		{
			/* memeriksa apakah kotak tidak kosong */
			if(board[j-1][i] != ' ')
			{
				/* memeriksa apakah kotak memiliki simbol yang samas*/
				if(board[j-1][i] == board[j][i])
					count++;
			}
		}
		
		/* jika simbol streak, maka menang */
		if(count >= numberToStreak)
			return 1;
		else
			count = 0;
	}
	/* jika tidak ada yang sama, belum ada pemenang */
	return 0;
}

/* Memeriksa kemengangan di diagonal Utama */
int CheckMainDiagonal(char **board, int boardSize)
{
	int i, j, count = 0;
	for(i = 1; i < boardSize; i++)
	{
		for(j = 1; j < boardSize; j++)
		{
			/* cek apakah sedang dalam kotak diagonal */
			if(i == j && board[i][j] != ' ')
			{
				/* cek apakah kotak diagonal sama dengan kotak diagonal sebelumnnya */
				if(board[i - 1][j - 1] == board[i][j])
					count++;
			}
			
		}
	}
	
	/* check streak */
	if(count >= StreakRule(boardSize))
		return 1;
	else
		count = 0;
	
	/* jika tidak ada yang sama, belum ada pemenang */
	return 0;
}

/* Memeriksa kemenangan di diagonal sekunder */
int CheckSecDiagonal(char **board, int boardSize)
{
	int i, j, count = 0;
	
	for(i = 1; i < boardSize; i++)
	{
		for(j = 0; j < boardSize; j++)
		{
			/* cek apakah sedang dalam kotak diagonal sekunder */
			if(i + j == boardSize - 1 && board[i][j] != ' ')
			{
				/* cek apakah kotak diagonal sama dengan kotak diagonal sebelumnnya */
				if(board[i - 1][j + 1] == board[i][j])
					count++;
			}
		}
	}
	
	/* check streak */
	if(count >= StreakRule(boardSize))
		return 1;
	else
		count = 0;
	
	/* jika tidak ada yang sama, belum ada pemenang */
	return 0;
}

/* Modul untuk men-cek kemenangan */
int CheckWin(char **board, int boardSize)
{
	/* mencek kemenangan secara horizontal, vertical dan diagonal */
	if(CheckHorizontal(board, boardSize) || CheckVertical(board, boardSize) || CheckMainDiagonal(board, boardSize) || CheckSecDiagonal(board, boardSize))
		return 1;
	else
		return 0;
}

void MakeMove(char **board, int boardSize, int *currentPlayer, int gameMode)
{
	/* Get Input */
	MoveFormat Move;
	
	if(gameMode == 1)
	{
		GetUserInput(board, &Move, currentPlayer, boardSize);
	}
	else if(gameMode == 2)
	{
		if(*currentPlayer == 1)
			GetUserInput(board, &Move, currentPlayer, boardSize);
		else if(*currentPlayer == 2)
			GetComputerInput(&Move, currentPlayer);	
	}
	
	/* Fill to Board */
	FillBoard(board, Move, boardSize, currentPlayer);
}

void GetUserInput(char **board, MoveFormat *playerMove, int *currentPlayer, int boardSize)
{
	int isValid = 1;
	do
	{
		if(isValid == 0)
		{
			DrawBoard(board, boardSize);
			printf("Inputan tidak valid, harap masukan kembali.\n\n");
		}
		printf("Giliran player %d. \n", *currentPlayer);
		printf("Masukan baris : "); scanf("%d", &playerMove->row);
		printf("Masukan kolom : "); scanf("%d", &playerMove->col);
		isValid = isValidInput(board, playerMove, boardSize);
	}while(isValid == 0);
}

void GetComputerInput(MoveFormat *computerMove, int *currentPlayer)
{
	
}

int isValidInput(char **board, MoveFormat *playerMove, int boardSize)
{
	if((playerMove->row < 0 || playerMove->row >= boardSize ) && (playerMove->col < 0 || playerMove->col >= boardSize))
		return 0;
	else
	{
		if(board[playerMove->row][playerMove->col] != ' ')
			return 0;
		else
			return 1;
	}
}

void FillBoard(char **board, MoveFormat Move, int boardSize, int *currentPlayer)
{
	if(Move.row < boardSize && Move.col < boardSize && board[Move.row][Move.col] == ' ')
	{
		if(*currentPlayer == 1)
		{
			board[Move.row][Move.col] = 'X';
			*currentPlayer = 2;
		}	
		else
		{
			board[Move.row][Move.col] = 'O';
			*currentPlayer = 1;
		}	
					
	}
}

void InputName(PlayerName *playerName)
{
	printf("Input Player 1 Name : ");
	fgets(playerName->NameP1, 12, stdin);
	printf("Input Player 2 Name  : ");
	fgets(playerName->NameP2, 12, stdin);
}



