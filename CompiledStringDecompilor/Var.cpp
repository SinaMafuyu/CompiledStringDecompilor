#include "csd.h"

Var::Var(char *name) {
	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);
	equalSize = 0;
	equalNum = 0;
	isEssen = true;
	def = 0;
	lessthen = 0xff;
	biggerthen = 0;
	equal = NULL;
	isStrDep = false;
	isSetted = false;
}
Var::~Var() {
	delete name;
	if (equal != NULL)
		delete equal;
}

char * Var::getName(void)
{
	return name;
}

void Var::setEssen(bool isEssen, int def) {
	this->isEssen = isEssen;
	this->def = def;
}

bool Var::getEssen(void) {
	return this->isEssen;
}

bool Var::getSet(void) {
	return this->isSetted;
}

void Var::setStrDep(bool isStrDep) {
	this->isStrDep = isStrDep;
}

bool Var::getStrDep(void) {
	return this->isStrDep;
}

int Var::getDef(void) {
	return def;
}

void Var::setLessthen(int lessthen) {
	this->lessthen = lessthen;
	isSetted = true;
}

void Var::setBiggerthen(int biggerthen) {
	this->biggerthen = biggerthen;
	isSetted = true;
}

void Var::addEqual(int num) // add num to equalList
{
	if (equalSize == 0)
	{
		equal = new int[10];
		equalSize = 10;
	}
	else if (equalSize == equalNum)
	{
		int *tmp = equal;
		equalSize += 10;
		equal = new int[equalSize];
		memcpy(equal, tmp, sizeof(int) * equalNum);
		delete tmp;
	}
	equal[equalNum] = num;
	++equalNum;
	isSetted = true;
}

void Var::addNequal(int num) // add num to NequalList
{
	if (nEqualSize == 0)
	{
		nEqual = new int[10];
		nEqualSize = 10;
	}
	else if (nEqualSize == nEqualNum)
	{
		int *tmp = nEqual;
		nEqualSize += 10;
		nEqual = new int[nEqualSize];
		memcpy(nEqual, tmp, sizeof(int) * nEqualNum);
		delete tmp;
	}
	nEqual[nEqualNum] = num;
	++nEqualNum;
	isSetted = true;
}

bool Var::isVaild(int num) // Is num can get in this variable?
{
	if (isEssen == false && num == def)
		return false;
	else if (num > lessthen)
		return false;
	else if (num < biggerthen)
		return false;
	else
	{
		for (int i = 0; i < nEqualNum; i++)
		{
			if (num == nEqual[i])
				return false;
		}
		for (int i = 0; i < equalNum; i++)
		{
			if (num == equal[i])
				return true;
		}
		if (equalNum > 0)
			return false;
		else
			return true;
	}
}