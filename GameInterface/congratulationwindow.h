
#ifndef CONGRATULATIONWINDOW_H
#define CONGRATULATIONWINDOW_H


class CongratulationWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CongratulationWindow(QWidget *parent = nullptr, QString winner = "");
private:
    QVBoxLayout* layout;
    QPushButton* exit_button;
    QLabel* winner;
protected:
    void paintEvent(QPaintEvent* event);
signals:
    void exit_the_game();
};

#endif // CONGRATULATIONWINDOW_H
