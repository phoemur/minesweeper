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
    
    std::string filename {"highscore.csv"};
    
    //helper functions
    bool file_exists(const std::string& name);
    std::vector<std::string> split_string(const std::string& input, const char delimiter);
    QString parse_file();
public:
    QLabel *scores;
    Highscore(QWidget* parent = nullptr);
    ~Highscore() = default;
    
}; // class Highscore

#endif // HIGHSCORE_HEADER