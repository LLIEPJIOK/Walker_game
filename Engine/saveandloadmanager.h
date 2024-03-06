#ifndef SAVEANDLOADMANAGER_H
#define SAVEANDLOADMANAGER_H

//класс для контроля загрузки и сохранения
class SaveAndLoadManager
{
private:
    //переменная для пути к файлу сохранения или загрузки
    QString file_name;
public:
    //конструктор
    SaveAndLoadManager(QString file_name_);
    //метод для изменения переменоой file_name
    void set_file_name(QString file_name_);

    void save_all();
    void load_all();

};

#endif // SAVEANDLOADMANAGER_H
