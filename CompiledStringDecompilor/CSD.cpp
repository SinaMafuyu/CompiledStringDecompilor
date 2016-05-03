#include "csd.h"

void CSD::sortVar(void)
{
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
					Var *tmp=vArr[i];
					vArr[i] = vArr[j];
					vArr[j] = tmp;

					for(int k=0; k<formatNum; k++)
					{
						if (format[k] == (i | 0x0100))
							format[k] = (j | 0x0100);
						else if (format[k] == (j | 0x0100))
							format[k] = (i | 0x0100);
					}

					break;
				}
				if (j + 1 == vNum)
					return;
			}
		}
	}
}

Var* CSD::getVar(char *name)
{
	for (int i = 0; i < vNum; i++)
	{
		if (!strcmp(vArr[i]->getName(), name))
		{
			return vArr[i];
		}
	}
	return NULL;
}

int CSD::addVar(char *name)
{
	if (vSize == 0)
	{
		vArr = new Var*[10];
		vSize = 10;
	}
	else if (vSize == vNum)
	{
		Var **tmp = vArr;
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
	if (formatSize == 0)
	{
		format = new int[50];
		formatSize = 50;
	}
	else if (formatSize == formatNum)
	{
		int* tmp = format;
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
};

CSD::~CSD() {
	if (format != NULL)
		delete format;
	if (vArr != NULL)
		delete vArr;
};

bool CSD::loadSds(char *fileName)
{
	FILE *fp = fopen(fileName, "r");
	char buf[100];

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
		if (strcmp(buf, "sds:v1.0"))
			throw "Invaild sds version Info";

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

		if (fscanf(fp, "%d %d", &idSize, &strSize) != 2)
			throw "No id, strSize in .txt";
		if (idSize < 1 || strSize < 1)
			throw "idSize, strSize must be more than 0";

		if(fgetc(fp) != 0x0a)
			throw "No id, strSize in .txt";

		int a, b;

		char var[30];

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
			Var *v;
			fscanf(fp, "%[^ ]", var);
			if (fgetc(fp) != ' ')
				throw "Invaild sds var option";
			a = fgetc(fp);
			v = getVar(var);
			if (v == NULL)
				throw "No var name in format";

			int tmp;

			if (a == '~')
			{
				fscanf(fp, "%x", &tmp);
				v->setEssen(false, tmp);
			}
			else if (a == '<')
			{
				a = fgetc(fp);

				fscanf(fp, "%x", &tmp);
				if (a != '=')
					--tmp;
				v->setLessthen(tmp);
			}
			else if (a == '>')
			{
				a = fgetc(fp);
				fscanf(fp, "%x", &tmp);
				if (a != '=')
					++tmp;
				v->setBiggerthen(tmp);
			}
			else if (a == '=')
			{
				fscanf(fp, "%x", &tmp);
				v->addEqual(tmp);
			}
			else if (a == '$')
			{
				v->setStrDep(true);
			}
			else if (a == '!' && fgetc(fp) == '=')
			{
				fscanf(fp, "%x", &tmp);
				v->addNequal(tmp);
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
	return true;
}

void CSD::showSds()
{
	for (int i = 0; i < formatNum; i++)
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
	for (int i = 0; i < vNum; i++)
	{
		cout << "#" << i << ": " << endl;
		cout << "name: " << vArr[i]->name << endl;
		cout << "isEssen: " << vArr[i]->isEssen << endl;
		cout << "isStrDep: " << vArr[i]->isStrDep << endl;
		cout << "def: " << vArr[i]->def << endl;
		cout << "equal: ";
		for (int j = 0; j < vArr[i]->equalNum; j++)
		{
			cout << "[" << j << "]" << vArr[i]->equal[j];
		}
		cout << endl;
		cout << "nEqual: ";
		for (int j = 0; j < vArr[i]->nEqualNum; j++)
		{
			cout << "[" << j << "]" << vArr[i]->nEqual[j];
		}
		cout << endl;
		cout << "lessthen: " << vArr[i]->lessthen << endl;
		cout << "biggerthen: " << vArr[i]->biggerthen << endl;
	}
}

bool CSD::decompile(char * src, char *dst)
{
	FILE *dat, *txt;

	char sel;

	cout << endl << ">> WARNING: this is DECOMPILE mode. this will destroy your work data if .txt file Exists.<<" << endl << endl << "Do you want to continue? (y/n) :";

	cin >> sel;

	if (sel != 'y')
	{
		cout << "exits..." << endl;
		return false;
	}

	try {
		dat = fopen(src, "rb");

		if (!dat)
			throw "No .dat file";

		txt = fopen(dst, "wb+");

		if (!txt)
			throw "Cannot create .txt file";

		if (formatNum==0)
			throw "Invaild sds file loaded";








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
		return false;
	}
	catch (...)
	{
	}
	cout << endl << "decompile finished." << endl;
	cout << endl << "<!--Important--!>" << endl;
	cout << "You should edit string in same byte (this is multibyte char data)" << endl;
	cout << "You shouldn't edit binary number data in file (it will corrupt data.jmp)" << endl;
	return true;
}

bool CSD::compile(char * src, char *dst)
{
	FILE *dat=NULL, *txt = NULL;
	int *v=NULL, tmp;
	char sel;
	char *id = NULL, *str = NULL;

	cout << endl << "info: this is compile mode. this will overwite if .dat file Exists." << endl << endl << "Do you want to continue? (y/n) :";

	cin >> sel;

	if (sel != 'y')
	{
		cout << "exits..." << endl;
		return false;
	}

	try {
		txt = fopen(src, "rb");

		if (!txt)
			throw "No .txt file";

		dat = fopen(dst, "wb+");

		if (!dat)
			throw "Cannot create .dat file";

		if (formatNum == 0)
			throw "Invaild sds file loaded";

		id = new char[idSize];
		str = new char[strSize];
		id[0] = 0;
		str[0] = 0;

		v = new int[vNum]();
		
		for (; !feof(txt);)
		{
			//cout << "v: ";
			for (int i = 0; i < vNum; i++)
			{
				if (fscanf(txt, "%d ", &v[i]) <= 0)
					throw "Invaild txt file";
			//	cout << "[" << i << "]" << v[i];
			}

			for (int i = 0; i <= idSize; i++)
			{
				id[i] = fgetc(txt);
				if (i == idSize)
					throw "No enough idSize";
				if (id[i] == 0x0a)
				{
					id[i] = 0;
					break;
				}
			}

			for (int i = 0; !feof(txt); i++)
			{
				str[i] = fgetc(txt);
				if (i >= strSize)
					throw "No enough strSize";
				if (str[i] == 0x0a)
				{
					if (fgetc(txt) == 0x0a)
					{
						tmp = fgetc(txt);
						if (feof(txt))
						{
							str[i] = 0;
							break;
						}
						if (tmp >= '0' && tmp <= '9')
						{
							fseek(txt, -1, SEEK_CUR);
							str[i] = 0;
							break;
						}
						fseek(txt, -1, SEEK_CUR);
					}
					fseek(txt, -1, SEEK_CUR);
				}
			}
			
			int vntmp;
			
			for (int i = 0; i < formatNum; i++)
			{
				if (!(format[i] & 0xff00))			// 그냥 평문
					fputc(format[i], dat);
				else if ((format[i] & 0xff00) == 0x0200)	// str dependence인 Hex
				{
					if(str[0]!=0)
						fputc(format[i] & 0xff, dat);
				}
				else if ((format[i] & 0xff00) == 0x0100)	// 변수
				{
					vntmp = format[i] & 0xff;

					if (vNum <= vntmp)
					{
						cout << vNum << " " << vntmp << " " << format[i] << endl;
						throw "Invaild sds : No var";
					}
					else if (vArr[vntmp]->getStrDep() && str[0]==0)		// 만약 str dependence인데 str이 없는 경우 
					{
						continue;
					}
					else if (vArr[vntmp]->getEssen())		// 만약 essen일경우 
					{
						if (vArr[vntmp]->isVaild(v[vntmp]))
							fputc(v[vntmp], dat);
						else
							throw "Invaild Var value";
					}
					else if (vArr[vntmp]->getDef() != v[vntmp])	// 만약 출력이 필요한 non-essen일경우 
					{
						cout << vArr[vntmp]->name << vArr[vntmp]->getDef() << v[vntmp] << endl;
						if (vArr[vntmp]->isVaild(v[vntmp]))
							fputc(v[vntmp], dat);
						else
							throw "Invaild Var value";
					}
					else
						continue;
				}
				else if ((format[i] & 0xff00) == 0x0400) // id
				{
					for (int i = 0;; i++)
					{
						if (id[i] == 0)
							break;
						fputc(id[i], dat);
					}
				}
				else if ((format[i] & 0xff00) == 0x0800) // str
				{
					for (int i = 0;; i++)
					{
						if (str[i] == 0)
							break;
						fputc(str[i], dat);
					}
				}
				else
				{
					cout << format[i];
					throw "Invaild format string";
				}
			}

		}



		delete id;
		delete str;
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
		if (id)
			delete id;
		if (str)
			delete str;
		return false;
	}
	catch (...)
	{
	}
	return true;
}