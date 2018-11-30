#include<iostream>
#include<bits/stdc++.h>
#include<boost/functional/hash.hpp>
using namespace std;
int Min_PageSize;
int No_Records_Page = 28;
unordered_map<int,int>Bplus;
unordered_map<pair<int,int>,int,boost::hash<pair<int,int>>>Sub_Graph_Edges;





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
	//Initialising
	int s = V_dash[0];
	int t = find_t(V_dash);
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
					}
				}
				if(Sub_Graph_Edges[make_pair(X[j],t)] == 1){
					count++;
				}
			}
			for(int k=0;k<Y.size();k++){
					if(Y[k]!=Y[i]){
						if(Sub_Graph_Edges[make_pair(Y[i],Y[k])] == 1){
							count = count+1;
						}
					}
			}
			if(Sub_Graph_Edges[make_pair(Y[i],t)] == 1){
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
		}
	}
	//left to right i.e X o Y
	vector<int>X_dummy;
	X_dummy = X;
	X = Y;
	Y = X_dummy;

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
					}
				}
				if(Sub_Graph_Edges[make_pair(X[j],t)] == 1){
					count++;
				}
			}
			for(int k=0;k<Y.size();k++){
					if(Y[k]!=Y[i]){
						if(Sub_Graph_Edges[make_pair(Y[i],Y[k])] == 1){
							count = count+1;
						}
					}
			}
			if(Sub_Graph_Edges[make_pair(Y[i],t)] == 1){
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
		}
	}



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
