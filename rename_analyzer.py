import os
import glob
import sys

# Fix Python 2.x.
try:
    input = raw_input
except NameError: pass

print("")
print("")
print("What would you like to call your new analyzer?")
print("")
print(">>The files under '/source' will be modified to use it.")
print(">>Examples include Serial, MySerial, JoesSerial, Gamecube, Wiimote, 2Wire, etc.")
print(">>Do not inclide the trailing word 'Analyzer' this will be added automaticly.")
print("")
print("(press CTRL-C to cancel)")
print("")

new_analyzer_name = input( "Your new analyzer name: " )

print("")
print("")
print("What is the analyzer's title? (as shown in the add new anlayzer drop down)")
print("")
print(">>Examples include Async Serial, I2C, Joe's Serial, Gamecube, Wiimote, 2Wire, etc.")
print("")
print("(press CTRL-C to cancel)")
print("")

new_analyzer_title = input( "Your new analyzer's title: " )

original_name = "SimpleSerial"


vs_project_path = "Visual Studio"
os.chdir( vs_project_path )

project_files = glob.glob("*.sln") + glob.glob("*.vcxproj") #returns only the file names, no paths.

for file in project_files:
	contents = open( file, 'r' ).read()
	contents = contents.replace( original_name + "Analyzer", new_analyzer_name + "Analyzer" )
	contents = contents.replace( original_name.upper() + "ANALYZER", new_analyzer_name.upper() + "ANALYZER" )
	contents = contents.replace( original_name + "SimulationDataGenerator", new_analyzer_name + "SimulationDataGenerator" )
	open( file, 'w' ).write( contents )

os.rename( glob.glob("*.sln")[0], new_analyzer_name + "Analyzer.sln" )
os.rename( glob.glob("*.vcxproj")[0], new_analyzer_name + "Analyzer.vcxproj" )

source_path = "source"
os.chdir( ".." )
os.chdir( source_path )

files = dict()

cpp_files = glob.glob( "*.cpp" );
h_files = glob.glob( "*.h" );

for file in cpp_files:
    files[file] = ".cpp"

for file in h_files:
    files[ file ] = ".h"

new_files = []

for file, extension in files.items():
    name_root = file.replace( original_name, "" )
    new_name = new_analyzer_name + name_root
    #print "renaming file " + file + " to " + new_name
    os.rename( file, new_name )
    new_files.append( new_name )

for file in new_files:
	contents = open( file, 'r' ).read()
	contents = contents.replace( original_name + "Analyzer", new_analyzer_name + "Analyzer" )
	contents = contents.replace( original_name.upper() + "_ANALYZER_", new_analyzer_name.upper() + "_ANALYZER_" )
	contents = contents.replace( original_name.upper() + "_SIMULATION_DATA_GENERATOR", new_analyzer_name.upper() + "_SIMULATION_DATA_GENERATOR" )
	contents = contents.replace( original_name + "SimulationDataGenerator", new_analyzer_name + "SimulationDataGenerator" )
	contents = contents.replace( "Simple Serial", new_analyzer_title )
	open( file, 'w' ).write( contents )