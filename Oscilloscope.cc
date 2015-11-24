#include "Oscilloscope.hh"

#include <ios>
#include <iostream>

#include "Utilities.hh"

ClassImp(LecroyHeader)

LecroyHeader::LecroyHeader(const char* buffer) {
  ReadData(buffer);
}

void LecroyHeader::ReadData(const char* buffer) {
  char* data = const_cast<char*>(buffer);
  fTemplateName = data+16;                                                      //16: 16 characters
  fWordFormat  = *(reinterpret_cast<int16_t*>(data+32));                        //32: enumerator, 0 = byte, 1 = word
  fLowBitFirst = *(reinterpret_cast<int16_t*>(data+34));                        //34: enumerator, 0 = high first, 1 = low first
  fWavedescLength = *(reinterpret_cast<int32_t*>(data+36));                     //36: length in bytes of block WAVEDESC
  fUsertextLength = *(reinterpret_cast<int32_t*>(data+40));                     //40: length in bytes of block USERTEXT
  fTrigtimeLength = *(reinterpret_cast<int32_t*>(data+48));                     //48: length in bytes of TRIGTIME array
  fRistimeLength = *(reinterpret_cast<int32_t*>(data+52));                      //52: length in bytes of RIS_TIME array
  fInstrumentName = data+76;                                                    //76: 16 characters
  fInstrumentNumber = *(reinterpret_cast<int32_t*>(data+92));                   //92: 
  fTraceLabel = data+96;                                                        //96: identifies waveform
  fPointsPerScreen = *(reinterpret_cast<int32_t*>(data+120));                   //120: number of data points on screen
  fFirstValidPoint = *(reinterpret_cast<int32_t*>(data+124));                   //124: #of points to skip before first good point
  fLastValidPoint = *(reinterpret_cast<int32_t*>(data+128));                    //128: last good data point
  fFirstPoint = *(reinterpret_cast<int32_t*>(data+132));                        //132: offset relative to beginning of trace buffer
  fSparsingFactor = *(reinterpret_cast<int32_t*>(data+136));                    //136: indicates sparsing into the transmitted data block
  fSegmentIndex = *(reinterpret_cast<int32_t*>(data+140));                      //140: index of transmitted segment
  fSubarrayCount = *(reinterpret_cast<int32_t*>(data+144));                     //144:
  fSweepsPerAcquisition = *(reinterpret_cast<int32_t*>(data+148));              //148: for average or extrema, the number of sweeps accumulated
  fPointsPerPair = *(reinterpret_cast<int16_t*>(data+152));                     //152: for peak detect waveforms the number of data points for each min/max pair
  fPairOffset = *(reinterpret_cast<int16_t*>(data+154));                        //154: for peak detect waveforms the offset of the first min/max pair relative to the first data value in data arary 1
  fVerticalGain = *(reinterpret_cast<float*>(data+156));                        //156: floating values from raw data:
  fVerticalOffset = *(reinterpret_cast<float*>(data+160));                      //160: fVerticalGain*data-fVerticalOffset
  fMaxValue = *(reinterpret_cast<float*>(data+164));                            //164: maximum allowed value = upper edge of grid
  fMinValue = *(reinterpret_cast<float*>(data+168));                            //168: minimum allowed value = lower edge of grid
  fNominalBits = *(reinterpret_cast<int16_t*>(data+172));                       //172: 8 bit for adc data, 10-12 bit for averaged data, etc.
  fNominalSubarrayCount = *(reinterpret_cast<int16_t*>(data+174));              //174: for sequence: the nominal segment count, else: 1
  fHorizontalInterval = *(reinterpret_cast<float*>(data+176));                  //176: sampling interval for time domain waveforms
  fHorizontalOffset = *(reinterpret_cast<double*>(data+180));                   //180: seconds between trigger and first data point
  fPixelOffset = *(reinterpret_cast<double*>(data+188));                        //188: needed to know how to display waveform
  fVerticalUnit = data+196;                                                     //196: 48 char: units of vertical axis
  fHorizontalUnit = data+244;                                                   //244: 48 char: units of horizontal axis
  fHorizontalUncertainty = *(reinterpret_cast<float*>(data+292));               //292: uncertainty in seconds
  //296: timestamp: 16 bytes: double seconds (0-59), int8_t minutes (0-59), int8_t hours (0-23), int8_t days (1-31), int8_t months (1-12), int16_t year (0-16k), int16_t unused
  fAcquisitionDuration = *(reinterpret_cast<float*>(data+312));                 //312: duration of acquisition in seconds
  //316: record type (_0 = single sweep, _1 = interleaved, _2 = histogram, _3 = graph, _4 = filter_coeff, _5 = complex, _6 = extrema, _7 = sequence_obsolete, _8 = centered_RIS, _9 = peak_detect)
  //318: processing done (_0 = no process., _1 = fir filter, _2 = interpol., _3 = sparsed, _4 = autoscaled, _5 = no result, _6 = rolling, _7 = cumulative)
  fRisSweeps = *(reinterpret_cast<int16_t*>(data+322));                         //322: for random interleaved sampling (RIS) sweeps: # of sweeps, else: 1
  fTimebase = *(reinterpret_cast<int16_t*>(data+324));                          //324: timebase (1, 2, 5, 10, 20, 50, 100, 200, 500 ps, 1, 2, 5, 10, 20, 50, 100, 200, 500 ns, and so on up to _47 = 5000 s and _100 = external)
  fVerticalCoupling = *(reinterpret_cast<int16_t*>(data+326));                  //326: vert. coupling (_0 = DC 50 Ohm, _1 = ground, _2 = DC 1 MOhm, _3 = ground, _4 = AC 1 MOhm)
  fProbeAttenuation = *(reinterpret_cast<float*>(data+328));                    //328: probe att.
  fFixedVerticalGain = *(reinterpret_cast<int16_t*>(data+332));                 //332: fixed vert. gain (1, 2, 5, 10, 50, 100, 200, 500 uV, 1, 2, 5, 10, 50, 100, 200, 500 mV, and so on up to _27 = 1 kV)
  fBandwidthLimit = *(reinterpret_cast<int16_t*>(data+334));                    //334: bandwidth limit (_0 = off, _1 = on)
  fVerticalVernier = *(reinterpret_cast<float*>(data+336));                     //336: vertical vernier (float)
  fAcquisitionVerticalOffset = *(reinterpret_cast<float*>(data+340));           //340: acq. vert. offset (float)
  fWaveSource = *(reinterpret_cast<int16_t*>(data+344));                        //344: wave source (_0 = chan. 1, _1 = chan. 2, _1 = chan. 3, _3 = chan. 4, _9 = unknown)

  if(fVerboseLevel > 1) {
	 Print();
  }
}

void LecroyHeader::Print(Option_t* opt) const {
  std::cout<<"template name: "<<fTemplateName<<std::endl
			  <<"word format: "<<fWordFormat<<std::endl
			  <<"bit order: "<<fLowBitFirst<<std::endl
			  <<"wavedesc length: "<<fWavedescLength<<std::endl
			  <<"usertext length: "<<fUsertextLength<<std::endl
			  <<"trigtime length: "<<fTrigtimeLength<<std::endl
			  <<"ristime length: "<<fRistimeLength<<std::endl
			  <<"instrument name: "<<fInstrumentName<<std::endl
			  <<"instrument number: "<<fInstrumentNumber<<std::endl
			  <<"trace label: "<<fTraceLabel<<std::endl
			  <<"points per screen: "<<fPointsPerScreen<<std::endl
			  <<"first valid point: "<<fFirstValidPoint<<std::endl
			  <<"last valid point: "<<fLastValidPoint<<std::endl
			  <<"first point: "<<fFirstPoint<<std::endl
			  <<"sparsing factor: "<<fSparsingFactor<<std::endl
			  <<"segment indice: "<<fSegmentIndex<<std::endl
			  <<"subarray count: "<<fSubarrayCount<<std::endl
			  <<"sweeps/acquisition: "<<fSweepsPerAcquisition<<std::endl
			  <<"points/pair: "<<fPointsPerPair<<std::endl
			  <<"pair offset: "<<fPairOffset<<std::endl
			  <<"vertical gain: "<<fVerticalGain<<std::endl
			  <<"vertical offset: "<<fVerticalOffset<<std::endl
			  <<"max. value: "<<fMaxValue<<std::endl
			  <<"min. value: "<<fMinValue<<std::endl
			  <<"nominal bit: "<<fNominalBits<<std::endl
			  <<"nominal subarray count: "<<fNominalSubarrayCount<<std::endl
			  <<"horizontal interval: "<<fHorizontalInterval<<std::endl
			  <<"horizontal offset: "<<fHorizontalOffset<<std::endl
			  <<"pixel offset: "<<fPixelOffset<<std::endl
			  <<"vertial unit: "<<fVerticalUnit<<std::endl
			  <<"horizontal unit: "<<fHorizontalUnit<<std::endl
			  <<"horizontal uncertaintie: "<<fHorizontalUncertainty<<std::endl
			  <<"acquisition duration: "<<fAcquisitionDuration<<std::endl
			  <<"RIS sweep: "<<fRisSweeps<<std::endl
			  <<"timebase: "<<fTimebase<<std::endl
			  <<"vertical coupling: "<<fVerticalCoupling<<std::endl
			  <<"probe attenuation: "<<fProbeAttenuation<<std::endl
			  <<"fixed vertical gain: "<<fFixedVerticalGain<<std::endl
			  <<"bandwidth limit: "<<fBandwidthLimit<<std::endl
			  <<"vertical vernier: "<<fVerticalVernier<<std::endl
			  <<"acquisition vertical offset: "<<fAcquisitionVerticalOffset<<std::endl
			  <<"wave source: "<<fWaveSource<<std::endl;
}

bool LecroyHeader::operator==(const LecroyHeader& rhs) const {
  //the string comparisons are all moved to the end
  if(fWordFormat                != rhs.fWordFormat               ) return false;
  if(fLowBitFirst               != rhs.fLowBitFirst              ) return false;
  if(fWavedescLength            != rhs.fWavedescLength           ) return false;
  if(fUsertextLength            != rhs.fUsertextLength           ) return false;
  if(fTrigtimeLength            != rhs.fTrigtimeLength           ) return false;
  if(fRistimeLength             != rhs.fRistimeLength            ) return false;
  if(fInstrumentNumber          != rhs.fInstrumentNumber         ) return false;
  if(fPointsPerScreen           != rhs.fPointsPerScreen          ) return false;
  if(fFirstValidPoint           != rhs.fFirstValidPoint          ) return false;
  if(fLastValidPoint            != rhs.fLastValidPoint           ) return false;
  if(fFirstPoint                != rhs.fFirstPoint               ) return false;
  if(fSparsingFactor            != rhs.fSparsingFactor           ) return false;
  if(fSegmentIndex              != rhs.fSegmentIndex             ) return false;
  if(fSubarrayCount             != rhs.fSubarrayCount            ) return false;
  if(fSweepsPerAcquisition      != rhs.fSweepsPerAcquisition     ) return false;
  if(fPointsPerPair             != rhs.fPointsPerPair            ) return false;
  if(fPairOffset                != rhs.fPairOffset               ) return false;
  if(fVerticalGain              != rhs.fVerticalGain             ) return false;
  if(fVerticalOffset            != rhs.fVerticalOffset           ) return false;
  if(fMaxValue                  != rhs.fMaxValue                 ) return false;
  if(fMinValue                  != rhs.fMinValue                 ) return false;
  if(fNominalBits               != rhs.fNominalBits              ) return false;
  if(fNominalSubarrayCount      != rhs.fNominalSubarrayCount     ) return false;
  if(fHorizontalInterval        != rhs.fHorizontalInterval       ) return false;
  //if(fHorizontalOffset          != rhs.fHorizontalOffset         ) return false; //seems to change all the time, so it's disabled fror now
  if(fPixelOffset               != rhs.fPixelOffset              ) return false;
  if(fHorizontalUncertainty     != rhs.fHorizontalUncertainty    ) return false;
  //if(fTimestamp                 != rhs.fTimestamp                ) return false;
  if(fAcquisitionDuration       != rhs.fAcquisitionDuration      ) return false;
  if(fRisSweeps                 != rhs.fRisSweeps                ) return false;
  if(fTimebase                  != rhs.fTimebase                 ) return false;
  if(fVerticalCoupling          != rhs.fVerticalCoupling         ) return false;
  if(fProbeAttenuation          != rhs.fProbeAttenuation         ) return false;
  if(fFixedVerticalGain         != rhs.fFixedVerticalGain        ) return false;
  if(fBandwidthLimit            != rhs.fBandwidthLimit           ) return false;
  if(fVerticalVernier           != rhs.fVerticalVernier          ) return false;
  if(fAcquisitionVerticalOffset != rhs.fAcquisitionVerticalOffset) return false;
  if(fWaveSource                != rhs.fWaveSource               ) return false;

  if(fTemplateName.compare(rhs.fTemplateName) != 0) return false;
  if(fInstrumentName.compare(rhs.fInstrumentName) != 0) return false;
  if(fTraceLabel.compare(rhs.fTraceLabel) != 0) return false;
  if(fVerticalUnit.compare(rhs.fVerticalUnit) != 0) return false;
  if(fHorizontalUnit.compare(rhs.fHorizontalUnit) != 0) return false;

  return true;
}

void LecroyHeader::Diff(const LecroyHeader& rhs) const {
  if(fTemplateName.compare(rhs.fTemplateName)                != 0) std::cout<<"template names differ: "<<fTemplateName<<" != "<<rhs.fTemplateName<<std::endl;
  if(fWordFormat                != rhs.fWordFormat               ) std::cout<<"word formats differ: "<<fWordFormat<<" != "<<rhs.fWordFormat<<std::endl;
  if(fLowBitFirst               != rhs.fLowBitFirst              ) std::cout<<"bit orders differ: "<<fLowBitFirst<<" != "<<rhs.fLowBitFirst<<std::endl;
  if(fWavedescLength            != rhs.fWavedescLength           ) std::cout<<"wavedesc lengths differ: "<<fWavedescLength<<" != "<<rhs.fWavedescLength<<std::endl;
  if(fUsertextLength            != rhs.fUsertextLength           ) std::cout<<"usertext lengths differ: "<<fUsertextLength<<" != "<<rhs.fUsertextLength<<std::endl;
  if(fTrigtimeLength            != rhs.fTrigtimeLength           ) std::cout<<"trigtime lengths differ: "<<fTrigtimeLength<<" != "<<rhs.fTrigtimeLength<<std::endl;
  if(fRistimeLength             != rhs.fRistimeLength            ) std::cout<<"ristime lengths differ: "<<fRistimeLength<<" != "<<rhs.fRistimeLength<<std::endl;
  if(fInstrumentName.compare(rhs.fInstrumentName)            != 0) std::cout<<"instrument names differ: "<<fInstrumentName<<" != "<<rhs.fInstrumentName<<std::endl;
  if(fInstrumentNumber          != rhs.fInstrumentNumber         ) std::cout<<"instrument numbers differ: "<<fInstrumentNumber<<" != "<<rhs.fInstrumentNumber<<std::endl;
  if(fTraceLabel.compare(rhs.fTraceLabel)                    != 0) std::cout<<"trace labels differ: "<<fTraceLabel<<" != "<<rhs.fTraceLabel<<std::endl;
  if(fPointsPerScreen           != rhs.fPointsPerScreen          ) std::cout<<"points per screen differ: "<<fPointsPerScreen<<" != "<<rhs.fPointsPerScreen<<std::endl;
  if(fFirstValidPoint           != rhs.fFirstValidPoint          ) std::cout<<"first valid points differ: "<<fFirstValidPoint<<" != "<<rhs.fFirstValidPoint<<std::endl;
  if(fLastValidPoint            != rhs.fLastValidPoint           ) std::cout<<"last valid points differ: "<<fLastValidPoint<<" != "<<rhs.fLastValidPoint<<std::endl;
  if(fFirstPoint                != rhs.fFirstPoint               ) std::cout<<"first points differ: "<<fFirstPoint<<" != "<<rhs.fFirstPoint<<std::endl;
  if(fSparsingFactor            != rhs.fSparsingFactor           ) std::cout<<"sparsing factors differ: "<<fSparsingFactor<<" != "<<rhs.fSparsingFactor<<std::endl;
  if(fSegmentIndex              != rhs.fSegmentIndex             ) std::cout<<"segment indices differ: "<<fSegmentIndex<<" != "<<rhs.fSegmentIndex<<std::endl;
  if(fSubarrayCount             != rhs.fSubarrayCount            ) std::cout<<"subarray counts differ: "<<fSubarrayCount<<" != "<<rhs.fSubarrayCount<<std::endl;
  if(fSweepsPerAcquisition      != rhs.fSweepsPerAcquisition     ) std::cout<<"sweeps/acquisition differ: "<<fSweepsPerAcquisition<<" != "<<rhs.fSweepsPerAcquisition<<std::endl;
  if(fPointsPerPair             != rhs.fPointsPerPair            ) std::cout<<"points/pair differ: "<<fPointsPerPair<<" != "<<rhs.fPointsPerPair<<std::endl;
  if(fPairOffset                != rhs.fPairOffset               ) std::cout<<"pair offsets differ: "<<fPairOffset<<" != "<<rhs.fPairOffset<<std::endl;
  if(fVerticalGain              != rhs.fVerticalGain             ) std::cout<<"vertical gains differ: "<<fVerticalGain<<" != "<<rhs.fVerticalGain<<std::endl;
  if(fVerticalOffset            != rhs.fVerticalOffset           ) std::cout<<"vertical offsets differ: "<<fVerticalOffset<<" != "<<rhs.fVerticalOffset<<std::endl;
  if(fMaxValue                  != rhs.fMaxValue                 ) std::cout<<"max. values differ: "<<fMaxValue<<" != "<<rhs.fMaxValue<<std::endl;
  if(fMinValue                  != rhs.fMinValue                 ) std::cout<<"min. values differ: "<<fMinValue<<" != "<<rhs.fMinValue<<std::endl;
  if(fNominalBits               != rhs.fNominalBits              ) std::cout<<"nominal bits differ: "<<fNominalBits<<" != "<<rhs.fNominalBits<<std::endl;
  if(fNominalSubarrayCount      != rhs.fNominalSubarrayCount     ) std::cout<<"nominal subarray counts differ: "<<fNominalSubarrayCount<<" != "<<rhs.fNominalSubarrayCount<<std::endl;
  if(fHorizontalInterval        != rhs.fHorizontalInterval       ) std::cout<<"horizontal intervals differ: "<<fHorizontalInterval<<" != "<<rhs.fHorizontalInterval<<std::endl;
  if(fHorizontalOffset          != rhs.fHorizontalOffset         ) std::cout<<"horizontal offsets differ: "<<fHorizontalOffset<<" != "<<rhs.fHorizontalOffset<<std::endl;
  if(fPixelOffset               != rhs.fPixelOffset              ) std::cout<<"pixel offsets differ: "<<fPixelOffset<<" != "<<rhs.fPixelOffset<<std::endl;
  if(fVerticalUnit.compare(rhs.fVerticalUnit)                != 0) std::cout<<"vertial units differ: "<<fVerticalUnit<<" != "<<rhs.fVerticalUnit<<std::endl;
  if(fHorizontalUnit.compare(rhs.fHorizontalUnit)            != 0) std::cout<<"horizontal units differ: "<<fHorizontalUnit<<" != "<<rhs.fHorizontalUnit<<std::endl;
  if(fHorizontalUncertainty     != rhs.fHorizontalUncertainty    ) std::cout<<"horizontal uncertainties differ: "<<fHorizontalUncertainty<<" != "<<rhs.fHorizontalUncertainty<<std::endl;
  if(fAcquisitionDuration       != rhs.fAcquisitionDuration      ) std::cout<<"acquisition durations differ: "<<fAcquisitionDuration<<" != "<<rhs.fAcquisitionDuration<<std::endl;
  if(fRisSweeps                 != rhs.fRisSweeps                ) std::cout<<"RIS sweeps differ: "<<fRisSweeps<<" != "<<rhs.fRisSweeps<<std::endl;
  if(fTimebase                  != rhs.fTimebase                 ) std::cout<<"timebases differ: "<<fTimebase<<" != "<<rhs.fTimebase<<std::endl;
  if(fVerticalCoupling          != rhs.fVerticalCoupling         ) std::cout<<"vertical couplings differ: "<<fVerticalCoupling<<" != "<<rhs.fVerticalCoupling<<std::endl;
  if(fProbeAttenuation          != rhs.fProbeAttenuation         ) std::cout<<"probe attenuations differ: "<<fProbeAttenuation<<" != "<<rhs.fProbeAttenuation<<std::endl;
  if(fFixedVerticalGain         != rhs.fFixedVerticalGain        ) std::cout<<"fixed vertical gains differ: "<<fFixedVerticalGain<<" != "<<rhs.fFixedVerticalGain<<std::endl;
  if(fBandwidthLimit            != rhs.fBandwidthLimit           ) std::cout<<"bandwidth limits differ: "<<fBandwidthLimit<<" != "<<rhs.fBandwidthLimit<<std::endl;
  if(fVerticalVernier           != rhs.fVerticalVernier          ) std::cout<<"vertical verniers differ: "<<fVerticalVernier<<" != "<<rhs.fVerticalVernier<<std::endl;
  if(fAcquisitionVerticalOffset != rhs.fAcquisitionVerticalOffset) std::cout<<"acquisition vertical offsets differ: "<<fAcquisitionVerticalOffset<<" != "<<rhs.fAcquisitionVerticalOffset<<std::endl;
  if(fWaveSource                != rhs.fWaveSource               ) std::cout<<"wave sources differ: "<<fWaveSource<<" != "<<rhs.fWaveSource<<std::endl;
}

Oscilloscope::Oscilloscope(std::string fileName) {
  fTimeConversion = 1.;
  fDownSampling = 1;
  fSamplingRate = 0.;
  fInvert = false;
  fFullScale = 10.;
  fAverage = false;
  fNumberOfSamples = 0;
  fCurrentSample = 0;
  fNoHeader = false;
  fCsv = false;
  fBinary = false;

  ReadFile(fileName.c_str());
}

Oscilloscope::Oscilloscope(const char* fileName) {
  fTimeConversion = 1.;
  fDownSampling = 1;
  fSamplingRate = 0.;
  fInvert = false;
  fFullScale = 10.;
  fAverage = false;
  fNumberOfSamples = 0;
  fCurrentSample = 0;
  fNoHeader = false;
  fCsv = false;
  fBinary = false;

  ReadFile(fileName);
}

void Oscilloscope::TimeConversion(const char* conv) {
  TimeConversion(std::string(conv));
}

void Oscilloscope::TimeConversion(std::string conv) {
  std::transform(conv.begin(), conv.end(), conv.begin(), ::tolower);

  if(conv.compare("s") == 0) {
    fTimeConversion = 1.;
    return;
  } else if(conv.compare("ms") == 0) {
    fTimeConversion = 1e3;
    return;
  } else if(conv.compare("us") == 0) {
    fTimeConversion = 1e6;
    return;
  } else if(conv.compare("ns") == 0) {
    fTimeConversion = 1e9;
    return;
  } else if(conv.compare("ps") == 0) {
    fTimeConversion = 1e12;
    return;
  }

  std::cerr<<"Sorry, I don't know the unit '"<<conv<<"'"<<std::endl;
  return;
}

bool Oscilloscope::ReadFile(std::string fileName) {
  return ReadFile(fileName.c_str());
}

bool Oscilloscope::ReadFile(const char* fileName) {
  if(fBinary) {
	 return ReadBinaryFile(fileName);
  }
  return ReadAsciiFile(fileName);
}

bool Oscilloscope::ReadAsciiFile(const char* fileName) {
  if(!FileExists(fileName)) {
    std::cerr<<"file '"<<fileName<<"' doesn't exist???"<<std::endl;
    return false;
  }

  std::ifstream file(fileName);

  std::string line;
  std::stringstream stringStream(std::stringstream::in | std::stringstream::out);

  char tmpString[16];
  double tmpDouble;

  fTime.clear();
  fVoltage.clear();

  fCurrentSample = 0;

  //according to the datasheet the scope uses an 8-bit ADC and has an accuracy of 1% of the full scale (which is not recorded in the file)
  fVoltageError = 0.01*fFullScale;

  fMin = 1e9;
  fMax = -1e9;

  if(fVerboseLevel > 2) {
    std::cout<<"starting to read file '"<<fileName<<"'"<<std::endl;
  }

  size_t sample = 0;

  if(fCsv) {
    for(size_t i = 0; i < 5; ++i) {
		std::getline(file,line);
    }
  }

  while(std::getline(file,line)) {
    if(fVerboseLevel > 3) {
      std::cout<<"starting on "<<sample<<"/"<<fSkippedSamples<<" line '"<<line<<"'"<<std::endl;
    }
    stringStream.clear();
    if(line.find("\"Record Length\"") != std::string::npos) {
      //find the second occurence of " in the std::string and create a sub-std::string from here on
      stringStream.str(line.substr(line.find('"',line.find('"')+1)+1));
      stringStream>>fRecordLength;
      //skip the 'unit' (should be "Points")
      stringStream>>tmpString;
    } else if(line.find("\"Sample Interval\"") != std::string::npos) {
      //find the second occurence of " in the std::string and create a sub-std::string from here on
      stringStream.str(line.substr(line.find('"',line.find('"')+1)+1));
      stringStream>>fSampleInterval;
      if(fVerboseLevel > 3) {
	std::cout<<"read sample interval = "<<fSampleInterval;
      }
      fSampleInterval *= fTimeConversion;
      if(fVerboseLevel > 3) {
	std::cout<<" => "<<fSampleInterval<<std::endl;
      }
      //skip the 'unit' (should be s)
      stringStream>>tmpString;
    } else if(line.find("\"Trigger Point\"") != std::string::npos) {
      stringStream.str(line.substr(line.find('"',line.find('"')+1)+1));
      stringStream>>tmpDouble;
      //find the second occurence of " in the std::string and create a sub-std::string from here on
      stringStream.str(line.substr(line.find('"',line.find('"')+1)+1));
      stringStream>>fTriggerPoint;
      //skip the 'unit' (should be "Samples")
      stringStream>>tmpString;
      if(fVerboseLevel > 3) {
	std::cout<<"found trigger point at "<<fTriggerPoint<<" samples, now at "<<stringStream.tellg()<<" in line '"<<stringStream.str()<<"'"<<std::endl;
      }
    } else if(line.find("\"Trigger Time\"") != std::string::npos) {
      //find the second occurence of " in the std::string and create a sub-std::string from here on
      stringStream.str(line.substr(line.find('"',line.find('"')+1)+1));
      stringStream>>fTriggerTime;
      //skip the 'unit' (should be s)
      stringStream>>tmpString;
    } else if(line.find("\"Horizontal Offset\"") != std::string::npos) {
      //find the second occurence of " in the std::string and create a sub-std::string from here on
      stringStream.str(line.substr(line.find('"',line.find('"')+1)+1));
      stringStream>>fHorizontalOffset;
      fHorizontalOffset *= fTimeConversion;
      //skip the 'unit' (should be s)
      stringStream>>tmpString;
    } else {
      if(fCsv) {
	line.replace(line.find(','),1," ");
      }
      stringStream.str(line);
    }
    if(sample < fSkippedSamples) {
      if(fVerboseLevel > 3) {
	std::cout<<"skipping sample "<<sample<<"/"<<fSkippedSamples<<std::endl;
      }
      ++sample;
      continue;
    }
    //read the two x and y values
    stringStream>>tmpDouble;
    fTime.push_back(tmpDouble);
    stringStream>>tmpDouble;
    if(fInvert) {
      fVoltage.push_back(-tmpDouble);
    } else {
      fVoltage.push_back(tmpDouble);
    }
    if(fVerboseLevel > 3) {
      std::cout<<fTime.size()<<": time = "<<fTime.back()<<", voltage = "<<fVoltage.back()<<std::endl;
    }
    if(fVoltage.back() < fMin) {
      fMin = fVoltage.back();
    }
    if(fVoltage.back() > fMax) {
      fMax = fVoltage.back();
    }
    ++sample;
  }

  file.close();

  //if there is no header in the file we need to calculate
  //fSampleInterval and fRecordLength
  if(fNoHeader || fCsv) {
    fRecordLength = fTime.size();
    if(fTime.size() > 1) {
      fSampleInterval = (fTime[1] - fTime[0])*fTimeConversion;
      if(fVerboseLevel > 3) {
	std::cout<<"calculated sample interval = "<<fSampleInterval;
      }
    } else {
      std::cerr<<"Warning, no header and only one sample, can't calculate a sample interval, setting it to 1!"<<std::endl;
      fSampleInterval = 1.;
    }
  }

  if(fSamplingRate != 0.) {
    if(fDownSampling != 0 && fDownSampling != 1) {
      if(std::abs(fSamplingRate/fSampleInterval - (double) fDownSampling) > fSampleInterval/fRecordLength) {
	std::cerr<<"Warning, the sampling rate you wish to have ("<<fSamplingRate<<") doesn't seem to match down-sampling you want ("<<fDownSampling<<"), going to re-calculate the down-sampling factor"<<std::endl;
      }
    }
    //(re-)calculate the down-sampling factor
    fDownSampling = (size_t) round(fSamplingRate/fSampleInterval);
    if(std::abs(fSamplingRate/fSampleInterval - (double) fDownSampling) > fSampleInterval/fRecordLength) {
      std::cerr<<"Warning, the sampling rate you wish to have ("<<fSamplingRate<<") doesn't seem to be a multiple of the sampling interval of the data ("<<fSampleInterval<<"), you'll have an effective sampling rate of "<<fDownSampling*fSampleInterval<<" (fDownSampling = "<<fDownSampling<<" = "<<fSamplingRate/fSampleInterval<<")"<<std::endl;
    }
  }

  fTime[0] *= fTimeConversion;
  for(size_t i = 1; i < fTime.size(); ++i) {
    fTime[i] *= fTimeConversion;
    fTime[i] -= fTime[0];
  }
  fTime[0] = 0.;

  //if we have a header the recordlength should be the original record length including all skippes samples
  if(fTime.size() != fRecordLength-fSkippedSamples && !fNoHeader && !fCsv) {
    std::cerr<<"Error, should have a record length of "<<fRecordLength-fSkippedSamples<<" points, but read "<<fTime.size()<<" lines!"<<std::endl;
    return false;
  }

  //do the down-sampling (the error is constant => needs no shuffling)
  if(fDownSampling > 1) {
    tmpDouble = 0.;
    for(size_t i = 0; i < fTime.size(); ++i) {
      if(fAverage) {
	tmpDouble += fVoltage[i];
      }
      if(i%fDownSampling == 0) {
	fTime[i/fDownSampling] = fTime[i];
	if(fAverage) {
	  fVoltage[i/fDownSampling] = tmpDouble/fDownSampling;
	  if(fVerboseLevel > 3) {
	    std::cout<<"set fVoltage["<<i/fDownSampling<<"] = "<<fVoltage[i/fDownSampling]<<" = "<<tmpDouble<<"/"<<fDownSampling<<std::endl;
	  }
	  tmpDouble = 0.;
	} else {
	  fVoltage[i/fDownSampling] = fVoltage[i];
	  if(fVerboseLevel > 3) {
	    std::cout<<"set fVoltage["<<i/fDownSampling<<"] = "<<fVoltage[i/fDownSampling]<<" = "<<fVoltage[i]<<std::endl;
	  }
	}
      }
    }
    //if we're averaging over fDownSampling samples, the error is smaller by sqrt(fDownSampling)
    fVoltageError /= sqrt(fDownSampling);
    //now we have up to (fTime.size()-1)/fDownSampling+1 copied samples, we only need to resize the vectors
    fTime.resize((fTime.size()-1)/fDownSampling+1);
    fVoltage.resize((fVoltage.size()-1)/fDownSampling+1);
    if(fVerboseLevel > 2) {
      std::cout<<"resized fVoltage to "<<fVoltage.size()<<std::endl;
    }
  }

  if(fTime.size() > fNumberOfSamples && fNumberOfSamples > 0) {
    fTime.resize(fNumberOfSamples);
    fVoltage.resize(fNumberOfSamples);

    if(fVerboseLevel > 2) {
      std::cout<<"resized fVoltage to "<<fVoltage.size()<<" = "<<fNumberOfSamples<<std::endl;
    }
  }

  fIntegral = std::accumulate(fVoltage.begin(),fVoltage.end(),0.);

  if(fVerboseLevel > 2) {
    std::cout<<"integral = "<<fIntegral<<std::endl;
  }

  fGraph.Set(fTime.size());

  for(size_t i = 0; i < fTime.size(); ++i) {
    fGraph.SetPoint(i,fTime[i],fVoltage[i]);
    fGraph.SetPointError(i,0.,fVoltageError);
  }
  
  return true;
}

bool Oscilloscope::ReadBinaryFile(const char* fileName) {
  if(!FileExists(fileName)) {
    std::cerr<<"file '"<<fileName<<"' doesn't exist???"<<std::endl;
    return false;
  }

  fTime.clear();
  fVoltage.clear();

  fCurrentSample = 0;

  //according to the datasheet the scope uses an 8-bit ADC and has an accuracy of 1% of the full scale (which is not recorded in the file)
  fVoltageError = 0.01*fFullScale;

  fMin = 1e9;
  fMax = -1e9;

  if(fVerboseLevel > 2) {
    std::cout<<"starting to read file '"<<fileName<<"'"<<std::endl;
  }

  std::ifstream infile(fileName, std::ifstream::binary | std::ifstream::in);

  infile.seekg(0, infile.end);
  size_t fileSize = infile.tellg();
  infile.seekg(0, infile.beg);

  std::vector<char> buffer(fileSize);
  if(!infile.read(buffer.data(), fileSize)) {
  	 std::cerr<<"failed to read file '"<<fileName<<"'"<<std::endl;
  	 return false;
  }

  //floating point: -1^(bit 31) * 2^(bits 30-23 - 127) * (bits 22-0 + 1), double precision: -1^(bit 63) * 2^(bits 62-52 - 1023) * (bits 51-0 + 1)

  //first find the offset by searching for 'WAVEDESC'
  std::string str(buffer.data(),64);//should be in the first 64 characters
  size_t offset = str.find("WAVEDESC");
  
  if(fVerboseLevel > 2) {
    std::cout<<"found offset "<<offset<<std::endl;
  }
  if(offset == std::string::npos) {
	 std::cerr<<"failed to find offset"<<std::endl;
	 return false;
  }

  fHeader.ReadData(buffer.data()+offset);

  if(fVerboseLevel > 2) {
	 std::cout<<"commType = "<<fHeader.GetWordFormat()<<", commOrder = "<<fHeader.GetLowBitFirst()<<std::endl
				 <<"voltage gain = "<<fHeader.GetVerticalGain()<<","<<std::endl
				 <<" voltage offset = "<<fHeader.GetVerticalOffset()<<","<<std::endl
				 <<" samplingRate = "<<fHeader.GetHorizontalInterval()<<","<<std::endl
				 <<" time offset = "<<fHeader.GetHorizontalOffset()<<","<<std::endl
				 <<" time uncertainty = "<<fHeader.GetHorizontalUncertainty()<<","<<std::endl
				 <<" time/Div = "<<fHeader.GetTimebase()<<","<<std::endl
				 <<" voltage/Div = "<<fHeader.GetFixedVerticalGain()<<std::endl;
  }

  //userblock
  //trigger time array: 0: trigger time (double), 8: trigger offset (double) all in seconds
  //random interleaved sampling (RIS) time array: 0: ris offset (double) in seconds
  //data array 1 (always there)
  //data array 2 (only for complex FFT, extrema, or peak detect)
  //simple array (indentical to data arary 1
  //dual array (indentical to data array 1 followed by data array 2)

  size_t i = offset+fHeader.GetWavedescLength()+fHeader.GetUsertextLength()+fHeader.GetTrigtimeLength()+fHeader.GetRistimeLength();

  if(fVerboseLevel > 2) {
	 std::cout<<"reading waveform from byte "<<i<<" = 0x"<<std::hex<<std::setw(8)<<std::setfill('0')<<i<<std::dec<<std::setfill(' ')<<" ("<<offset<<"+"<<fHeader.GetWavedescLength()<<"+"<<fHeader.GetUsertextLength()<<"+"<<fHeader.GetTrigtimeLength()<<"+"<<fHeader.GetRistimeLength()<<") to byte "<<fileSize<<" => "<<fileSize-i<<" bytes"<<std::endl;
  }

  double time = fHeader.GetHorizontalOffset();
  while(true) {
	 if(fHeader.GetWordFormat()) {
		if(i+2 <= fileSize) {
		  fVoltage.push_back(fHeader.GetVerticalGain()*static_cast<double>(*(reinterpret_cast<int16_t*>(buffer.data()+i))) - fHeader.GetVerticalOffset());
		} else {
		  break;
		}
		i += 2;
	 } else {
		if(i+1 <= fileSize) {
		  fVoltage.push_back(fHeader.GetVerticalGain()*static_cast<double>(*(reinterpret_cast<int8_t*>(buffer.data()+i))) - fHeader.GetVerticalOffset());
		} else {
		  break;
		}
		i += 1;
	 }
	 fTime.push_back(time*1e9);
	 if(fVerboseLevel > 3) {
		std::cout<<"time "<<time<<", voltage "<<fVoltage.back()<<std::endl;
	 }
	 time += fHeader.GetHorizontalInterval();
  }

  if(fVerboseLevel > 2) {
	 std::cout<<"read "<<fVoltage.size()<<" voltages"<<std::endl;
  }
  
  fGraph.Set(fTime.size());

  for(size_t i = 0; i < fTime.size(); ++i) {
    fGraph.SetPoint(i,fTime[i],fVoltage[i]);
    fGraph.SetPointError(i,fHeader.GetHorizontalUncertainty(),fVoltageError);
  }
  
  return true;
}
