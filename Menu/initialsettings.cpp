#include "initialsettings.h"

InitialSettings::InitialSettings(int _id, QWidget *parent)
    : QFrame(parent)
{
    id = _id;
    kol = 4;

    QFont font("Arial", 14, QFont::Normal, 1);
    setStyleSheet("QPushButton:active   {color: white;}"
                  "QPushButton:disabled {color: gray;}"
                  "QPushButton:hover    {color: rgb(255, 178, 102);}"
                  "QLabel               {color: white;}"
                  "QCheckBox            {color: white;}"
                  "QCheckBox:disabled   {color: gray;}"
                  "QCheckBox:hover      {color: rgb(255, 178, 102);}");

    label_id = new QLabel("Игрок " + QString::number(id + 1));
    label_id->setAlignment(Qt::AlignCenter);
    label_id->setFont(font);

    label_pic = new QLabel();
    label_pic->setStyleSheet("border: 2px solid grey;");
    label_pic->setFixedSize(150, 150);

    btn_left = new QPushButton("L");
    btn_left->setFlat(1);
    btn_left->setFixedSize(30, 30);

    btn_right = new QPushButton("R");
    btn_right->setFlat(1);
    btn_right->setFixedSize(30, 30);

    pic_hblay = new QHBoxLayout();
    pic_hblay->setSpacing(0);
    pic_hblay->setAlignment(Qt::AlignCenter);
    pic_hblay->addWidget(btn_right);
    pic_hblay->addWidget(label_pic);
    pic_hblay->addWidget(btn_left);

    edit_name = new QLineEdit();
    edit_name->setFixedSize(250, 30);
    edit_name->setPlaceholderText(tr("Enter your name"));

    edit_hblay = new QHBoxLayout();
    edit_hblay->setAlignment(Qt::AlignCenter);
    edit_hblay->addWidget(edit_name);

    is_ready = new QCheckBox(tr("Ready!"));
    is_ready->setFont(font);
    is_ready->setEnabled(0);


    check_hblay = new QHBoxLayout();
    check_hblay->setAlignment(Qt::AlignCenter);
    check_hblay->addWidget(is_ready);

    label_choose_stats = new QLabel(tr("Distribute points") + " (" + QString::number(kol) + ") " + tr("between following attributes"));
    label_choose_stats->setFont(QFont("Arial", 10));

    label_stats_hblay = new QHBoxLayout();
    label_stats_hblay->setAlignment(Qt::AlignCenter);
    label_stats_hblay->addWidget(label_choose_stats);

    force = 0;

    label_force = new QLabel(tr("Strength"));
    label_force->setFont(QFont("Arial", 10));
    label_force->setContentsMargins(10, 0, 0, 0);

    btn_minus_force = new QPushButton("-");
    btn_minus_force->setFixedSize(22, 22);
    btn_minus_force->setFont(QFont("Arial", 11, QFont::Bold));
    btn_minus_force->setFlat(1);
    btn_minus_force->setEnabled(0);

    btn_plus_force = new QPushButton("+");
    btn_plus_force->setFont(QFont("Arial", 11, QFont::Bold));
    btn_plus_force->setFlat(1);
    btn_plus_force->setFixedSize(22, 22);

    cur_force = new QLabel("0");
    cur_force->setFixedSize(40, 20);
    cur_force->setStyleSheet("border: 1px solid grey;");
    cur_force->setAlignment(Qt::AlignCenter);

    force_hblay = new QHBoxLayout();
    force_hblay->setAlignment(Qt::AlignCenter);
    force_hblay->setSpacing(0);
    force_hblay->addWidget(btn_minus_force);
    force_hblay->addWidget(cur_force);
    force_hblay->addWidget(btn_plus_force);

    force_vblay = new QVBoxLayout();
    force_vblay->setSpacing(0);
    force_vblay->setAlignment(Qt::AlignCenter);
    force_vblay->addWidget(label_force);
    force_vblay->addLayout(force_hblay);


    agility = 0;

    label_agility = new QLabel(tr("Agility"));
    label_agility->setFont(QFont("Arial", 10));
    label_agility->setContentsMargins(10, 0, 0, 0);

    btn_minus_agility = new QPushButton("-");
    btn_minus_agility->setFixedSize(22, 22);
    btn_minus_agility->setFont(QFont("Arial", 11, QFont::Bold));
    btn_minus_agility->setFlat(1);
    btn_minus_agility->setEnabled(0);

    btn_plus_agility = new QPushButton("+");
    btn_plus_agility->setFont(QFont("Arial", 11, QFont::Bold));
    btn_plus_agility->setFlat(1);
    btn_plus_agility->setFixedSize(22, 22);

    cur_agility = new QLabel("0");
    cur_agility->setFixedSize(40, 20);
    cur_agility->setStyleSheet("border: 1px solid grey;");
    cur_agility->setAlignment(Qt::AlignCenter);

    agility_hblay = new QHBoxLayout();
    agility_hblay->setAlignment(Qt::AlignCenter);
    agility_hblay->setSpacing(0);
    agility_hblay->addWidget(btn_minus_agility);
    agility_hblay->addWidget(cur_agility);
    agility_hblay->addWidget(btn_plus_agility);

    agility_vblay = new QVBoxLayout();
    agility_vblay->setSpacing(0);
    agility_vblay->setAlignment(Qt::AlignCenter);
    agility_vblay->addWidget(label_agility);
    agility_vblay->addLayout(agility_hblay);


    intelligence = 0;

    label_intelligence = new QLabel(tr("Intelligence"));
    label_intelligence->setFont(QFont("Arial", 10));
    label_intelligence->setContentsMargins(10, 0, 0, 0);

    btn_minus_intelligence = new QPushButton("-");
    btn_minus_intelligence->setFixedSize(22, 22);
    btn_minus_intelligence->setFont(QFont("Arial", 11, QFont::Bold));
    btn_minus_intelligence->setFlat(1);
    btn_minus_intelligence->setEnabled(0);

    btn_plus_intelligence = new QPushButton("+");
    btn_plus_intelligence->setFont(QFont("Arial", 11, QFont::Bold));
    btn_plus_intelligence->setFlat(1);
    btn_plus_intelligence->setFixedSize(22, 22);

    cur_intelligence = new QLabel("0");
    cur_intelligence->setAlignment(Qt::AlignCenter);
    cur_intelligence->setFixedSize(40, 20);
    cur_intelligence->setStyleSheet("border: 1px solid grey;");

    intelligence_hblay = new QHBoxLayout();
    intelligence_hblay->setAlignment(Qt::AlignCenter);
    intelligence_hblay->setSpacing(0);
    intelligence_hblay->addWidget(btn_minus_intelligence);
    intelligence_hblay->addWidget(cur_intelligence);
    intelligence_hblay->addWidget(btn_plus_intelligence);

    intelligence_vblay = new QVBoxLayout();
    intelligence_vblay->setAlignment(Qt::AlignCenter);
    intelligence_vblay->setSpacing(0);
    intelligence_vblay->addWidget(label_intelligence);
    intelligence_vblay->addLayout(intelligence_hblay);

    stats_hblay = new QHBoxLayout();
    stats_hblay->setSpacing(10);
    stats_hblay->setAlignment(Qt::AlignJustify);
    stats_hblay->addLayout(force_vblay);
    stats_hblay->addLayout(agility_vblay);
    stats_hblay->addLayout(intelligence_vblay);


    vblay = new QVBoxLayout();
    vblay->setSpacing(10);
    vblay->setAlignment(Qt::AlignCenter);
    vblay->addWidget(label_id);
    //vblay->addLayout(pic_hblay);
    vblay->addLayout(edit_hblay);
    vblay->addLayout(label_stats_hblay);
    vblay->addLayout(stats_hblay);
    vblay->addLayout(check_hblay);

    hblay = new QHBoxLayout();
    hblay->setAlignment(Qt::AlignCenter);
    hblay->addLayout(vblay);

    setLayout(hblay);

    connect(is_ready, &QCheckBox::clicked, this, &InitialSettings::player_is_ready);
    connect(btn_minus_force, SIGNAL(clicked()), this, SLOT(force_slot()));
    connect(btn_plus_force, SIGNAL(clicked()), this, SLOT(force_slot()));
    connect(btn_minus_agility, SIGNAL(clicked()), this, SLOT(agility_slot()));
    connect(btn_plus_agility, SIGNAL(clicked()), this, SLOT(agility_slot()));
    connect(btn_minus_intelligence, SIGNAL(clicked()), this, SLOT(intelligence_slot()));
    connect(btn_plus_intelligence, SIGNAL(clicked()), this, SLOT(intelligence_slot()));
}

void InitialSettings::clear()
{
    edit_name->setText("");
    is_ready->setChecked(0);
}

void InitialSettings::disenable()
{
    is_ready->setEnabled(1);
    btn_plus_force->setEnabled(0);
    btn_plus_agility->setEnabled(0);
    btn_plus_intelligence->setEnabled(0);
}

void InitialSettings::enable()
{
    is_ready->setEnabled(0);
    btn_plus_force->setEnabled(1);
    btn_plus_agility->setEnabled(1);
    btn_plus_intelligence->setEnabled(1);
}

void InitialSettings::player_is_ready()
{
    if (is_ready->isChecked())
    {
        btn_minus_force->setEnabled(0);
        btn_minus_agility->setEnabled(0);
        btn_minus_intelligence->setEnabled(0);
        if (!edit_name->text().isEmpty())
            name = edit_name->text().toStdString();
        else
        {
            name = tr("Player").toStdString() + std::to_string(id + 1);
            edit_name->setPlaceholderText(tr("Player") + QString::number(id + 1));
        }
    }
    else
    {
        if (force)
            btn_minus_force->setEnabled(1);
        if (agility)
            btn_minus_agility->setEnabled(1);
        if (intelligence)
            btn_minus_intelligence->setEnabled(1);
        if (name == tr("Player").toStdString() + std::to_string(id + 1))
            edit_name->setPlaceholderText(tr("Enter your name"));
    }
    btn_left->setEnabled(!btn_left->isEnabled());
    btn_right->setEnabled(!btn_right->isEnabled());
    edit_name->setEnabled(!edit_name->isEnabled());
    emit ready(id, is_ready->isChecked(), name, std::to_string(force) +
               std::to_string(agility) + std::to_string(intelligence));
}

void InitialSettings::force_slot()
{
    if (sender() == btn_minus_force)
    {
        enable();
        ++kol;
        --force;
        if (kol == 1)
            btn_plus_force->setEnabled(1);
        if (!force)
            btn_minus_force->setEnabled(0);
    }
    else
    {
        btn_minus_force->setEnabled(1);
        --kol;
        ++force;
        if (!kol)
            disenable();
    }
    label_choose_stats->setText(tr("Distribute points") + " (" + QString::number(kol) + ") " + tr("between following attributes"));
    cur_force->setText(QString::number(force));
}

void InitialSettings::agility_slot()
{
    if (sender() == btn_minus_agility)
    {
        enable();
        ++kol;
        --agility;
        if (kol == 1)
            btn_plus_agility->setEnabled(1);
        if (!agility)
            btn_minus_agility->setEnabled(0);
    }
    else
    {
        btn_minus_agility->setEnabled(1);
        --kol;
        ++agility;
        if (!kol)
            disenable();
    }
    label_choose_stats->setText(tr("Distribute points") + " (" + QString::number(kol) + ") " + tr("between following attributes"));
    cur_agility->setText(QString::number(agility));
}

void InitialSettings::intelligence_slot()
{
    if (sender() == btn_minus_intelligence)
    {
        enable();
        ++kol;
        --intelligence;
        btn_plus_intelligence->setEnabled(1);
        if (!intelligence)
            btn_minus_intelligence->setEnabled(0);
    }
    else
    {
        btn_minus_intelligence->setEnabled(1);
        --kol;
        ++intelligence;
        if (!kol)
            disenable();
    }
    label_choose_stats->setText(tr("Distribute points") + " (" + QString::number(kol) + ") " + tr("between following attributes"));
    cur_intelligence->setText(QString::number(intelligence));
}
