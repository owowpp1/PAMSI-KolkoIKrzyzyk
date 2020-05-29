#include <SFML/Graphics.hpp>
#include "Kolko_i_krzyzyk.hh"
#include <string>
#include <thread>
#include <chrono>
int main(){
START:
    std::string napis;  //służy do wypisania aktualnej wartości zmiennej wymiar
    int wynik_an=99;    //wynik analizy: -10 gdy zwycięża gracz, 10 gdy AI, 0 gdy remis
    int wymiar=3;       //zmienna służąca określenie rozmiaru planszy w pierwszym oknie
    float grubosc;      //grubość ramek, a także pomocnicza wartość do konstrukcji GUI
    bool czykomp=false; //czy teraz kolej AI?
    sf::Vector2f Mysz;  //współrzędne położenia myszy

    sf::Texture texture;    texture.loadFromFile("rama.jpg");   //tekstura ramek (textures.com)
    sf::Texture tlo;        tlo.loadFromFile("tlo.jpg");        //tekstura tła (charrodelrey.com)
    sf::Texture wyjscie;    wyjscie.loadFromFile("exit.png");   //tekstura guzika exit (wikipedia.org)
    sf::Texture kolko;      kolko.loadFromFile("o.png");        //tekstura kółka (własna)
    sf::Texture krzyzyk;    krzyzyk.loadFromFile("x.png");      //tekstura krzyżyka (własna)
    sf::Texture puste;      puste.loadFromFile("-.png");        //pusty obraz .png stawiany w pustym polu (własny)
    sf::Texture Tlo_komunikatu;                                 //Tło komunikatów używane przy ustawieniu wymiaru i informacji o wyniku
    sf::Texture potw;                                           //pomocnicza tekstura używana przy komunikacji
    sf::Texture text;       text.loadFromFile("wygrankox.png"); //Tekst wyświetlany w komunikatach jest również teksturą (własną)
    text.setSmooth(true);   //by tekst nie był rozpiskelowany


    sf::Sprite sprite;          //Sprite ramki
    sf::Sprite komunikat;       //sprite tła komunikatu
    sf::Sprite tlo_spr;         //sprite tła całego okna
    sf::Sprite wyjscie_spr;     //sprite przycisku exit
    sf::Sprite tekst;           //tekst wyświetlany w komunikatach
    sf::Sprite ye;              //guzik odpowiedzialny za potwierdzenie
    sf::Sprite nah;             //guzik odpowiedzialny za odrzucenie
    sf::Sprite ok;              //guzik zatwierdzający zmianę rozmiaru

    sf::Font czcionka;          //czcionka używana do wypisania aktualnego wymiaru planszy
    czcionka.loadFromFile("czciona.ttf");   //Zastosowana czcionka: EFN-eukalipte

    sf::Text info;                          //tekst służący do wyświetlenia aktualnego wymiaru planszy
    info.setFont(czcionka);                 //ustawienie czcionki
    info.setFillColor(sf::Color::Black);    //ustawienie koloru

    sf::Image tlo_komunikatu;               //obraz ten zostanie przetworzony w teksturę

    auto desktop = sf::VideoMode::getDesktopMode(); //pobranie informacji o pulpicie

    sf::RenderWindow opcje(     //utworzenie okna z wyborem rozmiaru
        sf::VideoMode(          //w trybie video
        (desktop.height*3)/4,   //szerokość=wysokość=3/4 wysokości pulpitu
        (desktop.height*3)/4),
        "Giera",                //nazwa okna
        sf::Style::None         //styl none- brak ramki i mozliwosci skalowania
        );
    //wysrodkowanie okna na ekranie
    opcje.setPosition(sf::Vector2i(desktop.width/2 - opcje.getSize().x/2, desktop.height/2 - opcje.getSize().y/2));
    //utworzenie tła komunikatu jako półprzezroczysty biały prostokąt
    tlo_komunikatu.create(0.8*opcje.getSize().x, 0.6*opcje.getSize().y, sf::Color(255, 255, 255, 128));
    //przeniesienie go na teksturę
    Tlo_komunikatu.loadFromImage(tlo_komunikatu);
    //ustawienie tekstury sprite'owi
    komunikat.setTexture(Tlo_komunikatu);
    //ustawienie punktu odniesienia na środek sprite'a
    komunikat.setOrigin(0.4*opcje.getSize().x, 0.3*opcje.getSize().y);
    //ustawienie pozycji sprite'a w oknie
    komunikat.setPosition(opcje.getSize().x/2,opcje.getSize().y/2);
    //ustawienie tekstury tła sprite'owi
    tlo_spr.setTexture(tlo);
    //nadanie sprite'owi wymiarów równych wymiarom okna
    tlo_spr.setTextureRect(sf::IntRect(0,0,opcje.getSize().x,opcje.getSize().y));
    //Jak wcześniej: wyśrodkowanie punktu odniesienia
    tekst.setOrigin(250.f, 50.f);
    //przeskalowanie sprite'a. Ta tekstura ma szerokość 500px, zatem
    //sprite będzie miał szerokość (x/500/1.5)*500 czyli w efekcie
    //szerokość okna podzieloną przez 1.5
    tekst.setScale(opcje.getSize().x/750, opcje.getSize().x/750);
    //ustawienie sprite'a na środku w poziomie i nieco w górze w pionie
    tekst.setPosition(opcje.getSize().x/2, opcje.getSize().y/3.5);
    //ponowienie poprzednich czynności dla reszty sprite'ów:
    ye.setOrigin(101.f, 50.f);
    ye.setPosition(opcje.getSize().x/3, opcje.getSize().y/1.5);
    nah.setOrigin(101.f, 50.f);
    nah.setPosition(opcje.getSize().x/1.5, opcje.getSize().y/1.5);
    ok.setOrigin(101.f, 50.f);
    ok.setPosition(opcje.getSize().x/2, opcje.getSize().y/1.5);
    //ustawienie rozmiaru czcionki na 1/10 wysokości ekranu
    info.setCharacterSize(opcje.getSize().y/10);
    /**************************USTAWIENIE ROZMIARU***************************************/
    while(opcje.isOpen()){  //gdy okno 'opcje' jest otwarte:

        sf::Event event;                //fragment kodu od dewelopera sfml
        while (                         //służył sprawdzeniu działania bibliotek
            opcje.pollEvent(event))      //teraz pozwala zamknąć okno guzikiem 'x'
            if (event.type ==
            sf::Event::Closed)
                opcje.close();
        opcje.clear();
        opcje.draw(tlo_spr);        //wyrysowanie tła
        opcje.draw(komunikat);      //wyrysowanie tła komunikatu
        //przywrócenie oryginalnej skali przyciskom +/-
        ye.setScale(0.5*tekst.getLocalBounds().height/100, 0.5*tekst.getLocalBounds().height/100);
        nah.setScale(0.5*tekst.getLocalBounds().height/100, 0.5*tekst.getLocalBounds().height/100);
        //wyśrodkowanie tekstu
        info.setOrigin(floor(info.getLocalBounds().width /2) , floor(info.getLocalBounds().height /2 ));
        info.setPosition(opcje.getSize().x/2,opcje.getSize().y/2);
        //skonstruowanie stringa z informacją o wymiarze
        napis=std::to_string(wymiar)+"x"+std::to_string(wymiar);
        //przekazanie stringa do sprite'a
        info.setString(napis);
        opcje.draw(info);                   //wypisanie wymiaru
        text.loadFromFile("jaka.png");      //ustawienie tekstury tekstu (własna)
        tekst.setTexture(text);             //przekazanie jej do sprite'a
        opcje.draw(tekst);                  //wyrysowanie tekstu
        potw.loadFromFile("--.png");        //ustawienie tekstury minusa (własna)
        ye.setTexture(potw);                //przekazanie jej do sprite'a
        opcje.draw(ye);                     //wyrysowanie minusa
        potw.loadFromFile("+.png");         //ustawienie tekstury plusa (własna)
        nah.setTexture(potw);               //przekazanie jej do sprite'a
        opcje.draw(nah);                    //wyrysowanie plusa
        potw.loadFromFile("ok.png");        //ustawienie tekstury zatwierdzenia (własna)
        ok.setTexture(potw);                //przekazanie jej do sprite'a
        opcje.draw(ok);                     //wyrysowanie sprite'a

        Mysz=opcje.mapPixelToCoords(sf::Mouse::getPosition(opcje)); //pobranie aktualnej lokalizacji myszki
        sf::FloatRect obszar=ye.getGlobalBounds();                  //obszar zajmowany przez plus
        if(obszar.contains(Mysz)){              //jezeli wskaźnik znajduje się na tym obszarze
            potw.loadFromFile("--.png");        //wczytaj ponownie teksturę, gdyż mogła zostać nadpisana
            //przeskaluj sprite'a
            ye.setScale(tekst.getLocalBounds().height/100, tekst.getLocalBounds().height/100);
            opcje.draw(ye);                     //wyrysuj sprite'a
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){        //wiemy, że wskaźnik jest na przycisku, zatem
                if(wymiar>3)    wymiar--;                           //gdy klikniemy LPM-> zmniejsz wymiar o 1 (gdy >3)
                std::this_thread::sleep_for(std::chrono::milliseconds(300));    //odczekaj chwilę- inaczej przycisk byłby
            }                                                                   //rejestrowany setki razy na sekundę
        }
        obszar=nah.getGlobalBounds();                               //sprawdzenie obszaru zajmowanego przez plus
        if(obszar.contains(Mysz)){                                  //analogiczne operacje
            potw.loadFromFile("+.png");
            nah.setScale(tekst.getLocalBounds().height/100, tekst.getLocalBounds().height/100);
            opcje.draw(nah);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                if(wymiar<13)   wymiar++;
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
        }
        obszar=ok.getGlobalBounds();                                //sprawdzenie obszaru zajmowanego przez OK
        if(obszar.contains(Mysz)){                                  //gdy wskaźnik na obszarze:
            potw.loadFromFile("ok2.png");                           //wczytaj alternatywną teksturę (kolor zielony)
            opcje.draw(ok);                                         //wyrysuj sprite'a
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){        //gdy zarejestrowano LPM:
                opcje.close();                                      //zamknij okno- zmienna zapisana
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
        }

        opcje.display();    //wyświetlenie wyrysowanych sprite'ów w oknie
    }
    /*******************************************POCZĄTEK GRY*****************************************/
    KiK plansza(wymiar);    //utworzenie gry o wymiarze wymiar

    sf::Sprite PLANSZA[plansza.wymiar][plansza.wymiar];             //utworzenie tablicy sprite'ów (każdy to oddzielne pole)
    sf::RenderWindow okno(                                          //utworzenie nowego okna
        sf::VideoMode((desktop.height*3)/4, (desktop.height*3)/4),  //rozmiar taki, jak poprzednio
        "Giera",
        sf::Style::Resize+sf::Style::Close                          //to okno można skalować i zamknąć guzikiem 'x'
        );
    //wyśrodkowanie okna na pulpicie
    okno.setPosition(sf::Vector2i(desktop.width/2 - okno.getSize().x/2, desktop.height/2 - okno.getSize().y/2));
    //Wyznaczenie grubości ramki. Każde pole ma wymiar 5*grubość na 5*grubość. Zatem szerokość i wysokość okna
    //można podzielić na 6*wymiar+1 równych części. Każde pole zajmuje 5 takich części, a ramka jedną.
    grubosc=okno.getSize().y/(6*plansza.wymiar+1);
    texture.setRepeated(true);          //by tekstura mogła się powtarzać
    tlo_spr.setTexture(tlo);            //przekazanie tekstury tła do sprite'a
    tlo_spr.setTextureRect(sf::IntRect(0,0,okno.getSize().x,okno.getSize().y)); //ustawienie rozmiaru sprite'a z tłem
    sprite.setTexture(texture);         //przekazanie tekstury ramki do sprite'a
    sprite.setTextureRect(sf::IntRect(0,0,okno.getSize().x,grubosc));           //ustawienie rozmiaru sprite'a z ramką
    wyjscie_spr.setTexture(wyjscie);    //przekazanie tekstury guzika exit do sprite'a
    wyjscie_spr.setOrigin(150.f,62.f);  //ustawienie punktu odniesienia na środku sprite'a (300x124px)
    wyjscie_spr.setScale(grubosc/124,grubosc/124);  //przeskalowanie sprite'a jak w przypadku tekstu w oknie 'opcje'
    wyjscie_spr.setPosition(okno.getSize().x/2, okno.getSize().y-0.05*okno.getSize().y/2);    //ustawienie pozycji sprite'a exit
    tlo_komunikatu.create(0.8*opcje.getSize().x, 0.6*opcje.getSize().y, sf::Color(255, 255, 255, 220)); //lekko przezroczyste
    Tlo_komunikatu.loadFromImage(tlo_komunikatu);   //przekazanie obrazu z tłem komunikatu do tekstury
    komunikat.setTexture(Tlo_komunikatu);           //a następnie do sprite'a
    //Analogiczne operacje na reszcie sprite'ów i tekstur:
    komunikat.setOrigin((okno.getSize().x-(4*grubosc))/2,(okno.getSize().y-(12*grubosc))/2);
    komunikat.setPosition(okno.getSize().x/2,okno.getSize().y/2);
    tekst.setOrigin(250.f, 50.f);
    tekst.setScale((okno.getSize().x-(4*grubosc))/500, (okno.getSize().x-(4*grubosc))/500);
    tekst.setPosition(okno.getSize().x/2, okno.getSize().y/2-(okno.getSize().x-(4*grubosc))/10);
    ye.setOrigin(101.f, 50.f);
    ye.setScale(2*grubosc/100, 2*grubosc/100);
    ye.setPosition(okno.getSize().x/2-plansza.wymiar*grubosc, okno.getSize().y/2+plansza.wymiar*grubosc);
    nah.setOrigin(101.f, 50.f);
    nah.setScale(2*grubosc/100, 2*grubosc/100);
    nah.setPosition(okno.getSize().x/2+plansza.wymiar*grubosc, okno.getSize().y/2+plansza.wymiar*grubosc);
    ok.setOrigin(101.f, 50.f);
    ok.setScale(2*grubosc/100, 2*grubosc/100);
    ok.setPosition(okno.getSize().x/2, okno.getSize().y/2+plansza.wymiar*grubosc);

    for(int i=0; i<plansza.wymiar; i++){                            //zapełnienie tablicy sprite'ów
        for(int j=0; j<plansza.wymiar; j++){                        //ustawienie każdego elementu jako pusty
            PLANSZA[i][j].setTexture(puste);                        //następnie przeskalowanie ich zgodnie z wcześniej
            PLANSZA[i][j].setScale(5*grubosc/100, 5*grubosc/100);   //ustaloną podziałką (5*grubosc x 5*grubosc)
            PLANSZA[i][j].setPosition(grubosc+6.1*i*grubosc,grubosc+6.1*j*grubosc);
        }   //położenie każdego sprite'a w odpowiedniej pozycji na planszy z pomocą indeksów i oraz j
    }

    while (okno.isOpen()){              //gdy okno 'okno' jest otwarte:

        sf::Event event;                //fragment kodu od dewelopera sfml
        while (                         //służył sprawdzeniu działania bibliotek
            okno.pollEvent(event))      //teraz pozwala zamknąć okno guzikiem 'x'
            if (event.type ==
            sf::Event::Closed)
                okno.close();

        okno.clear();                   //wyczyszczenie okna- by nie wyrysowywać miliona warstw sprite'ów
    okno.draw(tlo_spr);                 //wyrysowanie tła
    sprite.setPosition(0,0);            //ustawienie pozycji pierwszej części ramki na początek okna
    sprite.setRotation(0);              //wycofanie rotacji
    okno.draw(sprite);                  //wyrysowanie pierwszego elementu ramki
    for(int i=0; i<plansza.wymiar; i++){//wyrysowanie poziomych elementów ramki
        sprite.move(0, 6.1*grubosc);
        okno.draw(sprite);              //wyrysowanie sprite'u w nowym miejscu
    }
    sprite.rotate(90);                  //obrócenie sprite'u o 90 stopni, co przenosi lewy górny róg w miejsce prawego górnego
    sprite.setPosition(grubosc,0);      //zatem jego położenie to [grubosc, 0]
    okno.draw(sprite);                  //wyrysowanie pierwszego pionowego elementu ramki
    for(int i=0; i<plansza.wymiar; i++){//wyrysowanie pionowych elementów ramki
        sprite.move(6.1*grubosc, 0);
        okno.draw(sprite);              //wyrysowanie sprite'u w nowymi miejscu
    }

    wyjscie_spr.setScale(0.05*okno.getSize().y/124,0.05*okno.getSize().y/124);  //zresetowanie rozmiaru guzika exit
    okno.draw(wyjscie_spr);                         //wyrysowanie guzika exit
    for(int i=0; i<plansza.wymiar; i++){
        for(int j=0; j<plansza.wymiar; j++){
            if(plansza.plansza[i][j]==0)PLANSZA[i][j].setTexture(puste);    //Ustawienie tekstur wszystkim polom na planszy
            if(plansza.plansza[i][j]==1)PLANSZA[i][j].setTexture(krzyzyk);  //w zależności od sytuacji na planszy z klasy KiK
            if(plansza.plansza[i][j]==2)PLANSZA[i][j].setTexture(kolko);

            okno.draw(PLANSZA[i][j]);   //wyrysowanie gotowego sprite'u
        }
    }

        Mysz=okno.mapPixelToCoords(sf::Mouse::getPosition(okno));       //pobranie położenia wskaźnika
        sf::FloatRect Krawedzie_wyj=wyjscie_spr.getGlobalBounds();      //ustawienie obszaru zajmowanego przez guzik exit
        if (Krawedzie_wyj.contains(Mysz)){
            wyjscie_spr.setScale(0.075*okno.getSize().y/124,0.075*okno.getSize().y/124);      //gdy wskaźnik nad guzikiem- powiększ go
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){           //gdy kliknięto:
                okno.close();                                           //zamknij okno
                break;                                                  //przerwij pętlę
            }
        }
        for(int i=0; i<plansza.wymiar; i++){            //dla każdego pola na planszy
            for(int j=0; j<plansza.wymiar; j++){
                Mysz=okno.mapPixelToCoords(sf::Mouse::getPosition(okno));       //pobierz położenie wskaźnika
                sf::FloatRect Krawedzie_pole=PLANSZA[i][j].getGlobalBounds();   //zbadaj obszar zajmowany przez pole i j
                if (Krawedzie_pole.contains(Mysz)){                             //gdy wskaźnik na polu, a pole puste
                    if(plansza.plansza[i][j]==0&&wynik_an!=10&&wynik_an!=-10&&!plansza.czykoniec()){    //i nie koniec gry
                        PLANSZA[i][j].setColor(sf::Color(255, 255, 255, 128));  //ustaw teksturę sprite'u na półprzezroczystą
                        PLANSZA[i][j].setTexture(kolko);                        //i wyrysuj w tym miejscu kółko
                        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                            plansza.plansza[i][j]=2;                            //Gdy kliknięto- postaw kółko w polu i j
                            czykomp=true;                                       //zmień kolej na AI
                        }
                        okno.draw(PLANSZA[i][j]);                               //wyrysuj pole
                        PLANSZA[i][j].setColor(sf::Color(255, 255, 255, 255));  //powróć do nieprzezroczystej tekstury
                    }
                }
            }
        }
        if(wynik_an!=10&&wynik_an!=-10){    //by uniknąć zasypania konsoli informacjami o wykrytej wygranej
            wynik_an=plansza.analiza2();
            if(wynik_an==10){               //gdy odnotowano zwycięstwo- przeskocz kolejkę AI
                goto WYNIKI;
            }
            if(wynik_an==-10){              //gdy odnotowano zwycięstwo- przeskocz kolejkę AI
                goto WYNIKI;
            }
        }
        if(czykomp&&!plansza.czykoniec()){  //gdy kolej AI i są miejsca na planszy
            plansza.zrupruh();              //AI wykonuje ruch
            czykomp=false;                  //zmień kolej na gracza
        }
    /************************ANALIZA WYNIKÓW***********************************/
    WYNIKI:
        if(wynik_an==10){                           //gdy zwycięża krzyżyk
            okno.draw(komunikat);                   //wyrysuj tło komunikatu
            text.loadFromFile("wygrankox.png");     //ustaw teksturę na informację o wygranej AI
            tekst.setTexture(text);                 //przekaż ją do sprite'u
            okno.draw(tekst);                       //wyrysuj sprite'a
            potw.loadFromFile("ye.png");            //ustaw teksturę potwierdzającą
            ye.setTexture(potw);                    //przekaż ją do sprite'u
            okno.draw(ye);                          //wyrysuj sprite'a
            potw.loadFromFile("nah.png");           //wczytaj teksturę odrzucającą
            nah.setTexture(potw);                   //przekaż ją do sprite'u
            okno.draw(nah);                         //wyrysuj sprite'a
            Mysz=okno.mapPixelToCoords(sf::Mouse::getPosition(okno));   //pobierz położenie myszy
            sf::FloatRect Obszar=ye.getGlobalBounds();                  //oznacz obszar zajmowany przez potwierdzenie
            if (Obszar.contains(Mysz)){             //gdy mysz nad obszarem
                potw.loadFromFile("ye2.png");       //zmień teksturę na zieloną
                ye.setTexture(potw);                //przekaż ją do sprite'u
                okno.draw(ye);                      //wyrysuj sprite'a
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    plansza.wyzeruj();              //gdy kliknięto- wyzeruj i wróć na początek
                    goto START;
                }
            }
            Mysz=okno.mapPixelToCoords(sf::Mouse::getPosition(okno));   //pobierz położenie myszy
            Obszar=nah.getGlobalBounds();                               //oznacz obszar zajmowany przez odrzucenie
            if (Obszar.contains(Mysz)){             //gdy mysz nad obszarem
                potw.loadFromFile("nah2.png");      //zmień teksturę na czerwoną
                nah.setTexture(potw);               //przekaż ją do sprite'u
                okno.draw(nah);                     //wyrysuje sprite'a
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    okno.close();                   //gdy kliknięto- zamknij okno i przerwij pętlę
                    break;
                }
            }

        }
        //Analogicznie dla zwycięstwa gracza i remisu
        if(wynik_an==-10){
            okno.draw(komunikat);
            text.loadFromFile("wygrankoo.png");
            tekst.setTexture(text);
            okno.draw(tekst);
            potw.loadFromFile("ye.png");
            ye.setTexture(potw);
            okno.draw(ye);
            potw.loadFromFile("nah.png");
            nah.setTexture(potw);
            okno.draw(nah);
            Mysz=okno.mapPixelToCoords(sf::Mouse::getPosition(okno));
            sf::FloatRect Obszar=ye.getGlobalBounds();
            if (Obszar.contains(Mysz)){
                potw.loadFromFile("ye2.png");
                ye.setTexture(potw);
                okno.draw(ye);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    plansza.wyzeruj();
                    goto START;
                }
            }
            Mysz=okno.mapPixelToCoords(sf::Mouse::getPosition(okno));
            Obszar=nah.getGlobalBounds();
            if (Obszar.contains(Mysz)){\
                potw.loadFromFile("nah2.png");
                nah.setTexture(potw);
                okno.draw(nah);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    okno.close();
                    break;
                }
            }

        }
        if(wynik_an==0&&plansza.czykoniec()){
            okno.draw(komunikat);
            text.loadFromFile("wygranko-.png");
            tekst.setTexture(text);
            okno.draw(tekst);
            potw.loadFromFile("ye.png");
            ye.setTexture(potw);
            okno.draw(ye);
            potw.loadFromFile("nah.png");
            nah.setTexture(potw);
            okno.draw(nah);
            Mysz=okno.mapPixelToCoords(sf::Mouse::getPosition(okno));
            sf::FloatRect Obszar=ye.getGlobalBounds();
            if (Obszar.contains(Mysz)){
                potw.loadFromFile("ye2.png");
                ye.setTexture(potw);
                okno.draw(ye);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    plansza.wyzeruj();
                    goto START;
                }
            }
            Mysz=okno.mapPixelToCoords(sf::Mouse::getPosition(okno));
            Obszar=nah.getGlobalBounds();
            if (Obszar.contains(Mysz)){\
                potw.loadFromFile("nah2.png");
                nah.setTexture(potw);
                okno.draw(nah);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    okno.close();
                    break;
                }
            }

        }

    okno.draw(wyjscie_spr);     //wyrysuj guzik exit
    okno.display();             //wypisz scenę w oknie
    }


    return 0;
}
