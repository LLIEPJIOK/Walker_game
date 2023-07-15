#ifndef SAVEANDLOADMANAGER_H
#define SAVEANDLOADMANAGER_H

#include <string>

//класс для контроля загрузки и сохранения
class SaveAndLoadManager
{
private:
    //переменная для пути к файлу сохранения или загрузки
    std::string file_name;
public:
    //конструктор
    SaveAndLoadManager(std::string file_name_);
    //метод для изменения переменоой file_name
    void set_file_name(std::string file_name_);

    void save_all();
    void load_all();

};

#endif // SAVEANDLOADMANAGER_H
