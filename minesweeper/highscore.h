#ifndef HIGHSCORE_HEADER
#define HIGHSCORE_HEADER

#include <QDialog>
#include <QLabel>
#include <QPushButton>

class Highscore : public QDialog {
    Q_OBJECT
    
    QLabel *label;
    QLabel *scores;
    QPushButton *close;
public:
    Highscore(QWidget* parent = nullptr);
    ~Highscore() = default;
    
};

#endif // HIGHSCORE_HEADER