#include "saveandloadmanager.h"
#include "Turn.h"
#include "DataBase.h"
SaveAndLoadManager::SaveAndLoadManager(std::string file_name_)
{
    //присваивание значения переменной file_name
    file_name = file_name_;
}

void SaveAndLoadManager::set_file_name(std::string file_name_)
{
    //присваивание значения переменной file_name
    file_name = file_name_;
}

void SaveAndLoadManager::save_all()
{
    //открытие файлового потока для записи
    std::ofstream out("../Game/Saves/" + file_name + ".bin", std::ios::binary);
    //вызов метода записи у объекта класса DataBase
    DataBase::get_DataBase()->save(out);
    //вызов метода записи у объекта класса Turn
    Turn::get_Turn()->save(out);
    //закрытие потока для записи
    out.close();
}

void SaveAndLoadManager::load_all()
{
    //открытие файлового потока для чтения
    std::ifstream in("../Game/Saves/" + file_name + ".bin", std::ios::binary);
    //вызов метода чтения у объекта класса DataBase
    DataBase::get_DataBase()->load(in);
    //вызов метода чтения у объекта класса Turn
    Turn::get_Turn()->load(in);
    //закрытие потока для чтения
    in.close();
}
