#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
#include<chrono>
#include<thread>
using namespace std;

const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;
const int PREVIEW_SIZE = 4;

class Tetris {
private:
    vector<vector<int>> board;
    vector<vector<vector<int>>> pieces;
    vector<vector<int>> currpiece;
    int currpiecetype;
    int currx, curry;
    int score;
    int level;
    int linescleared;
    bool gameover;

    void InitialisePieces()
    {
        pieces.resize(7);
        pieces[0] = {{1, 1, 1, 1}};
        pieces[1] = {{1, 1}, {1, 1}};
        pieces[2] = {{0, 1, 0}, {1, 1, 1}};
        pieces[3] = {{0, 1, 1}, {1, 1, 0}};
        pieces[4] = {{1, 1, 0}, {0, 1, 1}};
        pieces[5] = {{1, 0, 0}, {1, 1, 1}};
        pieces[6] = {{0, 0, 1}, {1, 1, 1}};
    }

    void GotoXY(int x, int y)
    {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void HideCursor()
    {
        CONSOLE_CURSOR_INFO cursorinfo;
        GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorinfo);
        cursorinfo.bVisible = false;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorinfo);
    }

public:
    Tetris()
    {
        board.resize(BOARD_HEIGHT, vector<int>(BOARD_WIDTH, 0));
        InitialisePieces();
        score = 0;
        level = 1;
        linescleared = 0;
        gameover = false;
        srand(time(0));
        HideCursor();
        SpawnNewPiece();
    }

    void SpawnNewPiece()
    {
        currpiecetype = rand() % pieces.size();
        currpiece = pieces[currpiecetype];
        currx = BOARD_WIDTH / 2 - currpiece[0].size() / 2;
        curry = 0;
        if(!isValidPosition(currpiece, currx, curry))
        {
            gameover = true;
        }
    }

    bool isValidPosition(vector<vector<int>>& piece, int x, int y)
    {
        for(int i=0; i < piece.size(); i++)
        {
            for(int j=0; j < piece[i].size(); j++)
            {
                if(piece[i][j])
                {
                    int nx = x + j;
                    int ny = y + i;
                    if(nx < 0 || nx >= BOARD_WIDTH || ny >= BOARD_HEIGHT || (ny >= 0 && board[ny][nx]))
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void RotatePiece()
    {
        vector<vector<int>> rotated(currpiece[0].size(), vector<int>(currpiece.size()));
        for(int i=0; i < currpiece.size(); i++)
        {
            for(int j=0; j < currpiece[i].size(); j++)
            {
                rotated[j][currpiece.size() - 1 - i] = currpiece[i][j];
            }
        }
        if(isValidPosition(rotated, currx, curry))
        {
            currpiece = rotated;
        }
    }

    void MovePiece(int dx, int dy)
    {
        if(isValidPosition(currpiece, currx + dx, curry + dy))
        {
            currx += dx;
            curry += dy;
        }
    }

    void PlacePiece()
    {
        for(int i=0; i < currpiece.size(); i++)
        {
            for(int j=0; j < currpiece[i].size(); j++)
            {
                if(currpiece[i][j])
                {
                    board[curry + i][currx + j] = currpiecetype + 1;
                }
            }
        }
    }

    void ClearLines()
    {
        int count = 0;
        for(int i = BOARD_HEIGHT - 1; i>=0; i--)
        {
            bool full = true;
            for(int j=0; j < BOARD_WIDTH; j++)
            {
                if(board[i][j] == 0)
                {
                    full = false;
                    break;
                }
            }
            if(full)
            {
                board.erase(board.begin() + i);
                board.insert(board.begin(), vector<int>(BOARD_WIDTH, 0));
                count++;
                i++;
            }
        }
        if(count)
        {
            linescleared += count;
            score += count * 100 * level;
            level = 1 + linescleared / 10;
        }
    }

    bool DropPiece()
    {
        if(isValidPosition(currpiece, currx, curry + 1))
        {
            curry++;
            return false;
        }
        else
        {
            PlacePiece();
            ClearLines();
            SpawnNewPiece();
            return true;
        }
    }

    void Draw()
    {
        GotoXY(0, 0);
        vector<vector<int>> display = board;
        for(int i=0; i < currpiece.size(); i++)
        {
            for(int j=0; j < currpiece[i].size(); j++)
            {
                if(currpiece[i][j] && curry + i >= 0)
                {
                    display[curry + i][currx + j] = -1;
                }
            }
        }
        cout << "+";
        for(int j=0; j < BOARD_WIDTH; j++)
        {
            cout << "-";
        }
        cout << "+" << endl;
        for(int i=0; i < BOARD_HEIGHT; i++)
        {
            cout << "|";
            for(int j=0; j < BOARD_WIDTH; j++)
            {
                if(display[i][j] == 0)
                {
                    cout << " ";
                }
                else if(display[i][j] == -1)
                {
                    cout << "#";
                }
                else
                {
                    cout << "X";
                }
            }
            cout << "|";
            if(i == 1)
            {
                cout << "    Score: " << score;
            }
            else if(i == 3)
            {
                cout << "    Level: " << level;
            }
            else if(i == 5)
            {
                cout << "    Lines Cleared: " << linescleared;
            }
            else if(i == 7)
            {
                cout << "    Controls:";
            }
            else if(i == 8)
            {
                cout << "    A/D: Move Left/Right";
            }
            else if(i == 9)
            {
                cout << "    S: Move Down";
            }
            else if(i == 10)
            {
                cout << "    W: Rotate";
            }
            else if(i == 12)
            {
                cout << "    Q: Quit";
            }
            cout << endl;
        }
        cout << "+";
        for(int j=0; j < BOARD_WIDTH; j++)
        {
            cout << "-";
        }
        cout << "+" << endl;
        if(gameover)
        {
            cout << endl << "**GAME OVER**" << endl;
            cout << "Final Score: " << score << endl;
            cout << "Press any key to exit..." << endl;
        }
    }

    void HandleInput()
    {
        if(_kbhit())
        {
            char key = _getch();
            key = tolower(key);
            switch(key)
            {
                case 'a':
                    MovePiece(-1, 0);
                    break;
                case 'd':
                    MovePiece(1, 0);
                    break;
                case 's':
                    MovePiece(0, 1);
                    break;
                case 'w':
                    RotatePiece();
                    break;
                case 'q':
                    gameover = true;
                    break;
            }
        }
    }

    int GetDropDelay()
    {
        return max(50, 500 - (level - 1) * 50);
    }

    bool isGameOver()
    {
        return gameover;
    }

    void Run()
    {
        auto lastdrop = chrono::steady_clock::now();
        while(!gameover)
        {
            HandleInput();
            auto now = chrono::steady_clock::now();
            auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - lastdrop);
            if(elapsed.count() >= GetDropDelay())
            {
                DropPiece();
                lastdrop = now;
            }
            Draw();
            this_thread::sleep_for(chrono::milliseconds(50));
        }
        while(!_kbhit())
        {
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }
};

int main()
{
    cout << "Welcome to TETRIS!" << endl;
    cout << "Controls:" << endl;
    cout << "A/D - Move left/right" << endl;
    cout << "S - Soft drop" << endl;
    cout << "W - Rotate piece" << endl;
    cout << "Q - Quit game" << endl;
    cout << endl << "Press any key to start..." << endl;
    _getch();

    system("cls");
    Tetris tetris;
    tetris.Run();
    return 0;
}