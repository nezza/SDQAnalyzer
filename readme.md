# Saleae Analyzer SDK Sample Analyzer
The Saleae Analyzer SDK is used to create custom plugins for the Saleae Logic software. These plugins are used to decode protocol data from captured waveforms. 

The libraries required to build a custom analyzer are stored in another git repository, located here:
[https://github.com/saleae/AnalyzerSDK](https://github.com/saleae/AnalyzerSDK)

This repository should be used to create new analyzers for the Saleae software.

First, fork, clone or download this repository. Forking is recommended if you plan to use version control or share your custom analyzer publicly.

Note - This repository contains a submodule. Be sure to include submodules when cloning, for example `git clone --recursive https://github.com/saleae/SampleAnalyzer.git`. If you download the repository from Github, the submodules are not included. In that case you will also need to download the AnalyzerSDK repository linked above and place the AnalyzerSDK folder inside of the SampleAnalyzer folder.

Once downloaded, first run the script rename_analyzer.py. This script is used to rename the sample analyzer automatically. Specifically, it changes the class names in the source code, it changes the text name that will be displayed once the custom analyzer has been loaded into the Saleae Logic software, and it updates the visual studio project.

There are two names you need to provide to rename_analyzer. The first is the class name. For instance, if you are developing a SPI analyzer, the class names would be SPIAnalyzer, SPIAnalyzerResults, SPIAnalyzerSettings, etc.
The file names would be similar, like SPIAnalyzer.cpp, etc.

All analyzer classes should end with "Analyzer," so the rename script will add that for you. In the first prompt after starting the script, enter "SPI". The analyzer suffix will be added for you. This needs to be a valid C++ class name - no spaces, it can't start with a number, etc.

Second, the script will prompt you for the display name. This will appear in the software in the list of analyzers after the plugin has loaded. This string can have spaces, since it will always be treated as a string, and not as the name of a class.

After that, the script will complete the renaming process and exit.

    python rename_analyzer.py
    SPI
    Mark's SPI Analyzer

To build on Windows, open the visual studio project in the Visual Studio folder, and build. The Visual Studio solution has configurations for 32 bit and 64 bit builds. You will likely need to switch the configuration to 64 bit and build that in order to get the analyzer to load in the Windows software.

To build on Linux or OSX, run the build_analyzer.py script. The compiled libraries can be found in the newly created debug and release folders.

	python build_analyzer.py

To debug on Windows, please first review the article here:

[How do I develop custom analyzers for the Logic software on Windows?](http://support.saleae.com/hc/en-us/articles/208666946)

On Windows, it is not possible to attach a debugger to the latest software. In order to debug custom analyzers on Windows, you need to use an older version of the Windows software.
Specifically, you will need the Saleae Logic 1.1.18 Windows 32 bit standalone release. You can find it here:

[Older Saleae Logic Beta Downloads](http://support.saleae.com/hc/en-us/articles/210245603)

You will also need the 1.1.14 SDK, the last SDK release to support software versions 1.1.14-1.1.18. That is available on a separate branch of the Saleae AnalyzerSDK Github repository. Simply change the submodule configuration to point to that branch, and the existing visual studio project will just work. Be sure to only compile for 32 bit, since the 1.1.14 SDK was released before Saleae began targeting 64 bit Windows. This is also why the 32 bit standalone version of the 1.1.18 software is required.
It's also worth noting that you should use a #define and #ifdef to remove the contents of your Analyzer's GenerateFrameTabularText methods, because the methods ClearTabularText and AddTabularText were not yet present in the 1.1.14 SDK.

The Sample Analyzer Project also contains a branch, 1.1.14-for-win32-debug, which already contains the updated git submodule branch. Simply merge your changes into that branch, and then you will not need to switch submodule branches.

In the future, I would like to update this so that no branch changes are required, and the visual studio project just contains configurations for both the latest SDK and the 1.1.14 SDK.
