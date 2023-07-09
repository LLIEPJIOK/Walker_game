#ifndef ENTERNAME_H
#define ENTERNAME_H

#include "accept.h"

#include <QString>
#include <QLineEdit>

class EnterName : public QFrame
{
    Q_OBJECT
public:
    EnterName(QString _str, QString old_name = "", QWidget *parent = nullptr);
private:
    QString str;
    QLabel *label;
    QLineEdit *edit;
    QPushButton *btn_cansel;
    QPushButton *btn_save;
    Accept *ac;
    void turn(bool is_turn_on);
signals:
    void return_name(QString name);
private slots:
    void check_text(const QString &text);
    void return_slot();
    void check_slot(bool is_yes);
};

#endif // ENTERNAME_H
