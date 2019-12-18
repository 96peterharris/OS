//Krzysio
#include "Files_And_Directory_Management.hpp"
#include "CPU_Scheduling.hpp"

#include "Headers.h"
int Filesystem::findFreeBlock(std::bitset<Drive::blocks> &bt)
{
	for (int i = 0; i < bt.size(); i++)
	{
		if (bt[i] == false) return i;
	}
	return -1;
}
bool Filesystem::fileExists(const string &filename)
{
	if (maincatalogue.size() != 0)
	{
		for (int i = 0; i < maincatalogue.size(); i++)
		{
			if (maincatalogue.at(i).name == filename)
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		return false;
	}
}
bool Filesystem::createFile(const string &filename)
{
	if (fileExists(filename))
	{
		std::cout << "E001:File already exists" << std::endl << std::endl;
		return false;
	}
	else
	{
		//Tworzenie pliku		
		if (countFreeBlocks(blockTable) < 2)
		{
			std::cout << "E002:Insufficient drivespace" << std::endl << std::endl;
			return false;
		}
		else
		{
			//Blok indeksowy
			int indexblock = hireFreeBlock(blockTable);
			//1 automatycznie inicjowany blok danych
			int data = hireFreeBlock(blockTable);
			//Umieszczanie wpisu pliku w katalogu
			File tempfile(filename, indexblock);
			tempfile.towrite = data * 32;
			maindrive.drivespace[indexblock * 32] = data;
			data = maindrive.drivespace[indexblock * 32];
			maincatalogue.push_back(tempfile);
		}
		return true;
	}
}
int Filesystem::countFreeBlocks(std::bitset<Drive::blocks> &bt)
{
	int fb = 0;
	for (int i = 0; i < bt.size(); i++)
	{
		if (bt[i] == false) fb++;
	}
	return fb;
}
//"ZATRUDNIANIE WOLNEGO BLOKU"//DZIA£A
int Filesystem::hireFreeBlock(std::bitset<Drive::blocks> &bt) //"ZATRUDNIANIE" WOLNYCH BLOKÓW// DZIA£A
{
	int bn = findFreeBlock(bt);
	if (bn != -1)
	{
		bt[bn] = true;
		return bn;
	}
	return -1;
}
//WYŒWIETLA STATYSTYKI//DZIA£A
void Filesystem::stats()
{
	std::cout << "\nFree driveblocks:" << countFreeBlocks(blockTable) << std::endl << std::endl;
	std::cout << "Total driveblock number: " << Drive::blocks << std::endl << std::endl;
	std::cout << "Drive size:" << Drive::drivesize << std::endl << std::endl;
}
//DODAJE PLIK DO TABLICY OTWARTYCH PLIKÓW
bool Filesystem::openFile(const string &filename)
{
	//SPRAWDZAM CZY PLIK WGL ISTNIEJE
	if (fileExists(filename))
	{
		for (int i = 0; i < maincatalogue.size(); i++)
		{
			if (maincatalogue[i].name == filename)
			{

				//maincatalogue[i].sem.wait_sem(pid);
				maincatalogue[i].isOpen = true;
				openfiletable.insert(std::pair <string, string>(filename, "GUI_PROCESS"));
				return true;
			}
		}

	}
	else
	{
		std::cout << "E006:File does not exist" << std::endl << std::endl;
		return false;
	}
}
bool Filesystem::openFile(const string &filename, const string &pid)
{
	//SPRAWDZAM CZY PLIK WGL ISTNIEJE
	if (fileExists(filename))
	{
		for (int i = 0; i < maincatalogue.size(); i++)
		{
			if (maincatalogue[i].name == filename)
			{
				//maincatalogue[i].sem.wait_sem(pid);
				maincatalogue[i].isOpen = true;
				openfiletable.insert(std::pair <string, string>(filename, pid));
				return true;
			}
		}

	}
	else
	{
		std::cout << "E006:File does not exist" << std::endl << std::endl;
		return false;
	}
}
//USUWA PLIK Z TABLICY OTWARTYCH PLIKÓW
bool Filesystem::closeFile(const string &filename)
{
	if (fileExists(filename))
	{
		for (int i = 0; i < maincatalogue.size(); i++)
		{
			if (maincatalogue[i].name == filename)
			{
				if (maincatalogue[i].isOpen == true)
				{
					//maincatalogue[i].sem.signal_sem();
					openfiletable.erase(filename);
					maincatalogue[i].isOpen = false;
					//maincatalogue[i].toread = 0;
					return true;
				}
				else
				{
					std::cout << "E009:File is not opened" << std::endl << std::endl;
					return false;
				}
			}
		}
	}
	else
	{
		std::cout << "E006:File does not exist" << std::endl << std::endl;
		return false;
	}
}
bool Filesystem::closeFile(const string &filename, const string &pid)
{
	//SPRAWDZANIE CZY PLIK ISTNIEJE
	if (fileExists(filename))
	{
		for (int i = 0; i < maincatalogue.size(); i++)
		{
			if (maincatalogue[i].name == filename)
			{
				if (maincatalogue[i].isOpen == true)
				{
					maincatalogue[i].sem.signal_sem();
					openfiletable.erase(filename);
					maincatalogue[i].isOpen = false;
					maincatalogue[i].toread = 0;
					return true;
				}
				else
				{
					std::cout << "E009:File is not opened" << std::endl << std::endl;
					return false;
				}
			}
		}
	}
	else
	{
		std::cout << "E006:File does not exist" << std::endl << std::endl;
		return false;
	}
}
bool Filesystem::renameFile(const string &filename, const string &newfilename)
{
	//GUI
	if (guiverify(filename))
	{
		openFile(filename);
		if (fileExists(newfilename))
		{
			std::cout << "E011:New filename is already in use" << std::endl << std::endl;
			return false;
		}
		else
		{
			for (int i = 0; i < maincatalogue.size(); i++)
			{
				if (maincatalogue[i].name == filename)
				{
					File tempfile = maincatalogue[i];
					tempfile.name = newfilename;
					maincatalogue.erase(maincatalogue.begin() + (i));
					maincatalogue.push_back(tempfile);
				}
			}
			closeFile(filename);
			return true;
		}
	}
	else
	{
		return false;
	}

}
bool Filesystem::renameFile(const string &filename, const string &newfilename, const string &pid)
{
	if (verify(filename))
	{
		if (fileExists(newfilename))
		{
			std::cout << "E011:New filename is already in use" << std::endl << std::endl;
			return false;
		}
		else
		{
			for (int i = 0; i < maincatalogue.size(); i++)
			{
				if (maincatalogue[i].name == filename)
				{
					File tempfile = maincatalogue[i];
					tempfile.name = newfilename;
					maincatalogue.erase(maincatalogue.begin() + (i));
					maincatalogue.push_back(tempfile);
				}
			}
			return true;
		}
	}
	else
	{
		return false;
	}


}
bool Filesystem::overwriteFile(const string &filename, const string &content)
{//GUI
	if (guiverify(filename))
	{
		openFile(filename);
		int indexblock;
		File tempfile;
		int at;
		for (int i = 0; i < maincatalogue.size(); i++)
		{
			if (maincatalogue[i].name == filename)
			{
				tempfile = maincatalogue[i];
				indexblock = tempfile.adres * 32;//WSKAZUJE NA 1 POLE INDEKSOWE 
				at = i;
				maincatalogue[i].toread = 0;
				maincatalogue[i].towrite = 0;
			}
		}
		int b = 0;
		for (int i = 0; i < 32; i++)
		{
			if (maindrive.drivespace[i + indexblock] == 0)
			{
				break;
			}
			else
				b++;
		}
		//	std::cout << "B:" << b << std::endl;
		//	std::cout << content.size()/32+1 << "-To wymagane bloki" << (countFreeBlocks(blockTable) + b);
		if (((content.length() / 32) + 1) > (countFreeBlocks(blockTable) + b))
		{
			std::cout << "E007:Insufficient drivespace" << std::endl << std::endl;
			return false;
		}

		//USUWANIE DANYCH
		for (int i = 0; i < 32; i++)
		{
			if (maindrive.drivespace[i + indexblock] == 0)
			{
				break;
			}
			else
			{
				int blocknumber = maindrive.drivespace[i + indexblock];//NUMER OBECNIE ZWALNIANEGO BLOKU DYSKOWEGO 
				int currentblock = 32 * blocknumber;//ADRES TEGO BLOKU
				for (int j = 31; j > -1; j--)
				{
					maindrive.drivespace[currentblock + j] = 0;//ZERUJE POLA DANYCH
				}
				blockTable[blocknumber] = false;//ZWALNIA BLOK DANYCH
				maindrive.drivespace[i + indexblock] = 0; //USUWA BLOK Z BLOKU INDEKSOWEGO
				//USUWA W WEKTORZE
			}
		}
		//	int g;
		std::vector<int> blockvector;
		int rozmiar = content.size();
		for (int i = 0; i < ((rozmiar + 1) / 32) + 1; i++)//Rezerwacja bloków danych i przypisanie do bloku indeksowego 
		{
			int f = hireFreeBlock(blockTable);
			if (f != -1)
			{
				blockvector.push_back(f * 32);
				//		g = f * 32;//ZAPISUJE ADRESY BLOKÓW DANYCH DO ZAPISU
				maindrive.drivespace[indexblock + i] = f;
			}
			else
			{
				std::cout << "E001:Insufficient drivespace" << std::endl << std::endl;
				return false;
			}
		}
		std::vector <char> towrite;
		std::copy(content.begin(), content.end(), std::back_inserter(towrite));
		int j = 0;
		int ij = 0;
		int a = 0;
		for (int i = 0; i < content.size(); i++)
		{
			if (j >= blockvector.size())
			{
				break;
			}
			if (i >= 32)
			{
				i = 0;
				j++;
				if (j >= blockvector.size())
				{
					j = j - 1;
					break;
				}
			}

			maindrive.drivespace[blockvector[j] + i] = towrite[ij];

			ij++;
			if (ij == towrite.size())
			{
				break;
			}
			a = i + 1;
		}
		//std::cout << "JESTEM:" << j << " A rozmiar to:" << blockvector.size();

		ij = blockvector[j]; //TU NIE DZIA£A

		maincatalogue[at].towrite = ij + a + 1;
		closeFile(filename);
		return true;
		//ZAPIS DANYCH DO BLOKÓW
	}
	else
	{
		return false;
	}
}
bool Filesystem::overwriteFile(const string &filename, const string &content, const string &pid)
{
	if (verify(filename))
	{
		int indexblock;
		File tempfile;
		int at;
		for (int i = 0; i < maincatalogue.size(); i++)
		{
			if (maincatalogue[i].name == filename)
			{
				tempfile = maincatalogue[i];
				indexblock = tempfile.adres * 32;//WSKAZUJE NA 1 POLE INDEKSOWE 
				at = i;
				maincatalogue[i].toread = 0;
				maincatalogue[i].towrite = 0;
			}
		}
		int b = 0;
		for (int i = 0; i < 32; i++)
		{
			if (maindrive.drivespace[i + indexblock] == 0)
			{
				break;
			}
			else
				b++;
		}
		//	std::cout << "B:" << b << std::endl;
		//	std::cout << content.size()/32+1 << "-To wymagane bloki" << (countFreeBlocks(blockTable) + b);
		if (((content.length() / 32) + 1) > (countFreeBlocks(blockTable) + b))
		{
			std::cout << "E007:Insufficient drivespace" << std::endl << std::endl;
			return false;
		}

		//USUWANIE DANYCH
		for (int i = 0; i < 32; i++)
		{
			if (maindrive.drivespace[i + indexblock] == 0)
			{
				break;
			}
			else
			{
				int blocknumber = maindrive.drivespace[i + indexblock];//NUMER OBECNIE ZWALNIANEGO BLOKU DYSKOWEGO 
				int currentblock = 32 * blocknumber;//ADRES TEGO BLOKU
				for (int j = 31; j > -1; j--)
				{
					maindrive.drivespace[currentblock + j] = 0;//ZERUJE POLA DANYCH
				}
				blockTable[blocknumber] = false;//ZWALNIA BLOK DANYCH
				maindrive.drivespace[i + indexblock] = 0; //USUWA BLOK Z BLOKU INDEKSOWEGO
				//USUWA W WEKTORZE
			}
		}
		//	int g;
		std::vector<int> blockvector;
		int rozmiar = content.size();
		for (int i = 0; i < ((rozmiar + 1) / 32) + 1; i++)//Rezerwacja bloków danych i przypisanie do bloku indeksowego 
		{
			int f = hireFreeBlock(blockTable);
			if (f != -1)
			{
				blockvector.push_back(f * 32);
				//		g = f * 32;//ZAPISUJE ADRESY BLOKÓW DANYCH DO ZAPISU
				maindrive.drivespace[indexblock + i] = f;
			}
			else
			{
				std::cout << "E001:Insufficient drivespace" << std::endl << std::endl;
				return false;
			}
		}
		std::vector <char> towrite;
		std::copy(content.begin(), content.end(), std::back_inserter(towrite));
		int j = 0;
		int ij = 0;
		int a = 0;
		for (int i = 0; i < content.size(); i++)
		{
			if (j >= blockvector.size())
			{
				break;
			}
			if (i >= 32)
			{
				i = 0;
				j++;
				if (j >= blockvector.size())
				{
					j = j - 1;
					break;
				}
			}

			maindrive.drivespace[blockvector[j] + i] = towrite[ij];

			ij++;
			if (ij == towrite.size())
			{
				break;
			}
			a = i + 1;
		}
		//std::cout << "JESTEM:" << j << " A rozmiar to:" << blockvector.size();

		ij = blockvector[j]; //TU NIE DZIA£A

		maincatalogue[at].towrite = ij + a + 1;
		//ZAPIS DANYCH DO BLOKÓW
	}
	return true;
}
bool Filesystem::writeToFile(const string &filename, const string &content)
{
	//GUI
	if (guiverify(filename))
	{
		openFile(filename);
		int rozmiar = content.length();
		//std::cout << "\nRozmiar Pliku:" << rozmiar << std::endl;
		if (((rozmiar / 32)) > countFreeBlocks(blockTable))
		{
			std::cout << "E003:Insufficient drivespace" << std::endl << std::endl;
			return false;
		}
		//std::cout << "zapisuje do pliku";
		//ADRES BLOKU INDEKSOWEGO 
		int blokindeksowy;
		File tempfile;
		int at;
		for (int i = 0; i < maincatalogue.size(); i++)
		{
			if (maincatalogue[i].name == filename)
			{
				tempfile = maincatalogue[i];
				blokindeksowy = tempfile.adres * 32;
				at = i;
			}
		}
		int blokpamieci = (maindrive.drivespace[blokindeksowy]);//PIERWSZY ADRES Z BLOKU INDEKSOWEGO
		int adrespamieci = 32 * blokpamieci;

		int placetowrite = tempfile.towrite;

		int requiredblocks;
		if (rozmiar != 0)
		{
			requiredblocks = ceil((((double)rozmiar + 1) - (32 - placetowrite % 32)) / 32); //nieefektywne ale dzia³a	
		}

		if (rozmiar == 0)
		{
			requiredblocks = 0;
		}

		for (int i = 0; i < requiredblocks; i++)//Rezerwacja bloków danych i przypisanie do bloku indeksowego 
		{
			int f = hireFreeBlock(blockTable);
			if (f == -1)
			{
				std::cout << "\nE004:Insufficient drivespace\n" << std::endl;
				return false;
			}
			else
			{
				maindrive.drivespace[blokindeksowy + i + 1] = f;
			}
		}
		std::vector <char> towrite;
		int  it;
		it = 0;
		std::vector<int> blokidanych;
		while (maindrive.drivespace[blokindeksowy + it] != 0)
		{
			int f = maindrive.drivespace[blokindeksowy + it];
			blokidanych.push_back(f);
			it++;
		}
		std::copy(content.begin(), content.end(), std::back_inserter(towrite));
		int jmp = 0;
		int value;
		//Zapis do pliku
		for (int a = 0; a < towrite.size(); a++)
		{
			maindrive.drivespace[placetowrite] = towrite[a];
			placetowrite++;
			if (placetowrite % 32 == 0 && placetowrite != blokindeksowy)
			{

				if (jmp < blokidanych.size())
				{

					while (placetowrite > (blokidanych[jmp] * 32) + 31)//je¿eli zapisano wiêcej ni¿ 1 blok TO NIE BÊDZIE BRA£ 1 BLOKU DO ZAPISU TYLKO DALSZE 
					{
						jmp++;
						if (jmp < blokidanych.size())
						{

							break;
						}
					}
					placetowrite = 32 * blokidanych[jmp];
				}
				//Przesuwamy siê do nastêpnego bloku dyskowego
			}
		}
		maincatalogue[at].towrite = placetowrite;
		closeFile(filename);
		return true;
	}
	else
	{
		std::cout << "E005:Writing Error" << std::endl << std::endl;
		return false;
	}
}
bool Filesystem::writeToFile(const string &filename, const string &content, const string &pid)
{
	if (verify(filename))
	{
		int rozmiar = content.length();
		//std::cout << "\nRozmiar Pliku:" << rozmiar << std::endl;
		if (((rozmiar / 32)) > countFreeBlocks(blockTable))
		{
			std::cout << "E003:Insufficient drivespace" << std::endl << std::endl;
			return false;
		}
		//std::cout << "zapisuje do pliku";
		//ADRES BLOKU INDEKSOWEGO 
		int blokindeksowy;
		File tempfile;
		int at;
		for (int i = 0; i < maincatalogue.size(); i++)
		{
			if (maincatalogue[i].name == filename)
			{
				tempfile = maincatalogue[i];
				blokindeksowy = tempfile.adres * 32;
				at = i;
			}
		}
		int blokpamieci = (maindrive.drivespace[blokindeksowy]);//PIERWSZY ADRES Z BLOKU INDEKSOWEGO
		int adrespamieci = 32 * blokpamieci;

		int placetowrite = tempfile.towrite;

		int requiredblocks;
		if (rozmiar != 0)
		{
			requiredblocks = ceil((((double)rozmiar + 1) - (32 - placetowrite % 32)) / 32); //nieefektywne ale dzia³a	
		}

		if (rozmiar == 0)
		{
			requiredblocks = 0;
		}

		for (int i = 0; i < requiredblocks; i++)//Rezerwacja bloków danych i przypisanie do bloku indeksowego 
		{
			int f = hireFreeBlock(blockTable);
			if (f == -1)
			{
				std::cout << "\nE004:Insufficient drivespace\n" << std::endl;
				return false;
			}
			else
			{
				maindrive.drivespace[blokindeksowy + i + 1] = f;
			}
		}
		std::vector <char> towrite;
		int  it;
		it = 0;
		std::vector<int> blokidanych;
		while (maindrive.drivespace[blokindeksowy + it] != 0)
		{
			int f = maindrive.drivespace[blokindeksowy + it];
			blokidanych.push_back(f);
			it++;
		}
		std::copy(content.begin(), content.end(), std::back_inserter(towrite));
		int jmp = 0;
		int value;
		//Zapis do pliku
		for (int a = 0; a < towrite.size(); a++)
		{
			maindrive.drivespace[placetowrite] = towrite[a];
			placetowrite++;
			if (placetowrite % 32 == 0 && placetowrite != blokindeksowy)
			{

				if (jmp < blokidanych.size())
				{

					while (placetowrite > (blokidanych[jmp] * 32) + 31)//je¿eli zapisano wiêcej ni¿ 1 blok TO NIE BÊDZIE BRA£ 1 BLOKU DO ZAPISU TYLKO DALSZE 
					{
						jmp++;
						if (jmp < blokidanych.size())
						{

							break;
						}
					}
					placetowrite = 32 * blokidanych[jmp];
				}
				//Przesuwamy siê do nastêpnego bloku dyskowego
			}
		}
		maincatalogue[at].towrite = placetowrite;
		return true;
	}
	else
	{
		std::cout << "E005:Writing Error" << std::endl << std::endl;
		return false;
	}
}
bool Filesystem::readFile(const string &filename, int length)
{
	//GUI
	if (fileExists(filename))
	{
		openFile(filename);
		int blokindeksowy;
		File tempfile;
		int at;
		int read;
		for (int i = 0; i < maincatalogue.size(); i++)//PÊTLA SZUKAJ¥CA PLIKU W KATALOGU
		{
			if (maincatalogue[i].name == filename)
			{
				tempfile = maincatalogue[i];
				blokindeksowy = tempfile.adres * 32;//Adres bloku indeksowego
				at = i;//MIEJSCE W KATALOGU
				read = maincatalogue[i].toread;
			}
		}
		std::vector <int> toreadvector;
		string tempstr = "";
		//ODCZYT PLIKU
		for (int i = 0; i < 32; i++)//PÊTLA KTÓRA WYPE£NIA WEKTOR BLOKÓW Danych
		{
			if (maindrive.drivespace[blokindeksowy + i] == 0)
			{
				break;
			}
			else
			{
				int f = maindrive.drivespace[blokindeksowy + i] * 32;
				toreadvector.push_back(f);
			}
		}

		int blok;
		int sup;//Zmienna pomocnicza
		int c = 0;//Liczba odczytanych znaków
		int i = 0;//Czytam w i-tym bloku pliku
		while (c != length)
		{
			blok = toreadvector[i];
			if (read < ((i + 1) * 32) && (read >= (i * 32))) {//JE¯ELI ZMIENNA ODCZYTU ZNAJDUJE SIÊ W OBECNYM BLOKU TO //LE COŒ XD
				sup = read % 32;
				if (maindrive.drivespace[blok + sup] != 0)
				{

					tempstr = tempstr + maindrive.drivespace[blok + sup];
					read++;
					c++;
				}
				else
				{
					break;
				}
			}
			else //Je¿eli zmienna odczytu nie znajduje siê w tym bloku to zmieniamy blok
			{
				i++;
				if (i >= toreadvector.size())
				{
					break;
				}
			}
		}
		std::cout << "READ:" << tempstr;
		closeFile(filename);
		return true;
	}
	else
	{
		return false;
	}
}
bool Filesystem::readFile(const string &filename, const string &pid, int length, string &content)//LEPIEJ CHYBA PRZEZ REFERENCJE 
{
	if (verify(filename))
	{
		int blokindeksowy;
		File tempfile;
		int at;
		int read;
		for (int i = 0; i < maincatalogue.size(); i++)//PÊTLA SZUKAJ¥CA PLIKU W KATALOGU
		{
			if (maincatalogue[i].name == filename)
			{
				tempfile = maincatalogue[i];
				blokindeksowy = tempfile.adres * 32;//Adres bloku indeksowego
				at = i;//MIEJSCE W KATALOGU
				read = maincatalogue[i].toread;
			}
		}
		std::vector <int> toreadvector;
		string tempstr = "";
		//ODCZYT PLIKU
		for (int i = 0; i < 32; i++)//PÊTLA KTÓRA WYPE£NIA WEKTOR BLOKÓW Danych
		{
			if (maindrive.drivespace[blokindeksowy + i] == 0)
			{
				break;
			}
			else
			{
				int f = maindrive.drivespace[blokindeksowy + i] * 32;
				toreadvector.push_back(f);
			}
		}

		int blok;
		int sup;//Zmienna pomocnicza
		int c = 0;//Liczba odczytanych znaków
		int i = 0;//Czytam w i-tym bloku pliku
		while (c != length)
		{
			blok = toreadvector[i];
			if (read < ((i + 1) * 32) && (read >= (i * 32))) {//JE¯ELI ZMIENNA ODCZYTU ZNAJDUJE SIÊ W OBECNYM BLOKU TO //LE COŒ XD
				sup = read % 32;
				if (maindrive.drivespace[blok + sup] != 0)
				{

					tempstr = tempstr + maindrive.drivespace[blok + sup];
					read++;
					c++;
				}
				else
				{
					break;
				}
			}
			else //Je¿eli zmienna odczytu nie znajduje siê w tym bloku to zmieniamy blok
			{
				i++;
				if (i >= toreadvector.size())
				{
					break;
				}
			}
		}
		content = tempstr;
		maincatalogue[at].toread = read;
		return true;
	}
	else
	{
		return false;
	}
}
bool Filesystem::deleteFile(const string &filename)
{
	//GUI
	if (guiverify(filename))
	{
		openFile(filename);
		File tempfile;
		int at;
		int indexblock;
		for (int i = 0; i < maincatalogue.size(); i++)//WYSZUKUJE PLIK I POBIERAM PODSTAWOWE DANE
		{
			if (maincatalogue[i].name == filename)
			{
				tempfile = maincatalogue[i];
				indexblock = tempfile.adres * 32;
				at = i;
			}
		}
		std::vector <int> removalvector;
		for (int i = 0; i < 32; i++)
		{
			if ((int)maindrive.drivespace[indexblock + i] == 0)
			{
				break;
			}
			else
			{
				removalvector.push_back((int)maindrive.drivespace[indexblock + i]);
				maindrive.drivespace[indexblock + i] = 0;
			}
		}

		//USUWANIE PLIKU Z DYSKU 
		int a = -1;
		int b = 0;
		for (int i = 0; i < removalvector.size(); i++)
		{
			int blocknumber = removalvector[i]; //Numer obecnie usuwanego bloku dyskowego
			int currentblock = 32 * blocknumber;//Adres tego bloku
			for (int j = 31; j > -1; j--)
			{
				maindrive.drivespace[currentblock + j] = 0;//ZERUJE WSZYSTKIE POLA DANYCH BLOKU
			}
			blockTable[blocknumber] = false;//PO WYZEROWANIU DANYCH BLOK JEST ZWALNIANY
			maindrive.drivespace[i + indexblock] = 0; //W BLOKU INDEKSOWYM USUWAMY BLOK DYSKOWY
			//USUWA W WEKTORZE
		}
		blockTable[tempfile.adres] = false;//ZWOLNIENIE BLOKU DYSKOWEGO
		maincatalogue[at].sem.delete_sem();//ZWALNIAM KOLEJKÊ
		maincatalogue.erase(maincatalogue.begin() + at);
		openfiletable.erase(filename);
		return true;
	}
	else
	{
		return false;
	}


}
bool Filesystem::deleteFile(const string &filename, const string &pid)
{
	if (verify(filename))
	{
		File tempfile;
		int at;
		int indexblock;
		for (int i = 0; i < maincatalogue.size(); i++)//WYSZUKUJE PLIK I POBIERAM PODSTAWOWE DANE
		{
			if (maincatalogue[i].name == filename)
			{
				tempfile = maincatalogue[i];
				indexblock = tempfile.adres * 32;
				at = i;
			}
		}
		std::vector <int> removalvector;
		for (int i = 0; i < 32; i++)
		{
			if ((int)maindrive.drivespace[indexblock + i] == 0)
			{
				break;
			}
			else
			{
				removalvector.push_back((int)maindrive.drivespace[indexblock + i]);
				maindrive.drivespace[indexblock + i] = 0;
			}
		}

		//USUWANIE PLIKU Z DYSKU 
		int a = -1;
		int b = 0;
		for (int i = 0; i < removalvector.size(); i++)
		{
			int blocknumber = removalvector[i]; //Numer obecnie usuwanego bloku dyskowego
			int currentblock = 32 * blocknumber;//Adres tego bloku
			for (int j = 31; j > -1; j--)
			{
				maindrive.drivespace[currentblock + j] = 0;//ZERUJE WSZYSTKIE POLA DANYCH BLOKU
			}
			blockTable[blocknumber] = false;//PO WYZEROWANIU DANYCH BLOK JEST ZWALNIANY
			maindrive.drivespace[i + indexblock] = 0; //W BLOKU INDEKSOWYM USUWAMY BLOK DYSKOWY
			//USUWA W WEKTORZE
		}
		blockTable[tempfile.adres] = false;//ZWOLNIENIE BLOKU DYSKOWEGO
		maincatalogue[at].sem.delete_sem();//ZWALNIAM KOLEJKÊ
		maincatalogue.erase(maincatalogue.begin() + at);
		openfiletable.erase(filename);
	}

	return true;
}
bool Filesystem::verify(const string &filename)
{
	if (!fileExists(filename))
	{
		std::cout << "E006:File does not exist" << std::endl << std::endl;
		return false;
	}
	else
	{
		for (int i = 0; i < maincatalogue.size(); i++)
		{
			if (maincatalogue[i].name == filename)
			{
				if (maincatalogue[i].isOpen == true)
				{

					return true;
				}
				else
				{
					std::cout << "E010:File is closed" << std::endl << std::endl;
					return false;
				}
			}
		}
	}
}
bool Filesystem::guiverify(const string &filename)
{
	if (!fileExists(filename))
	{
		std::cout << "E006:File does not exist" << std::endl << std::endl;
		return false;
	}
	else
	{
		for (int i = 0; i < maincatalogue.size(); i++)
		{
			if (maincatalogue[i].name == filename)
			{
				if (maincatalogue[i].isOpen == true)
				{
					std::cout << "E010:File is opened" << std::endl << std::endl;
					return false;
				}
				else
				{

					return true;
				}
			}
		}
	}
}
void Filesystem::displaydrivecontent()
{
	std::cout << std::endl << std::endl;
	for (int i = 0; i < 32; i++)
	{
		displayblock(i);
	}
}
void Filesystem::displaycatalogue()
{
	std::cout << "\nFilename \t\tAdress\t\tRead characters\t\tWriting at\t\tSemaphore value";
	for (auto e : maincatalogue)
	{
		std::cout << std::endl << e.name << "\t\t\t" << e.adres << "\t\t\t" << e.toread << "\t\t\t" << e.towrite << "\t\t\t" << e.sem.value_sem();
	}
	std::cout << std::endl << std::endl;
}
void Filesystem::displayoft()
{
	std::cout << "Filename\t\tProcess" << std::endl;
	for (auto e : openfiletable)
	{
		std::cout << e.first << "\t\t\t" << e.second << std::endl;
	}
	std::cout << std::endl << std::endl;
}
void Filesystem::displayvector()
{
	std::cout << "Bitvector:\n";
	for (int i = 0; i < blockTable.size(); i++)
	{
		std::cout << blockTable[i] << " ";
	}

	std::cout << std::endl << std::endl;
}
void Filesystem::displayblock(int block)
{
	if (block < 32)
	{
		std::cout << "|" << block << "|";
		for (int i = block * 32; i < (block + 1) * 32; i++)
		{
			if (maindrive.drivespace[i] < 32 && maindrive.drivespace[i] != 0)
			{
				int f = maindrive.drivespace[i];
				std::cout << f << " ";
			}
			else
			{
				std::cout << maindrive.drivespace[i] << " ";
			}
		}
		std::cout << std::endl << std::endl;
	}
	else
	{
		std::cout << "E011:Number exceeds total blocks number";
	}

}
void Filesystem::printqueue(string &filename)
{
	for (int i = 0; i < maincatalogue.size(); i++)//PÊTLA SZUKAJ¥CA PLIKU W KATALOGU
	{
		if (maincatalogue[i].name == filename)
		{
			maincatalogue[i].sem.print_queue();
			break;
		}
	}
}