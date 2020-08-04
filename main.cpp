#include <iostream>
#include <ostream>
#include <vector>
#include <memory>
#include <ctime>
#include <Windows.h>

class Bacteria
{
private:
    int _age;

public:
    Bacteria()
    {
        _age = 0;
    }

    ~Bacteria() = default;

    inline void set_age(int age)
    {
        _age = age;
    }
    inline int get_age() { return _age; }
    inline void print_body()
    {
        std::cout << "O ";
    }
    inline bool is_toOld()
    {
        return _age > 5;
    }
    void became_older()
    {
        _age++;
    }
};

class Board
{
private:
    int _height, _width;
    std::vector<std::vector<std::unique_ptr<Bacteria>>> _board;

public:
    Board(const int width, const int height)
    {
        _height = height;
        _width = width;
        _board.resize(width);
        for (auto &x : _board)
        {
            x.resize(height);
        }
    }

    void print_board()
    {
        // system("CLS");
        for (auto &x : _board)
        {
            for (auto &y : x)
            {
                if (y != nullptr)
                {
                    y->print_body();
                }
                else
                {
                    std::cout << "  ";
                }
            }
            std::cout << std::endl;
        }
    }
    inline void summon_bacteria(int x, int y)
    {
        _board[x][y].reset(new Bacteria);
    }
    inline const bool is_outOfRange(int x, int y)
    {
        return (x < 0 || x >= _width || y < 0 || y >= _height);
    }
    int how_many_neigh(int x, int y)
    {
        int cur_neighboors = 0;
        for (int y_alpha = -1; y_alpha <= 1; ++y_alpha)
        {
            for (int x_alpha = -1; x_alpha <= 1; ++x_alpha)
            {
                if (x_alpha == 0 && y_alpha == 0)
                {
                    continue;
                }
                if (!is_outOfRange(x_alpha + x, y_alpha + y))
                {
                    // else if bacteria at [x_alpha + x][y_alpha + y] exist
                    if (_board[x_alpha + x][y_alpha + y].get())
                    {
                        cur_neighboors += 1;
                    }
                }
            }
        }
        return cur_neighboors;
    }

    void copy_board(const std::vector<std::vector<std::unique_ptr<Bacteria>>> &other)
    {
        for (auto x = 0; x < _width; ++x)
        {
            for (auto y = 0; y < _height; ++y)
            {
                if (other[x][y] == nullptr)
                {
                    _board[x][y].reset(nullptr);
                }
                else
                {
                    _board[x][y].reset(new Bacteria);
                    _board[x][y]->set_age(other[x][y]->get_age());
                }
            }
        }
    }

    void update_neighbors()
    {
        std::vector<std::vector<std::unique_ptr<Bacteria>>> temp_board;
        temp_board.resize(_width);
        for (auto &x : temp_board)
        {
            x.resize(_width);
        }
        for (int x = 0; x < _width; x++)
        {
            for (int y = 0; y < _height; y++)
            {
                int num_neigh = how_many_neigh(x, y);

                if (num_neigh == 3 && _board[x][y] == nullptr)
                {
                    temp_board[x][y].reset(new Bacteria);
                }
                else if (_board[x][y] != nullptr && (num_neigh < 2 || num_neigh > 3))
                {
                    temp_board[x][y].reset(nullptr);
                }
                else if (_board[x][y].get())
                {
                    temp_board[x][y].reset(new Bacteria);
                    temp_board[x][y]->set_age(_board[x][y]->get_age() + 1);
                }
                else
                {
                    temp_board[x][y].reset(nullptr);
                }
            }
        }
        copy_board(temp_board);
    }
};

int main()
{
    srand(time(NULL));
    int board_length = 10;
    Board life_sim(board_length, board_length);
    int population_max = int(board_length * board_length / 2);
    srand(time(NULL));
    for (int population = 0; population < population_max; population++)
    {
        int x = rand() % board_length;
        int y = rand() % board_length;
        life_sim.summon_bacteria(x, y);
    }

    // while spacebard is not pressed.

    // life_sim.summon_bacteria(0, 0);
    // life_sim.summon_bacteria(1, 0);
    // life_sim.summon_bacteria(0, 1);

    while (true)
    {
        life_sim.print_board();
        life_sim.update_neighbors();
        Sleep(1000);
    }

    std::cout << "END PROGRAM \n";
    return 0;
}