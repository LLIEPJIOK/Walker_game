
#include "player_status_widget.h"
#include <QPainter>

#include "Engine/Effect.h"
#include "advanced_chars_tab.h"

void Player_status_widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPixmap back("D:/Game/Game/Resources/Pictures/status.png");
    back = back.scaled(width(), height());

    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0, 0, this->width(), this->height(), back);
    QWidget::paintEvent(event);

}




Player_status_widget::Player_status_widget(QWidget *parent)
    : QWidget(parent)
{
}

Player_status_widget::Player_status_widget(QWidget *parent, Player *pl) : QWidget(parent)
{
    this->setGeometry(parent->width() / 2 - 350, parent->height() / 2 - 450, 350, 450);
    this->setFixedSize(350, 450);

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
                              "border: 1px solid red;"
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
    std::map<QString, QString> vital;
    vital["HP"] = "Current Health";
    vital["MAX_HP"] = "Maximum Health";
    vital["ARM"] = "Current Armour";
    vital["PIERCE_ARM"] = "Current Damage Prevention Percentage";
    advanced->addTab(new Advanced_chars_tab(this, vital, assigned_player), "Vitality");

    std::map<QString, QString> offence;
    offence["ATK"] = "Current Attack";
    offence["CRIT_CH"] = "Crit Chance";
    offence["CRIT_DMG"] = "Crit Damage Addative";
     offence["PIERCE"] = "Piercing Damage (through armour)";
    advanced->addTab(new Advanced_chars_tab(this, offence, assigned_player), "Offence");

    std::map<QString, QString> attributes;
    attributes["INT"] = "Intelligence";
    attributes["STR"] = "Strength";
    attributes["AGIL"] = "Agility";
    advanced->addTab(new Advanced_chars_tab(this, attributes, assigned_player), "Attributes");

    std::map<QString, QString> rolls;
    rolls["ROLL_MOD"] = "Movement Roll Modifier";
    rolls["DQNT"] = "Number of Dice";
    rolls["EVENT_ROLL_MOD"] = "Event Roll Modifier";
    advanced->addTab(new Advanced_chars_tab(this, rolls, assigned_player), "Rolls");

    advanced->setStyleSheet("QTabWidget::pane {"
        "border: 1px solid black;"
        "top:-1px; "
        "background-image: url(D:/Game/Game/Resources/Pictures/status.png);"
        "}"
    "QTabBar::tab {"
        "border: 1px solid lightblack; "
        " background-image: url(D:/Game/Game/Resources/Pictures/status.png)"
        "} "
    "QTabBar::tab:selected { "
        "border: 1px solid black; "
        "margin-bottom: -1px; "
        "background-image: url(D:/Game/Game/Resources/Pictures/status.png)"
        "}");

    main_layout->addWidget(advanced);


    QLabel* effects_label = new QLabel(this);
    effects_label->setText("Applied Effects");
    effects_label->setStyleSheet("font-size: 18px;"
                                 "font-style: italic;");
    effects_label->setAlignment(Qt::AlignHCenter);
    main_layout->addWidget(effects_label);

    effects = new QListWidget();
    effects->setStyleSheet("background-image: url(D:/Game/Game/Resources/Pictures/status.png);"
                           "border: 1px solid black;");
    effects->addItem("Burning");
    effects->addItem("Intoxication");
    effects->addItem("Weakness");
    main_layout->addWidget(effects);

    setLayout(main_layout);
}

Player_status_widget::~Player_status_widget()
{
    delete pl_img;
    delete name_label;
    delete ov_health_label;
    delete ov_atk_label;

    delete name_val;
    delete health_bar;
    delete ov_atk_value;

    for (auto t : advanced->children())
        delete t;

    delete advanced;

    delete effects;
}

void Player_status_widget::update_all()
{
    // updates bar and atk label
    health_bar->setValue(assigned_player->get_characteristics().at("HP"));
    health_bar->setMaximum(assigned_player->get_characteristics().at("MAX_HP"));
    ov_atk_value->setText(QString::number(assigned_player->get_characteristics().at("ATK")));

    // updates tabs
    for (int i = 0; i < 4; i++){
        advanced->setCurrentIndex(i);
        dynamic_cast<Advanced_chars_tab*>(advanced->currentWidget())->update_chars(assigned_player);
    }

    //updates all applied effects in qwlist
    effects->clear();
    for (Effect* eff : *assigned_player->get_active_effects()){
        effects->addItem(QString::fromStdString(eff->get_effect_name()));
    }

    advanced->setCurrentIndex(0);
}




