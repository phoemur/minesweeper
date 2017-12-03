#ifndef QMINESWEEPER_HEADER
#define QMINESWEEPER_HEADER

#include <QWidget>
#include <QMainWindow>
#include <QSignalMapper>
#include <QString>
#include <QLabel>
#include <vector>
#include <random>
#include "qrightclickbutton.h"

struct Cell;

class Minesweeper : public QMainWindow {
    
    Q_OBJECT
    
    unsigned size_x;
    unsigned size_y;
    unsigned mines;
    unsigned num_flags;
    
    std::random_device seeder {};
    std::mt19937 engine;
    std::uniform_int_distribution<int> dist;
    
    QWidget *widget;
    QSignalMapper *signalMapper;
    QSignalMapper *signalMapper2;
    QLabel lab {QString("")};
    
    std::vector<std::vector<Cell>> nodes; //representation of the minefield
    
    // helper functions
    void create_nodes(unsigned x, unsigned y);
    void insert_mines();
    bool is_valid_coord(int a, int b) const noexcept;
    void fill_cells();
    void put_icon(QRightClickButton* btn, int row, int cow);
    void breadth_first_open(int row, int col);
    void open_all();
    void open_cell(int row, int col);
    void open_cell(QString& coordinates, int row, int col);
    bool check_end() const noexcept;
    void break_after_end();
    void update_statusbar();
public:
    Minesweeper(unsigned sz_x, 
                unsigned sz_y, 
                unsigned mns, 
                QWidget* parent = nullptr);
    Minesweeper() : Minesweeper(8, 8, 10) {};
    ~Minesweeper() noexcept;
    
private slots:
    void hasRightClicked(QString);
    void revealCell(QString);
    void reset();
    void about() noexcept;
};


#endif // QMINESWEEPER_HEADER