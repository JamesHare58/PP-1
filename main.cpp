#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <thread>
#include <limits>

using namespace std;

//global vars
bool gameDone=false;
bool mazeDone=false;

//1: small maze
//2: large maze
int selectedMaze;

//Player Position within the maze
int playerXCoord;
int playerYCoord;

int mazeEndXCoord;
int mazeEndYCoord;

//1:Move
//2:Interact
//3:Inventory
//4:
//5: Quit
int selectedAction;
//1: North 2: East 3: South 4: West
int selectedDirection;

//25 Maze Tiles with bool values for each wall
// i.e [1][1] would be tile 2 with the north wall
//mazeWalls[rowNum][colNum][WallDirection]
//WallDirectionIndex: 1: North, 2: East, 3: South, 4: West
//wallDirection: 1: Wall is there, 0: Wall is not there
int mazeWalls[5][5][4] = {
    {
        {1,0,0,1},//row1
        {1,0,1,0},
        {1,0,0,0},
        {1,0,0,0},
        {1,1,1,0}
    },{
        {0,0,0,1},//row2
        {1,1,0,0},
        {0,1,1,1},
        {0,0,1,1},
        {1,1,0,0}
    },{
        {0,1,0,1},//row3
        {0,0,1,1},
        {1,0,0,0},
        {1,1,0,0},
        {0,1,1,1}
    },{
        {0,0,1,1},//row4
        {1,1,0,0},
        {0,1,1,1},
        {0,0,1,1},
        {1,1,0,0}
    },{
        {1,0,1,1},//row5
        {0,1,1,0},
        {1,0,0,1},
        {1,0,1,0},
        {0,1,1,0}
    }
};

//List of specific item ids and their locations
int itemCoords[5][5] = {
    {0,0,0,0,3},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
};

/*
//List of specific item ids and their locations
int itemCoords[5][5] = {
    {0,0,0,0,3},
    {0,0,0,0,0},
    {0,0,1,0,0},
    {0,0,2,0,0},
    {0,0,0,0,0},
};*/ //ran out of time to implement other items


string items[5] = {
    "nothing",
    "sign", //item id 1
    "game of chance",   //item id 2
    "pickaxe",          //item id 3
    "other"
};

bool playerInventory [5] = {
    false,
    false,
    false,
    false, //Pickaze item id
    false,
};

// player position in char array based on location is:
//x player location is equal to their cell coord times 3 minus 2 (1 for indexing, 1 for middle of 3 char string)
//y player location is equal to their cell's row number
//x player insert locations are 1, 4, 7, 10, 13
//y player insert locations are 0-4
string mazeDisplayChars [5];

string itemDisplay = "nothing";

string smallMaze [5] = {
"|  ___      __|\n",
"|     |__|__  |\n",
"|  |__      |_|\n",
"|__   |__|__ -|\n",
"|     |       |\n"};


//function declarations
void gameLoop();
void mazeLoop();
void drawMaze();
void selectMaze();
void selectAction();
void backstory();
void endstory();
void clearMazeTiles();
void setMazeTiles(int xcoord, int ycoord);
void move();
void sleep();
int validPlayerIntInput();
void checkCoords();
void event_Pickaxe();
void event_Sign();
void event_Chance();
void displayInventory();
void usePick();

//Loops the entire game, if a user wanted to play again after completing the maze for example
void gameLoop(){
    while(gameDone!=true){
        selectMaze();
        if(!gameDone){
            backstory();
            mazeLoop();
        }
    }
}

void mazeLoop() {
    while(mazeDone!=true){
        drawMaze();
        selectAction();
        if (selectedAction == 1) {
            move();
            checkCoords();
        }
        if (selectedAction == 2) {
            //Interact command
            cout << "\n You look around the area for a bit" << endl;
            for (int i = 0; i <3; i++) {
                cout << ".";
                sleep();
            }
            if (itemCoords[playerYCoord][playerXCoord] == 0) {
                cout << "\nThere's nothing in the area to interact with" << endl;
                sleep();
            }
            else {
                int itemId = itemCoords[playerYCoord][playerXCoord];
                if (itemId == 1) {
                    event_Sign();
                }
                if (itemId == 2) {
                    event_Chance();
                }
                if (itemId == 3) {
                    event_Pickaxe();
                }
            }
        }
        if (selectedAction == 3) {
            displayInventory();
        }
        if (selectedAction == 5) {
            gameDone=true;
        }
        if(mazeDone){
            drawMaze();
            endstory();
        }
    }
}

void drawMaze() {

    cout << "-------Current Maze Layout-------" <<endl;
    cout << "_______________" << endl;
    setMazeTiles(playerXCoord,playerYCoord);
    printf( "------   ------\n");
    cout << "-------In the Area-------\n - "<< itemDisplay << "\n" << endl;
}

//Allows a player to select a Maze to play
void selectMaze() {
    cout << "Welcome to Maze, please select which maze you'd like to play: \n 1: Small Maze 5x5 \n 2: Quit!" <<endl;
    selectedMaze = validPlayerIntInput();
    if (selectedMaze==1) {
        cout << "Excellent, you've selected the small maze, your adventure awaits!" << endl;
    }
    else if (selectedMaze==2) {
        cout << "Alright see ya later!" << endl;
        gameDone = true;
    }
    else {
        cout << "Error: Please enter either 1 or 2" << endl;
        selectMaze();
    }
}

void selectAction() {
    cout << "Select Action: \n 1: Move \n 2: Interact \n 3: Inventory \n 4: \n 5: Quit Game" << endl;
    selectedAction = validPlayerIntInput();
}

void displayInventory(){

    cout << "-------Current Player Inventory-------\n";
    for (int i = 0; i < 5; i++) {
        if (playerInventory[i]) {
            cout << i << ": " << items[i] << endl;
        }
    }
    cout <<"5: Back" << endl;
    cout << "\nEither select the number of item you'd like to use, or return with 5\n";
    int useItemId = validPlayerIntInput();
    if (useItemId == 3 && playerInventory[3]) {
        usePick();
    }
    else if (useItemId != 5) {
        displayInventory();
    }
}

void checkCoords() {
    //At this point the character has moved to its new location, we now need to check for end goals
    if((mazeEndYCoord == playerYCoord) && (mazeEndXCoord == playerXCoord))
    {
        mazeDone = true;
    }
    else {
        switch (itemCoords[playerYCoord][playerXCoord]) {
            case 0:
                itemDisplay = "nothing";
                break;
            case 1:
                itemDisplay = "A sign pointing south with the words 'GO THIS WAY'";
                break;
            case 2:
                itemDisplay = "A Game of chance lies in front of you";
                break;
            case 3:
                itemDisplay = "A rough looking Pickaxe";
                break;
            default:
                itemDisplay = "nothing";
                break;
        }
    }
}

void move() {
    selectedDirection = 0;
    cout << "Choose which direction you'd like to go:" << endl;
    if (mazeWalls[playerYCoord][playerXCoord][0] == 0)
        cout << "1: North\n";
    if (mazeWalls[playerYCoord][playerXCoord][1] == 0)
        cout << "2: East\n";
    if (mazeWalls[playerYCoord][playerXCoord][2] == 0)
        cout << "3: South\n";
    if (mazeWalls[playerYCoord][playerXCoord][3] == 0)
        cout << "4: West\n";
    selectedDirection = validPlayerIntInput();
    bool validMove = false;
    if(mazeWalls[playerYCoord][playerXCoord][selectedDirection-1] == 0) {
        if (selectedDirection == 1)
            playerYCoord -=1;
        if (selectedDirection == 2)
            playerXCoord +=1;
        if (selectedDirection == 3)
            playerYCoord +=1;
        if (selectedDirection == 4)
            playerXCoord -=1;
    }
    else{
        cout << "Invalid Direction, please enter a correct direction to move" << endl;
        move();
    }
}

void backstory(){
    cout << "Hello adventurer! You're probably wondering what you're doing here? Or even who I am? \n";
    sleep();
    cout << "Pah! Minor details my fair adventurer! \n";
    sleep();
    cout << "Just know you are in grave danger! You need to get out of here before it's too late \n";
    sleep();
    cout << "Or not honestly, I don't really care, I don't even know you, stay here and die for all I care! \n";
    sleep();
    cout << "Either way the choice is up to you, feel free to explore the area, if you figure a way out then great! \n";
    sleep();
    cout << "\n Hmm? What? Grave Danger? Did I say that? right right... yeah, maybe don't stick around too long, good luck!" << endl;
    sleep();
}

void endstory(){
    cout << "\nAfter a logn and treachorous journey, you've found the end of the maze...";
    sleep();
    cout << "\nOr... IS IT????";
    sleep();
    cout << "\n...";
    sleep();
    cout <<"\nno.. no... my mistake, this is a simple maze program, OF COURSE ITS THE END OMG!";
    sleep();
    cout << "\n\n\n Congrats! You've made it through the maze! You can replay if you want, or not, I'm not your mom!" << endl;
}

void sleep(){
    chrono::milliseconds dura( 3000 );
    std:this_thread::sleep_for( dura );
}

void setMazeTiles(int xCoord, int yCoord){
    //As a good measure also reset maze display chars
    for(int i =0; i<5; i++)
    {
        mazeDisplayChars[i]=smallMaze[i];
    }
    //The character index of the Player position in relation to the character array within the string
    int replaceCharIndex = xCoord * 3 + 1;
    //we do our work in a replacement string to avoid array troubles while accessing characters
    string rowReplacement = mazeDisplayChars[yCoord];

    rowReplacement[replaceCharIndex] = 'X';

    mazeDisplayChars[yCoord] = rowReplacement;

    for(int i=0;i<5;i++){
        cout << mazeDisplayChars[i];
    }
}

int validPlayerIntInput() {
    int input = 0;
    while(!(cin >> input)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid Input, please enter one of the listed options." << endl;
    }
    return input;
}

void event_Pickaxe() {
    cout << "\nAfter looking around you found a hefty pickaxe! However it seems to be precariously entrenched near a steep dropoff" << endl;
    sleep();
    cout << "\nAttempt to pick it up or leave it be?:" << endl;
    cout << "\n1: Pick up the Pickaxe\n2: Leave it alone, it's too close to the steep edge\n";
    int playerChoice = validPlayerIntInput();
    if (playerChoice != 1 && playerChoice != 2) {
        event_Pickaxe();
    }
    else {
        if (playerChoice == 1) {
            cout << "You reach for the pickaxe, and grab it firmly without falling off the edge\n +1 Pickaxe to inventory\n";
            playerInventory[3] = true;
            itemCoords[playerYCoord][playerXCoord] = 0;
            sleep();
            cout << "You can now break through a wall with you're pickaxe, simply open your inventory and use Pickaxe";
            itemDisplay = "nothing";
        }
        else {
            cout << "On second thought 'tis a silly idea to need a pickaxe! forget that dumb thing!...\n" <<endl;
            sleep();
            cout << "The pickaxe on hearing this, has it's feelings hurt and jumps off the ledge...\n" << endl;
            itemCoords[playerYCoord][playerXCoord] = 0;
            sleep();
            cout << "You jerk...\n";
            sleep();
            itemDisplay = "nothing";
        }
    }
}

void event_Chance() {
    //TODO add event chance
}

void event_Sign() {
    //TODO add event sign
}

void usePick() {
    cout << "\nPickaxe is being used, select a wall to mine through:";

    //used for border detection
    bool displayDirection [4] = {
        true,//north
        true,//east
        true,//south
        true //west
    };
    if (playerXCoord == 0)
        displayDirection[3] = false;
    if (playerXCoord == 4)
        displayDirection[1] = false;
    if (playerYCoord == 0)
        displayDirection[0] = false;
    if (playerYCoord == 4)
        displayDirection[2] = false;

     if (displayDirection[0] == true)
        cout << "\n1: North";
     if (displayDirection[1] == true)
        cout << "\n2: East";
     if (displayDirection[2] == true)
        cout << "\n3: South";
     if (displayDirection[3] == true)
        cout << "\n4: West";

    int pickDirection = validPlayerIntInput();
        if (pickDirection > 4 || pickDirection < 1) {
            cout << "\n Invalid Direction, try again" << endl;
            usePick();
        }
        else {
            if(displayDirection[pickDirection-1]){
                if (pickDirection == 1)
                    playerYCoord -=1;
                if (pickDirection == 2)
                    playerXCoord +=1;
                if (pickDirection == 3)
                    playerYCoord +=1;
                if (pickDirection == 4)
                    playerXCoord -=1;

                cout << "\nYou mined through the selected direction, but the walls remain, how odd! Oh well!" << endl;
                sleep();
            }
        }
}

//Main, calls gameLoop
int main()
{
    playerXCoord = 4;
    playerYCoord = 2;

    mazeEndXCoord = 2;
    mazeEndYCoord = 4;
    gameLoop();
}
