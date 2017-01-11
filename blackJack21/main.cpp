#include <iostream>
#include <cstdlib>

#define MAX_PLAYER_NAME_LENGTH 1000
#define NUMBER_OF_DECKS 1
#define MAX_PLAYERS_AT_TABLE 3
#define PLAYER_MIN_START_SUM 100

using namespace std;

struct Player{
    char name[MAX_PLAYER_NAME_LENGTH];
    int suma;
};
int blackJackDeck[14];
Player playersArray[MAX_PLAYERS_AT_TABLE];

void mainMenu();
void clearScreen();
int readValue();


void choseGame();
void playGame(int);
void playerGame();
void readPlayerName(Player&);
void setPlayerMoney(int i);

int main()
{
    mainMenu();

    int chosenValue = readValue();

    if ( chosenValue == 3){
        cout << "Good bye\n";
        return 0;
    }

    if( chosenValue == 1 ){
        clearScreen();
        choseGame();
    }



    return 0;
}


void mainMenu(){

    cout << "1. New game\n";
    cout << "2. Check high score\n";
    cout << "3. Exit\n";
    cout << "Enter value\n";
}

void clearScreen(){
#ifdef __linux
    system("clear");
#elif _WIN32
    system("CLS");
#elif _WIN64
    system("CLS");
#endif
}

int readValue(){

    char s[1000];
    cin.getline(s, 1000);

    if(s[1] != '\0'){
        cout << "entered value is not a digit, retype value u fucking pleb\n";
        readValue();
    } else {
        if(s[0] >= '0' && s[0] <= '9' ){
            return s[0] - '0';
        } else {
            cout << "entered value is not a digit, retype value u fucking pleb\n";
            readValue();
        }
    }

}

void readPlayerName(Player& participant){
    cout << "Your name is: ";
    cin.getline(participant.name, MAX_PLAYER_NAME_LENGTH);
}

void setPlayerMoney(int i){
    playersArray[i].suma = PLAYER_MIN_START_SUM;
}

void choseGame(){

    cout << "1. Single player\n";
    cout << "2. Multiplayer\n";

    int chosenValue;
    do{
        chosenValue = readValue();
    }while(chosenValue >= 2 && chosenValue <= 1);
    playGame(chosenValue);

}

void playGame(int typeOfGame){

    int numberOfPlayers, i;

    if(typeOfGame == 1){
        numberOfPlayers = 1;
    }

    if(typeOfGame == 2){
        int value;
        do{
            cout << "number of players: ";
            value = readValue();
            cout << '\n';
        }while(value > MAX_PLAYERS_AT_TABLE-1 || value <= 1);
        numberOfPlayers = value;
    }

    for(i = 1; i <= numberOfPlayers; ++i){

        readPlayerName(playersArray[i]);
        setPlayerMoney(i);

    }

    for(i = 1; i <= numberOfPlayers; ++i){
        playerGame();
    }

}

void playerGame(){

}



