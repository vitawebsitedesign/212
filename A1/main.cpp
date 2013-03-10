#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include "linkedlist.h"
using namespace std;

bool fileLine(string line){
	string perm;

	istringstream is(line);
	getline(is,perm,' ');

	if(perm.size() == 10 && perm[0] != 'd') return true;
	return false;
}

void read(LLPtr& list){
	string line, name;
	while(getline(cin,line)){
		if(fileLine(line)){
			//get name (ignore permissions and num of links)
			istringstream is(line);
			name = nextSeg(is); name = nextSeg(is); name = nextSeg(is);
			
			if (!list->usrExists(name)) list->addUsr(name);
			list->addDetails(name, is);
		}
	}
}

int main(){
	LLPtr list = new LinkedList;
	read(list);	
	
	list->trav("print","");	
	list->del();
}
