#include "exitwindow.h"

ExitWindow::ExitWindow(QWidget *parent) : QWidget(parent)
{
    QFont btn_font("Arial", 14, QFont::Normal, 1);
    setStyleSheet("QPushButton        {color: white;}"
                  "QPushButton:hover  {color: rgb(255, 178, 102);}"
                  "QLabel             {color: white;}");

    label = new QLabel();
    label->setFont(QFont("Arial", 16));
    label->setAlignment(Qt::AlignCenter);
    label->setText("Вы точно хотите выйти?");

    btn_yes = new QPushButton("Да");
    btn_yes->setFont(btn_font);
    btn_yes->setFlat(1);

    btn_no = new QPushButton("Нет");
    btn_no->setFont(btn_font);
    btn_no->setFlat(1);

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
