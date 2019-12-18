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
		bool isOpen;
		File(string name, int adres) :sem(1)
		{
			this->name = name;
			this->adres = adres;
			towrite = 0;
			toread = 0;
			isOpen = false;

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
	bool openFile(const string &filename, const string &pid);
	bool openFile(const string &filename);

	bool closeFile(const string &filename, const string &pid);
	bool closeFile(const string &filename);

	bool deleteFile(const string &filename, const string &pid); 
	bool deleteFile(const string &filename);

	bool overwriteFile(const string &filename, const string &content, const string &pid);
	bool overwriteFile(const string &filename, const string &content);

	bool writeToFile(const string &filename, const string &content, const string &pid);
	bool writeToFile(const string &filename, const string &content);

	bool verify(const string &filename);

	bool readFile(const string &filename, const string &pid, int length, string &content);
	bool readFile(const string &filename, int length);

	bool renameFile(const string &filename, const string  &newfilename, const string &pid);
	bool renameFile(const string &filename, const string  &newfilename);

	bool createFile(const string &filename);
	bool fileExists(const string &filename);
	int countFreeBlocks(std::bitset<Drive::blocks> &bt);


	int findFreeBlock(std::bitset<Drive::blocks> &bt);
	int hireFreeBlock(std::bitset<Drive::blocks> &bt);

	void displaycatalogue();
	void stats();//DZIA£a
	void displayoft();//DZIA£A
	void displayvector();//DZIA£A
	void displayblock(int block);//DZIA£A
	void displaydrivecontent();//DZIA£A
	void printqueue(string &name);
	
	std::vector<File> maincatalogue;
private:
	Drive maindrive;
	std::bitset<Drive::blocks> blockTable;
	//MAPA Z NAZW¥ PLIKU I PID
	std::map <string, string>  openfiletable;

};
