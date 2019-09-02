#include <bits/stdc++.h>
#define lli long long int
#define ONE 1
#define strv vector<string>
using namespace std;
 
lli MAX;				// order of btree
lli b,m,f,i,j;
string l,str;
strv output;			// output buffer
string f_name;						// file name

string query_type(string s)			// identify the query type
{

    if(s.find("INSERT") != string::npos )
        return "INSERT";
    else if(s.find("FIND") != string::npos)
        return "FIND";
    else if(s.find("RANGE") != string::npos)
        return "RANGE";
    else if(s.find("COUNT") != string::npos)
        return "COUNT";
    else
        return "kindly enter valid query";

}

strv tokens(string line)
{
    vector <string> tokens; 
      
    // stringstream class check1 
    stringstream check1(line); 
      
    string intermediate; 
      
    // Tokenizing w.r.t. space ' ' 
    while(getline(check1, intermediate, ' ')) 
    { 
        tokens.push_back(intermediate); 
    } 
    return tokens;
}

lli convert(string s)
{
    return stoi(s);
}

struct node					// structure of a node
{
    node **pt;
    node *ngh;
    lli count,leaf,*noDup;
    lli *data;
};

struct tree 				// structure of tree
{
    void put_data(node*,lli);
    void increment(node *temp);
    
    lli item;
    node *root,*suc;

    void init_node(node* ne, lli l)
    {
        ne->data = new lli[MAX+1];
        ne->noDup = new lli[MAX+1];
        ne->pt = new node*[MAX+2];
        ne->count=0;					// count number of keys
        ne->leaf=l;						// count number of leaf
        lli i=0;
        ne->ngh=NULL;

        while(i<=MAX)
        {
            ne->noDup[i]=ONE;
            ne->pt[i]=NULL;    
            i++;
        }    
        ne->pt[i]=NULL;        
    }

    node* create(lli l)
    {
        node *ne;
        ne = new node;
        init_node(ne, l);
        return ne;
    }

    bool do_split(node *t, node *p)
    {
        lli lhs = MAX;
        lhs--;
        lli rhs = t->count;
        if(lhs < rhs)
        {
            return true;
        }        
        else
        {
            return false;
        }
        
    }
    void process_out(strv &out,string l);  			// process the queries
    public:
        tree()
        {
            root=NULL;
            suc=NULL;
        }
        node* getroot()
        {
            return root;
        }
        void set_item(lli data)
        {
            item=data;
        }

        void insert(node *parent,node *temp)
        {
            if(!temp)
            {
                temp=root=create(ONE);
                temp->noDup[0]=ONE;
                lli c = temp->count;
                c++;
                temp->count=c;
                temp->data[0]=item;
            }
            else if(!temp->pt[0])
            {
                lli i=0;
                
                while(i<temp->count)
                {
                    if(temp->data[i]==item)
                    {
                        increment(temp);
                        return;
                    }
                    i++;
                }
                put_data(temp,item);
                i-=1;
            }
            else if(item>=temp->data[temp->count-1])
                insert(temp,temp->pt[temp->count]);
            
            else
            {
                lli i;
                for(i=0;i<=temp->count-1;i=i+1)
                {
                    if(item<temp->data[i])
    	            {
    	                insert(temp,temp->pt[i]);
    	                break;
    	            }
                    if(i> temp->count && MAX > i)
                    { 
                        
                        if(item>temp->data[i])
                            continue;
                        else
                        {
                            insert(temp,temp->pt[i]);
                            break;
                        }
                    } 
                    
                }
            }
            do_split(temp, parent);
            
            if(MAX-1 < temp->count)
            {
                split(parent,temp);
            }
                
        }
        /*
        lli range(node * root ,int x,int y)
            {
                int i = 0;
                lli count=0;
                node * temp = root;
                
                while (!temp->is_leaf) 
                {
                    i = 0;
                    while (i < temp->num_keys) 
                    {
                        if (x > temp->keys[i]) 
                            i++;
                        else 
                            break;
                    }

                    temp = (node *)temp->pointers[i];
                }

                if(temp->keys[temp->num_keys - 1] < x )
                    temp=(node *)temp->pointers[order - 1];

                count=count_range(temp,x,y);

                return count;
            }
        */
        
        lli range_search(lli l1,lli h,lli count,lli i)
        {
            
            node *temp=root;
            
            if(temp)
            {
                while(temp)
                {
                    
                    if(!temp->pt[0] && temp->data[i]>=l1)
                    {
                        
                        lli j=i;
                        
                        while(j <= temp -> count - 1 && temp -> data[j] < h+1)
                        {    
                            lli t = count + temp->noDup[j];
                            count= t;
                            j++;
                        }    
                        temp=temp->ngh;
                        lli p;
                        while(temp)
                        {
                            for(p=0;p < temp->count;p++)
                            {
                                bool fl=false;
                                if(temp->data[p] > h)
                                    return count;
                                
                                lli t = count+temp->noDup[p]; 
                                count = t;
                            }
                            temp=temp->ngh;
                        }
                        return count;
                    }
                    else if(l1<temp->data[i] || ((i + 1) == (temp->count + 1)))
                    {
                        node* t = temp->pt[i];
                        temp = t;
                        i=0;
                    }
                    else
                        i++;
                }
            }
            return 0;
        }

        void split(node *parent,node *temp)
        {
            bool flag=true;
            lli leaf=0;
            
            if(temp==root)
            {
                
                root=create(0);
                root->data[0]=temp->data[MAX/2];
                parent=root;
                root->count=root->count+1;
                parent->pt[0]=temp;
                flag=false;
            }
            
            for(int y=0;y<parent->count+1;y++)
            {
                if(parent->pt[y]==temp)
                {
                    lli i=parent->count+1;

                    for(;i>y+1;i--)
                    {
                         lli ab = i - 1;   
                         parent->pt[i]=parent->pt[ab];
                    }
                    if(temp->pt[0])
                        leaf=0;
                    else 
                        leaf=1;
                    
                    parent->pt[y+1]=create(leaf);
                    
                    lli z;
                    
                    for(z = 0;z <= MAX/2;z++)
                        (parent -> pt[y + 1])->pt[z] = temp->pt[z + 1 + MAX/2];
                    
                    lli zt=(MAX/2);
                    zt++;
                    
                    for(z=zt;z<=MAX;z++)
                        temp->pt[z]=NULL;
                    
                    lli pmax=MAX/2;
                    temp->count=pmax;
                    pmax=MAX-(MAX/2+1-leaf);
                    (parent->pt[y+1])->count=pmax;
                    
                    lli k=0;
                    zt=MAX/2+1-leaf;
                    for (z = zt;z<=MAX; z++,k++)
                    {
                        lli t = temp->noDup[z];
                        (parent->pt[y+1])->noDup[k]= t;
                        
                        t = temp->data[z];
                        (parent->pt[y+1])->data[k]=t;
                    }
                    if (leaf==1)
                    {
                        (parent->pt[y+1])->ngh=temp->ngh;
                        lli p;
                        temp->ngh=parent->pt[y+1];
                    }

                    if(flag)
                        put_data(parent,temp->data[MAX/2]);

                    
                    break;
                }
            }
        }
};
tree t;

void tree::put_data(node *temp,lli item)
{
    if(item>temp->data[temp->count-1])
    {
        lli z=1;
        temp->data[temp->count]=item;
    }

    else
    {
        lli i;
        for(i=0;i < temp->count;i++)
        {
            if(item>temp->data[i]-1)
            {
                continue;
            }
            lli j;
            for(j=temp->count;j>i;j--)
            {
                lli t = temp->data[j-1];
                temp->data[j]=t;
                t = temp->noDup[j-1];
                temp->noDup[j]=t;
            }
            
            temp->data[i]=item;
            temp->noDup[i]=j;
            j=1;
                // break;
            
        }
    }
    lli c = temp->count;
    c++;
    temp->count=c;
}

void print_output(strv &v)
{
    for(auto i:v)
    { 
        cout<<i<<endl;
    }
    v.clear();  
    
}

void write_to_file(strv &v)
{
    ofstream outfile("output.txt", std::ios_base::app);
    
    for(auto i:v)
    { 
        outfile<<i<<endl;
    }
    outfile.close();

}


void tree::process_out(strv &out,string l){
   
    string query = query_type(l);
    //cout<<query<<endl;
    if(query == "INSERT")    
    {
        strv v = tokens(l);
        t.set_item(convert(v[1]));
        
        node *temp = t.getroot();
        node *parent = t.getroot();
        if(!temp)
        {
            temp=root=create(1);
            temp->noDup[0]=1;
            lli c = temp->count;
            c++;
            temp->count=c;
            temp->data[0]=item;
        }
        else if(!temp->pt[0])
        {
            lli i=0;
            for(i=0;i<=temp->count-1;i=i+1)
            {
                if(temp->data[i]==item)
                {
                    lli i1=0;
                   
                    while(temp)
                    {
                        if(temp->data[i1]==item)
                        {
                            if(!temp->pt[0])
                            {
                                lli t= temp->noDup[i1];
                                t++;
                                temp->noDup[i1]=t;;
                                break;
                            }
                        }
                        else if(item <= temp->data[i1]-1 || i==temp->count)
                        {
                            temp=temp->pt[i1];
                            i1=0;
                        }
                        else
                            ++i1;
                    }
                    return;
                }
            }
            put_data(temp,item);
            i-=1;
        }
        else if(item>=temp->data[temp->count-1])
            insert(temp,temp->pt[temp->count]);
        else
        {
            lli i;
            for(i=0;i<=temp->count-1;i=i+1)
            {
                if(item<temp->data[i])
                {
                    insert(temp,temp->pt[i]);
                    break;
                }
            }
        }

        do_split(temp,parent);

        if(MAX-1<temp->count)
            split(parent,temp);
    }
    
    if(query == "RANGE")
    {   
        strv v = tokens(l);
        int ans = t.range_search(convert(v[1]),convert(v[2]),0,0);
        out.push_back(to_string(ans));            
    }


    if(query == "FIND")
    {
        strv v = tokens(l);
        lli val = convert(v[1]);
        t.set_item(val);
        
        lli i=0,vs;
        node *temp=root;

        while(temp!=NULL)
        {
            if((item<=temp->data[i]-1) || i==temp->count)
            {
                temp=temp->pt[i];
                i=0;
            }
            else if(!(temp->pt[0]) && temp->data[i]==item)
            {
                vs = temp->noDup[i];
                break;
            }
            else
                i++;
        }
        if(temp==NULL)
            vs = 0;

        if(vs<=0)
        {
            out.push_back("NO");
        }
        else
        {
            out.push_back("YES");
        }
    }


    if(query == "COUNT")
    {
        strv v = tokens(l);
        t.set_item(convert(v[1]));

        lli i=0,vs;
        
        node *temp=root;
        if(temp){
            while(temp)
            {
                if((item<=temp->data[i]-1)||i==temp->count)
                {
                    temp=temp->pt[i];
                    i=0;
                }
                else if(!(temp->pt[0]) && temp->data[i]==item)
                {
                    vs = temp->noDup[i];
                    break;
                }
                else 
                    i=i+1;
            }
        }
        if(!temp)
            vs = 0;



        out.push_back(to_string(vs));
    }
   
}
void tree::increment(node *temp){
    lli i=0;

    while(temp)
    {
        if(temp->data[i]==item)
        {
            if(!temp->pt[0])
            {
                lli t = temp->noDup[i]+1;
                temp->noDup[i] = t;
                return ;
            }
        }
        else if(item<=temp->data[i]-1 || i==temp->count)
        {
            node *t =temp->pt[i];
            temp = t;
            i=0;
        }
        else
            i++;
    }
}


void put_output(strv &v){
    print_output(v);
    write_to_file(v);
}


lli getDegree(lli b)
{    
    lli degree = ceil((b+4)/(float)12); 
    return degree;
}

void process(char* argv[])
{

        m = convert(argv[2]);
        b = convert(argv[3]);

        f_name = argv[1];
        ifstream infile(f_name);
        
        bool flag = true;
        MAX = getDegree(b);
        int i;
        
        // while there is unread records in input file , run the loop
        while(flag)                 
        {
            // 2d vector to store input records chunk by chunk. size of chunk is equal size of buffer
            vector < vector <string> > input(m - 1, strv (b) );    
            
            for(i = 0 ; i < m - 1 ; i++)
            {
                int j = 0;
                while(j < b)
                {
                    string x;
                    if(!getline(infile,x))
                    {
                        flag = false;   
                        break;   
                        
                    }   
                    else
                    {
                        input[i][j] = x;
                    }   
                    j++;
                }
                if(!flag)
                {
                    break;
                }       
            }

            // we have input in our 2d vector and ready to process it .
            lli y;
            
            for ( y = 0; y <= i-1; y++)
            {
                lli z;
                lli limit = input[y].size();
                for (z = 0; z<limit; z+=1)
                {
                    t.process_out(output,input[y][z]);
                    if(output.size()>b)put_output(output);
                }
            }
            if(i<=m-2)
            {
                lli z;
                lli limit = input[i].size();
                for(z=0;z<limit;++z)
                {
                    t.process_out(output,input[i][z]);
                    if(output.size()>b)put_output(output);   
                }
            }
        }
        
        put_output(output);
}

int main(int argc, char* argv[])
{
    // if parameters are not valid
    if(argc == 4)          
    {
        process(argv);
    }
    else
    {
        cout<<"Kindly provide valid inputs!";
        return 0;
    }   
    return 0;
}

