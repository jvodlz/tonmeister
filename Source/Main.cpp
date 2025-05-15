/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"

//==============================================================================
class TonmeisterApplication  : public juce::JUCEApplication
{
public:
    //==============================================================================
    TonmeisterApplication() {}

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        mainWindow.reset(new MainWindow(getApplicationName(), new MainComponent, *this));
    }

    void shutdown() override
    {
        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
private: 
    class MainWindow    : public juce::DocumentWindow
    {
    public:
        MainWindow(const juce::String& name, juce::Component* c, JUCEApplication& a)
            : DocumentWindow(name, juce::Desktop::getInstance().getDefaultLookAndFeel()
                .findColour(ResizableWindow::backgroundColourId),
                juce::DocumentWindow::allButtons),
            app(a)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(c, true);

            #if JUCE_ANDROID || JUCE_IOS
            setFullScreen(true);
            #else
            setResizable(true, false);
            setResizeLimits(310, 360, 910, 960);
            centreWithSize(getWidth(), getHeight());
            #endif

           setVisible(true);

        }


        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            //JUCEApplication::getInstance()->systemRequestedQuit();
            
            //
            app.systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */
    private:
        //
        JUCEApplication& app;
        //
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (TonmeisterApplication)
