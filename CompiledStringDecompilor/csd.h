#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>

using namespace std;

class Var {
	friend class CSD;
private:
	char *name;
	bool isEssen;
	bool isStrDep;	//str dependence
	int def;		// default val
	int *equal;
	int equalSize;	// total Size
	int equalNum;	// How many elements
	int *nEqual;
	int nEqualSize;	// Not Equal total
	int nEqualNum;	// Not Equal number of
	int lessthen;
	int biggerthen;

public:
	Var(char *name);
	~Var();
	char *getName(void);
	void setEssen(bool isEssen, int def);
	bool getEssen(void);
	void setStrDep(bool isStrDep);
	bool getStrDep(void);
	int getDef(void);
	void setLessthen(int lessthen);
	void setBiggerthen(int biggerthen);
	void addEqual(int num); // add num to equalList
	void addNequal(int num); // add num to NequalList
	bool isVaild(int num); // Is num can get in this variable?
};

class CSD {
private:
	int *format;	// Format string (in text, not binary)
	int formatSize; // Format string length
	int formatNum;	// number of elements
	Var **vArr;		// Variable array
	int vSize;		// total size
	int vNum;		// number of elements
	int idSize, strSize;	//for de/compile

	Var *getVar(char *name);
	int addVar(char *name);
	void addFormat(int num);
	void sortVar(void);

public:
	CSD();
	~CSD();
	bool loadSds(char *fileName);
	bool decompile(char * src, char *dst);
	bool compile(char * src, char *dst);
	void CSD::showSds();
};

bool isHex(int ch);
int toHex(int ch);