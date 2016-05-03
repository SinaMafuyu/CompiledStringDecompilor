#include "csd.h"

int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		CSD csd;
		csd.loadSds(argv[1]);
		csd.showSds();
	}
	else if (argc == 5 && !strcmp(argv[1],"decompile"))
	{

	}
	else if (argc == 5 && !strcmp(argv[1], "compile"))
	{
		CSD csd;
		if (csd.loadSds(argv[2]))
			csd.compile(argv[4], argv[3]);
	}
	else
	{
		cout << "Usage: csd [mode] \".sds\" \".dat\" \".txt\"" << endl;
		cout << "\tmode:\tcompile \t- .txt to dat" << endl;
		cout << "\t\tdecompile \t- .dat to .txt" << endl;
	}
	return 0;
}


bool isHex(int ch)
{
	if (ch >= '0' && ch <= '9')
		return true;
	else if (ch >= 'A' && ch <= 'F')
		return true;
	else if (ch >= 'a' && ch <= 'f')
		return true;
	else
		return false;
}

int toHex(int ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	else if (ch >= 'a' && ch <= 'f')
	{
		return ch - 'a' + 10;
	}
	else if (ch >= 'A' && ch <= 'F')
	{
		return ch - 'A' + 10;
	}
	else
		return -1;
}

int Hextoint(int hex)
{

}