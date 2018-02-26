# Saleae Analyzer SDK Sample Analyzer
The Saleae Analyzer SDK is used to create custom plugins for the Saleae Logic software. These plugins are used to decode protocol data from captured waveforms.

The libraries required to build a custom analyzer are stored in another git repository, located here:
[https://github.com/saleae/AnalyzerSDK](https://github.com/saleae/AnalyzerSDK)

This repository should be used to create new analyzers for the Saleae software.

First, fork, clone or download this repository. Forking is recommended if you plan to use version control or share your custom analyzer publicly.

Note - This repository contains a submodule. Be sure to include submodules when cloning, for example `git clone --recursive https://github.com/saleae/SampleAnalyzer.git`. If you download the repository from Github, the submodules are not included. In that case you will also need to download the AnalyzerSDK repository linked above and place the AnalyzerSDK folder inside of the SampleAnalyzer folder.

*Note: an additional submodule is used for debugging on Windows, see section on Windows debugging for more information.*

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

To debug on Windows, please first review the section titled `Debugging an Analyzer with Visual Studio` in the included `doc/Analyzer SDK Setup.md` document.

Unfortunately, debugging is limited on Windows to using an older copy of the Saleae Logic software that does not support the latest hardware devices. Details are included in the above document.