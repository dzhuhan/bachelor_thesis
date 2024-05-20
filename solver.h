#ifndef SOLVER_H
#define SOLVER_H

#include "global_data.h"
#include <utility>
#include <vector>
#include <iostream>
#include <sstream>

class Solver
{
public:
    static void update_state(state &st, const int move);
    static void get_moves(state &st, std::vector<int> &moves, const int r0, const int c0);

    static bool legal_move(state &st, const int move);
    static bool possible_move(state &st, const int move);
    static bool square_is_safe(state &st, bool s, const int r0, const int c0);
    static void promotion(state &st, const int r0, const int c0);
    static int get_value(int v);

    static void sort_by_check(bool s, state st, std::vector<int> &m);
    static void find_mate_in(state st, const int m, bool s);

    static void helpmate(state st, const int m, bool s);
    static void other_problem(state st, const int m, bool s);
    static void print_state(state st);
};

#endif // SOLVER_H
