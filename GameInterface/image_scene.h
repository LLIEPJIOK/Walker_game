
#ifndef IMAGE_SCENE_H
#define IMAGE_SCENE_H

#include "Inventory/item.h"
#include"effect_item.h"
#include <QWidget>
#include<QLabel>
#include<QPixmap>


class Image_scene : public QWidget
{
    Q_OBJECT

private:
    int img_size();
    QPixmap current;
    QPixmap background;
    QPixmap info;

    QListWidgetItem* prev_chosen;

    void paint_item(Item item);
    void paint_effect(Effect_item* item);
    static std::map<QString, QString> stat_names;
public:
    Image_scene();
    Image_scene(QWidget* parent);
    void reset();
public slots:
    void take_item(Item* item);
    void take_effect(QListWidgetItem* item);
protected:
    void paintEvent(QPaintEvent *event);

};

#endif // IMAGE_SCENE_H
