#include "mainwindow.h"
#include "qrightclickbutton.h"

#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QMenu>
#include <QMenuBar>
#include <QIcon>
#include <QAction>
#include <QMessageBox>
#include <QStatusBar>

#include <deque>
#include <utility>
#include <algorithm>

struct Cell { //represents one cell in the minefield
    int val = 0; // 0 - 8 for showing adjacent mines.
    bool is_flag = false;
    bool is_mine = false;
    bool is_open = false;
};

Minesweeper::Minesweeper(unsigned sz_x, 
                         unsigned sz_y, 
                         unsigned mns, 
                         QWidget* parent)
    : QMainWindow(parent), 
      size_x{sz_x}, 
      size_y{sz_y}, 
      mines{mns},
      num_flags{0},
      engine(this->seeder()),
      dist(0, (size_x*size_y)),
      widget{new QWidget()},
      nodes{}
{
    // Preliminay checks
    if (mines > size_x*size_y) {throw std::runtime_error("More mines than cells");}
    if (mines <= 0 || size_x <= 0 || size_y <= 0) {
        throw std::runtime_error("Cannot be zero or less");
    }
    
    // Create minefield
    create_nodes(size_x, size_y);
    insert_mines();
    fill_cells();
    
    // Menu File
    QPixmap quit_pix("images/exit.gif");
    QPixmap new_pix("images/new.gif");
    QAction *quit = new QAction(quit_pix, "&Quit", widget);
    QAction *new_game = new QAction(new_pix, "&New game", widget);
    quit->setShortcut(tr("CTRL+Q"));
    new_game->setShortcut(tr("CTRL+N"));
    QMenu *file;
    file = menuBar()->addMenu("&File");
    file->addAction(new_game);
    file->addSeparator();
    file->addAction(quit);
    connect(quit, &QAction::triggered, qApp, QApplication::quit);
    connect(new_game, &QAction::triggered, this, &Minesweeper::reset);
    
    // Menu Help
    QAction *sobre = new QAction("&About", widget);
    sobre->setShortcut(tr("CTRL+H"));
    QMenu *ajuda;
    ajuda = menuBar()->addMenu("&Help");
    ajuda->addAction(sobre);
    connect(sobre, &QAction::triggered, this, &Minesweeper::about);
    
    qApp->setAttribute(Qt::AA_DontShowIconsInMenus, false);
    
    
    // Layout
    QGridLayout *grid = new QGridLayout(widget);
    grid->setHorizontalSpacing(1);
    grid->setVerticalSpacing(1);
    
    
    // Buttons
    
    //We will need to map the click to an object's coordinates
    signalMapper = new QSignalMapper(widget);
    signalMapper2 = new QSignalMapper(widget);
    
    for (size_t i=0; i<size_x; ++i) {
        for (size_t j=0; j<size_y; ++j) {
            QRightClickButton *btn = new QRightClickButton(widget);
            btn->setAttribute(Qt::WA_LayoutUsesWidgetRect);
            grid->addWidget(btn, i, j);
            QString coordinates = QString::number(i)+","+QString::number(j); //Coordinate of the button
            //Map the coordinates to a particular MineSweeperButton
            signalMapper->setMapping(btn, coordinates);
            signalMapper2->setMapping(btn, coordinates);
            
            //Connections for the buttons
            connect(btn, SIGNAL(leftClicked()), signalMapper, SLOT(map()));
            connect(btn, SIGNAL(rightClicked()), signalMapper2, SLOT(map()));
        }
    }
    
    //Connect the signal mapper to this class so that we can handle its clicks
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(revealCell(QString))); //Left click
    connect(signalMapper2, SIGNAL(mapped(QString)), this, SLOT(hasRightClicked(QString))); //Right click
    
    // Window style
    QString styleSheet =
    "QRightClickButton {"
    "  width: 25px; height: 25px;"
    "  border-width: 1px;"
    "  border-style: outset;"
    "  border-color: black;"
    "  border-radius: 2px;"
    "  background-color: rgb(180, 180, 180);"
    "}"
    "QRightClickButton:hover {"
      "background-color: lightblue;"
    "}"
    "QRightClickButton:pressed {"
      "background-color: rgb(220, 220, 220);"
    "}";
    widget->setStyleSheet(styleSheet);
    widget->setLayout(grid);
    setCentralWidget(widget);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
    
    this->statusBar()->addPermanentWidget(&lab);
    this->statusBar()->setSizeGripEnabled(false);
    QIcon icon (QPixmap("images/mine.png"));
    this->setWindowIcon(icon);
    update_statusbar();
}


Minesweeper::~Minesweeper() noexcept
{
    delete widget; // everything that inherits from widget will also be deleted here
}

void Minesweeper::hasRightClicked(QString coordinates)
{
    //qDebug() << "Right clicked: " << coordinates << "\n";
    
    static QIcon flag, empty;
    flag.addPixmap(QPixmap("images/flag.png"), QIcon::Normal);
    empty.addPixmap(QPixmap(""), QIcon::Normal);
    
    
    // Obtain its coordinates
    QStringList results = coordinates.split(",");
    if ( results.size() != 2) { //Ensure that we receive two coordinates
        qFatal("Less than two numbers received");
    }
    int row = results.at(0).toInt();
    int col = results.at(1).toInt();
    
    QRightClickButton* buttonPushed = qobject_cast<QRightClickButton*>(signalMapper->mapping(coordinates));
    
    if (nodes[row][col].is_flag) {
        buttonPushed->setIcon(empty);
        nodes[row][col].is_flag = false;
        num_flags--;
    }
    else {
        buttonPushed->setIcon(flag);
        nodes[row][col].is_flag = true;
        num_flags++;
    }
    update_statusbar();
}

void Minesweeper::revealCell(QString coordinates)
{
    //qDebug() << "Left clicked: " << coordinates << "\n";
    
    // Obtain its coordinates
    QStringList results = coordinates.split(",");
    if ( results.size() != 2) { //Ensure that we receive two coordinates
        qFatal("Less than two numbers received");
    }
    int row = results.at(0).toInt();
    int col = results.at(1).toInt();
    
    if (nodes[row][col].is_flag) {return;}
    else if (nodes[row][col].is_mine) {
        open_all();
        QMessageBox::warning(this, "Ouch!", "SORRY, YOU LOST!!!");
        return;
    }
    
    open_cell(coordinates, row, col);
    
    if (nodes[row][col].val == 0) {
        breadth_first_open(row, col);
    }
    
    if (check_end()) {
        break_after_end();
        QMessageBox::information(this, "Congratulations", "CONGRATULATIONS, YOU WIN!!");
    }
}

void Minesweeper::create_nodes(unsigned x, unsigned y)
{ // populates the matrix with default values
    using sz_t = std::vector<std::vector<Cell>>::size_type;
    
    nodes.reserve(x);
    for (sz_t i = 0; i < x; ++i) {
        nodes.emplace_back(std::vector<Cell>(y));
    }
}

void Minesweeper::insert_mines()
{ // insert n random mines in the matrix
    int a, b, counter = 0;
    
    while (counter < static_cast<int>(mines)) {
        a = dist(engine) % size_x;
        b = dist(engine) % size_y;
        if (!nodes[a][b].is_mine) {
            nodes[a][b].is_mine = true;
            ++counter;
        }
    }
}

inline bool Minesweeper::is_valid_coord(int a, int b) const noexcept
// checks if is within the matrix
{
    if (a >= 0 && a < static_cast<int>(size_x) && 
        b >= 0 && b < static_cast<int>(size_y)) {
        return true;
    }else {return false;}
}

void Minesweeper::fill_cells()
/* This function populates the cell values according to
       the neighbouring mines */
{
    int n = size_x;
    int m = size_y;
    
    for (int i=0; i<n; ++i) {
        for (int j=0; j<m; ++j) {
            for (int b=-1; b<=1; ++b) {
                for (int a=-1; a<=1; ++a) {
                    if (is_valid_coord(b+i, a+j)) {
                        if (nodes[b+i][a+j].is_mine) {
                            nodes[i][j].val++;
                        }
                    }
                }
            }
        }
    }
}

void Minesweeper::put_icon(QRightClickButton* btn, int row, int col)
{
    static QIcon empty, i1, i2, i3, i4, i5, i6, i7, i8, mine;
    empty.addPixmap(QPixmap(""), QIcon::Disabled);
    i1.addPixmap(QPixmap("images/1.png"), QIcon::Disabled);
    i2.addPixmap(QPixmap("images/2.png"), QIcon::Disabled);
    i3.addPixmap(QPixmap("images/3.png"), QIcon::Disabled);
    i4.addPixmap(QPixmap("images/4.png"), QIcon::Disabled);
    i5.addPixmap(QPixmap("images/5.png"), QIcon::Disabled);
    i6.addPixmap(QPixmap("images/6.png"), QIcon::Disabled);
    i7.addPixmap(QPixmap("images/7.png"), QIcon::Disabled);
    i8.addPixmap(QPixmap("images/8.png"), QIcon::Disabled);
    mine.addPixmap(QPixmap("images/mine.png"), QIcon::Disabled);
    
    if (nodes[row][col].is_mine) {
        btn->setIcon(mine);
    }
    else {
        switch (nodes[row][col].val) {
            case 0:
                btn->setIcon(empty);
                break;
            case 1:
                btn->setIcon(i1);
                break;
            case 2:
                btn->setIcon(i2);
                break;
            case 3:
                btn->setIcon(i3);
                break;
            case 4:
                btn->setIcon(i4);
                break;
            case 5:
                btn->setIcon(i5);
                break;
            case 6:
                btn->setIcon(i6);
                break;
            case 7:
                btn->setIcon(i7);
                break;
            case 8:
                btn->setIcon(i8);
                break;
            default:
                btn->setIcon(empty);
                break;
        }
    }
}

void Minesweeper::breadth_first_open(int row, int col)
{ // open all adjacent zeroes (Breadth First Search)
    using namespace std;
    
    if (nodes[row][col].val != 0) {return;}
    
    vector<vector<bool>> visited (size_x, vector<bool>(size_y, false));
    deque<pair<int,int>> c_queue;
    c_queue.push_back(pair<int,int>(row,col));
    visited[row][col] = true;
    
    while (!c_queue.empty()) {
        auto p = c_queue.front();
        c_queue.pop_front();
        
        for (int b=-1; b<=1; ++b) {
            for (int a=-1; a<=1; ++a) {
                if (is_valid_coord(b+p.first, a+p.second)) {
                    if (!nodes[b+p.first][a+p.second].is_flag) {
                        open_cell(b+p.first, a+p.second);
                    }
                    if (!visited[b+p.first][a+p.second] &&
                        nodes[b+p.first][a+p.second].val == 0) {
                            c_queue.push_back(pair<int,int>(b+p.first, a+p.second));
                            visited[b+p.first][a+p.second] = true;
                    }
                }
            }
        }
    }
}

void Minesweeper::open_all()
{
    for (size_t i=0; i<size_x; ++i) {
        for (size_t j=0; j<size_y; ++j) {
            open_cell(i, j);
            nodes[i][j].is_open = true;
        }
    }
}

inline void Minesweeper::open_cell(int row, int col)
{
    QString coordinates = QString::number(row)+","+QString::number(col);
    open_cell(coordinates, row, col);
}

inline void Minesweeper::open_cell(QString& coordinates, int row, int col)
{
    QRightClickButton* buttonPushed = qobject_cast<QRightClickButton*>(signalMapper->mapping(coordinates));
    buttonPushed->setEnabled(false);
    buttonPushed->setDown(true);
    put_icon(buttonPushed, row, col);
    nodes[row][col].is_open = true;
}

bool Minesweeper::check_end() const noexcept
{ // end is when there is no cell without a mine to open
    unsigned counter = 0;
    
    for (size_t i=0; i<size_x; ++i) {
        for (size_t j=0; j<size_y; ++j) {
            if (nodes[i][j].is_open && !nodes[i][j].is_mine) {
                counter++;
            }
        }
    }
    
    return counter >= (size_x*size_y)-mines ? true: false;
}

void Minesweeper::break_after_end()
{
    static QIcon flag;
    flag.addPixmap(QPixmap("images/flag.png"), QIcon::Normal);
    
    for (size_t i=0; i<size_x; ++i) {
        for (size_t j=0; j<size_y; ++j) {
            if (!nodes[i][j].is_open) {
                QString coordinates = QString::number(i)+","+QString::number(j);
                QRightClickButton* buttonPushed = qobject_cast<QRightClickButton*>(signalMapper->mapping(coordinates));  
                
                if (!nodes[i][j].is_flag) {
                    buttonPushed->setEnabled(false);
                    buttonPushed->setIcon(flag);
                    num_flags++;                    
                }
                else {
                    buttonPushed->setEnabled(false);
                }
                update_statusbar();
            }
        }
    }
}

void Minesweeper::update_statusbar()
{
    QString msg = "Mines: " + QString::number(num_flags) + " / " + QString::number(mines) + "   ";
    //this->statusBar()->showMessage(msg);
    lab.setText(msg);
}

void Minesweeper::reset()
{
    static QIcon empty;
    num_flags = 0;
    nodes = {};
    create_nodes(size_x, size_y);
    insert_mines();
    fill_cells();
    
    for (size_t i=0; i<size_x; ++i) {
        for (size_t j=0; j<size_y; ++j) {
            QString coordinates = QString::number(i)+","+QString::number(j);
            QRightClickButton* buttonPushed = qobject_cast<QRightClickButton*>(signalMapper->mapping(coordinates));
            buttonPushed->setEnabled(true);
            buttonPushed->setDown(false);
            buttonPushed->setIcon(QIcon());
        }
    }
    update_statusbar();
}

void Minesweeper::about() noexcept
{
    QMessageBox::information(this, "About",
      "Minesweeper version 0.1\nAuthor: Fernando B. Giannasi\nNov/2017");
}