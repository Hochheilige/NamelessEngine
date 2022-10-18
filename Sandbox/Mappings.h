#pragma once
#include "Transform.h"
#include <iostream>

/*
 * Создать лоадер и проверяльщик, что все методы валидны
 * Надо будет разнести байндинги по различным классам, для различных объектов
 */
class Mappings
{
public:
    //TODO move to nested classes
    static void CS_CreateObj(float trans_x, float trans_y, float trans_z,
        float rot_x, float rot_y, float rot_z,
        float scale_x, float scale_y, float scale_z);
    static void CS_AudioOnCreate();
};

