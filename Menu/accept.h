#ifndef ACCEPT_H
#define ACCEPT_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QApplication>
#include <QScreen>
#include <QPainter>

class Accept : public QFrame
{
    Q_OBJECT
public:
    Accept(QString str, QWidget *parent = nullptr);
private:
    QLabel *label;
    QPushButton *btn_no;
    QPushButton *btn_yes;
protected:
    void paintEvent(QPaintEvent* event) override;
private slots:
    void return_slot();
signals:
    void accept_signal(bool is_delete);
};

#endif // ACCEPT_H
