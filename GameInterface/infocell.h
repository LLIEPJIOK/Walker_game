
#ifndef INFOCELL_H
#define INFOCELL_H


#include <QFrame>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
class InfoCell : public QFrame
{
    Q_OBJECT
public:
    explicit InfoCell(QWidget *parent = nullptr, QString terrain = "Земля", QString item = "Нет", QString event = "???");
    ~InfoCell();
    void set_terrain_type(QString str);
    void set_item_name(QString str);
    void set_event_name(QString str);
private:
    QLabel *terrain_type;
    QLabel *item_name;
    QLabel *event_name;
signals:

};

#endif // INFOCELL_H
