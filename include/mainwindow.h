#ifndef QMINESWEEPER_HEADER
#define QMINESWEEPER_HEADER

#include <QWidget>
#include <QMainWindow>
#include <QString>
#include <QLabel>
#include <QGridLayout>
#include <QVector>
#include <QTimer>
#include <vector>
#include <random>
#include <utility>
#include "qrightclickbutton.h"
#include "highscore.h"
#include "save_highscore.h"

struct Cell;

class Minesweeper : public QMainWindow {
    
    Q_OBJECT
    
    std::size_t size_x;
    std::size_t size_y;
    std::size_t mines;
    std::size_t num_flags;
    std::size_t open_so_far;
    double elapsed_time;
    int current_level;
    
    std::random_device seeder {};
    std::mt19937 engine;
    std::uniform_int_distribution<std::size_t> dist;
    
    QWidget *widget;
    QLabel lab {QString("")};
    QGridLayout *grid = nullptr;
    QVector<QVector<QRightClickButton *>> btn_storage;
    
    QTimer *timer = nullptr;
    
    std::vector<std::vector<Cell>> nodes; //representation of the minefield
    
    // helper functions
    void create_nodes(std::size_t x, std::size_t y);
    void insert_mines();
    bool is_valid_coord(int a, int b) const noexcept;
    void fill_cells();
    void create_buttons();
    void put_icon(QRightClickButton* btn, std::size_t row, std::size_t col);
    void breadth_first_open(std::size_t row, std::size_t col);
    void open_all();
    void open_cell(std::size_t row, std::size_t col);
    bool check_end() const noexcept;
    void break_after_end();
    void update_statusbar();
    
    
    explicit Minesweeper(std::size_t sz_x,
                         std::size_t sz_y,
                         std::size_t mns,
                         QWidget* parent = nullptr);
public:
    
    Minesweeper() : Minesweeper(8, 8, 10) {}
    ~Minesweeper() noexcept;
    
private slots:
    void hasRightClicked(std::size_t row, std::size_t col);
    void revealCell(std::size_t row, std::size_t col);
    void reset();
    void about() noexcept;
    void update_clock();
    void set_beginner();
    void set_intermediate();
    void set_expert();
    void show_highscore();
};


#endif // QMINESWEEPER_HEADER
