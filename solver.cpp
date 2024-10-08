#include "solver.h"
#include "mainwindow.h"

bool Solver::alpha;
double Solver::step;
double Solver::progress_steps;
MainWindow *Solver::mw;

void Solver::get_ui(MainWindow *ui)
{
    Solver::mw = ui;
}

void Solver::get_progress_rate(state st, bool s)
{
    alpha = true;
    step = 1;
    progress_steps = 0;
    state next;
    if(s == true)
    {
        for(auto e: st.white_moves)
        {
            next = st;
            update_state(next, e);
            progress_steps += next.black_moves.size();
            std::cout << next.black_moves.size() << " " << progress_steps << " ";
        }
    }
    else
    {
        for(auto e: st.black_moves)
        {
            next = st;
            update_state(next, e);
            progress_steps += next.white_moves.size();
        }
    }
}

void Solver::update_state(state &st, const int move)
{
    const int r0 = move / 1000 % 10;
    const int c0 = move / 100 % 10;
    const int r = move / 10 % 10;
    const int c = move % 10;
    
    if(st.b[r0][c0] > 0)
    {
        st.white_pieces.erase(std::remove(st.white_pieces.begin(), st.white_pieces.end(), std::make_pair(r0, c0)));
        st.white_pieces.push_back(std::make_pair(r, c));
        if(st.b[r][c] < 0)
            st.black_pieces.erase(std::remove(st.black_pieces.begin(), st.black_pieces.end(), std::make_pair(r, c)));
    }
    else if(st.b[r0][c0] < 0)
    {
        st.black_pieces.erase(std::remove(st.black_pieces.begin(), st.black_pieces.end(), std::make_pair(r0, c0)));
        st.black_pieces.push_back(std::make_pair(r, c));
        if(st.b[r][c] > 0)
            st.white_pieces.erase(std::remove(st.white_pieces.begin(), st.white_pieces.end(), std::make_pair(r, c)));
    }
    st.b[r][c] = st.b[r0][c0];
    st.b[r0][c0] = 0;
    
    
    if(st.b[r][c] == 6)
    {
        st.white_king_pos = std::make_pair(r, c);
        st.white_kingside_castle_right = false;
        st.white_queenside_castle_right = false;
        if(r0 == 7 && c0 == 4 && r == 7 && c == 6 && st.b[7][7] == 4)
        {
            st.white_pieces.erase(std::remove(st.white_pieces.begin(), st.white_pieces.end(), std::make_pair(7, 7)));
            st.white_pieces.push_back(std::make_pair(7, 5));
            st.b[7][5] = 4;
            st.b[7][7] = 0;
        }
        else if(r0 == 7 && c0 == 4 && r == 7 && c == 2 && st.b[7][0] == 4)
        {
            st.white_pieces.erase(std::remove(st.white_pieces.begin(), st.white_pieces.end(), std::make_pair(7, 0)));
            st.white_pieces.push_back(std::make_pair(7, 3));
            st.b[7][3] = 4;
            st.b[7][0] = 0;
        }
    }
    else if(st.b[r][c] == -6)
    {
        st.black_king_pos = std::make_pair(r, c);
        st.black_kingside_castle_right = false;
        st.black_queenside_castle_right = false;
        if(r0 == 0 && c0 == 4 && r == 0 && c == 6 && st.b[0][7] == -4)
        {
            st.black_pieces.erase(std::remove(st.black_pieces.begin(), st.black_pieces.end(), std::make_pair(0, 7)));
            st.black_pieces.push_back(std::make_pair(0, 5));
            st.b[0][5] = -4;
            st.b[0][7] = 0;
        }
        else if(r0 == 0 && c0 == 4 && r == 0 && c == 2  && st.b[0][0] == -4)
        {
            st.black_pieces.erase(std::remove(st.black_pieces.begin(), st.black_pieces.end(), std::make_pair(0, 0)));
            st.black_pieces.push_back(std::make_pair(0, 3));
            st.b[0][3] = -4;
            st.b[0][0] = 0;
        }
    }
    else if(st.b[r][c] == 4 && r0 == 7 && c0 == 7)
        st.white_kingside_castle_right = false;
    else if(st.b[r][c] == 4 && r0 == 7 && c0 == 0)
        st.white_queenside_castle_right = false;
    else if(st.b[r][c] == -4 && r0 == 0 && c0 == 7)
        st.black_kingside_castle_right = false;
    else if(st.b[r][c] == -4 && r0 == 0 && c0 == 0)
        st.black_queenside_castle_right = false;
    
    if(st.b[r][c] == 1)
    {
        if(st.en_passant == std::make_pair(r, c))
        {
            st.black_pieces.erase(std::remove(st.black_pieces.begin(), st.black_pieces.end(), std::make_pair(r0, c)));
            st.b[r0][c] = 0;
        }
        else if(r == (r0 - 2) && c == c0)
            st.en_passant = std::make_pair(r0 - 1, c0);
        else if(r == 0)
        {
            st.en_passant = std::make_pair(-1, -1);
            st.b[r][c] = move / 10000;
        }
        else
            st.en_passant = std::make_pair(-1, -1);
    }
    else if(st.b[r][c] == -1)
    {
        if(st.en_passant == std::make_pair(r, c))
        {
            st.white_pieces.erase(std::remove(st.white_pieces.begin(), st.white_pieces.end(), std::make_pair(r0, c)));
            st.b[r0][c] = 0;
        }
        else if(r == (r0 + 2) && c == c0)
            st.en_passant = std::make_pair(r0 + 1, c0);
        else if(r == 7)
        {
            st.en_passant = std::make_pair(-1, -1);
            st.b[r][c] = -move / 10000;
        }
        else
            st.en_passant = std::make_pair(-1, -1);
    }
    else
        st.en_passant = std::make_pair(-1, -1);
    

    st.white_moves.clear();
    st.black_moves.clear();
    for(int r = 0; r < 8; r++)
    {
        for(int c = 0; c < 8; c++)
        {
            if(st.b[r][c] > 0)
                get_moves(st, st.white_moves, r, c);
            else if(st.b[r][c] < 0)
                get_moves(st, st.black_moves, r, c);
        }
    }
}

void Solver::get_moves(state &st, std::vector<int> &moves, const int r0, const int c0)
{
    int r;
    int c;

    switch(st.b[r0][c0])
    {
        case 6:
            for(int i = 0, r = r0 - 1; r < 8 && i < 3; r++, i++)
                for(int j = 0, c = c0 - 1; c < 8 && j < 3; c++, j++)
                    if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                        moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            if(st.white_kingside_castle_right && r0 == 7 && c0 == 4 && legal_move(st, 7476))
                moves.push_back(7476);
            else if(st.white_queenside_castle_right && r0 == 7 && c0 == 4 && legal_move(st, 7472))
                moves.push_back(7472);
            break;
        case -6:
            for(int i = 0, r = r0 - 1; r < 8 && i < 3; r++, i++)
                for(int j = 0, c = c0 - 1; c < 8 && j < 3; c++, j++)
                    if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                        moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            if(st.black_kingside_castle_right && r0 == 0 && c0 == 4 && legal_move(st, 406))
                moves.push_back(406);
            else if(st.black_queenside_castle_right && r0 == 0 && c0 == 4 && legal_move(st, 402))
                moves.push_back(402);
            break;

        case 5:
        case -5:
            for(r = r0 + 1, c = c0 + 1; r < 8 && c < 8; r++, c++)
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            for(r = r0 + 1, c = c0 - 1; r < 8 && c >= 0; r++, c--)
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            for(r = r0 - 1, c = c0 + 1; r >= 0 && c < 8; r--, c++)
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            for(r = r0 - 1, c = c0 - 1; r >= 0 && c >= 0; r--, c--)
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            for(r = r0 + 1, c = c0; r < 8; r++)
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            for(r = r0 - 1, c = c0; r >= 0; r--)
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            for(c = c0 + 1, r = r0; c < 8; c++)
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            for(c = c0 - 1, r = r0; c >= 0; c--)
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            break;

        case 4:
        case -4:
            for(r = r0 + 1, c = c0; r < 8; r++)
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            for(r = r0 - 1, c = c0; r >= 0; r--)
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            for(c = c0 + 1, r = r0; c < 8; c++)
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            for(c = c0 - 1, r = r0; c >= 0; c--)
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            break;

        case 3:
        case -3:
            for(r = r0 + 1, c = c0 + 1; r < 8 && c < 8; r++, c++)
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            for(r = r0 + 1, c = c0 - 1; r < 8 && c >= 0; r++, c--)
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            for(r = r0 - 1, c = c0 + 1; r >= 0 && c < 8; r--, c++)
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            for(r = r0 - 1, c = c0 - 1; r >= 0 && c >= 0; r--, c--)
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            break;

        case 2:
        case -2:
            r = r0 + 2;
            c = c0 + 1;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            r = r0 + 2;
            c = c0 - 1;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            r = r0 - 2;
            c = c0 + 1;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            r = r0 - 2;
            c = c0 - 1;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            r = r0 + 1;
            c = c0 + 2;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            r = r0 + 1;
            c = c0 - 2;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            r = r0 - 1;
            c = c0 + 2;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            r = r0 - 1;
            c = c0 - 2;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            break;

        case 1:
            r = r0 - 1;
            if(r == 0)
            {
                c = c0 + 1;
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                {
                    moves.push_back(50000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(40000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(30000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(20000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                }
                c = c0 - 1;
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                {
                    moves.push_back(50000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(40000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(30000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(20000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                }
                c = c0;
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                {
                    moves.push_back(50000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(40000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(30000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(20000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                }
            }
            else
            {
                c = c0 + 1;
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
                c = c0 - 1;
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
                c = c0;
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            }
            r = r0 - 2;
            c = c0;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            break;

        case -1:
            r = r0 + 1;
            if(r == 7)
            {
                c = c0 + 1;
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                {
                    moves.push_back(50000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(40000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(30000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(20000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                }
                c = c0 - 1;
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                {
                    moves.push_back(50000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(40000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(30000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(20000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                }
                c = c0;
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                {
                    moves.push_back(50000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(40000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(30000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                    moves.push_back(20000 + r0 * 1000 + c0 * 100 + r * 10 + c);
                }
            }
            else
            {
                c = c0 + 1;
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
                c = c0 - 1;
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
                c = c0;
                if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                    moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            }
            r = r0 + 2;
            c = c0;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            break;
    }
}

bool Solver::legal_move(state &st, const int move)
{
    const int r0 = move / 1000 % 10;
    const int c0 = move / 100 % 10;
    const int r = move / 10 % 10;
    const int c = move % 10;
    int temp = st.b[r][c];
    
    if(possible_move(st, move))
    {
        st.b[r][c] = st.b[r0][c0];
        st.b[r0][c0] = 0;
        if(st.b[r][c] == 6)
            st.white_king_pos = std::make_pair(r, c);
        if(st.b[r][c] == -6)
            st.black_king_pos = std::make_pair(r, c);


        if((st.b[r][c] > 0 && square_is_safe(st, true, st.white_king_pos.first, st.white_king_pos.second)) ||
           (st.b[r][c] < 0 && square_is_safe(st, false, st.black_king_pos.first, st.black_king_pos.second)))
        {
            st.b[r0][c0] = st.b[r][c];
            st.b[r][c] = temp;
            if(st.b[r0][c0] == 6)
                st.white_king_pos = std::make_pair(r0, c0);
            if(st.b[r0][c0] == -6)
                st.black_king_pos = std::make_pair(r0, c0);
            return true;
        }
        else
        {
            st.b[r0][c0] = st.b[r][c];
            st.b[r][c] = temp;
            if(st.b[r0][c0] == 6)
                st.white_king_pos = std::make_pair(r0, c0);
            if(st.b[r0][c0] == -6)
                st.black_king_pos = std::make_pair(r0, c0);
            return false;
        }
    }
    else
        return false;
}

bool Solver::possible_move(state &st, const int move)
{
    const int r0 = move / 1000 % 10;
    const int c0 = move / 100 % 10;
    const int r = move / 10 % 10;
    const int c = move % 10;
    int t_row;
    int t_col;

    if((c >= 0 && c < 8 && r >= 0 && r < 8) &&
        ((st.b[r0][c0] > 0 && st.b[r][c] <= 0) || (st.b[r0][c0] < 0 && st.b[r][c] >= 0)))
    {
        switch(st.b[r0][c0])
        {
            case 6:
                return ((r == r0 + 1 || r == r0 - 1  || r == r0) && (c == c0 + 1 || c == c0 - 1 || c == c0))
                    || ((r0 == 7 && r == 7) && (c0 == 4 && c == 6) && st.white_kingside_castle_right
                    && st.b[7][5] == 0 && square_is_safe(st, true, 7, 4) && square_is_safe(st, true, 7, 5))
                    || ((r0 == 7 && r == 7) && (c0 == 4 && c == 2) && st.white_queenside_castle_right
                    && st.b[7][1] == 0 && st.b[7][3] == 0 && square_is_safe(st, true, 7, 4) && square_is_safe(st, true, 7, 3));
                break;
            case -6:
                return ((r == r0 + 1 || r == r0 - 1  || r == r0) && (c == c0 + 1 || c == c0 - 1 || c == c0))
                    || ((r0 == 0 && r == 0) && (c0 == 4 && c == 6) && st.black_kingside_castle_right
                    && st.b[0][5] == 0 && square_is_safe(st, false, 0, 4) && square_is_safe(st, false, 0, 5))
                    || ((r0 == 0 && r == 0) && (c0 == 4 && c == 2) && st.black_queenside_castle_right
                    && st.b[0][1] == 0 && st.b[0][3] == 0 && square_is_safe(st, false, 0, 4) && square_is_safe(st, false, 0, 3));
                break;

            case 5:
            case -5:
                if(r > r0 && c == c0)
                {
                    for(t_row = r0 + 1; t_row < r; t_row++)
                        if(st.b[t_row][c0] != 0)
                            return false;
                    return true;
                }
                else if(r == r0 && c < c0)
                {
                    for(t_col = c0 - 1; t_col > c; t_col--)
                        if(st.b[r0][t_col] != 0)
                            return false;
                    return true;
                }
                else if(r == r0 && c > c0)
                {
                    for(t_col = c0 + 1; t_col < c; t_col++)
                        if(st.b[r0][t_col] != 0)
                            return false;
                    return true;
                }
                else if(r < r0 && c == c0)
                {
                    for(t_row = r0 - 1; t_row > r; t_row--)
                        if(st.b[t_row][c0] != 0)
                            return false;
                    return true;
                }
                else if(r > r0 && c > c0 && (r - r0 == c - c0))
                {
                    for(t_row = r0 + 1, t_col = c0 + 1; t_row < r && t_col < c; t_row++, t_col++)
                        if(st.b[t_row][t_col] != 0)
                            return false;
                    return true;
                }
                else if(r > r0 && c < c0 && (r - r0 == c0 - c))
                {
                    for(t_row = r0 + 1, t_col = c0 - 1; t_row < r && t_col > c; t_row++, t_col--)
                        if(st.b[t_row][t_col] != 0)
                            return false;
                    return true;
                }
                else if(r < r0 && c > c0 && (r0 - r == c - c0))
                {
                    for(t_row = r0 - 1, t_col = c0 + 1; t_row > r && t_col < c; t_row--, t_col++)
                        if(st.b[t_row][t_col] != 0)
                            return false;
                    return true;
                }
                else if(r < r0 && c < c0 && (r0 - r == c0 - c))
                {
                    for(t_row = r0 - 1, t_col = c0 - 1; t_row > r && t_col > c; t_row--, t_col--)
                        if(st.b[t_row][t_col] != 0)
                            return false;
                    return true;
                }
                else
                    return false;
                break;

            case 4:
            case -4:
                if(r > r0 && c == c0)
                {
                    for(t_row = r0 + 1; t_row < r; t_row++)
                        if(st.b[t_row][c0] != 0)
                            return false;
                    return true;
                }
                else if(r == r0 && c < c0)
                {
                    for(t_col = c0 - 1; t_col > c; t_col--)
                        if(st.b[r0][t_col] != 0)
                            return false;
                    return true;
                }
                else if(r == r0 && c > c0)
                {
                    for(t_col = c0 + 1; t_col < c; t_col++)
                        if(st.b[r0][t_col] != 0)
                            return false;
                    return true;
                }
                else if(r < r0 && c == c0)
                {
                    for(t_row = r0 - 1; t_row > r; t_row--)
                        if(st.b[t_row][c0] != 0)
                            return false;
                    return true;
                }
                else
                    return false;
                break;

            case 3:
            case -3:
                if(r > r0 && c > c0 && (r - r0 == c - c0))
                {
                    for(t_row = r0 + 1, t_col = c0 + 1; t_row < r && t_col < c; t_row++, t_col++)
                        if(st.b[t_row][t_col] != 0)
                            return false;
                    return true;
                }
                else if(r > r0 && c < c0 && (r - r0 == c0 - c))
                {
                    for(t_row = r0 + 1, t_col = c0 - 1; t_row < r && t_col > c; t_row++, t_col--)
                        if(st.b[t_row][t_col] != 0)
                            return false;
                    return true;
                }
                else if(r < r0 && c > c0 && (r0 - r == c - c0))
                {
                    for(t_row = r0 - 1, t_col = c0 + 1; t_row > r && t_col < c; t_row--, t_col++)
                        if(st.b[t_row][t_col] != 0)
                            return false;
                    return true;
                }
                else if(r < r0 && c < c0 && (r0 - r == c0 - c))
                {
                    for(t_row = r0 - 1, t_col = c0 - 1; t_row > r && t_col > c; t_row--, t_col--)
                        if(st.b[t_row][t_col] != 0)
                            return false;
                    return true;
                }
                else
                    return false;
                break;

            case 2:
            case -2:
                return ((r == r0 + 1 || r == r0 - 1) && (c == c0 + 2 || c == c0 - 2))
                        || ((r == r0 + 2 || r == r0 - 2) && (c == c0 + 1 || c == c0 - 1));
                break;

            case 1:
                return ((r == r0 - 1 && (c == c0 + 1 || c == c0 - 1))
                        && (st.b[r][c] < 0 || st.en_passant == std::make_pair(r, c)))
                        || ((r == r0 - 1 && c == c0 && st.b[r][c] == 0)
                        || ((r == r0 - 2 && c == c0 && r == 4) && st.b[r][c] == 0 && st.b[r + 1][c] == 0));
                break;

            case -1:
                return ((r == r0 + 1 && (c == c0 + 1 || c == c0 - 1))
                        && (st.b[r][c] > 0 || st.en_passant == std::make_pair(r, c)))
                        || ((r == r0 + 1 && c == c0 && st.b[r][c] == 0)
                        || ((r == r0 + 2 && c == c0 && r == 3) && st.b[r][c] == 0 && st.b[r - 1][c] == 0));
                break;
            default:
                return false;
        }
    }
    else
        return false;
}

bool Solver::square_is_safe(state &st, bool s, const int r0, const int c0)
{
    int m;
    int r;
    int c;

    m = s ? -1 : 1;

    for(int i = 0, r = r0 - 1; r < 8 && i < 3; r++, i++)
        for(int j = 0, c = c0 - 1; c < 8 && j < 3; c++, j++)
            if((c >= 0 && c < 8 && r >= 0 && r < 8) && st.b[r][c] == 6 * m)
                return false;

    for(r = r0 + 1, c = c0 + 1; r < 8 && c < 8; r++, c++)
        if((c >= 0 && c < 8 && r >= 0 && r < 8) && (st.b[r][c] == 5 * m || st.b[r][c] == 3 * m))
            return false;
        else if(st.b[r][c] != 0)
            break;
    for(r = r0 + 1, c = c0 - 1; r < 8 && c >= 0; r++, c--)
        if((c >= 0 && c < 8 && r >= 0 && r < 8) && (st.b[r][c] == 5 * m || st.b[r][c] == 3 * m))
            return false;
        else if(st.b[r][c] != 0)
            break;
    for(r = r0 - 1, c = c0 + 1; r >= 0 && c < 8; r--, c++)
        if((c >= 0 && c < 8 && r >= 0 && r < 8) && (st.b[r][c] == 5 * m || st.b[r][c] == 3 * m))
            return false;
        else if(st.b[r][c] != 0)
            break;
    for(r = r0 - 1, c = c0 - 1; r >= 0 && c >= 0; r--, c--)
        if((c >= 0 && c < 8 && r >= 0 && r < 8) && (st.b[r][c] == 5 * m || st.b[r][c] == 3 * m))
            return false;
        else if(st.b[r][c] != 0)
            break;
    for(r = r0 + 1, c = c0; r < 8; r++)
        if((c >= 0 && c < 8 && r >= 0 && r < 8) && (st.b[r][c] == 5 * m || st.b[r][c] == 4 * m))
            return false;
        else if(st.b[r][c] != 0)
            break;
    for(r = r0 - 1, c = c0; r >= 0; r--)
        if((c >= 0 && c < 8 && r >= 0 && r < 8) && (st.b[r][c] == 5 * m || st.b[r][c] == 4 * m))
            return false;
        else if(st.b[r][c] != 0)
            break;
    for(c = c0 + 1, r = r0; c < 8; c++)
        if((c >= 0 && c < 8 && r >= 0 && r < 8) && (st.b[r][c] == 5 * m || st.b[r][c] == 4 * m))
            return false;
        else if(st.b[r][c] != 0)
            break;
    for(c = c0 - 1, r = r0; c >= 0; c--)
        if((c >= 0 && c < 8 && r >= 0 && r < 8) && (st.b[r][c] == 5 * m || st.b[r][c] == 4 * m))
            return false;
        else if(st.b[r][c] != 0)
            break;

    r = r0 + 2;
    c = c0 + 1;
    if((c >= 0 && c < 8 && r >= 0 && r < 8) && st.b[r][c] == 2 * m)
        return false;
    r = r0 + 2;
    c = c0 - 1;
    if((c >= 0 && c < 8 && r >= 0 && r < 8) && st.b[r][c] == 2 * m)
        return false;
    r = r0 - 2;
    c = c0 + 1;
    if((c >= 0 && c < 8 && r >= 0 && r < 8) && st.b[r][c] == 2 * m)
        return false;
    r = r0 - 2;
    c = c0 - 1;
    if((c >= 0 && c < 8 && r >= 0 && r < 8) && st.b[r][c] == 2 * m)
        return false;
    r = r0 + 1;
    c = c0 + 2;
    if((c >= 0 && c < 8 && r >= 0 && r < 8) && st.b[r][c] == 2 * m)
        return false;
    r = r0 + 1;
    c = c0 - 2;
    if((c >= 0 && c < 8 && r >= 0 && r < 8) && st.b[r][c] == 2 * m)
        return false;
    r = r0 - 1;
    c = c0 + 2;
    if((c >= 0 && c < 8 && r >= 0 && r < 8) && st.b[r][c] == 2 * m)
        return false;
    r = r0 - 1;
    c = c0 - 2;
    if((c >= 0 && c < 8 && r >= 0 && r < 8) && st.b[r][c] == 2 * m)
        return false;

    r = r0 - 1 * -m;
    c = c0 + 1;
    if((c >= 0 && c < 8 && r >= 0 && r < 8) && st.b[r][c] == 1 * m)
        return false;
    r = r0 - 1 * -m;
    c = c0 - 1;
    if((c >= 0 && c < 8 && r >= 0 && r < 8) && st.b[r][c] == 1 * m)
        return false;

    return true;
}

void Solver::sort_by_check(bool s, state st, std::vector<int> &m)
{
    int count = 0;
    int check_moves;
    int temp_m;
    state temp;
    std::pair<int, int> *king_pos;

    if(s)
        king_pos = &temp.black_king_pos;
    else
        king_pos = &temp.white_king_pos;

    std::vector<std::vector<int>>::size_type size = m.size();
    for(std::vector<std::vector<int>>::size_type i = 0; i < size; i++)
    {
        temp = st;
        update_state(temp, m[i]);
        if(!square_is_safe(temp, !s, king_pos->first, king_pos->second))
        {
            temp_m = m[count];
            m[count] = m[i];
            m[i] = temp_m;
            count++;
        }
    }
    
    check_moves = count;
    count = 0;
    for(int i = 0; i < check_moves; i++)
    {
        temp = st;
        update_state(temp, m[i]);
        if((temp.black_moves.size() == 1 && s) || (temp.white_moves.size() == 1 && !s))
        {
            temp_m = m[count];
            m[count] = m[i];
            m[i] = temp_m;
            count++;
        }
    }
}


std::vector<int> Solver::find_mate_in(state st, int m, bool s, bool t)
{
    if((aom * 2 - 1 - m) == 2)
    {
        mw->prog->setValue(step / progress_steps * 100.0);
        mw->prog->update();
        std::cout << "stp/pro" << step <<" "<<  progress_steps << std::endl;
        alpha = false;
        step++;
    }

    state next;
    std::vector<int> av;
    std::vector<int> temp;
    std::pair<int, int> *king_pos;
    std::vector<int> *p_moves;
    std::vector<int> *o_moves;
    

    if(t == true)
    {
        if(s)
            p_moves = &st.white_moves;
        else
            p_moves = &st.black_moves;
            
        sort_by_check(s, st, *p_moves);
        
        for(auto e: *p_moves)
        {
            ans = false; 
            next = st;
            update_state(next, e);

            if(m > 1)
                av = find_mate_in(next, m - 1, s, !t);
            
            if(ans)
            {
                av.push_back(e);
                return av;
            }
            
            if(s)
            {
                o_moves = &next.black_moves;
                king_pos = &next.black_king_pos;
            }
            else
            {
                o_moves = &next.white_moves;
                king_pos = &next.white_king_pos;
            }
            if(o_moves->empty())
            {
                if(m == 1 && !square_is_safe(next, !s, king_pos->first, king_pos->second))
                {
                    ans = true;
                    print_state(st_g);
                    av.push_back(e);
                    count++;
                    return av;
                }
                else if(!square_is_safe(next, !s, king_pos->first, king_pos->second))
                {
                    ans = true;
                    av.push_back(e);
                    print_state(next);
                    return av;
                }
            }
        }
    }
    else
    {
        if(s)
            o_moves = &st.black_moves;
        else
            o_moves = &st.white_moves;
            
        for(auto e: *o_moves)
        {
            ans = false;
            if(!av.empty())
                av.pop_back();
                
            next = st;
            update_state(next, e);
            if(m > 1)
                av = find_mate_in(next, m - 1, s, !t);
            
            if(ans)
            {
                av.push_back(e);
                if(av.size() > temp.size())
                    temp = av;  
                continue;
            }
            else if(!ans)
                return {};
        }
        if(temp.size() >= av.size())
            return temp;
    }
    return av;
}

void Solver::helpmate_heuristic(state &st)
{
    int count = 0;
    int swap;
    auto &m = st.black_moves;
    std::vector<std::vector<int>>::size_type size = m.size();
    
    if(size == 0)
        return;
    
    for(std::vector<std::vector<int>>::size_type i = 0; i < size; i++)
    {
        if(m[i] / 1000 % 10 == st.black_king_pos.first && m[i] / 100 % 10 == st.black_king_pos.second)
        {
            swap = m[count];
            m[count] = m[i];
            m[i] = swap;
            count++;
        }
    }
    if(count < 2)
    {
        swap = m[size - 1];
        m[size - 1] = m[0];
        m[0] = swap;
    }
}

void Solver::helpmate(state st, int m, bool s)
{
    if(m < 1)
        return;

    if((aom * 2 - m) == 2)
    {
        mw->prog->setValue(step/progress_steps * (double)100);
        mw->prog->update();
        std::cout << "stp/pro" << step <<" "<<  progress_steps << std::endl; 
        step++;
    }
    
    state next;

    if(s == true)
    {
        for(auto e: st.white_moves)
        {
            next = st;
            update_state(next, e);
            helpmate(next, m - 1, !s);
            if(ans)
            {
                ans_moves.push_back(e);
                return;
            }
            if(m == 1 && next.black_moves.empty()
                && !square_is_safe(next, false, next.black_king_pos.first, next.black_king_pos.second))
            {
                ans = true;
                count++;
                ans_moves.push_back(e);
                return;
            }
        }
    }
    else
    {
        helpmate_heuristic(st);
        
        for(auto e: st.black_moves)
        {
            next = st;
            update_state(next, e);
            helpmate(next, m - 1, !s);
            if(ans)
            {
                ans_moves.push_back(e);
                return;
            }
        }
    }
}

void Solver::selfmate_heuristic(state &st)
{
    int count = 0;
    int swap;
    state temp;
    auto &m = st.white_moves;

    std::vector<std::vector<int>>::size_type size = m.size();
    for(std::vector<std::vector<int>>::size_type i = 0; i < size; i++)
    {
        temp = st;
        update_state(temp, m[i]);
        if(temp.black_moves.size() < 4)
        {
            swap = m[count];
            m[count] = m[i];
            m[i] = swap;
            count++;
        }
    } 
}
std::vector<int> Solver::selfmate(state st, int m, bool s)
{  
    if(alpha && (aom * 2 - m) == 2)
    {
        mw->prog->setValue(step/progress_steps * (double)100);
        mw->prog->update();
        std::cout << "stp/pro" << step <<" "<<  progress_steps << std::endl;
        step++;
    }
    
    state next;
    std::vector<int> av;
    std::vector<int> temp;

    if(s == true)
    {
        selfmate_heuristic(st);
        
        for(auto e: st.white_moves)
        {
            ans = false;
            next = st;
            update_state(next, e);
            if(m > 1)
                av = selfmate(next, m - 1, !s);
            if(ans)
            {
                av.push_back(e);
                return av;
            }
        }
    }
    else
    {
        for(auto e: st.black_moves)
        {
            if(!av.empty())
                av.pop_back();
                
            ans = false;
            next = st;
            update_state(next, e);
            if(m > 1)
                av = selfmate(next, m - 1, !s);

            if(next.white_moves.empty() && 
                !square_is_safe(next, true, next.white_king_pos.first, next.white_king_pos.second))
            {
                count++;
                ans = true;
            }
                
            if(ans)
            {
                av.push_back(e);
                if(av.size() > temp.size())
                    temp = av;  
                continue;
            }
            else if(!ans)
                return {};
        }
        if(temp.size() >= av.size())
            return temp;
    }
    return av;
}

void Solver::print_state(state st)
{
    std::cout << st.white_king_pos.first << st.white_king_pos.second << "k+ ";
    for(auto e: st.white_pieces)
        std::cout << e.first << e.second << "+";
    std::cout << "   ";
    std::cout << st.black_king_pos.first << st.black_king_pos.second << "k- ";
    for(auto e: st.black_pieces)
        std::cout << e.first << e.second << "-";
    std::cout << std::endl;
    std::cout << "white moves: ";
    for(auto e: st.white_moves)
        std::cout << e << "+ ";
    std::cout << std::endl;
    std::cout << "black moves: ";
    for(auto e: st.black_moves)
        std::cout << e << "- ";
    std::cout << std::endl;
    std::cout << std::endl;
}
