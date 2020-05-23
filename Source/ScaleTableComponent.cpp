/*
==============================================================================

ScaleTableComponent.cpp
Created: 23 May 2020 9:29:57am
Author:  Rasmus Kjærbo

==============================================================================
*/

#include "ScaleTableComponent.h"

//==============================================================================
// A child component for our table
// TODO: Move to a different header and CPP file

ScaleTableComponent::TableComponent()
{
    loadData(); //load data from XML file
    
    addAndMakeVisible (table);
    
    table.setColour (ListBox::outlineColourId, Colours::grey);
    table.setOutlineThickness (1);
    
    if (columnList != nullptr)
    {
        forEachXmlChildElement (*columnList, columnXml)
        {
            table.getHeader().addColumn (columnXml->getStringAttribute ("name"),
                                          columnXml->getIntAttribute ("id"),
                                          columnXml->getIntAttribute ("width"),
                                          50,
                                          400,
                                          TableHeaderComponent::defaultFlags);
        }
    }
    
    table.getHeader().setSortColumnId (1, true);
    
    table.setMultipleSelectionEnabled (true);
}

int ScaleTableComponent::getNumRows() override
{
    return numRows;
}

void ScaleTableComponent::paintRowBackground (Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override
{
    auto alternateColour = getLookAndFeel().findColour (ListBox::backgroundColourId)
    .interpolatedWith (getLookAndFeel().findColour (ListBox::textColourId), 0.03f);
    if (rowIsSelected)
        g.fillAll (Colours::lightblue);
    else if (rowNumber % 2)
        g.fillAll (alternateColour);
}

void ScaleTableComponent::paintCell (Graphics& g, int rowNumber, int columnId,
                int width, int height, bool rowIsSelected) override
{
    g.setColour (rowIsSelected ? Colours::darkblue : getLookAndFeel().findColour (ListBox::textColourId));
    g.setFont (font);
    
    if (auto* rowElement = dataList->getChildElement (rowNumber))
    {
        auto text = rowElement->getStringAttribute (getAttributeNameForColumnId (columnId));
        
        g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
    }
    
    g.setColour (getLookAndFeel().findColour (ListBox::backgroundColourId));
    g.fillRect (width - 1, 0, 1, height);
}

void ScaleTableComponent::sortOrderChanged (int newSortColumnId, bool isForwards) override
{

}

Component* ScaleTableComponent::refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                    Component* existingComponentToUpdate) override
{
    return nullptr;  // to avoid warnings for now
}

int ScaleTableComponent::getColumnAutoSizeWidth (int columnId) override
{        
    int widest = 65;
    
    for (auto i = getNumRows(); --i >= 0;)
    {
        if (auto* rowElement = dataList->getChildElement (i))
        {
            auto text = rowElement->getStringAttribute (getAttributeNameForColumnId (columnId));
            
            widest = jmax (widest, font.getStringWidth (text));
        }
    }
    
    return widest + 8;
}

int ScaleTableComponent::getSelection (const int rowNumber) const
{
    return dataList->getChildElement (rowNumber)->getIntAttribute ("Select");
}

void ScaleTableComponent::setSelection (const int rowNumber, const int newSelection)
{
    dataList->getChildElement (rowNumber)->setAttribute ("Select", newSelection);
}

String ScaleTableComponent::getText (const int columnNumber, const int rowNumber) const
{
    return dataList->getChildElement (rowNumber)->getStringAttribute (getAttributeNameForColumnId (columnNumber));
}

void ScaleTableComponent::setText (const int columnNumber, const int rowNumber, const String& newText)
{
    const auto& columnName = table.getHeader().getColumnName (columnNumber);
    dataList->getChildElement (rowNumber)->setAttribute (columnName, newText);
}

void ScaleTableComponent::resized() override
{
    table.setBoundsInset (BorderSize<int> (8));
}

void ScaleTableComponent::paint (Graphics&) override {}


void ScaleTableComponent::loadData()
{
    auto dir = File::getCurrentWorkingDirectory();
    
    int numTries = 0;
    
    while (! dir.getChildFile ("Resources").exists() && numTries++ < 15)
        dir = dir.getParentDirectory();
    
    auto tableFile = dir.getChildFile ("Resources").getChildFile ("scaledata.xml");
    
    if (tableFile.exists())
    {
        tutorialData = XmlDocument::parse (tableFile);
        
        dataList   = tutorialData->getChildByName ("DATA");
        columnList = tutorialData->getChildByName ("HEADERS");
        
        numRows = dataList->getNumChildElements();
    }
}

String ScaleTableComponent::getAttributeNameForColumnId (const int columnId) const
{
    forEachXmlChildElement (*columnList, columnXml)
    {
        if (columnXml->getIntAttribute ("columnId") == columnId)
            return columnXml->getStringAttribute ("name");
    }
    
    return {};
}
//==============================================================================
//JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TableComponent)