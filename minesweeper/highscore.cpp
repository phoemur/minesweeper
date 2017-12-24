#include "highscore.h"
#include "helper.h"

#include <fstream>
#include <sstream>
#include <vector>

Highscore::Highscore(QWidget* parent)
    : QDialog(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    label = new QLabel(this);
    label->setGeometry(QRect(40, 20, 181, 41));
    label->setText("<b><h style=\" font-family:'Sans'; font-size:14pt; font-weight:400;\">Highscores:</h></b>");
    
    scores = new QLabel(this);
    scores->setGeometry(QRect(20, 70, 231, 221));
    scores->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
    QString msg = parse_file();
    scores->setText(msg);
    
    close = new QPushButton(this);
    close->setGeometry(QRect(140, 320, 93, 27));
    close->setText("Close");
    connect(close, SIGNAL(clicked()), this, SLOT(close()));
}

QString Highscore::parse_file()
{
    static const char delimiter = 30;
    static const std::vector<std::string> difs {"Beginner", "Intermediate", "Expert"};
    
    if (!helper_minesweeper::file_exists(filename)) {
        std::ofstream myfile {filename, std::ios::out};
        if (!myfile.is_open()) {
            throw std::runtime_error("Could not write to highscore file");
        }
        
        for (int i=0; i< 9; ++i) {
            myfile << "-" << delimiter << "-\n";
        }
        myfile.close();
    }
    
    std::ifstream myfile2 {filename, std::ios::in};
    if (!myfile2.is_open()) {
        throw std::runtime_error("Could not read highscore file");
    }
    
    std::string buffer;
    QString result;
    for (auto& ent: difs) {
        result += QString::fromStdString(ent);
        result += ":\n";
        for (int i=1; i<=3; ++i) {
            result += QString::number(i);
            result += ". ";
            getline(myfile2, buffer);
            auto tokens = helper_minesweeper::split_string(buffer, delimiter);
            if (tokens[0] == "-") {
                result += "\n";
            }
            else {
                result += QString::fromStdString(tokens[0]);
                result += "   : ";
                result += QString::fromStdString(tokens[1]);
                result += "s\n";
            }
        }
        result += "\n\n";
    }
    
    return result;
}