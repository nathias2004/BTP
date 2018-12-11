#include<iostream>
#include<bits/stdc++.h>
#include<boost/functional/hash.hpp>
#include<stdio.h>
using namespace std;
long int PageSize = 4034938;			//In KB
int No_Records_Page = 28;
int New_PageNo = 10075;
unordered_map<int,int>Bplus;
unordered_map<int,int>RecordSizes;
unordered_map<pair<int,int>,int,boost::hash<pair<int,int>>>Sub_Graph_Edges;
int policy = 1;


double calculate_gain(vector<int>X,vector<int>Y,int element,char side,int type);
int calculate_no_of_rows(vector<int>);
int calculate_no_of_suc_pred(int node); //calculate number of successors and predecessors of node
int calculate_crossedges(vector<int>X,vector<int>Y);
long int Calculate_Size(vector<int>V);
int string_to_int(string s);
void write_into_newpage(vector<int>records,int flag);
int find_t(vector<int>V_dash);
void Left_Shifting(vector<int>&X,vector<int>&Y,int s,int t,vector<int>&min_X,vector<int>&min_Y,double &Global_min_ratio_cut);
void Right_Shifting(vector<int>&X,vector<int>&Y,int s,int t,vector<int>&min_X,vector<int>&min_Y,double &Global_min_ratio_cut);
vector<int> Two_Way_Partitioning(vector<int>V_dash,long int Min_PageSize);
void cluster_nodes_into_pages(vector<int>Nodes,long int Min_Page_Size,int flag);
vector<int>BFS(vector<int>F,unordered_map<int,int>Nodes_present,long int limit);
void remove_from_F(vector<int>&F, vector<int>&V_dash, unordered_map<int,int>&Nodes_present);
void insert_node(string NewRecord_FileName,int New_node,int policy);
void Merge_two_pages(string Filename1,string Filename2,int Old_Node_Page);



bool sortbysec(const pair<int,int> &a, const pair<int,int> &b) { 
    return (a.second < b.second); 
} 
  




long int Calculate_Size(vector<int>V){
	long int size = 0;
	for(int i=0;i<V.size();i++){
		size = size + RecordSizes[V[i]];
	}
	return size;
}



int string_to_int(string s){
	stringstream geek(s);
	int x = 0;
	geek>>x;
	return x;
}





void write_into_newpage(vector<int>records,int flag){
	
	string pageno2;
	for(int i=0;i<records.size();i++){
		//goto repective page and bring it
		ifstream infile;
		ofstream outfile1;
		string pageno1 = "page_" + to_string(New_PageNo) + ".txt";
		outfile1.open(pageno1,ios_base::app);
		pageno2 = "page_"+to_string(Bplus[records[i]])+".txt";
		infile.open(pageno2);
		int count = 0;
		if(infile.is_open()){
			while(infile){
				string s;
			    if (!getline( infile, s )) break;
			    if(count == 0 && flag == 0){
			    	count++;
			    	continue;
			    }
			    istringstream ss( s );
			    vector <string> record;
			    while (ss)
			    {
			      string s;
			      if (!getline( ss, s, ' ' )) break;
			      record.push_back( s );
			    }
			    if(string_to_int(record[1]) == records[i] ){
			    	outfile1<<s<<endl;
			    }
				
			}
		}
		infile.close();
		Bplus[records[i]] = New_PageNo;
	}
	New_PageNo++;
}


vector<int>BFS(vector<int>F,unordered_map<int,int>Nodes_present,long int limit){
	unordered_map<int,int>visited;
	srand(time(0));
	int random_number=(rand()%F.size()); 
	int current_node = F[random_number];
	vector<int>V_dash;
	V_dash.push_back(current_node);
	visited[current_node] = 1;
	queue<int>my_queue;
	my_queue.push(current_node);
	long int count = 0;

	while(count<=(8388608-limit) && !my_queue.empty()){
		current_node = my_queue.front();
		my_queue.pop();
		ifstream infile;
		string pageno;
		pageno = "page_"+to_string(Bplus[current_node])+".txt";
		infile.open(pageno);
		int count_space = 0;
		while(infile){
			string s;
		    if (!getline( infile, s )) break;
		    if(count_space!=0){
		    	istringstream ss( s );
			    vector <string> record;
			    while (ss){
			      string s;
			      if (!getline( ss, s, ' ' )) break;
			      record.push_back( s );
			    }
			    int N2 = string_to_int(record[2]);
			    if(record.size()!=0 && record[0] == "s" && record[1] == to_string(current_node) && !visited[N2] && Nodes_present[N2]){
			    	my_queue.push(N2);
			    	V_dash.push_back(N2);
			    	visited[N2] = 1;
			    	count = count + RecordSizes[N2];
			    }
		    }
		    count_space++;
		}
		infile.close();
	}
	return V_dash;
}

int find_t(vector<int>V_dash){
	unordered_map<int,int>U_V_dash;
	unordered_map<int,int>visited;
	int Latest;
	for(int i=0;i<V_dash.size();i++){
		U_V_dash[V_dash[i]] = 1;    // This means these nodes are present in the subgraph
	}
	int current_node = V_dash[0];
	visited[current_node] = 1;
	queue<int>my_queue;
	my_queue.push(current_node);
	while(!my_queue.empty()){
		current_node = my_queue.front();
		my_queue.pop();
		ifstream infile;
		string pageno;
		pageno = "page_"+to_string(Bplus[current_node])+".txt";
		infile.open(pageno);
		int count_space = 0;
		while(infile){
			string s;
		    if (!getline( infile, s )) break;
		    if(count_space!=0){
			    istringstream ss( s );
			    vector <string> record;
			    while (ss)
			    {
			      string s;
			      if (!getline( ss, s, ' ' )) break;
			      record.push_back( s );
			    }
			    int N2 = string_to_int(record[2]);
			    if(record[0] == "s" && record[1] == to_string(current_node) && U_V_dash[N2]){
			    	pair <int,int> edge_pair;
			    	edge_pair = make_pair(current_node,N2);
			    	Sub_Graph_Edges[edge_pair] = 1;
			    	if(!visited[N2] ){
				    	my_queue.push(N2);
				    	visited[N2] = 1;
				    	Latest = N2;
			    	}
			    }
			    else if(record[0] == "p" && record[1] == to_string(current_node) && U_V_dash[N2]){
			    	pair <int,int> edge_pair;
			    	edge_pair = make_pair(N2,current_node);
			    	Sub_Graph_Edges[edge_pair] = 1;
			    	if(!visited[N2] ){
				    	my_queue.push(N2);
				    	visited[N2] = 1;
				    	Latest = N2;
			    	}
			    }
		    }
			count_space++;
		}
		infile.close();
	}

	return Latest;
}



void Left_Shifting(vector<int>&X,vector<int>&Y,int s,int t,vector<int>&min_X,vector<int>&min_Y,double &Global_min_ratio_cut){
	double ratio_cut;
	int module_to_move;

	while(Y.size()!=1){
		double Local_min_ratio_cut = 100000.0;
		for(int i=0;i<Y.size();i++){
			if(Y[i] == t){
				continue;
			}
			int count = 0;
			for(int j=0;j<X.size();j++){
				for(int k=0;k<Y.size();k++){
					if(Y[k]!=Y[i]){
						if(Sub_Graph_Edges[make_pair(X[j],Y[k])] == 1){
							count = count+1;
						}
                        if(Sub_Graph_Edges[make_pair(Y[k],X[j])] == 1){
							count = count+1;
						}
						if(Sub_Graph_Edges[make_pair(Y[i],Y[k])] == 1 && j==0){
							count = count + 1;
						}
						if(Sub_Graph_Edges[make_pair(Y[k],Y[i])] == 1 && j==0){
							count = count + 1;
						}
					}
				}
			}
            
			ratio_cut = (count+0.0)/((X.size()+1)*(Y.size()-1));
			if(ratio_cut<Local_min_ratio_cut){
				Local_min_ratio_cut = ratio_cut;
				module_to_move = Y[i];
			}
		}
		//We came to know which module to move
		//cout<<"module to move: "<<module_to_move<<" "<<Local_min_ratio_cut<<endl;
		X.push_back(module_to_move);
		vector<int>Y_dummy;
		for(int i=0;i<Y.size();i++){
			if(Y[i]!=module_to_move){
				Y_dummy.push_back(Y[i]);
			}
		}
		Y.clear();
		Y = Y_dummy;
		Y_dummy.clear();
		//moved
		if(Global_min_ratio_cut>Local_min_ratio_cut){
			Global_min_ratio_cut = Local_min_ratio_cut;
			min_X = X;
			min_Y = Y;
            //cout<<"Global_min_ratio_cut: "<<Global_min_ratio_cut<<endl;
            
		}
	}

}


void Right_Shifting(vector<int>&X,vector<int>&Y,int s,int t,vector<int>&min_X,vector<int>&min_Y,double &Global_min_ratio_cut){
	double ratio_cut;
	int module_to_move;

	while(X.size()!=1){
		double Local_min_ratio_cut = 100000.0;
		for(int i=0;i<X.size();i++){
			if(X[i] == s){
				continue;
			}
			int count = 0;
			for(int j=0;j<Y.size();j++){
				for(int k=0;k<X.size();k++){
					if(X[k]!=X[i]){
						if(Sub_Graph_Edges[make_pair(Y[j],X[k])] == 1){
							count = count+1;
						}
                        if(Sub_Graph_Edges[make_pair(X[k],Y[j])] == 1){
							count = count+1;
						}
						if(Sub_Graph_Edges[make_pair(X[i],X[k])] == 1 && j==0){
							count = count + 1;
						}
						if(Sub_Graph_Edges[make_pair(X[k],X[i])] == 1 && j==0){
							count = count + 1;
						}
					}
				}
			}
            
			ratio_cut = (count+0.0)/((X.size()-1)*(Y.size()+1));
			if(ratio_cut<Local_min_ratio_cut){
				Local_min_ratio_cut = ratio_cut;
				module_to_move = X[i];
			}
		}
		//We came to know which module to move
		//cout<<"module to move: "<<module_to_move<<" "<<Local_min_ratio_cut<<endl;
		Y.push_back(module_to_move);
		vector<int>X_dummy;
		for(int i=0;i<X.size();i++){
			if(X[i]!=module_to_move){
				X_dummy.push_back(X[i]);
			}
		}
		X.clear();
		X = X_dummy;
		X_dummy.clear();
		//moved
		if(Global_min_ratio_cut>Local_min_ratio_cut){
			Global_min_ratio_cut = Local_min_ratio_cut;
			min_X = X;
			min_Y = Y;
            //cout<<"Global_min_ratio_cut: "<<Global_min_ratio_cut<<endl;
            
		}
	}

}





vector<int> Two_Way_Partitioning(vector<int>V_dash,long int Min_PageSize){
	//Step1: i.e. Initialising
	int s = V_dash[0];
	int t = find_t(V_dash);
	double dummy_Global_min;
    char side='r';
	vector<int>X;
	vector<int>Y;
	vector<int>min_X;
	vector<int>min_Y;
	double Global_min_ratio_cut = 1000000.0;
	X.push_back(s);
	for(int i=1;i<V_dash.size();i++){
		Y.push_back(V_dash[i]);
	}
	//here X contains only s and Y contains remaining including t
	min_X = X;
	min_Y = Y;
	cout<<"Initial partition started"<<endl<<endl;
	Left_Shifting(X,Y,s,t,min_X,min_Y,Global_min_ratio_cut);
	dummy_Global_min = Global_min_ratio_cut;
	//move modules from Y to X completed and X contains all except t and Y contains only t
	Right_Shifting(X,Y,s,t,min_X,min_Y,Global_min_ratio_cut);
	//We get the intial partition in min_X and min_Y 

	cout<<"Iterative Shifting started"<<endl<<endl;
	int flag = 0;
	if(Global_min_ratio_cut<dummy_Global_min){
		//It means Global mincut due to right shifting
		flag = 1;
	}
	X = min_X;
	Y = min_Y;


	while(1){
		if(flag == 0){
			vector<int>duplicate_X;
			vector<int>duplicate_Y;
			dummy_Global_min = Global_min_ratio_cut;
			duplicate_X = X;
			duplicate_Y = Y;
			Global_min_ratio_cut = DBL_MAX;
			Right_Shifting(X,Y,s,t,min_X,min_Y,Global_min_ratio_cut);
			if(Global_min_ratio_cut>=dummy_Global_min){
				X = duplicate_X;
				Y = duplicate_Y;
				Global_min_ratio_cut = dummy_Global_min;
				break;
			}
			flag = 1;
			X = min_X;
			Y = min_Y;
		}
		else{
			vector<int>duplicate_X;
			vector<int>duplicate_Y;
			dummy_Global_min = Global_min_ratio_cut;
			duplicate_X = X;
			duplicate_Y = Y;
			Global_min_ratio_cut = DBL_MAX;
			Left_Shifting(X,Y,s,t,min_X,min_Y,Global_min_ratio_cut);
			if(Global_min_ratio_cut>=dummy_Global_min){
				X = duplicate_X;
				Y = duplicate_Y;
				Global_min_ratio_cut = dummy_Global_min;
				break;
			}
			flag = 0;
			X = min_X;
			Y = min_Y;
		}
	}


	cout<<"Group Swapping started"<<endl<<endl;

    //step:3 i.e group swapping
    unordered_map<int,int>locked;
    int total_size=X.size()+Y.size();
    int locked_count=2;
    double local_gain= -100000.0;
    double global_gain=0.1;
    int module_to_move = -1;
    vector<int>minX;
    vector<int>minY;
    int Number_of_passes = 0;
    //loop till global gain is non-positive
    while(1){
    	cout<<"The ratio_cut after iterative shifting is: "<<(calculate_crossedges(X,Y)+0.0)/(X.size()*Y.size())<<endl<<endl; 
		cout<<endl<<endl;
    	global_gain = 0.0;
        locked.clear();
        locked[s]=1;
        locked[t]=1;
        locked_count=2;
        //as the loop needs to be continued till all the elements are locked, a variable(locked_count) is initialised
        //in order to check which elements are locked, an unordered_map is initialised where s and t are always initialised to 1
        // a variable 'side' is maintained to check if the element with largest 'local_gain' is from X or Y
        vector<int>duplicate_X;
        vector<int>duplicate_Y;
        duplicate_X = X;
        duplicate_Y = Y;
        double Largest_Acc_Gain = 0.0;
        while(locked_count<total_size){
            //check for elements that are not locked in X
            local_gain=-100000.0;
            for(int i=0;i<X.size();i++){
                if(locked[X[i]]==0){ //if not locked
                    double tmp_gain;
                    tmp_gain=calculate_gain(X,Y,X[i],'x',1);// get the gain
                    if(tmp_gain>local_gain){ // if it is greater than local, change the value of local to tmp and update side
                        local_gain=tmp_gain;
                        //global_gain=max(global_gain,local_gain);
                        module_to_move=X[i];
                        side='x';
                    }
                }
            }
            //same for elements in Y
            for(int i=0;i<Y.size();i++){
                if(locked[Y[i]]==0){
                    double tmp_gain;
                    tmp_gain=calculate_gain(X,Y,Y[i],'y',1);
                    if(tmp_gain>local_gain){
                        local_gain=tmp_gain;
                        // global_gain=max(global_gain,local_gain);
                        module_to_move=Y[i];
                        side='y';
                    }
                }
            }
            //shift the module with largest gain here
            //element can be from X or Y. Use 'side' variable to find the set
            if(side=='x'){
                Y.push_back(module_to_move);
                vector<int>X_dummy;
                for(int i=0;i<X.size();i++){
                    if(X[i]!=module_to_move){
                        X_dummy.push_back(X[i]);
                    }
                }
                X=X_dummy;
                X_dummy.clear();
            }
            else{
                X.push_back(module_to_move);
                vector<int>Y_dummy;
                for(int i=0;i<Y.size();i++){
                    if(Y[i]!=module_to_move){
                        Y_dummy.push_back(Y[i]);
                    }
                }
                Y=Y_dummy;
                Y_dummy.clear();
            }
            
        	global_gain = global_gain + local_gain;
        	if(global_gain>Largest_Acc_Gain){
        		Largest_Acc_Gain = global_gain;
        		minX = X;
        		minY = Y;
        		cout<<"accumulated gain: "<<Largest_Acc_Gain<<endl<<endl;
        		cout<<"The ratio cut after improvement from before: "<<(calculate_crossedges(minX,minY)+0.0)/(minX.size()*minY.size())<<endl<<endl;
        	}
            locked_count++; //increase the count of locked nodes
            locked[module_to_move]=1; //update the map
        }
        cout<<"Largest accumulated gain: "<<Largest_Acc_Gain<<endl<<endl;
        if(Largest_Acc_Gain > 0){
        	cout<<"Largest accumulated gain is positive and new X , Y are:"<<endl<<endl;
        	X = minX;
        	Y = minY;
        }
        else{
        	X = duplicate_X;
        	Y = duplicate_Y;
        	break;
        }  
        Number_of_passes++;
        if(Number_of_passes>20){
        	cout<<"Broken Here"<<endl<<endl;
        	break;
        }
    }

	cout<<"group swapping ended"<<endl<<endl;
    //Here we get two partitions X and Y

    long int size_X = 0;
    long int size_Y = 0;
    size_X = Calculate_Size(X);
    size_Y = Calculate_Size(Y);

    cout<<"Old size X and Y : "<<size_X<<" "<<size_Y<<endl<<endl;
    int nodes_moved = 0; 

    while(size_X<Min_PageSize || size_Y<Min_PageSize){
    	if(size_X<Min_PageSize){
    		double minimum_gain = 100000.0;
    		int module_to_move;
    		for(int i=0;i<Y.size();i++){
    			double dummy_gain = calculate_gain(X,Y,Y[i],'y',2);
    			if(dummy_gain<minimum_gain){
    				module_to_move = Y[i];
    				minimum_gain = dummy_gain;
    			}
    		}
    		X.push_back(module_to_move);
    		vector<int> Y_dummy;
    		for(int i=0;i<Y.size();i++){
    			if(Y[i]!=module_to_move){
    				Y_dummy.push_back(Y[i]);
    			}
    		}
    		Y = Y_dummy;
    	}
    	else if(size_Y<Min_PageSize){
    		double minimum_gain = 100000.0;
    		int module_to_move;
    		for(int i=0;i<X.size();i++){
    			double dummy_gain = calculate_gain(X,Y,X[i],'x',2);
    			if(dummy_gain<minimum_gain){
    				module_to_move = X[i];
    				minimum_gain = dummy_gain;
    			}
    		}
    		Y.push_back(module_to_move);
    		vector<int> X_dummy;
    		for(int i=0;i<X.size();i++){
    			if(X[i]!=module_to_move){
    				X_dummy.push_back(X[i]);
    			}
    		}
    		X = X_dummy;
    	}
    	size_X = 0;
    	size_Y = 0;
    	size_X = Calculate_Size(X);
    	size_Y = Calculate_Size(Y);
	    nodes_moved++;
	    cout<<"Nodes moved : "<<nodes_moved<<endl<<endl;

    }

	cout<<"New size X and Y : "<<size_X<<" "<<size_Y<<endl<<endl;
    return X;
    
}


int calculate_crossedges(vector<int>X,vector<int>Y){
	int count=0;
    for(int i=0;i<X.size();i++){
        for(int j=0;j<Y.size();j++){
            if(Sub_Graph_Edges[make_pair(X[i],Y[j])] == 1){
                count++;
            }
            if(Sub_Graph_Edges[make_pair(Y[j],X[i])] == 1){
                count++;
            }
        }
    }
    return count;
}


double calculate_gain(vector<int>X,vector<int>Y,int element,char side, int type){
    //calculate ratio_cut1 without shifting the element
    double ratio_cut1,ratio_cut2;
    ratio_cut1 = (calculate_crossedges(X,Y)+0.0)/((Y.size())*(X.size()));
    int count=0;

    //calculate ratio_cut2. As there are two sets, there will be 2 conditions.
    //In order to transfer from set X to set Y, enter into condition1 else into condition2
    if(side=='x'){
        //calculate number of edges crossing the cut without 'element'
        Y.push_back(element);
        vector<int>X_dummy;
        for(int i=0;i<X.size();i++){
        	if(X[i]!=element){
        		X_dummy.push_back(X[i]);
        	}
        }
        X = X_dummy;
        
        ratio_cut2 = (calculate_crossedges(X,Y)+0.0)/((Y.size())*(X.size()));
    }
    else{
     	X.push_back(element);
     	vector<int>Y_dummy;
     	for(int i=0;i<Y.size();i++){
     		if(Y[i]!=element){
     			Y_dummy.push_back(Y[i]);
     		}
     	}
     	Y = Y_dummy;
        ratio_cut2 = (calculate_crossedges(X,Y)+0.0)/((Y.size())*(X.size()));
    }
    if(type==1)
    	return ratio_cut1-ratio_cut2;
    else return ratio_cut2-ratio_cut1;
}

void remove_from_F(vector<int>&F, vector<int>&V_dash, unordered_map<int,int>&Nodes_present){
	unordered_map<int,int>h;
	for(int i=0;i<V_dash.size();i++){ //maintain a hash map for element of V''
		h[V_dash[i]]=1;
	}
	vector<int>dummyF;
	dummyF=F;
	F.clear();
	for(int i=0;i<dummyF.size();i++){
		if(h[dummyF[i]]==0){
			F.push_back(dummyF[i]); //map all node that are present in F-V'
			Nodes_present[dummyF[i]]=1;
		}
	}
	dummyF.clear();
}

//main functions
void cluster_nodes_into_pages(vector<int>Nodes,long int Min_Page_Size,int Flag){
	vector<int>F;
	F = Nodes;

	while(F.size()!=0){
		//choose a V' from F
		Sub_Graph_Edges.clear();
		cout<<"F size: "<<F.size()<<endl<<endl;
		vector<int>V_dash;
		unordered_map<int,int>Nodes_present;
		for(int i=0;i<F.size();i++){
			Nodes_present[F[i]] = 1;
		}

		int BFS_count = 0;
		if(Flag == 0){
			while(1){
				cout<<"The size of V_dash is: "<<V_dash.size()<<" The size in bytes "<<(Calculate_Size(V_dash))/(1024*1024)<<" "<<Calculate_Size(V_dash)<<endl<<endl;

				if(Calculate_Size(V_dash)<PageSize){
					cout<<"F.size(): "<<F.size()<<endl<<endl;
					cout<<"V_dash.size(): "<<V_dash.size()<<endl<<endl;
					cout<<"Number of nodes still there : "<<F.size()-V_dash.size()<<endl<<endl;
					if(F.size()>0){ //there are nodes that need to be traversed
						BFS_count++;
						Nodes_present.clear();
						remove_from_F(F,V_dash,Nodes_present);//remove this line
						vector<int>dummy_V_dash;
						long int Size_limit = Calculate_Size(V_dash);
						dummy_V_dash = BFS(F,Nodes_present,Size_limit);
						cout<<"dummy_V_dash.size(): "<<dummy_V_dash.size()<<endl<<endl;
						V_dash.insert(V_dash.end(),dummy_V_dash.begin(),dummy_V_dash.end());
						dummy_V_dash.clear();
						remove_from_F(F,V_dash,Nodes_present);
					}
					else{
						cout<<"Need to do some more changes if i get segfault here"<<endl;
						write_into_newpage(V_dash,Flag);
						return;
						//handle extra
					}
				}
				else{
					break;
				}
			}
		}
		else{
			V_dash = Nodes;
			F.clear();
		}
		cout<<"BFS_count "<<BFS_count<<endl<<endl;
		cout<<"The size of V_dash is: "<<V_dash.size()<<" The size in bytes "<<(Calculate_Size(V_dash))/(1024*1024)<<" "<<Calculate_Size(V_dash)<<endl<<endl;
		vector<int>A;
		vector<int>A_dash;
		unordered_map<int,int>A_map;
		A = Two_Way_Partitioning(V_dash,Min_Page_Size);
		for(int i=0;i<A.size();i++){
			A_map[A[i]]=1;
		}
		for(int i=0;i<V_dash.size();i++){
			if(A_map[V_dash[i]]==0){
				A_dash.push_back(V_dash[i]);
				A_map[V_dash[i]]=1;
			}
		}
		// vector<int>F_dummy;
		// for(int i=0;i<F.size();i++){
		// 	if(A_map[F[i]]==0){
		// 		F_dummy.push_back(F[i]);
		// 	}
		// }
		long int A_size;
		if(Calculate_Size(A)>PageSize){
			cout<<"exceeded page size"<<endl;
			//remove from F i.e add to F_dummy
			for(int i=0;i<A.size();i++){
				F.push_back(A[i]);
			}
		}
		else{
			write_into_newpage(A,Flag);

		}
		long int A_dash_size;
		if(Calculate_Size(A_dash)>PageSize){
			//remove from F i.e add to F_dummy
			cout<<"exceeded page size"<<endl;
			for(int i=0;i<A_dash.size();i++){
				F.push_back(A_dash[i]);
			}
		}
		else{
			write_into_newpage(A_dash,Flag);
		}
		A.clear();
		A_dash.clear();
		V_dash.clear();
	}
}
int calculate_no_of_suc_pred(int node){ //calculate number of successors and predecessors of node
	string pageno ;
	pageno = "page_"+ to_string(Bplus[node])+".txt"; //get the pagenumber using 'Bplus' hash map
	ifstream infile;
	infile.open(pageno);
	int count=0;
	int count1 = 0;
	while(infile){
		string s;
		if (!getline( infile, s )) break;
		if(count1 == 0){
			count1++;
			continue;
		}
		istringstream ss( s );
		vector <string> record;
		while (ss){
			string s;
			if (!getline( ss, s, ' ' )) break;
			record.push_back( s );
		}
		int N2 = string_to_int(record[1]); //if node1==node increment the count
		if(N2 == node){
			count++;
		}
	}
	infile.close();
	return count;
}
//calculate the number of successors and predecessors of every node in vector
int calculate_no_of_rows(vector<int>A){
	int count=0;
	for(int i=0;i<A.size();i++){
		count+=calculate_no_of_suc_pred(A[i]); //get the count of suc and pred in A[i]
	}
	return count;
}



bool IsPresent(int check_node,vector<int>nodes){
	for(int i=0;i<nodes.size();i++){
		if(nodes[i] == check_node){
			return true;
		}
	}
	return false;
}

vector<int>Get_Nodes_In_Page(string FileName){
	vector<int>Nodes;
	ifstream infile;
	infile.open(FileName);
	if(infile.is_open()){
		while(infile){
			string s;
		    if (!getline( infile, s )) break;
		    istringstream ss( s );
		    vector <string> record;
		    while (ss)
		    {
		      string s;
		      if (!getline( ss, s, ' ' )) break;
		      record.push_back( s );
		    }
		    if(!IsPresent(string_to_int(record[1]),Nodes)){
		    	Nodes.push_back(string_to_int(record[1]));
		    }
			
		}
	}
	infile.close();
	return Nodes;
}



vector<int> Get_Neighbour_Pages(string NewRecord_FileName,int Node){
	vector<int>Neighbours_X;
	unordered_map<int,int>Neighbour_Check;
	ifstream infile;
	infile.open(NewRecord_FileName);
	if(infile.is_open()){
		while(infile){
			string s;
		    if (!getline( infile, s )) break;
		    istringstream ss( s );
		    vector <string> record;
		    while (ss)
		    {
		      string s;
		      if (!getline( ss, s, ' ' )) break;
		      record.push_back( s );
		    }
		    if(Node == string_to_int(record[1])){
		    	if(!Neighbour_Check[Bplus[string_to_int(record[2])]]){
		    	Neighbours_X.push_back(Bplus[string_to_int(record[2])]);
		    	Neighbour_Check[Bplus[string_to_int(record[2])]] = 1;
		    	}
		    }
		    
			
		}
	}
	infile.close();
	return Neighbours_X;
}



void update_pages_insert(vector<int>Neighbours_X,string NewRecord_FileName,int New_Node){
	
	for(int i=0;i<Neighbours_X.size();i++){
		//for each neighbour read the new record and add its succesors and predecessors
		ifstream infile;
		ofstream outfile1;
		string pageno1 = "page_" + to_string(Neighbours_X[i]) + ".txt";
		outfile1.open(pageno1,ios_base::app);
		string pageno2 = NewRecord_FileName;
		infile.open(pageno2);
		int count = 0;
		if(infile.is_open()){
			while(infile){
				string s;
			    if (!getline( infile, s )) break;
			    istringstream ss( s );
			    vector <string> record;
			    while (ss)
			    {
			      string s;
			      if (!getline( ss, s, ' ' )) break;
			      record.push_back( s );
			    }
			    if(Neighbours_X[i] == Bplus[string_to_int(record[2])]){
			    	if(record[0] == "s"){
			    		string changed_line = "p "+record[2]+" "+record[1];
			    		for(int j=3;j<record.size();j++){
			    			changed_line = changed_line + " "+record[j];
			    		}
			    		outfile1<<changed_line<<endl;
			    	}
			    	else if(record[0] == "p"){
			    		string changed_line = "s "+record[2]+" "+record[1];
			    		for(int j=3;j<record.size();j++){
			    			changed_line = changed_line + " "+record[j];
			    		}
			    		outfile1<<changed_line<<endl;
			    	}
			    }
				
			}
		}
		infile.close();
	}
}



long int Calculate_File_Size(string FileName){
		streampos begin,end;
  		ifstream myfile (FileName, ios::binary);
  		begin = myfile.tellg();
  		myfile.seekg (0, ios::end);
  		end = myfile.tellg();
  		myfile.close();
  		long int A;
  		A = end-begin;
  		return A;
}

int Select_Page(vector<int>Neighbours_X,string NewRecord_FileName,int New_Node){
	vector< pair <int,int> > vect;
	for (int i=0; i<Neighbours_X.size(); i++) 
		vect.push_back( make_pair(Neighbours_X[i],0) );  

	ifstream infile;
	infile.open(NewRecord_FileName);
	if(infile.is_open()){
		while(infile){
			string s;
		    if (!getline( infile, s )) break;
		    istringstream ss( s );
		    vector <string> record;
		    while (ss)
		    {
		      string s;
		      if (!getline( ss, s, ' ' )) break;
		      record.push_back( s );
		    }
		    
		    for(int i=0;i<Neighbours_X.size();i++){
		    	if(Neighbours_X[i] == Bplus[string_to_int(record[2])]){
		    		vect[i].second = vect[i].second + 1;
		    		break;
		    	}
		    }
			
		}
	}
	infile.close();

	//Vect consists of X-neighbour page nos  and number of neighbours of X in each page 
	sort(vect.begin(), vect.end(), sortbysec); 
	for(int i=0;i<Neighbours_X.size();i++){
		string Filename = "page_"+to_string(Neighbours_X[i])+".txt";
		if((Calculate_File_Size(Filename)+Calculate_File_Size(NewRecord_FileName))<=PageSize){
			return Neighbours_X[i];
		}
	}

	return Neighbours_X[0];

}


void insert_node(string NewRecord_FileName,int New_Node ,int policy){
	vector<int>Neighbours_X;
	Neighbours_X = Get_Neighbour_Pages(NewRecord_FileName,New_Node);
	for(int i=0;i<Neighbours_X.size();i++){
		cout<<Neighbours_X[i]<<endl;
	}

	if(Neighbours_X.size() == 0){
		//Add this record to a new page and return
		vector<int>records;
		records.push_back(New_Node);
		write_into_newpage(records,1);
		return;
	}

	//copy the pages to the main memeory and and update them
	update_pages_insert(Neighbours_X,NewRecord_FileName,New_Node);
	cout<<"Stage1"<<endl;
	//select a page among neighbours to push the record
	int Selected_Page = Select_Page(Neighbours_X,NewRecord_FileName,New_Node);
	//Append NewRecord_FileName contents to this page
	string Selected_Page_Name = "page_"+to_string(Selected_Page)+".txt";
	cout<<"selected page: "<<Selected_Page_Name<<endl<<endl;

	Merge_two_pages(Selected_Page_Name,NewRecord_FileName,Selected_Page);
	cout<<"Stage3"<<endl;
	if(policy == 1){
		for(int i=0;i<Neighbours_X.size();i++){
			string Filename = "page_"+to_string(Neighbours_X[i])+".txt";
			if(Calculate_File_Size(Filename)<=PageSize){
				//leave it no changes needed
				cout<<"Went into if"<<endl;
			}
			else{
				//cluster the nodes of this page into two pages
				cout<<"Went into else"<<endl;

				vector<int>Nodes_in_page;
				string FileName = "page_"+to_string(Neighbours_X[i])+".txt";
				cout<<"overflowed: "<<FileName<<endl<<endl;
				Nodes_in_page = Get_Nodes_In_Page(FileName);
				cout<<"Nodes in Page"<<endl;
				for(int i=0;i<Nodes_in_page.size();i++){
					cout<<Nodes_in_page[i]<<" ";
				}
				cout<<endl;

				cluster_nodes_into_pages(Nodes_in_page,PageSize/2,1);
				remove(Filename.c_str());
			}
		}
	}

}


vector<int> update_pages_delete(vector<int>Neighbours_X,int Old_Node){
	//for each neighbour remove the nodes of Old_node
	vector<int>Changed_Neighbours_X;
	for(int i=0;i<Neighbours_X.size();i++){
		ifstream infile;
		ofstream outfile1;
		string pageno1 = "page_" + to_string(New_PageNo) + ".txt";
		outfile1.open(pageno1,ios_base::app);
		string pageno2 = "page_"+to_string(Neighbours_X[i])+".txt";
		infile.open(pageno2);
		if(infile.is_open()){
			while(infile){
				string s;
			    if (!getline( infile, s )) break;
			    istringstream ss( s );
			    vector <string> record;
			    while (ss)
			    {
			      string s;
			      if (!getline( ss, s, ' ' )) break;
			      record.push_back( s );
			    }
			    if(string_to_int(record[2]) != Old_Node){
			    	outfile1<<s<<endl;
			    	Bplus[string_to_int(record[1])] = New_PageNo;
			    }
				
			}
		}
		infile.close();
		remove(pageno2.c_str());
		Changed_Neighbours_X.push_back(New_PageNo);
		New_PageNo++;
	}

}



void Delete_Record_From_Page(string Del_Node_Page,int Old_Node){
	ifstream infile;
	ofstream outfile1;
	string pageno1 = "page" + to_string(New_PageNo) + ".txt";
	outfile1.open(pageno1,ios_base::app);
	string pageno2 = Del_Node_Page;
	infile.open(pageno2);
	if(infile.is_open()){
		while(infile){
			string s;
		    if (!getline( infile, s )) break;
		    istringstream ss( s );
		    vector <string> record;
		    while (ss)
		    {
		      string s;
		      if (!getline( ss, s, ' ' )) break;
		      record.push_back( s );
		    }
		    if(string_to_int(record[1]) != Old_Node){
		    	outfile1<<s<<endl;
		    	Bplus[string_to_int(record[1])] = New_PageNo;
		    }
			
		}
	}
	infile.close();
	remove(pageno2.c_str());
	New_PageNo++;
}

void Merge_two_pages(string Filename1,string Filename2,int Old_Node_Page){
	ifstream infile;
	ofstream outfile1;
	outfile1.open(Filename1,ios_base::app);
	infile.open(Filename2);
	if(infile.is_open()){
		while(infile){
			string s;
		    if (!getline( infile, s )) break;
		    istringstream ss( s );
		    vector <string> record;
		    while (ss)
		    {
		      string s;
		      if (!getline( ss, s, ' ' )) break;
		      record.push_back( s );
		    }
		    outfile1<<s<<endl;
		    Bplus[string_to_int(record[1])] = Old_Node_Page;
			
		}
	}
	infile.close();
	remove(Filename2.c_str());

}


void Delete_node(int Old_Node,int policy){
	string Del_Node_Page = "page_"+to_string(Bplus[Old_Node])+".txt";
	vector<int>Neighbours_X;
	Neighbours_X = Get_Neighbour_Pages(Del_Node_Page,Old_Node);
	//Remove the node from the neighbours of X
	vector<int>Changed_Neighbours_X;
	Changed_Neighbours_X = update_pages_delete(Neighbours_X,Old_Node);
	Del_Node_Page = "page_"+to_string(Bplus[Old_Node])+".txt";
	Delete_Record_From_Page(Del_Node_Page,Old_Node);
	int Old_Node_Page = New_PageNo-1; 
	Bplus[Old_Node] = -1;
	
	if(policy == 1){
		string Filename = "page_"+to_string(Old_Node_Page)+".txt";
		string Filename1;
		if(Calculate_File_Size(Filename)<(PageSize/2)){
				//underflow so merge two files
			int flag = 0;
			for(int i=0;i<Changed_Neighbours_X.size();i++){
				if(Changed_Neighbours_X[i]!=Old_Node_Page){
					Filename1 = "page_"+to_string(Changed_Neighbours_X[i])+".txt";
					if((Calculate_File_Size(Filename) + Calculate_File_Size(Filename1))<=PageSize){
						//Merge these two
						Merge_two_pages(Filename,Filename1,Old_Node_Page);
						flag = 1;
						break;
					}
  				}
			}
			if(flag == 0){
				//choose any page and cluster them

				vector<int>Nodes_in_page1;
				vector<int>Nodes_in_page2;
				Nodes_in_page1 = Get_Nodes_In_Page(Filename);
				Nodes_in_page2 = Get_Nodes_In_Page(Filename1);
				Nodes_in_page1.insert(Nodes_in_page1.end(),Nodes_in_page2.begin(),Nodes_in_page2.end());
				cluster_nodes_into_pages(Nodes_in_page1,PageSize/2,1);
				remove(Filename.c_str());
				remove(Filename1.c_str());
			}

		}
		Bplus[Old_Node] = 0;
	}



}



int main(){
	
	ifstream infile;
	infile.open("BPLUS.txt");
	int count = 0;
	if(infile.is_open()){
		while(infile){
			string s;
		    if (!getline( infile, s )) break;
		    if(count == 0){
		    	count++;
		    	continue;
		    }
		    istringstream ss( s );
		    vector <string> record;
		    while (ss)
		    {
		      string s;
		      if (!getline( ss, s, ' ' )) break;
		      record.push_back( s );
		    }
		    Bplus[string_to_int(record[0])] = string_to_int(record[1]);
		}
	}
	infile.close();

	infile.open("Record_size.txt");
	count = 0;
	if(infile.is_open()){
		while(infile){
			string s;
		    if (!getline( infile, s )) break;
		    if(count == 0){
		    	count++;
		    	continue;
		    }
		    istringstream ss( s );
		    vector <string> record;
		    while (ss)
		    {
		      string s;
		      if (!getline( ss, s, ' ' )) break;
		      record.push_back( s );
		    }
		    RecordSizes[string_to_int(record[0])] = string_to_int(record[1]);
		}
	}
	infile.close();


	while(1){
		int flag1;

		cout<<"Press 1 for querying"<<endl;
		cout<<"Press 2 for inserting a node"<<endl;
		cout<<"Press 3 for inserting an edge"<<endl;
		cout<<"Press 4 for deleting a node"<<endl;
		cout<<"Press 5 for deleting an edge"<<endl;
		cout<<"Press 0 for exiting"<<endl;
		cin>>flag1;
		if(flag1 == 1){
			int flag2;
			cout<<"Press 1 for SP-TAG"<<endl;
			cout<<"Press 2 for BEST"<<endl;
			cin>>flag2;
			if(flag2 == 1){
				int Start_Node,End_Node,Time;
				cout<<"Start node : ";
				cin>>Start_Node;
				cout<<"End Node : ";
				cin>>End_Node;
				cout<<"Time : ";
				cin>>Time;


			}
			else{
				int Start_Node,End_Node,Time;
				cout<<"Start node : ";
				cin>>Start_Node;
				cout<<"End Node : ";
				cin>>End_Node;

			}

		}
		else if(flag1 == 2){
			//insert a node with ssuccessor list and predecessor list i.e a new record
			int File_No,New_Node;
			cout<<"Node no"<<endl;
			cin>>New_Node;
			cout<<"File Number"<<endl;
			cin>>File_No;
			string NewRecord_FileName = "page_"+to_string(File_No)+".txt";
			Bplus[New_Node] = File_No;
			insert_node(NewRecord_FileName,New_Node,policy);
		}
		else if(flag1 == 3){

		}
		else if(flag1 == 4){
			int Old_Node;
			cout<<"Node no"<<endl;
			cin>>Old_Node;
			Delete_node(Old_Node,policy);
		}
		else if(flag1 == 5){

		}
		else{
			return 0;
		}
	}

return 0;
}