#ifndef EXITWINDOW_H
#define EXITWINDOW_H



class ExitWindow : public QWidget
{
    Q_OBJECT
public:
    ExitWindow(QWidget *parent = nullptr);
    void update_lang();
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
