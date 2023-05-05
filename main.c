#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>
#define SIZE 12

int board[SIZE][SIZE];
int game_board[SIZE][SIZE];

//---------------------BUDOWA PLANSZY----------------------------------------------------------------------------
void budowanie_planszy() {
    int i, j;
    for(i = 0; i < SIZE; i++)                    //Wypelnianie calej planszy "-"
        for(j = 0; j < SIZE; j++)
            board[i][j] = '-';
    for(i = 0; i < SIZE; i++)                   //Wypelnianie pierwszego, ostatniego rzedu i kolumny znakami bialymi
        for( j = 0; j < SIZE; j++)
            if(i == 0 || i == SIZE - 1)
                board[i][j] = ' ';
    for(j = 0; j < SIZE; j++)
        for(i = 0; i < SIZE; i++)
            if(j == 0 || j == SIZE - 1)
                board[i][j] = ' ';
}

void bomby(int poziom){   //algorytm losowania i wstawiania bomb do planszy
    srand(time(0));
    int  random_x, random_y;
    for(int i= 0; i<(int)(poziom*5+5);) {   // bomb w zaleznosci od poziomu bedzie 10/15/20
        random_x = 1+rand() % (SIZE-2);
        random_y = 1+rand() % (SIZE-2);
        if(board[random_x][random_y] == '-'){board[random_x][random_y]= '*'; i++;}
    }
}

void otocz(){//otaczanie bomb cyframi oznaczajacymi ile jest bomb w sasiedztwie
    int i, j, x=0, y=0, h, g;

    for( i=0; i<SIZE; i++){
        for( j=0; j<SIZE; j++){
            if(board[i][j] == '*'){
                h= i+2;
                g = j+2;
                for(x= i-1; x<h; x++){
                    for(y= j-1; y<g; y++){
                        if(board[x][y]==7)board[x][y]++;
                        if(board[x][y]==6)board[x][y]++;
                        if(board[x][y]==5)board[x][y]++;
                        if(board[x][y]==4)board[x][y]++;
                        if(board[x][y]==3)board[x][y]++;
                        if(board[x][y]==2)board[x][y]++;
                        if(board[x][y]==1) board[x][y]++;
                        if(board[x][y] == '-') board[x][y] = 1;
                    }
                }
            }
        }
    }
}

int odkrywanie(int x, int y, int odslanianie){ //funkcja odpowiedzialna za odkrywanie planszy az do napotkania bomb (algorytm flood fill)
    if(x == 0 || y == 0 || x==11 || y==11)return;
    if(board[x][y]== '*')return;
    if(board[x][y]>0 &&board[x][y]<9)game_board[x][y]= board[x][y];
    if(game_board[x][y]=='o' || odslanianie == false ){
        odslanianie=true;
        game_board[x][y]= board[x][y];
        odkrywanie(x+1, y, odslanianie);
        odkrywanie(x-1, y, odslanianie);
        odkrywanie(x, y+1, odslanianie);
        odkrywanie(x, y-1, odslanianie);
        odkrywanie(x-1, y-1, odslanianie);
        odkrywanie(x+1, y+1, odslanianie);
        odkrywanie(x+1, y-1, odslanianie);
        odkrywanie(x-1, y+1, odslanianie);
    }
}

int sterowanie(int x, int y){ // funkcja odpowiedzialna za sterowanie
    int i =1;
    int odslanianie = false;
    wyswietl_plansze();
    while(i==1){
        printf("  wspolrzedna x: ");
        scanf("%d", &y);
        if(y<1 || y>10){puts("\n  Bledne dane!\n");Sleep(1000); system("cls");sterowanie(x, y);}//zabezpieczenie
        printf("  wspolrzedna y: ");
        scanf("%d", &x);
        if(x<1 || x>10){puts("\n  Bledne dane!\n");Sleep(1000); system("cls");sterowanie(x, y);}//zabezpieczenie
        if(board[x][y]== '*'){ system("cls");odkryta_plansza(); puts("  PRZEGRANA!\n  Wszedles na mine.\n"); koniec(0, 0);}
        system("cls");
        odkrywanie(x, y, odslanianie);
        wyswietl_plansze();
    }
}
//--------------------------WYSWIETLANIE PLANSZY---------------------------------------------------------------------------------------------------

struct Game {       // Struktura przechowuj�ca g��wne dane o grze
    char nick[50];
    double wynik;
    int poziom;
    clock_t timer_start, timer_stop;    // czas pocz�tku i ko�ca rozgrywki
} game;

void ekran_startowy() {
    system("cls");
    puts("------------------------------------------------------------------");
    puts("------------------------------------------------------------------");
    puts("-----------------------Witaj w Saperze!---------------------------");
    puts("--------------------------PP2_projekt-----------------------------");
    puts("------------------------------------------------------------------");
    puts("------------------------------------------------------------------");

}



void start() {
    int poziom = 0;
    while(poziom<1 || poziom>3) {
        system("cls");
        ekran_startowy();
        printf("Poziom trudnosci\n1. Latwy\n2. Sredni\n3. Trudny\nTwoj wybor: ");
        scanf("%d", &poziom);
    }
    game.timer_start = clock(); // rozpocz�cie odliczania
    game.poziom = poziom;
    int x, y;
    system("cls");
    budowanie_planszy();//funkcje budujace plansze
    bomby(poziom);
    otocz();
    zakryta_plansza();
    sterowanie(x, y);
}

void koniec(int czy_wygrana, int pierwszy_raz){ //zakonczenie   /pierwszy_raz informuje, ze w razie wygranej dane zostana zapisane do rankingu
    game.timer_stop = clock(); // zako�czenie odliczania
    char wybor;
    game.wynik = (double)(game.timer_stop - game.timer_start)/1000;
    if(czy_wygrana) {
        printf("  Twoj nick: %s\n  Twoj wynik: %.2lf s\n", game.nick, game.wynik);  // pokazane w razie wygranej
        if(pierwszy_raz)
            zapisz_wynik(game.nick, game.wynik, game.poziom);
    } else {
        printf("  Twoj nick: %s\n  Twoj wynik: %.2lf s (przegrana)\n", game.nick, game.wynik);  // pokazane w razie przegranej
    }
    printf("  Chcesz zagrac jeszcze raz?(y/n)", wybor);
    scanf("%s",&wybor);
    if(toupper(wybor) == 'Y'){
            getchar();
            start();
    }
    if(toupper(wybor) == 'N'){
     printf("\n\n  Do zobaczenia!\n\n");
            (void) getchar();
            exit(EXIT_SUCCESS);
    }
    else{
        getchar();
        printf("\n\n  Wybierz poprawna opcje...\n\n");
        Sleep(1000);
        system("cls");
        koniec(czy_wygrana, 0); // 0 przekazane �eby nie zapisa�o kolejny raz wyniku
    }
}

void odkryta_plansza() {     //funkcja wyswietlajaca odkryta plansze
    int i, j;

    printf("  ");
    for(i = 1; i < SIZE - 1; i++)//wyswietlanie numeracji kolumn
        printf("%d ", i);
    printf("\n");

    for(i = 0; i < SIZE; i++){
        for(j = 0; j < SIZE; j++){
            if(board[i][j] == '-' || board[i][j]=='*' || board[i][j]==' ')//wyswietlanie planszy
                printf("%c ", board[i][j]);
            else
                printf("%d ", board[i][j]);
        }

        if(i > 0 &&  i < SIZE - 1)//wyswietlanie numeracji wierszy
            printf("%d", i);
        printf("\n");
    }
}

void wygrana(){
    int czy_wygrana=0;
    for(int i = 1; i < SIZE-1; i++){
        for(int j = 1; j < SIZE-1; j++){
            if(game_board[i][j]== 'o')czy_wygrana++;//warunek zliczajacy ile zostalo zakrytych pol
        }
    }
    if(czy_wygrana==(game.poziom*5+5)){system("cls");odkryta_plansza();puts("  Wygrana! Gratulacje\n");koniec(1, 1);}//warunek sprawdzajacy czy gracz wyczerpal liczbe ruchow
}

void zakryta_plansza() {//Zakrywanie planszy

    int i, j;
    printf("  Tworzenie planszy....\n\n");
    Sleep(1000);
     for( j = 0; j < SIZE; j++)
         for(i = 0; i < SIZE; i++)
            if(i == 0 || i == SIZE-1 )
                game_board[i][j] = ' ';
    for(j = 0; j < SIZE; j++)
        for(i = 0; i < SIZE; i++)
            if(j == 0 || j == SIZE -1)
                game_board[i][j] = ' ';
    for(i = 1; i < SIZE-1; i++)
        for(j = 1; j < SIZE-1; j++){
            game_board[i][j] = 'o';
        }
}

void wyswietl_plansze(){ //Wyswietla plansze gry
    int i, j, czy_wygrana=0;
    printf("  ");
    for(i = 1; i < SIZE-1 ; i++)//wypisanie numeracji wierszy
        printf("%d ", i);
    printf("\n");

    for(i = 0; i < SIZE; i++){//wypisanie planszy gry
        for(j = 0; j < SIZE; j++){
            if(game_board[i][j] == '-' || game_board[i][j]=='*' || game_board[i][j]==' ' || game_board[i][j]== 'o')
                printf("%c ", game_board[i][j]);
            else
                printf("%d ", game_board[i][j]);
        }
        if(i > 0 &&  i < SIZE - 1)//wypisanie numeracji kolumn
            printf("%d", i);
        printf("\n");
    }
    wygrana();
}
//-----------------------------------RANKING--------------------------------------------------------------------------------------

void wyczysc_scan() {while(getchar()!='\n');}   // czyszczenie skanu z niepotrzebnych znakow, zeby scanf() zachowywał się normalnie

void podaj_sciezke(char sciezka[], int poziom) {    // "generuje" ścieżkę do odpowieniej listy rankingowej
    char t[2];
    sprintf(t, "%d", poziom);   // konwersja int na string
    strcat(sciezka, t);
    strcat(sciezka, ".txt");    // dopisanie reszty scieżki
}

int policz_rekordy(FILE *f) {   // liczy ilosc linii w pliku tekstowym
    int i=0;
    char buff[1024];
    while(fgets(buff, 1024, f)) i++;    // pętla zliczająca linie w pliku
    fclose(f);
    return i;
}

void sortuj(int n, double wyniki[], char nicki[n][50]) { // Sortowanie bąbelkowe rekordów ze sprawdzeniem czy zaszła zmiana
    int ch;
    double tmp1;
    char tmp2[50];
    for(int i=0; i<n-1; ++i) {  // pętle sortujące bąbelkowo
        ch = 0;
        for(int j=0; j<n-1-i; j++) {
            if(wyniki[j+1] < wyniki[j]) {   // wypchnięcie bąbelka wyżej
                tmp1 = wyniki[j+1];
                wyniki[j+1] = wyniki[j];
                wyniki[j] = tmp1;
                strcpy(tmp2, nicki[j+1]);
                strcpy(nicki[j+1], nicki[j]);
                strcpy(nicki[j], tmp2);
                ch = 1;
            }
        }
        if(!ch) break;  // jeśli w danym przejściu nie nastąpiła żadna zmiana, fukncja nie sortuje dalej
    }
}

void czytaj_wyniki(FILE *f, int n, char nicki[n][50], double wyniki[]) {    // zapisuje dane z listy rankingowej do tablic nicki i wyniki
    char buff[1024];
    for(int i=0; fgets(buff, 1024, f); i++) {   // pętla for zapisująca kolejne dane z pliku
        strcpy(nicki[i], strtok(buff, ";"));
        char *s2 = strtok(NULL, ";");
        sscanf(s2, "%lf", &wyniki[i]);
    }
    fclose(f);
}

void zapisz_wynik(char nick[50], double wynik, int poziom) {    // dodaje nowy wynik do listy rankingowej
    FILE *f;
    char sciezka[13] = "ranking";
    podaj_sciezke(sciezka, poziom);
    if(f = fopen(sciezka, "a")) {   // Jeśli uda się otworzyć plik w trybie dodawania doda rekord, jeśli nie wyświetli komunikat
        fprintf(f, "\n%s;%.2lf;", nick, wynik);
        fclose(f);
    } else perror("Nie udalo sie dodac wyniku\n");
}

void pokaz_ranking(int n, char nicki[n][50], double wyniki[]) { // wyswietla 5 rekordów z tablic nicki i wyniki
    for(int i=0; i<5; i++)
        printf("%d. %s: %.2lf s\n", i+1, nicki[i], wyniki[i]);
}

void ranking(int poziom) {  // pobiera wszystkie rekordy z listy rankingowej, zapisuje je do tablic, sortuje i wyświetla
    system("cls");
    FILE *f;
    char sciezka[20] = "ranking";
    podaj_sciezke(sciezka, poziom);
    int linie = policz_rekordy(f = fopen(sciezka, "r"));
    double wyniki[linie];
    char nicki[linie][50];
    if(f = fopen(sciezka, "r")) czytaj_wyniki(f, linie, nicki, wyniki); // Jeśli uda się otworzyć plik w trybie odczytu pobierze dane do tablicki nicki oraz wyniki
    sortuj(linie, wyniki, nicki);   // sortowanie danych z pliku
    pokaz_ranking(linie, nicki, wyniki);    // wyświetlenie 5 najlepszych wyników
    system("pause");
}
void saper(){
srand(time(0));
ekran_startowy();
system("pause");
puts("\n\nPrzed rozpoczeciem rozgrywki podaj swoj nick!");
printf("Nick: ");
gets(game.nick);
printf("Loading...");
int los = 2 + rand()%5;
for(int j=0; j<los;j++){
   int i;
   for (i=0; i <3; i++)
   {
      printf(" *");
      Sleep(600);
   }

}
Sleep(1200);
}
int main()
{
    saper();
        while(1) {  // pętla zarządzająca menu
        system("cls");
        int poziom = 0;
        ekran_startowy();
        printf("1. Graj\n2. Ranking\n3. Wyjdz\nTwoj wybor: ");
        switch(getchar()) {
        case '1':
            wyczysc_scan();
            start();
            break;
        case '2':
            while(poziom<1 || poziom >3) {  // Pętla pobierająca poziom trudności do wyświetlenia rankingu
                system("cls");
                ekran_startowy();
                printf("Poziom trudnosci\n1. Latwy\n2. Sredni\n3. Trudny\nTwoj wybor: ");
                scanf("%d", &poziom);
            }
            ranking(poziom);    // wyswietlenie rankingu z podanego poziomu trudnosci
            break;
        case '3':
            exit(0);
            break;
        }
    }
    return 0;
}
