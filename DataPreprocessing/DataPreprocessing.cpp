#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

#define NIESKONCZONOSC 10000;

using namespace std;

struct plik1
{
	int kolumna;
	double liczba;
};

struct plik2
{
	int kolumna;
	string wyraz;
};

vector <plik1> Read();

void symbols(vector <plik1> &data);

vector <double> min(vector <plik1> &data);

vector <double> maks(vector <plik1> &data);

void rozne(vector <plik1> &data);

vector <double> srednia(vector <plik1> &data);

vector <double> odchylenie_standardowe(vector <plik1> &data, vector <double> &mean);

void destroy(vector <plik1> &data);

void repair(vector <plik1> &data, vector <double> &mean);

vector <plik1> normalize(vector <plik1> &data, vector <double> &minimum, vector <double> &maksimum, int a, int b);

vector <plik1> stand(vector <plik1> &data, vector <double> &srednia, vector <double> &odchylenie);

vector <plik2> Read_csv();

vector <plik2> Dummy(vector <plik2> &data);

void Reduce(vector <plik2> &data);

//################################################# MAIN
int main()
{
	vector <plik1> data =  Read();

	//3a, 3b
	symbols(data);

	//---3c
	vector <double> minimum = min(data);

	for (int i = 0; i < minimum.size(); i++)
		cout << "Wartosc minimalna atrybutu " << i+1 << ": " << minimum[i] << endl;
	cout << endl;

	vector <double> maksimum = maks(data);

	for (int i = 0; i < maksimum.size(); i++)
		cout << "Wartosc maksymalna atrybutu " << i+1 << ": " << maksimum[i] << endl;
	cout << endl;

	//---3d/3e
	rozne(data);


	//---3f
	vector <double> mean = srednia(data);

	vector <double> odchylenia = odchylenie_standardowe(data, mean);

	for (int i = 0; i < odchylenia.size(); i++)
		cout << "Odchylenie standardowe dla atrybutu " << i + 1 << ": " << odchylenia[i] << endl;

	//---4a
	destroy(data);

	repair(data, mean);

	//---4b
	minimum = min(data);
	maksimum = maks(data);

	vector <plik1> normalized1 = normalize(data, minimum, maksimum, -1, 1);

	cout << endl << "Kilka wartosci po normalizacji do przedzialu <-1, 1>:" << endl;
	for (int i = 0; i < 20; i++)
	{
		if (normalized1[i].kolumna == 8)
			continue;
		cout << normalized1[i].liczba << endl;
	}

	normalized1 = normalize(data, minimum, maksimum, 0, 1);

	cout << endl << "Kilka wartosci po normalizacji do przedzialu <0, 1>:" << endl;
	for (int i = 0; i < 20; i++)
	{
		if (normalized1[i].kolumna == 8)
			continue;
		cout << normalized1[i].liczba << endl;
	}

	normalized1 = normalize(data, minimum, maksimum, -10, 10);

	cout << endl << "Kilka wartosci po normalizacji do przedzialu <-10, 10>:" << endl;
	for (int i = 0; i < 20; i++)
	{
		if (normalized1[i].kolumna == 8)
			continue;
		cout << normalized1[i].liczba << endl;
	}

	//---4c
	mean = srednia(data);
	odchylenia = odchylenie_standardowe(data, mean);
	vector <plik1> po_standaryzacji = stand(data, mean, odchylenia);

	cout << endl;
	cout << "Poczatkowe wartosci po standaryzacji:" << endl;
	for (int i = 0; i < 20; i++)
		cout << po_standaryzacji[i].liczba << endl;


	//---4d
	cout << endl << endl << "Odczyt z pliku csv..." << endl;

	vector <plik2> data2 = Read_csv();

	vector <plik2> data3 = Dummy(data2);

	Reduce(data3);
}
//################################################# MAIN

vector <plik1> Read()
{
	vector <plik1> content;
	
	fstream file;
	file.open("diabetes.txt", ios::in);
	
	if (file.good())
	{
		string line; //linia z pliku
		string value; //pojedyncza liczba z pliku

		while (!file.eof())
		{
			getline(file, line);

			int licznik = 0;

			for (int i=0; i<=line.length(); i++)
			{
				if (line[i] == ' ' ||  line[i] == '\0')
				{
					double save = stod(value);

					plik1 struktura;
					struktura.kolumna = licznik;
					struktura.liczba = save;

					content.push_back(struktura);

					value = "";
					licznik++;
				}
				else
				{
					value += line[i];
				}
			}
		}
	}
	else
	{
		cout << "Blad pliku. Sprawdz katalog z projektem" << endl;
	}

	return content;
}

void symbols(vector <plik1> &data)
{

	//---3a

	vector <double> symbole;
	int licznik = 0;

	for (int i=0; i<data.size(); i++)
	{
		if (data[i].kolumna == 8)
		{
			licznik = 0;

			if (symbole.size() == 0)
			{
				symbole.push_back(data[i].liczba);
			}
			else
			{
				for (int j = 0; j < symbole.size(); j++)
				{
					if (symbole[j] == data[i].liczba)
						break;
					else if (licznik == (symbole.size() - 1))
					{
						symbole.push_back(data[i].liczba);
						break;
					}
					else
					{
						licznik++;
						continue;
					}
				}
			}
		}
	}

	cout << "Dostepne symbole klas decyzyjnych:" << endl;
	for (int i = 0; i < symbole.size(); i++)
		cout << i+1 << ": " << symbole[i] << endl;
	cout << endl;

	//---3b

	vector <double> sym;
	licznik = 0;

	for (int i=0; i<data.size(); i++)
	{
		if (data[i].kolumna == 8)
			sym.push_back(data[i].liczba);
	}

	for (int i = 0; i < symbole.size(); i++)
	{
		for (int j = 0; j < sym.size(); j++)
		{
			if (sym[j] == symbole[i])
				licznik++;
		}
		cout << "Liczba wystapien symbolu '" << symbole[i] << "': " << licznik << endl;
		licznik = 0;
	}
	cout << endl;
}

//3c
vector <double> min(vector <plik1> &data)
{
	vector <double> mini;
	double mina = NIESKONCZONOSC;

	for (int i=0; i<8; i++)
	{
		for (int j=0; j<data.size(); j++)
		{
			if (data[j].kolumna == i)
			{
				if (data[j].liczba < mina)
					mina = data[j].liczba;
			}
		}
		mini.push_back(mina);
		mina = NIESKONCZONOSC;
	}
	return mini;
}

vector <double> maks(vector <plik1> &data)
{
	vector <double> maxi;
	double maksio = 0;

	for (int i=0; i<8; i++)
	{
		for (int j=0; j<data.size(); j++)
		{
			if (data[j].kolumna == i)
			{
				if (data[j].liczba > maksio)
					maksio = data[j].liczba;
			}
		}
		maxi.push_back(maksio);
		maksio = 0;
	}
	return maxi;
}

//3d, 3e
void rozne(vector <plik1> &data)
{
	vector <double> rozne_wart;
	vector < vector<double> > lista_wart;

	bool flaga = true;
	int licznik = 0;

	for (int i=0; i<8; i++)
	{
		for (int j=0; j<data.size(); j++)
		{
			if (data[j].kolumna == i)
			{
				if (licznik == 0)
				{
					rozne_wart.push_back(data[j].liczba);
					licznik++;
					continue;
				}
				for (int k=0; k<rozne_wart.size(); k++)
				{
					if (data[j].liczba == rozne_wart[k])
					{
						flaga = false;
						break;
					}
				}
				if (flaga == true)
					rozne_wart.push_back(data[j].liczba);
			}
			flaga = true;
		}

		//cout << endl << "Liczba roznych wartosci dla atrybutu " << i+1 << ": " << rozne_wart.size() << endl;
		
		//cout << endl << "Lista roznych dostepnych wartosci dla atrubutu " << i+1 << ":" << endl;
		//for (int l = 0; l < rozne_wart.size(); l++)
		//	cout << rozne_wart[l] << endl;

		rozne_wart.clear();
		licznik = 0;
	}
}

//3f
vector <double> srednia(vector <plik1> &data)
{
	vector <double> mean_values;
	double attribute_values = 0;
	int licznik = 0;

	for (int i=0; i<8; i++)
	{
		for (int j=0; j<data.size(); j++)
		{
			if (data[j].kolumna == i)
			{
				attribute_values += data[j].liczba;
				licznik++;
			}
		}

		mean_values.push_back(attribute_values / licznik);
		attribute_values = 0;
		licznik = 0;
	}

	return mean_values;
}

vector <double> odchylenie_standardowe(vector <plik1> &data, vector <double> &mean)
{
	vector <double> odchylenie_s;
	int licznik = 0;
	double attribute_values = 0;
	double wariancja;

	for (int i=0; i<8; i++)
	{
		for (int j=0; j<data.size(); j++)
		{
			if (data[j].kolumna == i)
			{
				attribute_values += pow((data[j].liczba - mean[i]), 2);
				licznik++;
			}
		}

		wariancja = attribute_values / licznik;
		odchylenie_s.push_back(sqrt(wariancja));

		attribute_values = 0;
		licznik = 0;
	}

	return odchylenie_s;
}

//4a
void destroy(vector <plik1> &data)
{
	int how_many3 = data.size() / 9;
	int how_many2 = how_many3 * 8;
	int how_many = how_many2 / 10;

	int licznik = 0;
	int nr_wart;
	double  nieznana = -100;

	srand(time(NULL));
	
	while (licznik != how_many)
	{
		//srand(time(NULL));
		nr_wart = rand() % data.size();
		if (data[nr_wart].kolumna == 8)
			continue;
		else if (data[nr_wart].liczba == nieznana)
			continue;
		else
		{
			licznik++;
			data[nr_wart].liczba = nieznana;
		}

	}
}

void repair(vector <plik1> &data, vector <double> &mean)
{
	double nieznana = -100;
	for (int i=0; i<data.size(); i++)
	{
		if (data[i].liczba == nieznana)
			data[i].liczba = mean[data[i].kolumna];
	}
}

//4b
vector <plik1> normalize(vector <plik1> &data, vector <double> &minimum, vector <double> &maksimum, int a, int b)
{
	vector <plik1> norm = data;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < norm.size(); j++)
		{
			if (norm[j].kolumna == i)
			{
				norm[j].liczba = (((norm[j].liczba - minimum[i]) * (b - a)) / (maksimum[i] - minimum[i])) + a;
			}
		}
	}

	return norm;
}

//4c
vector <plik1> stand(vector <plik1> &data, vector <double> &srednia, vector <double> &odchylenie)
{
	vector <plik1> standard = data;

	for (int i=0; i<standard.size(); i++)
	{
		if (standard[i].kolumna == 8)
		{
			continue;
		}
		else
		{
			standard[i].liczba = (standard[i].liczba - srednia[standard[i].kolumna]) / odchylenie[standard[i].kolumna];
		}
	}

	return standard;
}

//4d
vector <plik2> Read_csv()
{
	vector <plik2> content;

	fstream file;
	file.open("churn_modelling.txt", ios::in);

	if (file.good())
	{
		string line; //linia z pliku
		string value; //pojedynczy wyraz z pliku

		while (!file.eof())
		{
			getline(file, line);

			int licznik = 0;

			for (int i = 0; i <= line.length(); i++)
			{
				if (line[i] == ',' || line[i] == '\0')
				{
					plik2 struktura;
					struktura.kolumna = licznik;
					struktura.wyraz = value;

					content.push_back(struktura);

					value = "";
					licznik++;
				}
				else
				{
					value += line[i];
				}
			}
		}
	}
	else
	{
		cout << "Blad pliku. Sprawdz katalog z projektem" << endl;
	}

	return content;
}

vector <plik2> Dummy(vector <plik2> &data)
{
	vector <plik2> variables;
	plik2 struktura;

	for (int i=0; i<data.size(); i++)
	{
		if (data[i].wyraz == "France")
		{
			struktura.kolumna = 1;
			struktura.wyraz = "1";
			variables.push_back(struktura);
		}
		else if (data[i].wyraz == "Spain")
		{
			struktura.kolumna = 2;
			struktura.wyraz = "1";
			variables.push_back(struktura);
		}
		else if (data[i].wyraz == "Germany")
		{
			struktura.kolumna = 3;
			struktura.wyraz = "1";
			variables.push_back(struktura);
		}
		else continue;
	}
	
	return variables;
}

void Reduce(vector <plik2> &data)
{
	cout << endl << "Poczatkowe dane 'Geography' w postaci Dummy variables po redukcji nadmiarowosci:" << endl << endl;
	cout << "France Spain" << endl;
	for (int i=0; i<20; i++)
	{
		if (data[i].kolumna == 1)
			cout << "  " << data[i].wyraz << "      0" << endl;
		else if (data[i].kolumna == 2)
			cout << "  0      " << data[i].wyraz << endl;
		else cout << "  0      0" << endl;
	}
}