/**************************************************************************************************************************

OBSOLETE CODE IN ATTEMPT TO EXTRACT "t0s" for HMS DRIFT CHAMBER CALIBRATION: NEVERTHELESS, IT COULD SERVE AS STARTING POINT

**************************************************************************************************************************/

//Script to extract reference time "t0"  for each sense wire in a given HMS Wire Chamber Plane.  
//The "t0" is defined to be the time corresponding to 20% (MAX BIN CONTENT) of a drift time distribution in a given plane 
#include <vector>


void get_wire_refTimes()
{

//USER INPUT 
string st_DC_num;
string st_plane;
int run;

cout << "Enter RUN Number: ";
cin >> run;
cout << "Enter HMS Drift Chamber to get t-Zero (type 1 or 2): ";
cin >> st_DC_num;
cout << "Enter Plane to get t-Zero: (type x1 y1 u1 v1 y2 x2 ) ";
cin >> st_plane;

string st_DC_plane = st_DC_num+st_plane;


//Create a root file to store individual DC cell drift times
TString root_file = "hms_DC_"+st_DC_plane+Form("_%d", run)+".root";
TFile *g = new TFile(root_file, "RECREATE");
g->cd();

  
  //Open root file containing drift time histos
// TString root_file = "hms_DC_"+st_DC_plane+Form("_%d",run)+".root";
  
TFile *f = new TFile(root_file,"READ");
  f->cd();


    int total_wires;  //integer to store total sense wires for a plane chosen by the user
   
   if (st_plane.compare("x1")==0 || st_plane.compare("x2")==0) {  //compare the user input string to desired planes in order to create TH1F and TH2F accordingly
       TH1F *cell_dt[113];      
   total_wires=113; 
    //Declare bin properties for given sense wires in a plane
    int bin_max[113];                                  /*Array to store the bin number corresponding to the drift time distribution peak*/
  	int bin_maxContent[113];                           /*Array to store the content (# events) corresponding to the bin with maximum content*/
  	int time_max[113];                                 /*Array to store the x-axis(drift time (ns)) corresponding to bin_max*/
  	
  	double twenty_perc_maxContent[113];               /*Array to store 20% of maximum bin content (peak) -- the bin corresponding to 20% of max bin content 
										              //is taken to be the reference time, t0, of the drift time dist. for the wire in question*/ 
  	int ref_time[113];          //Array to store reference times for each sense wire

       }
   else if (st_plane.compare("u1")==0 || st_plane.compare("v1")==0) {
	   TH1F *cell_dt[107];
    total_wires=107;      
    int bin_max[107];                                 
  	int bin_maxContent[107];                           
  	int time_max[107];                               
  	
  	double twenty_perc_maxContent[107];                
										               
  	int ref_time[107];          

       }	   
   else if (st_plane.compare("y1")==0 || st_plane.compare("y2")==0) {
	   TH1F *cell_dt[52];
    total_wires=52;
    int bin_max[52];                                 
  	int bin_maxContent[52];                           
  	int time_max[52];                               
  	
  	double twenty_perc_maxContent[52];                
										               
  	int ref_time[52];          

	   }	 
    
 	

 //Loop over each sense wire of a plane in HMS Drift Chambers (DC1 & DC2)
 for (int sensewire=1; sensewire<=total_wires; sensewire++){

   //Title of histos in root file
   TString drift_time_histo = Form("wire_%d", sensewire); 
 
  //Get drift time histograms from root file
  cell_dt[sensewire-1] = (TH1F*)f->Get(drift_time_histo);

  //Get bin with Maximum Content
  bin_max[sensewire-1] = cell_dt[sensewire-1]->GetMaximumBin();

  //Get content of bin_max
  bin_maxContent[sensewire-1] = cell_dt[sensewire-1]->GetBinContent(bin_max[sensewire-1]);

  //Get time (ns) [x-axis] corresponding to bin_max 
  time_max[sensewire-1] = cell_dt[sensewire-1]->GetXaxis()->GetBinCenter(bin_max[sensewire-1]);

  //Calculate 20% of max content
  twenty_perc_maxContent[sensewire-1] = bin_maxContent[sensewire-1] * 0.20;

  //PRINT DRIFT TIME PEAK PROPERTIES for each plane
 
//  fileout << "" << endl;
//  fileout << "===== " << "DC XPLANE" << Form("wire_%d", sensewire) << " ======" << endl;
//  fileout << "" << endl;
//  fileout << "MAX BIN: " << bin_max[sensewire-1]  << endl;
//  fileout << "Maximum BIN Content: " << bin_maxContent[sensewire-1] << endl;
//  fileout << "MAX Drift Time: " << time_max[sensewire-1] << " ns " <<endl;
//  fileout << "20 % of MAX BIN CONTENT is " << twenty_perc_maxContent[sensewire-1] << endl;

	
}

//****************************************************
//Determine  which bin has around 20% max_BinContent *
//****************************************************


//Declarations
int content_bin;      //stores content for each bin
int counts;           //a counter used to count the number of bins that have >20% max bin content for a plane 
int bin;              //store bin number
int j;                //jth bin, used to loop over 5 bins 
int ymax;

//Declare vector arrays 
vector<int> content;               //stores bin content
vector <int> bin_num;           //stores bin number


TCanvas *c = new TCanvas("c", "", 2000, 2000);
c->Divide(10,12);

TLine *l;

//Create txt file to output t0
ofstream myfile;
TString t_zero_file = "tzero_dat/TESTING_hdc"+st_DC_plane+Form("_tzero_run%d.txt", run);
myfile.open (t_zero_file);
myfile << "WIRE " << "   "  << "Reference Time" <<  endl;

//Create file to store drift times with t0 plots
TString tzero_plots ="plots/hdc"+st_DC_plane+Form("tzeroTEST_run%d.eps", run);

//g->cd();
//Loop over each wire 
for(sensewire=1; sensewire<=total_wires; sensewire++) {

    //Loop over each bin in a plane
	for(bin=0; bin< bin_max[sensewire]; bin++) {

	content_bin = cell_dt[sensewire-1]->GetBinContent(bin);               //get bin content for all bins in a plane
    
    content.push_back(content_bin);                       //add bin content to array
    bin_num.push_back(bin);                               //add bin number to array
    
    
    // check if 5 bin contents have been stored and examine if these contents exceed or not 20% of peak
	if (content.size() == 2) {
	
	//initialize counter to count how many bin contents >= 20%
	counts = 0;

                // Loop over 2 bin contents stored in array content
                  for (j=0; j<2; j++){

			if(content[j] > =  twenty_perc_maxContent[sensewire-1]){
				counts = counts+1;
                 			
			if(counts >= 2) { goto stop; ; }


			}
			
			content.clear();
            bin_num.clear();

	}
	
}
}
	
// Print the reference times if 2/2 elements exceeds 20% of Max content (for each plane)
stop:
ref_time[sensewire-1] = cell_dt[sensewire-1] ->GetXaxis() -> GetBinCenter(bin_num[0]-5); //subtract 5 bins, to be consistent with macroscopic DC1X1 t0, as this same number of bins was subtracted from it. 
//myfile << "==== DC " <<  Form("WIRE_%d", sensewire) << "=========" << endl; 
//myfile << "BIN: " << bin_num[0] << endl;
//cout << "CONTENT: " << content[0] << endl;
//myfile << "Reference Time t0: " << ref_time[sensewire-1] << "ns" <<endl; 
//myfile << "==================== " << endl;
myfile << Form("%d", sensewire) << "         " << ref_time[sensewire-1] << endl;

//Plot drift time distributions
c->cd(sensewire);
cell_dt[sensewire-1]->Draw();
c->Update();
ymax = gPad->GetUymax();
l = new TLine(ref_time[sensewire-1],0,ref_time[sensewire-1],ymax);
l->SetLineColor(kRed);
l->Draw();
c->SaveAs(tzero_plots);

//g->Append(h[sensewire-1]);

}

//g->Write();
myfile.close();

//Make Plot of t0 vs. wire num

	TCanvas *t = new TCanvas("t", "", 2000,500);
	t->SetGrid();
	//TString mygraph = "hdc"+st_DC_plane+Form("_t_zero_run%d.txt", run);
	TGraph *graph = new TGraph(t_zero_file, "%lg %lg");
	TString title = "DC"+st_DC_plane+": t0 versus sensewire";
	graph->SetTitle(title);
	graph->SetMarkerStyle(20);
    graph->SetMarkerColor(1);
    graph->GetXaxis()->SetLimits(0., total_wires);
    graph->GetXaxis()->SetTitle("Wire Number");
    graph->GetXaxis()->CenterTitle();
    graph->GetYaxis()->SetTitle("t-Zero (ns)");
    graph->GetYaxis()->CenterTitle();
    graph->Draw("AP");
    
    TString my_plot = "plots/hdc"+st_DC_plane+Form("tzero_v_wire_%d.eps", run);
    t->SaveAs(my_plot);





}
