#include <iostream>
#include <vector>

using namespace std;
/*TODO:
*King and movement
*min max with alpha beta (only selects first move possible)
*/

const int BOARD_SIZE = 64;
int board[BOARD_SIZE];
int redCount;
int blackCount;
const int Black_Piece = 1;
const int Red_Piece = 2;
const int Empty = 0;
const int MAX_INFINITY = 1;
const int DRAW = 0;
const int MIN_INFINITY = -1;
const int MAX_CHILD_NODES = 9;
int computer_move[BOARD_SIZE];
short evaluateComputerMove(int , int , int , int , int[]);
short evaluateHumanMove(int , int , int , int , int[]);

//displays current state of board
void emitboard()
{
	for (int i = 0; i < BOARD_SIZE; i++)
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
	if (selection < 0 || selection>BOARD_SIZE)
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
	if (movePosition < 0 || movePosition>BOARD_SIZE || pieceSelected < 0 || pieceSelected>BOARD_SIZE)return false; //moves off board
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
	
	if (movePosition < 0 || movePosition>BOARD_SIZE)return false; //moves off board
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
	if (player == Black_Piece)
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

void getComputerMove(int player)
{
	evaluateComputerMove(player, 0, MIN_INFINITY - 1, MAX_INFINITY + 1, board);
	copyBoard(computer_move, board);
}
//gets players turn
void Turn(int player)
{
	if (player == Red_Piece)getComputerMove(player);
	else
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
}

//clears the entire board
void clearBoard()
{
	//for each (int i in board)
	//{
	//	i = Empty;
	//}
}
//computer checks to see if it can capture a piece from specifiec location
bool canCapture(int player, int pieceSelected, int curBoard[])
{
	if (player == Black_Piece)
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
	if (player == Red_Piece)
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
	return false;
}
//checks to see if there are any moves avaliabe at specifiec location
bool ifcanMove(int player, int pieceSelected, int curBoard[])
{
	if (player == Black_Piece)
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
	if (player == Red_Piece)
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
	if (player == Black_Piece && redCount == 0)return true;
	if (player == Red_Piece && blackCount == 0)return true;
	return false;
}

//computer finds move location
vector<int> findMoveLocation(int player, int pieceSelected, int curBoard[])
{
	vector<int> result;
	if (player == Black_Piece)
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
			if (curBoard[pieceSelected-7]==Red_Piece && curBoard[pieceSelected-14]==Empty)
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
			if (curBoard[pieceSelected - 9] == Red_Piece && curBoard[pieceSelected - 18] == Empty)
			{
				result.push_back(pieceSelected - 18);
			}
		}
	}
	if (player == Red_Piece)
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
		if (curBoard[i] == player)
		{
			if (ifcanMove(player, i,curBoard))
			{
				vector<int> mlocation=findMoveLocation(player, i, curBoard);
				for each (int j in mlocation)
				{
					pair<int, int>pMove = { i, j };
					moves.push_back(pMove);
				}
			}
		}
	}
	return moves;
}

//evaluates human move
short evaluateHumanMove(int player, int depth, int alpha, int beta, int curboard[])
{
	int  value;
	int new_board[BOARD_SIZE];
	short min = MAX_INFINITY + 1;


	//boards_checked++;

	/* The computer (max) just made a move, so we evaluate that move here */
	if (player == Red_Piece && checkIfWon(Black_Piece))  return MAX_INFINITY;
	vector<pair<int, int>> possibleMoves = findPossibleMoves(player, new_board);
	for each (pair<int, int> pMove in possibleMoves)
	{
		makeMove(pMove.first, pMove.second, new_board);
		int nextPlayer;
		if (player == Black_Piece)nextPlayer = Red_Piece;
		else nextPlayer = Black_Piece;
		value = evaluateComputerMove(nextPlayer, depth + 1, alpha, beta, new_board);

		if (value < min)
		{
			min = value;
			//	if (depth == 0) computer_move = i;
		}

		if (value < beta) beta = value;

		/* Prune this subtree by not checking any further successors */
		if (alpha >= beta) return beta;
	}
	return min;
}

//evaluate computer move
short evaluateComputerMove(int player, int depth, int alpha, int beta, int curboard[])
{
	int value;
	int new_board[BOARD_SIZE];
	int max = MIN_INFINITY - 1;
	copyBoard(curboard, new_board);
	//boards_checked++;

	/* The human (min) just made a move, so we evaluate that move here */
	if (player == Black_Piece && checkIfWon(Red_Piece)) return MIN_INFINITY;
	vector<pair<int, int>> possibleMoves = findPossibleMoves(player, new_board);
	for each (pair<int, int> pMove in possibleMoves)
	{
		makeMove(pMove.first, pMove.second, new_board);
		int nextPlayer;
		if (player == Black_Piece)nextPlayer = Red_Piece;
		else nextPlayer = Black_Piece;
		value = evaluateHumanMove(nextPlayer, depth + 1, alpha, beta, new_board);

		if (value > max)
		{
			max = value;
			if (depth == 0) copyBoard(new_board, computer_move);
		}

		if (value > alpha) alpha = value;

		/* Prune this subtree by not checking any further successors */
		if (alpha >= beta) return alpha;
	}
	return max;
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
		if (win = checkIfWon(currentPlayer))
		{
			if (currentPlayer == Black_Piece)cout << "Black wins!\n";
			else cout << "Red wins!\n";
			break;
		}
		if (currentPlayer == Black_Piece)currentPlayer = Red_Piece;
		else currentPlayer = Black_Piece;
	}
	return 0;
}