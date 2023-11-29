
#include "advanced_chars_tab.h"

Advanced_chars_tab::Advanced_chars_tab(QWidget *parent)
    : QWidget(parent)
{

}

Advanced_chars_tab::Advanced_chars_tab(QWidget *parent, std::map<QString, int>& stats) : QWidget(parent)
{
    main_layout = new QHBoxLayout(this);
    keys = new QVBoxLayout(this);
    values = new QVBoxLayout(this);

    for (std::pair<QString, int> p : stats){
        QLabel* key = new QLabel(this);
        key->setText(p.first);
        key->setStyleSheet("font-size: 12px;"
                           "font-style: italic;");
        keys->addWidget(key);

        QLabel* value = new QLabel(this);
        value->setText(QString::number(p.second));
        value->setAlignment(Qt::AlignHCenter);
        values->addWidget(value);
    }

    main_layout->addLayout(keys);
    main_layout->addLayout(values);
    setLayout(main_layout);
}

