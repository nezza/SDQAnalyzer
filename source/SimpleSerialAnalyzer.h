#ifndef SIMPLESERIAL_ANALYZER_H
#define SIMPLESERIAL_ANALYZER_H

#include <Analyzer.h>
#include "SimpleSerialAnalyzerResults.h"
#include "SimpleSerialSimulationDataGenerator.h"

class SimpleSerialAnalyzerSettings;
class ANALYZER_EXPORT SimpleSerialAnalyzer : public Analyzer2
{
public:
	SimpleSerialAnalyzer();
	virtual ~SimpleSerialAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< SimpleSerialAnalyzerSettings > mSettings;
	std::auto_ptr< SimpleSerialAnalyzerResults > mResults;
	AnalyzerChannelData* mSerial;

	SimpleSerialSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U32 mStartOfStopBitOffset;
	U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //SIMPLESERIAL_ANALYZER_H
