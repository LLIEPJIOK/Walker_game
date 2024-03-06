#include "saveandloadmanager.h"
#include "Turn.h"
#include "DataBase.h"

SaveAndLoadManager::SaveAndLoadManager(QString file_name_)
{
    //присваивание значения переменной file_name
    file_name = file_name_;
}

void SaveAndLoadManager::set_file_name(QString file_name_)
{
    //присваивание значения переменной file_name
    file_name = file_name_;
}

void SaveAndLoadManager::save_all()
{
    //открытие файлового потока для записи
    QFile in("../Game/Saves/" + file_name + ".bin");
    if (!in.open(QIODevice::WriteOnly))
    {
        qDebug() << "file \"../Game/Saves/" + file_name + ".bin\" did't open";
        return;
    }

    //вызов метода записи у объекта класса DataBase
    DataBase::get_DataBase()->save(in);
    //вызов метода записи у объекта класса Turn
    Turn::get_Turn()->save(in);
    //закрытие потока для записи
    in.close();
}

void SaveAndLoadManager::load_all()
{
    //открытие файлового потока для чтения
    QFile out("../Game/Saves/" + file_name + ".bin");
    if (!out.open(QIODevice::ReadOnly))
    {
        qDebug() << "file \"../Game/Saves/" + file_name + ".bin\" did't open";
        return;
    }

    //вызов метода чтения у объекта класса DataBase
    DataBase::get_DataBase()->load(out);
    //вызов метода чтения у объекта класса Turn
    Turn::get_Turn()->load(out);
    //закрытие потока для чтения
    out.close();
}
