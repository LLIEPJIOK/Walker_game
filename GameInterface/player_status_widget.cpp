
#include "player_status_widget.h"
#include <QPainter>

#include "advanced_chars_tab.h"

void Player_status_widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    /*QPixmap back("C:/proga/Game/Game/Resources/Pictures/inventory.png");
    back = back.scaled(width(), height());

    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0, 0, back);
    painter.end();*/

}

Player_status_widget::Player_status_widget(QWidget *parent)
    : QWidget(parent)
{
}

Player_status_widget::Player_status_widget(QWidget *parent, Player *pl)
{
    assigned_player = pl;

    main_layout = new QVBoxLayout(this);

    overview_lo = new QHBoxLayout(this);

    QLabel* character_sheet = new QLabel(this);
    character_sheet->setText("Character Sheet of " + QString::fromStdString(assigned_player->get_name()));
    character_sheet->setAlignment(Qt::AlignHCenter);
    character_sheet->setStyleSheet("font-size: 25px;"
                                   "font-style: italic;");
    main_layout->addWidget(character_sheet);

    QLabel* general = new QLabel(this);
    general->setText("General");
    general->setAlignment(Qt::AlignHCenter);
    general->setStyleSheet("font-size: 18px;"
                           "font-style: italic;");

    main_layout->addWidget(general);

    // ov_labels
    overview_labels = new QVBoxLayout(this);
    name_label = new QLabel(this);
    name_label->setText("Name");
    name_label->setStyleSheet("font-size: 15px;"
                              "font-style: italic;");
    overview_labels->addWidget(name_label);

    ov_health_label = new QLabel(this);
    ov_health_label->setText("Current Health");
    ov_health_label->setStyleSheet("font-size: 15px;"
                                   "font-style: italic;");

    overview_labels->addWidget(ov_health_label);

    ov_atk_label = new QLabel(this);
    ov_atk_label->setText("Current Attack");
    ov_atk_label->setStyleSheet("font-size: 15px;"
                                "font-style: italic;");

    overview_labels->addWidget(ov_atk_label);


    // ov_values
    overview_present = new QVBoxLayout(this);
    name_val = new QLabel(this);
    name_val->setText(QString::fromStdString(assigned_player->get_name()));
    name_val->setAlignment(Qt::AlignHCenter);
    name_val->setStyleSheet("font-size: 15px;");

    overview_present->addWidget(name_val);

    health_bar = new QProgressBar(this);
    health_bar->setMinimum(0);
    health_bar->setMaximum(assigned_player->get_characteristics().at("MAX_HP"));
    health_bar->setValue(assigned_player->get_characteristics().at("MAX_HP") / 3);
    health_bar->setStyleSheet("QProgressBar {"
                              "background-color: pink;"
                              "color: black;"
                              "border-width: 2px;"
                              "border-color: black;"
                              "border-radius: 0px;"
                              "text-align: center; }"

                              "QProgressBar::chunk {"
                              "background-color: red; }");;
    health_bar->setFormat("%v");
    //health_bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    health_bar->setAlignment(Qt::AlignHCenter);
    overview_present->addWidget(health_bar);

    ov_atk_value = new QLabel(this);
    ov_atk_value->setText(QString::number(assigned_player->get_characteristics().at("ATK")));
    ov_atk_value->setAlignment(Qt::AlignHCenter);
    overview_present->addWidget(ov_atk_value);

    overview_lo->addLayout(overview_labels);
    overview_lo->addLayout(overview_present);
    main_layout->addLayout(overview_lo);

    QLabel* advanced_chars = new QLabel(this);
    advanced_chars->setText("Advanced Characteristics");
    advanced_chars->setStyleSheet("font-size: 18px;"
                                  "font-style: italic;");
    advanced_chars->setAlignment(Qt::AlignHCenter);
    main_layout->addWidget(advanced_chars);

    // advanced chars
    advanced = new QTabWidget(this);
    std::map<QString, int> vital;
    vital["Current Health"] = assigned_player->get_characteristics().at("HP") / 3;
    vital["Maximum Health"] = assigned_player->get_characteristics().at("MAX_HP");
    vital["Current Armour"] = assigned_player->get_characteristics().at("ARM");
    vital["Current Damage Prevention Percentage"] = assigned_player->get_characteristics().at("PIERCE_ARM");
    advanced->addTab(new Advanced_chars_tab(this, vital), "Vitality");

    std::map<QString, int> offence;
    offence["Current Attack"] = assigned_player->get_characteristics().at("ATK");
    offence["Crit Chance"] = assigned_player->get_characteristics().at("CRIT_CH");
    offence["Crit Damage Addative"] = assigned_player->get_characteristics().at("CRIT_DMG");
    offence["Piercing Damage"] = assigned_player->get_characteristics().at("PIERCE");
    advanced->addTab(new Advanced_chars_tab(this, offence), "Offence");

    std::map<QString, int> attributes;
    attributes["Intelligence"] = assigned_player->get_characteristics().at("INT");
    attributes["Strength"] = assigned_player->get_characteristics().at("STR");
    attributes["Agility"] = assigned_player->get_characteristics().at("AGIL");
    advanced->addTab(new Advanced_chars_tab(this, attributes), "Attributes");

    std::map<QString, int> rolls;
    rolls["Movement Roll Modifier"] = assigned_player->get_characteristics().at("ROLL_MOD");
    rolls["Number of Dice"] = assigned_player->get_characteristics().at("DQNT");
    rolls["Event Roll Modifier"] = assigned_player->get_characteristics().at("EVENT_ROLL_MOD");
    advanced->addTab(new Advanced_chars_tab(this, rolls), "Rolls");

    main_layout->addWidget(advanced);


    QLabel* effects_label = new QLabel(this);
    effects_label->setText("Applied Effects");
    effects_label->setStyleSheet("font-size: 18px;"
                                 "font-style: italic;");
    effects_label->setAlignment(Qt::AlignHCenter);
    main_layout->addWidget(effects_label);

    effects = new QListWidget();
    effects->addItem("Burning");
    effects->addItem("Intoxication");
    effects->addItem("Weakness");
    main_layout->addWidget(effects);

    setLayout(main_layout);
}

Player_status_widget::~Player_status_widget()
{
}

void Player_status_widget::update_all()
{
    //here goes update;
}




