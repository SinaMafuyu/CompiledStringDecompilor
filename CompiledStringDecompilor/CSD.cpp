#include "csd.h"

bool CSD::isNextLine(FILE *fp)
{
	int t = 0, i,j=0;
	int tmp;
	int vn;

	t=fgetc(fp);
	if (feof(fp) || t==EOF){
		return true;
	}
	else{
		fseek(fp, -1, SEEK_CUR);
	}

	for (i = 0; i < 4 && i < formatNum && !feof(fp); i++)
	{
		if ((format[i] & 0xff00) == 0){
			t = fgetc(fp);
			if (t != format[i]){
				fseek(fp, -1 * i + j - 1, SEEK_CUR);
				return false;
			}
			else{
				//cout << loop << endl;
				if (loop != 0 && i == loop)
				{
					fseek(fp, -1, SEEK_CUR);
					if (isNextLine(fp)){
						fseek(fp, -1 * i + j, SEEK_CUR);
						return false;
					}
					else{
						fseek(fp, 1, SEEK_CUR);
					}
				}
				continue;
			}
		}
		else if ((format[i] & 0xff00) == 0x0100)			// Var
		{
			vn = format[i] & 0xff;
			if (vArr[vn]->getEssen())
			{
				t = fgetc(fp);
				if (vArr[vn]->isVaild(t)){
					continue;
				}
				else{
					fseek(fp, -1 * i + j - 1, SEEK_CUR);
					return false;
				}
			}
			else
			{
				if (i + 1 == formatNum){
					fseek(fp, -1 * i + j, SEEK_CUR);
					return true;
				}

				tmp = fgetc(fp);

				if ((format[i + 1] & 0xff00) == 0 && tmp == (format[i + 1] & 0xff))
				{
					fseek(fp, -1, SEEK_CUR);
					++j;
					continue;
				}
				else if ((format[i + 1] & 0xff00) == 0x0200 && tmp == (format[i + 1] & 0xff))	// Next is SD-Hex
				{
					fseek(fp, -1, SEEK_CUR);
					++j;
					continue;
				}
				else if (vArr[vn]->isVaild(tmp)) {
					continue;
				}
				else
				{
					fseek(fp, -1, SEEK_CUR);
					++j;
					continue;
				}
			}

		}
		else if ((format[i] & 0xff00) == 0x0200)
		{
			t = fgetc(fp);
			if (t == (format[i] & 0xff)) {
				continue;
			}
			else {
				fseek(fp, -1, SEEK_CUR);
				++j;
				continue;
			}
		}
		else
		{
			fseek(fp, -1 * i + j, SEEK_CUR);
			return true;
		}
	}
	fseek(fp, -1 * i + j, SEEK_CUR);
	return true;
}

int CSD::nextEssen(int i)
{
	int vn;
	for (int j = i+1; j < formatNum; j++)
	{
		if ((format[j] & 0xff00) == 0x0100)			// Var
		{
			vn = (format[j] & 0xff);
			if (vArr[vn]->getEssen() && vArr[vn]->getStrDep() == false)
				return format[j];
			else if (!(vArr[vn]->getEssen()) && vArr[vn]->getStrDep() == false)
				return format[j];

		}
		else if ((format[j] & 0xff00) == 0x0400)	// ID
		{
			continue;
		}
		else if ((format[j] & 0xff00) == 0x0800)	// ST
		{
			continue;
		}
		else if ((format[j] & 0xff00) == 0x0200)	// String Dependence Hex
		{
			continue;
		}
		else										// Hex
		{
			return format[j];
		}
	}
	return -1;
}

int CSD::nextEssenI(int i)
{
	int vn;
	for (int j = i + 1; j < formatNum; j++)
	{
		if ((format[j] & 0xff00) == 0x0100)			// Var
		{
			vn = (format[j] & 0xff);
			if (vArr[vn]->getEssen() && vArr[vn]->getStrDep() == false)
				return j;
			else if (!(vArr[vn]->getEssen()) && vArr[vn]->getStrDep() == false)
				return j;

		}
		else if ((format[j] & 0xff00) == 0x0400)	// ID
		{
			continue;
		}
		else if ((format[j] & 0xff00) == 0x0800)	// ST
		{
			continue;
		}
		else if ((format[j] & 0xff00) == 0x0200)	// String Dependence Hex
		{
			continue;
		}
		else										// Hex
		{
			return j;
		}
	}
	return -1;
}

int CSD::nextHex(int i)
{
	for (int j = i + 1; j < formatNum; j++)
	{
		if ((format[j] & 0xff00) == 0x0100)			// Var
		{
			continue;
		}
		else if ((format[j] & 0xff00) == 0x0400)	// ID
		{
			return -1;
		}
		else if ((format[j] & 0xff00) == 0x0800)	// ST
		{
			return -1;
		}
		else if ((format[j] & 0xff00) == 0x0200)	// String Dependence Hex
		{
			continue;
		}
		else										// Hex
		{
			return format[j];
		}
	}
	return -1;
}

int CSD::nextHexI(int i)
{
	for (int j = i + 1; j < formatNum; j++)
	{
		if ((format[j] & 0xff00) == 0x0100)			// Var
		{
			continue;
		}
		else if ((format[j] & 0xff00) == 0x0400)	// ID
		{
			return -1;
		}
		else if ((format[j] & 0xff00) == 0x0800)	// ST
		{
			return -1;
		}
		else if ((format[j] & 0xff00) == 0x0200)	// String Dependence Hex
		{
			continue;
		}
		else										// Hex
		{
			return j;
		}
	}
	return -1;
}

void CSD::sortVar(void)
{
	Var *tmp;
	for (int i = 0; i < vNum; i++)
	{
		if (vArr[i]->getEssen())
			continue;
		else
		{
			for (int j = i+1; j < vNum; j++)
			{
				if (vArr[j]->getEssen())
				{
					tmp=vArr[i];
					vArr[i] = vArr[j];
					vArr[j] = tmp;

					for(int k=0; k<formatNum; k++)
					{
						if (format[k] == (i | 0x0100))
						{
							format[k] = (j | 0x0100);
						}
						else if (format[k] == (j | 0x0100))
						{
							format[k] = (i | 0x0100);
						}
					}

					break;
				}
				if (j + 1 == vNum)
				{
					return;
				}
			}
		}
	}
}

int CSD::getVar(const char *name)
{
	char buf[100];
	for (int i = 0; i < vNum; i++)
	{
		vArr[i]->getName(buf);
		if (!strcmp(buf, name))
		{
			return i;
		}
	}
	return -1;
}

int CSD::addVar(const char *name)
{
	Var **tmp;
	if (vSize == 0)
	{
		vArr = new Var*[10];
		vSize = 10;
	}
	else if (vSize == vNum)
	{
		tmp = vArr;
		vSize += 10;
		vArr = new Var*[vSize];
		memcpy(vArr, tmp, sizeof(Var*) * vNum);
		delete tmp;
	}
	vArr[vNum] = new Var(name);
	++vNum;
	return vNum - 1;
}

void CSD::addFormat(int num)
{
	int* tmp;
	if (formatSize == 0)
	{
		format = new int[50];
		formatSize = 50;
	}
	else if (formatSize == formatNum)
	{
		tmp = format;
		formatSize += 50;
		format = new int[formatSize];
		memcpy(format, tmp, sizeof(int) * formatNum);
		delete tmp;
	}
	format[formatNum] = num;
	++formatNum;
}

CSD::CSD() {
	format = NULL;
	formatSize = 0;
	vArr = NULL;
	vSize = 0;
	vNum = 0;
	loop = 0;
	lastEssen = 0x8fff;
};

CSD::~CSD() {
	if (format != NULL)
		delete format;
	if (vArr != NULL)
		delete vArr;
};

bool CSD::loadSds(const char *fileName)
{
	FILE *fp = fopen(fileName, "r");
	char buf[100];
	int a, b;
	char var[30];
	int v;
	int tmp;

	if (!fp) {
		cout << "No .sds File Found!" << endl;
		return false;
	}

	try {
		for (; !feof(fp);)
		{
			if (fgetc(fp) == '/')
				if (fgetc(fp) == '/')
				{
					while (fgetc(fp) != 0x0a);
					continue;
				}
				else
					throw "Invaild expression: /";
			fseek(fp, -1, SEEK_CUR);
			break;
		}
		fgets(buf, 9, fp);
		if (strcmp(buf, "sds:v1.1"))
			throw "Invaild sds version, curr=1.1";

		while (fgetc(fp) != 0x0a);

		for (; !feof(fp);) {
			if (fgetc(fp) == '/')
				if (fgetc(fp) == '/')
				{
					while (fgetc(fp) != 0x0a);
					continue;
				}
				else
					fseek(fp, -1, SEEK_CUR);
			fseek(fp, -1, SEEK_CUR);
			break;
		}

		if (fscanf(fp, "%d %d %d %d", &idNum, &idSize, &strNum, &strSize) != 4)
			throw "No id, strSize in .txt";
		if (idNum<0 || strNum<0 || idSize < 1 || strSize < 1)
			throw "idSize, strSize must be more than 0";

		if(fgetc(fp) != 0x0a)
			throw "No id, strSize in .txt";


		for (; !feof(fp);)
		{
			a = fgetc(fp);

			if (a == '#')
			{
				fscanf(fp, "%[^\n ]", var);
				addFormat(addVar(var) | 0x0100);
				a = fgetc(fp);
				if (a == 0x0a)
					break;
				else if (a == ' ')
					continue;
				else if (feof(fp))
					break;
				else
					throw "Unknwon Error at adding Var";
			}
			b = fgetc(fp);
			if (a == 'I')
				if (b == 'D')
				{
					addFormat(0x0400);
					a = fgetc(fp);
					if (a == 0x0a)
						break;
					else if (a == ' ')
						continue;
					else if (feof(fp))
						break;
					else
						throw "Unknwon Error at adding ID";
				}
			if (a == 'S')
				if (b == 'T')
				{
					addFormat(0x0800);
					a = fgetc(fp);
					if (a == 0x0a)
						break;
					else if (a == ' ')
						continue;
					else if (feof(fp))
						break;
					else
						throw "Unknwon Error at adding ST";
				}

			if (a == '$')
			{
				a = fgetc(fp);
				if (isHex(a))
				{
					if (!isHex(b))
					{
						throw "Invaild Hex Data";
					}
					else
					{
						addFormat(toHex(b) << 4 | toHex(a) | 0x0200);
						a = fgetc(fp);
						if (a == 0x0a)
							break;
						else if (a == ' ')
							continue;
						else if (feof(fp))
							break;
						else
							throw "Unknwon Error at adding Hex with str dependence";
					}
				}
				else
					throw "Unknwon Error at adding Hex with str dependence";
			}

			if (isHex(a))
				if (!isHex(b))
				{
					throw "Invaild Hex Data";
				}
				else
				{
					addFormat(toHex(a) << 4 | toHex(b));
					a = fgetc(fp);
					if (a == 0x0a)
						break;
					else if (a == ' ')
						continue;
					else if (feof(fp))
						break;
					else
						throw "Unknwon Error at adding Hex";
				}
			throw "Invaild Expression";
		}

		for (; !feof(fp) && fgetc(fp) != '#';);

		for (;!feof(fp);)
		{
			fscanf(fp, "%[^ ]", var);
			if (fgetc(fp) != ' ')
				throw "Invaild sds var option";
			a = fgetc(fp);
			v = getVar(var);
			if (v == -1)
				throw "No var name in format";


			if (a == '~')
			{
				fscanf(fp, "%x", &tmp);
				vArr[v]->setEssen(false, tmp);
			}
			else if (a == '<')
			{
				a = fgetc(fp);

				fscanf(fp, "%x", &tmp);
				if (a != '=')
					--tmp;
				vArr[v]->setLessthen(tmp);
			}
			else if (a == '>')
			{
				a = fgetc(fp);
				fscanf(fp, "%x", &tmp);
				if (a != '=')
					++tmp;
				vArr[v]->setBiggerthen(tmp);
			}
			else if (a == '=')
			{
				fscanf(fp, "%x", &tmp);
				vArr[v]->addEqual(tmp);
			}
			else if (a == '$')
			{
				vArr[v]->setStrDep(true);
			}
			else if (a == '!' && fgetc(fp) == '=')
			{
				fscanf(fp, "%x", &tmp);
				vArr[v]->addNequal(tmp);
			}
			else
				throw "invaild setAtt option";
			for (; !feof(fp) && fgetc(fp) != '#';);
		}

	}
	catch (char *s)
	{
		cout << "[ERR] In loadSds() : " << s << endl;
		formatNum = 0;
		return false;
	}
	catch (...) {}

	fclose(fp);
	sortVar();
	for (int i = 0; i < 4; i++)
	{
		if ((format[i] & 0xff00) == 0)
		{
			for (int j = i + 1; j < 4; j++)
			{
				if (format[i] == format[j])
				{
					loop = j;
					i = 4;
					break;
				}
			}
			break;
		}
	}
	for (int i = 0; i < formatNum; i++)
	{
		if ((format[i] & 0xff00) == 0)
			lastEssen = i;
		else if ((format[i] & 0xff00) == 0x0100 && vArr[format[i] & 0xff]->getEssen())
			lastEssen = i;
	}
	return true;
}

void CSD::showSds()
{
	int i, j;
	for (i = 0; i < formatNum; i++)
	{
		if ((format[i] & 0xff00) == 0x0400) // id
		{
			cout << "ID " ;
		}
		else if ((format[i] & 0xff00) == 0x0800) // str
		{
			cout << "ST " ;
		}
		else if ((format[i] & 0xff00) == 0x0100) // var
		{
			cout << "#" << (format[i] & 0xff) << " ";
		}
		else if ((format[i] & 0xff00) == 0x0200) // non-essen hex
		{
			cout << "$" << (format[i] & 0xff) << " ";
		}
		else
		{
			cout << format[i] << " ";
		}
	}
	cout << endl;
	for (i = 0; i < vNum; i++)
	{
		cout << "#" << i << ": " << endl;
		cout << "name: " << vArr[i]->name << endl;
		cout << "isEssen: " << vArr[i]->isEssen << endl;
		cout << "isStrDep: " << vArr[i]->isStrDep << endl;
		cout << "def: " << vArr[i]->def << endl;
		cout << "equal: ";
		for (j = 0; j < vArr[i]->equalNum; j++)
		{
			cout << "[" << j << "]" << vArr[i]->equal[j];
		}
		cout << endl;
		cout << "nEqual: ";
		for (j = 0; j < vArr[i]->nEqualNum; j++)
		{
			cout << "[" << j << "]" << vArr[i]->nEqual[j];
		}
		cout << endl;
		cout << "lessthen: " << vArr[i]->lessthen << endl;
		cout << "biggerthen: " << vArr[i]->biggerthen << endl;
	}
}

bool CSD::decompile(const char * src, const char *dst, bool tmplt)
{
	FILE *dat = NULL, *txt = NULL;
	int *v = NULL, tmp;
	char sel;
	char **id = NULL, **str = NULL;
	int isStr, idS=0, strS=0;
	int i,j;
	int vn;

	if (tmplt) {
		cout << endl << ">> WARNING: this is template creation mode. this will destroy your work data if .txt file Exists.<<" << endl << endl << "Do you want to continue? (y/n) :";
	}
	else
	{
		cout << endl << ">> WARNING: this is DECOMPILE mode. this will destroy your work data if .txt file Exists.<<" << endl << endl << "Do you want to continue? (y/n) :";
	}

	cin >> sel;

	if (sel != 'y')
	{
		cout << "exits..." << endl;
		return false;
	}

	try {
		dat = fopen(src, "rb");

		if (!dat)
		{
			throw "No .dat file";
		}

		txt = fopen(dst, "wb+");

		if (!txt)
		{
			throw "Cannot create .txt file";
		}

		if (formatNum == 0)
		{
			throw "Invaild sds file loaded";
		}

		if (idNum) {
			id = new char*[idNum];
		}
		if (strNum){
			str = new char*[strNum];
		}

		for (i = 0; i < idNum; i++) {
			id[i] = new char[idSize];
		}
		for (i = 0; i < strNum; i++) {
			str[i] = new char[strSize];
		}

		v = new int[vNum];

		for (int i = 0; i < vNum; i++)
		{
			v[i] = vArr[i]->getDef();
		}
		




		for (; !feof(dat);)
		{
			idS = 0;
			strS = 0;
			isStr = 1;
			for (i = 0; i < idNum; i++) {
				id[i][0] = 0;
			}
			for (i = 0; i < strNum; i++) {
				str[i][0] = 0;
			}
			fgetc(dat);
			if (feof(dat)){
				return true;
			}
			else
			{
				fseek(dat, -1, SEEK_CUR);
			}
			for (i = 0; i < vNum; i++)
			{
				v[i] = vArr[i]->getDef();
			}
			for (j = 0; j < formatNum && !feof(dat); j++)
			{
				if (j > lastEssen && isNextLine(dat))
				{
					break;
				}
				else if ((format[j] & 0xff00) == 0x0100)			// Var
				{
					vn = format[j] & 0xff;
					if (vArr[vn]->getStrDep() && !isStr)
					{
						continue;
					}

					if (vArr[vn]->getEssen())
					{
						tmp = fgetc(dat);
						if (vArr[vn]->isVaild(tmp))
						{
							v[vn] = tmp;
							//printf(" Var:%x\n", tmp);
							continue;
						}
						else
							throw "Invaild Var Data";
					}
					else									// non-Essen Var
					{
						tmp = fgetc(dat);
						if ((format[j + 1] & 0xff00) == 0 && tmp == format[j + 1])	// Next is Hex
						{
							fseek(dat, -1, SEEK_CUR);
							continue;
						}
						else if ((format[j + 1] & 0xff00) == 0x0200 && isStr && tmp == format[j + 1])	// Next is SD-Hex
						{
							fseek(dat, -1, SEEK_CUR);
							continue;
						}
						else if ((format[j + 1] & 0xff00) == 0x0200 && isStr !=2 && nextHex(j + 1) == tmp)	// Next is SD-Hex
						{
							isStr = 0;
							fseek(dat, -1, SEEK_CUR);
							continue;
						}
						else if (vArr[vn]->getSet())
						{
							if (vArr[vn]->isVaild(tmp))
							{
								v[vn] = tmp;
								//printf(" non-EssenVar:%x\n", tmp);
							}
							else
							{
								fseek(dat, -1, SEEK_CUR);
							}
							continue;
						}
						else
						{
							throw "You need to set condition to non-Essen Var";
						}
					}
				}
				else if ((format[j] & 0xff00) == 0x0400)	// ID
				{
					for (i = 0; !feof(dat); i++)
					{
						if (i >= idSize-1)
						{
							throw "ERR: Need more id string buffer";
						}
						else if (isNextLine(dat))
						{
							++idS;
							break;
						}
						id[idS][i] = fgetc(dat); 
						if((format[j+1] & 0xff00) == 0x00 && id[idS][i] == (format[j + 1] & 0xff)) //hex
						{
							fseek(dat, -1, SEEK_CUR);
							++idS;
							break;
						}
						else if ((format[j + 1] & 0xff00) == 0x0200 && isStr && id[idS][i] == (format[j + 1] & 0xff)) // SD-Hex
						{
							fseek(dat, -1, SEEK_CUR);
							++idS;
							break;
						}
						else if ((format[j + 1] & 0xff00) == 0x0200 && isStr!=2 && id[idS][i] == nextHex(j + 1)) // SD-Hex
						{
							isStr = 0;
							fseek(dat, -1, SEEK_CUR);
							++idS;
							break;
						}
					}
					id[idS-1][i] = 0;
				}
				else if ((format[j] & 0xff00) == 0x0800)	// ST
				{
					if (isStr == 0)
					{
						continue;
					}

					for (i = 0; !feof(dat); i++)
					{
						if (i >= strSize-1)
							throw "ERR: Need more str string buffer";
						if (isNextLine(dat))
						{
							++strS;
							break;
						}
						str[strS][i] = fgetc(dat);
						if ((format[j + 1] & 0xff00) == 0x00 && str[strS][i] == (format[j + 1] & 0xff)) //hex
						{
							fseek(dat, -1, SEEK_CUR);
							++strS;
							break;
						}
						else if ((format[j + 1] & 0xff00) == 0x0200 && str[strS][i] == (format[j + 1] & 0xff)) // SD-Hex
						{
							fseek(dat, -1, SEEK_CUR);
							++strS;
							break;
						}
					}

					str[strS-1][i] = 0;
					isStr = 2;
				}
				else if ((format[j] & 0xff00) == 0x0200)	// String Dependence Hex
				{
					if (isStr == 0)
					{
						continue;
					}
					tmp = fgetc(dat);
					if (tmp == (format[j] & 0xff))
					{
						isStr = 2;
						//printf(" SD-Hex:%x\n", tmp);
						continue;
					}
					else if (isStr != 2)
					{
						isStr = 0;
						fseek(dat, -1, SEEK_CUR);
					}
					else
					{
						throw "Error SD-Hex";
					}
				}
				else										// Hex
				{
					tmp = fgetc(dat);
					if (tmp == format[j])
					{
						//printf(" Hex:%x\n", tmp);
						continue;
					}
					else
					{
						printf(" loop = %d\n", loop);
						printf("in sds: %#x ", format[j]);
						printf("in dat: %#x\nnext:\n", tmp);
						printf(" %#x\n", fgetc(dat));
						printf(" %#x\n", fgetc(dat));
						printf(" %#x\n", fgetc(dat));
						throw "Error reading Hex";
					}
				}
			}
			for (i = 0; i < vNum; i++)
			{
				fprintf(txt, "%d ", v[i]);
			}
			for (i = 0; i < idNum; i++)
			{
				fprintf(txt, "%s\n", id[i]);
			}
			if (!tmplt) {
				for (int i = 0; i < strNum; i++)
				{
					if (strNum > 1) {
						fprintf(txt, "[sTr]");
					}
					fprintf(txt, "%s", str[i]);
					fprintf(txt, "\n\n");
				}
			}
			else
			{
				for (int i = 0; i < strNum; i++)
				{
					if (strNum > 1) {
						fprintf(txt, "[sTr]");
					}
					fprintf(txt, "\n\n");
				}
			}
		}






		if (idNum) {
			delete[] id;
		}
		if (strNum) {
			delete[] str;
		}
		delete v;
		fclose(dat);
		fclose(txt);
	}
	catch (char *s)
	{
		cout << "[ERR] " << s << endl;
		if (dat)
			fclose(dat);
		if (txt)
			fclose(txt);
		if (v)
			delete v;
		if (id && idNum)
			delete[]id;
		if (str && strNum)
			delete[]str;
		return false;
	}
	catch (...)
	{
	}
	if (!tmplt)
	{
		cout << endl << "decompile finished." << endl;
	}
	else {
		cout << endl << "template creation complete." << endl;
	}
	cout << endl << "<!--Important--!>" << endl;
	cout << "You should edit string in same byte (this is multibyte char data)" << endl;
	cout << "You shouldn't edit binary number data in file (it will corrupt data.jmp)" << endl;
	return true;
}

bool CSD::compile(const char * src, const char *dst)
{
	FILE *dat=NULL, *txt = NULL;
	int *v=NULL, tmp;
	char sel;
	char **id = NULL, **str = NULL;
	int idC = 0, strC = 0;
	int i;

	cout << endl << "info: this is compile mode. this will overwite if .dat file Exists." << endl << endl << "Do you want to continue? (y/n) :";

	cin >> sel;

	if (sel != 'y')
	{
		cout << "exits..." << endl;
		return false;
	}

	try {
		txt = fopen(src, "rb");

		if (!txt) {
			throw "No .txt file";
		}

		dat = fopen(dst, "wb+");

		if (!dat) {
			throw "Cannot create .dat file";
		}

		if (formatNum == 0) {
			throw "Invaild sds file loaded";
		}

		if (idNum) {
			id = new char*[idNum];
		}
		if (strNum) {
			str = new char*[strNum];
		}
		
		for (int i = 0; i < idNum; i++){
			id[i] = new char[idSize];
		}
		for (int i = 0; i < strNum; i++){
			str[i] = new char[strSize];
		}

		v = new int[vNum]();
		
		for (; !feof(txt);)
		{
			sel = fgetc(txt);
			if (feof(txt))
			{
				break;
			}
			else {
				fseek(txt, -1, SEEK_CUR);
			}

			idC = 0;
			strC = 0;
			for (i = 0; i < idNum; i++) {
				id[i][0] = 0;
			}
			for (i = 0; i < strNum; i++) {
				str[i][0] = 0;
			}
			//cout << "v: ";
			for (i = 0; i < vNum; i++)
			{
				if (fscanf(txt, "%d ", &v[i]) <= 0) {
					throw "Invaild txt file";
				}
			//	cout << "[" << i << "]" << v[i];
			}

			for (; idC < idNum && !feof(txt); ++idC)
			{
				for (i = 0; i < idSize && !feof(txt); i++)
				{
					if (i == idSize-1) {
						throw "No enough idSize";
					}
					id[idC][i] = fgetc(txt);
					if (id[idC][i] == 0x0a || id[idC][i]==0xffff) {
						id[idC][i] = 0;
						break;
					}
				}
			}

			for (;strC < strNum; ++strC)
			{
				for (i = 0; !feof(txt); i++)
				{
					if (i >= strSize) {
						throw "No enough strSize";
					}
					str[strC][i] = fgetc(txt);
					if (str[strC][i] == 0x0a)
					{
						if (fgetc(txt) == 0x0a)
						{
							tmp = fgetc(txt);
							if (feof(txt)){
								str[strC][i] = 0;
								break;
							}
							else if (tmp >= '0' && tmp <= '9'){
								fseek(txt, -1, SEEK_CUR);
								str[strC][i] = 0;
								break;
							}
							else if (tmp == '[' && strNum >1) {
								if (fgetc(txt) == 's')
								{
									if (fgetc(txt) == 'T') {
										if (fgetc(txt) == 'r') {
											if (fgetc(txt) == ']') {
												if (fgetc(txt) == 0x0a) {
													str[strC][i] = 0;
													break;
												}
												else {
													fseek(txt, -1, SEEK_CUR);
												}
											}
											else {
												fseek(txt, -1, SEEK_CUR);
											}
										}
										else {
											fseek(txt, -1, SEEK_CUR);
										}
									}
									else {
										fseek(txt, -1, SEEK_CUR);
									}
								}
								else {
									fseek(txt, -1, SEEK_CUR);
								}
							}
							fseek(txt, -1, SEEK_CUR);
						}
						fseek(txt, -1, SEEK_CUR);
					}
				}
			}
			idC = 0;
			strC = 0;
			
			int vntmp;
			
			for (i = 0; i < formatNum; i++)
			{
				if (!(format[i] & 0xff00))			// 그냥 평문
				{
					fputc(format[i], dat);
				}
				else if ((format[i] & 0xff00) == 0x0200)	// str dependence인 Hex
				{
					if (str[strC][0] != 0)
					{
						fputc(format[i] & 0xff, dat);
					}
				}
				else if ((format[i] & 0xff00) == 0x0100)	// 변수
				{
					vntmp = format[i] & 0xff;

					if (vNum <= vntmp)
					{
						//cout << vNum << " " << vntmp << " " << format[i] << endl;
						throw "Invaild sds : No var";
					}
					else if (vArr[vntmp]->getStrDep() && str[strC][0]==0)		// 만약 str dependence인데 str이 없는 경우 
					{
						continue;
					}
					else if (vArr[vntmp]->getEssen())		// 만약 essen일경우 
					{
						if (vArr[vntmp]->isVaild(v[vntmp])) {
							fputc(v[vntmp], dat);
						}
						else {
							throw "Invaild Var value";
						}
					}
					else if (vArr[vntmp]->getDef() != v[vntmp])	// 만약 출력이 필요한 non-essen일경우 
					{
						//cout << vArr[vntmp]->name << vArr[vntmp]->getDef() << v[vntmp] << endl;
						if (vArr[vntmp]->isVaild(v[vntmp])) {
							fputc(v[vntmp], dat);
						}
						else {
							throw "Invaild Var value";
						}
					}
					else {
						continue;
					}
				}
				else if ((format[i] & 0xff00) == 0x0400) // id
				{
					for (int i = 0;; i++)
					{
						if (id[idC][i] == 0){
							break;
						}
						else{
							fputc(id[idC][i], dat);
						}
					}
					idC++;
				}
				else if ((format[i] & 0xff00) == 0x0800) // str
				{
					for (int i = 0;; i++)
					{
						if (str[strC][i] == 0){
							break;
						}
						else {
							fputc(str[strC][i], dat);
						}
					}
					strC++;
				}
				else
				{
					cout << format[i];
					throw "Invaild format string";
				}
			}

		}



		if (idNum) {
			delete[] id;
		}
		if (strNum) {
			delete[] str;
		}
		delete v;
		fclose(dat);
		fclose(txt);
	}
	catch (char *s)
	{
		cout << "[ERR] compile(): " << s << endl;
		if (dat)
			fclose(dat);
		if (txt)
			fclose(txt);
		if (v)
			delete v;
		if (id && idNum)
			delete []id;
		if (str && strNum)
			delete []str;
		return false;
	}
	catch (...)
	{
	}
	return true;
}