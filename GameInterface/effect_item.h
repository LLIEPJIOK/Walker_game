
#ifndef EFFECT_ITEM_H
#define EFFECT_ITEM_H

#include "Engine/Effect.h"



class Effect_item : public QListWidgetItem
{
    Effect* connected_effect;
public:
    Effect_item();
    Effect_item(Effect* effect);

    Effect* get_connected_effect() const;
};

#endif // EFFECT_ITEM_H
