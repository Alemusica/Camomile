/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "InstanceProcessor.h"
#include "PatchEditor.h"

class PatchEditor::AboutWindow : public DocumentWindow
{
private:
    class Content : public Component
    {
    private:
        DrawableImage m_image;
        juce::TextEditor m_text;
    public:
        Content()
        {
            m_image.setImage(ImageCache::getFromMemory(BinaryData::flowerG_png, BinaryData::flowerG_pngSize));
            m_image.setBoundingBox(RelativeParallelogram(Rectangle<float>(300.f - 128.f, 320.f - 128.f, 128.f, 128.f)));
            m_image.setOpacity(0.5f);
            addAndMakeVisible(&m_image, 0);
            
            m_text.setMultiLine(true);
            m_text.setReadOnly(true);
            m_text.setScrollbarsShown(false);
            m_text.setCaretVisible(false);
            m_text.setPopupMenuEnabled (true);
            m_text.setColour(juce::TextEditor::backgroundColourId, Colour::fromFloatRGBA(0.f, 0.f, 0.f, 0.f));
            m_text.setColour(juce::TextEditor::outlineColourId, Colour::fromFloatRGBA(0.f, 0.f, 0.f, 0.f));
            m_text.setColour(juce::TextEditor::shadowColourId,Colour::fromFloatRGBA(0.f, 0.f, 0.f, 0.f));
            m_text.setColour(juce::TextEditor::textColourId, Colour::fromFloatRGBA(1.f, 1.f, 1.f, 1.f));
            m_text.setText("Camomile is a dynamic Plugin that allows to load and control Pure Data patches "
                           "inside a digital audio workstation. Camomile translates the Cream library's user"
                           " graphical interfaces into Juce component for creating a modular plugin editor "
                           "and retrieves the parameters of this graphical objects in order to associate"
                           " them with the plugin host.\n\n"
                           "Author :\n"+ String(JucePlugin_Manufacturer) + "\n\n"
                           "Organizations :\nCICM | Université Paris 8 | Labex Arts H2H\n\n"
                           "Contacts :\n"+String(JucePlugin_ManufacturerEmail)+"\n"+ String(JucePlugin_ManufacturerWebsite)+"\n\n"
                           "Credits :\nPure Data by Miller Puckette\nJuce by ROLI Ltd.");
            m_text.setBounds(0, 0, 300, 320);
            addAndMakeVisible(&m_text, 1);
        }
    };
    Content m_content;
public:
    AboutWindow() : DocumentWindow("About Camomile v" + String(JucePlugin_VersionString), Colours::lightgrey, closeButton, false)
    {
        setUsingNativeTitleBar(true);
        setBounds(20, 20, 300, 320);
        setResizable(false, false);
        setAlwaysOnTop(true);
        setDropShadowEnabled(true);
        setVisible(true);
        setContentNonOwned(&m_content, false);
    }
    
    void closeButtonPressed() override
    {
        removeFromDesktop();
    }
};

class PatchEditor::ConsoleWindow : public DocumentWindow
{
private:
    class Content : public Component, public Messenger, public juce::TextEditor::Listener
    {
    private:
        juce::TextEditor m_text;
    public:
        Content(Instance const& instance) : Messenger(instance, "camo_console")
        {
            m_text.setMultiLine(true);
            m_text.setReadOnly(false);
            m_text.setScrollbarsShown(true);
            m_text.setCaretVisible(false);
            m_text.setPopupMenuEnabled (true);
            m_text.setColour(juce::TextEditor::backgroundColourId, Colour::fromFloatRGBA(0.f, 0.f, 0.f, 0.f));
            m_text.setColour(juce::TextEditor::outlineColourId, Colour::fromFloatRGBA(0.f, 0.f, 0.f, 0.f));
            m_text.setColour(juce::TextEditor::shadowColourId,Colour::fromFloatRGBA(0.f, 0.f, 0.f, 0.f));
            m_text.setText(Instance::getConsole());
            
            m_text.setBounds(0, 0, 300, 320);
            addAndMakeVisible(&m_text, 1);
        }
        
        void textEditorTextChanged(juce::TextEditor& ed) override
        {
            Instance::setConsole(ed.getText().toStdString());
        }
        
        void receive(Message const& message) override
        {
            m_text.setText(Instance::getConsole());
        }
    };
    Content m_content;
public:
    ConsoleWindow(Instance const& instance) : DocumentWindow("Camomile Console", Colours::lightgrey, closeButton, false),
    m_content(instance)
    {
        setUsingNativeTitleBar(true);
        setBounds(20, 20, 300, 320);
        setResizable(false, false);
        setAlwaysOnTop(true);
        setDropShadowEnabled(true);
        setVisible(true);
        setContentNonOwned(&m_content, false);
        
    }
    
    void closeButtonPressed() override
    {
        removeFromDesktop();
    }
};

// ==================================================================================== //
//                                  CAMOMILE INTERFACE                                  //
// ==================================================================================== //

PatchEditor::PatchEditor(InstanceProcessor& p) :
AudioProcessorEditor(&p),
m_processor(p),
m_dropping(false),
m_window(nullptr),
m_color_bg(Colours::lightgrey),
m_color_bd(Colours::darkgrey)
{
    m_button_infos = new DrawableButton("CamomileButton", DrawableButton::ImageStretched);
    m_button_infos->addListener(this);
    m_button_infos->setClickingTogglesState(false);
    m_button_infos->setAlwaysOnTop(true);
    m_button_infos->setRadioGroupId(1);
    m_button_infos->setBounds(3, 3, 15, 15);
    m_button_infos->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
    addAndMakeVisible(m_button_infos);
    
    TextButton* t = new TextButton("Open");
    t->addListener(this);
    t->setClickingTogglesState(false);
    t->setRadioGroupId(2);
    t->setColour(TextButton::textColourOffId, Colours::white);
    t->setBounds(22, 0, 40, 20);
    t->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
    m_buttons.add(t);
    
    t = new TextButton("Close");
    t->addListener(this);
    t->setClickingTogglesState(false);
    t->setRadioGroupId(3);
    t->setColour(TextButton::textColourOffId, Colours::white);
    t->setBounds(62, 0, 40, 20);
    t->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
    m_buttons.add(t);
    
    t = new TextButton("Reload");
    t->addListener(this);
    t->setClickingTogglesState(false);
    t->setRadioGroupId(4);
    t->setColour(TextButton::textColourOffId, Colours::white);
    t->setBounds(102, 0, 50, 20);
    t->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
    m_buttons.add(t);
    
    t = new TextButton("Console");
    t->addListener(this);
    t->setClickingTogglesState(false);
    t->setRadioGroupId(5);
    t->setColour(TextButton::textColourOffId, Colours::white);
    t->setBounds(152, 0, 55, 20);
    t->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
    m_buttons.add(t);

    t = new TextButton("Help");
    t->addListener(this);
    t->setClickingTogglesState(false);
    t->setRadioGroupId(6);
    t->setColour(TextButton::textColourOffId, Colours::white);
    t->setBounds(207, 0, 35, 20);
    t->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
    m_buttons.add(t);
    
    Component::setWantsKeyboardFocus(true);
    m_processor.addListener(this);
    setSize(600, 420);
    patchChanged();
}

PatchEditor::~PatchEditor()
{
    m_processor.removeListener(this);
}

void PatchEditor::paint(Graphics& g)
{
    g.fillAll(m_color_bg);
    g.setColour(m_color_bd);
    g.drawRect(getBounds().withZeroOrigin(), 1.f);
    g.drawLine(0.f, 20.f, getWidth(), 20.f, 1.f);
    const Patch patch = m_processor.getPatch();
    if(patch)
    {
        Gui camo = patch.getCamomile();
        if(!camo)
        {
            juce::Font f(Typeface::createSystemTypefaceFor(BinaryData::Font_ttf, BinaryData::Font_ttfSize));
            f.setHeight(22.f);
            g.setFont(f);
            g.setColour(Colours::white);
            g.drawText(juce::String("The patch has no interfaces !"), 0, 21, getWidth(), getHeight() - 21, juce::Justification::centred);
        }
    }
    else
    {
        juce::Font f(Typeface::createSystemTypefaceFor(BinaryData::Font_ttf, BinaryData::Font_ttfSize));
        f.setHeight(22.f);
        g.setFont(f);
        g.setColour(Colours::white);
        g.drawText(juce::String("Drag & Drop your patch..."), 0, 21, getWidth(), getHeight() - 21, juce::Justification::centred);
    }
    
    if(m_dropping)
    {
        g.fillAll(Colours::lightblue.withAlpha(0.2f));
    }
}

void PatchEditor::handleAsyncUpdate()
{
    const MessageManagerLock mml(Thread::getCurrentThread());
    if(mml.lockWasGained())
    {
        repaint();
    }
}

bool PatchEditor::isInterestedInFileDrag(const StringArray& files)
{
    if(files.size())
    {
        for(int i = 0; i < files.size(); i++)
        {
            if(files[i].endsWith(juce::StringRef(".pd")))
            {
                return true;
            }
        }
    }
    return false;
}

void PatchEditor::filesDropped(const StringArray& files, int x, int y)
{
    if(files.size())
    {
        for(int i = 0; i < files.size(); i++)
        {
            juce::File file(files[i]);
            if(file.getFileExtension() == juce::String(".pd"))
            {
                m_processor.loadPatch(file);
                return;
            }
        }
    }
}

void PatchEditor::patchChanged()
{
    removeAllChildren();
    for(int i = 0; i < m_buttons.size(); i++)
    {
        addAndMakeVisible(m_buttons[i]);
    }
    addAndMakeVisible(m_button_infos);
    
    m_objects.clear(true);
    const Patch patch = m_processor.getPatch();
    if(patch)
    {
        const Gui camo = patch.getCamomile();
        if(camo)
        {
            const std::vector<Gui> objects = patch.getGuis();
            const std::array<int,2> ref = camo.getPosition();
            for(auto it : objects)
            {
                if(it != camo)
                {
                    ObjectEditor* inte = m_objects.add(new ObjectEditor(*this, it));
                    const std::array<int,2> pos = it.getPosition();
                    const int offset = it.getBorderSize();
                    inte->setTopLeftPosition(pos[0] - ref[0] - offset + 2, pos[1] - ref[1] - offset + 24);
                    addChildComponent(inte);
                }
            }
            m_color_bg = tojColor(camo.getBackgroundColor());
            m_color_bd = tojColor(camo.getBorderColor());
            setSize(std::max(camo.getWidth() + 4, 250), std::max(camo.getHeight() + 26, 100));
        }
        m_last_path = patch.getPath() + File::separatorString + patch.getName();
    }
    
    DrawableImage image;
    image.setImage(ImageCache::getFromMemory(BinaryData::flowerG_png, BinaryData::flowerG_pngSize));
    DrawableImage image2(image);
    image2.setOverlayColour(m_color_bd);
    DrawableImage image3(image);
    image3.setOverlayColour(m_color_bd);
    m_button_infos->setImages(&image, &image2, &image3);
    
    const Colour overcolor = Colours::white.overlaidWith(m_color_bd);
    for(int i = 0; i < m_buttons.size(); i++)
    {
        m_buttons[i]->setColour(TextButton::textColourOnId, overcolor);
    }
    if(m_window)
    {
        m_window->setBackgroundColour(m_color_bg);
    }
    AsyncUpdater::triggerAsyncUpdate();
}

void PatchEditor::fileDragEnter(const StringArray& files, int x, int y)
{
    AsyncUpdater::triggerAsyncUpdate();
}

void PatchEditor::fileDragExit(const StringArray& files)
{
    m_dropping = false;
    AsyncUpdater::triggerAsyncUpdate();
}

void PatchEditor::buttonClicked(Button* button)
{
    if(button->getRadioGroupId() == 1)
    {
        if(!m_window || (m_window && m_window->getName() != String("About Camomile")))
        {
            m_window = new AboutWindow();
        }
        m_window->addToDesktop();
        m_window->centreAroundComponent(this, m_window->getWidth(), m_window->getHeight());
        m_window->setBackgroundColour(m_color_bg);
    }
    else if(button->getRadioGroupId() == 2)
    {
        if(m_window)
        {
            m_window = nullptr;
        }
        FileChooser fc("Open a patch...", File::getSpecialLocation(File::userDocumentsDirectory), "*.pd", true);
        if(fc.browseForFileToOpen())
        {
            juce::File file(fc.getResult());
            if(file.getFileExtension() == juce::String(".pd"))
            {
                m_processor.loadPatch(file);
            }
        }
    }
    else if(button->getRadioGroupId() == 3)
    {
        if(m_window)
        {
            m_window = nullptr;
        }
        m_processor.loadPatch(juce::File());
    }
    else if(button->getRadioGroupId() == 4)
    {
        File file(m_last_path);
        if(file.exists())
        {
            m_processor.loadPatch(file);
        }
    }
    else if(button->getRadioGroupId() == 5)
    {
        if(!m_window || (m_window && m_window->getName() != String("Camomile Console")))
        {
            m_window = new ConsoleWindow(m_processor);
        }
        m_window->addToDesktop();
        m_window->centreAroundComponent(this, m_window->getWidth(), m_window->getHeight());
        m_window->setBackgroundColour(m_color_bg.brighter(0.75));
    }
    else if(button->getRadioGroupId() == 6)
    {
        if(m_window)
        {
            m_window = nullptr;
        }
        juce::URL url("https://github.com/pierreguillot/Camomile/wiki");
        if(url.isWellFormed())
        {
            url.launchInDefaultBrowser();
        }
    }
}

