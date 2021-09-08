//Change if you need
#include "src/dofits/DoFit_MC_Z.cpp"
#include "src/compare_efficiency.cpp"
#include "src/create_folder.cpp"
#include "src/get_efficiency.cpp"
#include "src/make_hist.cpp"

//Which Muon Id do you want to study?
//string MuonId   = "trackerMuon";
string MuonId   = "standaloneMuon";
//string MuonId   = "globalMuon";

//Which quantity do you want to use?
//string quantity = "Pt";     double bins[] = {27.0,28.0,29.4,30.0,30.4,30.7,31.0,31.6,31.8,32.0,32.2,32.4,32.6,32.8,33.3,35.5,38.0,42.0,60.};
//string quantity = "Pt";     double bins[] = {15.0,20.0,25.0,30.0,40.0,50.0,60.0,120.0};
//string quantity = "Eta";    double bins[] = {-2.4, -1.8, -1.4, -1.2, -1.0, -0.8, -0.5, -0.2, 0, 0.2, 0.5, 0.8, 1.0, 1.2, 1.4, 1.8, 2.4};
string quantity = "Phi";    double bins[] = {-3.0, -1.8, -1.6, -1.2, -1.0, -0.7, -0.4, -0.2, 0, 0.2, 0.4, 0.7, 1.0, 1.2, 1.6, 1.8, 3.0};
void efficiency()
{
	//compare_efficiency(quantity, "results/efficiencies/Z_Run/Phi_trackerMuon.root", "results/efficiencies/Z_Run_MC/Phi_trackerMuon.root");
	
	//Path where is going to save results png for every bin 
	const char* path_bins_fit_folder = "results/bins_fit/";
	create_folder(path_bins_fit_folder, true);

	// Loop for every bin and fit it
	int bin_n = sizeof(bins)/sizeof(*bins) - 1;
	double** yields_n_errs = new double*[bin_n];
	for (int i = 0; i < bin_n; i++)
	{
		//Creates conditions
		string conditions = string(    "ProbeMuon_" + quantity + ">" + to_string(bins[i]  ));
		conditions +=       string(" && ProbeMuon_" + quantity + "<" + to_string(bins[i+1]));

		//Stores [yield_all, yield_pass, err_all, err_pass]
		yields_n_errs[i] = doFit(conditions, MuonId, path_bins_fit_folder);
	}

	//Path where is going to save efficiency 
	string directoryToSave = string("results/efficiencies/") + output_folder_name + string("/");
	create_folder(directoryToSave.c_str());

	//Create file
	string file_path = directoryToSave + prefix_file_name + quantity + "_" + MuonId + ".root";
	TFile* generatedFile = new TFile(file_path.c_str(),"recreate");
	
	//Create histograms
	generatedFile->mkdir("histograms/");
	generatedFile->   cd("histograms/");
	TH1D *yield_all  = make_hist("ALL", yields_n_errs, 0, bin_n, bins);
	TH1D *yield_pass = make_hist("PASS", yields_n_errs, 1, bin_n, bins);
	
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
