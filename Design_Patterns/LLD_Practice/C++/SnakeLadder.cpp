#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <map>

using namespace std;

const int BOARD_SIZE = 10;
const int SNAKE = -1;
const int LADDER = 1;

pair<int, int> getRandomPosition(int boardSize)
{
    return {rand() % boardSize, rand() % boardSize};
}

class Board
{
private:
    vector<vector<int>> board;
    map<int, int> transitions;
    vector<pair<int, int>> snakes;
    vector<pair<int, int>> ladders;

    void placeSnakesAndLadders()
    {
        int numSnakes = 5;
        int numLadders = 5;

        while (numSnakes > 0)
        {
            pair<int, int> head = getRandomPosition(BOARD_SIZE);
            pair<int, int> tail = getRandomPosition(BOARD_SIZE);
            int headIdx = head.first * BOARD_SIZE + head.second;
            int tailIdx = tail.first * BOARD_SIZE + tail.second;

            if (headIdx > tailIdx && transitions.find(headIdx) == transitions.end() && transitions.find(tailIdx) == transitions.end())
            {
                transitions[headIdx] = tailIdx;
                board[head.first][head.second] = SNAKE;
                snakes.push_back(head);
                numSnakes--;
            }
        }

        while (numLadders > 0)
        {
            pair<int, int> bottom = getRandomPosition(BOARD_SIZE);
            pair<int, int> top = getRandomPosition(BOARD_SIZE);
            int bottomIdx = bottom.first * BOARD_SIZE + bottom.second;
            int topIdx = top.first * BOARD_SIZE + top.second;

            if (bottomIdx < topIdx && transitions.find(bottomIdx) == transitions.end() && transitions.find(topIdx) == transitions.end())
            {
                transitions[bottomIdx] = topIdx;
                board[bottom.first][bottom.second] = LADDER;
                ladders.push_back(bottom);
                numLadders--;
            }
        }
    }

public:
    Board()
    {
        board.resize(BOARD_SIZE, vector<int>(BOARD_SIZE, 0));
        placeSnakesAndLadders();
    }

    int getNextPosition(int currentPosition)
    {
        if (transitions.find(currentPosition) != transitions.end())
        {
            if (transitions[currentPosition] > currentPosition)
                cout << "Voila, you stepped on a Ladder!" << endl;
            else
                cout << "Oouch, you got bite by a Snake." << endl;
            return transitions[currentPosition];
        }
        return currentPosition;
    }

    void printBoard()
    {
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[i][j] == SNAKE)
                {
                    cout << "X ";
                }
                else if (board[i][j] == LADDER)
                {
                    cout << "| ";
                }
                else
                {
                    cout << ". ";
                }
            }
            cout << endl;
        }

        cout << "Snakes: ";
        for (const pair<int, int> &snake : snakes)
        {
            cout << "(" << snake.first << ", " << snake.second << ") ";
        }
        cout << endl;

        cout << "Ladders: ";
        for (const pair<int, int> &ladder : ladders)
        {
            cout << "(" << ladder.first << ", " << ladder.second << ") ";
        }
        cout << endl;
    }
};

class Player
{
private:
    string name;
    int position;

public:
    Player(string name) : name(name), position(0) {}

    string getName() const { return name; }
    int getPosition() const { return position; }

    void move(int steps)
    {
        position += steps;
        if (position >= 100)
        {
            position = 100;
        }
    }

    void setPosition(int pos)
    {
        position = pos;
    }
};

class Game
{
private:
    Board board;
    vector<Player> players;
    int currentPlayerIndex;

    int rollDice()
    {
        return (rand() % 6) + 1;
    }

public:
    Game(vector<string> playerNames)
    {
        for (const string &name : playerNames)
        {
            players.emplace_back(name);
        }
        currentPlayerIndex = 0;
        srand(time(nullptr));
    }

    void play()
    {
        while (true)
        {
            int n;
            cout << "Enter 1 to continue, 0 to exit: ";
            cin >> n;
            if (n == 0)
            {
                break;
            }
            Player &currentPlayer = players[currentPlayerIndex];
            cout << currentPlayer.getName() << "'s turn. Current position: " << currentPlayer.getPosition() << endl;

            int diceRoll = rollDice();
            cout << "Rolled a " << diceRoll << endl;

            currentPlayer.move(diceRoll);
            int newPosition = board.getNextPosition(currentPlayer.getPosition());
            currentPlayer.setPosition(newPosition);

            cout << currentPlayer.getName() << " moved to position " << currentPlayer.getPosition() << endl;

            if (currentPlayer.getPosition() == 100)
            {
                cout << currentPlayer.getName() << " wins!" << endl;
                break;
            }

            currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
        }
    }

    void printBoard()
    {
        board.printBoard();
    }
};

int main()
{
    vector<string> playerNames = {"Alice", "Bob"};
    Game game(playerNames);

    game.printBoard();
    game.play();

    return 0;
}
