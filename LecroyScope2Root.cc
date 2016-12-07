#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TH2I.h"

#include "CommandLineInterface.hh"
#include "Utilities.hh"
#include "Oscilloscope.hh"

int main(int argc, char** argv) {
	CommandLineInterface interface;
	std::vector<std::string> inputFileNames;
	interface.Add("-if", "input file name(s) or single (!) input directory (required)", &inputFileNames);
	std::string outputFileName;
	interface.Add("-of", "output file name (required)", &outputFileName);
	uint16_t verboseLevel = 0;
	interface.Add("-vl","set the level of verbosity", &verboseLevel);

	interface.CheckFlags(argc, argv);

	if(inputFileNames.size() == 0 || outputFileName.empty()) {
		std::cerr<<"You need to provide at least one input ("<<inputFileNames.size()<<" provided) and an output file ("<<outputFileName<<")!"<<std::endl;
		return 1;
	}

	if(inputFileNames.size() == 1 && IsDirectory(inputFileNames[0])) {
		if(inputFileNames[0].back() != '/') {
			inputFileNames[0].append("/");
		}
		if(verboseLevel > 0) {
			std::cout<<"From directory '"<<inputFileNames[0]<<"', got ";
		}
		inputFileNames = GetFilesInDirectory(inputFileNames[0]);
		if(verboseLevel > 0) {
			std::cout<<inputFileNames.size()<<" files"<<std::endl;
			if(verboseLevel > 2) {
				for(size_t file = 0; file < inputFileNames.size() - 1; ++file) {
					std::cout<<inputFileNames[file]<<", ";
				}
				std::cout<<inputFileNames.back()<<std::endl;
			}
		}
	}

	Oscilloscope scope(verboseLevel);
	scope.TimeConversion("ns");
	scope.Binary(true);

	//-------------------- output file --------------------
	if(verboseLevel > 0) {
		std::cout<<"opening "<<outputFileName<<std::endl;
	}

	TFile outputFile(outputFileName.c_str(),"recreate");

	if(outputFile.IsZombie()) {
		return 1;
	}

	//-------------------- output tree for fit results
	TTree tree("tree","scope waveforms");

	int bufferSize = 1024000;

	//leafs
	std::vector<double> time;
	std::vector<double> voltage;
	TGraphErrors* graph = nullptr;

	tree.Branch("time",&time,bufferSize);
	tree.Branch("voltage",&voltage,bufferSize);
	tree.Branch("graph",&graph,bufferSize);

	tree.BranchRef();

	LecroyHeader header;

	Long64_t GByte = 1073741824L;
	tree.SetMaxTreeSize(10*GByte);

	//-------------------- histograms --------------------
	//TH2I* waveforms = new TH2I("waveforms","waveforms; time [ns]", numberOfSamples, -20.*samplingRate, (numberOfSamples-20.)*samplingRate,i 1000, -1., 1.1*fullScale);

	//-------------------- loop over tree --------------------
	int i = 0;
	for(auto file : inputFileNames) {
		if(!scope.ReadFile(file)) {
			std::cout<<"failed to read data from '"<<file<<"'"<<std::endl;
			continue;
		}

		time = scope.Time();
		voltage = scope.Voltage();
		graph = scope.Graph();

		tree.Fill();

		if(i == 0) {
		  header = scope.Header();
		} else if(header != scope.Header()) {
		  std::cerr<<"Warning, settings in lecroy header changed:"<<std::endl;
		  header.Diff(scope.Header());
		} else if(verboseLevel > 2) {
		  std::cout<<i<<": matching header information"<<std::endl;
		}

		if(i%100 == 0) {
		  std::cout<<(100*i)/inputFileNames.size()<<" % done\r"<<std::flush;
		}
		++i;
	}
	
	std::cout<<(100*i)/inputFileNames.size()<<" % done"<<std::endl;

	tree.Write("",TObject::kOverwrite);
	//waveforms->Write("", TObject::kOverwrite);
	header.Write("header",TObject::kOverwrite);

	outputFile.Close();

	return 0;
}
