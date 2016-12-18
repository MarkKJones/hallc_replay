#define wire_drift_times_cxx
#include "wire_drift_times.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void wire_drift_times::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L wire_drift_times.C
//      Root > wire_drift_times t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

//USER INPUT 
int run;
string st_DC_num;
string st_plane;

cout << "Enter Run Number Once More: ";
cin >> run;
cout << "Enter HMS Drift Chamber to analyze (type 1 or 2): ";
cin >> st_DC_num;
cout << "Enter Plane (type x1 y1 u1 v1 y2 x2 ) ";
cin >> st_plane;

string st_DC_plane = st_DC_num+st_plane;
string run_NUM = Form("run_%d",run);

//Create a root file to store individual DC cell drift times
TString root_file = "root_files/"+run_NUM+"/hms_DC_"+st_DC_plane+Form("_%d.root", run);
TFile *g = new TFile(root_file, "RECREATE");

g->cd();


   
int total_wires;  //integer to store total sense wires for a plane chosen by the user

//compare the user input string to desired planes in order to create TH1F and TH2F accordingly   
if (st_plane.compare("x1")==0 || st_plane.compare("x2")==0) {  

//Create 1D and 2D Histo Array to store WIRE drift times for DC --BASED ON USER INPUT      

TH1F *cell_dt[113];    
TH2F *wire_vs_dt = new TH2F("wire_vs_dt", "", 200., -50., 350., 113., 0.,113.);
total_wires = 113;
       }

else if (st_plane.compare("u1")==0 || st_plane.compare("v1")==0) {

TH1F *cell_dt[107];
TH2F *wire_vs_dt = new TH2F("wire_vs_dt", "", 200., -50., 350., 107., 0.,107);
total_wires=107;
       }	   

else if (st_plane.compare("y1")==0 || st_plane.compare("y2")==0) {

TH1F *cell_dt[52];
TH2F *wire_vs_dt = new TH2F("wire_vs_dt", "", 200., -50., 350., 52., 0.,52.);
total_wires=52;
       }	    
 
   Long64_t nbytes = 0, nb = 0;
   
//Loop over each sense wire in a given DC plane 
for (int sensewire=1; sensewire<=total_wires; sensewire++) {
   
//Set Histo name and range
cell_dt[sensewire-1] = new TH1F(Form("wire_%d", sensewire), "", 200., -50., 350.);

//Loop over nentries (# of triggers detected by DC)
   Long64_t nbytes = 0, nb = 0;

   for (Long64_t jentry=0; jentry<nentries; jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;


//============DC1 PLANE X1================================
      if (st_DC_plane.compare("1x1")==0) {   
         
        for (int i=0; i< Ndata_H_dc_1x1_wirenum; i++){

	
	  if (H_dc_1x1_wirenum[i]==sensewire) {
	 // cout << " wire num: " << H_dc_2v1_wirenum[i] << endl;
	 // cout << "Time: " << H_dc_2v1_time[i] << endl;

      //Fill the Histograms
      cell_dt[sensewire-1]->Fill(H_dc_1x1_time[i]);
      wire_vs_dt->Fill(H_dc_1x1_time[i], H_dc_1x1_wirenum[i]);

}

}
}
//=============DC1 PLANE Y1=================================
     else if (st_DC_plane.compare("1y1")==0) {   
         
        for (int i=0; i< Ndata_H_dc_1y1_wirenum; i++){

	
	  if (H_dc_1y1_wirenum[i]==sensewire) {
	 // cout << " wire num: " << H_dc_2v1_wirenum[i] << endl;
	 // cout << "Time: " << H_dc_2v1_time[i] << endl;

      //Fill the Histograms
      cell_dt[sensewire-1]->Fill(H_dc_1y1_time[i]);
      wire_vs_dt->Fill(H_dc_1y1_time[i], H_dc_1y1_wirenum[i]);

}

}
}
//===============DC1 PLANE U1================================
     else if (st_DC_plane.compare("1u1")==0) {   
         
        for (int i=0; i< Ndata_H_dc_1u1_wirenum; i++){

	
	  if (H_dc_1u1_wirenum[i]==sensewire) {
	 // cout << " wire num: " << H_dc_2v1_wirenum[i] << endl;
	 // cout << "Time: " << H_dc_2v1_time[i] << endl;

      //Fill the Histograms
      cell_dt[sensewire-1]->Fill(H_dc_1u1_time[i]);
      wire_vs_dt->Fill(H_dc_1u1_time[i], H_dc_1u1_wirenum[i]);

}

}
}
//============DC1 PLANE V1====================================
     else if (st_DC_plane.compare("1v1")==0) {   
         
        for (int i=0; i< Ndata_H_dc_1v1_wirenum; i++){

	
	  if (H_dc_1v1_wirenum[i]==sensewire) {
	 // cout << " wire num: " << H_dc_2v1_wirenum[i] << endl;
	 // cout << "Time: " << H_dc_2v1_time[i] << endl;

      //Fill the Histograms
      cell_dt[sensewire-1]->Fill(H_dc_1v1_time[i]);
      wire_vs_dt->Fill(H_dc_1v1_time[i], H_dc_1v1_wirenum[i]);

}

}
}
//==========DC1 PLANE Y2=====================================
     else if (st_DC_plane.compare("1y2")==0) {   
         
        for (int i=0; i< Ndata_H_dc_1y2_wirenum; i++){

	
	  if (H_dc_1y2_wirenum[i]==sensewire) {
	 // cout << " wire num: " << H_dc_2v1_wirenum[i] << endl;
	 // cout << "Time: " << H_dc_2v1_time[i] << endl;

      //Fill the Histograms
      cell_dt[sensewire-1]->Fill(H_dc_1y2_time[i]);
      wire_vs_dt->Fill(H_dc_1y2_time[i], H_dc_1y2_wirenum[i]);

}

}
}
//============DC1 PLANE X2===================================
     else if (st_DC_plane.compare("1x2")==0) {   
         
        for (int i=0; i< Ndata_H_dc_1x2_wirenum; i++){

	
	  if (H_dc_1x2_wirenum[i]==sensewire) {
	 // cout << " wire num: " << H_dc_2v1_wirenum[i] << endl;
	 // cout << "Time: " << H_dc_2v1_time[i] << endl;

      //Fill the Histograms
      cell_dt[sensewire-1]->Fill(H_dc_1x2_time[i]);
      wire_vs_dt->Fill(H_dc_1x2_time[i], H_dc_1x2_wirenum[i]);

}

}
}

  //============DC2 PLANE X1================================
      if (st_DC_plane.compare("2x1")==0) {   
         
        for (int i=0; i< Ndata_H_dc_2x1_wirenum; i++){

	
	  if (H_dc_2x1_wirenum[i]==sensewire) {
	 // cout << " wire num: " << H_dc_2v1_wirenum[i] << endl;
	 // cout << "Time: " << H_dc_2v1_time[i] << endl;

      //Fill the Histograms
      cell_dt[sensewire-1]->Fill(H_dc_2x1_time[i]);
      wire_vs_dt->Fill(H_dc_2x1_time[i], H_dc_2x1_wirenum[i]);

}

}
}
//=============DC2 PLANE Y1=================================
     else if (st_DC_plane.compare("2y1")==0) {   
         
        for (int i=0; i< Ndata_H_dc_2y1_wirenum; i++){

	
	  if (H_dc_2y1_wirenum[i]==sensewire) {
	 // cout << " wire num: " << H_dc_2v1_wirenum[i] << endl;
	 // cout << "Time: " << H_dc_2v1_time[i] << endl;

      //Fill the Histograms
      cell_dt[sensewire-1]->Fill(H_dc_2y1_time[i]);
      wire_vs_dt->Fill(H_dc_2y1_time[i], H_dc_2y1_wirenum[i]);

}

}
}
//===============DC2 PLANE U1================================
     else if (st_DC_plane.compare("2u1")==0) {   
         
        for (int i=0; i< Ndata_H_dc_2u1_wirenum; i++){

	
	  if (H_dc_2u1_wirenum[i]==sensewire) {
	 // cout << " wire num: " << H_dc_2v1_wirenum[i] << endl;
	 // cout << "Time: " << H_dc_2v1_time[i] << endl;

      //Fill the Histograms
      cell_dt[sensewire-1]->Fill(H_dc_2u1_time[i]);
      wire_vs_dt->Fill(H_dc_2u1_time[i], H_dc_2u1_wirenum[i]);

}

}
}
//============DC2 PLANE V1====================================
     else if (st_DC_plane.compare("2v1")==0) {   
         
        for (int i=0; i< Ndata_H_dc_2v1_wirenum; i++){

	
	  if (H_dc_2v1_wirenum[i]==sensewire) {
	 // cout << " wire num: " << H_dc_2v1_wirenum[i] << endl;
	 // cout << "Time: " << H_dc_2v1_time[i] << endl;

      //Fill the Histograms
      cell_dt[sensewire-1]->Fill(H_dc_2v1_time[i]);
      wire_vs_dt->Fill(H_dc_2v1_time[i], H_dc_2v1_wirenum[i]);

}

}
}
//==========DC2 PLANE Y2=====================================
     else if (st_DC_plane.compare("2y2")==0) {   
         
        for (int i=0; i< Ndata_H_dc_2y2_wirenum; i++){

	
	  if (H_dc_2y2_wirenum[i]==sensewire) {
	 // cout << " wire num: " << H_dc_2v1_wirenum[i] << endl;
	 // cout << "Time: " << H_dc_2v1_time[i] << endl;

      //Fill the Histograms
      cell_dt[sensewire-1]->Fill(H_dc_2y2_time[i]);
      wire_vs_dt->Fill(H_dc_2y2_time[i], H_dc_2y2_wirenum[i]);

}

}
}
//============DC2 PLANE X2===================================
     else if (st_DC_plane.compare("2x2")==0) {   
         
        for (int i=0; i< Ndata_H_dc_2x2_wirenum; i++){

	
	  if (H_dc_2x2_wirenum[i]==sensewire) {
	 // cout << " wire num: " << H_dc_2v1_wirenum[i] << endl;
	 // cout << "Time: " << H_dc_2v1_time[i] << endl;

      //Fill the Histograms
      cell_dt[sensewire-1]->Fill(H_dc_2x2_time[i]);
      wire_vs_dt->Fill(H_dc_2x2_time[i], H_dc_2x2_wirenum[i]);

}

}
}
   }

          cout << "\r          \r" << (float)sensewire / total_wires * 100.0 << "%" << flush; 

}

//Write wire drift time histos to file
g->Write();

}
