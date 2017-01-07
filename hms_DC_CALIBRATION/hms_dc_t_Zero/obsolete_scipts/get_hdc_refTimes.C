//Script to extract reference time "t0"  for each HMS Wire Chamber Plane.  
//The "t0" is defined to be the time corresponding to 20% (MAX BIN CONTENT) minus (a few bins) such as to place the histo at ~ 0 ns. 
//This criteria was determined from past experiments in Hall C
//All mesured times in each plane will be determined relative to t0

#include <vector>
#define NPLANES 12



void get_hdc_refTimes()
{
  int run_NUM;
  //Promt User for Input
  cout << "Enter Run Number: ";
  cin >> run_NUM;
  
  //Open root file containing drift time histos
  TFile *f = new TFile(Form("hms_dc_time_%d.root", run_NUM),"READ");
 
 //Define histogram array
 TH1F* h[NPLANES];
 
 //Define the number Drift Chamber planes
 TString plane_names[NPLANES]={"1x1", "1y1", "1u1", "1v1", "1y2", "1x2", "2x1", "2y1", "2u1", "2v1", "2y2", "2x2"};
	
  //Declare bin properties
  int bin_max[NPLANES];					/*Array to store the bin number corresponding to the drift time distribution peak*/
  int bin_maxContent[NPLANES];          /*Array to store the content (# events) corresponding to the bin with maximum content*/
  int time_max[NPLANES];                /*Array to store the x-axis(drift time (ns)) corresponding to bin_max*/
  
  double twenty_perc_maxContent[NPLANES];     /*Array to store 20% of maximum bin content (peak) -- the bin corresponding to 20% of max bin content 
										      is taken to be the reference time, t0, of the drift time dist. for the plane in question*/ 
  	
 //Loop over each plane of HMS Drift Chambers (DC1 & DC2)
 for (int ip=0; ip<NPLANES; ip++){

   TString drift_time_histo = "hdc"+plane_names[ip]+"_time"; 
 
  //Get drift time histograms from root file
  h[ip] = (TH1F*)f->Get(drift_time_histo);

  //Get bin with Maximum Content
  bin_max[ip] = h[ip]->GetMaximumBin();

  //Get content of bin_max
  bin_maxContent[ip] = h[ip]->GetBinContent(bin_max[ip]);

  //Get time (ns) [x-axis] corresponding to bin_max 
  time_max[ip] = h[ip]->GetXaxis()->GetBinCenter(bin_max[ip]);

  //Calculate 20% of max content
  twenty_perc_maxContent[ip] = bin_maxContent[ip] * 0.20;

  //PRINT DRIFT TIME PEAK PROPERTIES for each plane
  cout << "" << endl;
  cout << "===== " << "DC" << plane_names[ip] << " ======" << endl;
  cout << "" << endl;
  cout << "MAX BIN: " << bin_max[ip]  << endl;
  cout << "Maximum BIN Content: " << bin_maxContent[ip] << endl;
  cout << "MAX Drift Time: " << time_max[ip] << " ns " <<endl;
  cout << "20 % of MAX BIN CONTENT is " << twenty_perc_maxContent[ip] << endl;

	
}

//****************************************************
//Determine  which bin has around 20% max_BinContent *
//****************************************************


//Declarations
int content_bin;      //stores content for each bin
int counts;           //a counter used to count the number of bins that have >20% max bin content for a plane 
int bin;              //store bin number
int j;                //jth bin, used to loop over 5 bins 
int ref_time[NPLANES];          //Array to store reference times for each plane
int ymax;

//Declare vector arrays 
vector<int> content;               //stores bin content
vector <int> bin_num;           //stores bin number


TCanvas *c = new TCanvas();
c->Divide(3,4);

TLine *l;

//Create txt file to output t0
ofstream myfile;
myfile.open (Form("t_zero_run%d.txt", run_NUM));
myfile << "PLANE " << "   "  << "Reference Time" <<  endl;

//Loop over each wire chamber plane
for(ip=0; ip<NPLANES; ip++) {

    //Loop over each bin in a plane
	for(bin=0; bin< bin_max[ip]; bin++) {

	content_bin = h[ip]->GetBinContent(bin);               //get bin content for all bins in a plane
    
    content.push_back(content_bin);                       //add bin content to array
    bin_num.push_back(bin);                               //add bin number to array
    
    
    // check if 5 bin contents have been stored and examine if these contents exceed or not 20% of peak
	if (content.size() == 3) {
	
	//initialize counter to count how many bin contents >= 20%
	counts = 0;

                // Loop over 5 bin contents stored in array content
                  for (j=0; j<3; j++){
          
			if(content[j] > =  twenty_perc_maxContent[ip]){
				counts = counts+1;
                 			
			if(counts >= 2) { goto stop;}
	 

			}
			
			content.clear();
            bin_num.clear();

	}
	
}
}
	
// Print the reference times if 2/2 elements exceeds 20% of Max content (for each plane)
stop:
ref_time[ip] = h[ip] ->GetXaxis() -> GetBinCenter(bin_num[0]-5); //subtract a few bins, to obtain true t0
cout << "==== DC" <<  plane_names[ip] << "=========" << endl; 
cout << "BIN: " << bin_num[0]-5 << endl;
//cout << "CONTENT: " << content[0] << endl;
cout << "Reference Time t0: " << ref_time[ip] << "ns" <<endl; 
cout << "==================== " << endl;

  myfile << plane_names[ip] << std::setw(10) << ref_time[ip] << endl;
 // myfile << "Writing this to a file.\n";
 // myfile << "Writing this to a file.\n";
 // myfile << "Writing this to a file.\n";
//  myfile.close();

//Plot drift time distributions
c->cd(ip+1);
h[ip]->Draw();
c->Update();
ymax = gPad->GetUymax();
l = new TLine(ref_time[ip],0,ref_time[ip],ymax);
l->SetLineColor(kRed);
l->Draw();
}

//Save histograms 
 c->SaveAs(Form(/*"plots/ref_time_run%d.eps"*/ "plots/testing_%d.eps", run_NUM));

  myfile.close();



			     

}




