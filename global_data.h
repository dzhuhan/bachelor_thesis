#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#include <string>
#include <sstream>
#include <utility>
#include <vector>

extern bool ans;
extern long count;

extern int white_pieces_value;
extern int black_pieces_value;
extern int value_diff;
extern int advantage;

extern int b_copy[8][8];
extern const int init[8][8];

extern std::vector <std::vector<int>> ans_moves;

extern struct state
{
    int b[8][8];
    std::pair<int, int> en_passant;
    std::pair<int, int> white_king_pos;
    std::pair<int, int> black_king_pos;
    std::vector <std::pair<int, int>> white_pieces;
    std::vector <std::pair<int, int>> black_pieces;
    std::vector <std::vector<int>> white_moves;
    std::vector <std::vector<int>> black_moves;
} st_g;

#endif // GLOBAL_DATA_H
