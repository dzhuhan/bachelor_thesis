#ifndef SOLVER_H
#define SOLVER_H

#include "global_data.h"
#include <utility>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

class MainWindow;

class Solver
{
public:
    static bool alpha;
    static double step;
    static double progress_steps;
    static MainWindow *mw;
    static void get_ui(MainWindow *ui);
    static void get_progress_rate(state st, bool s);
    static void update_state(state &st, const int move);
    static void get_moves(state &st, std::vector<int> &moves, const int r0, const int c0);

    static bool legal_move(state &st, const int move);
    static bool possible_move(state &st, const int move);
    static bool square_is_safe(state &st, bool s, const int r0, const int c0);

    static void sort_by_check(bool s, state st, std::vector<int> &m);
    static std::vector<int> find_mate_in(state st, const int m, bool s, bool t);

    static void helpmate_heuristic(state &st);
    static void helpmate(state st, const int m, bool s);
    
    static void selfmate_heuristic(state &st);
    static std::vector<int> selfmate(state st, const int m, bool s);
    static void print_state(state st);
};

#endif // SOLVER_H
