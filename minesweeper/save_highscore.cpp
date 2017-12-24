#include "save_highscore.h"
#include "helper.h"

#include <fstream>
#include <vector>
#include <string>
#include <set>


SaveHighscore::SaveHighscore(int sc, int lv, QWidget* parent)
    : QDialog(parent), score{sc}, level{lv}
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    label = new QLabel(this);
    label->setGeometry(QRect(40, 20, 100, 22));
    label->setText("Enter your name: ");
    
    namefield = new QLineEdit(this);
    namefield->setGeometry(QRect(40, 47, 180, 22));
    namefield->setAlignment(Qt::AlignLeading|Qt::AlignRight|Qt::AlignTop);
    
    okay = new QPushButton(this);
    okay->setGeometry(QRect(140, 87, 93, 22));
    okay->setText("OK");
    connect(okay, SIGNAL(clicked()), this, SLOT(update()));
    
    if (this->is_highscore()) {
        this->show();
    }
    else {
        this->close();
    }
    
}

void SaveHighscore::update()
{
    std::string name = namefield->text().toStdString();
    
    std::set<std::pair<std::string,std::string>, helper_minesweeper::pair_compare> scores_set;
    
    std::ifstream myfile {filename, std::ios::in};
    if (!myfile.is_open()) {
        throw std::runtime_error("Could not read highscore file");
    }
    
    std::string buffer, result;
    for (int i=0; i<9; ++i) {
        if (i == level) {break;}
        getline(myfile, buffer);
        result += buffer;
        result += "\n";
    }
    for (int i=0; i<3; ++i) {
        getline(myfile, buffer);
        auto tokens = helper_minesweeper::split_string(buffer, delimiter);
        scores_set.insert(std::pair<std::string,std::string>(tokens[0], tokens[1]));
    }
    scores_set.insert(std::pair<std::string,std::string>(name, std::to_string(score)));
    
    auto it = scores_set.begin();
    for (int i=0; i<3; ++i) {
        result += it->first;
        result.push_back(delimiter);
        result += it->second;
        result += "\n";
        ++it;
    }
    
    while (!myfile.eof()) {
        getline(myfile, buffer);
        result += buffer;
        result += "\n";
    }
    
    myfile.close();
    helper_minesweeper::rtrim(result);
    std::ofstream myfile2 {filename, std::ios::out | std::ios::trunc};
    if (!myfile2.is_open()) {
        throw std::runtime_error("Could not write highscore file");
    }
    myfile2 << result;
    myfile2.close();
    this->close();
}

bool SaveHighscore::is_highscore()
{  
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
    
    for (int i=0; i<9; ++i) {
        if (i == level) {break;}
        getline(myfile2, buffer);
    }
    
    bool flag = false;
    for (int i=0; i<3; ++i) {
        getline(myfile2, buffer);
        auto tokens = helper_minesweeper::split_string(buffer, delimiter);
        if (tokens[0] == "-") {
            flag = true;
            break;
        }
        else if (score <= stol(tokens[1])) {
            flag = true;
            break;
        }
    }
    myfile2.close();
    return flag;   
}