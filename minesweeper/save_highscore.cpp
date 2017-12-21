#include "save_highscore.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>

struct pair_compare {
    bool operator()(const std::pair<std::string,std::string>& lhs,
                    const std::pair<std::string,std::string>& rhs)
    {
        if (rhs.second == "-") {return true;}
        else if (lhs.second == "-") {return false;}
        else {
            return stol(lhs.second) < stol(rhs.second);
        }
    }
};

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
    
    std::set<std::pair<std::string,std::string>, pair_compare> scores_set;
    
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
        auto tokens = split_string(buffer, delimiter);
        scores_set.insert(std::pair<std::string,std::string>(tokens[0], tokens[1]));
    }
    scores_set.insert(std::pair<std::string,std::string>(name, std::to_string(score)));
    scores_set.erase(std::prev(scores_set.end()));
    
    for (auto& p: scores_set) {
        result += p.first;
        result.push_back(delimiter);
        result += p.second;
        result += "\n";
    }
    
    while (!myfile.eof()) {
        getline(myfile, buffer);
        result += buffer;
        result += "\n";
    }
    
    myfile.close();
    
    std::ofstream myfile2 {filename, std::ios::out | std::ios::trunc};
    if (!myfile2.is_open()) {
        throw std::runtime_error("Could not write highscore file");
    }
    myfile2 << result;
    myfile2.close();
    this->close();
}

inline bool SaveHighscore::file_exists(const std::string& name)
{
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

std::vector<std::string> SaveHighscore::split_string(const std::string& input, 
                                                     const char delimiter) 
{
    std::stringstream ss {input};
    std::vector<std::string> result;
    // result.reserve(count(begin(input), end(input), delimiter));
    
    for (std::string buffer; 
         getline(ss, buffer, delimiter);) 
            {result.push_back(std::move(buffer));}
    
    return result;
}

bool SaveHighscore::is_highscore()
{  
    if (!file_exists(filename)) {
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
    for (int i=0; i<3; ++i) {
        getline(myfile2, buffer);
        auto tokens = split_string(buffer, delimiter);
        if (tokens[0] == "-") {
            return true;
        }
        else if (score < stol(tokens[1])) {
            return true;
        }
    }
    myfile2.close();
    return false;    
}