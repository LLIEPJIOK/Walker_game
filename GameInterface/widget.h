
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QPixmap>
#include <QWidgetList>
#include <QTabWidget>
#include <QListWidget>>



struct Player{
    std::unordered_map<QString, int> characteristics;
    QString name;
    int id;

    Player(){
        name = "some_player";
        id = 1;
        // Итоговые хар-ки, используемые в рассчетах при атаке, ивентах и получении урона:

        // Здоровье, оно же ОЗ и макс. ОЗ
        characteristics["HP"] = 100;
        characteristics["MAX_HP"] = 100;

        // Атрибуты, влияют на прохождение ивентов, а также на ваши ОЗ, криты
        characteristics["AGIL"] = 1;
        characteristics["STR"] = 1;
        characteristics["INT"] = 1;

        // хар-ки атаки: атк, крит шанс, крит урон, расстояние атаки и прорубающий урон (сквозь ARM)
        characteristics["ATK"] = 10;
        characteristics["CRIT_CH"] = 5;
        characteristics["CRIT_DMG"] = 100;
        characteristics["RNG"] = 1;
        characteristics["PIERCE"] = 0;


        // Хар-ки, влияющие на получаемый урон: броня, физ. сопротивление
        characteristics["ARM"] =  0;
        characteristics["PIERCE_ARM"] = 0;

        // хар-ки кубиков
        characteristics["DQNT"] = 1; // кол-во кубиков (влияет только на передвижение)
        characteristics["ROLL_MOD"] = 0; // модификатор ролла при передвижении
        characteristics["EVENT_ROLL_MOD"] = 0; // модификатор ролла в ивенте

        // я не знаю что это...
        characteristics["RGN"] = 0;

        // здоровье армора, это баг, надо фиксить, а может и все равно...
        characteristics["ARM_VIT"] = 1;

        // хар-ки, участвующие в рассчете итоговых:

        // ATK:
        characteristics["ATK_FLAT"] = 10; // плоское значение атаки
        characteristics["ATK_MULTI"] = 100; // процентный модификатор атаки

        // CRIT_CH:
        characteristics["CRIT_CH_FLAT"] = 5; // обычный крит. шанс
        characteristics["CRIT_CH_AGIL"] = characteristics["AGIL"] / 5; // шанс крита, полученный за счет ловкости
        characteristics["CRIT_CH_MULTI"] = 100; // множитель крита

        // CRIT_DMG:
        characteristics["CRIT_DMG_FLAT"] = 100; // обычный мультипликатор АТК при крит. ударе
        characteristics["CRIT_DMG_INT"] = characteristics["INT"] / 5; // крит урон, полученный за счет интеллекта
        characteristics["CRIT_DMG_MULTI"] = 100; // множитель крита

        // ARM:
        characteristics["ARM_FLAT"] = 0; // плоская броня
        characteristics["ARM_MULTI"] = 100; // мультипликатор брони

        // HP
        //characteristics["HP_FLAT"] = 5; // плоское ОЗ
        //characteristics["HP_MULTI"] = 100; // мультипликатор ОЗ
        characteristics["HP_MAX_STR"] = characteristics["STR"] * 2; // макс. ОЗ, полученные за счет силы
        characteristics["HP_MAX_FLAT"] = 100;

        characteristics["HP"] = characteristics["MAX_HP"];


    }
};

class Widget : public QWidget

{
    Q_OBJECT
    Player* assigned_player;

    QVBoxLayout* main_layout;

    //overview
    QHBoxLayout* overview_lo;
    QVBoxLayout* overview_labels;
    QVBoxLayout* overview_present;

    QPixmap* pl_img;

    QLabel* name_label;
    QLabel* ov_health_label;
    QLabel* ov_atk_label;

    QLabel* name_val;
    QProgressBar* health_bar;
    QLabel* ov_atk_value;

    //advanced chars
    QTabWidget* advanced;

    //effects
    QListWidget* effects;
protected:
    void paintEvent(QPaintEvent* event);

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
public slots:
    void update_all();
};

#endif // WIDGET_H
