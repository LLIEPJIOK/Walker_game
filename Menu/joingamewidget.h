
#ifndef JOINGAMEWIDGET_H
#define JOINGAMEWIDGET_H


#include <QObject>
#include <QWidget>


class JoinGameWidget : public QWidget
{
    Q_OBJECT
    QLabel* label;
    QLineEdit* field;
    QPushButton* cont_btn;
    QPushButton* cancel_btn;
private slots:
    void join();
public:
    JoinGameWidget();
    JoinGameWidget(QWidget* parent);

signals:
    void joinTo(std::string ip);
    void cancel();
};

#endif // JOINGAMEWIDGET_H
