
#ifndef ADVANCED_CHARS_TAB_H
#define ADVANCED_CHARS_TAB_H


#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMap>


class Advanced_chars_tab : public QWidget
{
    Q_OBJECT

    QHBoxLayout* main_layout;
    QVBoxLayout* keys;
    QVBoxLayout* values;

public:
    Advanced_chars_tab(QWidget *parent = nullptr);
    Advanced_chars_tab(QWidget *parent, std::map<QString, int>& stats);
};

#endif // ADVANCED_CHARS_TAB_H
