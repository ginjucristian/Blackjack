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
    int currentBet;

    int cardsStack[MAX_POSSIBLE_CARDS_FOR_A_BLACKJACK];
    int cardsStackTop;

    int cardSum;

    bool hasSplit;

    int cardSumOnSplit[2];

};
int blackJackDeck[14];
Player playersArray[MAX_PLAYERS_AT_TABLE];
bool playersOut[MAX_PLAYERS_AT_TABLE];

int bets[] = {0, 10, 25, 50, 100, 200};

void mainMenu();
void clearScreen();
int readValue();
int getChosenVal(int, int);

void choseGame();

void playGame(int);
void playHand(Player&);

void readPlayerName(Player&);
void setPlayerMoney(int);

void giveCards(Player&);

void reset(int);
void resetStack(Player&);
void giveFirstCards(Player&);
void addCard(Player&);
void placeBets(Player&);

void hitDecision(Player&);
void doubleDownDecision(Player&);
void splitDecision(Player&);

bool isOver(int);
int calculateSumOfCards(Player);
void showCardLetterOrDigit(int);
void surrenderDecision(Player&);

bool checkBusted(Player);
bool checkBlackJack(Player);
bool playerWon(Player);

void playLoop(Player&);
void dealerGame(int);
void showDealersCards();

int main()
{

    srand(time(NULL));

    mainMenu();

    int chosenValue = getChosenVal(1, 3);

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
    cout << "Enter value: ";
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
        cout << "entered value is not a digit, retype value u fucking pleb: ";
        readValue();
    } else {
        if(s[0] >= '0' && s[0] <= '9' ){
            return s[0] - '0';
        } else {
            cout << "entered value is not a digit, retype value u fucking pleb: ";
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
    playersArray[i].playerMaximumEverMoney = PLAYER_MIN_START_SUM;
}

void choseGame(){

    cout << "1. Single player\n";
    cout << "2. Multiplayer\n";

    int chosenValue = getChosenVal(1, 2);
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
            //clearScreen();
            playersOut[i] = true;
        }

        dealerGame(numberOfPlayers);

    }




}

void dealerGame(int numberOfPlayers){

    showDealersCards();
    int i;
    playersArray[0].cardSum = calculateSumOfCards(playersArray[0]);
    while(playersArray[0].cardSum < 17){

        addCard(playersArray[0]);
        playersArray[0].cardSum = calculateSumOfCards(playersArray[0]);
        showDealersCards();

    }

    if(numberOfPlayers == 1){
       if(playerWon(playersArray[1]) && !checkBlackJack(playersArray[1])){
            addCard(playersArray[0]);
       }
    }

    for(i = 1; i <= numberOfPlayers; ++i){

        if(playersArray[i].hasSplit){
            if(playersArray[i].cardSumOnSplit[0] <= 21 && playersArray[i].cardSumOnSplit[0] > playersArray[0].cardSum ){
                playersArray[i].playerCurrentMoney += playersArray[i].currentBet / 2;
                playersArray[i].currentBet /= 2;
                cout << playersArray[i].name << " won " << playersArray[i].currentBet/2 << "$\n";
            } else {
                cout << "Dealer won " << playersArray[i].currentBet/2 << "$ from " << playersArray[i].name << '\n';
            }
            if(playersArray[i].cardSumOnSplit[1] <= 21 && playersArray[i].cardSumOnSplit[1] > playersArray[0].cardSum ){
                playersArray[i].playerCurrentMoney += playersArray[i].currentBet / 2;
                playersArray[i].currentBet /= 2;
                cout << playersArray[i].name << " won " << playersArray[i].currentBet/2 << "$\n";
            } else {
                cout << "Dealer won " << playersArray[i].currentBet/2 << "$ from " << playersArray[i].name;
            }
        } else{
            if(playerWon(playersArray[i])){
                playersArray[i].playerCurrentMoney += 2*playersArray[i].currentBet;
                cout << playersArray[i].name << " won " << playersArray[i].currentBet << "$\n";
            } else {
                cout << "Dealer won " << playersArray[i].currentBet << "$ from " << playersArray[i].name << '\n';
            }
        }
        playersArray[i].currentBet = 0;
    }

}


void showDealersCards(){
    cout << "Dealer's cards: ";
    int i;
    for(i = 0; i <= playersArray[0].cardsStackTop; ++i){
        showCardLetterOrDigit(playersArray[0].cardsStack[i]);
    }
    cout << '\n';
}

void giveFirstCards(Player& participant){

    int i;
    for(i = 0; i < 2; ++i){
        addCard(participant);
    }

}

void addCard(Player& participant){
    int randomCard;
    do{
        randomCard = rand()%13 + 1;
    }while(blackJackDeck[randomCard] >= randomCard*4);

    participant.cardsStack[++participant.cardsStackTop] = randomCard;
    blackJackDeck[randomCard]++;
}


void reset(int numOfPlayers){
    int i;
    for(i = 0; i < numOfPlayers; ++i){
        resetStack(playersArray[i]);
        playersArray[i].currentBet = 0;
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

    cout << "Dealer's shown card: ";
    showCardLetterOrDigit(playersArray[0].cardsStack[0]);
    cout << '\n';

    cout << participant.name << "'s cards: ";

    int i;
    for(i = 0; i <= participant.cardsStackTop; ++i){
        showCardLetterOrDigit(participant.cardsStack[i]);
    }

    cout <<'\n';

}

void showCardLetterOrDigit(int cardValue){
     switch(cardValue){
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
            cout << cardValue << " ";
            break;
    }
}

void decisionMenu(){

    cout << "1. Hit\n";
    cout << "2. Stand\n";
    cout << "3. Double down\n";
    cout << "4. Split\n";
    cout << "5. Surrender\n";

}

bool checkBusted(Player participant){
    return participant.cardSum > 21;
}

bool checkBlackJack(Player participant){
    return participant.cardSum == 21;
}

bool playerWon(Player participant){
   return (participant.cardSum > playersArray[0].cardSum || playersArray[0].cardSum > 21) && !checkBusted(participant);
}

void playHand(Player& participant){

     placeBets(participant);

     showCards(participant);

     playLoop(participant);

}

void playLoop(Player& participant){

     participant.cardSum = calculateSumOfCards(participant);

     if( checkBlackJack(participant) ){
        cout << participant.name << " hit a blackjack ";
        return;
     }

     decisionMenu();

     int chosenVal = -1;
     while(chosenVal != 2){
        chosenVal = getChosenVal(1, 5);
        if(chosenVal == 1){
            hitDecision(participant);
            if(checkBusted(participant)){
                cout << "Player " << participant.name << " is BUSTED\n";
                return;
            }
            if(checkBlackJack(participant)){
                cout << "Player " << participant.name << " hit a blackjack\n";
                return;
            }
        }
        if(chosenVal == 3){
            doubleDownDecision(participant);
            if(checkBusted(participant)){
                cout << "Player " << participant.name << " is BUSTED\n";
            }
            if(checkBlackJack(participant)){
                cout << "Player " << participant.name << " hit a blackjack\n";
            }
            return;
        }
        if(chosenVal == 4){
            splitDecision(participant);
            return;
        }

        if(chosenVal == 5){
            surrenderDecision(participant);
            return;
        }

        if(chosenVal != 2){
            decisionMenu();
        }




     }

}

void doubleDownDecision(Player& participant){

    cout << "Player " << participant.name << " chose to double down\n";

    if(participant.cardsStackTop > 1){
        cout << "You cannot double down, sorry\n";
        return;
    }

    if(participant.playerCurrentMoney - participant.currentBet < 0 ){
        cout << "You do not have the money for a double down\n";
        return;
    }

    addCard(participant);
    participant.playerCurrentMoney -= participant.currentBet;
    participant.currentBet *= 2;
    participant.cardSum = calculateSumOfCards(participant);

    showCards(participant);

}

void splitDecision(Player& participant){

    cout << "Player " << participant.name << " " << " decided to split\n";


    if(participant.cardsStack[0] != participant.cardsStack[1] || participant.cardsStackTop > 1 || participant.hasSplit){
        cout << "You cannot split\n";
        return;
    }

    if(participant.playerCurrentMoney - participant.currentBet < 0){
        cout << "You do not have enough money to split\n";
        return;
    }

    participant.hasSplit = true;

    int baseOfNextStack = participant.cardsStack[participant.cardsStackTop--];

    addCard(participant);
    showCards(participant);

    participant.hasSplit = true;

    playLoop(participant);
    participant.cardSumOnSplit[0] = participant.cardSum;

    resetStack(participant);

    participant.cardsStack[++participant.cardsStackTop] = baseOfNextStack;
    addCard(participant);

    cout << "-----Split----\n";
    showCards(participant);


    playLoop(participant);
    participant.cardSumOnSplit[1] = participant.cardSum;

}

void surrenderDecision(Player& participant){

    if(participant.cardsStackTop > 1){
        cout << "You cannot surrender\n";
        return;
    }

    participant.playerCurrentMoney += participant.currentBet /= 2;

}



int getChosenVal(int left, int right){
    int chosenVal;
    bool firstTime = true;
    do{
        if(firstTime != true){
            cout << "You entered a wrong value, please retype the value: ";
        }
        firstTime = false;
        chosenVal = readValue();
    }while(chosenVal > right || chosenVal < left);
    return chosenVal;


}

void hitDecision(Player& participant){

    cout << participant.name << " decided to hit, his cards are: " << '\n';

    addCard(participant);
    showCards(participant);
    participant.cardSum = calculateSumOfCards(participant);

}

int calculateSumOfCards(Player participant){
    int i, softHand = 0, hardHand = 0;
    for(i = 0; i <= participant.cardsStackTop; ++i){
        if(participant.cardsStack[i] > 10){
            softHand += 10;
            hardHand += 10;
        } else if(participant.cardsStack[i] == 1){
            softHand += 1;
            hardHand += 11;
        } else {
            softHand += participant.cardsStack[i];
            hardHand += participant.cardsStack[i];
        }
    }

    if(hardHand > 21){
        return softHand;
    }
    return hardHand;
}

void placeBets(Player& participant){

    cout << participant.name << ' ' << "your bet is:\n";

    cout << "1. 10$\n";
    cout << "2. 25$\n";
    cout << "3. 50$\n";
    cout << "4. 100$\n";
    cout << "5. 200$\n";

    cout << "Enter bet value:\n";

    int chosenValue;
    bool isBetable = false;

    while(!isBetable){
        isBetable = true;
        chosenValue = getChosenVal(1, 5);
        if(participant.playerCurrentMoney < bets[chosenValue]){
            cout << "You don't really have money for this bet, please retype the bloody bet, pleb: ";
            isBetable = false;
        }
    }


    participant.currentBet = bets[chosenValue];
    participant.playerCurrentMoney -= participant.currentBet;

}
