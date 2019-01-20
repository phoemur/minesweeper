#include "qrightclickbutton.h"

QRightClickButton::QRightClickButton(QWidget *parent) :
    QPushButton(parent)
{
}

void QRightClickButton::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::RightButton) {
        emit rightClicked();
        //qDebug() << "rightclicked";
    }
    else if(e->buttons() == Qt::LeftButton) {
        emit leftClicked();
        //qDebug() << "leftclicked";
    }
}
