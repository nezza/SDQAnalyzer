#include "SDQAnalyzer.h"
#include "SDQAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

#include <cmath>

SDQAnalyzer::SDQAnalyzer()
:	Analyzer2(),  
	mSettings( new SDQAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
	UseFrameV2();
}

SDQAnalyzer::~SDQAnalyzer()
{
	KillThread();
}

void SDQAnalyzer::SetupResults()
{
	mResults.reset( new SDQAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannel );
}

void SDQAnalyzer::WorkerThread()
{
	mSampleRateHz = GetSampleRate();

	mSerial = GetAnalyzerChannelData( mSettings->mInputChannel );

	// Skip over e.g. beginning low data
	if( mSerial->GetBitState() == BIT_LOW )
		mSerial->AdvanceToNextEdge();

	uint64_t lastSample = mSerial->GetSampleNumber();
	int lastSampleIsHighEdge = 0;

	uint64_t watchwindow = (21e-6)*mSampleRateHz;

	while (1){
		uint8_t data = 0;
		uint8_t bits = 0;
		uint64_t break_pos = 0;
		uint64_t curSample = 0;
		uint64_t curLastSample = 0;
		uint64_t frame_starts[10] = {};
		int hasAdvanced = 1;

		while (1){
			if (bits >=8 && !mSerial->WouldAdvancingCauseTransition(watchwindow)) break;
			if (!hasAdvanced) lastSampleIsHighEdge = !lastSampleIsHighEdge;
			if (bits == 8 && !mSerial->WouldAdvancingCauseTransition(watchwindow)){
				curSample += watchwindow;
				hasAdvanced = 0;
			}else{
				mSerial->AdvanceToNextEdge(); // to falling endge
				hasAdvanced = 1;
				curSample = mSerial->GetSampleNumber();
			}
			// printf("curSample=%llu next=%llu\n",curSample,mSerial->GetSampleOfNextEdge());

			curLastSample = lastSample;
			U64 diff = curSample - lastSample;
			double time_seconds = double(diff)/double(mSampleRateHz);
			lastSample = curSample;
			lastSampleIsHighEdge = !lastSampleIsHighEdge;

			if (lastSampleIsHighEdge){
				//high duration
				if (time_seconds > 11e-6){
					//bad duration
					break;
				}
				mResults->AddMarker(curSample, AnalyzerResults::Dot, mSettings->mInputChannel);
			}else{
				//low duration
				if (time_seconds > 20e-6){
					//bad duration
					break;
				}

				if (time_seconds > 10e-6){
					if (bits){
						//bad duration
						break;
					}else{
						//encountered break
						break_pos = curSample;
						mResults->AddMarker(curSample, AnalyzerResults::Square, mSettings->mInputChannel);
					}
				}else{
					//encountered bit
					if (bits<sizeof(frame_starts)/sizeof(*frame_starts)) frame_starts[bits] = curLastSample;
					bits++;
					data >>= 1;
					if (time_seconds > 5e-6){
						mResults->AddMarker(curSample, AnalyzerResults::UpArrow, mSettings->mInputChannel);
					}else{
						mResults->AddMarker(curSample, AnalyzerResults::DownArrow, mSettings->mInputChannel);
						data |= 0x80;
					}
				}
			}
		}
		if (bits >= 8){
			//we have a byte to save. 
			Frame frame;
			frame.mData1 = data;
			frame.mFlags = 0;
			frame.mStartingSampleInclusive = frame_starts[bits-8];
			frame.mEndingSampleInclusive = curLastSample;

			if (!frame.mStartingSampleInclusive){
				// printf("bad frame data=0x%08x curSample=%llu\n",data,curSample);
				continue;
			}

			FrameV2 frame_v2;
			frame_v2.AddByte("value", data);
			mResults->AddFrameV2( frame_v2, "byte", frame.mStartingSampleInclusive, frame.mEndingSampleInclusive );

			mResults->AddFrame( frame );
			mResults->CommitResults();
			ReportProgress( frame.mEndingSampleInclusive );
		}
	}
}

bool SDQAnalyzer::NeedsRerun()
{
	return false;
}

U32 SDQAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 SDQAnalyzer::GetMinimumSampleRateHz()
{
	return mSettings->mBitRate * 4;
}

const char* SDQAnalyzer::GetAnalyzerName() const
{
	return "SDQ (Lightning)";
}

const char* GetAnalyzerName()
{
	return "SDQ (Lightning)";
}

Analyzer* CreateAnalyzer()
{
	return new SDQAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}