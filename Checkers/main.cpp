#include <iostream>
#include <vector>

using namespace std;
/*TODO:
*King and movement
*/

const int BOARD_SIZE = 64;
int board[BOARD_SIZE];
int redCount;
int blackCount;
const int Black_Player = 0;
const int Red_Player = 1;
const int Black_Piece = 1;
const int Red_Piece = 2;
const int Black_King = 3;
const int Red_King = 4;
const int Empty = 0;
const int MAX_INFINITY = 12;
const int MIN_INFINITY = -12;
const int MAX_DEPTH = 5;
int computer_move[BOARD_SIZE];
short evaluateComputerMove(int , int , int , int , int[], int,int );
short evaluateHumanMove(int , int , int , int , int[],int ,int );

//displays current state of board
void emitboard()
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if (i % 8 == 0)cout << "\n--------------------------------------------------------\n";
		if (board[i] == Empty) cout << "|     |";
		if (board[i] == Black_Piece) cout << "|  B  |";
		if (board[i] == Red_Piece) cout << "|  R  |";
		if (board[i] == Black_King)cout << "|  BK |";
		if (board[i] == Red_King)cout << "|  RK |";
	}
	cout << "\n--------------------------------------------------------\n";
	cout << endl;
}

//helper function for intilizeBoard()
//Places red pieces based on the part of board being intilized
void PlaceRedPiece(int i)
{
	if (i < 24)//Place Red Pieces
	{
		if (i < 8 || i >= 16)
		{
			if (i % 2 == 1)
			{
				board[i] = Red_Piece;
				redCount++;
			}
		}
		else
		{
			if (i % 2 == 0)
			{
				board[i] = Red_Piece;
				redCount++;
			}
		}
	}
}

//helper function for intilizeBoard()
//Places black pieces based on the part of board being intilized
void PlaceBlackPiece(int i)
{
	if (i >= 40)//Place Black Pieces
	{
		if (i < 48 || i >= 56)
		{
			if (i % 2 == 0 )
			{
				board[i] = Black_Piece;
				blackCount++;
			}
		}
		else
		{
			if (i % 2 == 1 )
			{
				board[i] = Black_Piece;
				blackCount++;
			}
		}
	}
}

//intilizesBoard to starting configuration
void intilizeBoard()
{	
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		board[i] = Empty;
		PlaceBlackPiece(i);
		PlaceRedPiece(i);
	}
}

//determines if the selected piece is valid
bool validPieceSelection(int player, int selection)
{
	if (selection < 0 || selection>BOARD_SIZE)return false;
	if (player == Red_Player)
	{
		return (board[selection] == Red_Piece || board[selection] == Red_King);
	}
	if (player == Black_Player)
	{
		return (board[selection] == Black_Piece || board[selection] == Black_King);
	}
	return false;
}

//select the piece the player wishes to move
int selectPiece(int player)
{
	int selection;
	bool valid = false;
	if (player == Black_Player)cout << "Blacks Turn:\n";
	else cout << "Reds Turn\n";
	while (!valid)
	{
		cout << "Please enter space you wish to select (0-top right, 63-bottom left): ";
		cin >> selection;
		if (validPieceSelection(player, selection)) valid = true;
		else
		{
			if (board[selection] == Empty)cout << "That space is empty please try again\n";
			else cout << "That is not your piece please try again\n";
		}
	}
	return selection;
}

//checks to see if the piece is/can capture another piece
//TODO King
bool canCapture(int player, int pieceSelected, int movePosition)
{
	if (movePosition < 0 || movePosition>BOARD_SIZE || pieceSelected < 0 || pieceSelected>BOARD_SIZE)return false; //moves off board
	if (pieceSelected % 8 == 0 && movePosition % 8 == 7)return false; //checks that the piece wont go from one side to the other
	if (pieceSelected % 8 == 7 && movePosition % 8 == 0)return false;//checks that the piece wont go from one side to the other
	if (board[movePosition] != Empty)return false; //checks to see if that location is empty
	if (player == Red_Player)
	{
		if (board[pieceSelected] == Red_Piece)
		{
			if (pieceSelected % 8 == 0)
			{
				if (pieceSelected + 18 == movePosition && board[pieceSelected + 9] == Black_Piece)return true;
			}
			if (pieceSelected % 8 == 7)
			{
				if (pieceSelected + 14 == movePosition && board[pieceSelected + 7] == Black_Piece)return true;
			}
			if ((pieceSelected + 14 == movePosition && board[pieceSelected + 7] == Black_Piece)
				|| (pieceSelected + 18 == movePosition && board[pieceSelected + 9] == Black_Piece))return true;
			if (pieceSelected + 14 > movePosition || pieceSelected + 18 > movePosition)
			{
				return (canCapture(player, pieceSelected + 14, movePosition)
					|| canCapture(player, pieceSelected + 18, movePosition));
			}
		}
	}
	if (player == Black_Player)
	{
		if (board[pieceSelected] == Black_Piece)
		{
			if (pieceSelected % 8 == 0)
			{
				if (pieceSelected - 18 == movePosition && board[pieceSelected - 9] == Red_Piece)return true;
			}
			if (pieceSelected % 8 == 7)
			{
				if (pieceSelected - 14 == movePosition && board[pieceSelected - 7] == Red_Piece)return true;
			}
			if ((pieceSelected - 14 == movePosition && board[pieceSelected - 7] == Red_Piece)
				|| (pieceSelected - 18 == movePosition && board[pieceSelected - 9] == Red_Piece))return true;
			if (pieceSelected - 14 > movePosition || pieceSelected - 18 > movePosition)		{
				return (canCapture(player, pieceSelected - 14, movePosition)
					|| canCapture(player, pieceSelected - 18, movePosition));
			}
		}
	}
	return false;
}
bool canCapture(int player, int pieceSelected, int movePosition, int curBoard[])
{
	if (movePosition < 0 || movePosition>BOARD_SIZE || pieceSelected < 0 || pieceSelected>BOARD_SIZE)return false; //moves off board
	if (pieceSelected % 8 == 0 && movePosition % 8 == 7)return false; //checks that the piece wont go from one side to the other
	if (pieceSelected % 8 == 7 && movePosition % 8 == 0)return false;//checks that the piece wont go from one side to the other
	if (curBoard[movePosition] != Empty)return false; //checks to see if that location is empty
	if (player == Red_Player)
	{
		if (curBoard[pieceSelected] == Red_Piece)
		{
			if (pieceSelected % 8 == 0)
			{
				if (pieceSelected + 18 == movePosition && curBoard[pieceSelected + 9] == Black_Piece)return true;
			}
			if (pieceSelected % 8 == 7)
			{
				if (pieceSelected + 14 == movePosition && curBoard[pieceSelected + 7] == Black_Piece)return true;
			}
			if ((pieceSelected + 14 == movePosition && curBoard[pieceSelected + 7] == Black_Piece)
				|| (pieceSelected + 18 == movePosition && curBoard[pieceSelected + 9] == Black_Piece))return true;
			if (pieceSelected + 14 > movePosition || pieceSelected + 18 > movePosition)
			{
				return (canCapture(player, pieceSelected + 14, movePosition, curBoard)
					|| canCapture(player, pieceSelected + 18, movePosition, curBoard));
			}
		}
	}
	if (player == Black_Player)
	{
		if (curBoard[pieceSelected] == Black_Piece)
		{
			if (pieceSelected % 8 == 0)
			{
				if (pieceSelected - 18 == movePosition && curBoard[pieceSelected - 9] == Red_Piece)return true;
			}
			if (pieceSelected % 8 == 7)
			{
				if (pieceSelected - 14 == movePosition && curBoard[pieceSelected - 7] == Red_Piece)return true;
			}
			if ((pieceSelected - 14 == movePosition && curBoard[pieceSelected - 7] == Red_Piece)
				|| (pieceSelected - 18 == movePosition && curBoard[pieceSelected - 9] == Red_Piece))return true;
			if (pieceSelected - 14 > movePosition || pieceSelected - 18 > movePosition)		{
				return (canCapture(player, pieceSelected - 14, movePosition, curBoard)
					|| canCapture(player, pieceSelected - 18, movePosition, curBoard));
			}
		}
	}
	return false;
}
bool canCapture(int player, int pieceSelected, int curBoard[])
{
	if (player == Black_Player)
	{
		if (board[pieceSelected] == Black_Piece)
		{
			if (pieceSelected % 8 == 0)
			{
				return (curBoard[pieceSelected - 9] == Red_Piece && curBoard[pieceSelected - 18] == Empty);
			}
			if (pieceSelected % 8 == 7)
			{
				return(curBoard[pieceSelected - 7] == Red_Piece && curBoard[pieceSelected - 14] == Empty);
			}
			if ((curBoard[pieceSelected - 7] == Red_Piece && curBoard[pieceSelected - 14] == Empty) ||
				(curBoard[pieceSelected - 9] == Red_Piece && curBoard[pieceSelected - 18] == Empty)) return true;
		}
	}
	if (player == Red_Player)
	{
		if (board[pieceSelected] == Red_Piece)
		{
			if (pieceSelected % 8 == 0)
			{
				return (curBoard[pieceSelected + 9] == Black_Piece && curBoard[pieceSelected + 18] == Empty);
			}
			if (pieceSelected % 8 == 7)
			{
				return(curBoard[pieceSelected + 7] == Black_Piece && curBoard[pieceSelected + 14] == Empty);
			}
			if ((curBoard[pieceSelected + 7] == Black_Piece && curBoard[pieceSelected + 14] == Empty) ||
				(curBoard[pieceSelected + 9] == Black_Piece && curBoard[pieceSelected + 18] == Empty)) return true;
		}
	}
	return false;
}
//checks to see if the move is valid
//TODO KING
bool validMove(int player, int pieceSelected, int movePosition)
{
	
	if (movePosition < 0 || movePosition>BOARD_SIZE)return false; //moves off board
	if (pieceSelected % 8 == 0 && movePosition % 8 == 7)return false; //checks that the piece wont go from one side to the other
	if (pieceSelected % 8 == 7 && movePosition % 8 == 0)return false;//checks that the piece wont go from one side to the other
	if (board[movePosition] != Empty)return false; //checks to see if that location is empty
	if (player == Red_Player)
	{
		if (board[pieceSelected] == Red_Piece)
		{
			if (pieceSelected % 8 == 0)
			{
				if (pieceSelected + 9 == movePosition)return true;
			}
			if (pieceSelected % 8 == 7)
			{
				if (pieceSelected + 7 == movePosition)return true;
			}
			if (pieceSelected + 7 == movePosition || pieceSelected + 9 == movePosition)return true;
			if (pieceSelected + 7 < movePosition || pieceSelected + 9 < movePosition)
			{
				return canCapture(player, pieceSelected, movePosition);
			}
		}
	}
	if (player == Black_Player)
	{
		if (board[pieceSelected] == Black_Piece)
		{
			if (pieceSelected % 8 == 0)
			{
				if (pieceSelected - 9 == movePosition)return true;
			}
			if (pieceSelected % 8 == 7)
			{
				if (pieceSelected - 7 == movePosition)return true;
			}
			if (pieceSelected - 7 == movePosition || pieceSelected - 9 == movePosition)return true;
			if (pieceSelected - 7 > movePosition || pieceSelected - 9 > movePosition)return canCapture(player, pieceSelected, movePosition);
		}
	}

	return false;
}
bool validMove(int player, int pieceSelected, int movePosition, int curBoard[])
{

	if (movePosition < 0 || movePosition>BOARD_SIZE)return false; //moves off board
	if (pieceSelected % 8 == 0 && movePosition % 8 == 7)return false; //checks that the piece wont go from one side to the other
	if (pieceSelected % 8 == 7 && movePosition % 8 == 0)return false;//checks that the piece wont go from one side to the other
	if (curBoard[movePosition] != Empty)return false; //checks to see if that location is empty
	if (player == Red_Player)
	{
		if (curBoard[pieceSelected] == Red_Piece)
		{
			if (pieceSelected % 8 == 0)
			{
				if (pieceSelected + 9 == movePosition)return true;
			}
			if (pieceSelected % 8 == 7)
			{
				if (pieceSelected + 7 == movePosition)return true;
			}
			if (pieceSelected + 7 == movePosition || pieceSelected + 9 == movePosition)return true;
			if (pieceSelected + 7 < movePosition || pieceSelected + 9 < movePosition)
			{
				return canCapture(player, pieceSelected, movePosition);
			}
		}
	}
	if (player == Black_Player)
	{
		if (curBoard[pieceSelected] == Black_Piece)
		{
			if (pieceSelected % 8 == 0)
			{
				if (pieceSelected - 9 == movePosition)return true;
			}
			if (pieceSelected % 8 == 7)
			{
				if (pieceSelected - 7 == movePosition)return true;
			}
			if (pieceSelected - 7 == movePosition || pieceSelected - 9 == movePosition)return true;
			if (pieceSelected - 7 > movePosition || pieceSelected - 9 > movePosition)return canCapture(player, pieceSelected, movePosition);
		}
	}

	return false;
}

//removes the piece(s) being captured from the board
//TODO King
void capture(int player, int pieceSelected, int movePosition)
{
	if (player == Red_Player)
	{
		if (board[pieceSelected] == Red_Piece)
		{
			if (pieceSelected % 8 == 0)
			{
				if (pieceSelected + 18 == movePosition && board[pieceSelected + 9] == Black_Piece)
				{
					board[pieceSelected + 9] = Empty;
					blackCount--;
				}
			}
			if (pieceSelected % 8 == 7)
			{
				if (pieceSelected + 14 == movePosition && board[pieceSelected + 7] == Black_Piece)
				{
					board[pieceSelected + 7] = Empty;
					blackCount--;
				}
			}
			if (pieceSelected + 14 == movePosition && board[pieceSelected + 7] == Black_Piece)
			{
				board[pieceSelected + 7] = Empty;
				blackCount--;
			}
			if (pieceSelected + 18 == movePosition && board[pieceSelected + 9] == Black_Piece)
			{
				board[pieceSelected + 9] = Empty;
				blackCount--;
			}
			if (pieceSelected + 14 > movePosition || pieceSelected + 18 > movePosition)
			{
				capture(player, pieceSelected + 14, movePosition);
				capture(player, pieceSelected + 18, movePosition);
			}
		}
	}
	if (player == Black_Player)
	{
		if (board[pieceSelected] == Black_Piece)
		{
			if (pieceSelected % 8 == 0)
			{
				if (pieceSelected - 18 == movePosition && board[pieceSelected - 9] == Red_Piece)
				{
					board[pieceSelected - 9] = Empty;
					redCount--;
				}
			}
			if (pieceSelected % 8 == 7)
			{
				if (pieceSelected - 14 == movePosition && board[pieceSelected - 7] == Red_Piece)
				{
					redCount--;
					board[pieceSelected - 7] = Empty;
				}
			}
			if (pieceSelected - 14 == movePosition && board[pieceSelected - 7] == Red_Piece)
			{
				board[pieceSelected - 7] = Empty;
				redCount--;
			}
			if (pieceSelected - 18 == movePosition && board[pieceSelected - 9] == Red_Piece)
			{
				redCount--;
				board[pieceSelected - 9] = Empty;
			}
			if (pieceSelected - 14 > movePosition || pieceSelected - 18 > movePosition)
			{
				capture(player, pieceSelected - 14, movePosition);
				capture(player, pieceSelected - 18, movePosition);
			}
		}
	}
}
void capture(int player, int pieceSelected, int movePosition, int curBoard[], int &black_Count, int &red_Count)
{
	if (pieceSelected == movePosition ||pieceSelected<0||pieceSelected>BOARD_SIZE)return;
	if (player == Red_Player)
	{
		if (curBoard[pieceSelected] == Red_Piece)
		{
			if (pieceSelected % 8 == 0)
			{
				if (pieceSelected + 18 == movePosition && curBoard[pieceSelected + 9] == Black_Piece)
				{
					curBoard[pieceSelected + 9] = Empty;
					black_Count--;
				}
			}
			if (pieceSelected % 8 == 7)
			{
				if (pieceSelected + 14 == movePosition && curBoard[pieceSelected + 7] == Black_Piece)
				{
					curBoard[pieceSelected + 7] = Empty;
					black_Count--;
				}
			}
			if (pieceSelected + 14 == movePosition && curBoard[pieceSelected + 7] == Black_Piece)
			{
				curBoard[pieceSelected + 7] = Empty;
				black_Count--;
			}
			if (pieceSelected + 18 == movePosition && curBoard[pieceSelected + 9] == Black_Piece)
			{
				curBoard[pieceSelected + 9] = Empty;
				black_Count--;
			}
			if (pieceSelected + 14 > movePosition || pieceSelected + 18 > movePosition)
			{
				capture(player, pieceSelected + 14, movePosition, curBoard, black_Count, red_Count);
				capture(player, pieceSelected + 18, movePosition, curBoard, black_Count, red_Count);
			}
		}
	}
	if (player == Black_Player)
	{
		if (curBoard[pieceSelected] == Black_Piece)
		{
			if (pieceSelected % 8 == 0)
			{
				if (pieceSelected - 18 == movePosition && curBoard[pieceSelected - 9] == Red_Piece)
				{
					curBoard[pieceSelected - 9] = Empty;
					red_Count--;
				}
			}
			if (pieceSelected % 8 == 7)
			{
				if (pieceSelected - 14 == movePosition && curBoard[pieceSelected - 7] == Red_Piece)
				{
					red_Count--;
					curBoard[pieceSelected - 7] = Empty;
				}
			}
			if (pieceSelected - 14 == movePosition && curBoard[pieceSelected - 7] == Red_Piece)
			{
				curBoard[pieceSelected - 7] = Empty;
				red_Count--;
			}
			if (pieceSelected - 18 == movePosition && curBoard[pieceSelected - 9] == Red_Piece)
			{
				red_Count--;
				curBoard[pieceSelected - 9] = Empty;
			}
			if (pieceSelected - 14 > movePosition || pieceSelected - 18 > movePosition)
			{
				capture(player, pieceSelected - 14, movePosition, curBoard, black_Count, red_Count);
				capture(player, pieceSelected - 18, movePosition, curBoard, black_Count, red_Count);
			}
		}
	}
}

//selects the location the player wishes the current selected piece to move to 
int selectMove(int player, int pieceSelected)
{
	bool valid=false;
	int move;
	while (!valid)
	{
		cout << "Please enter space you wish to move the piece in " << pieceSelected << " to (-1 to de select piece): ";
		cin >> move;
		if (validMove(player, pieceSelected, move) || move==-1)
		{
			if (canCapture(player, pieceSelected, move))
			{
				capture(player, pieceSelected, move);
			}
			valid = true;
		}
		else cout << "Invalid move please try again\n";
	}
	return move;
}

//updates board with the move
//might have to add convertPiece to King
void makeMove(int selection,int move)
{
	board[move] = board[selection];
	board[selection] = Empty;
}
void makeMove(int selection, int move, int curBoard[])
{
	curBoard[move] = curBoard[selection];
	curBoard[selection] = Empty;
}

//copys board from src to dest
void copyBoard(int src[], int dest[])
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		dest[i] = src[i];
	}
}

//gets computers move
void getComputerMove(int player)
{
	evaluateComputerMove(player, MAX_DEPTH, MIN_INFINITY - 1, MAX_INFINITY + 1, board,blackCount,redCount);
	copyBoard(computer_move, board);
}
//gets human Move
void getHumanMove(int player)
{
	int move = -1;
	int selection;
	while (move == -1)
	{
		selection = selectPiece(player);
		move = selectMove(player, selection);
	}
	makeMove(selection, move);
}
//gets players turn
void Turn(int player)
{
	if (player == Red_Player)getComputerMove(player);
	else getHumanMove(player);
}

//clears the entire board
void clearBoard()
{
	for each (int i in board)
	{
		i = Empty;
	}
}
//checks to see if BlackPLayer can Move at a specific location
//TODO King
bool canBlackMove(int player,int pieceSelected, int curBoard[])
{
	if (board[pieceSelected] == Black_Piece)
	{
		if (pieceSelected % 8 == 0)
		{
			return (curBoard[pieceSelected - 9] == Empty || canCapture(player, pieceSelected, curBoard));
		}
		if (pieceSelected % 8 == 7)
		{
			return(curBoard[pieceSelected - 7] == Empty || canCapture(player, pieceSelected, curBoard));
		}
		if (curBoard[pieceSelected - 7] == Empty || curBoard[pieceSelected - 9] == Empty || canCapture(player, pieceSelected, curBoard))return true;
	}
	return false;
}
//cheks to see if RedPlayer can move at a specific location
//TODO KING
bool canRedMove(int player, int pieceSelected, int curBoard[])
{
	if (curBoard[pieceSelected] == Red_Piece)
	{
		if (pieceSelected % 8 == 0)
		{
			return (curBoard[pieceSelected + 9] == Empty || canCapture(player, pieceSelected, curBoard));
		}
		if (pieceSelected % 8 == 7)
		{
			return(curBoard[pieceSelected + 7] == Empty || canCapture(player, pieceSelected, curBoard));
		}
		if (curBoard[pieceSelected + 7] == Empty || curBoard[pieceSelected + 9] == Empty || canCapture(player, pieceSelected, curBoard))return true;
	}
	return false;
}


//checks to see if the game has been won
bool checkIfWon(int player)
{
	if (player == Black_Player && redCount == 0)return true;
	if (player == Red_Player && blackCount == 0)return true;
	return false;
}

//computer Finds BlackPlayers Move Locations
//TODO King
vector<int> findBlackMoveLocation(int player, int pieceSelected, int curBoard[])
{
	vector<int> result;
	if (curBoard[pieceSelected] == Black_Piece)
	{
		if (pieceSelected % 8 == 0)
		{
			if (curBoard[pieceSelected - 9] == Empty)
			{
				result.push_back(pieceSelected - 9);
			}
			if (canCapture(player, pieceSelected, curBoard))
			{
				result.push_back(pieceSelected - 18);
			}
		}
		if (pieceSelected % 8 == 7)
		{
			if (curBoard[pieceSelected - 7] == Empty)
			{
				result.push_back(pieceSelected - 7);
			}
			if (canCapture(player, pieceSelected, curBoard))
			{
				result.push_back(pieceSelected - 14);
			}
		}
		if (curBoard[pieceSelected - 7] == Empty)
		{
			result.push_back(pieceSelected - 7);
		}
		else
		{
			//Checks to see if it can capture
			if (curBoard[pieceSelected - 7] == Red_Piece && curBoard[pieceSelected - 14] == Empty)
			{
				result.push_back(pieceSelected - 14);
			}
		}
		if (curBoard[pieceSelected - 9] == Empty)
		{
			result.push_back(pieceSelected - 9);
		}
		else
		{
			//Checks to see if it can capture
			if (curBoard[pieceSelected - 9] == Red_Piece && curBoard[pieceSelected - 18] == Empty)
			{
				result.push_back(pieceSelected - 18);
			}
		}
	}
	return result;
}
//computer Finds RedPlayers MoveLocation
//TODO King
vector<int> findRedMoveLocation(int player, int pieceSelected, int curBoard[])
{
	vector<int> result;
	if (curBoard[pieceSelected] == Red_Piece)
	{
		if (pieceSelected % 8 == 0)
		{
			if (curBoard[pieceSelected + 9] == Empty)
			{
				result.push_back(pieceSelected + 9);
			}
			if (canCapture(player, pieceSelected, curBoard))
			{
				result.push_back(pieceSelected + 18);
			}
		}
		if (pieceSelected % 8 == 7)
		{
			if (curBoard[pieceSelected + 7] == Empty)
			{
				result.push_back(pieceSelected + 7);
			}
			if (canCapture(player, pieceSelected, curBoard))
			{
				result.push_back(pieceSelected + 14);
			}
		}
		if (curBoard[pieceSelected + 7] == Empty)
		{
			result.push_back(pieceSelected + 7);
		}
		else
		{
			if (curBoard[pieceSelected + 7] == Black_Piece && curBoard[pieceSelected + 14] == Empty)
			{
				result.push_back(pieceSelected + 14);
			}
		}
		if (curBoard[pieceSelected + 9] == Empty)
		{
			result.push_back(pieceSelected + 9);
		}
		else
		{
			if (curBoard[pieceSelected + 9] == Black_Piece && curBoard[pieceSelected + 18] == Empty)
			{
				result.push_back(pieceSelected + 18);
			}
		}
	}
	return result;
}

//list of all possible moves computer can make
vector<pair<int, int>> findPossibleMoves(int player, int curBoard[])
{
	vector<pair<int, int>> moves;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if (player==Black_Player)
		{
			if (curBoard[i] == Black_King || curBoard[i]==Black_Piece)
			{
				if (canBlackMove(player, i, curBoard))
				{
					vector<int> mlocation = findBlackMoveLocation(player, i, curBoard);
					for each (int j in mlocation)
					{
						if (validMove(player, i, j, curBoard))
						{
							pair<int, int>pMove = { i, j };
							moves.push_back(pMove);
						}
					}
				}
			}
		}
		if (player==Red_Player)
		{
			if (curBoard[i] ==Red_King || curBoard[i] == Red_Piece)
			{
				if (canRedMove(player, i, curBoard))
				{
					vector<int> mlocation = findRedMoveLocation(player, i, curBoard);
					for each (int j in mlocation)
					{
						if (validMove(player, i, j, curBoard))
						{
							pair<int, int>pMove = { i, j };
							moves.push_back(pMove);
						}
					}
				}
			}
		}
	}
	return moves;
}

//evaluates human move
short evaluateHumanMove(int player, int depth, int alpha, int beta, int curboard[],int black_Count, int red_Count)
{
	int  value;
	int new_board[BOARD_SIZE];
	short min = MAX_INFINITY + 1;
	if (depth == 0)return (red_Count - black_Count);
	copyBoard(curboard, new_board);
	/* The computer (max) just made a move, so we evaluate that move here */
	if (checkIfWon(Red_Player))  return MAX_INFINITY;
	vector<pair<int, int>> possibleMoves = findPossibleMoves(player, new_board);
	for each (pair<int, int> pMove in possibleMoves)
	{
		if (canCapture(player, pMove.first, pMove.second, curboard))capture(player, pMove.first, pMove.second, new_board, black_Count, red_Count);
		makeMove(pMove.first, pMove.second, new_board);
		int nextPlayer;
		if (player == Black_Player)nextPlayer = Red_Player;
		else nextPlayer = Black_Player;
		value = evaluateComputerMove(nextPlayer, depth - 1, alpha, beta, new_board,black_Count,red_Count);

		if (value < min)
		{
			min = value;
		}

		if (value < beta) beta = value;

		/* Prune this subtree by not checking any further successors */
		if (alpha >= beta) return beta;
		copyBoard(curboard, new_board);
	}
	return min;
}

//evaluate computer move
short evaluateComputerMove(int player, int depth, int alpha, int beta, int curboard[], int black_Count, int red_Count)
{
	int value;
	int new_board[BOARD_SIZE];
	int max = MIN_INFINITY - 1;
	if (depth == 0)return (red_Count - black_Count);
	copyBoard(curboard, new_board);
	//boards_checked++;

	/* The human (min) just made a move, so we evaluate that move here */
	if (checkIfWon(Black_Player)&& player==Black_Player) return MIN_INFINITY;
	if (checkIfWon(Red_Player) && player ==Red_Player) return MIN_INFINITY;
	vector<pair<int, int>> possibleMoves = findPossibleMoves(player, new_board);
	for each (pair<int, int> pMove in possibleMoves)
	{

		if (canCapture(player, pMove.first, pMove.second, curboard))capture(player, pMove.first, pMove.second, new_board, black_Count, red_Count);
		makeMove(pMove.first, pMove.second, new_board);
		int nextPlayer;
		if (player == Black_Player)nextPlayer = Red_Player;
		else nextPlayer = Black_Player;
		value = evaluateHumanMove(nextPlayer, depth - 1, alpha, beta, new_board,black_Count,red_Count);

		if (value > max)
		{
			max = value;
			if (depth == MAX_DEPTH) copyBoard(new_board, computer_move);
		}

		if (value > alpha) alpha = value;

		/* Prune this subtree by not checking any further successors */
		if (alpha >= beta) return alpha;
		copyBoard(curboard, new_board);
	}
	return max;
}

int main()
{


	int currentPlayer = Black_Player;
	bool win = false;
	intilizeBoard();
	while (!win)
	{
		emitboard();
		Turn(currentPlayer);
		if (win = checkIfWon(currentPlayer))
		{
			if (currentPlayer == Black_Player)cout << "Black wins!\n";
			else cout << "Red wins!\n";
			break;
		}
		if (currentPlayer == Black_Player)currentPlayer = Red_Player;
		else currentPlayer = Black_Player;
	}
	return 0;
}