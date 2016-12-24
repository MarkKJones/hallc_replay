void get_hdc_2d_hist(){

  TFile *f = new TFile("../../ROOTfiles/testing_259.root");

  TH2F *h2 = (TH2F*)f->Get("hdc1v1_wire_num_vs_tdc_time");

  h2->GetXaxis()->SetRangeUser(0,107);
  h2->GetYaxis()->SetRangeUser(-50,350);
  h2->Draw("colz");
}
