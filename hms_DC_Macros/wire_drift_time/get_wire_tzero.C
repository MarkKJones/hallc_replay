/*Script to extract reference time "t0"  for each sense wire in a given HMS Wire Chamber Plane with COSMIC RUNS.  
20% (MAX BIN CONTENT) is calculated per wire, and the corresponding bin is fitted linearly about +/- 
a certain number of bins and this fit is extrapolated to y=0(x-axis). The extrapolated value is take to be t0*/

#include <vector>
#include <TMath>

void get_wire_tzero()
{

//Declare user-input variables 
string st_DC_num;
string st_plane;
int run;

//prompt user for input
cout << "Enter RUN Number: ";
cin >> run;
cout << "Enter HMS Drift Chamber to get t-Zero (type 1 or 2): ";
cin >> st_DC_num;
cout << "Enter Plane to get t-Zero: (type x1 y1 u1 v1 y2 x2 ) ";
cin >> st_plane;

 string st_DC_plane = st_DC_num+st_plane;
 string run_NUM = Form("run_%d", run);
 
 //Open root file containing drift time histos
TString root_file = "root_files/"+ run_NUM +"/hms_DC_"+st_DC_plane+Form("_%d.root",run);
TFile *f = new TFile(root_file,"READ");
 
//Create a file output file stream object to write t0 values to data file
ofstream ofs;
TString t_zero_file = "tzero_data/hdc_"+st_DC_plane+Form("tzero_run%d.txt", run);
ofs.open (t_zero_file);

//Set headers for subsequent columns of data
 ofs << "#WIRE " << "   "  << "t0" << "   " << "t0_err" << "   " << " entries " << endl;

//Create root file to store fitted wire drift times histos and "t0 vs. wirenum"
TString output_root_file = "root_files/"+run_NUM+"/hmsDC_"+st_DC_plane+Form("run%d_saved_histos.root", run);
TFile *g = new TFile(output_root_file,"RECREATE");

f->cd();  //change to file containing the wire drift times histos
 
int total_wires;  //integer to store total sense wires for a plane chosen by the user
   
//compare the user input string to desired planes in order to create TH1F and TH2F accordingly
if (st_plane.compare("x1")==0 || st_plane.compare("x2")==0) {  
TH1F *cell_dt[113]; //declare array of histos to store drift times     
total_wires=113; 

//Declare bin properties for given sense wires in a plane

int bin_max[113];                    /*Array to store the bin number corresponding to the drift time distribution peak*/
int bin_maxContent[113];             /*Array to store the content (# events) corresponding to the bin with maximum content*/
double time_max[113];                /*Array to store the x-axis(drift time (ns)) corresponding to bin_max*/
double twenty_perc_maxContent[113];  /*Array to store 20% of maximum bin content (peak)*/						     
double ref_time[113];               /*Array to store reference times for each sense wire*/

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
    
 	
/*Get wire histos from root file and loop over each 
sense wire of a plane in HMS Drift Chambers (DC1 or DC2)*/
 
for (int sensewire=1; sensewire<=total_wires; sensewire++){

//Get title of histos in root file
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
	

}



//****************************************************//
//Determine  which bin has around 20% max_BinContent *//
//****************************************************//


//Declarations
int content_bin;      //stores content for each bin
int counts;           //a counter used to count the number of bins that have >20% max bin content for a plane 
int bin;              //store bin number
int j;                //jth bin, used to loop over n bins 

//Declare vector arrays 
vector<int> content;               //stores bin content
vector <int> bin_num;           //stores bin number


//Loop over each wire 
for(sensewire=1; sensewire<=total_wires; sensewire++) {

    //Loop over each bin for individual wire drift time histo
	for(bin=0; bin < bin_max[sensewire-1]; bin++) {

	content_bin = cell_dt[sensewire-1]->GetBinContent(bin);              //get bin content for all bins in a wire
    
        content.push_back(content_bin);                                      //add bin content to array
        bin_num.push_back(bin);                                              //add bin number to array
    
    
    // check if 2 bin contents have been stored and examine if these contents exceed or not 20% of peak
	if (content.size() == 2) {
	
	//initialize counter to count how many bin contents >= 20%
	counts = 0;

                // Loop over 2 bin contents stored in array content
                  for (j=0; j<2; j++){

			if(content[j] > =  twenty_perc_maxContent[sensewire-1]){
				counts = counts+1;
                 			
			if(counts >= 2) { goto stop;}


			}
			
			content.clear();
            bin_num.clear();

	}
	
}
}
	
//Print the time(ns) and BIN NUM corresponding to 20% of MAX content 
//if 2/2 elements exceeds 20% of Max content (for each plane)

stop:
ref_time[sensewire-1] = cell_dt[sensewire-1] ->GetXaxis() -> GetBinCenter(bin_num[0]); //Get time corresponding ~20% Max BIN CONTENT  

//cout << " ******* " << "Wire " << sensewire << " ******* " << endl;
//cout << "time (20% of Max BIN): " << ref_time[sensewire-1] << " ns" << endl;
//cout << "BIN: " << bin_num[0] << endl;


//*********************************************************//
//*******Extract the "t0" Using a Fitting Procedure********//
//*********************************************************//

//Declarations
int time_init;           //start fit value 
int time_final;          //end fit value
int t_zero;
int entries;             //entries for each wire

double m;                //slope
double y_int;            //y-intercept
double m_err;
double y_int_err;
double t_zero_err;

//Get time corresponding to bin (fit range) 
time_init = cell_dt[sensewire-1] -> GetXaxis() -> GetBinCenter(bin_num[0]-10); 
time_final = cell_dt[sensewire-1] -> GetXaxis() -> GetBinCenter(bin_num[0]+10); 

//Create Fit Function
TF1* tZero_fit = new TF1("tZero_fit", "[0]*x + [1]", time_init, time_final);

//Set Parameter Names and Values
tZero_fit->SetParName(0, "slope");
tZero_fit->SetParName(1, "y-int");
tZero_fit->SetParameter(0, 1.0);
tZero_fit->SetParameter(1, 1.0);

//Fit Function in specified range
cell_dt[sensewire-1]->Fit("tZero_fit", "R");

//Get Parameters and their errors
m = tZero_fit->GetParameter(0);
y_int = tZero_fit->GetParameter(1);
m_err = tZero_fit->GetParError(0);
y_int_err = tZero_fit->GetParError(1);

//Calculate error on t0 using error propagation method of expanding partial derivatives
t_zero = - y_int/m;
t_zero_err = sqrt(y_int_err*y_int_err/(m*m) + y_int*y_int*m_err*m_err/(m*m*m*m) );
entries = cell_dt[sensewire-1]->GetEntries();  //number of entries (triggers) per wire

//Write "t0" values to file
ofs << sensewire << "          " << t_zero << "          " << t_zero_err << "          " << entries << endl;

//Change to output root file and write fitted histos to file
 g->cd();
cell_dt[sensewire-1]->Write();

}

// Make Plot of t0 versus Wire Number 

TCanvas *t = new TCanvas("t", "", 2000,500);
t->SetGrid();


TGraphErrors *graph = new TGraphErrors(t_zero_file, "%lg %lg %lg");
TString title = "DC"+st_DC_plane+": t0 versus sensewire";
graph->SetTitle(title);
graph->SetMarkerStyle(20);
graph->SetMarkerColor(1);
graph->GetXaxis()->SetLimits(0., total_wires);
graph->GetXaxis()->SetTitle("Wire Number");
graph->GetXaxis()->CenterTitle();
graph->GetYaxis()->SetTitle("t-Zero (ns)");
graph->GetYaxis()->CenterTitle();
graph->GetYaxis()->SetRangeUser(-50.0, 50.0);
graph->Write(title);   //write to a root file
graph->Draw("AP");

//close dat file
ofs.close();
//save plots
//TString tzero_plots = "plots/"+run_NUM +"/hdc"+st_DC_plane+Form("TESTING_tzero_v_wire_%d.eps", run);
//t->SaveAs(tzero_plots);


//*****************************************************************************************//
//        CALCULATE THE "t0s" WEIGHTED AVERAGE FOR WIRE DRIFT TIMES WITH ENTRIES > = 300   //
//*****************************************************************************************//


//open t0 dat file
  ifstream ifs;
  ifs.open (t_zero_file);
  string line;

  //open new data file from the output stream to write updated t0 values
  TString t_zero_file_corr = "tzero_data/hdc_"+st_DC_plane+Form("tzero_run%d_corr.txt", run);
  ofs.open(t_zero_file_corr);
  ofs << " #Wire " << "     " << " t_zero " << "     " << " t_zero_err " << "     " << " entries " << endl; 

//read line bt line the t_zero_file
while(getline(ifs, line)) {
	if(!line.length()|| line[0] == '#')
	   continue;
	sensewire = 0, t_zero = 0, t_zero_err = 0., entries = 0 ; //set values to zero
	sscanf(line.c_str(), "%d %d %lf %d", &sensewire, &t_zero, &t_zero_err, &entries); //assign each of the variables above a data in the t_zero_file
     //Check if entries for each sensewire exceeds a certain number of events
    if (entries>300) {
	ofs << sensewire << "        " << t_zero << "        " << t_zero_err << "        " << entries << endl;

}

}
ifs.close();

// Make Plot of t0 versus Wire Number for entries > 300 events
TCanvas *t1 = new TCanvas("t1", "", 2000,500);
t1->SetGrid();

//TString mygraph = "hdc"+st_DC_plane+Form("_t_zero_run%d.txt", run);
TGraphErrors *graph1 = new TGraphErrors(t_zero_file_corr, "%lg %lg %lg");
TString title1 = "hdc"+st_DC_plane+": t0 versus sensewire_corrected";
graph1->SetTitle(title);
graph1->SetMarkerStyle(20);
graph1->SetMarkerColor(1);
//graph1->GetXaxis()->SetLimits(0., total_wires);
graph1->GetXaxis()->SetTitle("Wire Number");
graph1->GetXaxis()->CenterTitle();
graph1->GetYaxis()->SetTitle("t-Zero (ns)");
graph1->GetYaxis()->CenterTitle();
graph1->GetYaxis()->SetRangeUser(-50.0, 50.0);
graph1->Write(title1);   //write to a root file
graph1->Draw("AP");

ofs.close();
}
