/*
 // Copyright (c) 2015 Pierre Guillot.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef __CAMOMILE_PATCH_EDITOR__
#define __CAMOMILE_PATCH_EDITOR__

#include "../JuceLibraryCode/JuceHeader.h"
#include "InstanceProcessor.h"
#include "ObjectEditor.h"

// ==================================================================================== //
//                                  CAMOMILE INTERFACE                                  //
// ==================================================================================== //

class PatchEditor  :
public AudioProcessorEditor,
public InstanceProcessor::Listener,
public FileDragAndDropTarget,
public Button::Listener
{
private:
    InstanceProcessor&      m_processor;
    OwnedArray<ObjectEditor>m_objects;
    bool                    m_dropping;
    OwnedArray<Component>   m_buttons;
public:
    PatchEditor(InstanceProcessor&);
    ~PatchEditor();
    void paint(Graphics&) override;
    void resized() override;
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;
    void fileDragEnter(const StringArray& files, int x, int y) override;
    void fileDragExit(const StringArray& files) override;
    void buttonClicked(Button* button) override;
    void patchChanged() override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchEditor)
};



#endif  // PLUGINEDITOR_H_INCLUDED
