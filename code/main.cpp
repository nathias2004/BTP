#include<iostream>
#include<bits/stdc++.h>
using namespace std;













//main functions
void cluster_nodes_into_pages(){

}



int Two_Way_Prtitioning(){
	
}










int main(){
	//initialise and define page and secondary memory and primary memory
	//read the data from the file
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
