
#ifndef ADVANCED_CHARS_TAB_H
#define ADVANCED_CHARS_TAB_H


#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMap>
#include <QVector>
#include "Engine/Player.h"


class Advanced_chars_tab : public QWidget
{
    Q_OBJECT

    QHBoxLayout* main_layout;
    QVBoxLayout* keys_lo;
    QVBoxLayout* values_lo;

    std::map<QString, QLabel*> chars;
    std::map<QString, QString> stat_names;
protected:
public:
    Advanced_chars_tab(QWidget *parent = nullptr);
    Advanced_chars_tab(QWidget *parent, std::map<QString, QString>& _stat_names, Player* assigned_player);
    void update_chars(Player* assigned_player);
};

#endif // ADVANCED_CHARS_TAB_H
