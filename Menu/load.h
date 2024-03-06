#ifndef LOAD_H
#define LOAD_H

#include "loadframe.h"
#include "entername.h"
#include "accept.h"

class Load : public QWidget
{
    Q_OBJECT

public:
    Load(QString _type, QWidget *parent = nullptr);
    ~Load();

    // сбрасывает выделение на слоте
    void reset();
    void update_lang();

private:

    // что сейчас делаем
    enum class State
    {
        CHANGE_NAME,
        ADD_NEW,
        REWRITE,
    };

    State state;

    bool is_available;

    QString type;

    QLabel *label;
    LoadFrame *lf;
    QVBoxLayout *vblay;
    QHBoxLayout *hblay;
    QHBoxLayout *btn_hblay;
    QHBoxLayout *ld_hblay;
    QPainter *painter;
    QObject* chosen;
    QPushButton *btn_prev;
    QPushButton *btn_delete;
    QPushButton *btn_add;
    QPushButton *btn_change;

    QPushButton *btn_rewrite;
    QPushButton *btn_load;

    EnterName *change_name_window;
    EnterName *enter_save_name_window;

    Accept *accept_changes_window;

    void set_available();
    void turn(bool is_turn_on);

protected:
    void paintEvent(QPaintEvent* event) override;

signals:
    void return_back_signal();
    void delete_load(QObject *ls);
    void load_game(QString file_name);
    void save_game(QString file_name);

private slots:
    void check_pressed(bool is_pressed, QObject* ls);
    void delete_window_slot();
    void load_slot();
    void open_change_slot();
    void get_name_slot(QString name);
    void delete_slot(bool is_delete);
    void add_slot();
    void rewrite_slot();
};

#endif // LOAD_H
