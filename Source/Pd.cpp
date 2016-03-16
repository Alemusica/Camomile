/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "Pd.hpp"
#include "PdInstance.hpp"
#include <signal.h>

extern "C"
{
#include "../ThirdParty/PureData/src/m_pd.h"
#include "../ThirdParty/PureData/src/g_canvas.h"
#include "../ThirdParty/PureData/src/s_stuff.h"
#include "../ThirdParty/PureData/src/m_imp.h"
EXTERN void pd_init(void);
    
void bob_tilde_setup();
void bonk_tilde_setup();
void choice_setup();
void expr_setup();
void fiddle_tilde_setup();
void loop_tilde_setup();
void lrshift_tilde_setup();
void pique_setup();
void sigmund_tilde_setup();
void stdout_setup();
}

namespace pd
{
    // ==================================================================================== //
    //                                      PD                                              //
    // ==================================================================================== //
    
    Pd::Pd() noexcept
    {
        signal(SIGFPE, SIG_IGN);
        sys_printhook = NULL;
        sys_soundin = NULL;
        sys_soundout = NULL;
        sys_schedblocksize = DEFDACBLKSIZE;
        sys_externalschedlib = 0;
        sys_printtostderr = 0;
        sys_usestdpath = 0;
        sys_debuglevel = 1;
        sys_verbose = DEBUG;
        sys_noloadbang = 0;
        sys_nogui = 1;
        sys_hipriority = 0;
        sys_nmidiin = 0;
        sys_nmidiout = 0;
        sys_init_fdpoll();
#ifdef HAVE_SCHED_TICK_ARG
        sys_time = 0;
#endif
        pd_init();
        sys_set_audio_api(API_DUMMY);
        sys_searchpath = NULL;
        
        int indev[MAXAUDIOINDEV], inch[MAXAUDIOINDEV],
        outdev[MAXAUDIOOUTDEV], outch[MAXAUDIOOUTDEV];
        indev[0] = outdev[0] = DEFAULTAUDIODEV;
        inch[0] = m_max_channels;
        outch[0] = m_max_channels;
        sys_set_audio_settings(1, indev, 1, inch,
                               1, outdev, 1, outch, 44100, -1, 1, DEFDACBLKSIZE);
        sched_set_using_audio(SCHED_AUDIO_CALLBACK);
        sys_reopen_audio();
        m_sample_ins    = sys_soundin;
        m_sample_outs   = sys_soundout;        
        sys_soundin     = nullptr;
        sys_soundout    = nullptr;
    }
    
    Pd::~Pd() noexcept
    {
        if(m_sample_ins)
        {
            freebytes(m_sample_ins, (sys_inchannels ? sys_inchannels : 2) * (DEFDACBLKSIZE*sizeof(t_sample)));
        }
        if(m_sample_outs)
        {
            freebytes(m_sample_outs, (sys_outchannels ? sys_outchannels : 2) * (DEFDACBLKSIZE*sizeof(t_sample)));
        }
    }
    
    Pd& Pd::get() noexcept
    {
        static Pd pd;
        static bool initialized = false;
        if(!initialized)
        {
            bob_tilde_setup();
            bonk_tilde_setup();
            choice_setup();
            expr_setup();
            fiddle_tilde_setup();
            loop_tilde_setup();
            lrshift_tilde_setup();
            pique_setup();
            sigmund_tilde_setup();
            stdout_setup();
            pd.m_console.clear();
            sys_printhook = reinterpret_cast<t_printhook>(print);
            initialized = true;
        }
        return pd;
    }
    
    void Pd::print(const char* s)
    {
        Pd& pd = Pd::get();
        std::string message(s);
        if(message.compare(0, 5, "error"))
        {
            std::lock_guard<std::mutex> guard(pd.m_console_mutex);
            pd.m_console.push_back({message, Post::Type::Error});
        }
        else if(message.compare(0, 7, "verbose"))
        {
            std::lock_guard<std::mutex> guard(pd.m_console_mutex);
            pd.m_console.push_back({message, Post::Type::Log});
        }
        else
        {
            std::lock_guard<std::mutex> guard(pd.m_console_mutex);
            pd.m_console.push_back({message, Post::Type::Post});
        }
#ifdef DEBUG
        std::cout << s;
#endif
    }
    
    std::string Pd::getPdVersion()
    {
        return std::to_string(PD_MAJOR_VERSION) + "." +
        std::to_string(PD_MINOR_VERSION) + "." +
        std::to_string(PD_BUGFIX_VERSION);
    }
    
    void Pd::addToSearchPath(std::string const& path) noexcept
    {
        Pd& pd = Pd::get();
        std::lock_guard<std::mutex> guard(pd.m_mutex);
        sys_searchpath = namelist_append(sys_searchpath, path.c_str(), 0);
    }
    
    void Pd::clearSearchPath() noexcept
    {
        Pd& pd = Pd::get();
        std::lock_guard<std::mutex> guard(pd.m_mutex);
        namelist_free(sys_searchpath);
        sys_searchpath = NULL;
    }
    
    void Pd::clearConsole() noexcept
    {
        Pd& pd = Pd::get();
        std::lock_guard<std::mutex> guard(pd.m_console_mutex);
        bool state = !pd.m_console.empty();
        pd.m_console.clear();
        pd.m_console_changed = state;
    }
    
    void Pd::postToConsole(std::string const& text) noexcept
    {
        Pd& pd = Pd::get();
        std::lock_guard<std::mutex> guard(pd.m_console_mutex);
        pd.m_console.push_back({text + "\n", Post::Type::Post});
        pd.m_console_changed = true;
    }
    
    void Pd::logToConsole(std::string const& text) noexcept
    {
        Pd& pd = Pd::get();
        std::lock_guard<std::mutex> guard(pd.m_console_mutex);
        pd.m_console.push_back({text + "\n", Post::Type::Log});
        pd.m_console_changed = true;
    }
    
    void Pd::errorToConsole(std::string const& text) noexcept
    {
        Pd& pd = Pd::get();
        std::lock_guard<std::mutex> guard(pd.m_console_mutex);
        pd.m_console.push_back({text + "\n", Post::Type::Error});
        pd.m_console_changed = true;
    }
    
    std::vector<Post> Pd::getConsole(bool state) noexcept
    {
        Pd& pd = Pd::get();
        std::lock_guard<std::mutex> guard(pd.m_console_mutex);
        pd.m_console_changed = state;
        return pd.m_console;
    }
    
    bool Pd::hasConsoleChanged() noexcept
    {
        Pd& pd = Pd::get();
        std::lock_guard<std::mutex> guard(pd.m_console_mutex);
        return pd.m_console_changed;
    }
    
    void Pd::lock() noexcept
    {
        Pd& pd = Pd::get();
        pd.m_mutex.lock();
    }
    
    void Pd::unlock() noexcept
    {
        Pd& pd = Pd::get();
        pd.m_mutex.unlock();
    }
    
    Instance Pd::createInstance() noexcept
    {
        Pd& pd = Pd::get();
        pd.m_mutex.lock();
        Instance instance(pdinstance_new());
        pd.m_mutex.unlock();
        return instance;
    }
    
    void Pd::free(Instance& instance)
    {
        Pd& pd = Pd::get();
        std::lock_guard<std::mutex> guard(pd.m_mutex);
        pdinstance_free(reinterpret_cast<t_pdinstance *>(instance.m_ptr));
    }
}



