#include "solver.h"
#include <algorithm>
#include <vector>

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
        if(st.b[r][c] != 0)
            st.black_pieces.erase(std::remove(st.black_pieces.begin(), st.black_pieces.end(), std::make_pair(r, c)));
    }
    else if(st.b[r0][c0] < 0)
    {
        st.black_pieces.erase(std::remove(st.black_pieces.begin(), st.black_pieces.end(), std::make_pair(r0, c0)));
        st.black_pieces.push_back(std::make_pair(r, c));
        if(st.b[r][c] != 0)
            st.white_pieces.erase(std::remove(st.white_pieces.begin(), st.white_pieces.end(), std::make_pair(r, c)));
    }
    st.b[r][c] = st.b[r0][c0];
    st.b[r0][c0] = 0;
    
    
    if(st.b[r][c] == 6)
    {
        st.white_king_pos = std::make_pair(r, c);
        st.white_kingside_castle_right = false;
        st.white_queenside_castle_right = false;
        if(r0 == 7 && c0 == 4 && r == 7 && c == 6)
        {
            st.white_pieces.erase(std::remove(st.white_pieces.begin(), st.white_pieces.end(), std::make_pair(7, 7)));
            st.white_pieces.push_back(std::make_pair(7, 5));
            st.b[7][5] = 4;
            st.b[7][7] = 0;
        }
        else if(r0 == 7 && c0 == 4 && r == 7 && c == 2)
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
        if(r0 == 0 && c0 == 4 && r == 0 && c == 6)
        {
            st.black_pieces.erase(std::remove(st.black_pieces.begin(), st.black_pieces.end(), std::make_pair(0, 7)));
            st.black_pieces.push_back(std::make_pair(0, 5));
            st.b[0][5] = -4;
            st.b[0][7] = 0;
        }
        else if(r0 == 0 && c0 == 4 && r == 0 && c == 2)
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
        else
            st.en_passant = std::make_pair(-1, -1);
        promotion(st, r, c);
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
        else
            st.en_passant = std::make_pair(-1, -1);
        promotion(st, r, c);
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
            c = c0 + 1;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            r = r0 - 1;
            c = c0 - 1;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            r = r0 - 1;
            c = c0;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            r = r0 - 2;
            c = c0;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            break;

        case -1:
            r = r0 + 1;
            c = c0 + 1;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            r = r0 + 1;
            c = c0 - 1;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
            r = r0 + 1;
            c = c0;
            if(legal_move(st, r0 * 1000 + c0 * 100 + r * 10 + c))
                moves.push_back(r0 * 1000 + c0 * 100 + r * 10 + c);
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

void Solver::promotion(state &st,const int r0, const int c0)
{
    if(r0 == 0 && c0 >= 0 && c0 < 8 && st.b[r0][c0] == 1)
        st.b[r0][c0] = 5;
    else if(r0 == 7 && c0 >= 0 && c0 < 8 && st.b[r0][c0] == -1)
        st.b[r0][c0] = -5;
}

int Solver::get_value(int v)
{
    int m;
    v > 0 ? m = 1 : m = -1;

    if(abs(v) == 2 || abs(v) == 4)
        v += m;
    if(abs(v) == 5)
        v += 4 * m;
    return v;
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
        // std::cout << m[i][0] << m[i][1] << m[i][2] << m[i][3] << std::endl;
        update_state(temp, m[i]);
        if(!square_is_safe(temp, !s, king_pos->first, king_pos->second))
        {
            temp_m = m[count];
            m[count] = m[i];
            m[i] = temp_m;
            // std::cout << m[i][0] << m[i][1] << m[i][2] << m[i][3] << std::endl;
            // std::cout << king_pos->first << king_pos->second << std::endl;
            // std::cout << size << std::endl;
            count++;
        }
    }
    
    check_moves = count;
    count = 0;
    for(int i = 0; i < check_moves; i++)
    {
        temp = st;
        // std::cout << m[i][0] << m[i][1] << m[i][2] << m[i][3] << std::endl;
        update_state(temp, m[i]);
        if(temp.black_moves.size() == 1)
        {
            temp_m = m[count];
            m[count] = m[i];
            m[i] = temp_m;
            // std::cout << m[i][0] << m[i][1] << m[i][2] << m[i][3] << std::endl;
            // std::cout << king_pos->first << king_pos->second << std::endl;
            // std::cout << size << std::endl;
            count++;
        }
    }
}
    // std::cout << "\nSTATE_UPDATED_START\n";
    // for(std::vector<std::vector<int>>::size_type i = 0; i < size; i++)
    // {
    //     std::cout << "move: " << ans_moves[i][0] << ans_moves[i][1] <<
    //         ans_moves[i][2] << ans_moves[i][3] << " :" << i << "\n";
    // }
    // print_state(st);
    // if(!legal_move(st, ans_moves[1][0], ans_moves[1][1], ans_moves[1][2], ans_moves[1][3]))
    // {
    //     ans = false;
    //     return;
    // }
    // update_state(st, ans_moves[1][0], ans_moves[1][1], ans_moves[1][2], ans_moves[1][3]);
    // if(!legal_move(st, ans_moves[2][0], ans_moves[2][1], ans_moves[2][2], ans_moves[2][3]))
    // {
    //     ans = false;
    //     return;
    // }
    // update_state(st, ans_moves[2][0], ans_moves[2][1], ans_moves[2][2], ans_moves[2][3]);

    // std::cout << "\nSTATE_BEFORE_UPDATE_IF_OK\n";
    // print_state(st);

    // update_state(st, ans_moves[i - 1][0], ans_moves[i - 1][1], ans_moves[i - 1][2], ans_moves[i - 1][3]);
    // std::cout << "\nSTATE_UPDATED_BLACK\n";
    // std::cout << "move: " << ans_moves[i - 1][0] << ans_moves[i - 1][1] <<
    //     ans_moves[i - 1][2] << ans_moves[i - 1][3] << " i: " << i - 1 <<"\n";
    // print_state(st);
    // update_state(st, ans_moves[i][0], ans_moves[i][1], ans_moves[i][2], ans_moves[i][3]);
    // std::cout << "\nSTATE_UPDATED_WHITE\n";
    // std::cout << "move: " << ans_moves[i][0] << ans_moves[i][1] <<
    //     ans_moves[i][2] << ans_moves[i][3] << " i: " << i <<"\n";
    // print_state(st);
    // std::cout << "move: " << ans_moves[3][0] << ans_moves[3][1] <<
    //     ans_moves[3][2] << ans_moves[3][3] << " n3\n";
    // std::cout << "move: " << ans_moves[4][0] << ans_moves[4][1] <<
    //     ans_moves[4][2] << ans_moves[4][3] << " n4\n";


void Solver::find_mate_in(state st, int m, bool s)
{
    if(m < 1)
        return;

    // if(m == 1)
    //     count++;

    bool s_sort = (m % 2 == 1);
    state next;

    if(s == true)
    {
        s = false;
        if(s_sort)
        {
            sort_by_check(true, st, st.white_moves);
            //print_state(st);
        }
        for(auto e: st.white_moves)
        {
            ans_moves.push_back(e);
            next = st;
            update_state(next, e);
            find_mate_in(next, m - 1, s);
            if(ans)
            {
                return;
            }
            if(m == 1 && next.black_moves.empty()
                && !square_is_safe(next, false, next.black_king_pos.first, next.black_king_pos.second))
            {
                ans = true;
                count++;
                
                std::vector<int> a;
                state st_g_copy = st_g;
                state temp = st_g;
                state temp0;
                
                // verify_mate...
                if(ans_moves.size() > 1)
                {
                    // if(!legal_move(temp, ans_moves[0][0], ans_moves[0][1], ans_moves[0][2], ans_moves[0][3]))
                    // {
                    //     std::cout << "\n\nUNACCEPTABLEEEEEEE!!!!!\n\n";
                    // }
                    update_state(temp, ans_moves[0]);
                    std::vector<std::vector<int>>::size_type size = ans_moves.size();
                    std::cout << "MOVES: " << size << "\n";
                    for(std::vector<std::vector<int>>::size_type i = 0; i < size; i++)
                    {
                        std::cout << "move: " << ans_moves[0] << "\n";
                    }
                    for(std::vector<std::vector<int>>::size_type i = 2; i < size; i += 2)
                    {
                        for(const auto &e: temp.black_moves)
                        {
                            std::cout << std::endl << "IN_BLACK_MOVES" << std::endl;
                            temp0 = temp;
                            update_state(temp0, e);
                            // if(!legal_move(temp0, ans_moves[i][0], ans_moves[i][1], ans_moves[i][2], ans_moves[i][3]))
                            // {
                            //     ans = false;
                            //     break;
                            // }
                            // ans = false;
                            for(std::vector<std::vector<int>>::size_type k = 1; k <= size - i; k += 2)
                            {
                                ans = false;
                                a = ans_moves;
                                st_g = temp0;
                                
                                if(e != ans_moves[i-1])
                                {
                                    std::cout << "\nE_NOT_IN_ANS_MOVES\n";
                                    ans_moves.clear();
                                    find_mate_in(temp0, k, true);
                                    std::cout << "size - i: " << size - i << "\n";
                                    std::cout << "ans: " << ans << "\n";
                                    std::cout << "i: " << i << "\n";
                                    std::cout << "k: " << k << "\n";
                                    if(ans)
                                    {
                                        std::cout << "\nmate_when_SHOULD_be\n";
                                        ans_moves = a;
                                        break;
                                    }
                                    else
                                    {
                                        ans_moves = a;
                                        continue;
                                    }
                                }
                                else if((k < size - i) && (size - i > 1))
                                {
                                    std::cout << "\nE_IN_ANS_MOVES\n";
                                    ans_moves.clear();
                                    find_mate_in(temp0, k, true);
                                    std::cout << "size - i: " << size - i << "\n";
                                    std::cout << "ans: " << ans << "\n";
                                    std::cout << "i: " << i << "\n";
                                    std::cout << "k: " << k << "\n";
                                    if(ans)
                                    {
                                        std::cout << "\nmate_when_SHOULD_NOT_be\n";
                                        ans = false;
                                        ans_moves = a;
                                        break;
                                    }
                                }
                                ans = true;
                                ans_moves = a;
                            }
                            if(!ans)
                                break;
                        }
                        if(!ans)
                        {
                            st_g = st_g_copy;
                            break;
                        }
                        std::cout << "\nSTATE_UPDATED_IF_OK\n";
                        for(std::vector<std::vector<int>>::size_type i = 0; i < size; i++)
                        {
                            std::cout << "move: " << ans_moves[i] << " :" << i << "\n";
                        }
                        update_state(temp, ans_moves[i - 1]);
                        update_state(temp, ans_moves[i]);
                    }
                    // if(!temp.black_moves.empty() || piece_is_safe(temp, temp.black_king_pos.first, temp.black_king_pos.second))
                    // {
                    //     ans = false;
                    // }
                }

                if(ans)
                {
                    st_g = next;
                    return;
                }
            }
            ans_moves.pop_back();
        }
    }
    else
    {
        s = true;
        if(s_sort)
        {
            sort_by_check(false, st, st.black_moves);
        }
        for(auto e: st.black_moves)
        {
            ans_moves.push_back(e);
            next = st;
            update_state(next, e);
            find_mate_in(next, m - 1, s);
            if(ans)
            {
                return;
            }
            if(m == 1 && next.white_moves.empty() &&
                !square_is_safe(next, true, next.white_king_pos.first, next.white_king_pos.second))
            {
                ans = true;
                count++;
                if(ans)
                {
                    st_g = next;
                    return;
                }
            }
            ans_moves.pop_back();
        }
    }
}

void Solver::helpmate(state st, int m, bool s)
{
    if(m < 1)
        return;

    state next;

    if(s == true)
    {
        s = false;
        // if(s_sort)
        // {
        //     sort_by_check(false, st, st.black_moves);
        // }
        for(auto e: st.white_moves)
        {
            ans_moves.push_back(e);
            next = st;
            update_state(next, e);
            helpmate(next, m - 1, s);
            if(ans)
            {
                return;
            }
            if(m == 1 && next.black_moves.empty()
                && !square_is_safe(next, false, next.black_king_pos.first, next.black_king_pos.second))
            {
                ans = true;
                count++;
                if(ans)
                {
                    st_g = next;
                    return;
                }
            }
            ans_moves.pop_back();
        }
    }
    else
    {
        s = true;
        // if(s_sort)
        // {
        //     sort_by_check(false, st, st.black_moves);
        // }
        for(auto e: st.black_moves)
        {
            ans_moves.push_back(e);
            next = st;
            update_state(next, e);
            helpmate(next, m - 1, s);
            if(ans)
            {
                return;
            }
            if(m == 1 && next.white_moves.empty() &&
                !square_is_safe(next, true, next.white_king_pos.first, next.white_king_pos.second))
            {
                ans = true;
                count++;
                if(ans)
                {
                    st_g = next;
                    return;
                }
            }
            ans_moves.pop_back();
        }
    }
}

void Solver::other_problem(state st, int m, bool s)
{
    if(m < 1)
        return;

    // state next;
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
    std::cout << "\n";
    std::cout << "white moves: ";
    for(auto e: st.white_moves)
        std::cout << e << "+ ";
    std::cout << "\n";
    std::cout << "black moves: ";
    for(auto e: st.black_moves)
        std::cout << e << "- ";
    std::cout << "\n";
    std::cout << "\n";
}
