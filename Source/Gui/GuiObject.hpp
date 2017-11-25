/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#pragma once

#include "../PluginProcessor.h"
#include "../Pd/PdObject.h"

// ==================================================================================== //
//                                      GUI OBJECT                                      //
// ==================================================================================== //

class GuiObject : public virtual Component, private Timer
{
public:
    GuiObject(pd::Gui& g);
    ~GuiObject() {}
    
protected:
    float getValueOriginal() const noexcept;
    void setValueOriginal(float v);
    
    float getValueScaled() const noexcept;
    void setValueScaled(float v);
    
    void startEdition() noexcept;
    void stopEdition() noexcept;
    
    void paint(Graphics& g) final;
    void mouseDown(const MouseEvent& event) final;
    void mouseDrag(const MouseEvent& event) final;
    void mouseUp(const MouseEvent& event) final;
private:
    void timerCallback() final;
    
    typedef void (*method_paint)(GuiObject& x, Graphics& g);
    typedef void (*method_mouse)(GuiObject& x, const MouseEvent& g);
    
    pd::Gui gui;
    bool  edited = false;
    float value  = 0;
    method_paint metpaint = nullptr;
    method_mouse metmousedown = nullptr;
    method_mouse metmousedrag = nullptr;
    method_mouse metmouseup = nullptr;
    
    static void paintToggle(GuiObject& x, Graphics& g);
    static void mouseDownToggle(GuiObject& x, const MouseEvent& g);
    
    static void mouseUpSlider(GuiObject& x, const MouseEvent& e);
    
    static void paintSliderHorizontal(GuiObject& x, Graphics& g);
    static void mouseDownSliderHorizontal(GuiObject& x, const MouseEvent& e);
    static void mouseDragSliderHorizontal(GuiObject& x, const MouseEvent& e);
    
    static void paintSliderVertical(GuiObject& x, Graphics& g);
    static void mouseDownSliderVertical(GuiObject& x, const MouseEvent& e);
    static void mouseDragSliderVertical(GuiObject& x, const MouseEvent& e);
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuiObject)
};

