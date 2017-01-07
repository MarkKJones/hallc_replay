#define wire_drift_times_cxx
#include "wire_drift_times.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#define NPLANES 12

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

int run;
//Promt User for Input
cout << "Enter Run Number: ";
cin >> run;


//Declare plane names to loop over
TString plane_names[NPLANES]={"1x1", "1y1", "1u1", "1v1", "1y2", "1x2", "2x1", "2y1", "2u1", "2v1", "2y2", "2x2"};

//Declare a root file array to store individual DC cell drift times
TString root_file[NPLANES];
TFile *g[NPLANES];
   
int total_wires;  //integer to store total sense wires for a plane chosen by the user
     
   Long64_t nbytes = 0, nb = 0;
   
//Loop over all planes
   TString run_dir="root_files/";
      run_dir = run_dir+Form("run_%d", run);
  if (gSystem->mkdir(run_dir)==0) {
   cout << " Created directory = " << run_dir << endl; 
   } else{
   cout << " Directory = " << run_dir << endl; 
  }
// Create 
	TH1F *cell_dt_x[4][113];    
	TH1F *cell_dt_y[4][52];    
	TH1F *cell_dt_uv[4][107];    
	TH2F *wire_vs_dt[12];
        Int_t ix=0,iy=0,iuv=0;
 for (int ip = 0; ip < NPLANES; ip++){	
	if(ip==0 || ip==5 || ip==6 || ip==11) {
	total_wires = 113;
        for (int wirenum=1; wirenum<=total_wires; wirenum++){
 	  cell_dt_x[ix][wirenum-1] = new TH1F(Form("pl_%s_wire_%d",plane_names[ip].Data(), wirenum), "", 200., -50., 350.);
         }
        ix++;
	wire_vs_dt[ip] = new TH2F(Form("pl_%s_wire_vs_dt",plane_names[ip].Data()), "", 200., -50., 350., 113., 0.,113.);
	}
	if(ip==2 || ip==3 || ip==8 || ip==9) {
	total_wires = 107;
        for (int wirenum=1; wirenum<=total_wires; wirenum++){
	  cell_dt_uv[iuv][wirenum-1] = new TH1F(Form("pl_%s_wire_%d",plane_names[ip].Data(), wirenum), "", 200., -50., 350.);
         }
	iuv++;
	wire_vs_dt[ip] = new TH2F(Form("pl_%s_wire_vs_dt",plane_names[ip].Data()), "", 200., -50., 350., 107., 0.,107.);
	}
	if(ip==1 || ip==4 || ip==7 || ip==10) {
	total_wires = 52;
        for (int wirenum=1; wirenum<=total_wires; wirenum++){
	  cell_dt_y[iy][wirenum-1] = new TH1F(Form("pl_%s_wire_%d",plane_names[ip].Data(), wirenum), "", 200., -50., 350.);
        }
	iy++;
	wire_vs_dt[ip] = new TH2F(Form("pl_%s_wire_vs_dt",plane_names[ip].Data()), "", 200., -50., 350., 52., 0.,52.);
	}
   }
    //Initialize wire drift time histograms
	
       //Loop over all entries (triggers or events)   
     for (Long64_t jentry=0; jentry<nentries; jentry++) {
       Long64_t ientry = LoadTree(jentry);
       if (jentry%10000 == 0) cout << " entry = " << jentry << endl;
       //  cout << " entry = " << jentry << endl;
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
 
   for (int i=0; i< Ndata_H_dc_1x1_wirenum; i++){
       wirenum = int(H_dc_1x1_wirenum[i]);
     cell_dt_x[0][wirenum-1]->Fill(H_dc_1x1_time[i]);
     wire_vs_dt[0]->Fill(H_dc_1x1_time[i], H_dc_1x1_wirenum[i]);
    }
   for (int i=0; i< Ndata_H_dc_1x2_wirenum; i++){
       wirenum = int(H_dc_1x2_wirenum[i]);
      cell_dt_x[1][wirenum-1]->Fill(H_dc_1x2_time[i]);
     wire_vs_dt[5]->Fill(H_dc_1x2_time[i], H_dc_1x2_wirenum[i]);
   }						
    for (int i=0; i< Ndata_H_dc_2x1_wirenum; i++){
       wirenum = int(H_dc_2x1_wirenum[i]);
     cell_dt_x[2][wirenum-1]->Fill(H_dc_2x1_time[i]);
     wire_vs_dt[6]->Fill(H_dc_2x1_time[i], H_dc_2x1_wirenum[i]);
   }		
   for (int i=0; i< Ndata_H_dc_2x2_wirenum; i++){
       wirenum = int(H_dc_2x2_wirenum[i]);
      cell_dt_x[3][wirenum-1]->Fill(H_dc_2x2_time[i]);
     wire_vs_dt[11]->Fill(H_dc_2x2_time[i], H_dc_2x2_wirenum[i]);
   }						
      for (int i=0; i< Ndata_H_dc_1u1_wirenum; i++){
     wirenum = int(H_dc_1u1_wirenum[i]);
     cell_dt_uv[0][wirenum-1]->Fill(H_dc_1u1_time[i]);
     wire_vs_dt[2]->Fill(H_dc_1u1_time[i], H_dc_1u1_wirenum[i]);
     }
  for (int i=0; i< Ndata_H_dc_1v1_wirenum; i++){
       wirenum = int(H_dc_1v1_wirenum[i]);
    cell_dt_uv[1][wirenum-1]->Fill(H_dc_1v1_time[i]);
     wire_vs_dt[3]->Fill(H_dc_1v1_time[i], H_dc_1v1_wirenum[i]);
    }
      for (int i=0; i< Ndata_H_dc_2u1_wirenum; i++){
       wirenum = int(H_dc_2u1_wirenum[i]);
      cell_dt_uv[2][wirenum-1]->Fill(H_dc_2u1_time[i]);
     wire_vs_dt[8]->Fill(H_dc_2u1_time[i], H_dc_2u1_wirenum[i]);
    }
   for (int i=0; i< Ndata_H_dc_2v1_wirenum; i++){
       wirenum = int(H_dc_2v1_wirenum[i]);
      cell_dt_uv[3][wirenum-1]->Fill(H_dc_2v1_time[i]);
     wire_vs_dt[9]->Fill(H_dc_2v1_time[i], H_dc_2v1_wirenum[i]);
   }
     for (int i=0; i< Ndata_H_dc_1y1_wirenum; i++){
       wirenum = int(H_dc_1y1_wirenum[i]);
     cell_dt_y[0][wirenum-1]->Fill(H_dc_1y1_time[i]);
     wire_vs_dt[1]->Fill(H_dc_1y1_time[i], H_dc_1y1_wirenum[i]);
    }
    for (int i=0; i< Ndata_H_dc_1y2_wirenum; i++){
       wirenum = int(H_dc_1y2_wirenum[i]);
      cell_dt_y[1][wirenum-1]->Fill(H_dc_1y2_time[i]);
     wire_vs_dt[4]->Fill(H_dc_1y2_time[i], H_dc_1y2_wirenum[i]);
    }
     for (int i=0; i< Ndata_H_dc_2y1_wirenum; i++){
       wirenum = int(H_dc_2y1_wirenum[i]);
     cell_dt_y[2][wirenum-1]->Fill(H_dc_2y1_time[i]);
     wire_vs_dt[7]->Fill(H_dc_2y1_time[i], H_dc_2y1_wirenum[i]);
    }
   for (int i=0; i< Ndata_H_dc_2y2_wirenum; i++){
       wirenum = int(H_dc_2y2_wirenum[i]);
       cell_dt_y[3][wirenum-1]->Fill(H_dc_2y2_time[i]);
     wire_vs_dt[10]->Fill(H_dc_2y2_time[i], H_dc_2y2_wirenum[i]);
    }
//
    }  // lopp over entries
//Write wire drift time histos to file
    cout << " writing out root file" << endl;
    ix=0;
    iy=0;
    iuv=0;
for (int ip = 0; ip < NPLANES; ip++){	
  cout << ip << endl;
  root_file[ip] = run_dir;
 root_file[ip] = root_file[ip]+"/hms_DC_"+plane_names[ip]+Form("_%d.root", run);
 cout << " root file = " << root_file[ip] << endl;
 g[ip] = new TFile(root_file[ip], "RECREATE");
 // g[ip]->cd();
	if(ip==0 || ip==5 || ip==6 || ip==11) {
 	total_wires = 113;
        for (int wirenum=1; wirenum<=total_wires; wirenum++){
            cell_dt_x[ix][wirenum-1]->Write();
         }
	ix++;
	}
	if (ip==2 || ip==3 || ip==8 || ip==9) {
 	total_wires = 107;
        for (int wirenum=1; wirenum<=total_wires; wirenum++){
            cell_dt_uv[iuv][wirenum-1]->Write();
         }
	iuv++;
	}
	if(ip==1 || ip==4 || ip==7 || ip==10) {
 	total_wires = 52;
        for (int wirenum=1; wirenum<=total_wires; wirenum++){
            cell_dt_y[iy][wirenum-1]->Write();
         }
	iy++;
	}
         wire_vs_dt[ip]->Write();
	   g[ip]->Close();
    }

 cout << " done" << endl;

}
