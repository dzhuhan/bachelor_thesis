#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "solver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(736, 555));

    board.load(":/graphics/board.png");

    kw.load(":/graphics/kingW.png");
    qw.load(":/graphics/queenW.png");
    rw.load(":/graphics/rookW.png");
    bw.load(":/graphics/bishopW.png");
    nw.load(":/graphics/knightW.png");
    pw.load(":/graphics/pawnW.png");

    kb.load(":/graphics/kingB.png");
    qb.load(":/graphics/queenB.png");
    rb.load(":/graphics/rookB.png");
    bb.load(":/graphics/bishopB.png");
    nb.load(":/graphics/knightB.png");
    pb.load(":/graphics/pawnB.png");

    aom = 1;
    turn = true;
    side = true;
    edit = false;
    select = false;
    problem = 0;

    processBoard = board;
    ui->label->setPixmap(QPixmap::fromImage(board));
    ui->textEdit->setReadOnly(true);
    init_board();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QImage* MainWindow::which_piece(int a)
{
    switch(a)
    {
        case 6:
            return &kw;
        case 5:
            return &qw;
        case 4:
            return &rw;
        case 3:
            return &bw;
        case 2:
            return &nw;
        case 1:
            return &pw;

        case -6:
            return &kb;
        case -5:
            return &qb;
        case -4:
            return &rb;
        case -3:
            return &bb;
        case -2:
            return &nb;
        case -1:
            return &pb;

        case 0:
        default:
            return nullptr;
    }
}

void MainWindow::init_board()
{
    int x;
    int y;

    QImage *piece = nullptr;
    processBoard = board;
    ui->label->setPixmap(QPixmap::fromImage(board));

    st_g.white_kingside_castle_right = true;
    st_g.white_queenside_castle_right = true;
    st_g.black_kingside_castle_right = true;
    st_g.black_queenside_castle_right = true;
    for(int r = 0; r < 8; r++)
    {
        for(int c = 0; c < 8; c++)
        {
            piece = which_piece(init[r][c]);
            st_g.b[r][c] = init[r][c];

            if(st_g.b[r][c] > 0)
            {
                if(st_g.b[r][c] == 6)
                    st_g.white_king_pos = std::make_pair(r, c);
                st_g.white_pieces.push_back(std::make_pair(r, c));
            }
            else if(st_g.b[r][c] < 0)
            {
                if(st_g.b[r][c] == -6)
                    st_g.black_king_pos = std::make_pair(r, c);
                st_g.black_pieces.push_back(std::make_pair(r, c));
            }

            if(piece != nullptr)
            {
                x = c * 50 + 55;
                y = r * 50 + 55;
                QPainter painter(&processBoard);
                painter.drawImage(x, y, *piece);
                ui->label->setPixmap(QPixmap::fromImage(processBoard));
            }
        }
    }
    for(int r = 0; r < 8; r++)
    {
        for(int c = 0; c < 8; c++)
        {
            if(st_g.b[r][c] > 0)
                Solver::get_moves(st_g, st_g.white_moves, r, c);
            else if(st_g.b[r][c] < 0)
                Solver::get_moves(st_g, st_g.black_moves, r, c);
        }
    }

    turn = true;
    select = false;
    ans = false;
}

void MainWindow::set_board()
{
    int x;
    int y;

    QImage *piece = nullptr;
    processBoard = board;
    ui->label->setPixmap(QPixmap::fromImage(board));

    for(int r = 0; r < 8; r++)
    {
        for(int c = 0; c < 8; c++)
        {
            piece = which_piece(st_g.b[r][c]);

            if(piece != nullptr)
            {
                x = c * 50 + 55;
                y = r * 50 + 55;
                QPainter painter(&processBoard);
                painter.drawImage(x, y, *piece);
                // ui->label->setPixmap(QPixmap::fromImage(processBoard));
            }
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(processBoard));
}

bool MainWindow::is_turn(const int r0, const int c0)
{
    if(turn && st_g.b[r0][c0] > 0)
    {
        turn = !turn;
        return true;
    }
    else if(!turn && st_g.b[r0][c0] < 0)
    {
        turn = !turn;
        return true;
    }
    return false;
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    int c;
    int r;

    r = ((e->pos().y() - 77 >= 0) ? e->pos().y() - 77 : -100) / 50;
    c = ((e->pos().x() - 55 >= 0) ? e->pos().x() - 55 : -100) / 50;

    if(select == false)
    {
        row = r;
        col = c;

        if(st_g.b[r][c] != 0)
            select = true;
    }
    else if(c >= 0 && c < 8 && r >= 0 && r < 8)
    {
        if(st_g.b[row][col] != 0 && ((st_g.b[row][col] > 0 && st_g.b[r][c] <= 0) ||
            (st_g.b[row][col] < 0 && st_g.b[r][c] >= 0)) &&
            (edit || (Solver::legal_move(st_g, row * 1000 + col * 100 + r * 10 + c) && is_turn(row, col))))
        {
            std::cout << "AAAAAAAAAAAAAAAA!!!!!!!!" << std::endl;
            Solver::update_state(st_g, row * 1000 + col * 100 + r * 10 + c);
            set_board();
            select = false;
            std::cout << "AAAAAAAAAAAAAAAA!!!!!!!!" << std::endl;

        }
        else
        {
            row = r;
            col = c;
        }
    }
    Solver::print_state(st_g);
}

void MainWindow::on_actionopen_triggered()
{
    int xs;
    int ys;
    int xd;
    int yd;
    int temp;
    int temp2;
    int prm;

    int move;
    QString line;

    QString name = QFileDialog::getOpenFileName(this, tr("Open"), "");
    if(name == "")
        return;

    QFile file(name);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);

    init_board();
    st_g.white_pieces.clear();
    st_g.black_pieces.clear();
    st_g.white_moves.clear();
    st_g.black_moves.clear();

    if(in.readLine().contains("/"))
    {
        st_g.white_kingside_castle_right = false;
        st_g.white_queenside_castle_right = false;
        st_g.black_kingside_castle_right = false;
        st_g.black_queenside_castle_right = false;
        
        in.seek(0);
        char temp;
        for(int r = 0; r < 8; r++)
        {
            int a = 0;
            for(int c = 0; c < 8; c++)
            {
                if(a == 0)
                {
                    in >> temp;
                    std::cout << temp;
                }
                // if(temp == '/' || temp == ' ')
                //     break;
                if(a != 0)
                {
                    st_g.b[r][c] = 0;
                    std::cout << 0;
                    --a;
                }
                else
                {
                    switch(temp)
                    {
                        case 'K':
                            st_g.b[r][c] = 6;
                            break;
                        case 'Q':
                            st_g.b[r][c] = 5;
                            break;
                        case 'R':
                            st_g.b[r][c] = 4;
                            break;
                        case 'B':
                            st_g.b[r][c] = 3;
                            break;
                        case 'N':
                            st_g.b[r][c] = 2;
                            break;
                        case 'P':
                            st_g.b[r][c] = 1;
                            break;
                            
                        case 'k':
                            st_g.b[r][c] = -6;
                            break;
                        case 'q':
                            st_g.b[r][c] = -5;
                            break;
                        case 'r':
                            st_g.b[r][c] = -4;
                            break;
                        case 'b':
                            st_g.b[r][c] = -3;
                            break;
                        case 'n':
                            st_g.b[r][c] = -2;
                            break;
                        case 'p':
                            st_g.b[r][c] = -1;
                            break;
                            
                        default:
                            a = (int)(temp - 48);
                            st_g.b[r][c] = 0;
                            std::cout << 0;
                            --a;
                    }
                }
                
                if(st_g.b[r][c] > 0)
                {
                    if(st_g.b[r][c] == 6)
                        st_g.white_king_pos = std::make_pair(r, c);
                    st_g.white_pieces.push_back(std::make_pair(r, c));
                }
                else if(st_g.b[r][c] < 0)
                {
                    if(st_g.b[r][c] == -6)
                        st_g.black_king_pos = std::make_pair(r, c);
                    st_g.black_pieces.push_back(std::make_pair(r, c));
                }
            }
            in >> temp;
            std::cout << '\n';
        }

        in >> temp;
        if(temp == 'w')
        {
            ui->comboBox_2->setCurrentIndex(0);
            side = true;
        }
        else
        {
            ui->comboBox_2->setCurrentIndex(1);
            side = false;
        }        
        in >> temp;
        in >> temp;
        while(temp != ' ' && temp  != '-')
        {
            switch(temp)
            {
                case 'K':
                    st_g.white_kingside_castle_right = true;
                    break;
                case 'Q':
                    st_g.white_queenside_castle_right = true;
                    break;
                case 'k':
                    st_g.black_kingside_castle_right = true;
                    break;
                case 'q':
                    st_g.black_queenside_castle_right = true;
                    break;
            }
            in >> temp;
        }
        
        in >> temp;
        if(temp  != '-')
        {
            st_g.en_passant.first = (int)(temp - 97);
            in >> temp;
            st_g.en_passant.second = (int)(temp - 48);
        }
        
        
        for(int r = 0; r < 8; r++)
        {
            for(int c = 0; c < 8; c++)
            {
                if(st_g.b[r][c] > 0)
                    Solver::get_moves(st_g, st_g.white_moves, r, c);
                else if(st_g.b[r][c] < 0)
                    Solver::get_moves(st_g, st_g.black_moves, r, c);
            }
        }
    }
    else if(in.readLine().toInt() / 10 == 0)
    {
        in.seek(0);
        for(int r = 0; r < 8; r++)
        {
            for(int c = 0; c < 8; c++)
            {
                in >> st_g.b[r][c];
                if(st_g.b[r][c] > 0)
                {
                    if(st_g.b[r][c] == 6)
                        st_g.white_king_pos = std::make_pair(r, c);
                    st_g.white_pieces.push_back(std::make_pair(r, c));
                }
                else if(st_g.b[r][c] < 0)
                {
                    if(st_g.b[r][c] == -6)
                        st_g.black_king_pos = std::make_pair(r, c);
                    st_g.black_pieces.push_back(std::make_pair(r, c));
                }
            }
        }

        for(int r = 0; r < 8; r++)
        {
            for(int c = 0; c < 8; c++)
            {
                if(st_g.b[r][c] > 0)
                    Solver::get_moves(st_g, st_g.white_moves, r, c);
                else if(st_g.b[r][c] < 0)
                    Solver::get_moves(st_g, st_g.black_moves, r, c);
            }
        }
    }
    else
    {
        in.seek(0);
        while (!in.atEnd()) {
            line = in.readLine();
            move = line.toInt();
            temp2 = move;

            if(move / 10000 == 0)
            {
                yd = move % 10;
                move /= 10;
                xd = move % 10;
                move /= 10;
                ys = move % 10;
                move /= 10;
                xs = move;

                yd = 7 - (yd - 1);
                ys = 7 - (ys - 1);

                xd -= 1;
                xs -= 1;

                temp = st_g.b[ys][xs];
                if(temp == 6 || temp == -6)
                {
                    switch(temp2)
                    {
                        case 5171:
                            st_g.b[7][7] = 0;
                            st_g.b[7][5] = 4;
                            break;

                        case 5131:
                            st_g.b[7][0] = 0;
                            st_g.b[7][3] = 4;
                            break;

                        case 5878:
                            st_g.b[0][7] = 0;
                            st_g.b[0][5] = 4;
                            break;

                        case 5838:
                            st_g.b[0][0] = 0;
                            st_g.b[0][3] = 4;
                            break;
                    }
                }
                st_g.b[ys][xs] = 0;
                st_g.b[yd][xd] = temp;


            }
            else
            {
                prm = move % 10;
                move /= 10;
                yd = move % 10;
                move /= 10;
                xd = move % 10;
                move /= 10;
                ys = move % 10;
                move /= 10;
                xs = move;

                yd = 7 - (yd - 1);
                ys = 7 - (ys - 1);

                xd -= 1;
                xs -= 1;

                temp = st_g.b[ys][xs];

                std::cout  <<"prm: " << prm << std::endl;
                switch(prm)
                {
                    case 1:
                        temp = temp > 0 ? 5 : -5;
                        break;
                    case 2:
                        temp = temp > 0 ? 4 : -4;
                        break;
                    case 3:
                        temp = temp > 0 ? 3 : -3;
                        break;
                    case 4:
                        temp = temp > 0 ? 2 : -2;
                        break;
                }

                std::cout  <<"temp: " << temp << std::endl;
                st_g.b[ys][xs] = 0;
                st_g.b[yd][xd] = temp;
            }

            std::cout << move << std::endl;
        }
    }
    set_board();
    file.close();
}

void MainWindow::on_pushButton_clicked()
{
    st_g.white_pieces.clear();
    st_g.black_pieces.clear();
    st_g.white_moves.clear();
    st_g.black_moves.clear();
    init_board();
}

void MainWindow::on_pushButton_2_clicked()
{
    count = 0;
    ans = false;
    ans_moves.clear();
    ui->textEdit->clear();
    std::stringstream ss;

    auto start = std::chrono::steady_clock::now();
    //std::cout << "PROBLEM: " << problem << "\n";
    switch(problem)
    {
        case 0:
            Solver::find_mate_in(st_g, (aom * 2 - 1), side);
            break;

        case 1:
            // for(int r = 0; r < 8; r++)
            // {
            //     for(int c = 0; c < 8; c++)
            //     {
            //         if(st_g.b[r][c] > 0)
            //             white_pieces_value += Solver::get_value(st_g.b[r][c]);
            //         else if(st_g.b[r][c] < 0)
            //             black_pieces_value += Solver::get_value(st_g.b[r][c]);
            //     }
            // }
            // value_diff = white_pieces_value + black_pieces_value;
            Solver::helpmate(st_g, (aom * 2), side);
            break;

        case 2:
            Solver::other_problem(st_g, (aom * 2 - 1), side);
            break;
    }
    auto stop = std::chrono::steady_clock::now();
    std::chrono::duration<double> du = stop - start;
    std::cout << "time: " << du.count() << std::endl;
    std::cout << "states checked: " << count <<std::endl;

    set_board();

    for (auto e: ans_moves)
    {
        ss << (char)(e / 100 % 10 + 97);
        ss << 8 - e / 1000 % 10;
        ss << (char)(e % 10 + 97);
        ss << 8- e / 10 % 10;
        ss << '\n';
    }
    ui->textEdit->setText(QString::fromStdString(ss.str()));
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    aom = arg1;
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    edit = (bool)arg1;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    problem = index;
}

void MainWindow::on_comboBox_2_activated(int index)
{
    if(index == 0)
        side = true;
    else if(index == 1)
        side = false;
}

