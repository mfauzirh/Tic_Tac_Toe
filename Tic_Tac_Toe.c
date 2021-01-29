#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#define HIGHSCORE_FILENAME "highscores.dat"

/* ======================================*/
/*				UDT						*/
/* ======================================*/

typedef struct
{
	int row;
	int col;
} MoveFormat;

typedef struct
{
	char NameP1[255];
	char NameP2[255];
} PlayerName;

typedef struct
{
	char name[20];
	int duration;
	int boardSize;
}HighscoreData;

HighscoreData data, listData[100], tempData; // highscore data

/* =====================================*/
/*				Deklarasi Modul			*/
/* =====================================*/

/* Modul Tampilan */
void MainMenuUI();
void GameModeUI();
void ChooseOpponentUI();
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
void MainMenu();
void GameMode(int *gameMode);
void ChooseOpponent(int *opponent);
void ChooseBoard(int *boardSize);
void GameOver(int currentPlayer, PlayerName playerName, int roundPlayed, int maxRound, int timeConsume, int boardSize);
char** CreateBoard(int boardSize);
int StreakRule(int boardSize);
int CheckStreak(char temp[], int boardSize, int numStreak);
int CheckHorizontal(char **board, int boardSize);
int CheckVertical(char **board, int boardSize);
int CheckMainDiagonal(char **board, int boardSize);
int CheckSecDiagonal(char **board, int boardSize);
int CheckWin(char **board, int boardSize);
void MakeMove(char **board, int boardSize, int *currentPlayer, int gameMode, int opponent);
int InputWithTime(unsigned int timeout);
void GetUserInput(char **board, MoveFormat *playerMove,  int *currentPlayer, int boardSize);
void GetComputerInput(char **board, MoveFormat *computerMove, int *currentPlayer, int boardSize, int opponent);
void RandomBasedBoard(char **board, MoveFormat *computerMove,int boardSize);
void RandomBasedPlayer(char **board, MoveFormat *move,int boardSize);
int isValidInput(char **board, MoveFormat *playerMove, int BoardSize);
void FillBoard(char **board, MoveFormat Move, int boardSize, int *currentPlayer);
void InputName(PlayerName *playerName);
void DeleteBoard(char **board, int boardSize);

/* Modul Highscore */
void Highscores();
void WriteData(char winner[15], int duration, int boardSize);
int ReadData();
void SwapData(int x, int y);
void SortHighscores(int criteria, int amount);
int SortCriteria(int i, int k, int criteria);

/* =====================================*/
/*				Modul Utama				*/
/* =====================================*/
// ini merupakan modul utama program

int main()
{
	// Deklarasi
	char **board;
	int boardSize, currentPlayer, isWin, maxRound, roundPlayed, opponent, gameMode, isExit, timeConsume;
	PlayerName playerName;
	clock_t elapsedTime;
	
	isExit = 0;
	while(isExit == 0)
	{
		// Inisialisasi
		currentPlayer = 1;
		roundPlayed = 0;
		isWin = 0;
		
		// Menampilkan main menu
		MainMenu();
		
		//Menampilkan pilihan mode permainan
		GameMode(&gameMode);
		
		// Jika memilih Vs Computer maka tampilkan pilihan lawan bermain
		if(gameMode == 2)
			ChooseOpponent(&opponent);
		
		// Menginputkan nama pemain
		InputName(&playerName);
		
		// Memilih ukuran papan permainan
		ChooseBoard(&boardSize);
		
		// Membuat papan permainan
		board = CreateBoard(boardSize);
		
		// Menentukan ronde maksimal
		maxRound = boardSize * boardSize;
		
		// Menjalankan Stopwatch
		elapsedTime = clock();
		
		// --- Game Start ---
		while(roundPlayed != maxRound)
		{
			DrawBoard(board, boardSize);
			isWin = CheckWin(board, boardSize);
			if(isWin > 0)
				break;
			MakeMove(board, boardSize, &currentPlayer, gameMode, opponent);
			roundPlayed++;
		}
		// --- Game Over ---
		
		// Memberhentikan stopwatch
		elapsedTime = clock() - elapsedTime;
		timeConsume = elapsedTime / (CLOCKS_PER_SEC);
		
		// Menampilkan keadaan akhir papan permainan
		Beep(600, 300);
		Beep(700, 300);
		Beep(550, 500);
		Beep(700, 300);
		Beep(700, 300);
		
		DrawBoard(board, boardSize);
		printf("Game Over, press any key to continue : ");getch();

		// Menampilkan tampilan Game Over
		GameOver(currentPlayer, playerName, roundPlayed, maxRound, timeConsume, boardSize);
		
		// Menghapus papan saat ini
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

void ChooseOpponentUI()
{
	system("CLS");
	printf("\t      <<============>>\n");
	printf("\t|====                 ====|\n");
	printf("\t|         Opponent        |\n");
	printf("\t|====                 ====|\n");
	printf("\t      <<== Choose ==>>\n\n");
	
	printf("       ============================\n");
	printf("      ||[1] The Randomizer        ||\n");
	printf("      ||[2] The Stalker           ||\n");
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
	printf("\t\t            <<============>>\n");
	printf("\t\t      |====                  ====|\n");
	printf("\t\t      |         Gameover         |\n");
	printf("\t\t      |====                  ====|\n");
	printf("\t\t            <<== winner ==>>\n\n");
	
	printf("       =========================================================\n");
	printf("                              %s                        \n", winner);
	printf("            Memenangkan permainan dengan waktu %d detik ++>\n", timeConsume);
	printf("       =========================================================\n");
	
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
    printf("   0     1     2  \n");
    printf(" _________________\n");
    printf("|     |     |     |\n");
    printf("|  %c  |  %c  |  %c  | 0\n",board[0][0],board[0][1],board[0][2]);
    printf("|_____|_____|_____|\n");
    printf("|     |     |     |\n");
    printf("|  %c  |  %c  |  %c  | 1\n",board[1][0],board[1][1],board[1][2]);
    printf("|_____|_____|_____|\n");
    printf("|     |     |     |\n");
    printf("|  %c  |  %c  |  %c  | 2\n",board[2][0],board[2][1],board[2][2]);
    printf("|_____|_____|_____|\n");
    printf("\n\n");
}

/* Modul untuk membuat tampilan papan 5x5 */
void Draw5x5Board(char**board)
{
    system("cls");
    printf("\n\n\tTic Tac Toe\n\n");
    printf("Player 1 (X) -- Player 2 (O)\n\n");
    printf("   0     1     2     3     4  \n");
    printf(" _____________________________\n");
    printf("|     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  | 0\n",board[0][0],board[0][1],board[0][2],board[0][3],board[0 ][4]);
    printf("|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  | 1\n",board[1][0],board[1][1],board[1][2],board[1][3],board[1][4]);
    printf("|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  | 2\n",board[2][0],board[2][1],board[2][2],board[2][3],board[2][4]);
    printf("|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  | 3\n",board[3][0],board[3][1],board[3][2],board[3][3],board[3][4]);
    printf("|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  | 4\n",board[4][0],board[4][1],board[4][2],board[4][3],board[4][4]);
    printf("|_____|_____|_____|_____|_____|\n");
    printf("\n\n");
}

/* Modul untuk membuat tampilan papan 7x7 */
void Draw7x7Board(char**board)
{
    system("cls");
    printf("\n\n\tTic Tac Toe\n\n");
    printf("Player 1 (X) -- Player 2 (O)\n\n");
    printf("   0     1     2     3     4     5     6  \n");
    printf(" _________________________________________\n");
    printf("|     |     |     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c  | 0\n",board[0][0],board[0][1],board[0][2],board[0][3],board[0][4], board[0][5],board[0][6]);
    printf("|_____|_____|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c  | 1\n",board[1][0],board[1][1],board[1][2],board[1][3],board[1][4], board[1][5],board[1][6]);
    printf("|_____|_____|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c  | 2\n",board[2][0],board[2][1],board[2][2],board[2][3],board[2][4], board[2][5],board[2][6]);
    printf("|_____|_____|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c  | 3\n",board[3][0],board[3][1],board[3][2],board[3][3],board[3][4], board[3][5],board[3][6]);
    printf("|_____|_____|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c  | 4\n",board[4][0],board[4][1],board[4][2],board[4][3],board[4][4], board[4][5],board[4][6]);
    printf("|_____|_____|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c  | 5\n",board[5][0],board[5][1],board[5][2],board[5][3],board[5][4], board[5][5],board[5][6]);
    printf("|_____|_____|_____|_____|_____|_____|_____|\n");
    printf("|     |     |     |     |     |     |     |\n");
    printf("|  %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c  | 6\n",board[6][0],board[6][1],board[6][2],board[6][3],board[6][4], board[6][5],board[6][6]);
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

void MainMenu()
{
	int choice;
	do
	{
		MainMenuUI();
		scanf("%d", &choice);
		switch(choice)
		{
			case 1: break;
			case 2: Highscores(); break;
			case 3: exit(0); break;
			case 4: HelpUI();
		}
	}while(choice != 1);
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

void ChooseOpponent(int *opponent)
{
	do
	{
		ChooseOpponentUI();
		scanf("%d", opponent);
	}while(*opponent != 1 && *opponent != 2);
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

void GameOver(int currentPlayer, PlayerName playerName, int roundPlayed, int maxRound, int timeConsume, int boardSize)
{
	char winner[15];
	int choice;
	int isWin;
	if(roundPlayed != maxRound)
	{
		if(currentPlayer == 1)
			strcpy(winner, playerName.NameP2);
		else if(currentPlayer == 2)
			strcpy(winner, playerName.NameP1);
		
		isWin = 1;
	}else if(roundPlayed == maxRound)
		isWin = 0;
	
	// Menulis data ke file higscore
	if(isWin == 1)
		WriteData(winner, timeConsume, boardSize);
	
	do
	{
		if(isWin == 1)
			WinnerUI(winner, timeConsume);
		else if(isWin == 0)
			TieUI();
			
		scanf("%d", &choice);
		if(choice == 1)
			exit(0);
		else if(choice == 2)
			break;
	}while(choice != 1 && choice != 2);
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
		numberToStreak = boardSize - 1;
	else if(boardSize == 7)
		numberToStreak = boardSize - 2;
	
	return numberToStreak;
}

int CheckStreak(char board[], int lineSize, int numStreak)
{
	// Modul CheckStreak berfungsi untuk men-cek pola streak 
	// lineSize adalah jumlah kotak yang tersedia
	// numStreak adalah jumlah total simbol yang membentuk pola kemenangan
	
	if(lineSize == 3) // Check semua kemungkinan pola menang di papan berisi 3 kotak
	{
		// jumlah streak 3
		// Kemungkinan Pola Streak : XXX
		if
		(
			(board[0] == board[1] && board[1] == board[2])	
		)
		{
			if(board[1] == 'X' || board[1] == 'O') 
				return 1;
		}
	}else if(lineSize == 4) // Check semua kemungkinan pola menang di papan berisi 4 kotak
	{
		// jumlah streak 4
		// Kemungkinan Pola Streak : XXXX
		if(board[0] == board[1] && board[1] == board[2] && board[2] == board[3])
		{
			if(board[1] == 'X' || board[1] == 'O')
				return 1;
		}
		
	}else if(lineSize == 5) 
	{
		// Check semua kemungkinan pola menang di papan berisi 5 kotak
		if(numStreak == 4) // jumlah streak 4
		{
			// Pola Streak  : _XXXX OR XXXX_ OR XXXXX
			if
			(
				(board[0] == board[1] && board[1] == board[2] && board[2] == board[3]) ||
				(board[1] == board[2] && board[2] == board[3] && board[3] == board[4])	
			)
			{
				if(board[2] == 'X' || board[2] == 'O')
					return 1;
			}
		}else if(numStreak == 5) // jumlah streak 5
		{
			// Pola Streak  : XXXXX
			if(board[0] == board[1] && board[1] == board[2] && board[2] == board[3] && board[3] == board[4])
			{
				if(board[2] == 'X' || board[2] == 'O')
					return 1;
			}
		}
		
	}else if(lineSize == 6) // Check semua kemungkinan pola menang di papan berisi 6 kotak
	{
		// jumlah streak 5
		// pola Streak : _XXXXX OR XXXXX_
		if
		(
			(board[0] == board[1] && board[1] == board[2] && board[2] == board[3] && board[3] == board[4]) ||
			(board[1] == board[2] && board[2] == board[3] && board[3] == board[4] && board[4] == board[5])	
		)
		{
			if(board[3] == 'X' || board[3] == 'O')
				return 1;
		}
	}else if(lineSize == 7)// Check semua kemungkinan pola menang di papan dengan berisi 7 kotak
	{
		// jumlah streak 5
		// pola Streak : __XXXXX OR _XXXXX_ OR XXXXX__
		if
		(
			(board[0] == board[1] && board[1] == board[2] && board[2] == board[3] && board[3] == board[4]) ||
			(board[2] == board[3] && board[3] == board[4] && board[4] == board[5] && board[5] == board[6]) ||
			(board[1] == board[2] && board[2] == board[3] && board[3] == board[4] && board[4] == board[5])	
		)
		{
			if(board[4] == 'X' || board[4] == 'O')
				return 1;
		}
	}
}

/* Modul untuk memeriksa kemenangan secara horizontal */
int CheckHorizontal(char **board, int boardSize)
{
	char temp[boardSize];
	int i, j;
	
	for(i = 0; i < boardSize; i++)
	{
		for(j = 0; j < boardSize; j++)
			temp[j] = board[i][j];
		
		if(CheckStreak(temp, boardSize, StreakRule(boardSize)) == 1)
		{
			return 1;
		}
	}
	
	return 0;
}

/* Modul untuk memeriksa kemenangan secara vertical */
int CheckVertical(char **board, int boardSize)
{
	char temp[boardSize];
	int i, j;
	
	for(i = 0; i < boardSize; i++)
	{
		for(j = 0; j < boardSize; j++)
			temp[j] = board[j][i];
		
		if(CheckStreak(temp, boardSize, StreakRule(boardSize)) == 1)
		{
			return 1;
		}
	}
	
	return 0;
}

/* Memeriksa kemengangan di diagonal Utama */
int CheckMainDiagonal(char **board, int boardSize)
{
	int i, j, k;
	char temp[boardSize];
	
	// check main diagonal 0
	k = 0;
	for(i = 0; i < boardSize; i++)
	{
		for(j = 0; j < boardSize; j++)
		{
			if(i == j)
			{
				temp[k] = board[i][j];
				k++;
			}
		}
	}	
	
	if(CheckStreak(temp, boardSize, StreakRule(boardSize)) == 1)
		return 1;
	
	// jika papan berukuran 3x3 tidak perlu lanjut
	if(boardSize == 3)
		return 0;
	
	// Check main diagonal +1
	k = 0;
	for(i = 0; i < boardSize - 1; i++)
	{
		for(j = 0; j < boardSize - 1; j++)
		{
			if(i == j)
			{
				temp[k] = board[i][j+1];
				k++;
			}
		}
	}
	
	if(CheckStreak(temp, boardSize - 1, StreakRule(boardSize)) == 1)
		return 1;
		
	// Check main diagonal -1
	k = 0;
	for(i = 0; i < boardSize - 1; i++)
	{
		for(j = 0; j < boardSize - 1; j++)
		{
			if(i == j)
			{
				temp[k] = board[i+1][j];
				k++;
			}
		}
	}
	
	if(CheckStreak(temp, boardSize - 1, StreakRule(boardSize)) == 1)
		return 1;
	
	// jika ukuran papan 5x5 tidak perlu lanjut
	if(boardSize == 5)
		return 0;
	
	// Check main diagonal +2
	k = 0;
	for(i = 0; i < boardSize - 2; i++)
	{
		for(j = 0; j < boardSize - 2; j++)
		{
			if(i == j)
			{
				temp[k] = board[i][j+2];
				k++;
			}
		}
	}
	
	if(CheckStreak(temp, boardSize - 2, StreakRule(boardSize)) == 1)
		return 1;
	
	// Check main diagonal -2
	k = 0;
	for(i = 0; i < boardSize - 2; i++)
	{
		for(j = 0; j < boardSize - 2; j++)
		{
			if(i == j)
			{
				temp[k] = board[i+2][j];
				k++;
			}
		}
	}
	
	if(CheckStreak(temp, boardSize - 2, StreakRule(boardSize)) == 1)
		return 1;
		
	return 0;
}

/* Memeriksa kemenangan di diagonal sekunder */
int CheckSecDiagonal(char **board, int boardSize)
{
	int i, j, k;
	char temp[boardSize];
	
	// Check Secondary diagonal 0
	k = 0;
	for(i = 0; i < boardSize; i++)
	{
		for(j = 0; j < boardSize; j++)
		{
			if(i + j == boardSize - 1)
			{
				temp[k] = board[i][j];
				k++;
			}
		}
	}	
	
	if(CheckStreak(temp, boardSize, StreakRule(boardSize)) == 1)
	{
		return 1;
	}
	
	// jika ukuran papan 3x3 tidak perlu lanjut
	if(boardSize == 3)
		return 0;
	
	// Check secondary diagonal +1
	k = 0;
	for(i = 0; i < boardSize - 1; i++)
	{
		for(j = 0; j < boardSize; j++)
		{
			if(i + j == boardSize - 1)
			{
				temp[k] = board[i][j-1];
				k++;
			}
		}
	}
	
	if(CheckStreak(temp, boardSize - 1, StreakRule(boardSize)) == 1)
		return 1;
		
	// Check secondary diagonal -1
	k = 0;
	for(i = 0; i < boardSize - 1; i++)
	{
		for(j = 0; j < boardSize; j++)
		{
			if(i + j == boardSize - 1)
			{
				temp[k] = board[i+1][j];
				k++;
			}
		}
	}
	
	if(CheckStreak(temp, boardSize - 1, StreakRule(boardSize)) == 1)
		return 1;
	
	// jika ukuran papan 5x5 tidak perlu lanjut
	if(boardSize == 5)
		return 0;
		
	// Check secondary diagonal +2
	k = 0;
	for(i = 0; i < boardSize - 2; i++)
	{
		for(j = 0; j < boardSize; j++)
		{
			if(i + j == boardSize - 1)
			{
				temp[k] = board[i][j-2];
				k++;
			}
		}
	}
	
	if(CheckStreak(temp, boardSize - 2, StreakRule(boardSize)) == 1)
		return 1;
	
	// Check secondary diagonal -2
	k = 0;
	for(i = 0; i < boardSize - 2; i++)
	{
		for(j = 0; j < boardSize; j++)
		{
			if(i + j == boardSize - 1)
			{
				temp[k] = board[i+2][j];
				k++;
			}
		}
	}
	
	if(CheckStreak(temp, boardSize - 2, StreakRule(boardSize)) == 1)
		return 1;
	
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

/* Melakukan Pergerakan di papan */
void MakeMove(char **board, int boardSize, int *currentPlayer, int gameMode, int opponent)
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
			GetComputerInput(board, &Move, currentPlayer, boardSize, opponent);	
	}
	
	/* Fill to Board */
	FillBoard(board, Move, boardSize, currentPlayer);
}

int InputWithTime(unsigned int timeout)
{
	for (; timeout != 0; timeout--)
    {
        if (kbhit())
        {
            return getch();
        }
    }

    return -1;
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
		//printf("Masukan baris : "); scanf("%d", &playerMove->row);
		//printf("Masukan kolom : "); scanf("%d", &playerMove->col);
		printf("Masukan baris : "); playerMove->row = InputWithTime(50000) - '0'; 
		if(playerMove->row != -49) 
			printf("%d", playerMove->row);
		printf("\nMasukan kolom : "); playerMove->col = InputWithTime(50000) - '0';
		if(playerMove->col != -49) 
			printf("%d", playerMove->col);
		
		if(playerMove->row == -49 || playerMove->col == -49)
		{
			GetComputerInput(board, playerMove, currentPlayer, boardSize, 1);
			Beep(600, 300);
			Beep(450, 500);
		}
			
			
		isValid = isValidInput(board, playerMove, boardSize);
	}while(isValid == 0);
}

void GetComputerInput(char **board, MoveFormat *computerMove, int *currentPlayer, int boardSize, int opponent)
{	
	if(opponent == 1)
	{
		RandomBasedBoard(board, computerMove, boardSize);
	}else if(opponent == 2)
	{
		RandomBasedPlayer(board, computerMove, boardSize);
	}
	
	
}

void RandomBasedBoard(char **board, MoveFormat *computerMove,int boardSize)
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

void RandomBasedPlayer(char **board, MoveFormat *move,int boardSize)
{
	int moveAvailable[4][2];
	int randMove = 0;
	int count = 0;
	
	// Check bottom box
	if(move->row + 1 < boardSize)
	{
		if(board[move->row + 1][move->col] == ' ')
		{
			moveAvailable[count][0] = move->row + 1;
			moveAvailable[count][1] = move->col;
			count++;
		}
	}
	
	// check top box
	if(move->row - 1 >= 0)
	{
		if(board[move->row - 1][move->col] == ' ')
		{
			moveAvailable[count][0] = move->row - 1;
			moveAvailable[count][1] = move->col;
			count++;
		}
	}
	
	// check right box
	if(move->col + 1 < boardSize)
	{
		if(board[move->row][move->col + 1] == ' ')
		{
			moveAvailable[count][0] = move->row;
			moveAvailable[count][1] = move->col+1;
			count++;
		}
	}
	
	// check left box
	if(move->col - 1 >= 0)
	{
		if(board[move->row][move->col - 1] == ' ')
		{
			moveAvailable[count][0] = move->row;
			moveAvailable[count][1] = move->col-1;
			count++;
		}
	}
	if(count == 0)
	{
		RandomBasedBoard(board, move, boardSize);
	}else
	{
		// Randomized
	srand(time(NULL));
	// Memilih secara acak kotak yang ingin di isi dari kotak kosong yang tersedia
	randMove = rand() % count;
	
	// mengisi kotak kosong dengan langkah yang telah di generate
	move->row = moveAvailable[randMove][0];
	move->col = moveAvailable[randMove][1];
	}
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

	// Membersihkan \n dari input sebelumnnya
	fflush(stdin);
	
	do
	{
		InputNameUI();
		if(playerName->NameP1[0] == '\n')
			printf("Harap masukan setidaknya 1 huruf\n");
			
		printf("Input Player 1 Name : ");
		fgets(playerName->NameP1, 15, stdin);
	}while(playerName->NameP1[0] == '\n');
	
	// Mengambil karakter \n efek dari fgets
	strtok(playerName->NameP1, "\n");
	
	do
	{
		InputNameUI();
		if(playerName->NameP2[0] == '\n')
			printf("Harap masukan setidaknya 1 huruf\n");
		
		printf("Input Player 2 Name : ");
		fgets(playerName->NameP2, 15, stdin);
	}while(playerName->NameP2[0] == '\n');
	
	// Mengambil karakter \n efek dari fgets
	strtok(playerName->NameP1, "\n");
}

/* Dealocate memory from board */
void DeleteBoard(char **board, int boardSize)
{
	int i;
	for(i = 0; i < boardSize; i++)
		free(board[i]);
	free(board);
}

// Highscore Modul
void Highscores()
{
	int amount, i, criteria = 1;
	amount = ReadData();
	do
	{
		system("CLS");
		printf("\t\t==================\n");
		printf("\t\t     Highscores   \n");
		printf("\t\t==================\n\n");
		printf("\t\t==================\n");
		printf("\tKetik [1] untuk mengurutkan berdasarkan durasi\n");
		printf("\tKetik [2] untuk mengurutkan berdasarkan ukuran papan\n");
		printf("\tKetik [3] untuk mengurutkan berdasarkan nama\n");
		printf("\tKetik [4] untuk kembali ke main menu\n\n\n");
		
		SortHighscores(criteria, amount);
		printf("Nama Pemain\t\tDurasi Permainan(detik)\t\tUkuran Papan\n");
		for(i = 0; i < amount; i++)
		{
			printf("%-20s", listData[i].name);
			printf("\t\t  ");
			printf("%d", listData[i].duration);
			printf("\t\t\t     ");
			printf("%d", listData[i].boardSize);
			printf("\n");
		}
		printf("sort berdasarkan : ");scanf("%d", &criteria);
	}while(criteria != 4);
}

void WriteData(char winner[15], int duration, int boardSize)
{
	// Mengisikan data ke highscore
	strcpy(data.name, winner);
	data.duration = duration;
	data.boardSize = boardSize;
	
	// mengisikan data ke file highscores
	FILE *fp = fopen(HIGHSCORE_FILENAME, "ab"); 
	fwrite(&data, sizeof(HighscoreData), 1, fp);
	fclose(fp);
	
}

int ReadData()
{
	// membaca semua aata yang ada di file highscores
	// mengembalikan jumlah data yang telah di baca
	int amount = 0;
	FILE *fp = fopen(HIGHSCORE_FILENAME, "rb");
	
	while( fread(&listData[amount], sizeof(HighscoreData), 1, fp) == 1)
		amount++;
		
	fclose(fp);
	return amount;
}

int SortCriteria(int i, int j, int criteria)
{
	if(criteria == 1)
	{
		// mengurutkan berdasarkan durasi permainan
		return listData[j].duration < listData[i].duration;
	} else if(criteria == 2)
	{
		// mengurutkan berdasarkan ukuran papan permainan
		return listData[j].boardSize > listData[i].boardSize;
	} else if(criteria == 3)
	{
		// mengurutkan berdasarkan nama
		return strcmp(listData[j].name, listData[i].name) < 0;
	}
}

void SortHighscores(int criteria, int amount)
{
	// size merupakan ukuran papan permainan
	// time merupakan durasi permainan
	int i, j;
	
	for(i = 0; i < amount - 1; i++)
	{
		for(j = i + 1; j < amount; j++)
		{
			if(SortCriteria(i, j, criteria))
			{
				SwapData(i, j);
			}
		}
	}
}

void SwapData(int x, int y)
{
	int temp;
	tempData = listData[y] ;
	listData[y] = listData[x];
	listData[x] = tempData ;
}

