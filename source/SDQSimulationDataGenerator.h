#ifndef SDQ_SIMULATION_DATA_GENERATOR
#define SDQ_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class SDQAnalyzerSettings;

class SDQSimulationDataGenerator
{
public:
	SDQSimulationDataGenerator();
	~SDQSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, SDQAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	SDQAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //SDQ_SIMULATION_DATA_GENERATOR