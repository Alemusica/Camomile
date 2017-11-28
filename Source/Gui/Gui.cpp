/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "Gui.hpp"

// ==================================================================================== //
//                                          GUI                                         //
// ==================================================================================== //

Font Gui::getFont()
{
    return Font(String("DejaVu Sans Mono"), String("Book"), 12.f);
}

int Gui::getBorderSize() noexcept
{
    return 1;
}

Colour const& Gui::getColorBg() noexcept
{
    return Colours::lightgrey;
}

Colour const& Gui::getColorBd() noexcept
{
    return Colours::darkgrey;
}

Colour const& Gui::getColorTxt() noexcept
{
    return Colours::black;
}

Colour const& Gui::getColorInv() noexcept
{
    static Colour c(Colour::fromFloatRGBA(0.f, 0.f, 0.f, 0.f));
    return c;
}





