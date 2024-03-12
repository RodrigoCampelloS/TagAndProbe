//Change if you need
//#include "src/dofits/DoFit_Jpsi_Run.h"

//uncomment the fit you want to do 
#include "src/dofits/DoFit_Z.h"
//#include "src/dofits/DoFit_Z_MC.h"


#include "src/create_folder.h"
#include "src/get_efficiency.h"
#include "src/make_TH1D.h"

//Which Muon Id do you want to study?
//string MuonId   = "trackerMuon";
//string MuonId   = "standaloneMuon";
string MuonId   = "globalMuon";

//Which Quantity?
string quantity = "Pt";     double bins[] = {15.0,20.0,25.0,30.0,40.0,120.0};
//string quantity = "Eta";    double bins[] = {-2.4, -1.8, -1.4, -1.2, -1.0, -0.8, -0.5, -0.2, 0, 0.2, 0.5, 0.8, 1.0, 1.2, 1.4, 1.8, 2.4};
//string quantity = "Phi";    double bins[] = {-3.0, -1.8, -1.6, -1.2, -1.0, -0.7, -0.4, -0.2, 0, 0.2, 0.4, 0.7, 1.0, 1.2, 1.6, 1.8, 3.0};
void efficiency_Z()
{

	//Path where is going to save results png for every bin 
	const char* path_bins_fit_folder = "results/bins_fit/efficiency/";
	create_folder(path_bins_fit_folder, true);

	// Loop for every bin and fit it
	int nbins = sizeof(bins)/sizeof(*bins) - 1;
	double** yields_n_errs = new double*[nbins];
	for (int i = 0; i < nbins; i++)
	{
		//Creates conditions
		string conditions = string(    "ProbeMuon_" + quantity + ">=" + to_string(bins[i]  ));
		conditions +=       string(" && ProbeMuon_" + quantity + "< " + to_string(bins[i+1]));

		//Stores [yield_all, yield_pass, err_all, err_pass]
		yields_n_errs[i] = doFit(conditions, MuonId, path_bins_fit_folder);
	}

	//Path where is going to save efficiency 
	string directoryToSave = string("results/efficiencies/efficiency/") + output_folder_name + string("/");
	create_folder(directoryToSave.c_str());

	//Create file
	string file_path = directoryToSave + prefix_file_name + quantity + "_" + MuonId + ".root";
	TFile* generatedFile = new TFile(file_path.c_str(),"recreate");
	
	//Create histograms
	generatedFile->mkdir("histograms/");
	generatedFile->   cd("histograms/");
	TH1D *yield_all  = make_TH1D("ALL" , yields_n_errs, 0, bins, nbins, quantity);
	TH1D *yield_pass = make_TH1D("PASS", yields_n_errs, 1, bins, nbins, quantity);
	
	//Create efficiencies
	generatedFile->   cd("/");
	get_efficiency(yield_all, yield_pass, quantity, MuonId, "", true);

	//Write file
	generatedFile->Write();

	cout << "\n[Settings]\n";
	cout << output_folder_name << "\n"<< quantity << " " << MuonId << "\n";
	cout << "Fitting:     " << fit_functions << "\n";
	cout << "Fit between: " << _mmin << " and " << _mmax << " GeV\n";
	cout << "Bins:        " << fit_bins << "\n";

	cout << "\n------------------------\n";
	cout << "Output: " << file_path;
	cout << "\n------------------------\n";
}
