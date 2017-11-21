/*
 // Copyright (c) 2015-2017 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#pragma once

#include <string>
#include <utility>
#include <vector>

//////////////////////////////////////////////////////////////////////////////////////////////
//                                      ENVIRONMENT                                         //
//////////////////////////////////////////////////////////////////////////////////////////////

//! @brief The class manages the global environmment for Camomile
class CamomileEnvironment
{
public:
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //                                      GLOBAL                                          //
    //////////////////////////////////////////////////////////////////////////////////////////
    
    //! @brief Initialize the environment (only if you want to do it in advance).
    static bool initialize();
    
    //! @brief Gets the name used by the plugin.
    static const char* getPluginNameUTF8();
    
    //! @brief Gets the name used by the plugin.
    static std::string getPluginName();
    
    //! @brief Gets the ID used by the plugin.
    static unsigned int getPluginID();
    
    //! @brief Gets the path to the Pd patch.
    static std::string getPatchPath();
    
    //! @brief Gets the name to the Pd patch.
    static std::string getPatchName();
    
    //! @brief Gets if the environment is valid.
    static bool isValid();
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //                                      OPTIONS                                         //
    //////////////////////////////////////////////////////////////////////////////////////////
    
    //! @brief Gets if the patch wants MIDI events.
    static bool wantsMidi();
    
    //! @brief Gets if the patch produces MIDI events.
    static bool producesMidi();
    
    //! @brief Gets if the patch wants play head information.
    static bool wantsPlayHead();
    
    //! @brief Gets if the patch is valid (no audio).
    static bool isMidiOnly();
    
    //! @brief Gets the tail length in seconds.
    static float getTailLengthSeconds();
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //                                      PROGRAMS                                        //
    //////////////////////////////////////////////////////////////////////////////////////////
    
    //! @brief Gets the programs.
    static std::vector<std::string> const& getPrograms();
    
private:
    static CamomileEnvironment& get();
    CamomileEnvironment();
    
    std::string     plugin_name = "Camomile";
    std::string     plugin_path = "";
    unsigned int    plugin_id   = 0x4b707139;
    std::string     patch_name  = "Camomile.pd";
    std::string     patch_path  = "";
    bool            valid       = false;
    
    
    bool    midi_in_support   = false;
    bool    midi_out_support  = false;
    bool    play_head_support = false;
    bool    midi_only         = false;
    float   tail_length_sec   = 0.f;
    
    std::vector<std::string> programs;
};
