#ifndef SIMPLESERIAL_SIMULATION_DATA_GENERATOR
#define SIMPLESERIAL_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class SimpleSerialAnalyzerSettings;

class SimpleSerialSimulationDataGenerator
{
public:
	SimpleSerialSimulationDataGenerator();
	~SimpleSerialSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, SimpleSerialAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	SimpleSerialAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //SIMPLESERIAL_SIMULATION_DATA_GENERATOR