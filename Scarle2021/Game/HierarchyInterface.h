#pragma once
#include "DrawData2D.h"
#include "GameData.h"
#include "SimpleMath.h"

class HierarchyInterface
{
public:
    virtual ~HierarchyInterface() = default;

    virtual void update(GameData* game_data, SimpleMath::Vector2& mouse_position) = 0;
    virtual void render(DrawData2D* draw_data) = 0;

    //TODO:: ADD STUFF FOR ORDERING HERE
};
