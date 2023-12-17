#ifndef LOADSLOT_H
#define LOADSLOT_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QMouseEvent>

class LoadSlot : public QWidget
{
    Q_OBJECT
public:
    LoadSlot(QString _file_name, QWidget *parent = nullptr);
    void unpress();
    void delete_save();
    QString get_name();
    QString get_file_name();
    void change_name(QString name);
private:
    QString file_name;
    QPainter *painter;
    QLabel *label_name;
    QLabel *label_date;
    bool is_hover;
    bool is_pressed;
protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void pressed(bool is_pressed);
};

#endif // LOADSLOT_H
