#ifndef LOADFRAME_H
#define LOADFRAME_H

#include "loadslot.h"

class LoadFrame : public QMainWindow
{
    Q_OBJECT

public:
    LoadFrame(QWidget *parent = nullptr);
    ~LoadFrame();
    QString add_slot(QString name);

private:
    QList<LoadSlot*> loads;
    QVBoxLayout *vblay;
    QScrollArea *scroll;
    QWidget *widget;

signals:
    void pressed(bool is_pressed, QObject *ls);

private slots:
    void unpress(bool is_pressed);

public slots:
    void delete_load(QObject *ls);
};

#endif // LOADFRAME_H
