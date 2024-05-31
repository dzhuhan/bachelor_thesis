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
extern int aom;

extern const int init[8][8];

extern std::vector<int> ans_moves;

extern struct state
{
    int b[8][8];
    
    bool white_kingside_castle_right;
    bool white_queenside_castle_right;
    bool black_kingside_castle_right;
    bool black_queenside_castle_right;
    std::pair<int, int> en_passant;
    
    std::pair<int, int> white_king_pos;
    std::pair<int, int> black_king_pos;
    std::vector <std::pair<int, int>> white_pieces;
    std::vector <std::pair<int, int>> black_pieces;
    std::vector <int> white_moves;
    std::vector <int> black_moves;
} st_g;

#endif // GLOBAL_DATA_H
