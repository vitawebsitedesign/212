#ifndef __LL__
#define __LL__

std::string nextSeg(std::istringstream&);

class User{
	private:
		std::string name;
		int numFiles;
		std::string oldest;
		std::string latest;
		float totSize;
	public:	
		User(){};
		User(std::string);
		User* next;
		
		std::string getName();
		int getNumFiles();
		std::string getOldest(std::string);
		std::string getLatest(std::string);
		float getTotSize();
		void printLine(std::string,int,std::string,std::string,std::string);
		
		void incNumFiles();
		bool compareOldestLatest(std::string,std::string);
		void setOldestLatest(std::string);
		void setTotSize(float);
}; typedef User* UPtr;

class LinkedList{
	public:
		LinkedList(){root=NULL;}
		UPtr root;
		
		bool usrExists(std::string);
		void addDetails(std::string,std::istringstream&);
		void addUsrInOrd(UPtr);
		void addUsr(std::string);
		
		UPtr trav(std::string,std::string);
		UPtr toEnd();
		void set2ndLastNull(UPtr&);
		bool del();
}; typedef LinkedList* LLPtr;

#endif
