#include "src/create_folder.h"


//choose the particle

string particle ="Z";
//string particle ="Jpsi";



TEfficiency* read_TEfficiency(const char* folder_path, const char* file_name, const char* TEfficiency_path)
{
	string file_path = string(folder_path) + string(file_name);
	TFile *file0 = TFile::Open(file_path.c_str());
	if (file0 == NULL)
	{
		std::cerr << "Could not find \"" << file_path << "\" file.\n";
		std::cerr << "ABORTING...\n";
		abort();
	}

	TEfficiency* pEff0 = (TEfficiency*)file0->Get(TEfficiency_path);
	if (pEff0== NULL)
	{
		std::cerr << "Could not find the histogram path \"" << TEfficiency_path << "\" in \"" << file_path << "\" file.\n";
		std::cerr << "ABORTING...\n";
		abort();
	}

	return pEff0;
}

void compare_efficiencies()
{
	if(particle=="Z"){
	//For real data
	
	const char*  input_folder_name = "results/efficiencies/efficiency/Z_Run/";
	const char* output_folder_name = "results/overplots/";
	
	//string MuonId   = "trackerMuon";
	//string MuonId   = "standaloneMuon";
	string MuonId   = "globalMuon";

	//string quantity = "Pt";
	//string quantity = "Eta";
	string quantity = "Phi";

	string file_name = quantity+"_"+MuonId+".root";
	TEfficiency* pEffRealData	= read_TEfficiency("results/efficiencies/efficiency/Z_Run/", file_name.c_str(), string(MuonId + "_" + quantity +  "_Efficiency").c_str());
	TEfficiency* pEffMCData		= read_TEfficiency("results/efficiencies/efficiency/Z_Run_MC/", file_name.c_str(), string(MuonId + "_" + quantity + "_Efficiency").c_str());

	TCanvas* c1 = new TCanvas("efficiency", "Efficiency");
	pEffRealData           ->SetLineColor(kBlue);
	pEffMCData		->SetLineColor(kRed);



	//Set range in y axis
	pEffRealData->Draw();
	pEffRealData->SetTitle((string(pEffRealData->GetTitle()).substr(0, string(pEffRealData->GetTitle()).size())).c_str());
	gPad->Update();
	auto graph = pEffRealData->GetPaintedGraph();
	graph->SetMinimum(0.66);
	graph->SetMaximum(1.);

	if (quantity == "Pt")
	{
		graph->SetMinimum(0.90);
		graph->SetMaximum(1.02);
	}
	else if (quantity == "Eta")
	{
		graph->SetMinimum(0.5);
		graph->SetMaximum(1.02);
	}

	gPad->Update();
	
	pEffRealData     ->Draw("same");
	pEffMCData		->Draw("same");

	//TLegend* tl = new TLegend(0.70,0.86,0.96,0.92);
	//TLegend* tl = new TLegend(0.2,0.86,0.4,0.92);
	TLegend* tl = new TLegend(0.15,0.45,0.35, 0.2);
	if (MuonId == "trackerMuon" && quantity != "Eta")
		tl->SetY2(0.8);
	tl->SetTextSize(0.04);
	tl->AddEntry(pEffRealData, 	"Z Data Fitting", 		"elp");
	tl->AddEntry(pEffMCData, 	"Z Mc Fitting", 	"elp");
	tl->SetY1(tl->GetY2() - tl->GetTextSize()*1.2*tl->GetNRows());
	tl->Draw();

	if (quantity == "Pt")
		c1->SetLogx();

	//if (quantity == "Eta")
		//c1->SetLogy();

	create_folder(output_folder_name);

	
}else if(particle=="Jpsi"){
//For real data
	
	const char*  input_folder_name = "results/efficiencies/efficiency/Jpsi_Run_2011/";
	const char* output_folder_name = "results/overplots/";
	
	//string MuonId   = "trackerMuon";
	//string MuonId   = "standaloneMuon";
	string MuonId   = "globalMuon";

	string quantity = "Pt";
	//string quantity = "Eta";
	//string quantity = "Phi";

	string file_name = quantity+"_"+MuonId+".root";
	TEfficiency* pEffRealData	= read_TEfficiency("results/efficiencies/efficiency/Jpsi_Run_2011/", file_name.c_str(), string(MuonId + "_" + quantity +  "_Efficiency").c_str());
	TEfficiency* pEffMCData		= read_TEfficiency("results/efficiencies/efficiency/Jpsi_MC_2020/", file_name.c_str(), string(MuonId + "_" + quantity + "_Efficiency").c_str());

	TCanvas* c1 = new TCanvas("efficiency", "Efficiency");
	pEffRealData           ->SetLineColor(kBlue);
	pEffMCData		->SetLineColor(kRed);



	//Set range in y axis
	pEffRealData->Draw();
	pEffRealData->SetTitle((string(pEffRealData->GetTitle()).substr(0, string(pEffRealData->GetTitle()).size())).c_str());
	gPad->Update();
	auto graph = pEffRealData->GetPaintedGraph();
	graph->SetMinimum(0.66);
	graph->SetMaximum(1.);

	if (quantity == "Pt")
	{
		graph->SetMinimum(0.90);
		graph->SetMaximum(1.02);
	}
	else if (quantity == "Eta")
	{
		graph->SetMinimum(0.5);
		graph->SetMaximum(1.02);
	}

	gPad->Update();
	
	pEffRealData     ->Draw("same");
	pEffMCData		->Draw("same");

	//TLegend* tl = new TLegend(0.70,0.86,0.96,0.92);
	//TLegend* tl = new TLegend(0.2,0.86,0.4,0.92);
	TLegend* tl = new TLegend(0.15,0.45,0.35, 0.2);
	if (MuonId == "trackerMuon" && quantity != "Eta")
		tl->SetY2(0.8);
	tl->SetTextSize(0.04);
	tl->AddEntry(pEffRealData, 	"#Upsilon(1S) Data Fitting", 		"elp");
	tl->AddEntry(pEffMCData, 	"#Upsilon(1S) Mc Fitting", 	"elp");
	tl->SetY1(tl->GetY2() - tl->GetTextSize()*1.2*tl->GetNRows());
	tl->Draw();

	if (quantity == "Pt")
		c1->SetLogx();

	//if (quantity == "Eta")
		//c1->SetLogy();

	create_folder(output_folder_name);

	
}
}
