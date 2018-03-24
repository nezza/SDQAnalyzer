#ifndef SDQ_ANALYZER_H
#define SDQ_ANALYZER_H

#include <Analyzer.h>
#include "SDQAnalyzerResults.h"
#include "SDQSimulationDataGenerator.h"

class SDQAnalyzerSettings;
class ANALYZER_EXPORT SDQAnalyzer : public Analyzer2
{
public:
	SDQAnalyzer();
	virtual ~SDQAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< SDQAnalyzerSettings > mSettings;
	std::auto_ptr< SDQAnalyzerResults > mResults;
	AnalyzerChannelData* mSerial;

	SDQSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	U32 mSampleRateHz;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //SDQ_ANALYZER_H
