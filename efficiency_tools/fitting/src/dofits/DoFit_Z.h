#ifndef DOFIT_HEADER
#define DOFIT_HEADER
const char* output_folder_name = "Z_Run";

//Header of this function
double _mmin = 70.0;
double _mmax = 110.0;
double fit_bins = 0; //Let it 0 if dont want to change

// Information for output at the end of run
const char* fit_functions = "Voigtian + CrystalBall + Polynomial";
string prefix_file_name = "";
#endif
using namespace RooFit;
#define DEFAULT_FUCTION_NAME_USED
	double* doFit(string condition, string MuonId, const char* savePath = NULL) // RETURNS ARRAY WITH [yield_all, yield_pass, err_all, err_pass]
{
	string MuonId_str = "";
	if      (MuonId == "trackerMuon")    MuonId_str = "PassingProbeTrackingMuon";
	else if (MuonId == "standaloneMuon") MuonId_str = "PassingProbeStandAloneMuon";
	else if (MuonId == "globalMuon")     MuonId_str = "PassingProbeGlobalMuon";
	
	TFile *file0       = TFile::Open("DATA/TagAndProbe_Z.root");
	//TFile *file0       = TFile::Open("DATA/TagAndProbe_Z_MC.root");
	TTree *DataTree    = (TTree*)file0->Get(("tagandprobe"));
	
	RooCategory MuonId_var (MuonId_str.c_str(), MuonId_str.c_str(), {{"Passing", 1},{"Failing", 0}});
	RooRealVar  InvariantMass ("InvariantMass", "InvariantMass", _mmin, _mmax);
	RooRealVar  quantityPt  ("ProbeMuon_Pt",  "ProbeMuon_Pt",  15.0, 120);
	RooRealVar  quantityEta ("ProbeMuon_Eta", "ProbeMuon_Eta", -2.4, 2.4);
	RooRealVar  quantityPhi ("ProbeMuon_Phi", "ProbeMuon_Phi", -TMath::Pi(), TMath::Pi());

	if (fit_bins > 0) InvariantMass.setBins(fit_bins);
	fit_bins = InvariantMass.getBinning().numBins();

	RooDataSet *Dataset = new RooDataSet("DATA","DATA",RooArgSet(InvariantMass, MuonId_var, quantityPt, quantityEta, quantityPhi),Import(*DataTree));

	RooFormulaVar* redeuce   = new RooFormulaVar("PPTM_cond",condition.c_str(), *Dataset->get());
	RooDataSet *Data_ALL     = new RooDataSet("DATA", "DATA",Dataset, *Dataset->get(),*redeuce);


	RooFormulaVar* cutvar    = new RooFormulaVar("PPTM_mounid", (MuonId_str + "==1").c_str(), *Data_ALL->get());
	RooDataSet *Data_PASSING = new RooDataSet("DATA_PASS", "DATA_PASS", Data_ALL, *Data_ALL->get(), *cutvar);
	
	RooDataHist* dh_ALL     = new RooDataHist(Data_ALL->GetName(),    Data_ALL->GetTitle(),     RooArgSet(InvariantMass), *Data_ALL);
	RooDataHist* dh_PASSING = new RooDataHist(Data_PASSING->GetName(),Data_PASSING->GetTitle(), RooArgSet(InvariantMass), *Data_PASSING);

	
	TCanvas* c_all  = new TCanvas;
	TCanvas* c_pass = new TCanvas;
	
	RooPlot *frame = InvariantMass.frame(RooFit::Title("Invariant Mass"));
	
	// VOIGTIAN VARIABLES
	RooRealVar mean("mean","mean",91.1976, 90., 92.);
	RooRealVar sigma("sigma","sigma",1.0);
	RooRealVar width("width","width",2.45);
	//CRYSTALBALL VARIABLES
	RooRealVar sigma_cb("sigma_cb","sigma_cb", 2.0);
	RooRealVar alpha("alpha", "alpha", 0.65);
	RooRealVar n("n", "n", 3.0);
	// BACKGROUND VARIABLES
    	RooRealVar a0("a0", "a0", 0, 0, 10);
    	RooRealVar a1("a1", "a1", 0, 0, 10);
	//FIT FUNCTIONS
	RooVoigtian voitgtian("VT","VT",InvariantMass,mean,width,sigma);
	RooPolynomial background("BG", "BG", InvariantMass, RooArgList(a0,a1));
	RooCBShape crystalball("CB", "CB", InvariantMass, mean, sigma_cb, alpha, n);
	double n_signal_initial_total = 80000;
	double n_back_initial = 10000;
	RooRealVar frac1("frac1","frac1",0.37);
	RooAddPdf* signal;
	signal      = new RooAddPdf("signal", "signal", RooArgList(voitgtian, crystalball), RooArgList(frac1));
	
	RooRealVar n_signal_total("n_signal_total","n_signal_total",n_signal_initial_total,0.,Data_ALL->sumEntries());
	RooRealVar n_signal_total_pass("n_signal_total_pass","n_signal_total_pass",n_signal_initial_total,0.,Data_PASSING->sumEntries());

	RooRealVar n_back("n_back","n_back",n_back_initial,0.,Data_ALL->sumEntries());
	RooRealVar n_back_pass("n_back_pass","n_back_pass",n_back_initial,0.,Data_PASSING->sumEntries());
	
	RooAddPdf* model;
	RooAddPdf* model_pass;
	
	model      = new RooAddPdf("model","model", RooArgList(*signal, background),RooArgList(n_signal_total, n_back));
	model_pass = new RooAddPdf("model_pass", "model_pass", RooArgList(*signal, background),RooArgList(n_signal_total_pass, n_back_pass));
	
	// SIMULTANEOUS FIT
	RooCategory sample("sample","sample") ;
	sample.defineType("All") ;
	sample.defineType("PASSING") ;
	
	RooDataHist combData("combData","combined data",InvariantMass,Index(sample),Import("ALL",*dh_ALL),Import("PASSING",*dh_PASSING));
	
	RooSimultaneous simPdf("simPdf","simultaneous pdf",sample) ;
	
	simPdf.addPdf(*model,"ALL");
	simPdf.addPdf(*model_pass,"PASSING");
	
	RooFitResult* fitres = new RooFitResult;
	fitres = simPdf.fitTo(combData, RooFit::Save());
	//legenda
	TH1F *orange = new TH1F("h1","Ex",1,-10,10);
	TH1F *green = new TH1F("h2","Ex",1,-10,10);
	TH1F *red = new TH1F("h3","Ex",1,-10,10);
	TH1F *magenta = new TH1F("h4","Ex",1,-10,10);
	TH1F *blue = new TH1F("h5","Ex",1,-10,10);
	orange->SetLineColor(kOrange);
	green->SetLineColor(kGreen);
	red->SetLineColor(kRed);
	magenta->SetLineColor(kMagenta);
	blue->SetLineColor(kBlue);
	orange->SetLineStyle(2);
	green->SetLineStyle(2);
	red->SetLineStyle(2);
	magenta->SetLineStyle(2);
	// OUTPUT ARRAY
	double* output = new double[4];
	
	RooRealVar* yield_ALL = (RooRealVar*) fitres->floatParsFinal().find("n_signal_total");
	RooRealVar* yield_PASS = (RooRealVar*) fitres->floatParsFinal().find("n_signal_total_pass");
	
	output[0] = yield_ALL->getVal();
	output[1] = yield_PASS->getVal();
	
	output[2] = yield_ALL->getError();
	output[3] = yield_PASS->getError();
	
	frame->SetTitle("ALL");
	frame->SetXTitle("#mu^{+}#mu^{-} invariant mass [GeV/c^{2}]");
	Data_ALL->plotOn(frame);
	
	model->plotOn(frame);
	model->plotOn(frame,RooFit::Components("signal"),RooFit::LineStyle(kDashed),RooFit::LineColor(kGreen));
	model->plotOn(frame,RooFit::Components("BG"),RooFit::LineStyle(kDashed),RooFit::LineColor(kRed));
	model->plotOn(frame,RooFit::Components("VT"),RooFit::LineStyle(kDashed),RooFit::LineColor(kOrange));
	model->plotOn(frame,RooFit::Components("CB"),RooFit::LineStyle(kDashed),RooFit::LineColor(kMagenta));
	
	
	c_all->cd();
	frame->Draw("");
	//CREATING LEGEND
	TLegend *leg = new TLegend(0.6,0.6,0.9,0.9);
	leg->AddEntry(blue,"Total Fit","l");
	leg->AddEntry(green,"Signal","l");
	leg->AddEntry(orange,"Voitgtian","l");
	leg->AddEntry(magenta,"Crystal ball","l");
	leg->AddEntry(red,"Background","l");
	leg->Draw();

	RooPlot *frame_pass = InvariantMass.frame(RooFit::Title("Invariant Mass"));
	
	c_pass->cd();
	
	frame_pass->SetTitle("PASSING");
	frame_pass->SetXTitle("#mu^{+}#mu^{-} invariant mass [GeV/c^{2}]");
	Data_PASSING->plotOn(frame_pass);
	
	model_pass->plotOn(frame_pass);
	model_pass->plotOn(frame_pass,RooFit::Components("signal"),RooFit::LineStyle(kDashed),RooFit::LineColor(kGreen));
	model_pass->plotOn(frame_pass,RooFit::Components("BG"),RooFit::LineStyle(kDashed),RooFit::LineColor(kRed));
	model_pass->plotOn(frame_pass,RooFit::Components("VT"),RooFit::LineStyle(kDashed),RooFit::LineColor(kOrange));
	model_pass->plotOn(frame_pass,RooFit::Components("CB"),RooFit::LineStyle(kDashed),RooFit::LineColor(kMagenta));
	

	frame_pass->Draw();
		//CREATING LEGEND
	leg->Draw();

	if (savePath != NULL)
	{
		c_pass->SaveAs((string(savePath) + condition + "_PASS.pdf").c_str());
		c_all->SaveAs ((string(savePath) + condition + "_ALL.pdf").c_str());
	}
		
	// DELETING ALLOCATED MEMORY
	delete file0;
	//
	delete Data_ALL;
	delete Data_PASSING;
	//
	delete dh_ALL;
	delete dh_PASSING;
	//
	delete cutvar;
	delete redeuce;
	//
	//delete signal;
	//
	delete c_all;
	delete c_pass;
	//
	delete model;
	delete model_pass;
	delete fitres;
	
	return output;
	
}
