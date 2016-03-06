/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "InstanceProcessor.h"
#include "PatchEditor.h"

Font    PatchEditor::font;
int     PatchEditor::bordersize;
Colour  PatchEditor::color_bg;
Colour  PatchEditor::color_bd;
Colour  PatchEditor::color_txt;
Colour  PatchEditor::color_invisible;

class PatchEditor::GuiParameter : public Component, public Timer, public SliderListener
{
public:
    GuiParameter(int index, InstanceProcessor& processor, pd::Gui const& gui) :
    m_index(index), m_processor(processor)
    {
        std::array<float, 4> bounds(gui.getBounds());
        std::array<float, 2> labelpos(gui.getLabelPosition());
        
        if(gui.getType() == pd::Gui::Type::HorizontalSlider)
        {
            Slider* sld = new Slider(Slider::LinearHorizontal, Slider::NoTextBox);
            sld->setRange(0.f, 1.f);
            sld->setBounds(int(bounds[0]), int(bounds[1]), int(bounds[2]), int(bounds[3]));
            sld->setValue(m_processor.getParameter(m_index));
            sld->setColour(Slider::backgroundColourId, PatchEditor::color_invisible);
            sld->setColour(Slider::thumbColourId, PatchEditor::color_txt);
            addAndMakeVisible(sld);
            sld->addListener(this);
            m_slider = sld;
        }
        else if(gui.getType() == pd::Gui::Type::VecticalSlider)
        {
            Slider* sld = new Slider(Slider::LinearVertical, Slider::NoTextBox);
            sld->setRange(0.f, 1.f);
            sld->setBounds(int(bounds[0]), int(bounds[1]), int(bounds[2]), int(bounds[3]));
            sld->setValue(m_processor.getParameter(m_index));
            sld->setColour(Slider::backgroundColourId, PatchEditor::color_invisible);
            sld->setColour(Slider::thumbColourId, PatchEditor::color_txt);
            addAndMakeVisible(sld);
            sld->addListener(this);
            m_slider = sld;
        }
        else if(gui.getType() == pd::Gui::Type::Toggle)
        {
            ;
        }
        else if(gui.getType() == pd::Gui::Type::Number)
        {
            ;
        }
       
        
        std::string name = gui.getName();
        if(!name.empty())
        {
            m_label = new Label("label", String(gui.getName()) + " " + gui.getLabel());
            m_label->setFont(PatchEditor::font);
            m_label->setJustificationType(Justification::topLeft);
            m_label->setColour(Label::backgroundColourId, PatchEditor::color_invisible);
            m_label->setColour(Label::textColourId, PatchEditor::color_txt);
            m_label->setBorderSize(BorderSize<int>());
            m_label->setBounds(labelpos[0],
                               labelpos[1] - PatchEditor::font.getHeight() * 0.5,
                               1000,
                               200);
            m_label->setInterceptsMouseClicks(false, false);
            addAndMakeVisible(m_label);
        }
        setInterceptsMouseClicks(false, true);
        Timer::startTimer(100);
    }
    
    ~GuiParameter()
    {
        Timer::stopTimer();
    }
    
    void timerCallback() final
    {
        if(m_index >= 0)
        m_slider->setValue(m_processor.getParameter(m_index), NotificationType::dontSendNotification);
    }
    
    void sliderValueChanged(Slider* slider) final
    {
        if(m_index >= 0)
        m_processor.setParameterNotifyingHost(m_index, slider->getValue());
    }
private:
    const int               m_index;
    InstanceProcessor&      m_processor;
    ScopedPointer<Slider>   m_slider;
    ScopedPointer<Label>    m_label;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuiParameter)
};

// ==================================================================================== //
//                                      IMAGE BUTTON                                    //
// ==================================================================================== //

class PatchEditor::ImgButton : public Button
{
private:
    DrawableImage   m_image1;
    DrawableImage   m_image2;
    DrawableImage   m_image3;
    PatchEditor*    m_editor;
public:
    ImgButton(PatchEditor* editor, const String& name, int rid) : Button(name), m_editor(editor)
    {
        addListener(editor);
        setClickingTogglesState(false);
        setAlwaysOnTop(true);
        setRadioGroupId(rid);
        m_image1.setImage(ImageCache::getFromMemory(BinaryData::flower1_png, BinaryData::flower1_pngSize));
        m_image2.setImage(ImageCache::getFromMemory(BinaryData::flower1_png, BinaryData::flower1_pngSize));
        m_image3.setImage(ImageCache::getFromMemory(BinaryData::flower2_png, BinaryData::flower2_pngSize));
        m_image1.setTransformToFit(Rectangle<float>(0.f, 0.f, 15.f, 15.f), RectanglePlacement::stretchToFit);
        m_image2.setTransformToFit(Rectangle<float>(0.f, 0.f, 15.f, 15.f), RectanglePlacement::stretchToFit);
        m_image3.setTransformToFit(Rectangle<float>(0.f, 0.f, 15.f, 15.f), RectanglePlacement::stretchToFit);
        m_image1.setOverlayColour(PatchEditor::color_txt);
        m_image2.setOverlayColour(PatchEditor::color_txt.interpolatedWith(PatchEditor::color_bg, 0.5f));
        addAndMakeVisible(m_image1, 0);
        addChildComponent(m_image2, 0);
        addAndMakeVisible(m_image3, -1);
        m_image3.setAlwaysOnTop(true);
        setBounds(3, 3, 15, 15);
    }
    
    void paintButton(Graphics& g, bool over, bool down) override {}
    
    void buttonStateChanged() override
    {
        if(isDown() || isOver())
        {
            m_image1.setVisible(false);
            m_image2.setVisible(true);
        }
        else
        {
            m_image1.setVisible(true);
            m_image2.setVisible(false);
        }
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImgButton)
};

// ==================================================================================== //
//                                          ABOUT                                       //
// ==================================================================================== //

class PatchEditor::About : public Component
{
private:
    DrawableImage    m_image1;
    juce::TextEditor m_text;
public:
    About()
    {
        m_image1.setImage(ImageCache::getFromMemory(BinaryData::flowerG_png, BinaryData::flowerG_pngSize));
        m_image1.setBoundingBox(RelativeParallelogram(Rectangle<float>(300.f - 128.f, 370.f - 128.f, 128.f, 128.f)));
        m_image1.setOpacity(0.5f);
        addAndMakeVisible(&m_image1);
        
        m_text.setMultiLine(true);
        m_text.setReadOnly(true);
        m_text.setScrollbarsShown(false);
        m_text.setCaretVisible(false);
        m_text.setPopupMenuEnabled(true);
        m_text.setColour(juce::TextEditor::backgroundColourId, PatchEditor::color_invisible);
        m_text.setColour(juce::TextEditor::outlineColourId, PatchEditor::color_invisible);
        m_text.setColour(juce::TextEditor::shadowColourId,PatchEditor::color_invisible);
        m_text.setColour(juce::TextEditor::textColourId, PatchEditor::color_txt);
        m_text.setFont(PatchEditor::font);
        m_text.setText("Camomile is a dynamic plugin made with Juce that allows to load and control Pure Data patches inside a digital audio workstation.\n\n"
                       "Author :\n"+ String(JucePlugin_Manufacturer) + "\n\n"
                       "Organizations :\nCICM | Université Paris 8 | Labex Arts H2H\n\n"
                       "Contacts :\n"+String(JucePlugin_ManufacturerEmail)+"\n"+ String(JucePlugin_ManufacturerWebsite)+"\n\n"
                       "Credits :\nPure Data by Miller Puckette\nJuce by ROLI Ltd.");
        m_text.setBounds(0, 0, 300, 370);
        addAndMakeVisible(&m_text, -1);
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(About)
};

// ==================================================================================== //
//                                          CONSOLE                                     //
// ==================================================================================== //

class PatchEditor::Console : public Component, public juce::Timer, public juce::TextEditor::Listener
{
private:
    juce::TextEditor m_text;
public:
    Console()
    {
        m_text.setMultiLine(true);
        m_text.setReadOnly(false);
        m_text.setScrollbarsShown(true);
        m_text.setCaretVisible(false);
        m_text.setPopupMenuEnabled (true);
        m_text.setColour(juce::TextEditor::backgroundColourId, PatchEditor::color_invisible);
        m_text.setColour(juce::TextEditor::outlineColourId, PatchEditor::color_invisible);
        m_text.setColour(juce::TextEditor::shadowColourId,PatchEditor::color_invisible);
        m_text.setColour(juce::TextEditor::textColourId, PatchEditor::color_txt);
        m_text.setFont(PatchEditor::font);
        m_text.setText(pd::Pd::getConsole());
        m_text.setBounds(0, 0, 300, 370);
        addAndMakeVisible(&m_text, 1);
        startTimer(20);
    }
    
    void textEditorTextChanged(juce::TextEditor& ed) override
    {
        pd::Pd::setConsole(ed.getText().toStdString());
    }
    
    void timerCallback() override
    {
        m_text.setText(pd::Pd::getConsole());
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Console)
};

// ==================================================================================== //
//                                          WINDOW                                      //
// ==================================================================================== //

class PatchEditor::PatchWin : public DocumentWindow
{
public:
    PatchWin(String const& name) :
    DocumentWindow(name, Colours::lightgrey, DocumentWindow::closeButton, false)
    {
        setUsingNativeTitleBar(true);
        setBounds(50, 50, 300, 370);
        setResizable(false, false);
        setAlwaysOnTop(true);
        setDropShadowEnabled(true);
        setVisible(true);
        setBackgroundColour(Colours::lightgrey);
    }

    void closeButtonPressed() override
    {
        removeFromDesktop();
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchWin)
};

// ==================================================================================== //
//                                  PATCHER EDITOR                                  //
// ==================================================================================== //

PatchEditor::PatchEditor(InstanceProcessor& p) :
AudioProcessorEditor(&p), m_processor(p),
m_dropping(false), m_window(nullptr)
{
    PatchEditor::font       = Font(String("Monaco"), 13.f, juce::Font::plain);
    PatchEditor::bordersize = 1;
    PatchEditor::color_bg   = Colours::lightgrey;
    PatchEditor::color_bd   = Colours::darkgrey;
    PatchEditor::color_txt  = Colours::darkgrey;
    PatchEditor::color_invisible = Colour::fromFloatRGBA(0.f, 0.f, 0.f, 0.f);
    
    
    Component::setWantsKeyboardFocus(true);
    m_processor.addListener(this);
    m_button = new ImgButton(this, "CamomileButton", 1);
    m_window = new PatchWin("");
    addAndMakeVisible(m_button);
    setSize(600, 420);
    patchChanged();
}

PatchEditor::~PatchEditor()
{
    m_processor.removeListener(this);
}

void PatchEditor::paint(Graphics& g)
{
    String text;
    g.fillAll(PatchEditor::color_bg);
    g.setColour(PatchEditor::color_bd);
    g.drawRect(getBounds().withZeroOrigin(), PatchEditor::bordersize);
    g.drawLine(0.f, 20.f, getWidth(), 20.f, PatchEditor::bordersize);
    g.setFont(PatchEditor::font);
    g.setColour(PatchEditor::color_txt);
    const pd::Patch patch = m_processor.getPatch();
    if(patch.isValid())
    {
        g.drawText(String(patch.getName()).trimCharactersAtEnd(".pd"), 0, 0, getWidth(), 20, juce::Justification::centred);
    }
    else
    {
        g.drawText(String("No Patch"), 0, 0, getWidth(), 20, juce::Justification::centred);
    }
    if(m_dropping)
    {
        g.fillAll(Colours::white.withAlpha(0.2f));
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

void PatchEditor::patchChanged()
{
    m_parameters.clear();
    const pd::Patch patch = m_processor.getPatch();
    if(patch.isValid())
    {
        std::array<float, 2> size(patch.getSize());
        setSize(size[0] > 0.f ? std::max(size[0], 20.f) : 600, size[1] > 0.f ? std::max(size[1], 20.f) + 20.f : 420);
        m_last_path = patch.getPath() + File::separatorString + patch.getName();
        
        
        std::vector<pd::Gui> guis(patch.getGuis());
        for(size_t i = 0; i < guis.size(); i++)
        {
            m_parameters.add(new GuiParameter(i, m_processor, guis[i]));
            m_parameters[i]->setBounds(getBounds().reduced(0, 10).withPosition(0, 20));
            addAndMakeVisible(m_parameters[i]);
        }
    }
    AsyncUpdater::triggerAsyncUpdate();
}

void PatchEditor::buttonClicked(Button* button)
{
    if(button->getRadioGroupId() == 1)
    {
        juce::PopupMenu m;
        m.addItem(1, "About");
        m.addItem(2, "Open");
        m.addItem(3, "Close");
        m.addItem(4, "Reload");
        m.addItem(5, "Console");
        m.addItem(6, "Help");
        const int result = m.showAt(button->getScreenBounds().translated(-2, 3));
        if(result == 1)
        {
            m_window->setContentOwned(new About(), false);
            m_window->setName("About Camomile v" + String(JucePlugin_VersionString));
            m_window->addToDesktop();
        }
        else if(result == 2)
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
        else if(result == 3)
        {
            if(m_window)
            {
                m_window = nullptr;
            }
            m_processor.loadPatch(juce::File());
        }
        else if(result == 4)
        {
            File file(m_last_path);
            if(file.exists())
            {
                m_processor.loadPatch(file);
            }
        }
        else if(result == 5)
        {
            m_window->setContentOwned(new Console(), false);
            m_window->setName("Camomile Console");
            m_window->addToDesktop();
        }
        else if(result == 6)
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
}

