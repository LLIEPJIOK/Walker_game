
#include "actionwindow.h"

ActionWindow::ActionWindow(QWidget *parent)
    : QFrame{parent}
{
    setFixedSize( 0.195 * parent->width(), 0.325 * parent->height() - 10);
    setFrameStyle(QFrame::WinPanel | QFrame::Plain);
    QPalette pal;
    pal.setBrush(backgroundRole(), QBrush(Qt::white));
    setPalette(pal);
    setAutoFillBackground(true);
    info_text = new QLabel(this);
    info_text->setWordWrap(true);
    info_text->setGeometry(5, 5, width() - 10, height() - 10);
    info_text->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    info_text->setFont(QFont("Arial", 20, QFont::Normal, 1));
}

QString ActionWindow::get_text()
{
    return info_text->text();
}

void ActionWindow::set_text(QString str)
{
    info_text->setText(str);
}

