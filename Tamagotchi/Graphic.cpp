#include "Graphic.h"

Graphic::Graphic(const std::vector<unsigned char>& graphic_, unsigned char graphicWidth_, unsigned char graphicHeight_)
    : PIXELS(graphic_), WIDTH(graphicWidth_), HEIGHT(graphicHeight_)
{
}