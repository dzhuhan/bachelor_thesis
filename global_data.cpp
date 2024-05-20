#include <string>
#include <sstream>
#include <utility>
#include <vector>

int white_pieces_value;
int black_pieces_value;
int value_diff;
int advantage;
long count;

bool ans;
std::vector <int> ans_moves;

int init[8][8] =
    {{-4,-2,-3,-5,-6,-3,-2,-4},
    {-1,-1,-1,-1,-1,-1,-1,-1},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {4, 2, 3, 5, 6, 3, 2, 4}};

struct state
{
    int b[8][8] =
    {{-4,-2,-3,-5,-6,-3,-2,-4},
    {-1,-1,-1,-1,-1,-1,-1,-1},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {4, 2, 3, 5, 6, 3, 2, 4}};   
    
    bool white_kinside_castle_right = true;
    bool white_quenside_castle_right = true;
    bool black_kinside_castle_right = true;
    bool black_quenside_castle_right = true;
    std::pair<int, int> en_passant = std::make_pair(-1, -1);
    
    std::pair<int, int> white_king_pos;
    std::pair<int, int> black_king_pos;
    std::vector <std::pair<int, int>> white_pieces;
    std::vector <std::pair<int, int>> black_pieces;
    std::vector <int> white_moves;
    std::vector <int> black_moves;
} st_g;
