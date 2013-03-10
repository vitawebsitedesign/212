//test on notebook w/ multiple users
//re-read specs + chk with tutor + submit
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include "linkedlist.h"
using namespace std;
/**
 * A line (eg: -rwx------ 1 mn626 csugc 43180 Mar  9 18:14 a.out) contains segments, seperated by spaces. This function returns each segment
 * @arg is Input stream to read the next segment from 
 * @return string
 */
string nextSeg(istringstream& is){
	string seg;
	while(getline(is,seg,' ')){
		if(seg.size() != 0) return seg;
	}
}
/**
 * Returns Date + Timestamp (eg: Mar8 19:46)
 * @return string
 */
string getDateTimestamp(istringstream& is, string seg){
	seg += " "; seg += nextSeg(is); seg+= " "; seg += nextSeg(is);
	return seg;
}
/**
 * Extracts the month, day or time from the full "DateTimestamp". Used when comparing a lines "dateTimestamp" with "oldest/latest"
 * @return string
 */
string getDateTimeSeg(istringstream& is, string flag){
	string tmp, seg;
	if(flag != "time"){	
		getline(is,tmp,' ');	//gets "Mar8"
		for(int i=0; i<tmp.size(); i++){
			if(flag == "month" && !isdigit(tmp[i])) seg += tmp[i];
			else if(flag == "day" && isdigit(tmp[i])) seg += tmp[i];
		}
	} else{
		//returns timestamp without colon
		string timeLeft,timeRight;

		getline(is,timeLeft,':');
		getline(is,timeRight);
		seg = timeLeft + timeRight;	
	}
	return seg;
}
/**
 * Converts a month string to an int (eg: "Feb" = 1). This allows month comparisons
 * @arg month
 * @return Int
 */
int monthToInt(string mon){
	string monArr[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	for(int i=0; i<12; i++){
		if(mon == monArr[i]) return i;
	}
}
/**
 * Returns total size with units (eg: 124KB)
 * @return string
 */
string getTotSizeUnits(float totSize){	//CHKPT: not ret anythin
	string totSizeUnits;
	string unitArr[] = {"B","KB","MB","GB"};
	
	for(int i=1; i<5; i++){
		if((totSize/pow((1024.0),i)) < 1){
			if(i==1) i==2;	//if totSize is Bytes, then pow=1 (for calculation below)
			double calcSize = (totSize/pow((1024.0),i-1));
			
			ostringstream os;
			os << calcSize;
			totSizeUnits = os.str();
			
			totSizeUnits.resize(5,'+');
			totSizeUnits += unitArr[i-1];
			return totSizeUnits;
		}
	}
}

User::User(string inName){
	next = NULL;
	name = inName;
	numFiles = 0;
	totSize = 0;
}

string User::getName(){
	return name;
}

int User::getNumFiles(){
	return numFiles;
}

string User::getOldest(string seg){
	return oldest;
}

string User::getLatest(string seg){
	return latest;
}

float User::getTotSize(){
	return totSize;
}

void User::printLine(string inName, int inNumFiles, string inOldest, string inLatest, string inTotSize){
	printf("%-10s%-10i%-20s%-20s%-20s\n",
		inName.c_str(),
		inNumFiles,
		inOldest.c_str(),
		inLatest.c_str(),
		inTotSize.c_str()
	);
}

void User::incNumFiles(){
	numFiles++;
}
/**
 * if param is older than oldest, return true
 * also converts month strings into ints to allow comparing
 * @return bool
 */
bool User::compareOldestLatest(string flag, string dateTimestamp){
		//get months, days and times from the "dateTimestamp" argument. ints allow comparison
		istringstream isLine(dateTimestamp);
		int monLine = monthToInt(getDateTimeSeg(isLine, "month"));
		int dayLine = atoi(getDateTimeSeg(isLine, "day").c_str());
		int timeLine = atoi(getDateTimeSeg(isLine, "time").c_str());

		//get months, days and times from users "oldest" and "latest" fields. ints allow comparison
		if(flag == "oldest"){
			istringstream isOldest(oldest);
			int monOldest = monthToInt(getDateTimeSeg(isOldest, "month"));
			int dayOldest = atoi(getDateTimeSeg(isOldest, "day").c_str());
			int timeOldest = atoi(getDateTimeSeg(isOldest, "time").c_str());
			
			//compare
			//cout<<"Comparing: "<<monLine<<" "<<dayLine<<" "<<timeLine<<"  <  "<<monOldest<<" "<<dayOldest<<" "<<timeOldest<<endl;	//dm
			//cout<<"Comparing: ["<<dayLine<<"]  <  ["<<dayOldest<<"]"<<endl;	//dm
			if(monLine < monOldest){
				return true;
			} else if(monLine == monOldest && dayLine < dayOldest){	
				return true;
			} else if(dayLine == dayOldest && timeLine < timeOldest){
				return true;
			}
		} else if (flag == "latest"){
			istringstream isLatest(latest);
			int monLatest = monthToInt(getDateTimeSeg(isLatest, "month"));
			int dayLatest = atoi(getDateTimeSeg(isLatest, "day").c_str());
			int timeLatest = atoi(getDateTimeSeg(isLatest, "time").c_str());
			
			//compare
			if(monLine > monLatest || monLine == monLatest && dayLine > dayLatest || dayLine == dayLatest && timeLine > timeLatest) return true;
		}
		return false;
}
/**
 * Uses a compare function to set oldest/latest. Compare priority is "month > day > time"
 * @arg dateTimestamp of the current line (eg: Mar 8 20:45)
 */
void User::setOldestLatest(string dateTimestamp){
	if(!oldest.size()){			//initial set
		oldest = dateTimestamp; 
		latest = dateTimestamp;
	} else{
		if (compareOldestLatest("oldest",dateTimestamp)) oldest = dateTimestamp;
		if (compareOldestLatest("latest",dateTimestamp)) latest = dateTimestamp;
	}
}

void User::setTotSize(float size){
	totSize += size;
}

bool LinkedList::usrExists(string inName){
	if(!root) return false;
	
	UPtr cur = root;
	while(cur != NULL){
		if(cur->getName() == inName) return true;
		cur = cur->next;
	}
	return false;
}
/**
 * Adds details for the specified user (eg: # of files, total size)
 * @arg name Users name
 * @arg is Input stream, filled with the users details
 */
void LinkedList::addDetails(string name, istringstream& is){
	if (UPtr usr = trav("search",name)){
		usr->incNumFiles();	//numFiles
			
		string seg;
		seg = nextSeg(is);	//ignore owner group
		for(int i=0; i<3; i++){
			seg = nextSeg(is);
			switch(i){
				case 0:
					usr->setTotSize(atof(seg.c_str()));
					break;
				case 1:
					usr->setOldestLatest(getDateTimestamp(is,seg));
					break;
				case 2:
					break;	//ignore filename
			}
		}
	}
}
/**
 * Adds a user node in ascending order (eg: "Angel", "Bee", "Carlos")
 * @arg tar The user node to add
 */
void LinkedList::addUsrInOrd(UPtr tar){
	UPtr p1 = root;
	UPtr* p2 = &root;
	while(p1 != NULL && p1->getName() < tar->getName()){
		p2 = &p1->next;
		p1 = p1->next;
	}
	*p2 = tar;
	tar->next = p1;
}

void LinkedList::addUsr(string name){
	UPtr tar = new User(name);
	if(!root){
		root = tar;
	} else{
		addUsrInOrd(tar);
	}
}
/**
 * Traverses the list. Has 2 uses, see "flag" argument below
 * @arg flag "search" or "print". Search returns a pointer to a specified node. Print simply prints each users details
 * @arg inName
 * @return When searching for a node with a specific name, returns a pointer to it. otherwise, returns an unused NULL
 */
UPtr LinkedList::trav(string flag, string inName){
	UPtr cur = root;
	
	if(flag == "search"){
		while(cur!=NULL){
			if(cur->getName() == inName) return cur;
			cur = cur->next;
		}
	} else if (flag == "print"){
		int sumNumFiles = 0;
		float sumTotSize;
		string sumOldest, sumLatest;
	
		printf("%-10s%-10s%-20s%-20s%-20s\n\n","User","Files","Oldest","Latest","TotSize");	//header
		while(cur!=NULL){
			cur->printLine(
				cur->getName(), 
				cur->getNumFiles(), 
				cur->getOldest("dateTimestamp"), 
				cur->getLatest("dateTimestamp"), 
				getTotSizeUnits(cur->getTotSize())
			);
			
			//set vars for summary line
			sumNumFiles += cur->getNumFiles();
			sumTotSize += cur->getTotSize();
			//oldest/latest: initial set
			if(!sumOldest.size()){
				sumOldest = cur->getOldest("dateTimestamp");
				sumLatest = cur->getLatest("dateTimestamp");
			}
			//oldest/latest: compare and set
			if (cur->compareOldestLatest("oldest",sumOldest)) sumOldest = cur->getOldest("dateTimestamp");
			if (cur->compareOldestLatest("latest",sumLatest)) sumLatest = cur->getLatest("dateTimestamp");
			
			cur = cur->next;
		}
		
		//Summary line (eg: ALL 623 Jul 3 1970 Jan 2 19:51 1.018GB)
		
		cur->printLine(
			"ALL", 
			sumNumFiles, 
			sumOldest, 
			sumLatest, 
			getTotSizeUnits(sumTotSize)
		);
	}
	return NULL;
}
/**
 * Returns a pointer to the last node in the list
 * @return UPtr
 */
UPtr LinkedList::toEnd(){
	UPtr cur = root;
	if(!root) return NULL;
	while(cur->next != NULL) cur = cur->next;
	return cur;
}
/**
 * Sets the 2nd last node in the linked list to NULL.
 * If only 1 node, it sets "root" to NULL
 * This functionality is needed because after deleting the last node, the program needs to know where end of the list is
 * @arg last The last node
 */
void LinkedList::set2ndLastNull(UPtr& last){
	UPtr cur = root;
	if(root->next != NULL){
		while(cur->next != last) cur = cur->next;
	} else{
		root = NULL;
	}
	cur->next = NULL;
}
/**
 * Deletes dynamic memory
 */
bool LinkedList::del(){
	while(UPtr last = toEnd()){
		if(!last) return false;
		set2ndLastNull(last);
		delete last;
	}
}