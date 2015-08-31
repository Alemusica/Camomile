/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef __CAMOMILE_PD_WIDGET__
#define __CAMOMILE_PD_WIDGET__

#include "PdInstance.h"

namespace pd
{
    // ==================================================================================== //
    //                                      TEXT EDITOR                                     //
    // ==================================================================================== //
    
    class TextEditor
    {
    private:
        friend class Gui;
        t_etexteditor* m_editor;
    public:
        
        inline TextEditor(std::string const& name) noexcept : m_editor(etexteditor_getfromsymbol(gensym(name.c_str()))) {}
        inline TextEditor(TextEditor const& other) noexcept : m_editor(other.m_editor) {}
        inline operator bool() const noexcept {return bool(m_editor);}
        
        inline std::string getName() const noexcept {return m_editor->c_name->s_name;}
        inline bool isEmpty() const noexcept {return !m_editor->c_text || !m_editor->c_size;}
        inline std::string getText() const noexcept
        {return (isEmpty()) ? (std::string()) : (std::string(m_editor->c_text));}
        inline std::array<int,2> getPosition() const noexcept {return std::array<int,2>({getX(), getY()});}
        inline int getX() const noexcept {return int(m_editor->c_bounds.x);}
        inline int getY() const noexcept {return int(m_editor->c_bounds.y);}
        inline std::array<int,2> getSize() const noexcept {return std::array<int,2>({getWidth(), getHeight()});}
        inline int getWidth() const noexcept {return int(m_editor->c_bounds.width);}
        inline int getHeight() const noexcept {return int(m_editor->c_bounds.height);}
        inline std::array<int,4> getBounds() const noexcept
        {return std::array<int,4>({getX(),getY(),getWidth(),getHeight()});}
        inline bool shouldWrap() const noexcept {return m_editor->c_wrap;}
        inline std::array<float,4> getBackgroundColor() const noexcept
        {return std::array<float,4>({m_editor->c_bgcolor.red, m_editor->c_bgcolor.green, m_editor->c_bgcolor.blue, m_editor->c_bgcolor.alpha});}
        inline std::array<float,4> getTextColor() const noexcept
        {return std::array<float,4>({m_editor->c_txtcolor.red, m_editor->c_txtcolor.green, m_editor->c_txtcolor.blue, m_editor->c_txtcolor.alpha});}
        inline float getFontSize() const noexcept {return m_editor->c_font.c_size;}
        
        inline void setText(std::string const& text) const noexcept {etexteditor_settext(m_editor, text.c_str());}
    };
    
    // ==================================================================================== //
    //                                      POPUP MENU                                      //
    // ==================================================================================== //
    
    class PopupMenu
    {
    private:
        friend class Gui;
        t_epopup* m_popup;
    public:
        
        inline PopupMenu(std::string const& name) noexcept : m_popup(epopupmenu_getfromsymbol(gensym(name.c_str()))) {}
        inline PopupMenu(PopupMenu const& other) noexcept : m_popup(other.m_popup) {}
        inline operator bool() const noexcept {return bool(m_popup);}
        
        inline std::string getName() const noexcept {return m_popup->c_name->s_name;}
        inline bool isEmpty() const noexcept {return !m_popup->c_items || !m_popup->c_size;}
        inline int getNumberOfItems() const noexcept {return m_popup->c_size;}
        inline int getItemId(int index) const noexcept {return m_popup->c_items[index].c_id;}
        inline std::string getItemLabel(int index) const noexcept {return std::string(m_popup->c_items[index].c_label);}
        inline bool isItemChecked(int index) const noexcept {return int(m_popup->c_items[index].c_checked);}
        inline bool isItemDisable(int index) const noexcept {return int(m_popup->c_items[index].c_disable);}
        inline bool isItemSeparator(int index) const noexcept {return int(m_popup->c_items[index].c_separator);}
        
        inline std::array<int,2> getPosition() const noexcept {return std::array<int,2>({getX(), getY()});}
        inline int getX() const noexcept {return int(m_popup->c_bounds.x);}
        inline int getY() const noexcept {return int(m_popup->c_bounds.y);}
        inline std::array<int,2> getSize() const noexcept {return std::array<int,2>({getWidth(), getHeight()});}
        inline int getWidth() const noexcept {return int(m_popup->c_bounds.width);}
        inline int getHeight() const noexcept {return int(m_popup->c_bounds.height);}
        inline std::array<int,4> getBounds() const noexcept
        {return std::array<int,4>({getX(),getY(),getWidth(),getHeight()});}
        
        inline std::array<float,4> getBackgroundColor() const noexcept
        {return std::array<float,4>({m_popup->c_bgcolor.red, m_popup->c_bgcolor.green, m_popup->c_bgcolor.blue, m_popup->c_bgcolor.alpha});}
        inline std::array<float,4> getTextColor() const noexcept
        {return std::array<float,4>({m_popup->c_txtcolor.red, m_popup->c_txtcolor.green, m_popup->c_txtcolor.blue, m_popup->c_txtcolor.alpha});}
        inline float getFontSize() const noexcept {return m_popup->c_font.c_size;}
    };
    
    // ==================================================================================== //
    //                                      PARAMETER                                       //
    // ==================================================================================== //
    
    //! @brief The plugin Parameter class.
    //! @details The Parameter is a wrapper for the cream's t_eparam.
    class Parameter
    {
    private:
        Instance    m_instance;
        t_eparam*   m_parameter;
    public:
        //! @brief The default constructor for an empty Parameter.
        //! @details Creates an empty Parameter.
        Parameter() noexcept;
        
        //! @brief The constructor for a new Parameter.
        //! @details Creates new valid Parameter. You should never have to use it. Use the
        //! @details Gui to retrieve a Parameter.
        Parameter(Gui const& gui, const std::string& name) noexcept;
        
        //! @brief The copy constructor.
        //! @details Creates a copy of another Parameter.
        Parameter(Parameter const& other) noexcept;
        
        //! @brief The destructor.
        //! @details Does not do anything.
        ~Parameter();
        
        //! @brief The copy operator.
        //! @details Copies the Parameter and increments his counter.
        Parameter& operator=(Parameter const& other);
        
        //! @brief Gets if the Parameter is valid.
        inline operator bool() const noexcept {return bool(m_parameter);}
        
        //! @brief Gets the name of the Parameter.
        std::string getName() const;
        
        //! @brief Gets the label of the Parameter.
        std::string getLabel() const;
        
        //! @brief Gets the Parameter is automatable.
        bool isAutomatable() const;
        
        //! @brief Gets the Parameter controls other  Parameter.
        bool isMetaParameter() const;
        
        //! @brief Gets the step between two states of the  Parameter.
        float getStep() const;
        
        //! @brief Gets the normalized step between two states of the  Parameter.
        float getNormalizedStep() const;
        
        //! @brief Gets the number of steps of the  Parameter.
        size_t getNumberOfStep() const;
        
        //! @brief Gets id the boundaries of Parameter are inverted.
        bool isInverted() const;
        
        //! @brief Gets the current value of the Parameter.
        float getValue() const;
        
        //! @brief Gets the current normalized value of the Parameter.
        float getNormalizedValue() const;
        
        //! @brief Sets the current value of the Parameter.
        void setValue(const float value);
        
        //! @brief Sets the current normalized value of the Parameter.
        void setNormalizedValue(const float value);
        
        //! @brief Gets the current value of the Parameter as a string.
        std::string getTextValue() const;
        
        //! @brief Sets the current value of the Parameter with a string.
        void setTextValue(const std::string& text) const;
    };
}

#endif
