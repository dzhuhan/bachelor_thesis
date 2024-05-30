#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "solver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(663, 518));

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
    ew = nullptr;

    processBoard = board;
    ui->label->setPixmap(QPixmap::fromImage(board));
    ui->textEdit->setReadOnly(true);
    
    stat = new QLabel("");
    ui->statusbar->addWidget(stat);
    jump_fmt = new QTextCharFormat();
    jump_cursor = new QTextCursor(ui->textEdit->document());
    set_board();
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
    jump = 0;
    st_arr.clear();
    ans_moves.clear();
    
    for(int r = 0; r < 8; r++)
        for(int c = 0; c < 8; c++)
            st_g.b[r][c] = init[r][c];
    set_board();
}
void MainWindow::set_board()
{
    int x;
    int y;

    QImage *piece = nullptr;
    processBoard = board;
    ui->label->setPixmap(QPixmap::fromImage(board));

    st_g.white_pieces.clear();
    st_g.black_pieces.clear();
    st_g.white_moves.clear();
    st_g.black_moves.clear();
    
    st_g.white_kingside_castle_right = true;
    st_g.white_queenside_castle_right = true;
    st_g.black_kingside_castle_right = true;
    st_g.black_queenside_castle_right = true;
    for(int r = 0; r < 8; r++)
    {
        for(int c = 0; c < 8; c++)
        {
            piece = which_piece(init[r][c]);

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
                x = c * 50 + 37;
                y = r * 50 + 37;
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
}

void MainWindow::clear_board()
{
    jump = 0;
    st_arr.clear();
    ans_moves.clear();
    
    st_g.white_pieces.clear();
    st_g.black_pieces.clear();
    st_g.white_moves.clear();
    st_g.black_moves.clear();
    
    for(int r = 0; r < 8; r++)
        for(int c = 0; c < 8; c++)
            st_g.b[r][c] = 0;
            
    draw_board();
}

void MainWindow::draw_board()
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
                x = c * 50 + 37;
                y = r * 50 + 37;
                QPainter painter(&processBoard);
                painter.drawImage(x, y, *piece);
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

    r = ((e->pos().y() - 58 >= 0) ? e->pos().y() - 58 : -100) / 50;
    c = ((e->pos().x() - 37 >= 0) ? e->pos().x() - 37 : -100) / 50;

    if(select == false && c >= 0 && c < 8 && r >= 0 && r < 8)
    {
        row = r;
        col = c;

        if(e->buttons() == Qt::RightButton)
        {
            if(st_g.b[r][c] != 0)
            {
                st_g.b[r][c] = 0;
                select = false;
                jump = 0;
                st_arr.clear();
                set_board();
                draw_board();
            }
        }
        if(ew != nullptr && ew->piece != 0)
        {
            st_g.b[r][c] = ew->piece;
            PromotionWindow promotion;
            if((r == 0 && st_g.b[r][c] == 1) || (r == 7 && st_g.b[r][c] == -1))
            {
                promotion.setModal(true);
                promotion.exec();
                st_g.b[r][c] = (st_g.b[r][c] > 0) ? (promotion.prom / 10000) : (promotion.prom / -10000);
            }
            if(st_g.b[r][c] == 6 && st_g.b[st_g.white_king_pos.first][st_g.white_king_pos.second] == 6
                && st_g.white_king_pos != std::make_pair(r, c))
                    st_g.b[st_g.white_king_pos.first][st_g.white_king_pos.second] = 0;
            else if(st_g.b[r][c] == -6 && st_g.b[st_g.black_king_pos.first][st_g.black_king_pos.second] == -6
                && st_g.black_king_pos != std::make_pair(r, c))
                    st_g.b[st_g.black_king_pos.first][st_g.black_king_pos.second] = 0;             
            
            jump = 0;
            st_arr.clear();
            set_board();
            draw_board();
            ew->piece = 0;
            select = false;
        }
        else if(st_g.b[r][c] != 0)
            select = true;
    }
    else if(c >= 0 && c < 8 && r >= 0 && r < 8)
    {
        if(st_g.b[row][col] != 0
            && (edit || (Solver::legal_move(st_g, row * 1000 + col * 100 + r * 10 + c)
            && is_turn(row, col))))
        {
            if(row == r && col == c)
            {
                select = false;
                return;
            }
            if(st_g.b[row][col] > 0 && st_g.b[r][c] > 0)
                st_g.white_pieces.erase(std::remove(st_g.white_pieces.begin(), st_g.white_pieces.end(), std::make_pair(r, c)));
            else if(st_g.b[row][col] < 0 && st_g.b[r][c] < 0)
                st_g.black_pieces.erase(std::remove(st_g.black_pieces.begin(), st_g.black_pieces.end(), std::make_pair(r, c)));
              
            PromotionWindow promotion;
            if((r == 0 && st_g.b[row][col] == 1) || (r == 7 && st_g.b[row][col] == -1))
            {
                promotion.setModal(true);
                promotion.exec();
            }
            jump = 0;
            st_arr.clear();
            Solver::update_state(st_g, promotion.prom + row * 1000 + col * 100 + r * 10 + c);
            draw_board();
            select = false;
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

    clear_board();

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
                        case 'S':
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
                        case 's':
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
        if(temp == ' ')
            in >> temp;
        if(temp != '-')
        {
            st_g.en_passant.second = (int)(temp - 97);
            in >> temp;
            st_g.en_passant.first = 8 - (int)(temp - 48);
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
    draw_board();
    file.close();
}

void MainWindow::on_pushButton_clicked()
{
    turn = true;
    select = false;
    ans = false;
    init_board();
}

void MainWindow::on_pushButton_2_clicked()
{
    count = 0;
    ans = false;
    state temp;
    std::stringstream ss;
        
    ans_moves.clear();
    st_arr.clear();
    ui->textEdit->clear();

    
    auto start = std::chrono::steady_clock::now();

    switch(problem)
    {
        case 0:
            jump = aom * 2 - 1;
            st_arr.push_back(st_g);
            Solver::find_mate_in(st_g, (aom * 2 - 1), side);
            break;

        case 1:
            jump = aom * 2;
            st_arr.push_back(st_g);
            Solver::helpmate(st_g, (aom * 2), side);
            break;

        case 2:
            jump = aom * 2;
            st_arr.push_back(st_g);
            Solver::selfmate(st_g, (aom * 2), side);
            break;
    }
    
    auto stop = std::chrono::steady_clock::now();
    std::chrono::duration<double> du = stop - start;
    auto hrs = std::chrono::duration_cast<std::chrono::hours>(du);
    auto min = std::chrono::duration_cast<std::chrono::minutes>(du - hrs);
    auto sec = std::chrono::duration_cast<std::chrono::seconds>(du - hrs - min);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(du - hrs - min - sec);
    ss << "time: " << hrs.count() << "h " << min.count() << "m " 
        << sec.count() << "s " << ms.count() << "ms" << "        " << "states checked: " << count;
    stat->setText(QString::fromStdString(ss.str()));
    
    draw_board();

    if(ans)
    {
        ss.str(std::string());
        for (long i = 0; i < ans_moves.size(); i++)
        {
            temp = st_arr[i];
            Solver::update_state(temp, ans_moves[i]);
            st_arr.push_back(temp);
            
            ss << (char)(ans_moves[i] / 100 % 10 + 97);
            ss << 8 - ans_moves[i] / 1000 % 10;
            ss << (char)(ans_moves[i] % 10 + 97);
            ss << 8- ans_moves[i] / 10 % 10;
            if(i % 2 == 1)
                ss << '\n';
            else
                ss << ' ';
        }
        ui->textEdit->setText(QString::fromStdString(ss.str()));
        highlight_move();
    }
    else
    {
        jump = 0;
        st_arr.clear();
    }
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    aom = arg1;
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    edit = (bool)arg1;
    if(ew == nullptr)
        ew = new Editwindow(this);
    if(edit)
        ew->show();
    else if(ew != nullptr)
    {
        ew->close();
    }
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


void MainWindow::on_Clear_clicked()
{
    clear_board();
}

void MainWindow::highlight_move()
{
    jump_fmt->setBackground(Qt::white);
    jump_cursor->setCharFormat(*jump_fmt);
    if(jump > 0)
    {
        jump_fmt->setBackground(Qt::cyan);
        jump_cursor->setPosition((jump - 1) * 5, QTextCursor::MoveAnchor);
        jump_cursor->setPosition((jump - 1) * 5 + 4, QTextCursor::KeepAnchor);
        jump_cursor->setCharFormat(*jump_fmt);
    }
    else
    {
        jump_fmt->setBackground(Qt::lightGray);
        jump_cursor->setPosition(0, QTextCursor::MoveAnchor);
        jump_cursor->setPosition(4, QTextCursor::KeepAnchor);
        jump_cursor->setCharFormat(*jump_fmt);
    }
}

void MainWindow::on_previous_clicked()
{
    if(!st_arr.empty() && jump > 0 && jump <= st_arr.size())
    {
        st_g = st_arr[--jump];
        set_board();
        draw_board();
        highlight_move();
    }
}


void MainWindow::on_next_clicked()
{
    if(!st_arr.empty() && jump < st_arr.size() - 1)
    {
        st_g = st_arr[++jump];
        set_board();
        draw_board();
        highlight_move();
    }
}


void MainWindow::on_first_clicked()
{
    if(!st_arr.empty())
    {
        st_g = st_arr[0];
        set_board();
        draw_board();
        jump = 0;
        highlight_move();
    }
}


void MainWindow::on_last_clicked()
{
    if(!st_arr.empty() && jump < st_arr.size())
    {
        jump = st_arr.size();
        st_g = st_arr[--jump];
        set_board();
        draw_board();
        highlight_move();
    }
}

