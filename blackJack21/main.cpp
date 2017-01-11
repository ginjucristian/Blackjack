#include <iostream>
#include <cstdlib>
#include <ctime>

#define MAX_PLAYER_NAME_LENGTH 1000
#define NUMBER_OF_DECKS 1
#define MAX_PLAYERS_AT_TABLE 3
#define PLAYER_MIN_START_SUM 100
#define MAX_POSSIBLE_CARDS_FOR_A_BLACKJACK 11

using namespace std;

struct Player{
    char name[MAX_PLAYER_NAME_LENGTH];
    int playerCurrentMoney;
    int playerMaximumEverMoney;

    int cardsStack[MAX_POSSIBLE_CARDS_FOR_A_BLACKJACK];
    int cardsStackTop;

    int cardSum;

};
int blackJackDeck[14];
Player playersArray[MAX_PLAYERS_AT_TABLE];
bool playersOut[MAX_PLAYERS_AT_TABLE];

void mainMenu();
void clearScreen();
int readValue();


void choseGame();

void playGame(int);
void playHand(Player&);

void readPlayerName(Player&);
void setPlayerMoney(int i);

void giveCards(Player&);

void reset(int numOfPlayers);
void resetStack(Player& participant);
void giveFirstCards(Player& participant);

bool isOver(int numberOfPlayers);


int main()
{
    srand(time(NULL));

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
    playersArray[i].playerCurrentMoney = PLAYER_MIN_START_SUM;
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
        playersArray[i].cardsStackTop = -1;

    }

    while(isOver(numberOfPlayers) == false){

        reset(numberOfPlayers);

        for(i = 0; i <= numberOfPlayers; ++i){
            giveFirstCards(playersArray[i]);
        }

        for(i = 1; i <= numberOfPlayers; ++i){
            playHand(playersArray[i]);
            playersOut[i] = true;
        }

    }




}


void giveFirstCards(Player& participant){

    int i;
    for(i = 0; i < 2; ++i){

        int randomCard;
        do{
            randomCard = rand()%14 + 1;
        }while(blackJackDeck[randomCard] >= randomCard*4);

        participant.cardsStack[++participant.cardsStackTop] = randomCard;
        blackJackDeck[randomCard]++;

    }


}

void reset(int numOfPlayers){
    int i;
    for(i = 0; i < numOfPlayers; ++i){
        resetStack(playersArray[i]);
    }

    for(i = 1; i <= 13; ++i){
        blackJackDeck[i] = 0;
    }


}

void resetStack(Player& participant){

    while(participant.cardsStackTop >= 0){
        participant.cardsStack[participant.cardsStackTop--] = 0;
    }

}

bool isOver(int numberOfPlayers){
    int i;
    for(i = 1; i <= numberOfPlayers && playersOut[i]; ++i);
    return i > numberOfPlayers;
}

void showCards(Player participant){

    cout << "Dealer's shown card: " << playersArray[0].cardsStack[0] << '\n';
    cout << participant.name << "'s cards: ";

    int i;
    for(i = 0; i <= participant.cardsStackTop; ++i){

        switch(participant.cardsStack[i]){
        case 1:
            cout << "A ";
            break;
        case 11:
            cout << "J ";
            break;
        case 12:
            cout << "Q ";
            break;
        case 13:
            cout << "K ";
            break;
        default:
            cout << participant.cardsStack[i] << " ";
            break;
        }

    }

    cout <<'\n';

}

void decisionMenu(){

    cout << "1. Hit\n";
    cout << "2. Stand\n";
    cout << "3. Double down\n";
    cout << "4. Split\n";
    cout << "5. Surrender\n";

}

void playHand(Player& participant){

     showCards(participant);

     decisionMenu();

}


