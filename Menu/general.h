#ifndef GENERAL_H
#define GENERAL_H

#include <QObject>
#include <QString>

class General : public QObject
{
    Q_OBJECT
public:
    ~General();
    static General* get_general();
private:
    static General* general;
    General(QObject *parent = nullptr);
    General(const General&) = delete;
    const General& operator = (const General&) = delete;
signals:
    void start_game(std::vector<std::pair<std::string, std::string>>);
    void load_game(QString file_name);
};

#endif // GENERAL_H
