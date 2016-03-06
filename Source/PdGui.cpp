/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "PdGui.hpp"
#include "PdPatch.hpp"
#include "PdInstance.hpp"
#include "Pd.hpp"

extern "C"
{
#include "../ThirdParty/PureData/src/m_pd.h"
#include "../ThirdParty/PureData/src/g_canvas.h"
#include "../ThirdParty/PureData/src/s_stuff.h"
#include "../ThirdParty/PureData/src/m_imp.h"
#include "../ThirdParty/PureData/src/g_all_guis.h"
}

namespace pd
{
    // ==================================================================================== //
    //                                      OBJECT                                          //
    // ==================================================================================== //
    
    Gui::Gui() noexcept : m_ptr(nullptr), m_type(Type::Invalid), m_patch(), m_index(-1)
    {
        
    }
    
    Gui::Gui(Patch const& patch, Type type, void* ptr, int& index) noexcept :
    m_ptr(ptr), m_type(type), m_patch(patch), m_index(-1)
    {
        if(isParameter())
        {
            m_index = ++index;
        }
    }
    
    Gui::Gui(Gui const& other) noexcept :
    m_ptr(other.m_ptr), m_type(other.m_type), m_patch(other.m_patch), m_index(other.m_index)
    {
        ;
    }
    
    Gui::Gui(Gui&& other) noexcept :
    m_ptr(other.m_ptr), m_type(other.m_type), m_patch(other.m_patch), m_index(other.m_index)
    {
        other.m_ptr   = nullptr;
        other.m_type  = Type::Invalid;
        other.m_patch = Patch();
        other.m_index = -1;
    }
    
    Gui& Gui::operator=(Gui const& other) noexcept
    {
        m_ptr   = other.m_ptr;
        m_type  = other.m_type;
        m_patch = other.m_patch;
        m_index = other.m_index;
        return *this;
    }
    
    Gui& Gui::operator=(Gui&& other) noexcept
    {
        std::swap(m_ptr, other.m_ptr);
        std::swap(m_type, other.m_type);
        std::swap(m_patch, other.m_patch);
        std::swap(m_index, other.m_index);
        return *this;
    }
    
    Gui::~Gui() noexcept
    {
        m_ptr = nullptr;
        m_patch = Patch();
    }
    
    bool Gui::isValid() const noexcept
    {
        return bool(m_ptr) && m_patch.isValid();
    }
    
    Gui::Type Gui::getType() const noexcept
    {
        return m_type;
    }
    
    int Gui::getIndex() const noexcept
    {
        return m_index;
    }
    
    bool Gui::isParameter() const noexcept
    {
        return isValid() && !getName().empty() && getBindingName() != nullptr;
    }
    
    std::string Gui::getName() const
    {
        if(isValid())
        {
            t_symbol* s = reinterpret_cast<t_iemgui *>(m_ptr)->x_lab;
            if(s)
            {
                std::string name(s->s_name);
                if(!name.empty() && name != "empty")
                {
                    auto pos = name.find("_");
                    if(pos != std::string::npos)
                    {
                        name.erase(name.begin()+pos, name.end());
                    }
                    return name;
                }
            }
        }
        return std::string();
    }
    
    std::string Gui::getLabel() const
    {
        t_symbol* s = reinterpret_cast<t_iemgui *>(m_ptr)->x_lab;
        if(s)
        {
            std::string name(s->s_name);
            if(!name.empty() && name != "empty")
            {
                auto pos = name.find("_");
                if(pos != std::string::npos)
                {
                    name.erase(name.begin(), name.begin()+pos+1);
                    return name;
                }
            }
        }
        return std::string();
    }
    
    BindingName Gui::getBindingName() const
    {
        if(isValid())
        {
            if(reinterpret_cast<t_iemgui *>(m_ptr)->x_rcv != gensym("empty"))
            {
                return BindingName(reinterpret_cast<t_iemgui *>(m_ptr)->x_rcv);
            }
        }
        return BindingName(nullptr);
    }
    
    size_t Gui::getNumberOfSteps() const noexcept
    {
        if(isValid())
        {
            if(m_type == Type::HorizontalSlider)
            {
                return 0;
            }
            else if(m_type == Type::VecticalSlider)
            {
                return 0;
            }
            else if(m_type == Type::Number)
            {
                return 0;
            }
            else if(m_type == Type::Toggle)
            {
                return 2;
            }
        }
        return 0.f;
    }
    
    float Gui::getMinimum() const noexcept
    {
        if(isValid())
        {
            if(m_type == Type::HorizontalSlider)
            {
                return reinterpret_cast<t_hslider *>(m_ptr)->x_min;
            }
            else if(m_type == Type::VecticalSlider)
            {
                return reinterpret_cast<t_vslider *>(m_ptr)->x_min;
            }
            else if(m_type == Type::Number)
            {
                return reinterpret_cast<t_my_numbox *>(m_ptr)->x_min;
            }
            else if(m_type == Type::Toggle)
            {
                return 0;
            }
        }
        return 0.f;
    }
    
    float Gui::getMaximum() const noexcept
    {
        if(isValid())
        {
            if(m_type == Type::HorizontalSlider)
            {
                return reinterpret_cast<t_hslider *>(m_ptr)->x_max;
            }
            else if(m_type == Type::VecticalSlider)
            {
                return reinterpret_cast<t_vslider *>(m_ptr)->x_max;
            }
            else if(m_type == Type::Number)
            {
                return reinterpret_cast<t_my_numbox *>(m_ptr)->x_max;
            }
            else if(m_type == Type::Toggle)
            {
                return 1;
            }
        }
        return 1.f;
    }
    
    float Gui::getValue() const noexcept
    {
        if(isValid())
        {
            if(m_type == Type::HorizontalSlider)
            {
                return reinterpret_cast<t_hslider *>(m_ptr)->x_fval;
            }
            else if(m_type == Type::VecticalSlider)
            {
                return reinterpret_cast<t_vslider *>(m_ptr)->x_fval;
            }
            else if(m_type == Type::Number)
            {
                return reinterpret_cast<t_my_numbox *>(m_ptr)->x_val;
            }
            else if(m_type == Type::Toggle)
            {
                return reinterpret_cast<t_toggle *>(m_ptr)->x_on;
            }
        }
        return 0.f;
    }
    
    std::array<float, 4> Gui::getBounds() const noexcept
    {
        if(isValid())
        {
            return m_patch.getGuiBounds(*this);
        }
        return {0.f, 0.f, 0.f, 0.f};
    }
    
    std::array<float, 2> Gui::getLabelPosition() const noexcept
    {
        if(isValid())
        {
            return m_patch.getGuiLabelPosition(*this);
        }
        return {0.f, 0.f};
    }
}



