#include <iostream>
using namespace std;
/*TODO:
*King and movement
*min max with alpha beta
*check to see if game has been won

*/

int board[64];
const int Black_Piece = 1;
const int Red_Piece = 2;
const int Empty = 0;
//displays current state of board
void emitboard()
{
	for (int i = 0; i < 64; i++)
	{
		if (i % 8 == 0)cout << "\n----------------------------------------\n";
		if (board[i] == Empty) cout << "|   |";
		if (board[i] == Black_Piece) cout << "| B |";
		if (board[i] == Red_Piece) cout << "| R |";
	}
	cout << "\n----------------------------------------\n";
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
			}
		}
		else
		{
			if (i % 2 == 0)
			{
				board[i] = Red_Piece;
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
			}
		}
		else
		{
			if (i % 2 == 1 )
			{
				board[i] = Black_Piece;
			}
		}
	}
}

//intilizesBoard to starting configuration
void intilizeBoard()
{
	
	for (int i = 0; i < 64; i++)
	{
		board[i] = Empty;
		PlaceBlackPiece(i);
		PlaceRedPiece(i);
	}
}

//determines if the selected piece is valid
bool validPieceSelection(int player, int selection)
{
	if (selection < 0 || selection>64)
	{
		throw - 1;
		return false;
	}
	return board[selection] == player;
}

//select the piece the player wishes to move
int selectPiece(int player)
{
	int selection;
	bool valid = false;
	if (player == Black_Piece)cout << "Blacks Turn:\n";
	else cout << "Reds Turn\n";
	while (!valid)
	{
		try{
			cout << "Please enter space you wish to select (0-top right, 63-bottom left): ";
			cin >> selection;
			if (validPieceSelection(player, selection)) valid = true;
			else
			{
				if (board[selection] == Empty)cout << "That space is empty please try again\n";
				else cout << "That is not your piece please try again\n";
			}
		}
		catch (int e)
		{
			cout << "Invalid Input\n";
		}
	}
	return selection;
}

//checks to see if the piece is capturing
bool canCapture(int player, int pieceSelected, int movePosition)
{
	if (movePosition < 0 || movePosition>64 || pieceSelected < 0 || pieceSelected>64)return false; //moves off board
	if (pieceSelected % 8 == 0 && movePosition % 8 == 7)return false; //checks that the piece wont go from one side to the other
	if (pieceSelected % 8 == 7 && movePosition % 8 == 0)return false;//checks that the piece wont go from one side to the other
	if (board[movePosition] != Empty)return false; //checks to see if that location is empty
	if (player == Red_Piece)
	{
		if (pieceSelected % 8 == 0)
		{
			if (pieceSelected + 18 == movePosition && board[pieceSelected + 9] == Black_Piece)return true;
		}
		if (pieceSelected % 8 == 7)
		{
			if (pieceSelected + 14 == movePosition && board[pieceSelected + 7] == Black_Piece)return true;
		}
		if ((pieceSelected + 14 == movePosition && board[pieceSelected+7]==Black_Piece)
			|| (pieceSelected + 18 == movePosition && board[pieceSelected + 9] == Black_Piece))return true;
		if (pieceSelected + 14 > movePosition || pieceSelected + 18 > movePosition)
		{
			return (canCapture(player, pieceSelected + 14, movePosition)
				|| canCapture(player, pieceSelected + 18, movePosition));
		}
	}
	if (player == Black_Piece)
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
	return false;
}

//checks to see if the move is valid
bool validMove(int player, int pieceSelected, int movePosition)
{
	
	if (movePosition < 0 || movePosition>64)return false; //moves off board
	if (pieceSelected % 8 == 0 && movePosition % 8 == 7)return false; //checks that the piece wont go from one side to the other
	if (pieceSelected % 8 == 7 && movePosition % 8 == 0)return false;//checks that the piece wont go from one side to the other
	if (board[movePosition] != Empty)return false; //checks to see if that location is empty
	if (player == Red_Piece)
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
	if (player = Black_Piece)
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

	return false;
}

//removes the piece(s) being captured from the board
void capture(int player, int pieceSelected, int movePosition)
{
	if (player == Red_Piece)
	{
		if (pieceSelected % 8 == 0)
		{
			if (pieceSelected + 18 == movePosition && board[pieceSelected + 9] == Black_Piece)board[pieceSelected + 9] = Empty;
		}
		if (pieceSelected % 8 == 7)
		{
			if (pieceSelected + 14 == movePosition && board[pieceSelected + 7] == Black_Piece)board[pieceSelected + 7] = Empty;
		}
		if (pieceSelected + 14 == movePosition && board[pieceSelected + 7] == Black_Piece)board[pieceSelected + 7] = Empty;
		if(pieceSelected + 18 == movePosition && board[pieceSelected + 9] == Black_Piece)board[pieceSelected + 9] = Empty;
		if (pieceSelected + 14 > movePosition || pieceSelected + 18 > movePosition)
		{
			capture(player, pieceSelected + 14, movePosition);
			capture(player, pieceSelected + 18, movePosition);
		}
	}
	if (player == Black_Piece)
	{
		if (pieceSelected % 8 == 0)
		{
			if (pieceSelected - 18 == movePosition && board[pieceSelected - 9] == Red_Piece)board[pieceSelected - 9] = Empty;
		}
		if (pieceSelected % 8 == 7)
		{
			if (pieceSelected - 14 == movePosition && board[pieceSelected - 7] == Red_Piece)board[pieceSelected - 7] = Empty;
		}
		if (pieceSelected - 14 == movePosition && board[pieceSelected - 7] == Red_Piece)board[pieceSelected - 7] = Empty;
		if (pieceSelected - 18 == movePosition && board[pieceSelected - 9] == Red_Piece)board[pieceSelected - 9] = Empty;
		if (pieceSelected - 14 > movePosition || pieceSelected - 18 > movePosition)
		{
			capture(player, pieceSelected - 14, movePosition);
			capture(player, pieceSelected - 18, movePosition);
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
void makeMove(int selection,int move)
{
	board[move] = board[selection];
	board[selection] = Empty;
}

//gets players turn
void Turn(int player)
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

//clears the entire board
void clearBoard()
{
	for each (int i in board)
	{
		i = Empty;
	}
}
int main()
{
	int currentPlayer = Black_Piece;
	bool win = false;
	intilizeBoard();
	while (!win)
	{
		emitboard();
		Turn(currentPlayer);
		if (currentPlayer == Black_Piece)currentPlayer = Red_Piece;
		else currentPlayer = Black_Piece;
	}
	return 0;
}