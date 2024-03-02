QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

PRECOMPILED_HEADER = "Resources/Files/build.pch"
CONFIG += PRECOMPILED_HEADER

SOURCES += \
    Engine/DataBase.cpp \
    Engine/Transceiver.cpp \
    Engine/Effect.cpp \
    Engine/Equipment.cpp \
    Engine/Player.cpp \
    Engine/Turn.cpp \
    Engine/saveandloadmanager.cpp \
    Engine/translator.cpp \
    Events/Event.cpp \
    Events/event_window.cpp \
    GameInterface/Player_status_widget.cpp \
    GameInterface/Advanced_chars_tab.cpp \
    GameInterface/actionwindow.cpp \
    GameInterface/congratulationwindow.cpp \
    GameInterface/effect_item.cpp \
    GameInterface/general_info_widget.cpp \
    GameInterface/image_scene.cpp \
    GameInterface/informationwindow.cpp \
    GameInterface/pausemenu.cpp \
    GameInterface/game_interface.cpp \
    Inventory/dropitem.cpp \
    Inventory/equipeditems.cpp \
    Inventory/inventory_modified.cpp \
    Inventory/item.cpp \
    Map/GraphMap.cpp \
    Map/Hex.cpp \
    Map/HexMap.cpp \
    Map/hexmodel.cpp \
    Map/mapcell.cpp \
    Map/mapobject.cpp \
    Map/minimap.cpp \
    Map/movingarea.cpp \
    Map/playersmodel.cpp \
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
    Menu/joingamewidget.cpp \
    main.cpp

HEADERS += \
    Engine/DataBase.h \
    Engine/Transceiver.h \
    Engine/Effect.h \
    Engine/Equipment.h \
    Engine/Player.h \
    Engine/Turn.h \
    Engine/saveandloadmanager.h \
    Engine/translator.h \
    Events/Event.h \
    Events/event_window.h \
    GameInterface/Player_status_widget.h \
    GameInterface/Advanced_chars_tab.h \
    GameInterface/actionwindow.h \
    GameInterface/congratulationwindow.h \
    GameInterface/effect_item.h \
    GameInterface/general_info_widget.h \
    GameInterface/image_scene.h \
    GameInterface/informationwindow.h \
    GameInterface/pausemenu.h \
    GameInterface/game_interface.h \
    Inventory/dropitem.h \
    Inventory/equipeditems.h \
    Inventory/inventory_modified.h \
    Inventory/item.h \
    Map/GraphMap.h \
    Map/Hex.h \
    Map/HexMap.h \
    Map/hexmodel.h \
    Map/mapcell.h \
    Map/mapobject.h \
    Map/minimap.h \
    Map/movingarea.h \
    Map/playersmodel.h \
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
    Menu/loadslot.h\
    Menu/joingamewidget.h \

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/Resources.qrc

TRANSLATIONS += Game_en.ts \
                Game_ru.ts
