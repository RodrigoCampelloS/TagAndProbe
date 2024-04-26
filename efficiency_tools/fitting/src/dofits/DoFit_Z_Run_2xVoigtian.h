#ifndef DOFIT_HEADER
#define DOFIT_HEADER
//We start by declaring the nature of our dataset. (Is the data real or simulated?)
const char* output_folder_name = "Z_Run";

//Header of this function
double _mmin = 70.0;
double _mmax = 110.0;
double fit_bins = 0; //Let it 0 if dont want to change

//Information for output at the end of run
const char* fit_functions = "2xVoigtian  + Chebychev ";
string prefix_file_name = "";
#endif
using namespace RooFit;

//Returns array with [yield_all, yield_pass, err_all, err_pass]
/*#ifdef DEFAULT_FUCTION_NAME_USED
	double* doFit2xGaus
#else
	#define DEFAULT_FUCTION_NAME_USED
	double* doFit
#endif*/
#define DEFAULT_FUCTION_NAME_USED
	double* doFit2xVoitgtian
(string condition, string MuonId, const char* savePath = NULL)
{
	cout << "----- Fitting data on bin -----\n";
	cout << "Conditions: " << condition << "\n";
	cout << "-------------------------------\n";

	string MuonId_str = "";
	if      (MuonId == "trackerMuon")    MuonId_str = "PassingProbeTrackingMuon";
	else if (MuonId == "standaloneMuon") MuonId_str = "PassingProbeStandAloneMuon";
	else if (MuonId == "globalMuon")     MuonId_str = "PassingProbeGlobalMuon";
	
	TFile *file0       = TFile::Open("DATA/TagAndProbe_Z.root");
	TTree* DataTree = (TTree*)file0->Get(("tagandprobe"));
	
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
	RooRealVar mean1("mean","mean",91.1976, 90., 92.);
	RooRealVar mean2("mean","mean",91.1976, 89., 93.);
	RooRealVar sigma1("sigma","sigma",1.0);
	RooRealVar sigma2("sigma","sigma",3.0);
	RooRealVar width("width","width",2.45);
	
	//CHEBYBHEV VARIABLES
	RooRealVar a0("a0", "a0", 0.25, 0, 0.5);
	RooRealVar a1("a1", "a1", 0.25, 0, 0.1);
	RooRealVar a2("a2", "a2", 0, 0, 0.25);
	
	/*RooRealVar a0("a0", "a0", 0.25, 0, 0.5);
	RooRealVar a1("a1", "a1", -0.25, -1, 0.1);
	RooRealVar a2("a2", "a2", 0, -0.25, 0.25);*/
	//FIT FUNCTIONS
	RooVoigtian voitgtian1("VT1","VT1",InvariantMass,mean1,width,sigma1);
	RooVoigtian voitgtian2("VT2","VT2",InvariantMass,mean2,width,sigma2);
	RooChebychev background("background","background",InvariantMass, {a0,a1,a2});
	
	//RooRealVar frac1("frac1","frac1",0.5);
	RooRealVar vFrac("vFrac","vfrac",0.5);

	//SUM::signal(vFrac[0.8,0.5,1]*signal1, signal2)",
	
	//RooAddPdf* signal = new RooAddPdf("signal", "signal", RooArgList(vFrac*voitgtian1, voitgtian2), RooArgList(frac1));
	RooAddPdf* signal = new RooAddPdf("signal", "signal", RooArgList(voitgtian1, voitgtian2), RooArgList(vFrac));
	RooRealVar n_signal_total("n_signal_total","n_signal_total",Data_ALL->sumEntries()/2,0.,Data_ALL->sumEntries());
	RooRealVar n_signal_total_pass("n_signal_total_pass","n_signal_total_pass",Data_PASSING->sumEntries()/2,0.,Data_PASSING->sumEntries());

	RooRealVar n_back("n_back","n_back",Data_ALL->sumEntries()/2,0.,Data_ALL->sumEntries());
	RooRealVar n_back_pass("n_back_pass","n_back_pass",Data_PASSING->sumEntries()/2,0.,Data_PASSING->sumEntries());

	RooAddPdf* model      = new RooAddPdf("model", "model", RooArgList(*signal, background),RooArgList(n_signal_total, n_back));
	RooAddPdf* model_pass = new RooAddPdf("model_pass", "model_pass", RooArgList(*signal, background),RooArgList(n_signal_total_pass, n_back_pass));
	
	// SIMULTANEOUS FIT
	RooCategory sample("sample","sample") ;
	sample.defineType("All") ;
	sample.defineType("Passing") ;
	
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
	
	RooRealVar* yield_all = (RooRealVar*) fitres->floatParsFinal().find("n_signal_total");
	RooRealVar* yield_pass = (RooRealVar*) fitres->floatParsFinal().find("n_signal_total_pass");
	
	output[0] = yield_all->getVal();
	output[1] = yield_pass->getVal();
	
	output[2] = yield_all->getError();
	output[3] = yield_pass->getError();
	
	frame->SetTitle("ALL");
	frame->SetXTitle("#mu^{+}#mu^{-} invariant mass [GeV/c^{2}]");
	Data_ALL->plotOn(frame);
	
	model->plotOn(frame);

	model->plotOn(frame,RooFit::Components("signal"),RooFit::LineStyle(kDashed),RooFit::LineColor(kGreen));
	model->plotOn(frame,RooFit::Components("VT1"),RooFit::LineStyle(kDashed),RooFit::LineColor(kOrange));
	model->plotOn(frame,RooFit::Components("VT2"),RooFit::LineStyle(kDashed),RooFit::LineColor(kMagenta - 5));
	model->plotOn(frame,RooFit::Components("background"),RooFit::LineStyle(kDashed),RooFit::LineColor(kRed));
	
	c_all->cd();
	frame->Draw("");
	
	//CREATING LEGEND
	
	TLegend *leg = new TLegend(0.6,0.6,0.9,0.9);
	leg->AddEntry(blue,"Total Fit","l");
	leg->AddEntry(green,"Signal","l");
	leg->AddEntry(orange,"Voitgtian-1","l");
	leg->AddEntry(magenta,"Voitgtian-2","l");
	leg->AddEntry(red,"Background","l");
	leg->Draw();
	RooPlot* frame_pass = InvariantMass.frame(RooFit::Title("Invariant Mass"));
	
	c_pass->cd();
	
	frame_pass->SetTitle("PASSING");
	frame_pass->SetXTitle("#mu^{+}#mu^{-} invariant mass [GeV/c^{2}]");
	Data_PASSING->plotOn(frame_pass);
	
	model_pass->plotOn(frame_pass);
	model_pass->plotOn(frame_pass,RooFit::Components("signal"),RooFit::LineStyle(kDashed),RooFit::LineColor(kGreen));
	model_pass->plotOn(frame_pass,RooFit::Components("VT1"),RooFit::LineStyle(kDashed),RooFit::LineColor(kOrange));
	model_pass->plotOn(frame_pass,RooFit::Components("VT2"),RooFit::LineStyle(kDashed),RooFit::LineColor(kMagenta - 5));
	model_pass->plotOn(frame_pass,RooFit::Components("background"),RooFit::LineStyle(kDashed),RooFit::LineColor(kRed));
	
	
	frame_pass->Draw();

	//CREATING LEGEND
	leg->Draw();
	
	if (savePath != NULL)
	{
		c_pass->SaveAs((string(savePath) + condition + "_PASS.png").c_str());
		c_all->SaveAs ((string(savePath) + condition + "_ALL.png").c_str());
	}

	cout << "-------------------------------\n";

	// Deleting allocated memory
	delete file0;

	//delete fv_CUT;
	//delete fv_ALL;
	//delete fv_PASS;
	//delete Data_CUT;
	delete Data_ALL;
	delete Data_PASSING;
	delete dh_ALL;
	delete dh_PASSING;
	delete cutvar;
	delete redeuce;
	delete Dataset;
	delete c_all;
	delete c_pass;

	delete model;
	delete model_pass;

	delete signal;
	delete fitres;
	
	return output;
}
