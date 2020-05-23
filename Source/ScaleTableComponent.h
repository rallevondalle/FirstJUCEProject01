/*
  ==============================================================================

    ScaleTableComponent.h
    Created: 23 May 2020 9:29:57am
    Author:  Rasmus Kjærbo

  ==============================================================================
*/
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

//==============================================================================
// A child component for our table
// TODO: Move to a different header and CPP file
class ScaleTableComponent   : public Component,
                              public TableListBoxModel
{
public:
    ScaleTableComponent();
    
    int getNumRows() override;
    
    void paintRowBackground (Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
    
    void paintCell (Graphics& g, int rowNumber, int columnId,
                    int width, int height, bool rowIsSelected) override;
    
    void sortOrderChanged (int newSortColumnId, bool isForwards) override;
    
    Component* refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                        Component* existingComponentToUpdate) override;
    
    int getColumnAutoSizeWidth (int columnId) override;
    
    int getSelection (const int rowNumber) const;
    
    void setSelection (const int rowNumber, const int newSelection);
    
    String getText (const int columnNumber, const int rowNumber) const;
    
    void setText (const int columnNumber, const int rowNumber, const String& newText);
    
    void resized() override;
    
    void paint (Graphics&) override;
    
private:
    TableListBox table  { {}, this };
    Font font           { 14.0f };
    
    std::unique_ptr<XmlElement> tutorialData;
    XmlElement* columnList = nullptr;
    XmlElement* dataList = nullptr;
    int numRows = 0;
    
    void loadData();
    
    String getAttributeNameForColumnId (const int columnId) const;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScaleTableComponent)
};
