
#include "effect_item.h"

#include<QSize>
#include"Engine/translator.h"

Effect_item::Effect_item() : QListWidgetItem()
{
    connected_effect = nullptr;
}

Effect_item::Effect_item(Effect *effect) : QListWidgetItem()
{
    connected_effect = effect;
    this->setText(Translator::translate(effect->get_effect_name().c_str()).c_str());
    this->setSizeHint(QSize(1, 30));
}

Effect *Effect_item::get_connected_effect() const
{
    return connected_effect;
}

