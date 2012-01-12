// Caustics Code
// This code genertates simulated caustics below a water surface
// John Novak July 2011

#ifndef __CINT__
#include <iostream>
#include <iomanip>
#include <math.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TStyle.h>
#include <TH2.h>
#include <time.h>
#endif


using namespace std;

// Variables
float surface[500][500]; // heigth of the surface
float derivative[500][500][2]; // derivative of the height of the surface, has extra index because the surface derivative is directional
float bottom[500][500]; // intensity of light at points on the bottom, if color is implemented this could be expanded by an index which could hold the data for each color
int seed;
float A,k,w; // A amplitude, k wave scalar, w frequency
float xnaught, ynaught, x, y, tnaught, t;
int h,i,j,i2,j2,iterations;
float height;
float wScale,kScale,depth;
FILE *input;
char bottomname[40];
char topname[40];
double timet;

// Prototypes
void ZeroArrays(void);
void generatesurface(void);
float wavefunction(float A, float k, float w, float xnaught, float ynaught, float x, float y, float tnaught);
float wavefunctionderivativex(float A, float k, float w, float xnaught, float ynaught, float x, float y, float tnaught);
float wavefunctionderivativey(float A, float k, float w, float xnaught, float ynaught, float x, float y, float tnaught);
void fillsurfaceplots(void);
void dodiffraction(void);

TCanvas *c1;
TCanvas *c2;

TH2 *surfaceHist;
TH2 *bottomHist;

// Main loop
int main(){
	seed=822008;
	srand(seed);
	//timet=time(NULL);
	//srand(timet);
	srand(time(NULL));
	
	input=fopen("input.txt","r");
	fscanf(input,"%d",&iterations);
	fscanf(input,"%d",&wScale);
	fscanf(input,"%d",&kScale);
	fscanf(input,"%d",&depth);
	fclose(input);
	
	c1 = new TCanvas("c1","Surface Plot",10,10,2500,2500);
	surfaceHist = new TH2F("Surface","Surface",500,0,500,500,0,500);
	c2 = new TCanvas("c2","Surface Plot",10,10,2500,2500);
	bottomHist = new TH2F("Bottom","Bottom",500,0,500,500,0,500);
	
	ZeroArrays();
	
	for(h=0;h<iterations;h++){
		generatesurface();
	}
	
	dodiffraction();
	
	fillsurfaceplots();
	c1->cd();
	surfaceHist->Draw("COL");
	c2->cd();
	bottomHist->Draw("CONT");
	sprintf(topname,"Caustics Plots/Surface_%d.jpg",time(NULL));
	sprintf(bottomname,"Caustics Plots/Bottom_%d.jpg",time(NULL));
	c1->Print(topname);
	c2->Print(bottomname);
	
}

// Functions
void ZeroArrays(void){
	for(i=0;i<500;i++){
		for(j=0;j<500;j++){
			surface[i][j]=0;
			bottom[i][j]=0;
			derivative[i][j][0]=0;
			derivative[i][j][1]=0;
		}
	}
	cout << "Arrays Zeroed" << endl;
}

void generatesurface(void){
	A=(rand() % 150) + 50;
	w=wScale/A;
	k=w*w/kScale;
	
	xnaught=(float)((rand() % 1000) - 500);
	ynaught=(float)((rand() % 1000) - 500);
	tnaught=(float)((rand() % 10));
	
	for(i=0;i<500;i++){
		i2=i-250;
		for(j=0;j<500;j++){
			j2=j-250;
			surface[i][j]=surface[i][j]+wavefunction(A,k,w,xnaught,ynaught,i2,j2,tnaught);
			derivative[i][j][0]=derivative[i][j][0]+wavefunctionderivativex(A,k,w,xnaught,ynaught,i2,j2,tnaught);
			derivative[i][j][1]=derivative[i][j][1]+wavefunctionderivativey(A,k,w,xnaught,ynaught,i2,j2,tnaught);
		}
	}
}

float wavefunction(float A, float k, float w, float xnaught, float ynaught, float x, float y, float tnaught){
	height = A*cos(k*sqrt((x-xnaught)*(x-xnaught)+(y-ynaught)*(y-ynaught))+w*(tnaught))/sqrt((x-xnaught)*(x-xnaught)+(y-ynaught)*(y-ynaught))*exp(-w*tnaught);
	return height;
}

float wavefunctionderivativex(float A, float k, float w, float xnaught, float ynaught, float x, float y, float tnaught){
	height = A*(xnaught-x)*(k*sqrt((xnaught-x)*(xnaught-x)+(y-ynaught)*(y-ynaught))*sin(k*sqrt((xnaught-x)*(xnaught-x)+(y-ynaught)*(y-ynaught)))+cos(k*sqrt((xnaught-x)*(xnaught-x)+(y-ynaught)*(y-ynaught))))/sqrt(((xnaught-x)*(xnaught-x)+(y-ynaught)*(y-ynaught))*((xnaught-x)*(xnaught-x)+(y-ynaught)*(y-ynaught))*((xnaught-x)*(xnaught-x)+(y-ynaught)*(y-ynaught)))*exp(-w*tnaught);
	return height;
}

float wavefunctionderivativey(float A, float k, float w, float xnaught, float ynaught, float x, float y, float tnaught){
	height = A*(ynaught-y)*(k*sqrt((x-xnaught)*(x-xnaught)+(ynaught-y)*(ynaught-y))*sin(k*sqrt((x-xnaught)*(x-xnaught)+(ynaught-y)*(ynaught-y)))+cos(k*sqrt((x-xnaught)*(x-xnaught)+(ynaught-y)*(ynaught-y))))/sqrt(((x-xnaught)*(x-xnaught)+(ynaught-y)*(ynaught-y))*((x-xnaught)*(x-xnaught)+(ynaught-y)*(ynaught-y))*((x-xnaught)*(x-xnaught)+(ynaught-y)*(ynaught-y)))*exp(-w*tnaught);
	return height;
}

void fillsurfaceplots(void){
	for(i=0;i<500;i++){
		for(j=0;j<500;j++){
			surfaceHist->Fill(i,j,surface[i][j]);
			bottomHist->Fill(i,j,bottom[i][j]);
			//cout << " " << bottom[i][j];
		}
	}
}

/*void dodiffraction(void){
//	double thetax1,thetay1,thetax2,thetay2;
	float stepx,stepy;
	
	for(i=0;i<500;i++){
		for(j=0;j<500;j++){
			thetax1=atan((double)derivative[i][j][0]);
			thetay1=atan((double)derivative[i][j][1]);
			thetax2=asin(4*sin(thetax1)/3);
			thetay2=asin(4*sin(thetay1)/3);
			stepx=tan(thetax2)*(depth+(double)surface[i][j]);
			stepy=tan(thetay2)*(depth+(double)surface[i][j]);
			//stepx=derivative[i][j][0]*(depth+surface[i][j]);
			//stepy=derivative[i][j][1]*(depth+surface[i][j]);
			i2=i+(int)stepx;
			j2=j+(int)stepy;
			if( (i2 >= 0) && (i2 < 500) && (j2 >= 0) && (j2 < 500)) bottom[i2][j2]++;
		}
	}
}*/

void dodiffraction(void){
	float stepx[4],stepy[4];
	int a[2],b[2],c[2],d[2],cor[4][2];
	
	for(i=0;i<500;i++){
		for(j=0;j<500;j++){
			stepx[0]=tan(asin(4*sin(atan((double)derivative[i][j][0]))/3))*(depth+(double)surface[i][j]);
			stepy[0]=tan(asin(4*sin(atan((double)derivative[i][j][1]))/3))*(depth+(double)surface[i][j]);
			stepx[1]=tan(asin(4*sin(atan((double)derivative[i+1][j][0]))/3))*(depth+(double)surface[i+1][j]);
			stepy[1]=tan(asin(4*sin(atan((double)derivative[i+1][j][1]))/3))*(depth+(double)surface[i+1][j]);
			stepx[2]=tan(asin(4*sin(atan((double)derivative[i][j+1][0]))/3))*(depth+(double)surface[i][j+1]);
			stepy[2]=tan(asin(4*sin(atan((double)derivative[i][j+1][1]))/3))*(depth+(double)surface[i][j+1]);
			stepx[3]=tan(asin(4*sin(atan((double)derivative[i+1][j+1][0]))/3))*(depth+(double)surface[i+1][j+1]);
			stepy[3]=tan(asin(4*sin(atan((double)derivative[i+1][j+1][1]))/3))*(depth+(double)surface[i+1][j+1]);
			/*stepx[0]=derivative[i][j][0]*(depth+surface[i][j]);
			stepy[0]=derivative[i][j][1]*(depth+surface[i][j]);
			stepx[1]=derivative[i+1][j][0]*(depth+surface[i+1][j]);
			stepy[1]=derivative[i+1][j][1]*(depth+surface[i+1][j]);
			stepx[2]=derivative[i][j+1][0]*(depth+surface[i][j+1]);
			stepy[2]=derivative[i][j+1][1]*(depth+surface[i][j+1]);
			stepx[3]=derivative[i+1][j+1][0]*(depth+surface[i+1][j+1]);
			stepy[3]=derivative[i+1][j+1][1]*(depth+surface[i+1][j+1]);*/
			cor[0][0]=i+stepx[0];
			cor[0][1]=j+stepy[0];
			cor[1][0]=i+stepx[1]+1;
			cor[1][1]=j+stepy[1];
			cor[2][0]=i+stepx[2];
			cor[2][1]=j+stepy[2]+1;
			cor[3][0]=i+stepx[3]+1;
			cor[3][1]=j+stepy[3]+1;
			if((stepx[0]>=(stepx[1]+1))&&(stepx[0]>=stepx[2])&&(stepx[0]>=(stepx[3]+1))){
				a[0]=cor[0][0];
				a[1]=cor[0][1];
				if(((stepx[1]+1)<=stepx[2])&&(stepx[1]<=stepx[3])){
					d[0]=cor[1][0];
					d[1]=cor[1][1];
					if(stepy[2]>=stepy[3]){
						b[0]=cor[2][0];
						b[1]=cor[2][1];
						c[0]=cor[3][0];
						c[1]=cor[3][1];
					}
					if(stepy[2]<=stepy[3]){
						b[0]=cor[3][0];
						b[1]=cor[3][1];
						c[0]=cor[2][0];
						c[1]=cor[2][1];
					}
				}
				if((stepx[2]<=(stepx[1]+1))&&(stepx[2]<=(stepx[3]+1))){
					d[0]=cor[2][0];
					d[1]=cor[2][1];
					if(stepy[1]>=(stepy[3]+1)){
						b[0]=cor[1][0];
						b[1]=cor[1][1];
						c[0]=cor[3][0];
						c[1]=cor[3][1];
					}
					if(stepy[1]<=(stepy[3]+1)){
						b[0]=cor[3][0];
						b[1]=cor[3][1];
						c[0]=cor[1][0];
						c[1]=cor[1][1];
					}
				}
				if(((stepx[3]+1)<=stepx[2])&&(stepx[3]<=stepx[1])){
					d[0]=cor[3][0];
					d[1]=cor[3][1];
					if(stepy[1]>=(stepy[2]+1)){
						b[0]=cor[1][0];
						b[1]=cor[1][1];
						c[0]=cor[2][0];
						c[1]=cor[2][1];
					}
					if(stepy[1]<=(stepy[2]+1)){
						b[0]=cor[2][0];
						b[1]=cor[2][1];
						c[0]=cor[1][0];
						c[1]=cor[1][1];
					}
				}
			}
			if(((stepx[1]+1)>=stepx[0])&&((stepx[1]+1)>=stepx[2])&&(stepx[1]>=stepx[3])){
				a[0]=cor[1][0];
				a[1]=cor[1][1];
				if((stepx[0]<=stepx[2])&&(stepx[0]<=(stepx[3]+1))){
					d[0]=cor[0][0];
					d[1]=cor[0][1];
					if(stepy[2]>=stepy[3]){
						b[0]=cor[2][0];
						b[1]=cor[2][1];
						c[0]=cor[3][0];
						c[1]=cor[3][1];
					}
					if(stepy[2]<=stepy[3]){
						b[0]=cor[3][0];
						b[1]=cor[3][1];
						c[0]=cor[2][0];
						c[1]=cor[2][1];
					}
				}
				if((stepx[2]<=stepx[0])&&(stepx[2]<=(stepx[3]+1))){
					d[0]=cor[2][0];
					d[1]=cor[2][1];
					if(stepy[0]>=(stepy[3]+1)){
						b[0]=cor[0][0];
						b[1]=cor[0][1];
						c[0]=cor[3][0];
						c[1]=cor[3][1];
					}
					if(stepy[0]<=(stepy[3]+1)){
						b[0]=cor[3][0];
						b[1]=cor[3][1];
						c[0]=cor[0][0];
						c[1]=cor[0][1];
					}
				}
				if(((stepx[3]+1)<=stepx[2])&&((stepx[3]+1)<=stepx[0])){
					d[0]=cor[3][0];
					d[1]=cor[3][1];
					if(stepy[0]>=(stepy[2]+1)){
						b[0]=cor[0][0];
						b[1]=cor[0][1];
						c[0]=cor[2][0];
						c[1]=cor[2][1];
					}
					if(stepy[0]<=(stepy[2]+1)){
						b[0]=cor[2][0];
						b[1]=cor[2][1];
						c[0]=cor[0][0];
						c[1]=cor[0][1];
					}
				}
			}
			if((stepx[2]>=(stepx[1]+1))&&(stepx[2]>=stepx[0])&&(stepx[2]>=(stepx[3]+1))){
				a[0]=cor[2][0];
				a[1]=cor[2][1];
				if(((stepx[1]+1)<=stepx[0])&&(stepx[1]<=stepx[3])){
					d[0]=cor[1][0];
					d[1]=cor[1][1];
					if(stepy[0]>=(stepy[3]+1)){
						b[0]=cor[0][0];
						b[1]=cor[0][1];
						c[0]=cor[3][0];
						c[1]=cor[3][1];
					}
					if(stepy[0]<=(stepy[3]+1)){
						b[0]=cor[3][0];
						b[1]=cor[3][1];
						c[0]=cor[0][0];
						c[1]=cor[0][1];
					}
				}
				if((stepx[0]<=(stepx[1]+1))&&(stepx[0]<=(stepx[3]+1))){
					d[0]=cor[0][0];
					d[1]=cor[0][1];
					if(stepy[1]>=(stepy[3]+1)){
						b[0]=cor[1][0];
						b[1]=cor[1][1];
						c[0]=cor[3][0];
						c[1]=cor[3][1];
					}
					if(stepy[1]<=(stepy[3]+1)){
						b[0]=cor[3][0];
						b[1]=cor[3][1];
						c[0]=cor[1][0];
						c[1]=cor[1][1];
					}
				}
				if(((stepx[3]+1)<=stepx[0])&&(stepx[3]<=stepx[1])){
					d[0]=cor[3][0];
					d[1]=cor[3][1];
					if(stepy[1]>=stepy[0]){
						b[0]=cor[1][0];
						b[1]=cor[1][1];
						c[0]=cor[0][0];
						c[1]=cor[0][1];
					}
					if(stepy[1]<=stepy[0]){
						b[0]=cor[0][0];
						b[1]=cor[0][1];
						c[0]=cor[1][0];
						c[1]=cor[1][1];
					}
				}
			}
			if((stepx[3]>=stepx[1])&&((stepx[3]+1)>=stepx[2])&&((stepx[3]+1)>=stepx[0])){
				a[0]=cor[3][0];
				a[1]=cor[3][1];
				if(((stepx[1]+1)<=stepx[2])&&((stepx[1]+1)<=stepx[0])){
					d[0]=cor[1][0];
					d[1]=cor[1][1];
					if((stepy[2]+1)>=stepy[0]){
						b[0]=cor[2][0];
						b[1]=cor[2][1];
						c[0]=cor[0][0];
						c[1]=cor[0][1];
					}
					if((stepy[2]+1)<=stepy[0]){
						b[0]=cor[0][0];
						b[1]=cor[0][1];
						c[0]=cor[2][0];
						c[1]=cor[2][1];
					}
				}
				if((stepx[2]<=(stepx[1]+1))&&(stepx[2]<=stepx[0])){
					d[0]=cor[2][0];
					d[1]=cor[2][1];
					if(stepy[1]>=stepy[0]){
						b[0]=cor[1][0];
						b[1]=cor[1][1];
						c[0]=cor[0][0];
						c[1]=cor[0][1];
					}
					if(stepy[1]<=stepy[0]){
						b[0]=cor[0][0];
						b[1]=cor[0][1];
						c[0]=cor[1][0];
						c[1]=cor[1][1];
					}
				}
				if((stepx[0]<=stepx[2])&&(stepx[0]<=(stepx[1]+1))){
					d[0]=cor[0][0];
					d[1]=cor[0][1];
					if(stepy[1]>=(stepy[2]+1)){
						b[0]=cor[1][0];
						b[1]=cor[1][1];
						c[0]=cor[2][0];
						c[1]=cor[2][1];
					}
					if(stepy[1]<=(stepy[2]+1)){
						b[0]=cor[2][0];
						b[1]=cor[2][1];
						c[0]=cor[1][0];
						c[1]=cor[1][1];
					}
				}
			}
			/*cout << a[0] << " " << a[1] << endl;
			cout << b[0] << " " << b[1] << endl;
			cout << c[0] << " " << c[1] << endl;
			cout << d[0] << " " << d[1] << endl;*/
			cout << stepx[0] << " " << stepy[0] << endl;
			for(i2=d[0];i2<min(b[0],c[0]);i2++){
				for(j2=(int)(d[1]+((c[1]-d[1])/(c[0]-d[0])*(i2-d[0])));j2<(int)(d[1]+((b[1]-d[1])/(b[0]-d[0])*(i2-d[0])));j2++){
					if( (i2 >= 0) && (i2 < 500) && (j2 >= 0) && (j2 < 500)) bottom[i2][j2]+=20/(abs(a[0]-d[0])*abs(b[0]-c[0]));
					cout << (abs(a[0]-d[0])*abs(b[0]-c[0])) << "test 1" << endl ;
				}
				cout << (abs(a[0]-d[0])*abs(b[0]-c[0])) << "test 1" << endl ;
			}
			for(i2=min(b[0],c[0]);i2<max(b[0],c[0]);i2++){
				if(b[0]>c[0]){
					for(j2=(int)(c[1]+((a[1]-c[1])/(a[0]-c[0])*(i2-c[0])));j2<(int)(d[1]+((b[1]-d[1])/(b[0]-d[0])*(i2-d[0])));j2++){
						if( (i2 >= 0) && (i2 < 500) && (j2 >= 0) && (j2 < 500)) bottom[i2][j2]+=20/(abs(a[0]-d[0])*abs(b[0]-c[0]));
						cout << (abs(a[0]-d[0])*abs(b[0]-c[0])) << "test 2" << endl ;
					}
					cout << (abs(a[0]-d[0])*abs(b[0]-c[0])) << "test 2" << endl ;
				}
				if(c[0]>b[0]){
					for(j2=(int)(d[1]+((c[1]-d[1])/(c[0]-d[0])*(i2-d[0])));j2<(int)(b[1]+((a[1]-b[1])/(a[0]-b[0])*(i2-b[0])));j2++){
						if( (i2 >= 0) && (i2 < 500) && (j2 >= 0) && (j2 < 500)) bottom[i2][j2]+=20/(abs(a[0]-d[0])*abs(b[0]-c[0]));
						cout << (abs(a[0]-d[0])*abs(b[0]-c[0])) << "test 3" << endl ;
					}
				}
			}
			for(i2=max(b[0],c[0]);i2<a[0];i2++){
				for(j2=(int)(c[1]+((a[1]-c[1])/(a[0]-c[0])*(i2-c[0])));j2<(int)(b[1]+((a[1]-b[1])/(a[0]-b[0])*(i2-b[0])));j2++){
					if( (i2 >= 0) && (i2 < 500) && (j2 >= 0) && (j2 < 500)) bottom[i2][j2]+=20/(abs(a[0]-d[0])*abs(b[0]-c[0]));
					cout << (abs(a[0]-d[0])*abs(b[0]-c[0])) << "test 4" << endl ;
				}
				cout << (abs(a[0]-d[0])*abs(b[0]-c[0])) << "test 4" << endl ;
			}
		}
	}
}