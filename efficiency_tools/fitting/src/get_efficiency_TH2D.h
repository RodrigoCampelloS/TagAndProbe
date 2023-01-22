
TH2D* get_efficiency_TH2D(TH2D* hall, TH2D* hpass, string xquantity, string yquantity, string MuonId, string prefix_name = "")
{
	TH2D* heff  = (TH2D*)hall ->Clone();
	bool should_save = true;
	//Set eery bin content and error
	for (int i=1; i<=hall->GetNbinsX(); i++)
	{
		for (int j=1; j<=hall->GetNbinsY(); j++)
		{
			double val_all  = hall ->GetBinContent(i,j);
			double val_pass = hpass->GetBinContent(i,j);
			double unc_all  = hall ->GetBinError(i,j);
			double unc_pass = hpass->GetBinError(i,j);

			double value     = val_pass/val_all;
			double uncertain = fabs(value)*sqrt(pow(unc_pass/val_pass, 2) + pow(unc_all/val_all, 2));

			heff->SetBinContent(i,j, value);
			heff->SetBinError(i,j, uncertain);
		}
	}

	//Set plot config
	if (prefix_name != "")
	{
		heff->SetName(string(MuonId + "_" + yquantity + "_" + xquantity + "_" + prefix_name + "_Efficiency").c_str());
		heff->SetTitle(string("Efficiency for " + MuonId + " (" + prefix_name + ")").c_str());
		heff->GetYaxis()->SetDecimals();
		heff->GetYaxis()->SetMaxDigits(3);
		heff->SetStats(kFALSE);
	}
	else
	{
		heff->SetName(string(MuonId + "_" + yquantity + "_" + xquantity + "_Efficiency").c_str());
		heff->SetTitle(string("Efficiency for " + MuonId).c_str());
		heff->GetYaxis()->SetDecimals();
		heff->GetYaxis()->SetMaxDigits(3);
		heff->SetStats(kFALSE);
	}

	TCanvas* c1 = new TCanvas();
	c1->cd();
	heff->Draw("colztexte");
	//c1->SetLogx();
	
	if (should_save)
	{
	//string output_folder_name = "results/efficiencies/systematic_2D/Z_Run";
	string output_folder_name = "results/efficiencies/systematic_2D/Z_Run_MC";
	//create_folder(output_folder_name.c_str());
	c1->SaveAs(string(output_folder_name + MuonId + "_eff_TH2D.png").c_str());
	}
	return heff;
	
}
