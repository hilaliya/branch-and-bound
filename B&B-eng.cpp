#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<cmath>
#include<string.h>
using namespace std;

class tree{
	public:
		double secx;         // the variable chosen for prunning
		double mom[12][12]; // BFS
		double xmom[12];   // x deðerleri;
		double zmom;      // optimal value
		int kst1;        // constraint included for child1
		int kst2;	    // constraint included for child2
		double child1[12][12];
		double child2[12][12];
		double xchild1[12];
		double zchild1;
		double xchild2[12];
		double zchild2;
		int ctrl1;  //control for prunnig child1   (-1: infeasible solution,  0: no improvement in z-value  1: optimal solution)
		int ctrl2;  //control for prunnig child2
};


int main(){

 char tip[4];  //Keeps the type of problem (max / min)
	cout<<"Enter the problem type (max or min): ";
	cin>>tip;
	
    int N;   //number of variables
	int m;	//number of constraints
	cout<<"\nEnter the number of variables: ";
	cin>>N;
	cout<<endl;
		
	double X[N][1];   //keeps optimal values of variable values
	for(int i; i<N; i++){X[i][0]=0;}
	

	char ozx[N]; 
	
	for(int i=0; i<N; i++){
		cout<<"Enter '<' if the  "<<i+1<<". variable is less than zero, '>' if greater than zero, and '0' if unbounded: ";
     	cin>>ozx[i];
	}
	cout<<endl;
	
	double C[1][N];  // objective function coefficients
	for(int i=0; i<N; i++)
	{
		cout<<"Enter the coefficient of the "<<i+1<<". variable in the objective function: ";
    	cin>>C[0][i];
	}

	cout<<"\nEnter the number of constraints:";
	cin>>m;
	cout<<endl;

	double matrix[m][N]; //
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<N;j++)
		{
			cout<<"Enter the coefficient of the "<<j+1<<". variable of the " <<i+1<<". constraint: ";  
			cin>>matrix[i][j];
		}
	}
		
	int n=N; 
	for(int i=0; i<n; i++)
	{	
		if(ozx[i]=='0'){n=n+1;}			
	}
	
	double newmatrix[m][n];  //The new matrix formed if any of the variables are <0 or infinite
	double c[1][n];       //New objective function coefficients when any of the variables are <0 or infinite
	
	for(int i=0; i<m; i++)
	{
		for(int j=0; j<n; j++)
		{
			newmatrix[i][j]=0;
		}
	}
	
	int y=0;
	for(int i=0; i<n;i++)
	{	
		if(ozx[i-y]=='<')
		{
			c[0][i]=(-1)*C[0][i-y];
			for(int j=0; j<m; j++)
			{
				newmatrix[j][i]=(-1)*matrix[j][i-y];
			}	
		}
		
		if(ozx[i-y]=='>')
		{
			c[0][i]=C[0][i-y];		
			for(int j=0; j<m; j++)
			{
				newmatrix[j][i]=matrix[j][i-y];
			
			}
		}
		if(ozx[i-y]=='s')
		{	
			c[0][i]=C[0][i-y];
			c[0][i+1]=(-1)*C[0][i-y];		
			for(int j=0; j<m; j++)
			{
				newmatrix[j][i]=matrix[j][i-y];
				newmatrix[j][i+1]=(-1)*matrix[j][i-y];
			}
			i=i+1;
			y=y+1;	
		}
	}
	
	for(int i=0; i<N; i++)
	{
		if(ozx[i]=='s' || ozx[i]=='k')
		{
			cout<<endl<<"\n Updated matrix: \n";
			for(int i=0; i<m; i++)
			{
				for(int j=0; j<n; j++)
				{
					cout<<" "<<newmatrix[i][j];
				}
				cout<<endl;
			}	
		}
	}
		

	double RHS[m+1][1];   // one-dimensional matrix holding right-hand side constraints
	RHS[0][0]=0;		
	cout<<endl;
	
	for(int i=1; i<m+1; i++)
	{
		cout<<"Enter the right-hand side of the "<<i<<". constraint: ";
    	cin>>RHS[i][0];
	}

    char sign[m];    // Keeps signs of constraints
	for(int i=0; i<m; i++)
	{
		cout<<endl<<". Enter the sign of the " <<i+1<<". constraint ( < , > , = ) : ";
    	cin>>sign[i];
    }

	char isrt[3]={'=','<','>'};
	for(int i=1;i<=m;i++)
	{
		if(RHS[i][0]<0){RHS[i][0]=(-1)*RHS[i][0];
		if(sign[i-1]==isrt[1]){ sign[i-1]=isrt[2];}
		else if (sign[i-1]==isrt[2]){ sign[i-1]=isrt[1];}
		for(int j=0;j<n;j++){newmatrix[i][j]=(-1)*newmatrix[i][j];}}
	}
	
    int sk=0;  //number of slack variables
    for(int i=0;i<m;i++){if(sign[i]==isrt[1]){sk=sk+1; }}
	cout<<"\nnumber of slack variables: "<<sk<<endl;

	int ak=0;  // number of residual variables
	for(int i=0;i<m;i++){if(sign[i]==isrt[2]){ak=ak+1;  }}
	cout<<"number of residual variables: "<<ak<<endl;	

	int yk=0;  //number of artificial variables
	for(int i=0;i<m;i++){if(sign[i]==isrt[0] || sign[i]==isrt[2]){yk=yk+1; }}
    cout<<"number of artificial variables: "<<yk<<endl;	

    double z[1][n+sk+ak+yk];      // row of the objective function
    double brm[m][sk];     
    double BFS0[m][n+sk+ak+yk];
    double BFS1[m+1][n+sk+ak+yk];
    double BFS[m+1][n+sk+ak+yk+1];  //basic feasible solution
    

double M=100;

	//BFS0	
	for(int i=0; i<m; i++)
	{	
		for(int j=0; j<n; j++)
		{
			BFS0[i][j]=newmatrix[i][j];
		}	
	}

	for(int i=0; i<m; i++)
	{
		for(int j=n; j<n+sk+ak+yk; j++)
		{
			 BFS0[i][j]=0;  		
		}
	}
	

	int q=0;
	for(int i=0; i<m; i++)
	{
		if(sign[i]==isrt[0]) {BFS0[i][n+i+q]=1;}  // =
		else if(sign[i]==isrt[1]){ BFS0[i][n+i+q]=1; } // <
		else if(sign[i]==isrt[2]) {BFS0[i][n+i+q]=-1;  BFS0[i][n+i+q+1]=1; q=q+1;}   //  >
	}
	
	
	//z row
	for(int i=0; i<n; i++)   z[0][i]=(-1)*c[0][i];
	for(int i=n; i<n+ak+sk+yk; i++)   z[0][i]=0;
	
	q=0;
	if(!strcmp(tip, "min"))
	{
		for(int i=0; i<m; i++)
		{
			if(sign[i]==isrt[0]) z[0][n+i+q]=-M;
			else if(sign[i]==isrt[2] ) {z[0][n+i+1+q]=-M; q=q+1;}
		}	
	}

	q=0;
	if(!strcmp(tip, "max"))
	{
		for(int i=0; i<m; i++)
		{
			if(sign[i]==isrt[0]) z[0][n+i+q]=M;
			else if(sign[i]==isrt[2] ) {z[0][n+i+1+q]=M; q=q+1;}
		}	
	}
	
	//BFS1
	for(int i=0; i<n+sk+ak+yk; i++)	BFS1[0][i]=z[0][i];
	
	for(int i=1; i<m+1; i++)
	{
		for(int j=0; j<n+sk+ak+yk; j++)
		{
			BFS1[i][j]=BFS0[i-1][j];
		}
	}
	
	//BFS
	for(int i=0; i<m+1; i++)
	{
		for(int j=0; j<n+sk+ak+yk+1; j++)
		{
			if(j==n+sk+ak+yk) BFS[i][j]=RHS[i][0];
			else BFS[i][j]=BFS1[i][j]; 
		}
	}
		
	int sat[yk], idx[yk]; // holds column number of synthetic variable
	int k=0;
	for(int i=0; i<n+sk+ak+yk; i++)
	{		
		if(z[0][i]==M || z[0][i]==-M) 
		{
			idx[k]=i; 	
			for(int j=1; j<m+1; j++) 
			{
				if(BFS[j][i]==1){sat[k]=j; k=k+1;} 
			}
		}
	}
	for(int i=0; i<yk; i++)  cout<<endl<<"  idx["<<i+1<<"]: "<<idx[i]+1<<endl;
	
	for(int i=0; i<yk; i++)
	{
		for(int j=0; j<n+sk+ak+yk+1; j++)
		{
			if(!strcmp(tip, "min")) BFS[0][j]=M*BFS[sat[i]][j]+BFS[0][j];
			else if(!strcmp(tip, "max")) BFS[0][j]=-M*BFS[sat[i]][j]+BFS[0][j];
		}		
	}

	cout<<"\n BFS: \n";
	for(int i=0; i<m+1; i++)
	{
		for(int j=0; j<n+sk+ak+yk+1; j++)
		{
			cout<<" "<<BFS[i][j]<<" ";
		}
		cout<<endl;
	}	

bool optimal=false;

//optimality condition

if(!strcmp(tip, "max"))
{
	int k=0;
    for(int i=0; i<n+sk+ak+yk; i++)
	{
		if(BFS[0][i]<=0){k=k+1;}	
	}
	if(k==0)
	{
		cout<<"\nOptimality condition is ensured for the max problem."; optimal=true;	
		cout<<"\nX values: ";
		for(int i=0; i<n; i++){cout<<X[i][0]<<" ";}
        cout<<"\nz: "<<BFS[0][n+sk+ak+yk]<<endl;
	}
}
else if(!strcmp(tip, "min"))
{    
    int k=0;
	for(int i=0; i<n+sk+ak+yk; i++)
	{
		if(BFS[0][i]>=0){k=k+1;}
	}
	if(k==0)
	{
		cout<<"\nOptimality condition is ensured for the min problem.\n"; optimal=true; 
		cout<<"\nX values: ";
		for(int i=0; i<n; i++){cout<<X[i][0]<<" ";}
		cout<<"\nz: "<<BFS[0][n+sk+ak+yk]<<endl;
	}
}


//Simplex algorithm 

int iter=0;
double optz;        //Optimal value of z
int x=-1;          // key column number
    y=-1;         //key row number


if(optimal==false)
{
	do 
	{	
		iter=iter+1;
		cout<<"\niteration: "<<iter<<endl;

		//Key column and row
		double pcol[m+1][1];   
		double prow[1][n+sk+ak+yk+1];


		if(!strcmp(tip, "max"))
		{ 

			double xx=0; 
			x=-1;
			for(int i=0; i<n+sk+ak+yk; i++)
			{
				if(BFS[0][i]<0)
				{
					if(abs(BFS[0][i])>xx){xx=abs(BFS[0][i]); x=i; cout<<"x= "<<x; }
				}	
			}
		
			for(int i=0; i<yk; i++)
			{
				if(x==-1 || x==idx[i])	{ x=-2;}
			}
			
			if(x==-2){cout<<"\n***Infeasible solution!***\n"; 	break;}
				
			
			cout<<"\n\npivot column: "<<x+1<<". column";   
			
			for(int i=0; i<m+1; i++)
			{
		    	pcol[i][0]= BFS[i][x];
		    	cout<<"\n"<<pcol[i][0];	
			}
					
			
			double yy=99999; 
			y=-1;       
			for(int i=1; i<m+1; i++)
			{
				if(pcol[i][0]>0)
				{
					if(abs(BFS[i][n+sk+yk+ak]/pcol[i][0])<=yy)   
					{
					   yy=abs(BFS[i][n+sk+yk+ak]/pcol[i][0]);
					   y=i;
					}
				}
			}
			
			if(y==-1){cout<<"\n\n***Unbounded solution!***"; 	break;}
			
			cout<<"\n\n pivot row: "<<y+1<<". row"; 	
			for(int i=0; i<n+sk+ak+yk+1; i++)
			{
		    	prow[0][i]= BFS[y][i];
			}
			
			cout<<"\npivot row: ";
			for(int i=0; i<n+sk+ak+yk+1; i++)
			{
		    	cout<<" "<<prow[0][i];	
			}	
			cout<<"\n\nThe chosen element: "<<BFS[y][x]<<endl;	
		}

		else if(!strcmp(tip, "min"))
		{

			double xx=0;
			x=-1;        
			for(int i=0; i<n+sk+ak+yk; i++)
			{ 
				if(BFS[0][i]>0)
				{
					if(BFS[0][i] >xx){xx=BFS[0][i]; x=i;}
				}	
			}
			
			for(int i=0; i<yk; i++)
			{
				if(x==-1 || x==idx[i])	{ x=-2;}
			}
			
			if(x==-2){cout<<"\n***Infeasible solution!***\n"; 	break;}
			
			else {cout<<"\npivot column: "<<x+1<<". column"; }
			
			for(int i=0; i<m+1; i++)
			{
		    	pcol[i][0]= BFS[i][x];
		    	cout<<"\n"<<pcol[i][0];			    	
			}
			
			double yy=99999;
			y=-1;        
			for(int i=1; i<m+1; i++)
			{
				if(pcol[i][0]>0)
				{
					if(abs(BFS[i][n+sk+ak+yk]/pcol[i][0])<=yy)
					{
					   yy=abs(BFS[i][n+sk+ak+yk]/pcol[i][0]);
					   y=i;
					}
				}
			}
				
			
			if(y==-1){cout<<"\n\n***Unbounded solution!***"; 	break;}

			
			cout<<"\n\nkey row: "<<y+1<<". row"; 
			
			for(int i=0; i<n+sk+ak+yk+1; i++)
			{
		    	prow[0][i]= BFS[y][i];
			}
			
			cout<<"\nkey row: ";
			for(int i=0; i<n+sk+ak+yk+1; i++)
			{
		    	cout<<" "<<prow[0][i];	
			}
			
			cout<<"\n\nChosen element: "<<BFS[y][x]<<endl;   
		} 
		


		//Row operations
		
		double cell=BFS[y][x];
		double k[m+1][1]; // vector holding row coefficients
		for(int i=0; i<m+1; i++)
		{
			k[i][0]=-pcol[i][0]/cell;
		}
		
		
		// multiply coefficients by matrix
		for(int i=0; i<m+1; i++)
		{
			for(int j=0; j<n+sk+ak+yk+1; j++)
			{
				BFS[i][j]=prow[0][j]*k[i][0]+BFS[i][j];
			}
		}   
		
		for(int i=0; i<n+sk+ak+yk+1; i++)
		{
			BFS[y][i]=prow[0][i]/cell;
		}
		
		cout<<"\nmatrix \n";
		for(int i=0; i<m+1; i++)
		{
		    for(int j=0; j<n+sk+ak+yk+1; j++)
			{
		    	cout<<" "<<BFS[i][j]<<" ";	
			}
			cout<<endl;
		}   

						
		//optimality condition
		
		if(!strcmp(tip, "max"))
		{
			int k=0;
		    for(int i=0; i<n+sk+ak+yk; i++)
			{
				if(BFS[0][i]<0){k=k+1;}	
			}
			if(k==0){ optimal=true;}
		}
		else if(!strcmp(tip, "min"))
		{    
		    int k=0;
			for(int i=0; i<n+sk+ak+yk; i++)
			{
				if(BFS[0][i]>0){k=k+1;}
			}
			if(k==0) { optimal=true; }	
		}
	
		for(int i=0,q=0; i<n+sk+ak+yk; i++)
		{
			if(BFS[0][i]==0) q=q+1;
			if(q>m){cout<<endl<<"\n***There is an alternative solution!****\n";}
		}
		
		double x[n]; 
		if(optimal==true)
		{
			for(int i=0; i<n; i++)
			{
				int bir=0;
				int sfr=0;
				int g=0;  // keeps the row index
				for(int j=0; j<m+1; j++)
	    		{
					if(BFS[j][i]==1) {bir=bir+1; g=j;}
					if(abs(BFS[j][i])<0.0000001){ sfr=sfr+1;}
		    	}
						
				if(bir==1 && sfr==m) x[i]=BFS[g][n+sk+ak+yk];
				else x[i]=0; 		  	
			}						
				
			optz=BFS[0][n+sk+ak+yk];
			double fonk=0;
				
			for(int i=0; i<N; i++)
			{
				fonk= fonk +  C[0][i]*x[i];
			}
				
			if(round(fonk)!=round(optz)) { cout<<"\n***Infeasible solution!***\n  "; optimal=false; break;	}
				
			for(int i=0,q=0; i<N; i++)
			{
				if(ozx[i]=='<')	{	X[i][0]=(-1)*x[i+q];}  
				if(ozx[i]=='0')	{	X[i][0]=x[i+q]-x[i+q+1];q=q+1;}
				if(ozx[i]=='>') {   X[i][0]=x[i+q];	}
			}
		
		    cout<<"\n\n\n Optimal Solution Found: ";
			 
		    cout<<"\n\nX values:\n\n";
			for(int i=0; i<N; i++){cout<<"X["<<i+1<<"]= "<<X[i][0]<<endl;}
			cout<<"\nz: "<<optz<<endl;
		}
			
	}while(optimal==false);
}

double xr; //variable selected for the first branch

//pruning 1
int j=0;   
for(int i=0; i<n; i++)
{
	if(X[i][0]==int(X[i][0])) j+=1;
}
if(j==n && y!=-1) cout<<"\nAn integer solution is found";

else if(j!=n && y!=-1)   


//Branch and Bound Algorithm

if(j!=n && y!=-1)
{
	
int tms;  
tree family[20];   //objelerden oluþan dizi

do
{
cout<<"\n\nTo find an integer solution, please press 1: ";
cin>>tms;
}while(tms!=1);

//Branch-Bound Algorithm for max problem using breadth-first search
int	byt=0;  
	
cout<<"\nX values of the 1. node: ";
for(int i=0; i<n; i++){family[0].xmom[i]=X[i][0];}
for(int i=0; i<n; i++){cout<<family[0].xmom[i]<<" ";}
family[0].zmom=optz;
	
for(int i=0; i<m+1; i++){
for(int j=0; j<n+sk+1; j++){
		family[0].mom[i][j]=BFS[i][j];
	}
}
	
for(int i=0; i<n; i++)
{	
	xr= family[0].xmom[i];
	if(family[0].xmom[i]!=int(family[0].xmom[i]))
	{
		cout<<"\nVariable selected for branching : "<<xr;
		break;
	}
} 


int h=0; 

do{		
	family[h].secx=xr;	
	
	if(xr>round(xr))
	{
		family[h].kst1=round(xr);
		family[h].kst2=int(xr)+1;
	}
	if(xr<round(xr))
	{
		family[h].kst1=int(xr);
		family[h].kst2=round(xr);
	}	
		
	
	cout<<"\nz:  "<<family[h].zmom<<endl;	
	cout<<"Constraint 1: x<="<<family[h].kst1<<endl;  
	cout<<"Constraint 2: x>="<<family[h].kst2<<endl;	
	
// creating the simplex table for the first constraint kst1
	
	for(int i=0; i<m+1+byt; i++)
	{
		for(int j=0; j<n+sk+byt; j++)
		{
		family[h].child1[i][j]=family[h].mom[i][j];		
		}
	}
	
	for(int i=0; i<m+1+byt; i++)
	{
		family[h].child1[i][n+sk+byt]=0;
		family[h].child1[i][n+sk+byt+1]=family[h].mom[i][n+sk+byt];
	}
	
	int rfrns;  //holds the row number of the selected variable x in the simplex table
	
	for(int i=0; i<n; i++)  family[h].child1[m+byt+1][i]=0;  
	family[h].child1[m+byt+1][n+sk+byt]=1;
	family[h].child1[m+byt+1][n+sk+byt+1]=family[h].kst1-family[h].secx;
	
	for(int i=0; i<m+1; i++)
	{
		if(family[h].mom[i][n+sk+byt]==family[h].secx)	rfrns=i;
	}

	byt=byt+1;	
	for(int i=n; i<n+sk+byt-1; i++)  
	{	
		family[h].child1[m+byt][i]=(-1)*family[h].mom[rfrns][i];
	}
	
		
	cout<<"\n\nthe BFS of the family["<<h<<"].child1:\n";
	for(int i=0; i<m+1+byt; i++)
	{
		for(int j=0; j<n+sk+1+byt; j++)
		{
			cout<<" "<<family[h].child1[i][j]<<" ";
		}
		cout<<endl;
	}
	
//Pivot column and row
double pcol[m+1+byt][1];
double prow[1][n+sk+1+byt];
int x=-1;       // pivot column number
int y=-1;      //pivot row number


	//pivot row
	for(int i=0; i<m+1+byt; i++)
	{
		if(family[h].child1[i][n+sk+byt]<0)	x=i; 			  
	}
	

	if(x==-1)
	{
		cout<<"\nInfeasible solution\n"; 	
		family[h].ctrl1=-1;					  
	}

	for(int i=0; i<n+sk+byt+1; i++)  {prow[0][i]=family[h].child1[x][i];}
	
	cout<<"\npivot row: ";
	for(int i=0; i<n+sk+byt+1; i++)  {cout<<" "<<prow[0][i];}
	
	
	//pivot column
	double xx=999999;
	for(int i=0; i<n+sk+byt; i++)
	{
		if(family[h].child1[x][i]<0)
		{
			if(abs(family[h].child1[0][i]/family[h].child1[x][i])<xx)
			{	
				xx=abs(family[h].child1[0][i]/family[h].child1[x][i]);
				y=i;
			}
		}
	}
	
	if(y==-1)
	{
		cout<<"\nUnbounded solution\n"; 	
		family[h].ctrl1=-1;					  
	}	
	else
	{
		for(int i=0; i<m+1+byt; i++)
		{
	    	pcol[i][0]=family[h].child1[i][y];	
		}
		
		cout<<"\n\npivot column: \n";
		for(int i=0; i<m+1+byt; i++)
		{
	    	cout<<pcol[i][0]<<"\n";	
		}
		
		cout<<"\nChosen element: "<<family[h].child1[x][y];
		
		
		//Row operations
		double cell=family[h].child1[x][y];
		double k[m+1+byt][1]; //vector holding the coefficients
		for(int i=0; i<m+1+byt; i++)	k[i][0]=-pcol[i][0]/cell;
		cout<<"\ncoefficients: ";
		for(int i=0; i<m+1+byt; i++)	cout<<k[i][0]<<" ";
		
		// katsayýlarý tüm matris ile çarp
		for(int i=0; i<m+1+byt; i++)
		{
		    for(int j=0; j<n+sk+byt+1; j++)
			{
		    	family[h].child1[i][j]=prow[0][j]*k[i][0]+family[h].child1[i][j];
			}
		}   
	
		for(int i=0; i<n+sk+byt+1; i++)
		{
			family[h].child1[x][i]=prow[0][i]/cell;
		}
		
		for(int i=0; i<m+1+byt; i++)
		{
		    for(int j=0; j<n+sk+byt+1; j++)
			{
		    	if(abs(family[h].child1[i][j])<=0.0000000001) {	family[h].child1[i][j]=0;};
			}
		} 
	
		
		cout<<"\n\nfamily["<<h<<"].child1 \n";
		for(int i=0; i<m+1+byt; i++)
		{
		    for(int j=0; j<n+sk+byt+1; j++)
			{
		    	cout<<" "<<family[h].child1[i][j]<<" ";
			}
			cout<<endl;
		} 
		
		for(int i=0; i<n; i++)
		{
			int bir=0;
			int sfr=0;
			int g=0;
			for(int j=0; j<m+1+byt; j++)
			{
				if(family[h].child1[j][i]==1) {bir=bir+1; g=j;}
				if(family[h].child1[j][i]==0) sfr=sfr+1;
			}
			if(bir==1 && sfr==m+byt) {family[h].xchild1[i]=family[h].child1[g][n+sk+byt];}  
		}
			
		cout<<"\nX values: ";
		for(int i=0; i<n; i++){cout<<family[h].xchild1[i]<<" ";}
		family[h].zchild1=family[h].child1[0][n+sk+byt];
	    cout<<"\n\nz value of family["<<h<<"].child1: "<<family[h].zchild1<<endl;
	
	        
	    int jj=0;
	    for(int i=0; i<n; i++)
	    {
	    	if(abs(int(family[h].xchild1[i])-family[h].xchild1[i])<0.00001) 
			{
				jj=jj+1;
				cout<<"jj: "<<jj<<endl;
				cout<<abs(int(family[h].xchild1[i])-family[h].xchild1[i])<<endl;
			}
			if(jj==n) {	family[h].ctrl1=1; cout<<"\nAn optimal integer soltion is found!\n";}
		}
	    		
	}//else 
	

//creating the simplex table for the second constraint kst2

byt=byt-1;
	
	for(int i=0; i<m+1+byt; i++)
	{
		for(int j=0; j<n+sk+byt; j++)
		{
			family[h].child2[i][j]=family[h].mom[i][j];		
		}
	}
	
	for(int i=0; i<m+1+byt; i++)
	{
		family[h].child2[i][n+sk+byt]=0;
		family[h].child2[i][n+sk+byt+1]=family[h].mom[i][n+sk+byt];
	}
	
	rfrns=0;  
	
	for(int i=0; i<n; i++)  family[h].child2[m+byt+1][i]=0;  
	family[h].child2[m+byt+1][n+sk+byt]=1;
	family[h].child2[m+byt+1][n+sk+byt+1]=family[h].secx-family[h].kst2;
	
	for(int i=0; i<m+1; i++)
	{
		if(family[h].mom[i][n+sk+byt]==family[h].secx)	rfrns=i;
	}

	byt=byt+1;	
	for(int i=n; i<n+sk+byt-1; i++)  
	{	
		family[h].child2[m+byt][i]=family[h].mom[rfrns][i];
	}
	
		
	cout<<"\n\n BFS for the family["<<h<<"].child2:\n";
	for(int i=0; i<m+1+byt; i++)
	{
		for(int j=0; j<n+sk+1+byt; j++)
		{
			cout<<" "<<family[h].child2[i][j]<<" ";
		}
		cout<<endl;
	}
		
	x=-1;       // pivot column number
	y=-1;      //pivot row number
	
	//pivot row
	for(int i=0; i<m+1+byt; i++)
	{
		if(family[h].child2[i][n+sk+byt]<0)	x=i; 
				  
	}
	
	if(x==-1)
	{
		cout<<"\nInfeasible solution\n"; 	
		family[h].ctrl2=-1;					  
	}

	for(int i=0; i<n+sk+byt+1; i++)  {prow[0][i]=family[h].child2[x][i];}
	
	cout<<"\n\npivot row: ";
	for(int i=0; i<n+sk+byt+1; i++)  {cout<<" "<<prow[0][i];}
	
	
	//pivot column
	xx=999999;
	for(int i=0; i<n+sk+byt; i++)
	{
		if(family[h].child2[x][i]<0)
		{
			if(abs(family[h].child2[0][i]/family[h].child2[x][i])<xx)
			{	
				xx=abs(family[h].child2[0][i]/family[h].child2[x][i]);
				y=i;
			}
		}
	}
	
	if(y==-1)
	{
		cout<<"\nInfeasible solution\n"; 	
		family[h].ctrl2=-1;				  
	}	
	else
	{
		for(int i=0; i<m+1+byt; i++)
		{
    		pcol[i][0]=family[h].child2[i][y];	
		}
	
		cout<<"\n\npivot column: \n";
		for(int i=0; i<m+1+byt; i++)
		{
    		cout<<pcol[i][0]<<"\n";	
		}
	
		cout<<"\nChosen element: "<<family[h].child2[x][y];
	
	
		//row operations
		double cell=family[h].child2[x][y];
		double k[m+1+byt][1];
		for(int i=0; i<m+1+byt; i++)	k[i][0]=-pcol[i][0]/cell;
		cout<<"\ncoefficients: ";
	
		for(int i=0; i<m+1+byt; i++)	cout<<k[i][0]<<" ";
		
		for(int i=0; i<m+1+byt; i++)
		{
		    for(int j=0; j<n+sk+byt+1; j++)
			{
		    	family[h].child2[i][j]=prow[0][j]*k[i][0]+family[h].child2[i][j];
			}
		}   

		for(int i=0; i<n+sk+byt+1; i++)
		{
			family[h].child2[x][i]=prow[0][i]/cell;
		}
		
		for(int i=0; i<m+1+byt; i++)
		{
		    for(int j=0; j<n+sk+byt+1; j++)
			{
		    	if(abs(family[h].child2[i][j])<=0.0000000001) {	family[h].child2[i][j]=0;};
			}
		} 
		
		cout<<"\n\nfamily["<<h<<"].child2 \n";
		for(int i=0; i<m+1+byt; i++)
		{
		    for(int j=0; j<n+sk+byt+1; j++)
			{
		    	cout<<" "<<family[h].child2[i][j]<<" ";
			}
			cout<<endl;
		} 
		
			for(int i=0; i<n; i++)
			{
				int bir=0;
				int sfr=0;
				int g=0;
				for(int j=0; j<m+1+byt; j++)
		    	{
					if(family[h].child2[j][i]==1) {bir=bir+1; g=j;}
					if(family[h].child2[j][i]==0) sfr=sfr+1;
			    }
			    if(bir==1 && sfr==m+byt) {family[h].xchild2[i]=family[h].child2[g][n+sk+byt];}  
			}
			
			cout<<"\nX values: ";
			for(int i=0; i<n; i++){cout<<family[h].xchild2[i]<<" ";}
			family[h].zchild2=family[h].child2[0][n+sk+byt];
	        cout<<"\n\n z value of family["<<h<<"].child2 : "<<family[h].zchild2<<endl;
	        
	        int jj=0;
	        for(int i=0; i<n; i++)
	        {
	        	if(abs(int(family[h].xchild2[i])-family[h].xchild2[i])<0.00001) 
				{
					jj=jj+1;
					cout<<"jj: "<<jj<<endl;
					cout<<abs(int(family[h].xchild2[i])-family[h].xchild2[i])<<endl;
				}
				if(jj==n) {	family[h].ctrl2=1; cout<<"\nAn optimal integer soltion is found!\n";} 
			}
	            
	}// else child2

	//If the 1st branch did not give an integer result and gave a better result than the 2nd branch, the branch is continued from the 1st branch.
	if( !strcmp(tip, "max") && family[h].ctrl1==0 && family[h].zchild1>family[h].zchild2)   
	{	
		for(int i=0; i<m+1+byt; i++)
		{for(int j=0; j<n+sk+byt+1; j++) {family[h+1].mom[i][j]=family[h].child1[i][j];}}
		
		for(int i=0; i<n; i++)
		{	
			xr= family[h].xchild1[i]; 
			if(family[h].xchild1[i]!=int(family[h].xchild1[i]))
			{
			 cout<<"\nVariable selected for branching : "<<xr;
			 break;
			}
		}
		
		family[h+1].zmom=family[h].zchild1;
		h=h+1;
	}
	
	else if(!strcmp(tip, "max") && family[h].ctrl2==0 && family[h].zchild2>family[h].zchild1)
	{
		for(int i=0; i<m+1+byt; i++)
		{for(int j=0; j<n+sk+byt+1; j++){family[h+1].mom[i][j]=family[h].child2[i][j];}}
		
		for(int i=0; i<n; i++)
		{	
			xr= family[h].xchild2[i]; 
			if(family[h].xchild2[i]!=int(family[h].xchild2[i]))
			{
		 	cout<<"\nVariable selected for branching : "<<xr;
		 	break;
			}
		}

		family[h+1].zmom=family[h].zchild2;
		h=h+1;
	}
	
	//If the 1st branch did not give an integer result and gave a better result than the 2nd branch, the branch is continued from the 1st branch.
	if( !strcmp(tip, "min") && family[h].ctrl2==0 && family[h].zchild1>family[h].zchild2) 
	{	
		for(int i=0; i<m+1+byt; i++)
		{for(int j=0; j<n+sk+byt+1; j++) {family[h+1].mom[i][j]=family[h].child2[i][j];}}
		
		for(int i=0; i<n; i++)
		{	
			xr= family[h].xchild2[i];
			if(family[h].xchild2[i]!=int(family[h].xchild2[i]))
			{
			 cout<<"\nVariable selected for branching : "<<xr;
			 break;
			}
		}
		family[h+1].zmom=family[h].zchild2;
		h=h+1;
	}
	
	else if(!strcmp(tip, "min") && family[h].ctrl1==0 && family[h].zchild2>family[h].zchild1)
	{
		for(int i=0; i<m+1+byt; i++)
		{for(int j=0; j<n+sk+byt+1; j++){family[h+1].mom[i][j]=family[h].child1[i][j];}}
		
		for(int i=0; i<n; i++)
		{	
			xr= family[h].xchild1[i]; 
			if(family[h].xchild1[i]!=int(family[h].xchild1[i]))
			{
			 cout<<"\nVariable selected for branching : "<<xr;
			 break;
			}
		}
		family[h+1].zmom=family[h].zchild1;
		h=h+1;
	}
	
}while(family[h].ctrl1==0 || family[h].ctrl2==0);

int best=0;
int child=0;

if(!strcmp(tip, "max"))
{
	optz=0;

	for(int i=0;i<=h; i++)
	{
	    if(family[i].ctrl1==1)
	    {
	    	if(optz<family[i].zchild1)
			{
				optz=family[i].zchild1;
				best=i;	
				child=1;	
			} 
		}
			
		if(family[i].ctrl2==1)
		{
			if(optz<family[i].zchild2) 
			{
				optz=family[i].zchild2;
				best=i;	
				child=2;	
			} 
		}
	}
}

else if(!strcmp(tip, "min"))
{
	optz=999999;

	for(int i=0;i<=h; i++)
	{
	    if(family[i].ctrl1==1)
	    {
	    	if(optz>family[i].zchild1)
			{
				optz=family[i].zchild1;
				best=i;	
				child=1;	
			} 
		}
			
		if(family[i].ctrl2==1)
		{
			if(optz>family[i].zchild2) 
			{
				optz=family[i].zchild2;
				best=i;	
				child=2;	
			} 
		}
	}
}


cout<<"\n\n\nOptimal result: "<<optz<<endl;
cout<<"\nVariables: ";
if(child==1) for(int i=0; i<n; i++){cout<<"x"<<i+1<<": "<<family[best].xchild1[i]<<" ";}
else if(child==2) for(int i=0; i<n; i++){cout<<"\nx"<<i+1<<": "<<family[best].xchild2[i]<<" ";}

} //branch and bound

return 0;
}



