#include <bits/stdc++.h>
#define lli long long int
using namespace std;

// number of buffers and size of each buffer;
lli m , b;     

// main hash_value table
vector<set<lli> > v; 		

lli temp;
// to store store output
vector<lli> output; 		

// will point at the index which will get split
lli split_ptr;				


// represents value in hash_value function
lli split_val;				

// h(1) .denotes mod value
lli hash_value = 2;

// r value, to find the ratio
lli number_of_records = 0;
lli bucket_size;

// to store file name
char* f_name;
ifstream file_name;

bool flag;

// find key for the value supplied
lli linear_hashing(lli input, lli size)
{
	lli x;
	x = input % hash_value;
	if(input < 0)
	{
		x+=hash_value;
	}	

	lli y=x;
	while(x > size)
	{
		lli temp = x;
		lli bt = -1;

		y=temp;
		while(temp)
		{
			bt+=1;
			temp/=2;
		}	
		x^=1<<bt;
	}	
	return x;
}

void print_output(vector<lli> v)
{
    for(auto i:v)
    { 
        cout<<i<<endl;
    }    
}

void write_to_file(vector<lli> v)
{
    ofstream outfile("output.txt", std::ios_base::app);
    
    for(auto i:v)
    { 
        outfile<<i<<endl;
    }
    outfile.close();

}

// send the output buffer to the file
void flush_buffer(vector<lli> v)
{
	print_output(v);
	write_to_file(v);
	return ;
}


// upon splitting all records in the bucket would get rehashed to new buckets
void rehash()
{
	set <lli > ::iterator it = v[split_ptr].begin();
	vector<lli> temp;
	auto end = v[split_ptr].end(); 
	for(;it!=end ; it++)
	{
		lli index = linear_hashing(*it,v.size());
		if(split_ptr != index)
		{
			temp.push_back(*it);
			v[split_ptr].erase(*it);
			//v[index].insert(*it);
		}	
	}	
	lli lim = temp.size();
	for(lli i =0;i<lim;i++)
	{
		lli index = linear_hashing(temp[i],v.size());
		v[index].insert(temp[i]);
		lim=temp.size();
	}	
	return;
}


// implements split function
void split()
{
	bucket_size += (b/4);
	
	set<lli> s;
	v.push_back(s);
	hash_value = 1<<(int)(ceil(log2(v.size())));

	rehash();
	
	split_ptr = (split_ptr + 1) % split_val;
	if(!split_ptr)
		split_val*=2;

	return;
}

// handles linear hashing, splitting , rehashing and overflow
void hashing(lli input)
{
	lli index,size_of_bucket;
	lli number_of_buckets = v.size() - 1;
	index = linear_hashing(input,number_of_buckets);
	size_of_bucket = v[index].size();
	v[index].insert(input);
	//cout<<input<<endl;
	if(v[index].size() > size_of_bucket)
	{
		output.push_back(input);
		lli t = number_of_records+1;
		number_of_records=t;
		//cout<<"->"<<input<<"\n";
		if(output.size() == b)
		{
			flush_buffer(output);
			output.clear();
		}
	} 

	// output buffer full
		

	//check for splitting

	float ratio = number_of_records / ((float)(bucket_size));
	if(ratio > 0.75)
	{
		split();
	}
	return;	

}

lli convert(string s)
{
    return stoi(s);
}


int main(int argc , char* argv[])
{

	                                       //	TAKE INPUT FROM COMMANDLINE
	if(argc != 4)		// if parameters are not valid
	{
		cout<<"Kindly provide valid inputs!";
		return 0;
	}	

	f_name = argv[1];
	ifstream infile(f_name);
	// getting value of m and b from command line
	m = 0;
	b = 0;
	m = convert(argv[2]);
	b = convert(argv[3]);


												// INITIALIZE ALL VALUES
	
	split_ptr = 0;
	split_val = 2;
	bucket_size = (b/4);
	set <lli> t0,t1;
	v.push_back(t0);			// initial buckets
	v.push_back(t1);			// initial buckets

	
	lli flag = 1,i;
	// while there is unread records in input file , run the loop
	while(flag)					
	{
		// 2d vector to store input records chunk by chunk. size of chunk is equal size of buffer
		
		vector < vector <lli> > input(m - 1, vector<lli> (b) );   		
		lli j = 0;
		for(i = 0 ; i < m - 1 ; i++)
		{
			j=0;
			
			while(j < b)
			{
				lli x;
				if(infile >> x)
				{	
					input[i][j] = x;
					
				}	
				else
				{
					flag = 0;	
					break;
				}	
				j++;
			}
			if(!flag)
			{
				break;
			}		
		}
		// NOW we have input in our 2d vector and ready to process it .
		
		lli p=0;

		for( p=0;p<i;p++)
		{
			for(lli q=0;q<b;q++)
			{
				//cout<<"here"<<input[p][q]<<endl;
				hashing(input[p][q]);
			}	
		}
		if(i<=m-2)
		{
			lli q=0;
			while(q<j)
			{
				//cout<<"here"<<input[p][q]<<endl;
				hashing(input[p][q]);
				q++;
			}
		}	 		
	}

	// send all remaining values in output buffer to file.
	flush_buffer(output);
	return 1;
}
