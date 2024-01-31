#ifndef INITIALSETTINGS_H
#define INITIALSETTINGS_H

#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <string>

class InitialSettings : public QFrame
{
    Q_OBJECT
public:
    InitialSettings(int _id, QWidget *parent = nullptr);
    void clear();
    int id;
    void update_lang();
private:
    int kol;
    QLabel *label_id;
    QLabel *label_pic;
    QLineEdit *edit_name;
    QPushButton *btn_right;
    QPushButton *btn_left;
    QVBoxLayout *vblay;
    QHBoxLayout *hblay;
    QHBoxLayout *pic_hblay;
    QHBoxLayout *edit_hblay;
    QHBoxLayout *check_hblay;

    QLabel *label_choose_stats;
    QHBoxLayout *label_stats_hblay;

    QLabel *label_force;
    QLabel *cur_force;
    QPushButton *btn_minus_force;
    QPushButton *btn_plus_force;
    QHBoxLayout *force_hblay;
    QVBoxLayout *force_vblay;
    int force;

    QLabel *label_agility;
    QLabel *cur_agility;
    QPushButton *btn_minus_agility;
    QPushButton *btn_plus_agility;
    QHBoxLayout *agility_hblay;
    QVBoxLayout *agility_vblay;
    int agility;

    QLabel *label_intelligence;
    QLabel *cur_intelligence;
    QPushButton *btn_minus_intelligence;
    QPushButton *btn_plus_intelligence;
    QHBoxLayout *intelligence_hblay;
    QVBoxLayout *intelligence_vblay;
    int intelligence;

    QHBoxLayout *stats_hblay;

    QStringList pathes;
    QCheckBox* is_ready;
    std::string name;
    //std::string players_stats;
    //player stats
    void disenable();
    void enable();
signals:
    void ready(int id, bool is_ready, const std::string& name, const std::string& stats);
private slots:
    void player_is_ready();
    void force_slot();
    void agility_slot();
    void intelligence_slot();
};

#endif // INITIALSETTINGS_H
