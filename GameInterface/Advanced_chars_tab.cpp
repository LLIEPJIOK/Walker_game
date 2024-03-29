
#include "advanced_chars_tab.h"

Advanced_chars_tab::Advanced_chars_tab(QWidget *parent)
    : QWidget(parent)
{
    for (auto k : keys_lo->children()){
        delete k;
    }

    delete keys_lo;

    for (auto k : values_lo->children()){
        delete k;
    }

    delete values_lo;
    delete main_layout;
}


Advanced_chars_tab::Advanced_chars_tab(QWidget *parent, std::map<QString, QString>& _stat_names, Player* assigned_player) : QWidget(parent)
{
    main_layout = new QHBoxLayout(this);
    keys_lo = new QVBoxLayout();
    values_lo = new QVBoxLayout();

    stat_names = _stat_names;

    for (const std::pair<QString, QString> &p : _stat_names) {
        QLabel* key = new QLabel(this);
        key->setText(tr(p.second.toStdString().c_str()));
        key->setStyleSheet("font-size: 16px;"
                           "font-style: arial;");
        keys_lo->addWidget(key);

        QLabel* value = new QLabel(this);
        value->setText(tr(std::to_string(assigned_player->get_characteristics().at(p.first.toStdString())).c_str()));
        value->setAlignment(Qt::AlignHCenter);
        values_lo->addWidget(value);

        chars[p.first] = value;
    }

    main_layout->setAlignment(Qt::AlignTop);
    main_layout->addLayout(keys_lo);
    main_layout->addLayout(values_lo);
    setLayout(main_layout);
}

void Advanced_chars_tab::update_chars(Player* assigned_player){
    for (auto i : chars) {
        i.second->setText(tr(std::to_string(assigned_player->get_characteristics().at(i.first.toStdString())).c_str()));
    }
}

