QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Engine/DataBase.cpp \
    Engine/Effect.cpp \
    Engine/Equipment.cpp \
    Engine/Json.cpp \
    Engine/Player.cpp \
    Engine/Turn.cpp \
    Engine/mapcell.cpp \
    Engine/saveandloadmanager.cpp \
    Events/Event.cpp \
    Events/event_window.cpp \
    GameInterface/actionwindow.cpp \
    GameInterface/congratulationwindow.cpp \
    GameInterface/equipeditems.cpp \
    GameInterface/gamemapobject.cpp \
    GameInterface/informationwindow.cpp \
    GameInterface/inventory.cpp \
    GameInterface/inventoryitem.cpp \
    GameInterface/pausemenu.cpp \
    GameInterface/playersmodel.cpp \
    GameInterface/cell.cpp \
    GameInterface/game_interface.cpp \
    GameInterface/gamemap.cpp \
    GameInterface/infocell.cpp \
    GameInterface/dragitem.cpp \
    GameInterface/dropitem.cpp \
    GameInterface/minimap.cpp \
    GameInterface/movingarea.cpp \
    Menu/accept.cpp \
    Menu/entername.cpp \
    Menu/exitwindow.cpp \
    Menu/general.cpp \
    Menu/initialsettings.cpp \
    Menu/menu.cpp \
    Menu/newgame.cpp \
    Menu/playerssettingswindow.cpp \
    Menu/credits.cpp \
    Menu/creditstext.cpp \
    Menu/load.cpp \
    Menu/loadframe.cpp \
    Menu/loadslot.cpp \
    main.cpp

HEADERS += \
    Engine/DataBase.h \
    Engine/Effect.h \
    Engine/Equipment.h \
    Engine/Json.h \
    Engine/Player.h \
    Engine/Turn.h \
    Engine/saveandloadmanager.h \
    Engine/mapcell.h \
    Events/Event.h \
    Events/event_window.h \
    GameInterface/actionwindow.h \
    GameInterface/congratulationwindow.h \
    GameInterface/equipeditems.h \
    GameInterface/gamemapobject.h \
    GameInterface/informationwindow.h \
    GameInterface/inventory.h \
    GameInterface/inventoryitem.h \
    GameInterface/pausemenu.h \
    GameInterface/playersmodel.h \
    GameInterface/cell.h \
    GameInterface/game_interface.h \
    GameInterface/gamemap.h \
    GameInterface/infocell.h \
    GameInterface/minimap.h \
    GameInterface/movingarea.h \
    GameInterface/dragitem.h \
    GameInterface/dropitem.h \
    Menu/accept.h \
    Menu/entername.h \
    Menu/exitwindow.h \
    Menu/general.h \
    Menu/initialsettings.h \
    Menu/menu.h \
    Menu/newgame.h \
    Menu/playerssettingswindow.h \
    Menu/credits.h \
    Menu/creditstext.h \
    Menu/load.h \
    Menu/loadframe.h \
    Menu/loadslot.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
