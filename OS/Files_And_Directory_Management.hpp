#pragma once
//Krzysio
#include <string>
#include <vector>
#include <set>
#include <bitset>
#include <map>

#include "Drive.hpp"
#include "Sync_Mech.hpp"

//SEMAFOR W PLIKU COŒ DZIA£A
//CZY MOGÊ ZAMIAST ZNAKU ODPOWIADAJ¥CEGO NUMEROWI BLOKU WYŒWIETLAÆ NUMER TEGO BLOKU PRZY WYŒWIETLANIU DYSKU?
//SKORO MAMY WSKAZYWAÆ NA Nastêpne pole do zapisu, to co wtedy gdy ostatnie zapisane pole bêdzie ostatnim polem bloku?
class Filesystem
{

	using string = std::string;
	struct File
	{

		Semaphore sem;
		string name;
		int adres, towrite, toread;
		File(string name, int adres) :sem(1)
		{
			this->name = name;
			this->adres = adres;
			towrite = 0;
			toread = 0;

		}
		File() :sem(1)
		{
			towrite = 0;
			toread = 0;
		}
	};
public:
	Filesystem() {}
	//TD
	//READFILE
	//OGARN¥Æ DOBRZE SPRAWDZANIE CZY WGL JEST TYLE MIEJSCA
	bool openFile(const string &filename, const string &pid);//DZIA£A
	bool openFile(const string &filename);//DZIA£A

	bool closeFile(const string &filename, const string &pid);//DZIA£A
	bool closeFile(const string &filename);//DZIA£A

	bool deleteFile(const string &filename, const string &pid); //NIE DZIA£A
	bool deleteFile(const string &filename);

	bool overwriteFile(const string &filename, const string &content, const string &pid);//DZIA£A
	bool overwriteFile(const string &filename, const string &content);//DZIA£A

	bool writeToFile(const string &filename, const string &content, const string &pid);//DZIA£A
	bool writeToFile(const string &filename, const string &content);//DZIA£A

	bool verify(const string &filename);//DZIA£a

	bool readFile(const string &filename, const string &pid, int length, string &content);//zobaczymy
	bool readFile(const string &filename, int length, string &content);//zobaczymy

	bool renameFile(const string &filename, const string  &newfilename, const string &pid);//DZIA£A
	bool renameFile(const string &filename, const string  &newfilename);//DZIA£A

	bool createFile(const string &filename);//DZIA£a
	bool fileExists(const string &filename);//DZIA£a
	int countFreeBlocks(std::bitset<Drive::blocks> &bt);//DZIA£a


	int findFreeBlock(std::bitset<Drive::blocks> &bt);//DZIA£a
	int hireFreeBlock(std::bitset<Drive::blocks> &bt);

	void displaycatalogue();//DZIA£A
	void stats();//DZIA£a
	void displayoft();//DZIA£A
	void displayvector();//DZIA£A
	void displayblock(int block);//DZIA£A
	void displaydrivecontent();//DZIA£A
	void printqueue(string &name);
private:
	Drive maindrive;
	std::bitset<Drive::blocks> blockTable;
	//MAPA Z NAZW¥ PLIKU I PID
	std::map <string, string>  openfiletable;

	std::vector<File> maincatalogue;
};
