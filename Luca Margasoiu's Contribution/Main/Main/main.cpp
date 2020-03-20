
//
//  main.cpp
//  Hello World
//
//  Created by user on 28/01/2020.
//  Copyright © 2020 user. All rights reserved.
//

#include <iostream>
#include <string.h>
#include <fstream>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>
#include <time.h>
#include <math.h>
using namespace std;


//Global variables, maps, arrays, game map and battle UI.
map<char,class Block> usermap;
map<pair<int, int>, class Pocket_Monsters> enemyLocations;
vector<class Pocket_Monsters> PlayersPokemons;
char gameMap1[40][40];
string battleUI[17];
string UIcopy[17];


//Function that changes the map

char changeMap(int x1,int y1, int x2, int y2,char inc){
    char temp=gameMap1[x2][y2];
    gameMap1[x2][y2]=gameMap1[x1][y1];
    gameMap1[x1][y1]=(int)inc;
    return temp;
}

//Function that prints the map

void printMap(int x=10){
    cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
    for(int i=0;i<x;i++){
        for(int j=0;j<30;j++){
            cout<<gameMap1[i][j];
        }
        cout<<endl;
    }
}

//Function tht prints the battle ui; Called when battle starts or new round beginds
void printBattleUI(){
    for(int i=0;i<17;i++)
        cout<<battleUI[i]<<endl;
}







class Pocket_Monsters{
public:
    char initial;
    string type;
    string weakness;
    int level;
    int xp;
    // IN EACH ARRAY, FIRST MEMBER IS BASE VALUE, SECOND ONE IS ACTUAL VALUE CALC. BY LEVEL AND BASE VALUE AND THIRD IS BONUSES TO THAT CATEGORY
    float attack[3];
    float health[3];
    float defence[3];
    float dodgeChance[3];
    float battleStats[4]; //The stats that the pokemon uses to go in battle.
    int numberOfBattles;
    bool isDowned;
    //We might use a mana type system for the future in order to diversify the way attacks work
    
    
    
    Pocket_Monsters(){
        
    }
    
    //Constructor;
    Pocket_Monsters(char i,string t, string w, initializer_list<float> bStats){
        initial=i;
        type=t;
        weakness=w;
        attack[0]=attack[1]=bStats.begin()[0];
        health[0]=health[1]=bStats.begin()[1];
        defence[0]=defence[1]=bStats.begin()[2];
        dodgeChance[0]=dodgeChance[1]=bStats.begin()[3];
        isDowned=false;
    }
    //This function is called after a battle ends. If a pokemon has enough xp for lvl up it goes to assignValues
    void isXpEnoughForLvlUp(bool k){
        this->xp+=10*(1+k);
        if(this->xp>level*1.5){
            cout<<"Your pokemon has leveled up!";
            this->assignValues(this->level+1);
        }
    }
    //This assigns values after battle
    void assignValues(int lvl){
        this->level=lvl;
        for(int i=1;i<level;i++){
            this->calculateAV();
        }
    }
    //This calculates Actual Values
    void calculateAV(){
        this->attack[1]=(int)(this->attack[1]+.1*this->attack[0]);
        this->health[1]=(int)(this->health[1]+.1*this->health[0]);
        this->defence[1]=(int)(this->defence[1]+.1*this->defence[0]);
        this->dodgeChance[1]=(int)(this->dodgeChance[1]+.1*this->dodgeChance[0]);
        this->battleStats[0]=this->attack[1];
        this->battleStats[1]=this->health[1];
        this->battleStats[2]=this->defence[1];
        this->battleStats[3]=this->dodgeChance[1];
    }
    
    void reassignBattleStats(){
        this->battleStats[0]=this->attack[1]+this->attack[2];
        this->battleStats[1]=this->health[1]+this->health[2];
        this->battleStats[2]=this->defence[1]+this->defence[2];
        this->battleStats[3]=this->dodgeChance[1]+this->dodgeChance[2];
    }
    //Oveloading the output operator
    friend ostream &operator<<(ostream &output, Pocket_Monsters &temp);
    
    //Overloading the -= operator. Used to calculate damage between pokemons during the battle
    Pocket_Monsters operator-=(Pocket_Monsters &x){
        Pocket_Monsters temp;
        int random = ((int)rand())%10-5; // this int caclled random will get a number between -5 and 5.
        int dodge = (int)rand()%100;
        int damagedealt;
        if(this->battleStats[3]>dodge)
            damagedealt=0;
        else
            damagedealt = this->battleStats[1]-(x.battleStats[0]-x.battleStats[0]*this->battleStats[2]*pow(10,-3))+random + 20;
        // formula that calcultes damage dealt
        //Formula is really buggy
        if(this->weakness==x.type)
            damagedealt= damagedealt+damagedealt*.2;
        this->battleStats[1]=this->battleStats[1]-damagedealt;
        return *this;
    }
};

ostream &operator<<(ostream &output, Pocket_Monsters &temp)
{
    output<<"This pokemon is of type ";
    output<<temp.type;
    output<<endl;
    output<<"This pokemon's weakness is ";
    output<<temp.weakness;
    output<<endl;
    output<<"This pokemon has level ";
    output<<temp.level;
    output<<endl;
    output<<"This pokemon is ";
    if(!temp.isDowned)
        output<<"not ";
    output<<"downed";
    output<<endl;
    output<<"The pokemon has the following stats:"<<"Atack: "<<temp.attack[1]<<", Health: "<<temp.health[1]<<", Defence: "<< temp.defence[1]<<", Dodge Change: "
    <<temp.dodgeChance[1];
    output<<endl;
    
    return output;
}





void changeBattleUI(Pocket_Monsters enemy, Pocket_Monsters ally){
    
    // 2 16   4 7    4 12
    string HP= to_string(int(enemy.health[1]));
    string curHP=to_string(int(enemy.battleStats[1]));
    string temp;
    battleUI[2].replace(17,1,temp=to_string((int)enemy.level));
    battleUI[4].replace(7,1,temp=enemy.initial);
    battleUI[4].replace(12, (curHP+"/"+HP).length(), curHP+"/"+HP);
    
    HP= to_string(int(ally.health[1]));
    curHP=to_string(int(ally.battleStats[1]));
    battleUI[8].replace(17,1,temp=to_string((int)ally.level));
    battleUI[10].replace(7,1,temp=ally.initial);
    battleUI[10].replace(12, (curHP+"/"+HP).length(), curHP+"/"+HP);
    
    printBattleUI();
    for(int i=0;i<18;i++){
        copy(UIcopy,UIcopy+17,battleUI);
    }
}





bool engageBattle(Pocket_Monsters enemy, Pocket_Monsters ally){
    // FOR FUTURE - ENEMY WILL GIVE DEBUFF TO YOU
    
    enemy.reassignBattleStats();
    ally.reassignBattleStats();
    changeBattleUI(enemy,ally);
    int turnnumber=0;
    while(1){
        if(enemy.battleStats[1]<0)
            return true;
        if(ally.battleStats[1]<0)
            return false;
        
        if(turnnumber%2==0){
            int in;
            cin>>in;
            switch (1) {
                case 1:
                    enemy-=ally;
                    break;
                case 2:
                    //Nothing happens...
                    break;
                case int(3):
                    return false;
                    break;
            }
            if(in==3){
                return false;
                
            }
        }
        else
            ally-=enemy;
        turnnumber++;
        changeBattleUI(enemy,ally);
    }
}

class Block{
private:
    string color;
    string type;
    float eP; //Encounter Probability
    
public:
    char representation; //What character will be used to show this
    bool isPassable; // If you can go through the block
    bool canEnter; // if you can enter the block I. E. shop. So far not used
    
    //Constructors
    Block(char rep, string clr, string typ, float P=0.0, bool passable=false, bool enter=false){
        representation=rep;
        color=clr;
        type=typ;
        eP=P;
        isPassable=passable;
        canEnter=enter;
    }
    Block(){
        isPassable=NULL;
        representation=NULL;
        color.clear();
        type.clear();
        eP=NULL;
        canEnter=NULL;
    }
    
    friend ostream &operator<<(ostream &output, Block &temp);
    
    // function that chagnes block values. Used when
    void changeValues(Block& x){
        Block temp(this->representation,this->color,this->type,this->eP,this->isPassable,this->canEnter);
        this->representation=x.representation;
        this->color=x.color;
        this->type=x.type;
        this->eP=x.eP;
        this->isPassable=x.isPassable;
        this->canEnter=x.canEnter;
        
    }
    
    void printValues(){
        cout<<this->isPassable<<endl;
        cout<< this->color<<endl;
        cout<< this->representation<<endl;
        cout<< this->type<<endl;
        cout<< this->eP<<endl;
    }
    
};


ostream &operator<<(ostream &output, Block& temp)
{
    output<<temp.isPassable<<endl;
    output<<temp.canEnter<<endl;
    output<<temp.color<<endl;
    output<<temp.type<<endl;
    output<<temp.representation<<endl;
    output<<temp.eP<<endl<<endl;
    return output;
}




class PlayerClass: public Block{
    
    using Block::Block;
    
    int ammountOfPBalls;
    int currency;
    int numberOfPotions;
    int positionX=5;
    int positionY=19;
    
public:
    
    void changePosition(int x, int y){
        this->positionX=x;
        this->positionY=y;
    }
    
    
    void moveCharacter(char keyPress){
        pair<int,int> coordInitial(this->positionX, this->positionY);// The initial coordinates of the player
        pair<int,int> coordMove;// The coordinates that the player wants to move into. Has a different value for each case of movement.
        char var;
        keyPress=tolower(keyPress);
        if(keyPress=='w' or keyPress=='a' or keyPress=='s' or keyPress== 'd'){
            switch(keyPress){
                case 'w':
                    coordMove.first=this->positionX-1;
                    coordMove.second=this->positionY;
                    var = gameMap1[(this->positionX)-1][this->positionY];
                    break;
                    
                case 'a':
                    coordMove.first=this->positionX;
                    coordMove.second=this->positionY-1;
                    var = gameMap1[this->positionX][this->positionY-1];
                    break;
                    
                case 's':
                    coordMove.first=this->positionX+1;
                    coordMove.second=this->positionY;
                    var = gameMap1[this->positionX+1][this->positionY];
                    break;
                case 'd':
                    coordMove.first=this->positionX;
                    coordMove.second=this->positionY+1;
                    var = gameMap1[this->positionX][this->positionY+1];
                    break;
            }
            if(var=='E'){ // If the tile we want to move into is an enemy
                pair<int,int> t;
                t.first=coordMove.first;
                t.second=coordMove.second;
                auto search = enemyLocations.find(t);
                if(search==enemyLocations.end())
                    cout<<"There has occured an error";
                else{
                    bool x=engageBattle(enemyLocations[t],PlayersPokemons[0]);
                    if(!x){
                        cout<<"Battle is over! The enemy still stands!";
                        return;
                    }
                    else{
                        gameMap1[t.first][t.second]=' ';
                        var=' ';
                        cout<<"You won! The enemy has been defeated!"<<endl;
                    }
                    PlayersPokemons[0].isXpEnoughForLvlUp(x);
                }
            }
        }
        cin.get();
        if(usermap[var].isPassable){
            changeMap(coordInitial.first, coordInitial.second, coordMove.first, coordMove.second, this->Block::representation);
            this->changeValues(usermap[var]);
            this->changePosition(coordMove.first, coordMove.second);
        }
        
    }
};


int main(int num_args, char** arg_strings){
    
    srand( static_cast<unsigned int>(time(nullptr)));//FOUND ON THE INTERNET
    
    
    Pocket_Monsters EarthMonster('E',"Earth","Fire",{10,110,25,10}); // Hefty pokemon that is able to withstand a beating and persevere through a big HP pool and Defence.
    Pocket_Monsters FireMonster('F',"Fire","Water",{20,50,10,10}); // Hard hitting pokemon that is low on health but packs a punch.
    Pocket_Monsters WaterMonster('W',"Water","Wind",{15,75,15,15}); // Jack of all trades pokemon that doesn't have one great skill but is a solid pick and very reliable.
    Pocket_Monsters WindMonster('A',"Air","Earth",{15,50,10,20}); // Agile pokemon that strives to win the fight through it's ability to reach an insane dodge chance.
    
    cout<<endl<<endl; // Code from some forum
    string line;
    ifstream myfile( "BattleUI.txt" );
    if (myfile)
    {
        int i=0;
        while (getline( myfile, line )){
            battleUI[i]=line;
            i++;
        }
        myfile.close(); // Code from some forum - end
    }
    for(int i=0;i<18;i++){
        copy(battleUI,battleUI+18,UIcopy);
    }
    
    Pocket_Monsters Monster1=EarthMonster;
    Monster1.assignValues(3);
    
    Pocket_Monsters Monster2=WindMonster;
    Monster2.assignValues(2);
    
    
    Block Forest('F', "Dark Green", "Forest", 0.23, true);
    Block Mountain('M', "Gray","Mountain");
    Block Grass('G',"Light Green","Grass",0.30, true);
    Block Paveway(' ', "No colour","Paveway",0, true);
    Block PlayerBlock ('C',"Blue","Player");
    Block Enemy ('E',"Red","Enemy",0,true);
    PlayerClass PlayerInfo(' ', "No colour","Paveway", true);
    
    //    Forest.printValues();
    //    cout<<Forest;
    //
    
    
    
    
    // <READING AND STORING THE GAME MAP>
    string temp;
    
    ifstream in;
    in.open("mapfile.txt");
    if(in.fail()){
        cout<<"Error Opening File"<<endl;
    }
    
    int j=0,nrColumns=1,i=0,k=0;
    while (getline(in, temp,'\0')){
        for(j=0;j<temp.length();j++){
            if(temp[j]=='\n')
                nrColumns++;
        }
    }
    
    for(i=0;i<nrColumns;i++){
        for(j=0;j<30;j++){
            gameMap1[i][j]=temp[k];
            k++;
        }
        k++;
    }
    // <READING AND STORING THE GAME MAP\>
    
    
    usermap['F']=Forest;
    usermap['M']=Mountain;
    usermap['G']=Grass;
    usermap[' ']=Paveway;
    usermap['C']=PlayerBlock;
    usermap['E']=Enemy;
    //    cout<<usermap['F'];
    
    pair<int,int> coord(4,24);
    
    enemyLocations[coord]=Monster1;
    coord.first=6;
    coord.second=0;
    enemyLocations[coord]=Monster2;
    
    int nr;
    cout<<"Welcome to the game!"<<endl;
    cout<<"To start, pick a pokemon!"<<endl;
    cout<<"Press 1 for Fire"<<endl<<"Press 2 for Water"<<endl<<"Press 3 for Wind"<<endl<<"Press 4 for Earth"<<endl;
    cin>>nr;
    Pocket_Monsters P1;
    switch (nr) {
        case 1:
            P1= FireMonster;
            P1.assignValues(1);
            PlayersPokemons.push_back(P1);
            break;
        case 2:
            P1= WaterMonster;
            P1.assignValues(1);
            PlayersPokemons.push_back(P1);
            break;
        case 3:
            P1= WindMonster;
            P1.assignValues(1);
            PlayersPokemons.push_back(P1);
            break;
        case 4:
            P1= EarthMonster;
            P1.assignValues(1);
            PlayersPokemons.push_back(P1);
            break;
            
    }
    
    while(1){
        cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
        printMap();
        char x;
    back:
        cin>>x;
        if(x=='q'){
            for(int i=0;i<PlayersPokemons.size();i++){
                cout<<"Pokemon Nr. "+to_string(i+1)+" Information"<<endl<<endl;
                cout<<PlayersPokemons[i]<<endl<<endl;
            }
            cin.get();
            goto back;
        }
        PlayerInfo.moveCharacter(x);
    }
}





// Code at line 74 adapted from -
//https://docs.microsoft.com/en-us/cpp/standard-library/overloading-the-output-operator-for-your-own-classes?view=vs-2019
