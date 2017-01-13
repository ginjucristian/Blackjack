#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>


#define MAX_PLAYER_NAME_LENGTH 1000
#define NUMBER_OF_DECKS 1
#define MAX_PLAYERS_AT_TABLE 5
#define PLAYER_MIN_START_SUM 100
#define MAX_POSSIBLE_CARDS_FOR_A_BLACKJACK 11

using namespace std;



struct highScore {
    char name[MAX_PLAYER_NAME_LENGTH];
    int score;
};



struct Player {

    char name[MAX_PLAYER_NAME_LENGTH];
    int playerCurrentMoney;
    int playerMaximumEverMoney;
    int currentBet;

    int cardsStack[MAX_POSSIBLE_CARDS_FOR_A_BLACKJACK];
    int cardsStackTop;

    int cardSum;

    bool hasSplit;
    bool hasSurrendered;

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
bool doubleDownDecision(Player&);
bool splitDecision(Player&);

bool isOver(int);
int calculateSumOfCards(Player);
void showCardLetterOrDigit(int);
void surrenderDecision(Player&);

bool checkBusted(Player);
bool checkBlackJack(Player);
bool playerWon(Player);

void playerBusted(Player);
void playerBlackJack(Player);

void playLoop(Player&);
void dealerGame(int);
void showDealersCards();

void showCurrentMoney(Player participant) {
    cout << participant.name << " has " << participant.playerCurrentMoney << "$ \n";
}

void swapHighScores(highScore& score1, highScore& score2){

    char auxName[MAX_PLAYER_NAME_LENGTH];

    strcpy(auxName, score1.name);
    strcpy(score1.name, score2.name);
    strcpy(score2.name, auxName);

    int auxScore;
    auxScore = score1.score;
    score1.score = score2.score;
    score2.score = auxScore;

}

void sortHighScores(highScore scores[], int arrayLength) {

    int i;
    bool sorted;
    while(!sorted) {
        sorted = true;
        for(i = 1; i <= arrayLength-1; ++i) {
            if(scores[i].score < scores[i+1].score) {
                swapHighScores(scores[i], scores[i+1]);
                sorted = false;
            }
        }
    }

}



void updateHighScores(int numberOfPlayers) {

    highScore scores[100];
    char pos[3];
    int i, lastScore;

    ifstream fin("highscores.in");

    lastScore = 1;

    while(fin>>pos){
        fin>>scores[lastScore].name>>scores[lastScore].score;
        lastScore++;
    }

    for(i = 1; i <= numberOfPlayers; ++i){
        strcpy(scores[lastScore].name, playersArray[i].name);
        scores[lastScore].score = playersArray[i].playerMaximumEverMoney;
        lastScore++;
    }

    sortHighScores(scores, lastScore);

    fin.close();
    ofstream fout("highscores.in");

    if(lastScore > 10) lastScore = 11;
    for(i = 1; i < lastScore; ++i){
       fout << i << ". " << scores[i].name << " " << scores[i].score << '\n';
    }

}


void checkHighScores(){
    highScore scores[100];

    int lastScore = 1;
    char pos[3];

    ifstream fin("highscores.in");
    while(fin>>pos){
        fin>>scores[lastScore].name>>scores[lastScore].score;
        cout << lastScore << ". " << scores[lastScore].name << " " << scores[lastScore].score << '\n';
        lastScore++;
    }

}

int main() {

    srand(time(NULL));

    mainMenu();

    int chosenValue = getChosenVal(1, 3);

    if ( chosenValue == 3) {
        cout << "Good bye\n";
        return 0;
    }

    if( chosenValue == 1 ) {
        clearScreen();
        choseGame();
    }

    if( chosenValue == 2 ){
        checkHighScores();
        cout << "Enter 1 to go exit: ";
        chosenValue = getChosenVal(1, 1);
        return 0;
    }


    return 0;
}


void mainMenu() {

    cout << "1. New game\n";
    cout << "2. Check high scores\n";
    cout << "3. Exit\n";
    cout << "Enter value: ";
}

void clearScreen() {
#ifdef __linux
    system("clear");
#elif _WIN32
    system("CLS");
#elif _WIN64
    system("CLS");
#endif
}

int readValue() {

    char s[1000];
    cin.getline(s, 1000);

    if(s[1] != '\0') {
        cout << "entered value is not a digit, retype value: ";
        readValue();
    } else {
        if(s[0] >= '0' && s[0] <= '9' ) {
            return s[0] - '0';
        } else {
            cout << "entered value is not a digit, retype value: ";
            readValue();
        }
    }

}

void readPlayerName(Player& participant) {
    cout << "Your name is: ";
    cin.getline(participant.name, MAX_PLAYER_NAME_LENGTH);
    char *spacePointer = strchr(participant.name, ' ');
    if(spacePointer != NULL) *spacePointer = '\0';
}

void setPlayerMoney(int i) {
    playersArray[i].playerCurrentMoney = PLAYER_MIN_START_SUM;
    playersArray[i].playerMaximumEverMoney = PLAYER_MIN_START_SUM;
}

void choseGame() {

    cout << "1. Single player\n";
    cout << "2. Multiplayer\n";

    int chosenValue = getChosenVal(1, 2);
    playGame(chosenValue);

}

void playGame(int typeOfGame) {

    int numberOfPlayers, i;

    if(typeOfGame == 1) {
        numberOfPlayers = 1;
    }

    if(typeOfGame == 2) {
        int value;
        do {
            cout << "Number of players: ";
            value = readValue();
            cout << '\n';
        } while(value > MAX_PLAYERS_AT_TABLE-1 || value <= 1);
        numberOfPlayers = value;
    }

    for(i = 1; i <= numberOfPlayers; ++i) {

        readPlayerName(playersArray[i]);
        setPlayerMoney(i);
        playersArray[i].cardsStackTop = -1;

    }
    int nextHandKey;
    while(isOver(numberOfPlayers) == false) {

        reset(numberOfPlayers);

        for(i = 0; i <= numberOfPlayers; ++i) {
            if(!playersOut[i])
                giveFirstCards(playersArray[i]);
        }
        for(i = 1; i <= numberOfPlayers; ++i) {
            if(!playersOut[i]) {
                cout << "\n----------------------- " << playersArray[i].name << " -----------------------\n\n";
                playHand(playersArray[i]);
            }

            //clearScreen();
        }

        dealerGame(numberOfPlayers);
        for(i = 1; i <= numberOfPlayers; ++i) {
            if(!playersOut[i])
                showCurrentMoney(playersArray[i]);
        }
        cout << "Enter 1 for the next round...\n";
        nextHandKey = getChosenVal(1, 1);
        clearScreen();
    }

    updateHighScores(numberOfPlayers);


}

void dealerGame(int numberOfPlayers) {

    cout << "\n----------------------- " << "Dealer"<< " -----------------------\n\n";

    showDealersCards();
    int i, j;
    playersArray[0].cardSum = calculateSumOfCards(playersArray[0]);
    while(playersArray[0].cardSum < 17) {

        addCard(playersArray[0]);
        playersArray[0].cardSum = calculateSumOfCards(playersArray[0]);
        showDealersCards();

    }

    for(i = 1; i <= numberOfPlayers; ++i) {

        if(playersOut[i]){
            continue;
        }


        if(playersArray[i].hasSplit) {
            for(j = 0; j < 2; ++j) {
                if(playersArray[i].cardSumOnSplit[j] <= 21 && (playersArray[i].cardSumOnSplit[j] > playersArray[0].cardSum || playersArray[0].cardSum  > 21)  ) {
                    playersArray[i].playerCurrentMoney += playersArray[i].currentBet;
                    cout << playersArray[i].name << " won " << playersArray[i].currentBet / 2 << "$\n";
                } else {
                    cout << "Dealer won " << playersArray[i].currentBet/2 << "$ from " << playersArray[i].name << '\n';
                }
            }
        } else if(playersArray[i].hasSurrendered) {
            cout << "Dealer won " << playersArray[i].currentBet<< "$ from " << playersArray[i].name << '\n';
        } else {



            if(playersArray[i].cardsStackTop == 1 && playersArray[i].cardSum == 21){
                if(playersArray[0].cardsStackTop == 1 && playersArray[0].cardSum == 21){
                    playersArray[i].playerCurrentMoney += playersArray[i].currentBet;
                    cout << "Both " << playersArray[i].name << " and Dealer" << " have got a blackjack " << "returning money to " << playersArray[i].name << '\n';
                } else {
                    playersArray[i].playerCurrentMoney += 2*playersArray[i].currentBet + playersArray[i].currentBet/2;
                    cout << "Player " << playersArray[i].name << " got a blackjack and won" << playersArray[i].currentBet + playersArray[i].currentBet/2 << " $\n";
                }
            } else {
                if(playerWon(playersArray[i])) {
                    playersArray[i].playerCurrentMoney += 2*playersArray[i].currentBet;
                    cout << playersArray[i].name << " won " << playersArray[i].currentBet << "$\n";
                } else {
                    cout << "Dealer won " << playersArray[i].currentBet << "$ from " << playersArray[i].name << '\n';
                }
            }


        }

        if(playersArray[i].playerCurrentMoney > playersArray[i].playerMaximumEverMoney) {
            playersArray[i].playerMaximumEverMoney = playersArray[i].playerCurrentMoney;
        }

        if(playersArray[i].playerCurrentMoney <= 10 ) {
            playersOut[i] = true;
        }

        playersArray[i].currentBet = 0;

    }

}


void showDealersCards() {
    cout << "Dealer's cards: ";
    int i;
    for(i = 0; i <= playersArray[0].cardsStackTop; ++i) {
        showCardLetterOrDigit(playersArray[0].cardsStack[i]);
    }
    cout << '\n';
}

void giveFirstCards(Player& participant) {

    int i;
    for(i = 0; i < 2; ++i) {
        addCard(participant);
    }

}

void addCard(Player& participant) {
    int randomCard;
    do {
        randomCard = rand()%13 + 1;
    } while(blackJackDeck[randomCard] >= randomCard*4);

    participant.cardsStack[++participant.cardsStackTop] = randomCard;
    blackJackDeck[randomCard]++;
}


void reset(int numOfPlayers) {
    int i;
    for(i = 0; i <= numOfPlayers; ++i) {
        resetStack(playersArray[i]);
        playersArray[i].currentBet = 0;
        playersArray[i].hasSplit=false;
    }

    for(i = 1; i <= 13; ++i) {
        blackJackDeck[i] = 0;
    }
}

void resetStack(Player& participant) {

    while(participant.cardsStackTop >= 0) {
        participant.cardsStack[participant.cardsStackTop--] = 0;
    }

}

bool isOver(int numberOfPlayers) {
    int i;
    for(i = 1; i <= numberOfPlayers && playersOut[i]; ++i);
    return i > numberOfPlayers;
}

void getDealerShownCard() {
    cout << "\n";
    cout << "Dealer's shown card: ";
    showCardLetterOrDigit(playersArray[0].cardsStack[0]);
    cout << "\n\n";
}

void showCards(Player participant) {

    cout <<"\n";

    cout << participant.name << "'s cards: ";

    int i;
    for(i = 0; i <= participant.cardsStackTop; ++i) {
        showCardLetterOrDigit(participant.cardsStack[i]);
    }

    cout <<"\n\n";

}

void showCardLetterOrDigit(int cardValue) {
    switch(cardValue) {
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

void decisionMenu() {

    cout << "1. Hit\n";
    cout << "2. Stand\n";
    cout << "3. Double down\n";
    cout << "4. Split\n";
    cout << "5. Surrender\n";
    cout << "6. Show cards\n";
    cout << "7. See dealer's shown card\n";
    cout << "8. Show decision menu\n\n";


}

bool checkBusted(Player participant) {
    return participant.cardSum > 21;
}


bool checkBlackJack(Player participant) {
    return participant.cardSum == 21;
}

bool playerWon(Player participant) {
    return (participant.cardSum > playersArray[0].cardSum || playersArray[0].cardSum > 21) && !checkBusted(participant);
}

void playerBusted(Player participant) {

    cout << "Player " << participant.name << " is BUSTED\n";
    showCards(participant);

}

void playerBlackJack(Player participant) {

    cout << "Player " << participant.name << " hit 21\n";
    showCards(participant);

}

void playHand(Player& participant) {

    placeBets(participant);
    playLoop(participant);

}

void playLoop(Player& participant) {

    participant.cardSum = calculateSumOfCards(participant);

    if( checkBlackJack(participant) ) {
        playerBlackJack(participant);
        return;
    }

    int chosenVal = -1;
    bool gameIsOver = false;

    if(!participant.hasSplit) {
        showCards(participant);
        decisionMenu();
    }



    while(!gameIsOver) {

        cout << "Choose menu value: ";


        chosenVal = getChosenVal(1, 8);

        cout <<"\n";

        if(chosenVal == 2) {
            gameIsOver = true;
        }

        if(chosenVal == 1) {
            hitDecision(participant);

            if(checkBusted(participant)) {
                playerBusted(participant);
                gameIsOver = true;
            }

            if(checkBlackJack(participant)) {
                playerBlackJack(participant);
                gameIsOver = true;
            }
        }
        if(chosenVal == 3 && doubleDownDecision(participant)) {

            if(checkBusted(participant)) {
                playerBusted(participant);
            }

            if(checkBlackJack(participant)) {
                playerBlackJack(participant);
            }

            gameIsOver = true;

        }

        if(chosenVal == 4) {
            if(!participant.hasSplit) {
                if(splitDecision(participant)) {
                    gameIsOver = true;
                }
            } else {
                cout << "You cannot split\n";
            }
        }

        if(chosenVal == 5) {
            if(!participant.hasSplit) {
                surrenderDecision(participant);
                gameIsOver = true;
            } else {
                cout << "You cannot surrender\n";
            }

        }

        if(chosenVal == 8) {
            decisionMenu();
        }

        if(chosenVal == 6) {
            showCards(participant);
        }

        if(chosenVal == 7) {
            getDealerShownCard();
        }

    }



}

bool doubleDownDecision(Player& participant) {

    cout << "Player " << participant.name << " chose to double down\n";

    if(participant.cardsStackTop > 1) {
        cout << "You cannot double down, sorry\n";
        return false;
    }

    if(participant.playerCurrentMoney - participant.currentBet < 0 ) {
        cout << "You do not have the money for a double down\n";
        return false;
    }

    addCard(participant);
    participant.playerCurrentMoney -= participant.currentBet;
    participant.currentBet *= 2;
    participant.cardSum = calculateSumOfCards(participant);
    showCards(participant);
    return true;
}

bool splitDecision(Player& participant) {

    cout << "Player " << participant.name << " " << " decided to split\n";


    if(participant.cardsStack[0] != participant.cardsStack[1] || participant.cardsStackTop > 1 || participant.hasSplit) {
        cout << "You cannot split\n";
        return false;
    }

    if(participant.playerCurrentMoney - participant.currentBet < 0) {
        cout << "You do not have enough money to split\n";
        return false;
    }

    participant.hasSplit = true;
    participant.playerCurrentMoney  -= participant.currentBet;
    participant.currentBet *= 2;
    int baseOfNextStack = participant.cardsStack[participant.cardsStackTop--];

    if(participant.cardsStack[0] == 1) {

        cout << "-----FIRST STACK----\n";
        addCard(participant);
        participant.cardSumOnSplit[0] = calculateSumOfCards(participant);

        showCards(participant);
        resetStack(participant);

        cout << "-----SECOND STACK----\n";
        participant.cardsStack[++participant.cardsStackTop] = baseOfNextStack;
        addCard(participant);
        participant.cardSumOnSplit[1] = calculateSumOfCards(participant);

        showCards(participant);
        resetStack(participant);

        return true;
    }

    cout << "-----FIRST STACK----\n";



    addCard(participant);
    showCards(participant);

    playLoop(participant);
    participant.cardSumOnSplit[0] = participant.cardSum;


    resetStack(participant);

    cout << "-----SECOND STACK----\n";

    participant.cardsStack[++participant.cardsStackTop] = baseOfNextStack;
    addCard(participant);
    showCards(participant);

    playLoop(participant);
    participant.cardSumOnSplit[1] = participant.cardSum;

    return true;

}

void surrenderDecision(Player& participant) {

    if(participant.cardsStackTop > 1 || participant.hasSplit) {
        cout << "You cannot surrender\n";
        return;
    }

    participant.playerCurrentMoney += participant.currentBet / 2;
    participant.currentBet /= 2;

    participant.hasSurrendered = true;

}



int getChosenVal(int left, int right) {
    int chosenVal;
    bool firstTime = true;
    do {
        if(firstTime != true) {
            cout << "You entered a wrong value, please retype the value: ";
        }
        firstTime = false;
        chosenVal = readValue();
    } while(chosenVal > right || chosenVal < left);
    return chosenVal;


}

void hitDecision(Player& participant) {

    cout << '\n' << participant.name << " decided to hit " << '\n';

    addCard(participant);
    participant.cardSum = calculateSumOfCards(participant);
    showCards(participant);

}

int calculateSumOfCards(Player participant) {
    int i, softHand = 0, hardHand = 0;
    for(i = 0; i <= participant.cardsStackTop; ++i) {
        if(participant.cardsStack[i] > 10) {
            softHand += 10;
            hardHand += 10;
        } else if(participant.cardsStack[i] == 1) {
            softHand += 1;
            hardHand += 11;
        } else {
            softHand += participant.cardsStack[i];
            hardHand += participant.cardsStack[i];
        }
    }

    if(hardHand > 21) {
        return softHand;
    }
    return hardHand;
}

void placeBets(Player& participant) {

    cout << participant.name << ' ' << "you have : " << participant.playerCurrentMoney << "$\n";
    cout << participant.name << ' ' << "your bet is:\n";

    cout << "1. 10$\n";
    cout << "2. 25$\n";
    cout << "3. 50$\n";
    cout << "4. 100$\n";
    cout << "5. 200$\n";

    cout << "Enter bet value: ";

    int chosenValue;
    bool isBetable = false;

    while(!isBetable) {
        isBetable = true;
        chosenValue = getChosenVal(1, 5);
        if(participant.playerCurrentMoney < bets[chosenValue]) {
            cout << "You don't have enough money for this bet, retype the bet: ";
            isBetable = false;
        }
    }


    participant.currentBet = bets[chosenValue];
    participant.playerCurrentMoney -= participant.currentBet;

}


