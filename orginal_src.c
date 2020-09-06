#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <bits/stdc++.h> 
using namespace std;
FILE *f,*s;
fstream ha;
int bs=92,r[92],cr[132],ms=224;  //value of block size and  r+c is initialized
void block(int c[92]);
void omflip();
void cshr(int c[224]);
void omflip(int x[224],int w);
void swap(int *x, int *y);
void omflip1(int x[224],int m[92],int w);
void createMap(unordered_map<string, char> *um);
string convertBinToHex(string bin);

int main()
{
	
	clock_t begin = clock();
	int n=0,i,pad,temp,a[100],n1,j,p=1,k,c[92],o[276],b[224],seed[224],si=0,flag=0; // n-length of message,pad-length of padding,i-loops,j-loops,bs-block size/size of r
        int ch,mes[100696],mes1[100696],counter=0,mastercount=0,ns,jcounter=0; // mes-the message array,ch-copying values from file,o is the hash array
	string bin;	
	printf("\n Reading Message\n");  // reading the message
    for(i=0;i<92;i++) // setting initial value of r as 0
		r[i]=0;
	for(i=0;i<132;i++) // setting initial value of c as 0
		cr[i]=0;
	for(i=0;i<256;i++) // setting initial value of h as 0
		o[i]=0;
	f=fopen("message.txt","r+"); // opening input file
	ha.open ("hash.txt", ios::out ); // opening out output file
	
	if((f==NULL))
	{
		printf("\n File is not available");
	}
	else 
	{	
		while((ch=fgetc(f))!=EOF)   // finding the length of the message
		{       
			if(ch==48)
				ch=0;
			else
				if(ch==49) 
					ch=1;
			mes[n]=ch;
			mes1[n]=ch;			
			n++;                	
		}                         
		flag=1;	// the file has been read
		n=n-1;
	    ns=n;
	}
	printf("%d",n);
	n=ns; // initializing n to the size of the message
        pad=bs-(n%bs);                                // determining the length of the padding
        n1=n; 
	for(i=0;n1>0;i++)       // conversion of number into binary  
	{    
		a[i]=n1%2;    
		n1=n1/2;    
	}    
        temp=pad+n;
        n1=i-1;
        mes[n]=1; // first bit of padding is set as 1
	//printf("%d",n); 
        for(i=n+1;i<(temp);i++) // the remaining bits of the padding is set as 0
         	mes[i]=0;    
	for(i=0;i<=n1;i++,temp--) // adding the binary value of the size of the message to the end of the message , the remaining values are intitialized as 0 by default    
	{       
		mes[temp-1]=a[i];
                
	}
    n=n+pad;

	for(i=0;i<n;)  // division into blocks
	{
		for(j=i,k=0;j<i+bs;k++,j++)
		{
			c[k]=mes[j];
		}
		
		block(c);
		i=j;
	}

	for(i=0;i<bs;i++)
			b[i]=r[i];
	for(i=0;i<132;i++)
			b[i+92]=cr[i];
	
	omflip(b,224);  // blank f to prevent slide attack
	cshr(b);
	for(i=0;i<bs;i++)
		r[i]=b[i];		
	for(i=0;i<132;i++)
		cr[i]=b[i+92];	
	
	j=0;
	jcounter=0;
	while(j<256) // squeezing phase
	{
		for(i=0;i<bs;i++)  // copying r section to b
		{	
			b[i]=r[i];
			
		}
		for(i=0;i<bs;i++) // copying r to hash array
		{	
			if(jcounter==0)
				o[i]=b[i];
			if(jcounter==1)
				o[i+92]=b[i];
			if(jcounter==2)
				o[i+184]=b[i];
			if(jcounter==3)
				o[i+276]=b[i];
			if(jcounter==4)
				o[i+368]=b[i];
			j++;
			
		}
		for(i=0;i<132;i++) // copying c section to b array , b array is where intermediate values are stored
			b[i+92]=cr[i];
		for(i=0;i<2;i++) // repeating f 
		{
			omflip(b,224);
			cshr(b);
		}
		for(i=0;i<bs;i++) // changing r to new values after f
		{
			r[i]=b[i];		
			
		}
		for(i=0;i<132;i++) // changing c to new values after f
			cr[i]=b[i+92];
		jcounter++;			
	}
	for (i=0;i<256;i++) 		//Converting binary to string
	{
		bin.push_back(o[i] + '0');
	}
        printf("\n");
	cout << convertBinToHex(bin);	//converting binary string to hex
    	ha<<bin;
	bin="";
	printf("\n %d %d ",n,ns);
	ha.close();
	fclose(f);
		
	clock_t end = clock();
	double time_spent = (double)(end-begin)/CLOCKS_PER_SEC;      	     		printf("\ntime spent : %lf",time_spent); // printing time spent		
	return 0; 
} 


void block(int c[92]) // for each block
{       int i,b[224];

	for(i=0;i<bs;i++)   // xoring the block
		r[i]=c[i]^r[i];
	for(i=0;i<bs;i++)
		b[i]=r[i];
	for(i=0;i<132;i++)
		b[i+92]=cr[i];
	for(i=0;i<2;i++) // value of nr , applying f
	{
		omflip1(b,c,224);
		cshr(b);
	}
	for(i=0;i<bs;i++)
		r[i]=b[i];
	for(i=0;i<132;i++)
		cr[i]=b[i+92];
	
}

// swap is part of omega flip
void swap(int *x, int *y) {

   int temp;
   temp = *x;    /* save the value at address x */
   *x = *y;      /* put y into x */
   *y = temp;    /* put temp into y */
  
   return;
}

void omflip(int x[224],int w)
{	
	int z[224];
	int a[16],k,i;
	for(k=223,i=0;i<16;k--,i++)
	    a[i]=x[k];
	k=0;
	while(k<16)
	{
	    for (i = 0; i < 2; i++,k++)
    	{
    		if (a[i]==0)
    		{
    		    
    			for (int j=0; j<w/2 ; j++)
    			{			
    				z[2*j] = x[j];
    				z[(2*j)+1] = x[j+w/2];
    				if (x[j+(i*w/2)] == 1)
    					swap(&z[2*j], &z[(2*j)+1]);
    				
    			}
    		}
    		else
    		{
    			for (int j = 0; j < w/2 ; j = j + 1)
    			{		
    				z[j] = x[2*j];
    				z[j+w/2] = x[(2*j)+1];
    				if (x[j+(i*w/2)] == 1)
    					swap(&z[j], &z[j+ w/2] );
    			
    			}
    		}
	        
            for(k=0;k<w;k++)
		    {
			    
			    x[k]=z[k];
		    }
	    }
	}
}

void omflip1(int x[224],int m[92],int w)
{	
	int z[224];
	int a[2],k;
	a[0]=m[0];
	a[1]=m[1];
	int y[224],j=0,i=0;
	while(i!=112)
	{
	    y[i++]=m[j++];
	    if(j==46)
	        j=0;
	}
	while(i!=224)
	{
	    y[i++]=m[j++];
	    if(j==92)
	        j=46;
	}
	
	for(k=223,i=0;i<16;k--,i++)
	    a[i]=x[k];
	k=0;
	while(k<16)
	{
	    for (i = 0; i < 2; i = i + 1)
    	{
    		if (a[i]==0)
    		{
    			for (j=0; j<w/2 ; j++)
    			{			
    				z[2*j] = x[j];
    				z[(2*j)+1] = x[j+w/2];
    				if (y[j+(i*w/2)] == 1)
    					swap(&z[2*j], &z[(2*j)+1]);
    				
    			}
    		}
    		else
    		{
    			for (j = 0; j < w/2 ; j = j + 1)
    			{		
    				z[j] = x[2*j];
    				z[j+w/2] = x[(2*j)+1];
    				if (y[j+(i*w/2)] == 1)
    					swap(&z[j], &z[j+ w/2] );
    				
    			}
        		}
	    	
            for(k=0;k<w;k++)
	    	{
	    		
	    		x[k]=z[k];
	    	}
    	}
    }
}

void cshr( int c[224])
{ 
	int i=0,j=0,v[8],z[224],p,k,l;
	for(l=0;l<13;l++) // 3 neighbourhood
	{	for(i=0;i<(ms);i++) // change to message size
		{	p=i%8; //after 7 to come back to 0
			if((p==0||p==3||p==5)&&(i>0)&&(i<(ms-1)))
			{
				z[i]=(c[i]&c[i+1])^c[i-1]^c[i]^c[i+1];
	
			}
			if((p==4)&&(i<(ms-1)))
			{
				z[i]=c[i-1]^c[i+1]^(c[i]&c[i+1]);
			}
			if((p==1||p==6)&&(i<(ms-1)))
			{
				z[i]=c[i-1]^c[i+1];
                        }
			if((p==2||p==7)&&(i<(ms-1)))
			{
				z[i]=c[i-1]^c[i]^c[i+1];
			}	
			if(i==0) //first bit
			{
				z[i]=(c[i]&c[i+1])^1^c[i]^c[i+1];
	
			}
			if(i==(ms-1)) // last bit in this case 223rd bit
			{
				z[i]=c[i-1]^c[i]^0;
			}
			
		
		}
		for(k=0;k<ms;k++) //copying the new array back to the old array** change values to message size**
			c[k]=z[k];
	}
		
}






void createMap(unordered_map<string, char> *um) 
{ 
    (*um)["0000"] = '0'; 
    (*um)["0001"] = '1'; 
    (*um)["0010"] = '2'; 
    (*um)["0011"] = '3'; 
    (*um)["0100"] = '4'; 
    (*um)["0101"] = '5'; 
    (*um)["0110"] = '6'; 
    (*um)["0111"] = '7'; 
    (*um)["1000"] = '8'; 
    (*um)["1001"] = '9'; 
    (*um)["1010"] = 'A'; 
    (*um)["1011"] = 'B'; 
    (*um)["1100"] = 'C'; 
    (*um)["1101"] = 'D'; 
    (*um)["1110"] = 'E'; 
    (*um)["1111"] = 'F'; 
} 
  
// function to find hexadecimal  
// equivalent of binary 
string convertBinToHex(string bin) 
{ 
    int l = bin.size(); 
    int t = bin.find_first_of('.'); 
      
    // length of string before '.' 
    int len_left = t != -1 ? t : l; 
      
    // add min 0's in the beginning to make 
    // left substring length divisible by 4  
    for (int i = 1; i <= (4 - len_left % 4) % 4; i++) 
        bin = '0' + bin; 
      
    // if decimal point exists     
    if (t != -1)     
    { 
        // length of string after '.' 
        int len_right = l - len_left - 1; 
          
        // add min 0's in the end to make right 
        // substring length divisible by 4  
        for (int i = 1; i <= (4 - len_right % 4) % 4; i++) 
            bin = bin + '0'; 
    } 
      
    // create map between binary and its 
    // equivalent hex code 
    unordered_map<string, char> bin_hex_map; 
    createMap(&bin_hex_map); 
      
    int i = 0; 
    string hex = ""; 
      
    while (1) 
    { 
        // one by one extract from left, substring 
        // of size 4 and add its hex code 
        hex += bin_hex_map[bin.substr(i, 4)]; 
        i += 4; 
        if (i == bin.size()) 
            break; 
              
        // if '.' is encountered add it 
        // to result 
        if (bin.at(i) == '.')     
        { 
            hex += '.'; 
            i++; 
        } 
    } 
      
    // required hexadecimal number 
    return hex;     
}