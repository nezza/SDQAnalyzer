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

	for( ; ; )
	{
		// std::cout << "ANALYZING\n";
		U8 data = 0;
		U8 mask = 1;
		
		U64 starting_sample = 0;
		U64 frame_start = 0;
		U64 bits = 0;

		while(bits != 8)
		{
			// std::cout << ".";
			mSerial->AdvanceToNextEdge(); // Go to first falling edge

			starting_sample = mSerial->GetSampleNumber();

			// Go to next raising edge
			mSerial->AdvanceToNextEdge();
			U64 raising_edge = mSerial->GetSampleNumber();

			U64 diff = raising_edge - starting_sample;
			double time_seconds = double(diff)/double(mSampleRateHz);

			ReportProgress( mSerial->GetSampleNumber() );

			if(bits == 0) {
				frame_start = mSerial->GetSampleNumber();
			}

			double bitrate = double(mSettings->mBitRate);
			// The 0 low flank is roughly 0.000_00655 uS
			double FALSE_TIME = (1.0/bitrate) * 0.644;
			double TRUE_TIME = (1.0/bitrate) * 0.175;
			double RESET_TIME = (1.0/bitrate) * 1.33;

			// ca. +/- 1 microsecond at 98425 baud
			double MATCH_PRECISION = (1.0/bitrate) * 0.098;

			if(std::abs(time_seconds - RESET_TIME) < MATCH_PRECISION)
			{
				mResults->AddMarker(mSerial->GetSampleNumber(), AnalyzerResults::Square, mSettings->mInputChannel);

				data = 0;
				mask = 1;
				bits = 0;
			} else if(std::abs(time_seconds - FALSE_TIME) < MATCH_PRECISION)
			{
				mResults->AddMarker(mSerial->GetSampleNumber(), AnalyzerResults::UpArrow, mSettings->mInputChannel);
				bits += 1;

				mask = mask << 1;
			} else if(std::abs(time_seconds - TRUE_TIME) < MATCH_PRECISION) {
				mResults->AddMarker(mSerial->GetSampleNumber(), AnalyzerResults::DownArrow, mSettings->mInputChannel);
				data |= mask;
				bits += 1;

				mask = mask << 1;
			} else {
				mResults->AddMarker(mSerial->GetSampleNumber(), AnalyzerResults::ErrorDot, mSettings->mInputChannel);
			}

		}
	
		//we have a byte to save. 
		Frame frame;
		frame.mData1 = data;
		frame.mFlags = 0;
		frame.mStartingSampleInclusive = frame_start;
		frame.mEndingSampleInclusive = mSerial->GetSampleNumber();

		mResults->AddFrame( frame );
		mResults->CommitResults();
		ReportProgress( frame.mEndingSampleInclusive );
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