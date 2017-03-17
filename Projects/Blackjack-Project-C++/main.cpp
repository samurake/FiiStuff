// A simple console blackjack game made in first semester of university.
// Unique idea, you can play with up to 10 players, using files to store cards.
// Using files to store players, the limit can be modified.
// Stage: Final. Maybe a few bugs.
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <fstream>

using namespace std;
ifstream cardsave("cards.txt");
ifstream playerstats("players.txt");

struct CardS
{
    int all[52];
    int taken[52];
};

struct BJplayers
{
    int playcards[10][10];
    int bet[10];
    int funds[10];
    int score[10];
    int lost[10];
    int cardscounter[10];
    bool dealernotake;
};

int card1dealer;
int card2dealer;
int cardtotaldealer;
int playerscount;
int cardcounter = 2;
int over=0;
CardS Deck;
BJplayers players;
int random(int min, int max);

void dealerlost();
void dealerhasblackjack();
void playershaveblackjack();
void dealerwin();
void greatscorewin();
void replay();
void endgame();

string hitstick;

void newGame();
void playTheGame();
void dealCards();
void dealAnotherCard(int i);
void dealerPlay();
void youLose();
void youWin();

int takeCard(CardS Deck)
{
    int randomposition,ok=0;
    randomposition = random(0,52);
    while(ok==0)
    {
        if(Deck.taken[randomposition]==1)
            randomposition = random(0,52);
        else
        {
            Deck.taken[randomposition]=1;
            ok=1;
        }
    }
    return Deck.all[randomposition];
}

int main()
{
    int x,i=0;
    while(cardsave>>x)
    {
        Deck.all[i]=x;
        Deck.taken[i]=0;
        i++;
    }
    newGame();
}

int random(int min, int max) //range : [min, max)
{
   static bool first = true;
   if ( first )
   {
      srand(time(NULL)); //seeding for the first time only!
      first = false;
   }
   return min + rand() % (max - min);
}

void checkCards()
{
    int j=0;
    if(cardtotaldealer > 21)
        dealerlost();
    else if(cardtotaldealer == 21)
        dealerhasblackjack();
    else
    if(cardtotaldealer<21)
    {
        for(int i=0;i<playerscount;i++)
        {
            if(players.score[i]==21)
                playershaveblackjack();
        }
    }
    for(int i=0;i<playerscount;i++)
    {
        if(players.score[i]>21 && players.lost[i]!=1)
        {
            players.lost[i] = 1;
            players.funds[i] = players.funds[i] - players.bet[i];
            cout<< "Player " << i+1 << " lost his bet !"<< endl;
        }
    }
    for(int i=0;i<playerscount;i++)
        if(players.lost[i]==1)
            j++;
    if(playerscount==j)
        dealerwin();
    if(players.dealernotake==true)
        greatscorewin();
}

void newGame()
{
    players.dealernotake = false;
    for(int i=0;i<52;i++)
        Deck.taken[i]=0;
    system("cls");
    cout << "BlackJack" << endl;
    cout << "*********" << endl;
    cout << "Welcome to BlackJack!" << endl;
    playerstats>>playerscount;
    cout << "There are " << playerscount+1 << " players in this game with The Dealer." << endl;
    for(int i=0;i<playerscount;i++)
    {
        playerstats>>players.funds[i];
        cout << "Player " << i+1 << " funds are: " << players.funds[i] << endl;
        players.cardscounter[i]=2;
        players.lost[i]=0;
    }
    for(int i=0;i<playerscount;i++)
    {
        cout<<endl;
        cout << "Player " << i+1 << " will write his bet: " << endl;
        cin >> players.bet[i];
        cout << endl;
        if(players.bet[i]>players.funds[i])
        {
            cout<<"Not enough funds, insert a lower bet !";
            cout << endl;
            cin >> players.bet[i];
            cout << endl;
        }
        players.funds[i]=players.funds[i]-players.bet[i];
    }
    cin.get();
    cout << "Please press ENTER twice to draw all players first cards:" << endl;
    cin.get();
    dealCards();
    for(int i=0;i<playerscount;i++)
    {
        cout<< "Player " << i+1 << " Play Cards are: ";
        for(int j=0;j<2;j++)
        {
            cout<<players.playcards[i][j]<<" ";
        }
        cout<<endl;
        cout << "Player " << i+1 << " score is: " << players.score[i] << endl;
    }
    cout << endl;
    cout << "Dealer's Cards are: " << card1dealer << " " <<card2dealer << endl;
    cardtotaldealer = card1dealer + card2dealer;
    cout << "Dealer's card total is: " << cardtotaldealer << endl;
    cin.get();
    checkCards();
    playTheGame();
}

void playTheGame()
{
    for(int i=0;i<playerscount;i++)
    {
        if(players.lost[i]!=1)
        {
            cout << endl;
            cout <<"Player "<<i+1<<" decision:"<< "Do you want to 'Hit' or 'Stick' ?" << endl;
            cin >> hitstick;
            if (hitstick=="Hit" || hitstick=="hit")
                dealAnotherCard(i);
            if (hitstick=="Stick" || hitstick=="stick")
            {
                cout<<endl;
                cout<<"Player "<<i+1<<" decided to Stick!"<<endl;
            }
        }
    }
    dealerPlay();
    checkCards();
    if(over==0)
        playTheGame();

}


void dealCards() {

    for(int i=0;i<playerscount;i++)
    for(int j=0;j<2;j++)
    {
        players.playcards[i][j] = takeCard(Deck);
        players.score[i] = players.score[i] + players.playcards[i][j];
        players.cardscounter[i]=2;
    }
    card1dealer = takeCard(Deck);
    card2dealer = takeCard(Deck);
}

void dealAnotherCard(int i)
{
    int currentcard,cardcounter=players.cardscounter[i];
    currentcard = takeCard(Deck);
    players.cardscounter[i]=players.cardscounter[i]+1;
    cout << "Player " <<i+1<<" card is: "<<currentcard<<endl;
    players.playcards[i][cardcounter]=currentcard;
    players.score[i]=players.score[i]+currentcard;
    cout<< "Your card score is: " << players.score[i] << endl;

}

void dealerPlay() {
     int cardtaken;
     if(cardtotaldealer < 17)
     {
         cardtaken = takeCard(Deck);
         cardtotaldealer = cardtotaldealer + cardtaken;
         cout<< endl;
         cout<< "Dealer take :" << cardtaken << endl;
         cout<< "Dealer total is :" << cardtotaldealer << endl;
     }
}

void replay()
{
     players.dealernotake = false;
     for(int i=0;i<playerscount;i++)
     {
         players.bet[i]=0;
         players.score[i]=0;
         players.lost[i]=0;
         for(int j=0;j<players.cardscounter[i];j++)
            players.playcards[i][j]=0;
         players.cardscounter[i]=0;
     }
     string playagain;
     cout << "Do you wan't to play again?[Y]/[N]";
     cin >> playagain;
     if(playagain == "Y" || playagain == "y")
     {
         system("cls");
         newGame();
     }
     if(playagain == "N" || playagain == "n")
     {
        endgame();
     }
}
void dealerlost()
{
    cout<< endl;
    cout<< "Dealer Lost!" <<endl;
    for(int i=0;i<playerscount;i++)
    {
        if(players.lost[i]!=1)
        {
            cout<<endl;
            cout<< "Player " <<i+1<<" won 2X"<<players.bet[i]<<"!"<<endl;
            players.funds[i]=players.funds[i]+players.bet[i]*2;
        }
        else
            players.funds[i]=players.funds[i]-players.bet[i];
    }
    replay();

}
void dealerhasblackjack()
{
    cout<< endl;
    cout<< "Dealer has blackjack! Players with Blackjack will have they're bet increased by half." << endl;
    for(int i=0;i<playerscount;i++)
    {
        if(players.score[i]==21)
        {
            cout<<endl;
            cout<<"Player "<<i+1<<" won (1/2)X"<<players.bet[i]<<"!"<<endl;
            players.funds[i]=players.funds[i]+players.bet[i]*(1/2);
        }
        else
            players.funds[i]=players.funds[i]-players.bet[i];
    }
    replay();
}
void dealerwin()
{
    cout<<endl;
    cout<< "All players LOST !"<<endl;
    for(int i=0;i<playerscount;i++)
    {
        players.funds[i]=players.funds[i]-players.bet[i];
    }
    replay();
}
void playershaveblackjack()
{
    cout<<endl;
    cout<< "Dealer Lost. Players with blackjack will have they're bet tripled" << endl;
    for(int i=0;i<playerscount;i++)
    {
        if(players.score[i]==21)
        {
            cout<<endl;
            cout<<"Player "<<i+1<<" won 3X"<<players.bet[i]<<"!"<<endl;
            players.funds[i]=players.funds[i]+players.bet[i]*3;
        }
        else
            players.funds[i]=players.funds[i]-players.bet[i];
    }
    replay();
}
void greatscorewin()
{
    cout<<endl;
    cout<< "Dealer reached his maximum cards. Players with the greatest score won.";
    int maxx=0;
    for(int i=0;i<playerscount;i++)
    {
        if(players.lost[i] != 1 && players.score[i]>maxx)
            maxx=players.score[i];
    }
    for(int i=0;i<playerscount;i++)
        if(players.score[i]==maxx)
        {
            cout<<endl;
            cout<<"Player "<<i+1<<" won 2X"<<players.bet[i]<<"!"<<endl;
            players.funds[i]=players.funds[i]+players.bet[i]*2;
        }
        else
            players.funds[i]=players.funds[i]-players.bet[i];
    replay();
}
void endgame()
{
    over=1;
    cout<<"The Game is now over!";
}
