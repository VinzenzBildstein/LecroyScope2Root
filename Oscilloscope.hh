#ifndef OSCILLOSCOPE_HH
#define OSCILLOSCOPE_HH

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>

#include "TObject.h"
#include "TGraphErrors.h"

class LecroyHeader : public TObject {
public:
  LecroyHeader() {}
  LecroyHeader(const char*);
  ~LecroyHeader() {}

  void ReadData(const char*);

  void Print(Option_t *opt = "") const;

  void Diff(const LecroyHeader& rhs) const;

  bool operator==(const LecroyHeader& rhs) const;
  bool operator!=(const LecroyHeader& rhs) const {
	 return !(*this == rhs);
  }

  void SetVerboseLevel(size_t verboseLevel) { fVerboseLevel = verboseLevel; }

  std::string GetTemplateName() { return fTemplateName; }
  bool GetWordFormat() { return fWordFormat; }
  bool GetLowBitFirst() { return fLowBitFirst; }
  int32_t GetWavedescLength() { return fWavedescLength; }
  int32_t GetUsertextLength() { return fUsertextLength; }
  int32_t GetTrigtimeLength() { return fTrigtimeLength; }
  int32_t GetRistimeLength() { return fRistimeLength; }
  std::string GetInstrumentName() { return fInstrumentName; }
  int32_t GetInstrumentNumber() { return fInstrumentNumber; }
  std::string GetTraceLabel() { return fTraceLabel; }
  int32_t GetPointsPerScreen() { return fPointsPerScreen; }
  int32_t GetFirstValidPoint() { return fFirstValidPoint; }
  int32_t GetLastValidPoint() { return fLastValidPoint; }
  int32_t GetFirstPoint() { return fFirstPoint; }
  int32_t GetSparsingFactor() { return fSparsingFactor; }
  int32_t GetSegmentIndex() { return fSegmentIndex; }
  int32_t GetSubarrayCount() { return fSubarrayCount; }
  int32_t GetSweepsPerAcquisition() { return fSweepsPerAcquisition; }
  int16_t GetPointsPerPair() { return fPointsPerPair; }
  int16_t GetPairOffset() { return fPairOffset; }
  float   GetVerticalGain() { return fVerticalGain; }
  float   GetVerticalOffset() { return fVerticalOffset; }
  float   GetMaxValue() { return fMaxValue; }
  float   GetMinValue() { return fMinValue; }
  int16_t GetNominalBits() { return fNominalBits; }
  int16_t GetNominalSubarrayCount() { return fNominalSubarrayCount; }
  float   GetHorizontalInterval() { return fHorizontalInterval; }
  double  GetHorizontalOffset() { return fHorizontalOffset; }
  double  GetPixelOffset() { return fPixelOffset; }
  std::string GetVerticalUnit() { return fVerticalUnit; }
  std::string GetHorizontalUnit() { return fHorizontalUnit; }
  float   GetHorizontalUncertainty() { return fHorizontalUncertainty; }
  float   GetAcquisitionDuration() { return fAcquisitionDuration; }
  int16_t GetRisSweeps() { return fRisSweeps; }
  int16_t GetTimebase() { return fTimebase; }
  int16_t GetVerticalCoupling() { return fVerticalCoupling; }
  float   GetProbeAttenuation() { return fProbeAttenuation; }
  int16_t GetFixedVerticalGain() { return fFixedVerticalGain; }
  bool    GetBandwidthLimit() { return fBandwidthLimit; }
  float   GetVerticalVernier() { return fVerticalVernier; }
  float   GetAcquisitionVerticalOffset() { return fAcquisitionVerticalOffset; }
  int16_t GetWaveSource() { return fWaveSource; }

  size_t  GetVerboseLevel() { return fVerboseLevel; }

private:
  std::string fTemplateName;            //16: 16 characters
  bool fWordFormat;                     //32: enumerator, 0 = byte, 1 = word
  bool fLowBitFirst;                    //34: enumerator, 0 = high first, 1 = low first
  int32_t fWavedescLength;              //offset+36: length in bytes of block WAVEDESC
  int32_t fUsertextLength;              //offset+40: length in bytes of block USERTEXT
  int32_t fTrigtimeLength;              //offset+48: length in bytes of TRIGTIME array
  int32_t fRistimeLength;               //offset+52: length in bytes of RIS_TIME array
  std::string fInstrumentName;          //76: 16 characters
  int32_t fInstrumentNumber;            //92: 
  std::string fTraceLabel;              //96: identifies waveform
  int32_t fPointsPerScreen;             //120: number of data points on screen
  int32_t fFirstValidPoint;             //124: #of points to skip before first good point
  int32_t fLastValidPoint;              //128: last good data point
  int32_t fFirstPoint;                  //132: offset relative to beginning of trace buffer
  int32_t fSparsingFactor;              //136: indicates sparsing into the transmitted data block
  int32_t fSegmentIndex;                //140: index of transmitted segment
  int32_t fSubarrayCount;               //144:
  int32_t fSweepsPerAcquisition;        //148: for average or extrema, the number of sweeps accumulated
  int16_t fPointsPerPair;               //152: for peak detect waveforms the number of data points for each min/max pair
  int16_t fPairOffset;                  //154: for peak detect waveforms the offset of the first min/max pair relative to the first data value in data arary 1
  float   fVerticalGain;                //156: floating values from raw data:
  float   fVerticalOffset;              //160: fVerticalGain*data-fVerticalOffset
  float   fMaxValue;                    //164: maximum allowed value = upper edge of grid
  float   fMinValue;                    //168: minimum allowed value = lower edge of grid
  int16_t fNominalBits;                 //172: 8 bit for adc data, 10-12 bit for averaged data, etc.
  int16_t fNominalSubarrayCount;        //174: for sequence: the nominal segment count, else: 1
  float   fHorizontalInterval;          //176: sampling interval for time domain waveforms
  double  fHorizontalOffset;            //180: seconds between trigger and first data point
  double  fPixelOffset;                 //188: needed to know how to display waveform
  std::string fVerticalUnit;            //196: 48 char: units of vertical axis
  std::string fHorizontalUnit;          //244: 48 char: units of horizontal axis
  float   fHorizontalUncertainty;       //292: uncertainty in seconds
  //time    fTimestamp;                   //296: timestamp: 16 bytes: double seconds (0-59), int8_t minutes (0-59), int8_t hours (0-23), int8_t days (1-31), int8_t months (1-12), int16_t year (0-16k), int16_t unused
  float   fAcquisitionDuration;         //312: duration of acquisition in seconds
  //316: record type (_0 = single sweep, _1 = interleaved, _2 = histogram, _3 = graph, _4 = filter_coeff, _5 = complex, _6 = extrema, _7 = sequence_obsolete, _8 = centered_RIS, _9 = peak_detect)
  //318: processing done (_0 = no process., _1 = fir filter, _2 = interpol., _3 = sparsed, _4 = autoscaled, _5 = no result, _6 = rolling, _7 = cumulative)
  int16_t fRisSweeps;                   //322: for random interleaved sampling (RIS) sweeps: # of sweeps, else: 1
  int16_t fTimebase;                    //324: timebase (1, 2, 5, 10, 20, 50, 100, 200, 500 ps, 1, 2, 5, 10, 20, 50, 100, 200, 500 ns, and so on up to _47 = 5000 s and _100 = external)
  int16_t fVerticalCoupling;            //326: vert. coupling (_0 = DC 50 Ohm, _1 = ground, _2 = DC 1 MOhm, _3 = ground, _4 = AC 1 MOhm)
  float   fProbeAttenuation;            //328: probe att.
  int16_t fFixedVerticalGain;           //332: fixed vert. gain (1, 2, 5, 10, 50, 100, 200, 500 uV, 1, 2, 5, 10, 50, 100, 200, 500 mV, and so on up to _27 = 1 kV)
  bool    fBandwidthLimit;              //334: bandwidth limit (_0 = off, _1 = on)
  float   fVerticalVernier;             //336: vertical vernier (float)
  float   fAcquisitionVerticalOffset;   //340: acq. vert. offset (float)
  int16_t fWaveSource;                  //344: wave source (_0 = chan. 1, _1 = chan. 2, _1 = chan. 3, _3 = chan. 4, _9 = unknown)

  size_t fVerboseLevel;

  ClassDef(LecroyHeader, 2)
};

class Oscilloscope {
public:
  Oscilloscope(size_t verboseLevel = 0){
    fTimeConversion = 1.;
    fDownSampling = 1;
    fSamplingRate = 0.;
    fInvert = false;
    fVerboseLevel = verboseLevel;
    fCurrentSample = 0;
    fNoHeader = false;
    fCsv = false;
	 fHeader.SetVerboseLevel(verboseLevel);
  }
  Oscilloscope(std::string);
  Oscilloscope(const char*);
  ~Oscilloscope(){};

  void TimeConversion(std::string);
  void TimeConversion(const char*);

  void DownSampling(size_t downSampling) {
    fSkippedSamples *= downSampling/fDownSampling;
    fDownSampling = downSampling;
  }
  //sampling rate in unit give by TimeConversion (s by default)
  void SamplingRate(double samplingRate) {
    fSkippedSamples *= fSamplingRate/samplingRate;
    fSamplingRate = samplingRate;
  }
  void Invert(bool invert) {
    fInvert = invert;
  }
  void FullScale(double fullScale) {
    fFullScale = fullScale;
  }
  void Average(bool average) {
    fAverage = average;
  }
  void SkippedSamples(size_t skippedSamples) {
    fSkippedSamples = skippedSamples;
  }
  void NumberOfSamples(size_t numberOfSamples) {
    fNumberOfSamples = numberOfSamples;
  }
  void VerboseLevel(size_t verboseLevel) {
    fVerboseLevel = verboseLevel;
  }
  void NoHeader(bool noHeader) {
    fNoHeader = noHeader;
  }
  void Csv(bool csv) {
    fCsv = csv;
  }
  void Binary(bool binary) {
    fBinary = binary;
  }

  bool ReadFile(std::string);
  bool ReadFile(const char*);
  bool ReadAsciiFile(const char*);
  bool ReadBinaryFile(const char*);

  //return functions of values read from file
  size_t RecordLength() {
    return fRecordLength;
  }
  double SampleInterval() {
    return fSampleInterval;
  }
  size_t TriggerPoint() {
    return fTriggerPoint;
  }
  double TriggerTime() {
    return fTriggerTime;
  }
  double HorizontalOffset() {
    return fHorizontalOffset;
  }
  std::vector<double> Time() {
    return fTime;
  }
  std::vector<double> Voltage() {
    return fVoltage;
  }
  double VoltageError() {
    return fVoltageError;
  }
  double Min() {
    return fMin;
  }
  double Max() {
    return fMax;
  }
  double Integral() {
    return fIntegral;
  }

  TGraphErrors* Graph() {
    return &fGraph;
  }

  size_t DownSampling() {
    return fDownSampling;
  }

  bool NextSample(double& sample) {
    if(fCurrentSample < fVoltage.size()) {
      sample = fVoltage[fCurrentSample];
      ++fCurrentSample;
      return true;
    }
    
    return false;
  }

  LecroyHeader Header() {
	 return fHeader;
  }

private:
  //read from file:
  size_t fRecordLength;
  double fSampleInterval;
  size_t fTriggerPoint;
  double fTriggerTime;
  double fHorizontalOffset;

  std::vector<double> fTime;
  std::vector<double> fVoltage;
  double fVoltageError;

  TGraphErrors fGraph;

  double fMin;
  double fMax;
  double fIntegral;

  size_t fCurrentSample;

  //settings
  bool fInvert;
  double fTimeConversion;
  size_t fDownSampling;
  double fSamplingRate;
  double fFullScale;
  bool fAverage;
  size_t fSkippedSamples;
  size_t fNumberOfSamples;
  bool fNoHeader;
  bool fCsv;
  bool fBinary;

  LecroyHeader fHeader;

  size_t fVerboseLevel;
};

#endif
