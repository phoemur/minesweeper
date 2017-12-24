#ifndef HIGHSCORE_HEADER
#define HIGHSCORE_HEADER

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QString>

#include <string>

class Highscore : public QDialog {
    Q_OBJECT
    
    QLabel *label;
    
    QPushButton *close;
    
    std::string filename {"highscore.dat"};
    
    //helper functions
    QString parse_file();
public:
    QLabel *scores;
    Highscore(QWidget* parent = nullptr);
    ~Highscore() = default;
    
}; // class Highscore

#endif // HIGHSCORE_HEADER