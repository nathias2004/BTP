#include<iostream>
#include<bits/stdc++.h>
using namespace std;
int Min_PageSize;












//main functions
void cluster_nodes_into_pages(){

}



int Two_Way_Prtitioning(int Min_PageSize){

}



int string_to_int(string s){
	stringstream geek(s);
	int x = 0;
	geek>>x;
	return x;
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
	unordered_map<int,int>Bplus;
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
	infile.open("dataset_btp.txt");
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
				pageno2 = "page_"+to_string(Bplus[string_to_int(record[2])])+".txt";
				outfile1.open(pageno1,ios_base::app);
				outfile2.open(pageno2,ios_base::app);
				outfile1<<"\ns "<<record[1]<<" "<<record[2]<<" "<<record[3]<<" ";
				outfile2<<"\np "<<record[2]<<" "<<record[1]<<" "<<record[3]<<" ";
			}
			else{
				outfile1<<record[3]<<" ";
				outfile2<<record[3]<<" ";
			}
			count++;
			if(count == 10080){
				outfile1.close();
				outfile2.close();
				count = 0;
			}
		}
	}




	//Process and arrange the data 
	//define the record and page 
	//create the graph(static create) and arrange the nodes into the pages

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
