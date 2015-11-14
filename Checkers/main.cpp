#include <iostream>
#include <vector>

using namespace std;


/*NOTES:
Black always moves first
Black starts at bottom
Red starts at top
No Double jump
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
const int MAX_INFINITY = 24;
const int MIN_INFINITY = -24;
int MAX_DEPTH;
int computer_move[BOARD_SIZE];
short evaluateComputerMove(int , int , int , int , int[], int,int );
short evaluateHumanMove(int , int , int , int , int[],int ,int );



bool isRedKing(int location, int curBoard[])
{
	if (location<0 || location>BOARD_SIZE)return false;
	return (curBoard[location] == Red_King);
}
bool isRedPiece(int location, int curBoard[])
{
	if (location<0 || location>BOARD_SIZE)return false;
	return (curBoard[location] == Red_Piece);
}
bool isBlackKing(int location, int curBoard[])
{
	if (location<0 || location>BOARD_SIZE)return false;
	return(curBoard[location] == Black_King);
}
bool isBlackPiece(int location, int curBoard[])
{
	if (location<0 || location>BOARD_SIZE)return false;
	return(curBoard[location] == Black_Piece);
}
bool isEmpty(int location, int curBoard[])
{
	if (location<0 || location>BOARD_SIZE)return false;
	return(curBoard[location] == Empty);
}
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

bool validPieceSelection(int player, int selection, int curBoard[])
{
	if (selection < 0 || selection>BOARD_SIZE)return false;
	if (player == Red_Player)
	{
		return (curBoard[selection] == Red_Piece || curBoard[selection] == Red_King);
	}
	if (player == Black_Player)
	{
		return (curBoard[selection] == Black_Piece || curBoard[selection] == Black_King);
	}
	return false;
}

//select the piece the player wishes to move
int selectPiece(int player)
{
	int selection;
	bool valid = false;
	while (!valid)
	{
		cout << "Please enter space you wish to select (0-top right, 63-bottom left): ";
		cin >> selection;
		if (validPieceSelection(player, selection,board)) valid = true;
		else
		{
			if (board[selection] == Empty)cout << "That space is empty please try again\n";
			else cout << "That is not your piece please try again\n";
		}
	}
	return selection;
}

//checks to see if Black is/can Capture a Red Piece
bool canBlackCapture(int pieceSelected, int movePosition, int curBoard[])
{
	//if (pieceSelected - 14 < 0 || pieceSelected + 14 > BOARD_SIZE || pieceSelected - 18 < 0 || pieceSelected + 18 > BOARD_SIZE)return false;
//	if ((pieceSelected + 7) % 8 == 0 || (pieceSelected + 7) % 8 == 7 || (pieceSelected - 7) % 8 == 0 || (pieceSelected + 7) % 8 == 7) return false;
//	if ((pieceSelected + 9) % 8 == 0 || (pieceSelected + 9) % 8 == 7 || (pieceSelected - 9) % 8 == 0 || (pieceSelected + 9) % 8 == 7) return false;
	if (movePosition < 0 || movePosition>BOARD_SIZE || pieceSelected < 0 || pieceSelected>BOARD_SIZE)return false; //moves off board
	if (pieceSelected % 8 == 0 && movePosition % 8 == 7)return false; //checks that the piece wont go from one side to the other
	if (pieceSelected % 8 == 7 && movePosition % 8 == 0)return false;//checks that the piece wont go from one side to the other
//	if (pieceSelected + 7 > BOARD_SIZE || pieceSelected + 9 > BOARD_SIZE || pieceSelected - 7 < 0 || pieceSelected - 9 < 0) return false;
	if (!isEmpty(movePosition,curBoard))return false; //checks to see if that location is empty
	if (pieceSelected % 8 == 1 && (movePosition == pieceSelected + 14 || movePosition == pieceSelected - 18))return false;
	if (pieceSelected % 8 == 6 && (movePosition == pieceSelected - 14 || movePosition == pieceSelected + 18))return false;
	//spots adjacent to pieceSelected where a piece may be
	const int upleft = pieceSelected - 9;
	const int upright = pieceSelected - 7;
	const int downleft = pieceSelected + 7;
	const int downright = pieceSelected + 9;
	bool result = false;
	if (isBlackPiece(pieceSelected,curBoard))
	{
		if (pieceSelected - 14 >= 0)
		{
			if (pieceSelected - 14 == movePosition &&
				(isRedPiece(upright, curBoard) || isRedKing(upright, curBoard)))result =  true;
			//if (pieceSelected - 14 > movePosition)
			//{
			//	result =  (canBlackCapture(pieceSelected - 14, movePosition, curBoard));
			//}
		}
		if (pieceSelected - 18 >= 0)
		{
			if (pieceSelected - 18 == movePosition &&
				(isRedPiece(upleft, curBoard) || isRedKing(upleft, curBoard)))result =  true;

			//if (pieceSelected - 18 > movePosition)result =  canBlackCapture(pieceSelected - 18, movePosition, curBoard);
		}
	}
	if (isBlackKing(pieceSelected, curBoard))
	{
		if (pieceSelected - 14 >= 0)
		{
			if (pieceSelected - 14 == movePosition &&
				(isRedPiece(pieceSelected - 7, curBoard) || isRedKing(pieceSelected - 7, curBoard))) result =  true;
		}
		if (pieceSelected - 18 >= 0)
		{
			if (pieceSelected - 18 == movePosition &&
				(isRedPiece(pieceSelected - 9, curBoard) || isRedKing(pieceSelected - 9, curBoard)))result =  true;
		}
		if (pieceSelected + 14 < BOARD_SIZE)
		{
			if (pieceSelected + 14 == movePosition &&
				(isRedPiece(pieceSelected + 7, curBoard) || isRedKing(pieceSelected + 7, curBoard)))result =  true;
		}
		if (pieceSelected + 18 < BOARD_SIZE)
		{
			if (pieceSelected + 18 == movePosition &&
				(isRedPiece(pieceSelected + 9, curBoard) || isRedKing(pieceSelected + 9, curBoard)))result =  true;
		}
		//if (pieceSelected - 14 > movePosition)return (canBlackCapture(pieceSelected - 14, movePosition, curBoard));
		//if (pieceSelected - 18 > movePosition) return canBlackCapture(pieceSelected - 18, movePosition, curBoard);
		//if (pieceSelected + 14 < movePosition) return canBlackCapture(pieceSelected + 14, movePosition, curBoard);
		//if (pieceSelected + 18 < movePosition) return canBlackCapture(pieceSelected + 18, movePosition, curBoard);
	}
	return result;
}
bool canBlackCapture(int pieceSelected, int curBoard[])
{
	//if ((pieceSelected + 7) % 8 == 0 || (pieceSelected + 7) % 8 == 7 || (pieceSelected - 7) % 8 == 0 || (pieceSelected + 7) % 8 == 7) return false;
//	if ((pieceSelected + 9) % 8 == 0 || (pieceSelected + 9) % 8 == 7 || (pieceSelected - 9) % 8 == 0 || (pieceSelected + 9) % 8 == 7) return false;
	if (pieceSelected - 14 < 0 || pieceSelected + 14 > BOARD_SIZE || pieceSelected - 18 < 0 || pieceSelected + 18 > BOARD_SIZE)return false;
	if (pieceSelected<0 || pieceSelected>BOARD_SIZE)return false;
	if (pieceSelected + 7 > BOARD_SIZE || pieceSelected + 9 > BOARD_SIZE || pieceSelected - 7 < 0 || pieceSelected - 9 < 0) return false;
	if (board[pieceSelected] == Black_Piece)
	{
		if (((curBoard[pieceSelected - 7] == Red_Piece || curBoard[pieceSelected-7]==Red_King) && curBoard[pieceSelected - 14] == Empty) ||
			((curBoard[pieceSelected - 9] == Red_Piece || curBoard[pieceSelected-9]==Red_King) && curBoard[pieceSelected - 18] == Empty)) return true;
	}
	if (board[pieceSelected] == Black_King)
	{
		if ((isRedKing(pieceSelected - 7, curBoard) || isRedPiece(pieceSelected - 7, curBoard))
			&& isEmpty(pieceSelected - 14, curBoard)) return true;
		if ((isRedKing(pieceSelected - 9, curBoard) || isRedPiece(pieceSelected - 9, curBoard))
			&& isEmpty(pieceSelected - 18, curBoard)) return true;
		if ((isRedKing(pieceSelected + 7, curBoard) || isRedPiece(pieceSelected + 7, curBoard))
			&& isEmpty(pieceSelected + 14, curBoard)) return true;
		if ((isRedKing(pieceSelected + 9, curBoard) || isRedPiece(pieceSelected + 9, curBoard))
			&& isEmpty(pieceSelected + 18, curBoard)) return true;
	}
	return false;
}
//checks to see if Red is/can Capture a Black Piece
bool canRedCapture(int pieceSelected, int movePosition, int curBoard[])
{
	//if (pieceSelected - 14 < 0 || pieceSelected + 14 > BOARD_SIZE || pieceSelected - 18 < 0 || pieceSelected + 18 > BOARD_SIZE)return false;
	//if ((pieceSelected + 7) % 8 == 0 || (pieceSelected + 7) % 8 == 7 || (pieceSelected - 7) % 8 == 0 || (pieceSelected + 7) % 8 == 7) return false;
	//if ((pieceSelected + 9) % 8 == 0 || (pieceSelected + 9) % 8 == 7 || (pieceSelected - 9) % 8 == 0 || (pieceSelected + 9) % 8 == 7) return false;
	if (movePosition < 0 || movePosition>BOARD_SIZE || pieceSelected < 0 || pieceSelected>BOARD_SIZE)return false; //moves off board
	if (pieceSelected % 8 == 0 && movePosition % 8 == 7)return false; //checks that the piece wont go from one side to the other
	if (pieceSelected % 8 == 7 && movePosition % 8 == 0)return false;//checks that the piece wont go from one side to the other
//	if (pieceSelected + 7 > BOARD_SIZE || pieceSelected + 9 > BOARD_SIZE || pieceSelected - 7 < 0 || pieceSelected - 9 < 0) return false;
	if (pieceSelected % 8 == 1 && (movePosition == pieceSelected + 14 || movePosition == pieceSelected - 18))return false;
	if (pieceSelected % 8 == 6 && (movePosition == pieceSelected - 14 || movePosition == pieceSelected + 18))return false;
	if (curBoard[movePosition] != Empty)return false; //checks to see if that location is empty
	bool result = false;
	if (curBoard[pieceSelected] == Red_Piece)
	{

		if (pieceSelected + 14 < BOARD_SIZE)
		{
			if (pieceSelected + 14 == movePosition &&
				(curBoard[pieceSelected + 7] == Black_Piece || curBoard[pieceSelected + 7] == Black_King))result= true;
			/*if (pieceSelected + 14 > movePosition)
			{
				result= (canRedCapture(pieceSelected + 14, movePosition, curBoard));
			}*/
		}
		if (pieceSelected + 18 < BOARD_SIZE)
		{
			if (pieceSelected + 18 == movePosition &&
				(curBoard[pieceSelected + 9] == Black_Piece || curBoard[pieceSelected + 9] == Black_King))result= true;

			//if (pieceSelected + 18 > movePosition)
			//{
			//	result= canRedCapture(pieceSelected + 18, movePosition, curBoard);
			//}
		}
	}
	if (curBoard[pieceSelected] == Red_King)
	{
		if (pieceSelected + 14 < BOARD_SIZE)
		{
			if (pieceSelected + 14 == movePosition &&
				(curBoard[pieceSelected + 7] == Black_Piece || curBoard[pieceSelected + 7] == Black_King))result= true;
		}
		if (pieceSelected + 18 < BOARD_SIZE)
		{
			if (pieceSelected + 18 == movePosition &&
				(curBoard[pieceSelected + 9] == Black_Piece && curBoard[pieceSelected + 9] == Black_King))result= true;
		}
		if (pieceSelected - 14 >= 0)
		{
			if (pieceSelected - 14 == movePosition &&
				(curBoard[pieceSelected - 7] == Black_Piece || curBoard[pieceSelected - 7] == Black_King))result= true;
		}
		if (pieceSelected - 18 >= 0)
		{
			if (pieceSelected - 18 == movePosition &&
				(curBoard[pieceSelected - 9] == Black_Piece && curBoard[pieceSelected - 9] == Black_King))result= true;
		}
		//if (pieceSelected + 14 > movePosition)return (canRedCapture(pieceSelected + 14, movePosition, curBoard));
		//if (pieceSelected + 18 > movePosition)return canRedCapture(pieceSelected + 18, movePosition, curBoard);
		//if (pieceSelected - 14 < movePosition)return canRedCapture(pieceSelected - 14, movePosition, curBoard);
		//if (pieceSelected - 18 < movePosition)return canRedCapture(pieceSelected - 18, movePosition, curBoard);
	}
	return result;
}
bool canRedCapture(int pieceSelected,  int curBoard[])
{
	if (pieceSelected - 14 < 0 || pieceSelected + 14 > BOARD_SIZE || pieceSelected - 18 < 0 || pieceSelected + 18 > BOARD_SIZE)return false;
	if (pieceSelected<0 || pieceSelected>BOARD_SIZE)return false;
	//if ((pieceSelected + 7) % 8 == 0 || (pieceSelected + 7) % 8 == 7 || (pieceSelected - 7) % 8 == 0 || (pieceSelected + 7) % 8 == 7) return false;
	//if ((pieceSelected + 9) % 8 == 0 || (pieceSelected + 9) % 8 == 7 || (pieceSelected - 9) % 8 == 0 || (pieceSelected + 9) % 8 == 7) return false;
	if (pieceSelected + 7 > BOARD_SIZE || pieceSelected + 9 > BOARD_SIZE || pieceSelected - 7 < 0 || pieceSelected - 9 < 0) return false;
	if (board[pieceSelected] == Red_Piece)
	{
		if ((curBoard[pieceSelected + 7] == Black_Piece || curBoard[pieceSelected + 7] == Black_King) &&
			curBoard[pieceSelected + 14] == Empty) return true;
		if ((curBoard[pieceSelected + 9] == Black_Piece || curBoard[pieceSelected + 9] == Black_King) &&
			curBoard[pieceSelected + 18] == Empty) return true;
	}

	if (board[pieceSelected]==Red_King)
	{
		if ((curBoard[pieceSelected + 7] == Black_Piece || curBoard[pieceSelected + 7] == Black_King) &&
			curBoard[pieceSelected + 14] == Empty) return true;
		if ((curBoard[pieceSelected + 9] == Black_Piece || curBoard[pieceSelected + 9] == Black_King) &&
			curBoard[pieceSelected + 18] == Empty) return true;
		if ((curBoard[pieceSelected - 7] == Black_Piece || curBoard[pieceSelected - 7] == Black_King) &&
			curBoard[pieceSelected - 14] == Empty) return true;
		if ((curBoard[pieceSelected - 9] == Black_Piece || curBoard[pieceSelected - 9] == Black_King) &&
			curBoard[pieceSelected - 18] == Empty) return true;
	}
	return false;
}
//checks to see if the piece is/can capture another piece
bool canCapture(int player, int pieceSelected, int movePosition, int curBoard[])
{
	if (movePosition < 0 || movePosition>BOARD_SIZE || pieceSelected < 0 || pieceSelected>BOARD_SIZE)return false; //moves off board
	if (pieceSelected % 8 == 0 && movePosition % 8 == 7)return false; //checks that the piece wont go from one side to the other
	if (pieceSelected % 8 == 7 && movePosition % 8 == 0)return false;//checks that the piece wont go from one side to the other
	if (curBoard[movePosition] != Empty)return false; //checks to see if that location is empty
	if (player == Red_Player)
	{
		return canRedCapture(pieceSelected, movePosition, curBoard);
	}
	if (player == Black_Player)
	{
		return canBlackCapture(pieceSelected, movePosition, curBoard);
	}
	return false;
}
bool canCapture(int player, int pieceSelected, int curBoard[])
{
	if (player == Black_Player)
	{
		return canBlackCapture(pieceSelected, curBoard);
	}
	if (player == Red_Player)
	{
		return canRedCapture(pieceSelected, curBoard);
	}
	return false;
}

//checks to see if Blacks move is a valid move
bool validBlackMove(int pieceSelected, int movePosition, int curBoard[])
{
	if (curBoard[pieceSelected] == Black_Piece)
	{
		if (pieceSelected % 8 == 0)
		{
			if (pieceSelected - 7 == movePosition)return true;
		}
		if (pieceSelected % 8 == 7)
		{
			if  (pieceSelected - 9 == movePosition) return true;
		}
		if (pieceSelected - 7 == movePosition || pieceSelected - 9 == movePosition)return true;
		if (pieceSelected - 7 > movePosition || pieceSelected - 9 > movePosition)return canBlackCapture( pieceSelected, movePosition, curBoard);
	}
	if (curBoard[pieceSelected] == Black_King)
	{
		if (pieceSelected % 8 == 0)
		{
			if (pieceSelected - 7 == movePosition || pieceSelected + 9 == movePosition)return true;
		}
		if (pieceSelected % 8 == 7)
		{
			if (pieceSelected - 9 == movePosition || pieceSelected + 7 == movePosition) return true;
		}
		if (pieceSelected - 7 == movePosition || pieceSelected - 9 == movePosition ||
			pieceSelected + 7 == movePosition || pieceSelected + 9 == movePosition)return true;
		if (pieceSelected - 7 > movePosition || pieceSelected - 9 > movePosition ||
			pieceSelected + 7 < movePosition || pieceSelected + 9 < movePosition)return canBlackCapture(pieceSelected, movePosition, curBoard);
	}
	return false;
}
//checks to see if Red move is a valid move
bool validRedMove(int pieceSelected, int movePosition, int curBoard[])
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
			return canRedCapture(pieceSelected, movePosition, curBoard);
		}
	}
	if (curBoard[pieceSelected] == Red_King)
	{
		if (pieceSelected % 8 == 0)
		{
			if (pieceSelected - 7 == movePosition || pieceSelected + 9 == movePosition)return true;
		}
		if (pieceSelected % 8 == 7)
		{
			if (pieceSelected - 7 == movePosition || pieceSelected + 9 == movePosition) return true;
		}
		if (pieceSelected - 7 == movePosition || pieceSelected - 9 == movePosition ||
			pieceSelected + 7 == movePosition || pieceSelected + 9 == movePosition)return true;
		if (pieceSelected - 7 > movePosition || pieceSelected - 9 > movePosition ||
			pieceSelected + 7 < movePosition || pieceSelected + 9 < movePosition)return canRedCapture(pieceSelected, movePosition, curBoard);
	}
	return false;
}
//checks to see if the move is valid


bool validMove(int player, int pieceSelected, int movePosition, int curBoard[])
{
	if (movePosition < 0 || movePosition>BOARD_SIZE)return false; //moves off board
	if (pieceSelected % 8 == 0 && movePosition % 8 == 7)return false; //checks that the piece wont go from one side to the other
	if (pieceSelected % 8 == 7 && movePosition % 8 == 0)return false;//checks that the piece wont go from one side to the other
	if (curBoard[movePosition] != Empty)return false; //checks to see if that location is empty
	if (player == Red_Player)
	{
		return validRedMove(pieceSelected, movePosition, curBoard);
	}
	if (player == Black_Player)
	{
		return validBlackMove(pieceSelected, movePosition, curBoard);
	}
	return false;
}

//removes the Black piece(s) being captured that Red Captures  from the board
void RedCapture(int pieceSelected, int movePosition, int curBoard[], int &black_Count)
{
	if (pieceSelected == movePosition || pieceSelected<0 || pieceSelected>BOARD_SIZE)return;
	if (movePosition<0 || movePosition>BOARD_SIZE)return;
	if (pieceSelected - 7 < 0 || pieceSelected - 9 < 0 || pieceSelected + 7 > BOARD_SIZE || pieceSelected + 9 > BOARD_SIZE) return;
	if (curBoard[pieceSelected] == Red_Piece)
	{
		if (pieceSelected + 14 == movePosition && 
			(curBoard[pieceSelected + 7] == Black_Piece || curBoard[pieceSelected + 7] == Black_King))
		{
			if (curBoard[pieceSelected + 7] == Black_Piece)black_Count--;
			if (curBoard[pieceSelected + 7] == Black_King)black_Count -= 2;
			curBoard[pieceSelected + 7] = Empty;
		}
		if (pieceSelected + 18 == movePosition && 
			(curBoard[pieceSelected + 9] == Black_Piece || curBoard[pieceSelected + 9] == Black_King))
		{
			if (curBoard[pieceSelected + 9] == Black_Piece)black_Count--;
			if (curBoard[pieceSelected + 9] == Black_King)black_Count -= 2;
			curBoard[pieceSelected + 9] = Empty;
		}
		if (pieceSelected + 14 > movePosition || pieceSelected + 18 > movePosition)
		{
			RedCapture(pieceSelected + 14, movePosition, curBoard, black_Count);
			RedCapture(pieceSelected + 18, movePosition, curBoard, black_Count);
		}
	}
	if (curBoard[pieceSelected] == Red_King)
	{
		if (pieceSelected + 14 == movePosition &&
			(curBoard[pieceSelected + 7] == Black_Piece || curBoard[pieceSelected + 7] == Black_King))
		{
			if (curBoard[pieceSelected + 7] == Black_Piece)black_Count--;
			if (curBoard[pieceSelected + 7] == Black_King)black_Count -= 2;
			curBoard[pieceSelected + 7] = Empty;
		}
		if (pieceSelected + 18 == movePosition &&
			(curBoard[pieceSelected + 9] == Black_Piece || curBoard[pieceSelected + 9] == Black_King))
		{
			if (curBoard[pieceSelected + 9] == Black_Piece)black_Count--;
			if (curBoard[pieceSelected + 9] == Black_King)black_Count -= 2;
			curBoard[pieceSelected + 9] = Empty;
		}
		if (pieceSelected - 14 == movePosition &&
			(curBoard[pieceSelected - 7] == Black_Piece || curBoard[pieceSelected - 7] == Black_King))
		{
			if (curBoard[pieceSelected - 7] == Black_Piece)black_Count--;
			if (curBoard[pieceSelected - 7] == Black_King)black_Count -= 2;
			curBoard[pieceSelected - 7] = Empty;
		}
		if (pieceSelected - 18 == movePosition &&
			(curBoard[pieceSelected - 9] == Black_Piece || curBoard[pieceSelected - 9] == Black_King))
		{
			if (curBoard[pieceSelected - 9] == Black_Piece)black_Count--;
			if (curBoard[pieceSelected - 9] == Black_King)black_Count -= 2;
			curBoard[pieceSelected - 9] = Empty;
		}
	}
}
//removes the Red piece(s) being captured that Black Captures from the board
void BlackCapture(int pieceSelected, int movePosition, int curBoard[], int &red_Count)
{
	if (pieceSelected == movePosition || pieceSelected<0 || pieceSelected>BOARD_SIZE)return;
	if (movePosition<0 || movePosition>BOARD_SIZE)return;
	if (pieceSelected - 7 < 0 || pieceSelected - 9 < 0 || pieceSelected + 7 > BOARD_SIZE || pieceSelected + 9 > BOARD_SIZE) return;
	if (curBoard[pieceSelected] == Black_Piece)
	{
		if (pieceSelected - 14 == movePosition && 
			(curBoard[pieceSelected - 7] == Red_Piece || curBoard[pieceSelected - 7] == Red_King))
		{
			if (curBoard[pieceSelected - 7] == Red_Piece)red_Count--;
			if (curBoard[pieceSelected - 7] == Red_King)red_Count -= 2;
			curBoard[pieceSelected - 7] = Empty;
		}
		if (pieceSelected - 18 == movePosition && 
			(curBoard[pieceSelected - 9] == Red_Piece || curBoard[pieceSelected - 9] == Red_King))
		{
			if (curBoard[pieceSelected - 9] == Red_Piece)red_Count--;
			if (curBoard[pieceSelected - 9] == Red_King)red_Count -= 2;
			curBoard[pieceSelected - 9] = Empty;
		}
		if (pieceSelected - 14 > movePosition)
		{
			BlackCapture(pieceSelected - 14, movePosition, curBoard, red_Count);
		}
		if (pieceSelected - 18 > movePosition)
		{
			BlackCapture(pieceSelected - 18, movePosition, curBoard, red_Count);
		}
	}
	if (curBoard[pieceSelected]==Black_King)
	{
		if (pieceSelected - 14 == movePosition &&
			(curBoard[pieceSelected - 7] == Red_Piece || curBoard[pieceSelected - 7] == Red_King))
		{
			if (curBoard[pieceSelected - 7] == Red_Piece)red_Count--;
			if (curBoard[pieceSelected - 7] == Red_King)red_Count -= 2;
			curBoard[pieceSelected - 7] = Empty;
		}
		if (pieceSelected - 18 == movePosition &&
			(curBoard[pieceSelected - 9] == Red_Piece || curBoard[pieceSelected - 9] == Red_King))
		{
			if (curBoard[pieceSelected - 9] == Red_Piece)red_Count--;
			if (curBoard[pieceSelected - 9] == Red_King)red_Count -= 2;
			curBoard[pieceSelected - 9] = Empty;
		}
		if (pieceSelected + 14 == movePosition && curBoard[pieceSelected + 7] == Red_Piece)
		{
			if (curBoard[pieceSelected + 7] == Red_Piece)red_Count--;
			if (curBoard[pieceSelected +7] == Red_King)red_Count -= 2;
			curBoard[pieceSelected + 7] = Empty;
		}
		if (pieceSelected + 18 == movePosition && curBoard[pieceSelected + 9] == Red_Piece)
		{
			if (curBoard[pieceSelected +9] == Red_Piece)red_Count--;
			if (curBoard[pieceSelected +9] == Red_King)red_Count -= 2;
			curBoard[pieceSelected + 9] = Empty;
		}
	}
}
//removes the piece(s) being captured from the board
void capture(int player, int pieceSelected, int movePosition, int curBoard[], int &black_Count, int &red_Count)
{
	if (pieceSelected == movePosition ||pieceSelected<0||pieceSelected>BOARD_SIZE)return;
	if (player == Red_Player)
	{
		RedCapture(pieceSelected, movePosition, curBoard, black_Count);
	}
	if (player == Black_Player)
	{
		BlackCapture(pieceSelected, movePosition, curBoard, red_Count);
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
		if (validMove(player, pieceSelected, move, board) || move==-1)
		{
			if (canCapture(player, pieceSelected, move, board))
			{
				capture(player, pieceSelected, move, board, blackCount, redCount);
			}
			valid = true;
		}
		else cout << "Invalid move please try again\n";
	}
	return move;
}

void king(int location, int curBoard[], int& Red_Count, int& Black_Count)
{
	if (location < BOARD_SIZE && location >= BOARD_SIZE - 8)
	{
		if (curBoard[location] == Red_Piece)
		{
			curBoard[location] = Red_King;
			Red_Count++;
		}
	}
	if (location >= 0 && location < 8)
	{
		if (curBoard[location] == Black_Piece)
		{
			curBoard[location] = Black_King;
			Black_Count++;
		}
	}
}
//checks to see if a piece can be kinged
bool canKing(int location, int curBoard[])
{
	if (location < 8 && location >= 0)
	{
		return(curBoard[location] == Black_Piece);
	}
	if (location >= BOARD_SIZE - 8 && location < BOARD_SIZE)
	{
		return(curBoard[location] == Red_Piece);
	}
	return false;
}
//updates board with the move
void makeMove(int selection, int move, int curBoard[], int &Red_Count, int &Black_Count)
{
	curBoard[move] = curBoard[selection];
	curBoard[selection] = Empty;
	if (canKing(move, curBoard))king(move, curBoard, Red_Count, Black_Count);
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
	makeMove(selection, move,board,redCount,blackCount);
}
//gets players turn
//GameType
//0 Player vs Player
//1 Player vs Computer
//2 Computer vs Player
//3 Computer vs Computer
void Turn(int player, int gameType)
{
	switch (gameType)
	{
	case(0) :
		getHumanMove(player);
		break;
	case(1):
		if (player == Red_Player)getComputerMove(player);
		else getHumanMove(player);
		break;
	case(2) :
		if (player == Black_Player)getComputerMove(player);
		else getHumanMove(player);
		break;
	case(3) :
		getComputerMove(player);
		break;
	default:
		break;
	}


	//getComputerMove(player);
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
bool canBlackMove(int pieceSelected, int curBoard[])
{
	if (canBlackCapture(pieceSelected, curBoard))return true;
	if (board[pieceSelected] == Black_Piece)
	{
		if (pieceSelected % 8 == 0)
		{
			if (curBoard[pieceSelected - 7] == Empty) return true;
			return false;
		}
		if (pieceSelected % 8 == 7)
		{
			if (curBoard[pieceSelected - 9] == Empty)return true;
			return false;
		}
		if (pieceSelected - 7 >= 0)
		{
			if (curBoard[pieceSelected - 7] == Empty) return true;
		}
		if (pieceSelected - 9 >= 0)
		{
			if (curBoard[pieceSelected - 9] == Empty )return true;
		}
	}
	if (curBoard[pieceSelected] == Black_King)
	{
		if (pieceSelected % 8 == 0)
		{
			if (curBoard[pieceSelected - 7] == Empty) return true;
			if (curBoard[pieceSelected + 9] == Empty)return true;
			return false;
		}
		if (pieceSelected % 8 == 7)
		{
			if (curBoard[pieceSelected - 9] == Empty)return true;
			if (curBoard[pieceSelected + 7] == Empty)return true;
			return false;
		}
		if (pieceSelected - 7 >= 0)
		{
			if (curBoard[pieceSelected - 7] == Empty) return true;
		}
		if (pieceSelected - 9 >= 0)
		{

			if (curBoard[pieceSelected - 9] == Empty )return true;
		}
		if (pieceSelected + 7 <BOARD_SIZE)
		{
			if (curBoard[pieceSelected + 7] == Empty) return true;
		}
		if (pieceSelected + 9 < BOARD_SIZE)
		{
			if (curBoard[pieceSelected + 9] == Empty)return true;
		}
	}
	return false;
}
//cheks to see if RedPlayer can move at a specific location
bool canRedMove(int pieceSelected, int curBoard[])
{
	if (canRedCapture(pieceSelected, curBoard))return true;
	if (curBoard[pieceSelected] == Red_Piece)
	{
		if (pieceSelected % 8 == 0)
		{
			if(curBoard[pieceSelected + 9] == Empty)return true;
			return false;
		}
		if (pieceSelected % 8 == 7)
		{
			if(curBoard[pieceSelected + 7] == Empty)return true;
			return false;
		}
		if (pieceSelected + 7 <BOARD_SIZE)
		{
			if (curBoard[pieceSelected + 7] == Empty) return true;
		}
		if (pieceSelected + 9 < BOARD_SIZE)
		{
			if (curBoard[pieceSelected + 9] == Empty)return true;
			if (curBoard[pieceSelected - 9] == Empty) return true;
		}
	}
	if (curBoard[pieceSelected] == Red_King)
	{
		if (pieceSelected % 8 == 0)
		{
			if (curBoard[pieceSelected + 9] == Empty)return true;
			if (curBoard[pieceSelected - 7] == Empty)return true;
			return false;
		}
		if (pieceSelected % 8 == 7)
		{
			if (curBoard[pieceSelected + 7] == Empty)return true;
			return false;
		}
		if (pieceSelected - 7 >= 0)
		{
			if (curBoard[pieceSelected - 7] == Empty) return true;
		}
		if (pieceSelected - 9 >= 0)
		{
			if (curBoard[pieceSelected - 9] == Empty)return true;
		}
		if (pieceSelected + 7 <BOARD_SIZE)
		{
			if (curBoard[pieceSelected + 7] == Empty) return true;
		}
		if (pieceSelected + 9 < BOARD_SIZE)
		{
			if (curBoard[pieceSelected + 9] == Empty)return true;
		}
	}
	return false;
}
//checks to see if specific player can move
bool canMove(int player, int pieceSelected, int curBoard[])
{
	if (player == Black_Player)return canBlackMove( pieceSelected, curBoard);
	if (player == Red_Player)return canRedMove( pieceSelected, curBoard);
	return false;
}


//checks to see if the game has been won
bool checkIfWon(int player, int Black_Count, int Red_Count, int curBoard[])
{
	if (player == Black_Player)
	{
		if (Red_Count == 0) return true;
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			if(canMove(Red_Player, i, curBoard))return false;
		}
		return true;
	}
	if (player == Red_Player)
	{
		if (Black_Count == 0) return true;
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			if (canMove(Black_Player, i, curBoard))return false;
		}
		return true;
	}
	return false;
}

//finds all possible move locations for the black piece selected
vector<int> findBlackPieceMoveLocations(int pieceSelected, int curBoard[])
{
	vector<int> result;
	if (curBoard[pieceSelected - 7] == Empty)
	{
		result.push_back(pieceSelected - 7);
	}
	else
	{
		//Checks to see if it can capture
		if ((curBoard[pieceSelected - 7] == Red_Piece || curBoard[pieceSelected - 7] == Red_King) &&
			curBoard[pieceSelected - 14] == Empty)
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
		if ((curBoard[pieceSelected - 9] == Red_Piece || curBoard[pieceSelected - 9] == Red_King) &&
			curBoard[pieceSelected - 18] == Empty)
		{
			result.push_back(pieceSelected - 18);
		}
	}
	return result;
}
//finds all possible move locations for the black king selected
vector<int> findBlackKingMovesLocations(int pieceSelected, int curBoard[])
{
	vector<int> result;
	if (curBoard[pieceSelected - 7] == Empty)
	{
		result.push_back(pieceSelected - 7);
	}
	else
	{
		if ((curBoard[pieceSelected - 7] == Red_Piece || curBoard[pieceSelected - 7] == Red_King) &&
			curBoard[pieceSelected - 14] == Empty)
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
		if ((curBoard[pieceSelected - 9] == Red_Piece || curBoard[pieceSelected - 9] == Red_King) &&
			curBoard[pieceSelected - 18] == Empty)
		{
			result.push_back(pieceSelected - 18);
		}
	}
	if (curBoard[pieceSelected + 7] == Empty)
	{
		result.push_back(pieceSelected + 7);
	}
	else
	{
		if ((curBoard[pieceSelected + 7] == Red_Piece || curBoard[pieceSelected + 7] == Red_King) &&
			curBoard[pieceSelected + 14] == Empty)
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
		//Checks to see if it can capture
		if ((curBoard[pieceSelected + 9] == Red_Piece || curBoard[pieceSelected + 9] == Red_King) &&
			curBoard[pieceSelected + 18] == Empty)
		{
			result.push_back(pieceSelected + 18);
		}
	}
	return result;
}
//computer Finds BlackPlayers Move Locations
vector<int> findBlackMoveLocation(int pieceSelected, int curBoard[])
{
	vector<int> result;
	vector<int> pieceMoves;
	vector<int> kingMoves;
	if (curBoard[pieceSelected] == Black_Piece)
	{
		pieceMoves = findBlackPieceMoveLocations(pieceSelected, curBoard);
	}
	for each (int move in pieceMoves)
	{
		result.push_back(move);
	}
	if (curBoard[pieceSelected]==Black_King)
	{
		kingMoves = findBlackKingMovesLocations(pieceSelected, curBoard);
	}
	for each (int move in kingMoves)
	{
		result.push_back(move);
	}
	return result;
}

//finds all possible move locations for the red piece selected
vector<int> findRedPieceMoveLocations(int pieceSelected, int curBoard[])
{
	vector<int> result;
	if (curBoard[pieceSelected + 7] == Empty)
	{
		result.push_back(pieceSelected + 7);
	}
	else
	{
		if ((curBoard[pieceSelected + 7] == Black_Piece || curBoard[pieceSelected + 7] == Black_King) &&
			curBoard[pieceSelected + 14] == Empty)
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
		if ((curBoard[pieceSelected + 9] == Black_Piece || curBoard[pieceSelected + 9] == Black_King) &&
			curBoard[pieceSelected + 18] == Empty)
		{
			result.push_back(pieceSelected + 18);
		}
	}
	return result;
}
//finds all possible move locations for the red king selected
vector<int> findRedKingMoveLocations(int pieceSelected, int curBoard[])
{
	vector<int> result;
	if (curBoard[pieceSelected + 7] == Empty)
	{
		result.push_back(pieceSelected + 7);
	}
	else
	{
		if ((curBoard[pieceSelected + 7] == Black_Piece || curBoard[pieceSelected + 7] == Black_King) &&
			curBoard[pieceSelected + 14] == Empty)
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
		if ((curBoard[pieceSelected + 9] == Black_Piece || curBoard[pieceSelected + 9] == Black_King) &&
			curBoard[pieceSelected + 18] == Empty)
		{
			result.push_back(pieceSelected + 18);
		}
	}
	if (curBoard[pieceSelected - 7] == Empty)
	{
		result.push_back(pieceSelected - 7);
	}
	else
	{
		if ((curBoard[pieceSelected - 7] == Black_Piece || curBoard[pieceSelected - 7] == Black_King) &&
			curBoard[pieceSelected - 14] == Empty)
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
		if ((curBoard[pieceSelected - 9] == Black_Piece || curBoard[pieceSelected - 9] == Black_King) &&
			curBoard[pieceSelected - 18] == Empty)
		{
			result.push_back(pieceSelected - 18);
		}
	}
	return result;
}
//computer Finds RedPlayers MoveLocation
vector<int> findRedMoveLocation( int pieceSelected, int curBoard[])
{
	vector<int> result;
	vector<int> pieceMoves;
	vector<int> kingMoves;
	if (curBoard[pieceSelected] == Red_Piece)
	{
		pieceMoves = findRedPieceMoveLocations(pieceSelected, curBoard);
	}
	for each (int move in pieceMoves)
	{
		result.push_back(move);
	}
	if (curBoard[pieceSelected] == Red_King)
	{
		kingMoves = findRedKingMoveLocations(pieceSelected, curBoard);
	}
	for each (int move in kingMoves)
	{
		result.push_back(move);
	}
	return result;
}
//finds a list of all possible moves location a specific player can make
vector<int> findMoveLocation(int player, int pieceSelected, int curBoard[])
{
	vector<int> result;
	if (player == Red_Player)return findRedMoveLocation( pieceSelected, curBoard);
	if (player == Black_Player)return findBlackMoveLocation( pieceSelected, curBoard);
	return result;
}

//list of all possible moves a player can make
vector<pair<int, int>> findPossibleMoves(int player, int curBoard[])
{
	vector<pair<int, int>> moves;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		if (validPieceSelection(player, i, curBoard))
		{
			if (canMove(player, i, curBoard))
			{
				vector<int> mlocation = findMoveLocation(player, i, curBoard);
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
	return moves;
}

//evaluates human move
short evaluateHumanMove(int player, int depth, int alpha, int beta, int curboard[],int black_Count, int red_Count)
{
	int  value;
	int new_board[BOARD_SIZE];
	short min = MAX_INFINITY + 1;
	copyBoard(curboard, new_board);
	vector<pair<int, int>> possibleMoves = findPossibleMoves(player, new_board);
	if (depth == 0)return possibleMoves.size();
	/* The computer (max) just made a move, so we evaluate that move here */
	if (checkIfWon(player,black_Count,red_Count,curboard)) return MAX_INFINITY;

	for each (pair<int, int> pMove in possibleMoves)
	{
		if (canCapture(player, pMove.first, pMove.second, curboard))capture(player, pMove.first, pMove.second, new_board, black_Count, red_Count);
		makeMove(pMove.first, pMove.second, new_board, red_Count,black_Count);
		int nextPlayer;
		if (player == Black_Player)nextPlayer = Red_Player;
		else nextPlayer = Black_Player;
		value = evaluateComputerMove(nextPlayer, depth - 1, alpha, beta, new_board,black_Count,red_Count);

		if (value <= min)
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
	copyBoard(curboard, new_board);
	vector<pair<int, int>> possibleMoves = findPossibleMoves(player, new_board);
	if (depth == 0)return possibleMoves.size();//(red_Count - black_Count);

	//boards_checked++;

	/* The human (min) just made a move, so we evaluate that move here */
	if (checkIfWon(player, black_Count, red_Count, curboard)) return MIN_INFINITY;
	
	for each (pair<int, int> pMove in possibleMoves)
	{
		if (canCapture(player, pMove.first, pMove.second, curboard))capture(player, pMove.first, pMove.second, new_board, black_Count, red_Count);
		makeMove(pMove.first, pMove.second, new_board, red_Count,black_Count);
		int nextPlayer;
		if (player == Black_Player)nextPlayer = Red_Player;
		else nextPlayer = Black_Player;
		value = evaluateHumanMove(nextPlayer, depth - 1, alpha, beta, new_board,black_Count,red_Count);

		if (value >= max)
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

int menu()
{
	cout << "Game Types (Black vs Red):\n"
		<< "0: Player vs Player\n"
		<< "1: Player vs Computer\n"
		<< "2: Computer vs Player\n"
		<< "3: Computer vs Computer\n"
		<<"Select Game Type: ";
	int input;
	cin >> input;
	switch (input)
	{
	case(1) :
	case(2) :
	case(3) :
		cout << "Select Difficulty (1-10 where 1 being easy and 10 being very hard): ";
		cin >> MAX_DEPTH;
		break;
	default:
		break;
	}
	return input;
}

int main()
{
	int currentPlayer = Black_Player;
	bool win = false;
	intilizeBoard();
	int in;
	in = menu();
	emitboard();
	while (!win)
	{
		if (currentPlayer == Black_Player)cout << "Blacks Turn:\n";
		else cout << "Reds Turn:\n";

		Turn(currentPlayer, in);
		emitboard();
		if (win = checkIfWon(currentPlayer,blackCount,redCount,board))
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