#ifndef EXITWINDOW_H
#define EXITWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QApplication>

class ExitWindow : public QWidget
{
    Q_OBJECT
public:
    ExitWindow(QWidget *parent = nullptr);
private:
    QPushButton *btn_yes;
    QPushButton *btn_no;
    QLabel *label;
    QVBoxLayout *vblay;
    QHBoxLayout *hblay;
    QGridLayout *gridlay;
private slots:
    void close_slot();
    void return_slot();
signals:
    void signal_open_menu();
};

#endif // EXITWINDOW_H
