#include <iostream>
#include <vector>
#include <deque>
using namespace std;

enum class PieceType
{
    X,
    O
};

class PlayerPiece
{
public:
    PieceType pieceType;
    PlayerPiece(PieceType pieceType)
    {
        this->pieceType = pieceType;
    }
};

class PlayingPieceX : public PlayerPiece
{
public:
    PlayingPieceX() : PlayerPiece(PieceType::X) {}
};

class PlayingPieceO : public PlayerPiece
{
public:
    PlayingPieceO() : PlayerPiece(PieceType::O) {}
};

class Board
{
    int size;
    vector<vector<PlayerPiece *>> board;

public:
    Board(int size)
    {
        this->size = size;
        board.resize(size, vector<PlayerPiece *>(size, nullptr));
    }

    int getSize()
    {
        return size;
    }

    vector<vector<PlayerPiece *>> getBoard()
    {
        return board;
    }

    void printBoard(vector<vector<PlayerPiece *>> board)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (board[i][j] == nullptr)
                {
                    cout << "-";
                }
                else
                {
                    cout << (board[i][j]->pieceType == PieceType::X ? "X" : "O");
                }
                cout << " ";
            }
            cout << endl;
        }
    }
};

class Player
{
    string name;
    PlayerPiece *piece;

public:
    Player(string name, PlayerPiece *piece)
    {
        this->name = name;
        this->piece = piece;
    }

    string getName()
    {
        return name;
    }

    PlayerPiece *getPiece()
    {
        return piece;
    }
};

class TicTacToe
{
    Board *board;
    deque<Player *> players;

public:
    TicTacToe()
    {
        initGame();
    }

    void initGame()
    {
        board = new Board(3);
        players.push_back(new Player("Player 1", new PlayingPieceX()));
        players.push_back(new Player("Player 2", new PlayingPieceO()));
    }

    string startGame()
    {
        bool noWinner = true;
        while (noWinner)
        {
            for (auto player : players)
            {
                cout << player->getName() << " turn" << endl;
                int row, col;
                cout << "Enter row and column: ";
                cin >> row >> col;
                if (board->getBoard()[row][col] == nullptr)
                {
                    board->getBoard()[row][col] = player->getPiece();
                    board->printBoard(board->getBoard());
                }
                else
                {
                    cout << "Invalid move" << endl;
                    continue;
                }
                if (checkWinner())
                {
                    cout << "Hey this the the winner";
                    noWinner = false;
                    return player->getName();
                }
            }
        }
        return "Tie";
    }

    bool checkWinner()
    {
        auto currBoard = board->getBoard();
        int n = board->getSize();
        // check rows
        for (int i = 0; i < n; i++)
        {
            if (currBoard[i][0] != nullptr && currBoard[i][0] == currBoard[i][1] && currBoard[i][1] == currBoard[i][2])
            {
                return true;
            }
        }
        // check columns
        for (int i = 0; i < n; i++)
        {
            if (currBoard[0][i] != nullptr && currBoard[0][i] == currBoard[1][i] && currBoard[1][i] == currBoard[2][i])
            {
                return true;
            }
        }
        // check diagonals
        if (currBoard[0][0] != nullptr && currBoard[0][0] == currBoard[1][1] && currBoard[1][1] == currBoard[2][2])
        {
            return true;
        }
        if (currBoard[0][2] != nullptr && currBoard[0][2] == currBoard[1][1] && currBoard[1][1] == currBoard[2][0])
        {
            return true;
        }
        return false;
    }
};

int main()
{
    TicTacToe game;
    cout << game.startGame() << " wins" << endl;
    return 0;
}
