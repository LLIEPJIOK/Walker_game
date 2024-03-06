#ifndef INFORMATIONWINDOW_H
#define INFORMATIONWINDOW_H

class InformationWindow : public QFrame
{
    Q_OBJECT
public:
    InformationWindow(QWidget *parent = nullptr);
    void inform(QString str);

private:
    int opacity;

    QTimer* timer;

    QLabel* label;

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void timer_tick();
};

#endif // INFORMATIONWINDOW_H
