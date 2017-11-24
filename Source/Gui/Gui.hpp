/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
/*
#include "../CamomileProcessor.hpp"
#include "GuiParameter.hpp"
#include "GuiRadio.hpp"
#include "GuiToggle.hpp"
#include "GuiSlider.hpp"
#include "GuiNumbox.hpp"
#include "GuiLabel.hpp"
#include "GuiComment.hpp"
#include "GuiPatcher.hpp"

 */
#include "GuiButtons.hpp"
#include "GuiConsole.hpp"

// ==================================================================================== //
//                                      GUI                                      //
// ==================================================================================== //

class Gui
{
public:
    static Font getFont();
    static int getBorderSize() noexcept;
    static Colour const& getColorBg() noexcept;
    static Colour const& getColorBd() noexcept;
    static Colour const& getColorTxt() noexcept;
    static Colour const& getColorInv() noexcept;
};

