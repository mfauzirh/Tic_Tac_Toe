#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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

/* Modul Tampilan */
void MainMenuUI();
void HighscoreUI();
void GameModeUI();
void HelpUI();
void InputNameUI();
void ChooseBoardUI();
void WinnerUI(char winner[], int timeConsume);
void TieUI();
void Draw3x3Board(char**);
void Draw5x5Board(char**);
void Draw7x7Board(char**);
void DrawBoard(char **board, int boardSize);

/* Modul Logika */
void MainMenu(int *choice);
void GameMode(int *gameMode);
void ChooseBoard(int *boardSize);
void GameOver(int currentPlayer, PlayerName playerName, int roundPlayed, int maxRound, int timeConsume);
char** CreateBoard(int boardSize);
int StreakRule(int boardSize);
int CheckHorizontal(char **board, int boardSize);
int CheckVertical(char **board, int boardSize);
int CheckMainDiagonal(char **board, int boardSize);
int CheckSecDiagonal(char **board, int boardSize);
int CheckWin(char **board, int boardSize);
void MakeMove(char **board, int boardSize, int *currentPlayer, int gameMode);
void GetUserInput(char **board, MoveFormat *playerMove,  int *currentPlayer, int boardSize);
void GetComputerInput(char **board, MoveFormat *computerMove, int *currentPlayer, int boardSize);
int isValidInput(char **board, MoveFormat *playerMove, int BoardSize);
void FillBoard(char **board, MoveFormat Move, int boardSize, int *currentPlayer);
void InputName(PlayerName *playerName);
void DeleteBoard(char **board, int boardSize);


/* =====================================*/
/*				Modul Utama				*/
/* =====================================*/
// ini merupakan modul utama program

int main()
{
	/* App Variabel */
	int choice, gameMode = 1;
	PlayerName playerName;
	int gameOver = 0;
	
	/* Game Variabel */
	char **board;
	int boardSize = 3, col, row, currentPlayer = 1, isWin = 0, maxRound, roundPlayed = 0;
	char winner[12];
	
	/* Time variabel */
	clock_t elapsedTime, timer;
	int timeConsume;
	
	while(gameOver == 0)
	{
		currentPlayer = 1;
		roundPlayed = 0;
		/* Halaman Menu */
		MainMenu(&choice);
		
		GameMode(&gameMode);
		
		/* Clear \n after scanf */
		while ( (choice = getchar()) != '\n' && choice != EOF );
		
		InputName(&playerName);
		
		ChooseBoard(&boardSize);
		
		board = CreateBoard(boardSize);
		maxRound = boardSize * boardSize;
		
		
		
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
		
		DrawBoard(board, boardSize);
		printf("Game Over, press any key to continue : ");getch();
		
		
		GameOver(currentPlayer, playerName, roundPlayed, maxRound, timeConsume);
		scanf("%d", &choice);
		switch(choice)
		{
			case 1: exit(0); break;
			case 2: break;
		}
		
		DeleteBoard(board, boardSize);
	}
	
	
	return 0;
}

/* =====================================*/
/*				Modul Tampilan			*/
/* =====================================*/
// Dibawah ini merupakan kumpulan modul-modul untuk mengurus bagian tampilan

/* Modul untuk membuat tampilan papan 3x3 */

void MainMenuUI()
{
	system("CLS");
	printf("\t      <<==========>>\n");
	printf("\t|====               ====|\n");
	printf("\t|      TIC TAC TOE      |\n");
	printf("\t|====               ====|\n");
	printf("\t      <<== Lite ==>>\n\n");
	
	printf("       =============================\n");
	printf("      ||Press '1' To Play The Game ||\n");
	printf("      ||Press '2' To Look Highscore||\n");
	printf("      ||Press '3' To Exit The Game ||\n");
    printf("      ||Press '4' To Help          ||\n");
    printf("       =============================\n");
	printf("\nYour Choice : ");
}

void HighscoreUI()
{
	system("CLS");
	printf("\t      <<==========>>\n");
	printf("\t|====               ====|\n");
	printf("\t|       HIGHSCORE       |\n");
	printf("\t|====               ====|\n");
	printf("\t      <<== User ==>>\n\n");
	printf("John : 5 Minutes to clear\n\n");
	printf("enter any key to go back : ");
	getch();
}

void GameModeUI()
{
	system("CLS");
	printf("\t      <<============>>\n");
	printf("\t|====                 ====|\n");
	printf("\t|        Game Mode        |\n");
	printf("\t|====                 ====|\n");
	printf("\t      <<== Choose ==>>\n\n");
	
	printf("       ============================\n");
	printf("      ||[1] For Player Vs Player  ||\n");
	printf("      ||[2] For Player Vs Computer||\n");
	printf("       ============================\n");
	printf("\nYour Choice : ");
}

void HelpUI()
{
	system("CLS");
	printf("\t     <<============>>\n");
	printf("\t|====                ====|\n");
	printf("\t|          Help          |\n");
	printf("\t|====                ====|\n");
	printf("\t      <<== Choose ==>>\n\n");

	printf("       =========================================================================================\n");
	printf("      ||Di dalam game ini terdapat 2 Pemain Yaitu X dan O.                                     ||\n");
	printf("      ||Cara mainnya adalah setiap pemain meletakkan nilai X atau O pada 9 kolom yang tersedia.||\n");
	printf("       =========================================================================================\n");

	printf("       ================================\n");
	printf("      ||Press '1' To Next\n           ||");
	printf("      ||Press '2' To Back to main menu||\n");
	printf("       ================================\n");
	printf("\nYour Choice : ");
	
	system("CLS");
	printf("\t     <<============>>\n");
	printf("\t|====                ====|\n");
	printf("\t|          Help          |\n");
	printf("\t|====                ====|\n");
	printf("\t      <<== Choose ==>>\n\n");

	printf("       ===========================================================\n");
	printf("      ||Ketika index untuk meletakkan X atau O (ex: 1, 3, atau 5)||");
	printf("       ===========================================================\n");

	printf("       ================================\n");
	printf("      ||Press '1' To Next             ||\n");
	printf("      ||Press '2' To Previous         ||\n");
	printf("      ||Press '3' To Back to main menu||\n");
	printf("       ================================\n");
	printf("\nYour Choice : ");
	system("CLS");
	printf("\t     <<============>>\n");
	printf("\t|====                ====|\n");
	printf("\t|          Help          |\n");
	printf("\t|====                ====|\n");
	printf("\t      <<== Choose ==>>\n\n");

	printf("       =====================================================================================================================\n");
	printf("      ||Untuk memenangkan permainan kamu harus dapat membentuk nilai X atau O berbentuk vertikal, horizontal, atau diagonal||");
	printf("       =====================================================================================================================\n");
	
	printf("       ================================\n");
	printf("      ||Press '1' To Previous         ||\n");
	printf("      ||Press '2' To Back to main menu||\n");
	printf("       ================================\n");
	printf("\nYour Choice : ");
}

void InputNameUI()
{
	system("CLS");
	printf("\t     <<=================>>\n");
	printf("\t|====                      ====|\n");
	printf("\t|      Game Configuration      |\n");
	printf("\t|====                      ====|\n");
	printf("\t      <<== Only 8 char ==>>\n\n");
}

void ChooseBoardUI()
{
	system("CLS");
	printf("\t        <<============>>\n");
	printf("\t|====                     ====|\n");
	printf("\t|      Choose Board Size      |\n");
	printf("\t|====                     ====|\n");
	printf("\t         <<== Choose ==>>\n\n");
	
	printf("            ========================\n");
	printf("           ||Press '3' For 3x3 Size||\n");
	printf("           ||Press '5' For 5x5 Size||\n");
	printf("           ||Press '7' For 7x7 Size||\n");
	printf("            ========================\n");
	printf("\nYour Choice : ");
}

void WinnerUI(char winner[], int timeConsume)
{
	system("CLS");
	printf("\t      <<============>>\n");
	printf("\t|====                  ====|\n");
	printf("\t|         Gameover         |\n");
	printf("\t|====                  ====|\n");
	printf("\t      <<== winner ==>>\n\n");
	
	printf("       =============================================================\n");
	printf("           %s Memenangkan permainan dengan waktu %d detik ++>\n", winner, timeConsume);
	printf("       =============================================================\n");
	
	printf("       ================================\n");
	printf("      ||Press '1' To Exit             ||\n");
	printf("      ||Press '2' To Back to main menu||\n");
	printf("       ================================\n");
	printf("\nYour Choice : ");
}

void TieUI()
{
	system("CLS");
	printf("\t      <<============>>\n");
	printf("\t|====                  ====|\n");
	printf("\t|         Gameover         |\n");
	printf("\t|====                  ====|\n");
	printf("\t      <<==   Tie   ==>>\n\n");
	
	printf("       =========================       \n");
	printf("           Tidak ada pemenang		   \n");
	printf("       =========================       \n");
	
	printf("       ================================\n");
	printf("      ||Press '1' To Exit             ||\n");
	printf("      ||Press '2' To Back to main menu||\n");
	printf("       ================================\n");
	printf("\nYour Choice : ");
}

void Draw3x3Board(char**board)
{
    system("cls");
    printf("\n\n\tTic Tac Toe\n\n");
    printf("Player 1 (X) -- Player 2 (O)\n\n");
    printf(" _________________\n");
    printf("|     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |\n",board[0][0],board[0][1],board[0][2]);
    printf("|_____|_____|_____|\n");
    printf("|     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |\n",board[1][0],board[1][1],board[1][2]);
    printf("|_____|_____|_____|\n");
    printf("|     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |\n",board[2][0],board[2][1],board[2][2]);
    printf("|_____|_____|_____|\n");
    printf("\n\n");
}

/* Modul untuk membuat tampilan papan 5x5 */
void Draw5x5Board(char**board)
{
    system("cls");
    printf("\n\n\tTic Tac Toe\n\n");
    printf("Player 1 (X) -- Player 2 (O)\n\n");
    printf(" _____________________________\n");
    printf("|     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |\n",board[0][0],board[0][1],board[0][2],board[0][3],board[0 ][4]);
    printf("|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |\n",board[1][0],board[1][1],board[1][2],board[1][3],board[1][4]);
    printf("|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |\n",board[2][0],board[2][1],board[2][2],board[2][3],board[2][4]);
    printf("|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |\n",board[3][0],board[3][1],board[3][2],board[3][3],board[3][4]);
    printf("|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |\n",board[4][0],board[4][1],board[4][2],board[4][3],board[4][4]);
    printf("|_____|_____|_____|_____|_____|\n");
    printf("\n\n");
}

/* Modul untuk membuat tampilan papan 7x7 */
void Draw7x7Board(char**board)
{
    system("cls");
    printf("\n\n\tTic Tac Toe\n\n");
    printf("Player 1 (X) -- Player 2 (O)\n\n");
    printf(" _________________________________________\n");
    printf("|     |     |     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |\n",board[0][0],board[0][1],board[0][2],board[0][3],board[0][4], board[0][5],board[0][6]);
    printf("|_____|_____|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |\n",board[1][0],board[1][1],board[1][2],board[1][3],board[1][4], board[1][5],board[1][6]);
    printf("|_____|_____|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |\n",board[2][0],board[2][1],board[2][2],board[2][3],board[2][4], board[2][5],board[2][6]);
    printf("|_____|_____|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |\n",board[3][0],board[3][1],board[3][2],board[3][3],board[3][4], board[3][5],board[3][6]);
    printf("|_____|_____|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |\n",board[4][0],board[4][1],board[4][2],board[4][3],board[4][4], board[4][5],board[4][6]);
    printf("|_____|_____|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |\n",board[5][0],board[5][1],board[5][2],board[5][3],board[5][4], board[5][5],board[5][6]);
    printf("|_____|_____|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |\n",board[6][0],board[6][1],board[6][2],board[6][3],board[6][4], board[6][5],board[6][6]);
    printf("|_____|_____|_____|_____|_____|_____|_____|\n");
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

void MainMenu(int *choice)
{
	do
	{
		MainMenuUI();
		scanf("%d", choice);
		switch(*choice)
		{
			case 1: break;
			case 2: HighscoreUI(); break;
			case 3: exit(0); break;
			case 4: HelpUI();
		}
	}while(*choice != 1);
}

void GameMode(int *gameMode)
{
	do
	{
		GameModeUI();
		scanf("%d", gameMode);
		switch(*gameMode)
		{
			case 1: *gameMode = 1; break;
			case 2: *gameMode = 2; break;
			default: *gameMode = 0; break;
		}
	}while(*gameMode == 0);
}

void ChooseBoard(int *boardSize)
{
	do
	{
		ChooseBoardUI();
		scanf("%d", boardSize);
		switch(*boardSize)
		{
			case 3: *boardSize = 3; break;
			case 5: *boardSize = 5; break;
			case 7: *boardSize = 7; break;
			default: *boardSize = 0; break;
		}
	}while(*boardSize == 0);
}

void GameOver(int currentPlayer, PlayerName playerName, int roundPlayed, int maxRound, int timeConsume)
{
	char winner[12];
	if(roundPlayed != maxRound)
	{
		if(currentPlayer == 1)
			strcpy(winner, playerName.NameP2);
		else if(currentPlayer == 2)
			strcpy(winner, playerName.NameP1);
		
		WinnerUI(winner, timeConsume);
	}else if(roundPlayed == maxRound)
		TieUI();
}

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
	int i, j, count = 0, diagonalNum = 1;
	/*Check Secondary 0*/
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
	
	if(boardSize == 3)
		return 0;
	
	/* Check secondary diagonal +1 */
	for(i = 0; i < boardSize - 2; i++)
	{
		for(j = 0; j < boardSize - 2; j++)
		{
			if(i == j)
			{
				if(board[i][j+1] != ' ')
				{
					if(board[i][j + 1] == board[i + 1][j + 2])
						count++;
				}
				
			}
		}
	}
	
	if(count >= StreakRule(boardSize))
		return 1;
	else
		count = 0;
		
	/* Check Secondary Diagonal -1 */
	for(i = 0; i < boardSize - 2; i++)
	{
		for(j = 0; j < boardSize - 2; j++)
		{
			if(i == j)
			{
				if(board[i+1][j] != ' ')
				{
					if(board[i+1][j] == board[i + 2][j + 1])
						count++;
				}
			}
		}
	}
	
	if(count >= StreakRule(boardSize))
		return 1;
	else
		count = 0;
	
	if(boardSize == 5)
		return 0;
	
	/* Check Secondary Diagonal +2 */
	for(i = 0; i < boardSize - 3; i++)
	{
		for(j = 0; j < boardSize - 3; j++)
		{
			if(i == j)
			{
				if(board[i][j+2] != ' ')
				{
					if(board[i][j + 2] == board[i + 1][j + 3])
						count++;
				}
				
			}
		}
	}
	
	if(count >= StreakRule(boardSize))
		return 1;
	else
		count = 0;
	
	/* Check Secondary Diagonal -2 */
	for(i = 0; i < boardSize - 3; i++)
	{
		for(j = 0; j < boardSize - 3; j++)
		{
			if(i == j)
			{
				if(board[i+2][j] != ' ')
				{
					if(board[i+2][j] == board[i + 3][j + 1])
						count++;
				}
				
			}
		}
	}
	
	if(count >= StreakRule(boardSize))
		return 1;
	else
		count = 0;
	
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
	
	/* Check Secondary Diagonal + 1 */
	for(i = 0; i < boardSize - 2; i++)
	{
		for(j = 0; j < boardSize; j++)
		{
			/* cek apakah sedang dalam kotak diagonal sekunder */
			if(i + j == boardSize - 1)
			{
				if(board[i][j - 1] != ' ')
				{
					/* cek apakah kotak diagonal sama dengan kotak diagonal sebelumnnya */
					if(board[i][j - 1] == board[i+1][j-2])
						count++;
				}
				
			}
		}
	}
	/* check streak */
	if(count >= StreakRule(boardSize))
		return 1;
	else
		count = 0;
	
	/* Check Secondary Diagonal - 1 */
	for(i = 0; i < boardSize - 2; i++)
	{
		for(j = 0; j < boardSize; j++)
		{
			/* cek apakah sedang dalam kotak diagonal sekunder */
			if(i + j == boardSize - 1)
			{
				if(board[i + 1][j] != ' ')
				{
					/* cek apakah kotak diagonal sama dengan kotak diagonal sebelumnnya */
					if(board[i+1][j] == board[i+2][j-1])
						count++;
				}
				
			}
		}
	}
	/* check streak */
	if(count >= StreakRule(boardSize))
		return 1;
	else
		count = 0;
	
	/* Check Secondary Diagonal + 2 */
	for(i = 0; i < boardSize - 3; i++)
	{
		for(j = 0; j < boardSize; j++)
		{
			/* cek apakah sedang dalam kotak diagonal sekunder */
			if(i + j == boardSize - 1)
			{
				if(board[i][j - 2] != ' ')
				{
					/* cek apakah kotak diagonal sama dengan kotak diagonal sebelumnnya */
					if(board[i][j - 2] == board[i+1][j-3])
						count++;
				}
				
			}
		}
	}
	/* check streak */
	if(count >= StreakRule(boardSize))
		return 1;
	else
		count = 0;
		
	/* Check Secondary Diagonal - 2 */
	for(i = 0; i < boardSize - 3; i++)
	{
		for(j = 0; j < boardSize; j++)
		{
			/* cek apakah sedang dalam kotak diagonal sekunder */
			if(i + j == boardSize - 1)
			{
				if(board[i+2][j] != ' ')
				{
					/* cek apakah kotak diagonal sama dengan kotak diagonal sebelumnnya */
					if(board[i+2][j] == board[i+3][j-1])
						count++;
				}
				
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
			GetComputerInput(board, &Move, currentPlayer, boardSize);	
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

void GetComputerInput(char **board, MoveFormat *computerMove, int *currentPlayer, int boardSize)
{	
	int row, col;
	int moveAvailable[boardSize*boardSize][2];
	int currentIdx = 0;
	int randMove;
	
	// check kotak kosong tersedia
	for(row = 0; row < boardSize; row++)
	{
		for(col = 0; col < boardSize; col++)
		{
			if(board[row][col] == ' ')
			{
				// kumpulkan kotak tersedia
				moveAvailable[currentIdx][0] = row;
				moveAvailable[currentIdx][1] = col;
				currentIdx++;
			}
		}
	}
	
	srand(time(NULL));
	// Memilih secara acak kotak yang ingin di isi dari kotak kosong yang tersedia
	randMove = rand() % currentIdx;
	
	// mengisi kotak kosong dengan langkah yang telah di generate
	computerMove->row = moveAvailable[randMove][0];
	computerMove->col = moveAvailable[randMove][1];
	
}

int isValidInput(char **board, MoveFormat *playerMove, int boardSize)
{
	if((playerMove->row < 0 || playerMove->row >= boardSize ) || (playerMove->col < 0 || playerMove->col >= boardSize))
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
	InputNameUI();
	printf("Input Player 1 Name : ");scanf("%s", &playerName->NameP1);
	//fgets(playerName->NameP1, 12, stdin);
	printf("Input Player 2 Name  : ");scanf("%s", &playerName->NameP2);
	//fgets(playerName->NameP2, 12, stdin);
}

/* Dealocate memory from board */
void DeleteBoard(char **board, int boardSize)
{
	int i;
	for(i = 0; i < boardSize; i++)
		free(board[i]);
	free(board);
}
