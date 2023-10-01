#pragma once

#include <vector>
#include "Position.h"

enum class Tranformation : unsigned char;

class Graphic
{
public:
    const std::vector<unsigned char> PIXELS;
    const unsigned char WIDTH;
    const unsigned char HEIGHT;

    Position screenPos;
    Tranformation transformation;

    Graphic(const std::vector<unsigned char>& graphic_, unsigned char graphicWidth_, unsigned char graphicHeight_);
};

enum class Tranformation : unsigned char
{
        Default,
        HorizontalMirror,
        VerticalMirror
};