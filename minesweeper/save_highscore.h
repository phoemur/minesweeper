#ifndef SAVE_HIGHSCORE_HEADER
#define SAVE_HIGHSCORE_HEADER

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QLineEdit>

#include <vector>
#include <string>

class SaveHighscore : public QDialog {
    Q_OBJECT
    
    QLabel *label;
    QPushButton *okay;
    QLineEdit *namefield;
    int score;
    int level;
    
    const char delimiter = 30;
    const std::vector<std::string> difs {"Beginner", "Intermediate", "Expert"};
    std::string filename {"highscore.csv"};
    
    //helper functions
    bool is_highscore();
    bool file_exists(const std::string& name);
    std::vector<std::string> split_string(const std::string& input, const char delimiter);
public:
    SaveHighscore(int sc, int lv, QWidget* parent = nullptr);
    ~SaveHighscore() = default;
    
private slots:
    void update();
    
}; // class SaveHighscore


#endif // SAVE_HIGHSCORE_HEADER