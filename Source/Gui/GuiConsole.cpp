/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "GuiConsole.hpp"
#include "GuiButtons.hpp"
#include "Gui.hpp"
#include <iostream>
// ==================================================================================== //
//                                      GUI CONSOLE                                     //
// ==================================================================================== //
    
GuiConsole::GuiConsole(CamomileAudioProcessor& p) : m_processor(p)
{
    m_size  = 0;
    setWantsKeyboardFocus(true);
    TableHeaderComponent* header = new TableHeaderComponent();
    header->addColumn(String("ID"), 1, 300, 300, 300, TableHeaderComponent::notResizableOrSortable, -1);
    header->setStretchToFitActive(false);
    header->setColumnVisible(1, true);
    
    m_table.setBounds(0, 0, 300, 350);
    m_table.setModel(this);
    m_table.setOutlineThickness(0);
    m_table.setWantsKeyboardFocus(true);
    m_table.setMultipleSelectionEnabled(false);
    m_table.setMouseMoveSelectsRows(false);
    m_table.setHeaderHeight(0);
    m_table.setRowHeight(Gui::getFont().getHeight() + 2);
    m_table.setColour(ListBox::ColourIds::backgroundColourId, Colours::transparentWhite);
    m_table.getViewport()->setScrollBarsShown(true, false, true, false);
    m_table.getViewport()->setScrollBarThickness(4);
    m_table.setModel(this);
    m_table.setHeader(header);
    addAndMakeVisible(m_table);
    
    /*
    m_clear_button.addListener(this);
    addAndMakeVisible(&m_clear_button);
    m_copy_button.addListener(this);
    addAndMakeVisible(&m_copy_button);
    m_level_button.addListener(this);
    addAndMakeVisible(&m_level_button);
     */
    startTimer(100);
}

GuiConsole::~GuiConsole()
{
    stopTimer();
}

void GuiConsole::buttonClicked(Button* button)
{
    /*
    if(button == &m_clear_button)
    {
        m_history.clear();
    }
    else if(button == &m_copy_button)
    {
        String text;
        for(size_t i = 0; i < m_size; i++)
        {
            Post message(m_history.get_post_to_level(i, m_level));
            if(!message.text.empty())
            {
                text += message.text + "\n";
            }
        }
        SystemClipboard::copyTextToClipboard(text);
    }
    else
    {
        juce::PopupMenu m;
        m.addItem(1, "Fatal", true, m_level == Level::fatal);
        m.addItem(2, "Error", true, m_level == Level::error);
        m.addItem(3, "Normal", true, m_level == Level::normal);
        m.addItem(4, "All", true, m_level == Level::log);
        int level = m.showAt(getScreenBounds().translated(0, -6 * (Gui::getFont().getHeight() + 2) - 4),
                             0, 0, Gui::getFont().getHeight() + 2);
        if(level && level != int(m_level)+1)
        {
            m_level = Level(level-1);
            m_size = m_history.get_number_of_posts_to_level(m_level);
            m_table.updateContent();
        }
    }
     */
}

bool GuiConsole::keyPressed(const KeyPress& key)
{
    /*
    if(key.getModifiers().isCommandDown() && key.getTextCharacter() == 'c')
    {
        SparseSet<int> selection = m_table.getSelectedRows();
        if(!selection.isEmpty())
        {
            String text;
            for(size_t i = 0; i < selection.size(); i++)
            {
                Post message(m_history.get_post_to_level(i ,m_level));
                if(!message.text.empty())
                {
                    text += message.text + "\n";
                }
            }
            SystemClipboard::copyTextToClipboard(text);
            return true;
        }
    }
     */
    return false;
}

int GuiConsole::getNumRows()
{
    return int(m_size);
}

void GuiConsole::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    
}

void GuiConsole::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    /*
    Post message(m_history.get_post_to_level(rowNumber, m_level));
    if(rowIsSelected)
    {
        g.setColour(Gui::getColorTxt());
        g.fillRect(0, 0, width, height);
    }
    g.setFont(Gui::getFont());
    if(message.type == Level::fatal)
    {
        g.setColour(Colours::red);
    }
    else if(message.type == Level::error)
    {
        g.setColour(Colours::orange);
    }
    else if(message.type == Level::normal)
    {
        g.setColour(rowIsSelected ? Gui::getColorBg() : Gui::getColorTxt());
    }
    else
    {
        g.setColour(Colours::green);
    }
    */
    g.setColour(Gui::getColorTxt().withAlpha(0.5f));
    if(rowNumber < m_processor.getNumPrints())
    {
        String const mess = String(m_processor.getPrint(rowNumber)).trimCharactersAtEnd(" \n");
        if(mess.isNotEmpty())
        {
            g.drawText(mess, 2, 0, width, height, juce::Justification::centredLeft);
        }
    }
}

void GuiConsole::timerCallback()
{
    const size_t size = m_processor.getNumPrints();
    if(m_size != size)
    {
        m_size = size;
        m_table.updateContent();
    }
}



