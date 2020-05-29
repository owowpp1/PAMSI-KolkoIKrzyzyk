
#include "Kolko_i_krzyzyk.hh"
#define p_w pole.wiersz
#define p_k pole.kolumna
using namespace std;
//Konstruktor bezparametryczny tworzący standardową planszę 3x3
KiK::KiK(){
    plansza= new int*[3];
    for(int i=0; i<3; i++){
        plansza[i]=new int[3];
        for(int j=0; j<3; j++){
            plansza[i][j]=0;
        }
    }
    wygranko=pow(2, 3); //pomocnicza wartość oznaczająca iloczyn, jaki należy osiągnąć w rzędzie
    wymiar=3;           //by zwyciężyło kółko, nie wyliczając przy tym potęgi przy każdym porównaniu.
}
//Konstruktor parametryczny pozwalający utworzyć planszę o rozmiarze wym x wym
KiK::KiK(int wym){
    plansza= new int*[wym];
    for(int i=0; i<wym; i++){
        plansza[i]=new int[wym];
        for(int j=0; j<wym; j++){
            plansza[i][j]=0;
        }
    }
    wygranko=pow(2, wym); //pomocnicza wartość oznaczająca iloczyn, jaki należy osiągnąć w rzędzie
    wymiar=wym;           //by zwyciężyło kółko, nie wyliczając przy tym potęgi przy każdym porównaniu.
}
//Metoda sprawdzająca, czy na planszy pozostały jeszcze wolne miejsca
bool KiK::czykoniec(){

    for(int i=0; i<wymiar; i++){
        for(int j=0; j<wymiar; j++){
            if (plansza[i][j]==0) return false;
        }
    }
    return true;
}
//Metoda wypisująca planszę do terminala
void KiK::wypisz(){
    cout<<"╔";
    for(int i=0; i<2*wymiar+1; i++) cout<<"═";
    cout<<"╗\n";
    for(int i=0; i<wymiar; i++){
        cout<<"║ ";
        for(int j=0; j<wymiar; j++){
            if(plansza[i][j]==0) cout<<"- ";
            if(plansza[i][j]==1) cout<<"x ";
            if(plansza[i][j]==2) cout<<"o ";
        }
        cout<<"║\n";
    }
    cout<<"╚";
    for(int i=0; i<2*wymiar+1; i++) cout<<"═";
    cout<<"╝\n";
}
//Metoda zerująca planszę w celu ponownego rozpoczęcia gry.
void KiK::wyzeruj(){

    for(int i=0; i<wymiar; i++){
        for(int j=0; j<wymiar; j++){
            plansza[i][j]=0;
        }
    }
}
//Metoda analizująca obecny stan planszy i zwracająca odpowiadające jemu wartości:
//0, gdy nie odnotowano zwycięstwa
//10, gdy zwycięża krzyżyk (MAX)
//-10, gdy zwycięża kółko (MIN)
int KiK::analiza(){
    int iloczyn=1;                                  //funkcja wymnaża wartości w rzędach, kolumnach i przekątnych
    for(int i = 0; i < wymiar; i++) {               //Gdyż poszukiwane są konkretne iloczyny: 0-rząd niepełny
        iloczyn=1;                                  //1-zwycięża krzyżyk, 2^wymiar- zwycięża kółko
        //w poziomie
        for(int j=0; j<wymiar; j++){
            iloczyn*=plansza[i][j];
        }
        if(iloczyn==1)          return  10;
        if(iloczyn==wygranko)   return -10;
        //w pionie
        iloczyn=1;
        for(int j=0; j<wymiar; j++){
            iloczyn*=plansza[j][i];
        }
        if(iloczyn==1)          return  10;
        if(iloczyn==wygranko)   return -10;
	}
	iloczyn=1;
	for(int i=0; i<wymiar; i++){
        int j=i;
        iloczyn*=plansza[i][j];
        j++;
	}
    if(iloczyn==1)          return  10;
    if(iloczyn==wygranko)   return -10;
    iloczyn=1;
    for(int i=wymiar-1; i>0; i--){
        int j=0;
        iloczyn*=plansza[i][j];
        j++;
	}
    if(iloczyn==1)          return  10;
    if(iloczyn==wygranko)   return -10;

	return 0;
}
//Funkcja utworzona, by ocenić wartość ruchu po przekroczeniu dopuszczalnej głębokości rekursji
//Przydziela ona wartość 5, gdzy AI postawi swój symbol obok innego swojego symbolu
//Nie przynosi to jednak zamierzonego efektu, gdyż powoduje to zapełnianie planszy po kolei.
int KiK::analiza_somsiad(ruch pole){

    if(p_w>0&&p_k>0&&p_w<wymiar-1&&p_k<wymiar-1){
            if(plansza[p_w][p_k]==plansza[p_w-1][p_k-1]||
               plansza[p_w][p_k]==plansza[p_w-1][p_k]||
               plansza[p_w][p_k]==plansza[p_w-1][p_k+1]||
               plansza[p_w][p_k]==plansza[p_w][p_k-1]||
               plansza[p_w][p_k]==plansza[p_w][p_k+1]||
               plansza[p_w][p_k]==plansza[p_w+1][p_k-1]||
               plansza[p_w][p_k]==plansza[p_w+1][p_k]||
               plansza[p_w][p_k]==plansza[p_w+1][p_k+1]){
                    if(plansza[p_w][p_k]==1) return 5;
                    if(plansza[p_w][p_k]==2) return -5;
            }
    }
    else{
        if(p_w==0&&p_k==0){
            if(plansza[p_w][p_k]==plansza[p_w+1][p_k+1]||
               plansza[p_w][p_k]==plansza[p_w+1][p_k]||
               plansza[p_w][p_k]==plansza[p_w][p_k+1]){
                if(plansza[p_w][p_k]==1) return 5;
                if(plansza[p_w][p_k]==2) return -5;
            }
        }
        if(p_w==0&&p_k>0&&p_k<wymiar-1){
            if(plansza[p_w][p_k]==plansza[p_w][p_k-1]||
               plansza[p_w][p_k]==plansza[p_w][p_k+1]||
               plansza[p_w][p_k]==plansza[p_w+1][p_k-1]||
               plansza[p_w][p_k]==plansza[p_w+1][p_k]||
               plansza[p_w][p_k]==plansza[p_w+1][p_k+1]){
                if(plansza[p_w][p_k]==1) return 5;
                if(plansza[p_w][p_k]==2) return -5;
            }
        }
        if(p_w==0&&p_k==wymiar-1){
            if(plansza[p_w][p_k]==plansza[p_w][p_k-1]||
               plansza[p_w][p_k]==plansza[p_w+1][p_k-1]||
               plansza[p_w][p_k]==plansza[p_w+1][p_k]){
                if(plansza[p_w][p_k]==1) return 5;
                if(plansza[p_w][p_k]==2) return -5;
            }
        }
        if(p_k==0&&p_w==wymiar-1){
            if(plansza[p_w][p_k]==plansza[p_w-1][p_k]||
               plansza[p_w][p_k]==plansza[p_w-1][p_k+1]||
               plansza[p_w][p_k]==plansza[p_w][p_k+1]){
                if(plansza[p_w][p_k]==1) return 5;
                if(plansza[p_w][p_k]==2) return -5;
            }
        }
        if(p_k==0&&p_w>0&&p_w<wymiar-1){
            if(plansza[p_w][p_k]==plansza[p_w-1][p_k]||
               plansza[p_w][p_k]==plansza[p_w-1][p_k+1]||
               plansza[p_w][p_k]==plansza[p_w][p_k+1]||
               plansza[p_w][p_k]==plansza[p_w+1][p_k]||
               plansza[p_w][p_k]==plansza[p_w+1][p_k+1]){
                if(plansza[p_w][p_k]==1) return 5;
                if(plansza[p_w][p_k]==2) return -5;
            }
        }
        if(p_k==wymiar-1&&p_w>0&&p_w<wymiar-1){
            if(plansza[p_w][p_k]==plansza[p_w-1][p_k]||
               plansza[p_w][p_k]==plansza[p_w-1][p_k-1]||
               plansza[p_w][p_k]==plansza[p_w][p_k-1]||
               plansza[p_w][p_k]==plansza[p_w+1][p_k]||
               plansza[p_w][p_k]==plansza[p_w+1][p_k-1]){
                if(plansza[p_w][p_k]==1) return 5;
                if(plansza[p_w][p_k]==2) return -5;
            }
        }
        if(p_k==wymiar-1&&p_w==wymiar-1){
            if(plansza[p_w][p_k]==plansza[p_w-1][p_k-1]||
               plansza[p_w][p_k]==plansza[p_w-1][p_k]||
               plansza[p_w][p_k]==plansza[p_w][p_k-1]){
                if(plansza[p_w][p_k]==1) return 5;
                if(plansza[p_w][p_k]==2) return -5;
            }
        }
    }
    return 0;
}
//Klon metody analiza(), który wypisuje gdzie zostało wykryte zwycięstwo.
//Używana do debuggingu.
int KiK::analiza2(){
    int iloczyn=1;                                  //funkcja wymnaża wartości w rzędach, kolumnach i przekątnych
    for(int i = 0; i < wymiar; i++) {               //Gdyż poszukiwane są konkretne iloczyny: 0-rząd niepełny
        iloczyn=1;                                  //1-zwycięża krzyżyk, 2^wymiar- zwycięża kółko
        //w poziomie
        for(int j=0; j<wymiar; j++){
            iloczyn*=plansza[i][j];
        }
        if(iloczyn==1){
            cout<<"Wykryto x w rzędzie "<<i<<".\n";
            return  10;
        }
        if(iloczyn==wygranko){
            cout<<"Wykryto o w rzędzie "<<i<<".\n";
            return  -10;
        }
        //w pionie
        iloczyn=1;
        for(int j=0; j<wymiar; j++){
            iloczyn*=plansza[j][i];
        }
        if(iloczyn==1){
            cout<<"Wykryto x w kolumnie "<<i<<".\n";
            return  10;
        }
        if(iloczyn==wygranko){
            cout<<"Wykryto o w kolumnie "<<i<<".\n";
            return  -10;
        }
	}
	iloczyn=1;
	for(int i=0; i<wymiar; i++){
        int j=i;
        iloczyn*=plansza[i][j];
        j++;
	}
    if(iloczyn==1){
            cout<<"Wykryto x po przekątnej ↘.\n";
            return  10;
        }
    if(iloczyn==wygranko){
            cout<<"Wykryto o po przekątnej ↗.\n";
            return  -10;
        }
    iloczyn=1;
//    cout<<"przekątna /:";
    int j=0;
    for(int i=wymiar-1; i>-1; i--){
//        cout<<"plansza["<<i<<"]["<<j<<"]: "<<plansza[i][j]<<' ';
        iloczyn*=plansza[i][j];
        j++;
	}
//	cout<<"koniec przekątnej\n";
    if(iloczyn==1){
            cout<<"Wykryto x po przekątnej /.\n";
            return  10;
        }
    if(iloczyn==wygranko){
            cout<<"Wykryto o po przekątnej /.\n";
            return  -10;
        }

	return 0;
}
//Funkcja odpowiedzialna za rozpoczęcie algorytmu minimax.
//glebokosc odpowiada za maksymalną dopuszczalną głębokość rekursji
//pole pozwala przekazać dane o aktualnie rozpatrywanym polu do metody analiza_somsiad()
ruch KiK::minimax(int **plansza, int glebokosc, ruch pole) {
	int najlepszy=99999;//Zainicjalizowanie najlepszego ruchu jako wysoką wartość
	ruch RUCH;          //ruch, który zostanie zwrócony przez metodę i wykonany przez SI

	for(int i=0; i<wymiar; i++){
		for(int j=0; j<wymiar; j++){
			if(plansza[i][j]==0){                                       //gdy pole jest puste
				plansza[i][j]=komp;                                     //postaw krzyżyk
				pole.wiersz=i; pole.kolumna=j;                          //zapisanie współrzędnych
//				cout<<"Lecimy plansza["<<i<<"]["<<j<<"]\n";
				int wagaruchu=MAX(plansza, glebokosc, pole);            //Wykonanie ruchu maximizera
//				cout<<"plansza["<<i<<"]["<<j<<"]: "<<wagaruchu<<endl;
				if(wagaruchu<=najlepszy){                               //Gdy ruch jest lepszy
					najlepszy=wagaruchu;                                //podmień wartości
					RUCH.wiersz =i;                                     //zapisz współrzędne
					RUCH.kolumna=j;
				}
				plansza[i][j]=0;                                        //wycofaj krzyżyk
			}
		}
	}
	return RUCH;        //zwróć współrzędne
}
int KiK::MAX(int **plansza, int glebokosc, ruch pole) {
	if(czykoniec()) return -analiza();                  //gdy koniec możliwych ruchów zwróć stan planszy
//	int wynik_an=analiza();
//	if(wynik_an==10||wynik_an==-10) return -wynik_an;   //gdy zwycięstwo- zwróć stan planszy
//    if(glebokosc<0) return -analiza_somsiad(pole);      //gdy przekroczono głębokość- uprość

	int najlepszy=-99999;
	for(int i=0; i<wymiar; i++){
		for(int j=0; j<wymiar; j++){
			if(plansza[i][j]==0){
				plansza[i][j]=gracz;
				pole.wiersz=i; pole.kolumna=j;
				int wagaruchu=MIN(plansza, glebokosc-1, pole);
				if(wagaruchu>=najlepszy) {
					najlepszy=wagaruchu;
				}
				plansza[i][j]=0;
			}
		}
	}
	return najlepszy;
}
int KiK::MIN(int **plansza, int glebokosc, ruch pole) {
	if(czykoniec()) return -analiza();
//	int wynik_an=analiza();
//	if(wynik_an==10||wynik_an==-10) return -wynik_an;
//    if(glebokosc<0) return -analiza_somsiad(pole);

	int najlepszy=99999;
	for(int i=0; i<wymiar; i++){
		for(int j=0; j<wymiar; j++){
			if(plansza[i][j]==0) {
				plansza[i][j]=komp;
				pole.wiersz=i; pole.kolumna=j;
				int wagaruchu=MAX(plansza, glebokosc-1, pole);
				if(wagaruchu<=najlepszy) {
					najlepszy=wagaruchu;
				}
				plansza[i][j]=0;
			}
		}
	}
	return najlepszy;
}
//Funkcja odpowiedzialna za wykonanie ruchu jako AI
void KiK::zrupruh(){
    ruch pole;
    ruch RUCH=minimax(plansza, 20, pole);
    plansza[RUCH.wiersz][RUCH.kolumna]=komp;
}
