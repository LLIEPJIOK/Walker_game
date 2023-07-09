#include "exitwindow.h"

ExitWindow::ExitWindow(QWidget *parent) : QWidget(parent)
{
    QFont font ("Arial", 16, QFont::Normal, 1);
    QString style("color: rgb(255, 255, 255)");
    Q_UNUSED(parent);
    label = new QLabel();
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);
    label->setText("Вы точно хотите выйти?");
    label->setStyleSheet(style);
    btn_yes = new QPushButton();
    btn_yes->setFont(font);
    btn_yes->setText("Да");
    btn_yes->setFlat(1);
    btn_yes->setStyleSheet(style);
    btn_no = new QPushButton();
    btn_no->setFont(font);
    btn_no->setText("Нет");
    btn_no->setFlat(1);
    btn_no->setStyleSheet(style);
    gridlay = new QGridLayout();
    gridlay->setSpacing(70);
    gridlay->addWidget(label, 0, 1, 1, 2);
    gridlay->addWidget(btn_yes, 3, 0);
    gridlay->addWidget(btn_no, 3, 3);
    hblay = new QHBoxLayout();
    hblay->addLayout(gridlay);
    hblay->setAlignment(Qt::AlignCenter);
    vblay = new QVBoxLayout();
    vblay->addLayout(hblay);
    vblay->setAlignment(Qt::AlignCenter);
    setLayout(vblay);
    connect(btn_yes, &QPushButton::clicked, this, &ExitWindow::close_slot);
    connect(btn_no, &QPushButton::clicked, this, &ExitWindow::return_slot);
}

void ExitWindow::close_slot()
{
    emit signal_open_menu();
    QApplication::quit();
}

void ExitWindow::return_slot()
{
    emit signal_open_menu();
    close();
}
