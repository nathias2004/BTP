#include<iostream>
#include<bits/stdc++.h>
#include<boost/functional/hash.hpp>
using namespace std;
int Min_PageSize;
int No_Records_Page = 28;
unordered_map<int,int>Bplus;
unordered_map<pair<int,int>,int,boost::hash<pair<int,int>>>Sub_Graph_Edges;


double calculate_gain(vector<int>X,vector<int>Y,int element,char side);


int string_to_int(string s){
	stringstream geek(s);
	int x = 0;
	geek>>x;
	return x;
}



vector<int>BFS(vector<int>F,unordered_map<int,int>Nodes_present){
	unordered_map<int,int>visited;
	int current_node = F[0];
	vector<int>V_dash;
	V_dash.push_back(current_node);
	visited[current_node] = 1;
	queue<int>my_queue;
	my_queue.push(current_node);
	int count = 1;

	while(count<=56 && !my_queue.empty()){
		current_node = my_queue.front();
		ifstream infile;
		string pageno;
		pageno = "page_"+to_string(Bplus[current_node])+".txt";
		infile.open(pageno);
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
		    int N2 = string_to_int(record[2]);
		    if(record[0] == "s" && record[1] == to_string(current_node) && !visited[N2] && Nodes_present[N2]){
		    	my_queue.push(N2);
		    	V_dash.push_back(N2);
		    	visited[N2] = 1;
		    	count++;
		    }

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
		ifstream infile;
		string pageno;
		pageno = "page_"+to_string(Bplus[current_node])+".txt";
		infile.open(pageno);
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

		}
		infile.close();
	}

	return Latest;
}


vector<int> Two_Way_Partitioning(vector<int>V_dash,int Min_PageSize){
	//Step1: i.e. Initialising
	int s = V_dash[0];
	int t = find_t(V_dash);
    char side='r';
	vector<int>X;
	vector<int>Y;
	vector<int>min_X;
	vector<int>min_Y;
	double Global_min_ratio_cut = 1000000.0;
	X.push_back(s);
	for(int i=1;i<V_dash.size();i++){
		if(V_dash[i]!=t){
			Y.push_back(V_dash[i]);
		}
	}
	min_X = X;
	min_Y = Y;
	//move from Y to X
	double ratio_cut;
	int module_to_move;

	while(Y.size()!=0){
		double Local_min_ratio_cut = 100000.0;
		for(int i=0;i<Y.size();i++){
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
					}
				}
				if(Sub_Graph_Edges[make_pair(X[j],t)] == 1){
					count++;
				}
                if(Sub_Graph_Edges[make_pair(t,X[j])] == 1){
					count++;
				}
			}
            //can be fitted in the above loop
			for(int k=0;k<Y.size();k++){
                if(Y[k]!=Y[i]){
                    if(Sub_Graph_Edges[make_pair(Y[i],Y[k])] == 1){
                        count = count+1;
                    }
                    if(Sub_Graph_Edges[make_pair(Y[k],Y[i])] == 1){
                        count = count+1;
                    }
                }
			}
			if(Sub_Graph_Edges[make_pair(Y[i],t)] == 1){
				count++;
			}
            if(Sub_Graph_Edges[make_pair(t,Y[i])] == 1){
				count++;
			}
			ratio_cut = count/((X.size()+1)*(Y.size()));
			if(ratio_cut<Local_min_ratio_cut){
				Local_min_ratio_cut = ratio_cut;
				module_to_move = Y[i];
			}
		}
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
		if(Global_min_ratio_cut>Local_min_ratio_cut){
			min_X = X;
			min_Y = Y;
            side='y';
		}
	}
	//left to right i.e X to Y

    for(int i=0;i<X.size();i++){
	    if(X[i]!=s){
            Y.push_back(X[i]);
        }
    }
    X.clear();
    X.push_back(t);

	while(Y.size()!=0){
		double Local_min_ratio_cut = 100000.0;
		for(int i=0;i<Y.size();i++){
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
					}
				}
				if(Sub_Graph_Edges[make_pair(X[j],t)] == 1){
					count++;
				}
                if(Sub_Graph_Edges[make_pair(t,X[j])] == 1){
					count++;
				}
			}
			for(int k=0;k<Y.size();k++){
                if(Y[k]!=Y[i]){
                    if(Sub_Graph_Edges[make_pair(Y[i],Y[k])] == 1){
                        count = count+1;
                    }
                    if(Sub_Graph_Edges[make_pair(Y[k],Y[i])] == 1){
                        count = count+1;
                    }
                }
			}
			if(Sub_Graph_Edges[make_pair(Y[i],s)] == 1){
				count++;
			}
            if(Sub_Graph_Edges[make_pair(s,Y[i])] == 1){
				count++;
            }
			ratio_cut = count/((X.size()+1)*(Y.size()));
			if(ratio_cut<Local_min_ratio_cut){
				Local_min_ratio_cut = ratio_cut;
				module_to_move = Y[i];
			}
		}
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
		if(Global_min_ratio_cut>Local_min_ratio_cut){
			min_X = X;
			min_Y = Y;
            side='x';
		}
	}

    //step:2 i.e. iterative shifting
    if(side=='y'){
        X=min_X;
        Y=min_Y;
    }
    else{
        X=min_Y;
        Y=min_X;
    }
    double prev_ratio_cut=DBL_MAX;
    while(Global_min_ratio_cut!=prev_ratio_cut){
        prev_ratio_cut=Global_min_ratio_cut;
        Global_min_ratio_cut=DBL_MAX;
        min_X.clear();
        min_Y.clear();
        //from Y to X
        if(side=='x'){
            while(Y.size()!=0){
                double Local_min_ratio_cut = 100000.0;
                for(int i=0;i<Y.size();i++){
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
                            }
                        }
                        if(Sub_Graph_Edges[make_pair(X[j],t)] == 1){
                            count++;
                        }
                        if(Sub_Graph_Edges[make_pair(t,X[j])] == 1){
                            count++;
                        }
                    }
                    for(int k=0;k<Y.size();k++){
                        if(Y[k]!=Y[i]){
                            if(Sub_Graph_Edges[make_pair(Y[i],Y[k])] == 1){
                                count = count+1;
                            }
                            if(Sub_Graph_Edges[make_pair(Y[k],Y[i])] == 1){
                                count = count+1;
                            }
                        }
                    }
                    if(Sub_Graph_Edges[make_pair(Y[i],t)] == 1){
                        count++;
                    }
                    if(Sub_Graph_Edges[make_pair(t,Y[i])] == 1){
                        count++;
                    }
                    ratio_cut = count/((X.size()+1)*(Y.size()));
                    if(ratio_cut<Local_min_ratio_cut){
                        Local_min_ratio_cut = ratio_cut;
                        module_to_move = Y[i];
                    }
                }
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
                if(Global_min_ratio_cut>Local_min_ratio_cut){
                    min_X = X;
                    min_Y = Y;
                    side='y';
                }
            }
        }
        //left to right i.e X to Y
        else{
            while(X.size()!=0){
                double Local_min_ratio_cut = 100000.0;
                for(int i=0;i<X.size();i++){
                    int count = 0;
                    for(int j=0;j<Y.size();j++){
                        for(int k=0;k<X.size();k++){
                            if(X[k]!=X[i]){
                                if(Sub_Graph_Edges[make_pair(X[k],Y[j])] == 1){
                                    count = count+1;
                                }
                                if(Sub_Graph_Edges[make_pair(Y[j],X[k])] == 1){
                                    count = count+1;
                                }
                            }
                        }
                        if(Sub_Graph_Edges[make_pair(Y[j],s)] == 1){
                            count++;
                        }
                        if(Sub_Graph_Edges[make_pair(s,Y[j])] == 1){
                            count++;
                        }
                    }
                    for(int k=0;k<X.size();k++){
                        if(X[k]!=X[i]){
                            if(Sub_Graph_Edges[make_pair(X[i],X[k])] == 1){
                                count = count+1;
                            }
                            if(Sub_Graph_Edges[make_pair(X[k],X[i])] == 1){
                                count = count+1;
                            }
                        }
                    }
                    if(Sub_Graph_Edges[make_pair(X[i],s)] == 1){
                        count++;
                    }
                    if(Sub_Graph_Edges[make_pair(s,X[i])] == 1){
                        count++;
                    }
                    ratio_cut = count/((Y.size()+1)*(X.size()));
                    if(ratio_cut<Local_min_ratio_cut){
                        Local_min_ratio_cut = ratio_cut;
                        module_to_move = X[i];
                    }
                }
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
                if(Global_min_ratio_cut>Local_min_ratio_cut){
                    min_X = X;
                    min_Y = Y;
                    side='x';
                }
            }
        }
        X=min_X;
        Y=min_Y;
    }
    //step:3 i.e group swapping
    if(side=='x'){
        Y.push_back(t);
    }
    else{
        X.push_back(s);
    }
    Global_min_ratio_cut=DBL_MAX;
    unordered_map<int,int>locked;
    int total_size=X.size()+Y.size();
    int locked_count=2;
    double local_gain=DBL_MIN;
    double global_gain=0.1;
    locked[s]=1;
    locked[t]=1;
    //loop till global gain is non-positive
    while(global_gain>0){
        locked.clear();
        locked[s]=1;
        locked[t]=1;
        locked_count=2;
        local_gain=DBL_MAX;
        //as the loop needs to be continued till all the elements are locked, a variable(locked_count) is initialised
        //in order to check which element are locked, an unordered_map is initialised where s and t are always initialised to 1
        // a variable 'side' is maintained to check if the element with largest 'local_gain' is from X or Y
        while(locked_count<total_size){
            //check for elements that are not locked in X
            for(int i=0;i<X.size();i++){
                if(locked[X[i]]==0){ //if not locked
                    double tmp_gain;
                    tmp_gain=calculate_gain(X,Y,X[i],'x');// get the gain
                    if(tmp_gain>local_gain){ // if it is greater than local, change the value of local to tmp and update side
                        local_gain=tmp_gain;
                        global_gain=max(global_gain,local_gain);
                        module_to_move=X[i];
                        side='x';
                    }
                }
            }
            //same for elements in Y
            for(int i=0;i<Y.size();i++){
                if(locked[Y[i]]==0){
                    double tmp_gain;
                    local_gain=calculate_gain(X,Y,Y[i],'y');
                    if(tmp_gain>local_gain){
                        local_gain=tmp_gain;
                        global_gain=max(global_gain,local_gain);
                        module_to_move=X[i];
                        side='y';
                    }
                }
            }
            //swap the module with largest gain here
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
            locked_count++; //increase the count of locked nodes
            locked[module_to_move]=1; //update the map
        }
    }
}

double calculate_gain(vector<int>X,vector<int>Y,int element,char side){
    //calculate ratio_cut1 without shifting the element
    int count=0,ratio_cut1,ratio_cut2;
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
    ratio_cut1 = count/((Y.size())*(X.size()));
    count=0;

    //calculate ratio_cut2. As there are two sets, there will be 2 conditions.
    //In order to transfer from set X to set Y, enter into condition1 else into condition2
    if(side=='x'){
        //calculate number of edges crossing the cut without 'element'
        for(int i=0;i<X.size();i++){
            for(int j=0;j<Y.size();j++){
                if(X[i]!=element){
                    if(Sub_Graph_Edges[make_pair(X[i],Y[j])] == 1){
                        count++;
                    }
                    if(Sub_Graph_Edges[make_pair(Y[j],X[i])] == 1){
                        count++;
                    }
                }
            }
        }
        //now include the 'element' into the count
        for(int i=0;i<X.size();i++){
            if(Sub_Graph_Edges[make_pair(X[i],element)] == 1){
                count++;
            }
            if(Sub_Graph_Edges[make_pair(element,X[i])] == 1){
                count++;
            }
        }
        ratio_cut2 = count/((Y.size()+1)*(X.size()-1));
    }
    else{
        for(int i=0;i<Y.size();i++){
            for(int j=0;j<X.size();j++){
                if(Y[i]!=element){
                    if(Sub_Graph_Edges[make_pair(Y[i],X[j])] == 1){
                        count++;
                    }
                    if(Sub_Graph_Edges[make_pair(X[j],Y[i])] == 1){
                        count++;
                    }
                }
            }
        }
        for(int i=0;i<Y.size();i++){
            if(Sub_Graph_Edges[make_pair(Y[i],element)] == 1){
                count++;
            }
            if(Sub_Graph_Edges[make_pair(element,Y[i])] == 1){
                count++;
            }
        }
        ratio_cut2 = count/((Y.size()-1)*(X.size()+1));
    }
    return ratio_cut1-ratio_cut2;
}



//main functions
void cluster_nodes_into_pages(vector<int>Nodes,unordered_map<int,int>Nodes_present,int Page_Size){
	vector<int>F;
	F = Nodes;
	while(F.size()!=0){
		//choose a V' from F
		vector<int>V_dash;
		V_dash = BFS(F,Nodes_present);
		vector<int>A;
		A = Two_Way_Partitioning(V_dash,Page_Size);
	}
}


int main(){
	//initialise and define page and secondary memory and primary memory
	//read the data from the file
	string pageno1,pageno2;
	int number = 1;
	pageno1 = "page" + to_string(number) + ".txt";
	ifstream infile;
	ofstream outfile1;
	ofstream outfile2;
	infile.open("dataset_btp.txt");
	int count=0;
	unordered_map<int,vector<int>>U_successor;
	unordered_map<int,vector<int>>U_predecessor;
	unordered_map<int,int>U_Nodes;
	vector<int>Nodes;
	if(infile.is_open()){
		while(infile){
			string s;
		    if (!getline( infile, s )) break;
		    istringstream ss( s );
		    vector <string> record;
		    while (ss)
		    {
		      string s;
		      if (!getline( ss, s, ',' )) break;
		      record.push_back( s );
		    }

			if(count == 0){
				U_successor[string_to_int(record[1])].push_back(string_to_int(record[2]));
				U_predecessor[string_to_int(record[2])].push_back(string_to_int(record[1]));
				if(U_Nodes[string_to_int(record[1])] == 0){
					U_Nodes[string_to_int(record[1])] = 1;
					Nodes.push_back(string_to_int(record[1]));
				}

				if(U_Nodes[string_to_int(record[2])] == 0){
					U_Nodes[string_to_int(record[2])] = 1;
					Nodes.push_back(string_to_int(record[2]));
				}

			}
			count++;
			if(count == 10080){
				count = 0;
			}
		}
	}
	else{
		cout<<"File not opened"<<endl;
	}

	infile.close();
	cout<<Nodes.size()<<endl;
	count = 0;
	int p_no = 1;
	for(int i=0;i<Nodes.size();i++){
		Bplus[Nodes[i]] = p_no;
		count++;
		if(count == 12){
			count = 0;
			p_no++;
		}
	}
	
	count = 0;
	infile.open("/home/sainath/Desktop/dataset_btp.txt");
	if(infile.is_open()){
		while(infile){
			string s;
		    if (!getline( infile, s )) break;
		    istringstream ss( s );
		    vector <string> record;
		    while (ss)
		    {
		      string s;
		      if (!getline( ss, s, ',' )) break;
		      record.push_back( s );
		    }

			if(count == 0){
				pageno1 = "page_"+to_string(Bplus[string_to_int(record[1])])+".txt";
				outfile1.open(pageno1,ios_base::app);
				outfile1<<"\ns "<<record[1]<<" "<<record[2]<<" "<<record[3]<<" ";
			}
			else{
				outfile1<<record[3]<<" ";
			}
			count++;
			if(count == 10080){
				count = 0;
				outfile1.close();
			}
		}
	}

	infile.close();

	infile.open("/home/sainath/Desktop/dataset_btp.txt");
	if(infile.is_open()){
		while(infile){
			string s;
		    if (!getline( infile, s )) break;
		    istringstream ss( s );
		    vector <string> record;
		    while (ss)
		    {
		      string s;
		      if (!getline( ss, s, ',' )) break;
		      record.push_back( s );
		    }

			if(count == 0){
				pageno2 = "page_"+to_string(Bplus[string_to_int(record[2])])+".txt";
				outfile2.open(pageno2,ios_base::app);
				outfile2<<"\np "<<record[2]<<" "<<record[1]<<" "<<record[3]<<" ";
			}
			else{
				outfile2<<record[3]<<" ";
			}
			count++;
			if(count == 10080){
				count = 0;
				outfile2.close();
			}
		}
	}




	//Process and arrange the data 
	//define the record and page 
	//create the graph(static create) and arrange the nodes into the pages
	unordered_map<int,int>Nodes_present;
	for(int i=0;i<Nodes.size();i++){
		Nodes_present[Nodes[i]] = 1;
	}
	cluster_nodes_into_pages(Nodes,Nodes_present,Min_PageSize);




	//Now the quering starts

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

		}
		else if(flag1 == 3){

		}
		else if(flag1 == 4){

		}
		else if(flag1 == 5){

		}
		else{
			return 0;
		}
	}

return 0;
}