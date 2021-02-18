/* 
Program 	: 	TicTacToe_MFauziRH_SyahdaADUT_RijalAORS.c
Deskripsi 	:	Program Permainan Tic Tac Toe [Tugas Besar]
Nama/NIM 	: 	Muhammad Fauzi Rizki Hamdalah/201524015
				Rijal Azmi Oktoro Rahmatika S/201524025
				Syahda Afia Dhiya Ulhaq Tajudin/201524030
Tanggal 	:	18 Februari 2021/ ver 1.0
Compiler 	:	DevC++ TDM-GCC 4.9.2 64-bit Release
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#define HIGHSCORE_FILENAME "highscores.dat"
#define HOW_TO_PLAY "howToPlay.txt"
#define NameLength 13

/* ======================================*/
/*				UDT						*/
/* ======================================*/

typedef struct
{
	int row;
	int col;
} MoveFormat; // UDT for move Format

typedef struct
{
	char NameP1[255];
	char NameP2[255];
} PlayerName; // UDT for player data

typedef struct
{
	char name[20];
	int duration;
	int boardSize;
	char level[20];
}HighscoreData; // UDT for Highscore data

HighscoreData data, listData[100], tempData; // highscore data [global Variabel]

/* =====================================*/
/*				Deklarasi Modul			*/
/* =====================================*/

/* Modul Tampilan */
void MainMenuUI();
void GameModeUI();
void LevelingUI();
void ChooseOpponentUI();
void InputNameUI();
void ChooseBoardUI();
void ThanksUI();
void WinnerUI(char winner[], int timeConsume);
void TieUI();
void Draw3x3Board(char**);
void Draw5x5Board(char**);
void Draw7x7Board(char**);
void DrawBoard(char **board, int boardSize);

/* Modul Logika */

// Navigation
void MainMenu();
void HowToPlay();
void GameMode(int *gameMode);
void Leveling (int *waktu, char *level);
void ChooseOpponent(int *opponent);
void ChooseBoard(int *boardSize);
void GameOver(int currentPlayer, PlayerName playerName, int roundPlayed, int maxRound, int timeConsume, int boardSize, char level[20], int isWin);

// Board Modul
char** CreateBoard(int boardSize);
void DeleteBoard(char **board, int boardSize);

// Win logic
int StreakRule(int boardSize);
int CheckStreak(char temp[], int boardSize, int numStreak);
int CheckHorizontal(char **board, int boardSize);
int CheckVertical(char **board, int boardSize);
int CheckMainDiagonal(char **board, int boardSize);
int CheckSecDiagonal(char **board, int boardSize);
int CheckWin(char **board, int boardSize);

// Make Move in board
void MakeMove(char **board, int boardSize, int *currentPlayer, int gameMode, int opponent, int waktu);
void FillBoard(char **board, MoveFormat Move, int boardSize, int *currentPlayer);

// Get User Input
void GetUserInput(char **board, MoveFormat *playerMove,  int *currentPlayer, int boardSize, int waktu);
int InputWithTime(unsigned int timeout);
int isValidInput(char **board, MoveFormat *playerMove, int BoardSize);

// Generate AI input
void GetComputerInput(char **board, MoveFormat *computerMove, int *currentPlayer, int boardSize, int opponent);
void RandomBasedBoard(char **board, MoveFormat *computerMove,int boardSize);
void RandomBasedPlayer(char **board, MoveFormat *move,int boardSize);
void RandomSmartMove(char **board, MoveFormat *move,int boardSize);
MoveFormat GetWinningMove(char **board, int boardSize);
MoveFormat GetBlockMove(char **board, int boardSize);

// Player Config & other
void InputName(PlayerName *playerName, int gameMode);
void PlayMusic(int list);


/* Modul Highscore */ // Adaptation from Author Imam Saiful
void Highscores();
void WriteData(char winner[15], int duration, int boardSize, char level[20]);
void SwapData(int x, int y);
void SortHighscores(int criteria, int amount);
int ReadData();
int SortCriteria(int i, int k, int criteria);

/* =====================================*/
/*				Modul Utama				*/
/* =====================================*/
// ini merupakan modul utama program

int main()
{
	// Deklarasi
	char **board, level[20];
	int boardSize, currentPlayer, isWin, maxRound, roundPlayed, opponent, gameMode, waktu, isExit, timeConsume;
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
			
		//Menampilkan pilihan level
		Leveling(&waktu, level);

		// Menginputkan nama pemain
		InputName(&playerName, gameMode);
		
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
			MakeMove(board, boardSize, &currentPlayer, gameMode, opponent, waktu);
			isWin = CheckWin(board, boardSize);
			if(isWin > 0)
				break;
			roundPlayed++;
		}
		// --- Game Over ---
		
		// Memberhentikan stopwatch
		elapsedTime = clock() - elapsedTime;
		timeConsume = elapsedTime / (CLOCKS_PER_SEC);
		
		// Menampilkan keadaan akhir papan permainan
		PlayMusic(2);
		
		DrawBoard(board, boardSize);
		printf("Game Over, press any key to continue <(^_^)> : ");getch();

		// Menampilkan tampilan Game Over
		GameOver(currentPlayer, playerName, roundPlayed, maxRound, timeConsume, boardSize, level, isWin);
		
		// Menghapus papan saat ini
		DeleteBoard(board, boardSize);
	}
	
	return 0;
}

/* =====================================*/
/*				Modul Tampilan			*/
/* =====================================*/
// Dibawah ini merupakan kumpulan modul-modul untuk mengurus bagian tampilan

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
	printf("      ||Press '3' To Read Guide    ||\n");
    printf("      ||Press '4' To Exit The Game ||\n");
    printf("       =============================\n");
	printf("\nYour Choice <(^_^)> : ");
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
	printf("\nYour Choice <(^_^)> : ");
}

void LevelingUI()
{
		system("CLS");
	printf("\t      <<============>>\n");
	printf("\t|====                 ====|\n");
	printf("\t|          Level          |\n");
	printf("\t|====                 ====|\n");
	printf("\t      <<== Choose ==>>\n\n");
	
	printf("       ============================\n");
	printf("      ||[1] Beginner              ||\n");
	printf("      ||[2] Competent             ||\n");
	printf("      ||[3] Expert                ||\n");
	printf("       ============================\n");
	printf("\nYour Choice <(^_^)> : ");
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
	printf("      ||[3] The Thinker           ||\n");
	printf("       ============================\n");
	printf("\nYour Choice <(^_^)> : ");
}

void InputNameUI()
{
	system("CLS");
	printf("\t     <<==================>>\n");
	printf("\t|====                      ====|\n");
	printf("\t|      Game Configuration      |\n");
	printf("\t|====                      ====|\n");
	printf("\t     <<== Only 12 char ==>>\n\n");
}

void ChooseBoardUI()
{
	system("CLS");
	printf("\t     <<================>>\n");
	printf("\t|====                     ====|\n");
	printf("\t|      Choose Board Size      |\n");
	printf("\t|====                     ====|\n");
	printf("\t     <<==== Choose ====>>\n\n");
	
	printf("            ========================\n");
	printf("           ||Press '1' For 3x3 Size||\n");
	printf("           ||Press '2' For 5x5 Size||\n");
	printf("           ||Press '3' For 7x7 Size||\n");
	printf("            ========================\n");
	printf("\nYour Choice <(^_^)> : ");
}

void ThanksUI()
{
	system("CLS");
	printf("\t\t            <<============>>\n");
	printf("\t\t      |====                  ====|\n");
	printf("\t\t      |          Thanks          |\n");
	printf("\t\t      |====                  ====|\n");
	printf("\t\t            <<== <(^_^)> ==>>\n\n");
	
	printf("        =====================================================================\n");
	printf("       ||                                                                   ||\n");
	printf("       ||  TERIMA KASIH KEPADA PEMAIN KARENA TELAH MEMAINKAN PERMAINAN INI  ||\n");
	printf("       ||                                                                   ||\n");
	printf("        =====================================================================\n");
}

void WinnerUI(char winner[], int timeConsume)
{
	system("CLS");
	printf("\t\t            <<============>>\n");
	printf("\t\t      |====                  ====|\n");
	printf("\t\t      |         Gameover         |\n");
	printf("\t\t      |====                  ====|\n");
	printf("\t\t            <<== Winner ==>>\n\n");
	
	printf("       =========================================================\n");
	printf("                              %s                        \n", winner);
	printf("            Memenangkan permainan dengan waktu %d detik \n", timeConsume);
	printf("       =========================================================\n");
	
	printf("       ================================\n");
	printf("      ||Press '1' To Exit             ||\n");
	printf("      ||Press '2' To Back to main menu||\n");
	printf("       ================================\n");
	printf("\nYour Choice <(^_^)> : ");
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
	printf("\nYour Choice <(^_^)> : ");
}

void Draw3x3Board(char**board)
{
    system("cls");
    printf("                  =======  \n");
    printf("            ======       ======\n");
    printf("     =======                   =======\n");
    printf("    ||          Tic Tac Toe          ||\n");
    printf("    ||  Player 1 (X) -- Player 2 (O) ||\n");
    printf("     =======                   =======\n");
    printf("            ======       ======\n");
    printf("                  =======\n");
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
    printf("                  =======  \n");
    printf("            ======       ======\n");
    printf("     =======                   =======\n");
    printf("    ||          Tic Tac Toe          ||\n");
    printf("    ||  Player 1 (X) -- Player 2 (O) ||\n");
    printf("     =======                   =======\n");
    printf("            ======       ======\n");
    printf("                  =======\n");
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
    printf("                  =======  \n");
    printf("            ======       ======\n");
    printf("     =======                   =======\n");
    printf("    ||          Tic Tac Toe          ||\n");
    printf("    ||  Player 1 (X) -- Player 2 (O) ||\n");
    printf("     =======                   =======\n");
    printf("            ======       ======\n");
    printf("                  =======\n");
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

/* Navigasi */

void MainMenu()
{
	// Modul Navigasi Main menu
	int choice;
	do
	{
		MainMenuUI();
		choice = getch() - '0';
		switch(choice)
		{
			case 1: break;
			case 2: Highscores(); break;
			case 3: HowToPlay(); break;
			case 4: ThanksUI(); exit(0); break;
		}
	}while(choice != 1);
}

void HowToPlay()
{
	// Modul untuk menampilkan tampilan how to play dengan membaca file
	system("cls");
	FILE *fp = fopen(HOW_TO_PLAY, "r");
	char c;
	
	if(fp != NULL)
	{
		while(!feof(fp))
		{
			c = fgetc(fp);
			printf("%c", c);
		}
	
		printf("\n\nPress Any Key to Continue <(^_^)>"); getch();
	
		fclose(fp);
	}else if(fp == NULL)
		printf("Unable To Create the file <(^_^)>\n\n\n");
}

void GameMode(int *gameMode)
{
	do
	{
		GameModeUI();
		*gameMode = getch() - '0';
	}while(*gameMode != 1 && *gameMode != 2);
}

void Leveling(int *waktu, char *level)
{
	do
	{
		LevelingUI();
		*waktu = getch ()- '0';
		switch (*waktu)
		{
			case 1: *waktu =15; strcpy (level, "beginner"); break;
			case 2: *waktu =10; strcpy (level, "competent"); break;
			case 3: *waktu =5; strcpy (level, "expert"); break;
		}
	} while (*waktu != 15 && *waktu != 10 && *waktu != 5);
}

void ChooseOpponent(int *opponent)
{
	do
	{
		ChooseOpponentUI();
		*opponent = getch() - '0';
	}while(*opponent != 1 && *opponent != 2 && *opponent != 3);
}

void ChooseBoard(int *boardSize)
{
	do
	{
		ChooseBoardUI();
		*boardSize = getch() - '0';
		switch(*boardSize)
		{
			case 1: *boardSize = 3; break;
			case 2: *boardSize = 5; break;
			case 3: *boardSize = 7; break;
			default: *boardSize = 0; break;
		}
	}while(*boardSize == 0);
}

void GameOver(int currentPlayer, PlayerName playerName, int roundPlayed, int maxRound, int timeConsume, int boardSize, char level[20], int isWin)
{
	char winner[NameLength];
	int choice;
	int i;
	
	if(isWin > 0) // jika ada pemenang
	{
		if(currentPlayer == 1)
			strcpy(winner, playerName.NameP2);
		else if(currentPlayer == 2)
			strcpy(winner, playerName.NameP1);
			
		for(i = 0; winner[i]; i++)
			winner[i] = tolower(winner[i]);
	}
	
	// Menulis data ke file higscore
	if(isWin == 1)
		WriteData(winner, timeConsume, boardSize, level);
	
	do
	{
		// Navigasi game over
		if(isWin == 1)
			WinnerUI(winner, timeConsume);
		else if(isWin == 0)
			TieUI();
			
		choice = getch() - '0';
		if(choice == 1)
			exit(0);
		else if(choice == 2)
			break;
	}while(choice != 1 && choice != 2);
}

/* Board Modul*/

// Ini adalah modul untuk membuat papan permainan 
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
	
	return board; // mengembalikan papan yang telah dialokasikan memory
}

// Dealocate memory from board
void DeleteBoard(char **board, int boardSize)
{
	int i;
	for(i = 0; i < boardSize; i++)
		free(board[i]);
	free(board);
}

/* Win Modul */

int StreakRule(int boardSize)
{
	// Moduk untuk menenentukan syarat streak
	int numberToStreak;
	if(boardSize == 3)
		numberToStreak = boardSize - 1;
	else if(boardSize == 5)
		numberToStreak = boardSize - 1;
	else if(boardSize == 7)
		numberToStreak = boardSize - 2;
	
	return numberToStreak; // mengembalikan syarat streak
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
		for(j = 0; j < boardSize; j++) // mengisi pola horizontal baris ke-i
			temp[j] = board[i][j]; 
		
		if(CheckStreak(temp, boardSize, StreakRule(boardSize)) == 1) // mengirim pola horizontal baris ke-i untuk di cek
		{
			return 1; // jika terbentuk streak
		}
	}
	
	return 0; // jika tidak
}

/* Modul untuk memeriksa kemenangan secara vertical */
int CheckVertical(char **board, int boardSize)
{
	char temp[boardSize];
	int i, j;
	
	for(i = 0; i < boardSize; i++)
	{
		for(j = 0; j < boardSize; j++) // mengisi pola vertical baris ke-i
			temp[j] = board[j][i];
		
		if(CheckStreak(temp, boardSize, StreakRule(boardSize)) == 1) // mengirim pola vertical baris ke-i untuk di cek
		{
			return 1; // jika terbentuk streak
		}
	}
	
	return 0; // jika tidak
}

/* Memeriksa kemengangan di diagonal Utama */
int CheckMainDiagonal(char **board, int boardSize)
{
	// asumsi acuan diagonal ke-0 pada main diagonal untuk mengecek diagonal berikutnya
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

// Memeriksa kemenangan di diagonal sekunder 
int CheckSecDiagonal(char **board, int boardSize)
{
	// asumsi acuan diagonal ke-0 pada secondary diagonal untuk mengecek diagonal berikutnya
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

// Modul untuk men-cek kemenangan 
int CheckWin(char **board, int boardSize)
{
	/* mencek kemenangan secara horizontal, vertical dan diagonal */
	if(CheckHorizontal(board, boardSize) || CheckVertical(board, boardSize) || CheckMainDiagonal(board, boardSize) || CheckSecDiagonal(board, boardSize))
		return 1;
	else
		return 0;
}

/* Make Move in board*/

void MakeMove(char **board, int boardSize, int *currentPlayer, int gameMode, int opponent, int waktu)
{
	// Modul untuk melakukan pergerakan di papan
	
	/* Get Input */
	MoveFormat Move;
	
	if(gameMode == 1) // jika PvP
	{
		GetUserInput(board, &Move, currentPlayer, boardSize, waktu);
	}
	else if(gameMode == 2) // Jika PvE
	{
		if(*currentPlayer == 1)
			GetUserInput(board, &Move, currentPlayer, boardSize, waktu);
		else if(*currentPlayer == 2)
			GetComputerInput(board, &Move, currentPlayer, boardSize, opponent);	
	}
	
	/* Fill to Board */
	FillBoard(board, Move, boardSize, currentPlayer);
}

void FillBoard(char **board, MoveFormat Move, int boardSize, int *currentPlayer)
{
	// Melukis simbol dipapan berdasarkan gerakan yang telah di dapat
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

/* Get User Input */

int InputWithTime(unsigned int timeout)
{
	clock_t startTime = clock();
	timeout *= 1000;
	do
	{
		if (kbhit())
        {
            return getch();
        }
	}while(clock() < startTime + timeout);

    return -1;
}

void GetUserInput(char **board, MoveFormat *playerMove, int *currentPlayer, int boardSize, int waktu)
{
	int isValid = 1;
	do
	{
		if(isValid == 0)
		{
			DrawBoard(board, boardSize);
			printf("Inputan tidak valid, harap masukan kembali <(^_^)> \n\n");
		}
		printf("Giliran player %d. \n", *currentPlayer);
		printf("Masukan baris <(^_^)> : "); playerMove->row = InputWithTime(waktu) - '0'; 
		if(playerMove->row >= 0 && playerMove->row < boardSize) 
			printf("%d", playerMove->row);
		printf("\nMasukan kolom <(^_^)> : "); playerMove->col = InputWithTime(waktu) - '0';
		if(playerMove->row >= 0 && playerMove->col < boardSize) 
			printf("%d", playerMove->col);
		
		if(playerMove->row == -49 || playerMove->col == -49)
		{
			GetComputerInput(board, playerMove, currentPlayer, boardSize, 1);
			PlayMusic(1); // playing skip sound
		}
			
			
		isValid = isValidInput(board, playerMove, boardSize);
	}while(isValid == 0);
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

/* Generate AI Input */

MoveFormat GetWinningMove(char **board, int boardSize)
{
	// mencek semua kemungkinan gerakan kemenangan pada papan
	int i, j;
	MoveFormat winningMove;
	winningMove.row = -1;
	winningMove.col = -1;
	
	for(i = 0; i < boardSize; i++)
	{
		for(j = 0; j < boardSize; j++)
		{
			if(board[i][j] == ' ')
			{
				board[i][j] = 'O';
				
				if(CheckWin(board, boardSize) == 1) // jika ada gerakan kemenangan
				{
					winningMove.row = i;
					winningMove.col = j;
					board[i][j] = ' ';
					return winningMove; // mengembalikan gerakan kemenangan
				}
				
				board[i][j] = ' ';
			}
		}
	}
	return winningMove;
}

MoveFormat GetBlockMove(char **board, int boardSize)
{
	// mencari kemungkinan gerakan untuk mencegah player menang
	int i, j;
	MoveFormat blockMove;
	blockMove.row = -1;
	blockMove.col = -1;
	
	for(i = 0; i < boardSize; i++)
	{
		for(j = 0; j < boardSize; j++)
		{
			if(board[i][j] == ' ')
			{
				board[i][j] = 'X';
				
				if(CheckWin(board, boardSize) == 1) // jika ada block move
				{
					blockMove.row = i;
					blockMove.col = j;
					board[i][j] = ' ';
					return blockMove;
				}
				
				board[i][j] = ' ';
			}
		}
	}
	return blockMove;
}

void GetComputerInput(char **board, MoveFormat *computerMove, int *currentPlayer, int boardSize, int opponent)
{	
	if(opponent == 1) // vs. Randomizer
		RandomBasedBoard(board, computerMove, boardSize);
	else if(opponent == 2) // vs. Stalker
		RandomBasedPlayer(board, computerMove, boardSize);
	else if(opponent == 3) // vs. Thinker
		RandomSmartMove(board, computerMove, boardSize);
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
	int blockMove = 0;
	MoveFormat tempMove;
	
	tempMove = GetBlockMove(board, boardSize);
	if(tempMove.row != -1)
	{
		blockMove = 1;
		*move = tempMove;
	}
		
	
	if(blockMove == 0)
	{
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
	
}

void RandomSmartMove(char **board, MoveFormat *computerMove,int boardSize)
{
	int winMove = 0, blockMove = 0;
	
	// mencari gerakan kemenangan
	*computerMove = GetWinningMove(board, boardSize);
	if(computerMove->row != -1)
		winMove = 1;
	
	// mencari gerakan untuk mencegah kemenangan player
	if(winMove == 0)
		*computerMove = GetBlockMove(board, boardSize);
		
	if(computerMove->row != -1)
		blockMove = 1;
	
	// jika tidak ada gunakan random move
	if(winMove == 0 && blockMove == 0)
		RandomBasedBoard(board, computerMove, boardSize);
}

/* Player Config & Other */

void InputName(PlayerName *playerName, int gameMode)
{
	char buffer[255];
	int inputLen = 5;
	do
	{
		InputNameUI();
		if(inputLen < 5 || inputLen > NameLength)
			printf("Masukan minimal 4 karakter dan maximal 12 karakter <(^_^)> \n");

		fflush(stdin); // membersihkan inputan sebelumnnya
		printf("Masukan Nama Player 1 <(^_^)> : ");
		fgets(buffer, 255, stdin);	
		inputLen = strlen(buffer); // menghitung panjang inputan
		buffer[inputLen - 1] = '\0'; // mengambil default \n dari fgets
	}while(inputLen < 5 || inputLen > NameLength);
	
	strncpy(playerName->NameP1, buffer, NameLength - 1);
	
	if(gameMode == 1)
	{
		do
		{
			InputNameUI();
			if(inputLen < 5 || inputLen > NameLength)
				printf("Masukan minimal 4 karakter dan maximal 12 karakter <(^_^)> \n");
	
			fflush(stdin); // membersihkan inputan sebelumnnya
			printf("Masukan Nama Player 2 <(^_^)> : ");
			fgets(buffer, 255, stdin);	
			inputLen = strlen(buffer); // menghitung panjang inputan
			buffer[inputLen - 1] = '\0'; // mengambil default \n dari fgets
		}while(inputLen < 5 || inputLen > NameLength);
	
		strncpy(playerName->NameP2, buffer, NameLength - 1);
	}else
	{
		strcpy(playerName->NameP2, "computer");
	}
	
}

void PlayMusic(int list)
{
	// 1 for skip soundeffect
	// 2 for gameover soundeffect
	if(list == 1)
	{
		Beep(600, 300);
		Beep(450, 500);	
	}else if(list == 2)
	{
		Beep(600, 300);
		Beep(700, 300);
		Beep(550, 500);
		Beep(700, 300);
		Beep(700, 300);
	}
}

/* Highscore Modul : referensi imam saiful */

void Highscores()
{
	int amount, i, criteria = 1;
	amount = ReadData();
	do
	{
		system("CLS");
		printf("\t\t\t     <<============>>\n");
		printf("\t\t\t|====                ====|\n");
		printf("\t\t\t|       Highscores       |\n");
		printf("\t\t\t|====                ====|\n");
		printf("\t\t\t     <<============>>\n\n");
		
		printf("\t ========================================================\n");
		printf("\t|| Ketik [1] untuk mengurutkan berdasarkan durasi       ||\n");
		printf("\t|| Ketik [2] untuk mengurutkan berdasarkan ukuran papan ||\n");
		printf("\t|| Ketik [3] untuk mengurutkan berdasarkan nama         ||\n");
		printf("\t|| Ketik [4] untuk mengurutkan berdasarkan level        ||\n");
		printf("\t|| Ketik [5] untuk kembali ke main menu                 ||\n");
		printf("\t ========================================================\n\n\n");
		
		if(criteria < 1 || criteria > 5)
		{
			criteria = 1;
			printf("[Masukan Inputan yang valid] <(^_^)> \n\n");
		}
		
		SortHighscores(criteria, amount);
		printf("Nama Pemain\t\tDurasi Permainan(detik)\t\tUkuran Papan\t\tLevel\n");
		printf("=======================================================================================\n");
		for(i = 0; i < amount; i++)
		{
			printf("%-20s", listData[i].name);
			printf("\t\t  ");
			printf("%d", listData[i].duration);
			printf("\t\t\t     ");
			printf("%d", listData[i].boardSize);
			printf("\t\t\t");
			printf("%-10s\n", listData[i].level);
		}
		printf("\nsort berdasarkan <(^_^)> : ");
		criteria = getch() - '0';
	}while(criteria != 5);
}

void WriteData(char winner[15], int duration, int boardSize, char level[20])
{
	// Mengisikan data ke highscore
	strcpy(data.name, winner);
	data.duration = duration;
	data.boardSize = boardSize;
	strcpy(data.level, level);
	
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
	}else if(criteria == 4)
	{
		// mengurutkan berdasarkan level
		return strcmp(listData[j].level, listData[i].level) > 0;
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

