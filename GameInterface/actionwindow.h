
#ifndef ACTIONWINDOW_H
#define ACTIONWINDOW_H


#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QTimer>
class ActionWindow : public QFrame
{
    Q_OBJECT
public:  
    explicit ActionWindow(QWidget *parent = nullptr);

    QString get_text();
private:
    QLabel *info_text;
signals:
public slots:
    void set_text(QString str);
};

#endif // ACTIONWINDOW_H
