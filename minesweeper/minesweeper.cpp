#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    
    Minesweeper window (8, 8, 10); // (size, number of mines)
    window.move(200, 200);
    window.setWindowTitle("Minesweeper");
    window.show();

    return app.exec();
}
