#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <vector>
#include <conio.h>
using namespace std;

struct Kategoria {	// Funkcja definiuje strukture danych Kategoria, kategoria sklada sie z pola nazwa i pola hasla.
	string nazwa;
	vector<string> hasla;
};
void wczytaj_plik(const string& nazwa_pliku, vector<string>& kategorie, vector<vector<string>>& hasla) {
	ifstream plik(nazwa_pliku);	
	string linia;				// Funkcja do wczytywania kategorii i hasel z pliku tekstowego.	
	string kategoria = "";
	vector<string> lista_hasel;

	while (getline(plik, linia)) {
		stringstream ss(linia);   
		string slowo;
		while (ss >> slowo) {
			if (isupper(slowo[0])) {		// Funckja ta wczytuje i tworzy wektory z kategorii i hase³.
				if (kategoria != "") {
					kategorie.push_back(kategoria);
					hasla.push_back(lista_hasel);
					kategoria = "";
					lista_hasel.clear();
				}
				kategoria = slowo;
			}
			else {
				lista_hasel.push_back(slowo);
			}
		}
	}
	if (kategoria != "") {
		kategorie.push_back(kategoria);
		hasla.push_back(lista_hasel);
	}										// Funkcja ta dodaje ,,kategoria" dodaje do ,,Kategorii" oraz ,,hasla" do "lista_hasel".
	if (!kategoria.empty()) {
		kategorie.push_back(kategoria);
		hasla.push_back(lista_hasel);
	}
}
string wybierz_haslo(const vector<string>& lista_hasel) {
	if (lista_hasel.empty()) {				// Funkcja ta pobiera wektor "lista_hasel" i wybiera losowo jedno haslo z listy (jesli brak hasel informuje o tym).
		return "Lista hasel jest pusta";
	}						
	int index = rand() % lista_hasel.size();
	return lista_hasel[index];
}
int zakrec_kolem() {
	const int segmenty_kola = 12;
	const int segment_BANKRUT = 0;
	const int minimalna_kwota = 100;
	const int maksymalna_kwota = 2500;
	int rozmiar_segmentu = 360 / segmenty_kola;		// Funkcja do krecenia kolem fortuny.
	int kat = rand() % 360;	
	int segment = kat / rozmiar_segmentu;
	if (segment == segment_BANKRUT) {
		return -1;
	}
	else {
		return rand() % (maksymalna_kwota - minimalna_kwota + 1) + minimalna_kwota;
	}
}
void rozegraj_gre(const vector<string>& kategorie, const vector<vector<string>>& hasla) {
	cout << "  Podaj swoje imie: ";
	string gracz;
	cin >> gracz;
	ofstream plik_wyjsciowy;				// Tworzenie pliku wyjsciowego, powitanie gracza i zapoznanie go z instrukcja obslugi.
	plik_wyjsciowy.open("w.txt", ios::app);				
	cout << "\n\n****************Witaj " << gracz << " w Kole Fortuny!******************\n\n" << endl;
	cout << "W tym teleturnieju bedziesz musial odgadnac haslo z podanej przez nas kategorii." << endl;
	cout << "Za kazda odgadnieta litere zostaniesz nagrodzony wylosowana przez ciebie kwota." << endl;
	cout << "Natomiast jesli odpowiesz zle, utracisz czesc swoich zarobionych pieniedzy." << endl;
	cout << "UWAGA Jesli wylosujesz pole BANKRUT odpadniesz z teleturnieju, a pieniadze przepadna.\n\n" << endl;

	int liczba_kategorii = kategorie.size();
	if (liczba_kategorii == 0) {
		cerr << "Blad programu: Lista kategorii jest pusta." << endl;
		throw "Blad programu: Lista kategorii jest pusta.";
	}
	int kategoria_index = rand() % liczba_kategorii;		// Funkcje sprawdzajace zawartosc pliku wejsciowego (informacyjne).
	string kategoria = kategorie[kategoria_index];
	vector<string> lista_hasel = hasla[kategoria_index];
	if (lista_hasel.empty()) {
		cerr << "Blad programu: Lista hasel jest pusta." << endl;
		throw "Blad programu: Lista hasel jest pusta.";
	}
	string haslo = wybierz_haslo(lista_hasel);
	string odgadywanie(haslo.length(), '_');			// Funkcja tworzy pole do wpisania za pomoca "_"(dlugosc hasla = ilosc "_"), oraz inicjiuje pule pieniezna, bledne odpowiedzi, odgadniete przez gracza litery.
	int pula_pieniedzy = 0;
	int liczba_blednych_odpowiedzi = 0;
	int maksymalna_liczba_blednych_odpowiedzi = haslo.length() / 2;
	vector<char> odgadniete_litery;

	while (liczba_blednych_odpowiedzi < maksymalna_liczba_blednych_odpowiedzi) {
		cout << "Kategoria: " << kategoria << endl;
		cout << "Haslo: " << odgadywanie << endl;		// Ta funkcja jest zapentlona puki gracz nie przekroczy liczby dozwolonych bledow. Wywoluje funkcje "zakrec_kolem"
		cout << "Podane przez ciebie litery: ";
		for (char c : odgadniete_litery) {
			cout << c << " ";
		}
		cout << endl;
		cout << "Krecisz Kolem Fortuny... ";
		cin.ignore();
		int pieniadze = zakrec_kolem();

		if (pieniadze == -1) {		// Funkcja ta informuje gracza o pechowym wylosowaniu pola BANKRUT, zeruje pieniadze i zakancza teleturniej.
			pula_pieniedzy = 0;
			cout << "Ups! Wyladowales na BANKRUCIE odpadasz z teleturnieju!!!" << endl;
			cout << "Pula pieniedzy: " << pula_pieniedzy << " zl" << endl;
			return;
		}
		else {
			cout << "Zarobiles " << pieniadze << " zl w tej rundzie!" << endl;	// Jesli jednak gracz nie trafi na pole BANKRUT przyznawana mu jest losowa kwota i moze on grac dalej.
			pula_pieniedzy += pieniadze;
		}
		cout << "Podaj litere: ";
		char litera;
		cin >> litera;			// Funkcja ta prosi gracza o podanie litery a nastepnie informuje go czy niepoprawnie odpowiedzial, podal ta sama litere. 
		cout << "\n";
		if (isupper(litera)) {
			cout << "Kategoria: " << kategoria << endl;
			cout << "Haslo: " << odgadywanie << endl;
			cout << "Podane przez ciebie litery: ";				
			for (char c : odgadniete_litery) {
				cout << c << " ";
			}
			cout << endl;
			cout << "Przykro mi, to nie jest dobra odpowiedz. Podaj nowa litere." << endl;
			continue;
		}
		if (odgadniete_litery.size() > 0 && find(odgadniete_litery.begin(), odgadniete_litery.end(), litera) != odgadniete_litery.end()) {
			cout << "Kategoria: " << kategoria << endl;
			cout << "Haslo: " << odgadywanie << endl;
			cout << "Podane przez ciebie litery: ";
			for (char c : odgadniete_litery) {
				cout << c << " ";
			}
			cout << endl;
			cout << "\nJuz podales ta litere, sprobuj jeszcze raz!" << endl;
			continue;
		}
		odgadniete_litery.push_back(litera);
		bool znajdz_litere = false;
		for (int i = 0; i < haslo.length() + 1; i++) {
			if (haslo[i] == litera) {		// Funkcja ta dodaje podana przez gracza litere do wektora i sprawdza czy litera ta znajduje sie w hasle.
				odgadywanie[i] = litera;
				znajdz_litere = true;
			}
		}
		if (znajdz_litere) {
			cout << "Kategoria: " << kategoria << endl;
			cout << "Haslo: " << odgadywanie << endl;
			cout << "Podane przez ciebie litery: ";		// Jesli gracz odgadl litere wyswietla sie komunikat "Dobrze!" 
			for (char c : odgadniete_litery) {
				cout << c << " ";
			}
			cout << endl;
			cout << "Dobrze!\n" << endl;
		}
		else {
			liczba_blednych_odpowiedzi++;
			cout << "Kategoria: " << kategoria << endl;
			cout << "Haslo: " << odgadywanie << endl;
			cout << "Podane przez ciebie litery: ";
			for (char c : odgadniete_litery) {
				cout << c << " ";
			}
			cout << endl;		// Funkcja ta informuje gracza ze odpowiedzial niepoprawnie a nastepnie odlicza mu kare pieniezna z puli pieniedzy.
			cout << "Przykro mi, ta litera nie znajduje sie w hasle." << endl;
			int strata = pula_pieniedzy / (2 * maksymalna_liczba_blednych_odpowiedzi);
			cout << "Straciles " << strata << " zl.\n" << endl;
			pula_pieniedzy -= strata;
		}

		if (odgadywanie == haslo) {
			cout << "Wysmienicie, odgadles haslo!" << endl;
			plik_wyjsciowy << "Gratulacje " << gracz << "!";

			plik_wyjsciowy << "\n\nPodane przez ciebie litery ";
			for (char c : odgadniete_litery) {
				plik_wyjsciowy << c << " ";								// Funkcja ta informuje gracza o zakonczeniu gry z powodzeniem i zapisuje statystyki do pliku wyjsciowego "w.txt".
			}
			plik_wyjsciowy << "Kategoria: " << kategoria << endl;
			plik_wyjsciowy << "Haslo to: " << haslo << endl;
			plik_wyjsciowy << "Odgadles haslo prawidlowo." << endl;
			plik_wyjsciowy << "Pula wygranej: " << pula_pieniedzy << " zl" << endl << endl << endl << endl;
			cout << "Pula wygranej: " << pula_pieniedzy << " zl" << endl;
			return;
		}
	}
	plik_wyjsciowy << "Imie gracza: " << gracz;
	plik_wyjsciowy << "\n\nPodane przez ciebie litery: ";
	for (char c : odgadniete_litery) {
		plik_wyjsciowy << c << " ";
	}
	plik_wyjsciowy << "Kategoria: " << kategoria << endl;		// Ta funkcja zapisuje statystyki gracza po zakonczeniu gry (porazka w skutek wyczerpania sie prob).
	plik_wyjsciowy << "Haslo to: " << haslo << endl;			
	plik_wyjsciowy << "Pula pieniedzy: " << pula_pieniedzy << " zl" << endl << endl << endl << endl;
	cout << "Przykro mi, wyczerpaly ci sie proby." << endl;
	cout << "Haslo to: " << haslo << endl;
	cout << "Pula pieniedzy: " << pula_pieniedzy << " zl" << endl;
	plik_wyjsciowy.close();
}
int main() {
	srand(time(0));
	vector<string> kategorie;
	vector<vector<string>> hasla;	// Funkcja glowna deklarujaca wszystkie zmienne, wczytujaca plik "i.txt" (zapisuje je do wektorw) i rozpoczynajaca gre W KOLO FORTUNY!!!
	wczytaj_plik("i.txt", kategorie, hasla);
	rozegraj_gre(kategorie, hasla);
	return 0;
}