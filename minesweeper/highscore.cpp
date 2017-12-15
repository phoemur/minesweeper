#include "highscore.h"
#include <QString>

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
    QString msg = "Beginner:\n1. Teste\n2. Teste\n3. Teste\n\nIntermediate:\n1. Teste\n2. Teste\n3. Teste\n\nExpert:\n1. Teste\n2. Teste\n3. Teste\n\n";
    scores->setText(msg);
    
    close = new QPushButton(this);
    close->setGeometry(QRect(140, 320, 93, 27));
    close->setText("Close");
    connect(close, SIGNAL(clicked()), this, SLOT(close()));
}