// FinalProject.cpp
// Bilal Khan, CISP 400
// December 7, 2023
// Professor Fowler

#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

/*
AI Robot Project:

The robot will be placed on a 10x10 grid with 40 batteries randomly placed on
the grid. The robot will have 5 units of power. The robot will have 16 genes.
Each gene will have 4 states and 1 action. The states are N, S, E, W. The action
is the direction the robot will go. The robot will start at a random location on
the grid. The robot will move in the direction of the action. If the robot
encounters a battery it will increase its power by 5. If the robot encounters a
border it will not move. The robot will move until it runs out of power. The
robot will have 5% chance of mutating. If the robot mutates it will change one
of its genes. The robot will have 200 genes. The robot will run 1000 generations
and will print out the fitness score every 10 generations. The fitness score is
the average energy harvested. The robot will have 16 genes. The robot will run
until it runs out of power. After each generation the top 50% of the robots will
be kept and the bottom 50% will be deleted. The top 50% will be merged together
to create the next generation. The top 50% will be sorted by fitness score. The
top 50% will be merged together by randomly selecting genes from each parent.

Map Properties:
    1. 10x10 grid
    2. 40 batteries

Gene Properties:
    1. 4 states
    2. 1 action

Robot Properties:
    1. 5 units of power
    2. 16 genes
    3. Has a couter for power harvested
    4. Can move in 4 directions

Robot Children Properties:
    1. 5% chance of mutating
    2. 50% chance of getting a gene from parent 1 otherwise parent 2

Evolution Properties:
    1. 200 robots
    2. 1000 generations
    3. Top 50% of robots will be kept
    4. Bottom 50% of robots will be deleted
    5. Top 50% of robots will be merged together to produce bottom 50% of robots



Map class which holds the map
Each robot has their own map
X = Empty Space
B = Battery
R = Robot
-----------------------------------------
| B |   | B | B |   |   |   | B | B |   |
-----------------------------------------
| R | B |   | B |   | B |   |   | B |   |
-----------------------------------------
|   |   |   |   |   | B | B |   |   |   |
-----------------------------------------
| B | B |   |   |   | R |   | B | B | B |
-----------------------------------------
|   |   |   | B |   | B |   |   |   | B |
-----------------------------------------
|   | B |   | B |   |   | B | B |   | B |
-----------------------------------------
|   |   |   | B |   | B | B | B |   |   |
-----------------------------------------
| B | B |   |   | B |   |   | B | B |   |
-----------------------------------------
|   |   | B | B |   |   | B |   | B | B |
-----------------------------------------
|   |   | B |   |   |   |   |   |   | B |
-----------------------------------------

Robots Current State
        |---|---|---|
        |   | 2 |   |
        |---|---|---|
        | 1 | R | 1 |
        |---|---|---|
        |   | 2 |   |
        |---|---|---|

Robots Genes:
        N   E   S   W   A
Gene 0: 0   0   0   0   N
Gene 1: 0   0   0   1   E
Gene 2: 0   0   1   0   S
Gene 3: 0   0   1   1   W
Gene 4: 0   1   0   0   N
Etc...

Robot Child will take half of the genes from one parent and half from the other
*/

/*
Custom vector class made in class
Handles ints and has various functions to gather data
Is similar to python's list
*/
class Vector {
  friend ostream &operator<<(ostream &out, const Vector v);

private:
  int size;
  int *arr = nullptr;

  // Checks if the index is valid
  bool validIndex(int index) {
    if (index < 0 || index >= size) {
      cout << "Error: Invalid Index" << endl;
      return false;
    }
    return true;
  }

public:
  Vector() {
    size = 0;
    arr = new int[size];
  }

  Vector(const Vector &v) {
    size = v.size;
    arr = new int[size];

    for (int i = 0; i < size; i++) {
      arr[i] = v.arr[i];
    }
  }

  // Adds an item to the end of the array
  void add(int numAdded) {

    // Increase size
    size += 1;

    // Create temp array
    int *pTmp = new int[size];

    // Copy over old array to new temp
    for (int i = 0; i < size - 1; i++) {
      pTmp[i] = arr[i];
    }

    // Copy the passed in item as the last element
    pTmp[size - 1] = numAdded;

    delete[] arr;
    arr = pTmp;
    pTmp = nullptr;
  }

  // Returns the size of the array
  int getSize() { return size; }

  // Removes an item given an index
  void remove(int index) {
    // Check if the array is empty
    if (size < 1) {
      cout << "Error: Vector is empty" << endl;
      return;
    }

    // Check if the index is valid
    if (!validIndex(index)) {
      return;
    }

    // Shift the elements after the index one position to the left
    for (int i = index + 1; i < size; i++) {
      arr[i - 1] = arr[i];
    }

    // Decrement the size of the array
    size--;

    int *temp = new int[size];
    for (int i = 0; i < size; i++) {
      temp[i] = arr[i];
    }
    delete[] arr;
    arr = temp;
  }

  // Deletes the last element
  void remove() {
    int index = size - 1;
    remove(index);
  }

  // prints the vector in the form [1,2,3,4]
  string getArray() {
    if (size < 1) {
      return "";
    } else {
      string answer = "[";
      for (int i = 0; i < size; i++) {
        answer += to_string(arr[i]);
        if (i < size - 1) {
          answer += ", ";
        }
      }
      answer += "]";
      return answer;
    }
  }

  // Returns the element at the given index
  int get(int index) {
    if (validIndex(index)) {
      return arr[index];
    } else {
      return -1;
    }
  }

  // Return the element at the given index to the given value
  int operator[](int i) { return get(i); }

  ~Vector() {
    delete[] arr;
    arr = nullptr;
  }

  ostream &operator<<(ostream &out) {
    out << getArray();
    return out;
  }

  istream &operator>>(istream &in) {
    int x;
    cin >> x;
    add(x);
    return in;
  }

  // Returns the sum of all the elements in the array
  void reverse() {
    int *pTmp = new int[size];
    for (int i = 0; i < size; i++) {
      pTmp[i] = arr[size - i - 1];
    }
    delete[] arr;
    arr = pTmp;
    pTmp = nullptr;
  }

  // Insert an item at the given index
  void insert(int number, int index) {
    if (validIndex(index)) {
      size += 1;
      int *pTmp = new int[size];

      for (int i = 0; i < size; i++) {
        if (i < index) {
          pTmp[i] = arr[i];
        } else if (i == index) {
          pTmp[i] = number;
        } else {
          pTmp[i] = arr[i - 1];
        }
      }

      delete[] arr;
      arr = pTmp;
      pTmp = nullptr;
    }
  }

  // Sort from greatest to smallest
  void sort() {
    for (int i = 0; i < size; i++) {
      for (int j = i; j < size; j++) {
        if (arr[i] < arr[j]) {
          int temp = arr[i];
          arr[i] = arr[j];
          arr[j] = temp;
        }
      }
    }
  }

  // Returns the largest element in the array
  int max() {
    int max = arr[0];
    for (int i = 0; i < size; i++) {
      if (arr[i] > max) {
        max = arr[i];
      }
    }
    return max;
  }

  // Returns the smallest element in the array
  int min() {
    int min = arr[0];
    for (int i = 0; i < size; i++) {
      if (arr[i] < min) {
        min = arr[i];
      }
    }
    return min;
  }

  // Returns the index of the given number
  int indexOf(int number) {
    for (int i = 0; i < size; i++) {
      if (arr[i] == number) {
        return i;
      }
    }
    return -1;
  }

  // Implement in the future
  // slice

  // clear
};

/*
Helper functions used throughout the code
Includes color manipulation and random number generation
*/
class {
public:
  // Returns a random number between low and high inclusive
  int randNum(int low, int high) { return rand() % (high - low + 1) + low; }

  string toGreen(string str) { return "\033[32m" + str + "\033[0m"; }

  string toBlue(string str) { return "\033[34m" + str + "\033[0m"; }

  string toRed(string str) { return "\033[31m" + str + "\033[0m"; }

  string toYellow(string str) { return "\033[33m" + str + "\033[0m"; }

  // Write a method which takes an array of int and returns a sorted array

} Helper;

class Map {

protected:
  char map[10][10];
  int numberOfBatteries = 40;
  bool botPlaced = false;
  int robotX;
  int robotY;

public:
  // Constructor
  Map() {
    // Initialize the map array
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        map[i][j] = ' ';
      }
    }
    placeBatteries();
    placeRobot();
  }

  void printMap() {
    cout << "Getting Map..." << endl;
    cout << setfill('-') << setw(45) << "" << setfill(' ') << endl;

    // when bot goes up, delete the battery, and recharge bot

    // Add row header
    cout << "|   | ";
    for (int i = 0; i < 10; i++) {
      cout << i << " | ";
    }
    cout << endl;
    // once robot is on a battery, the battery does not exist afterwards
    for (int i = 0; i < 10; i++) // i is for rows
    {
      // add column header

      cout << setfill('-') << setw(45) << "" << setfill(' ') << endl;
      cout << "| " << i << " ";
      cout << "| ";
      for (int j = 0; j < 10; j++) // j is for columns
      {
        if (i == robotY && j == robotX) {
          cout << Helper.toRed(string(1, 'R'));
          // Increase the power of the robot
          map[i][j] = ' ';
        } else if (map[i][j] == 'B') {
          cout << Helper.toGreen(string(1, map[i][j]));
        } else {
          cout << map[i][j];
        }
        cout << " | ";
      }
      cout << endl;
    }
    cout << setfill('-') << setw(45) << "" << setfill(' ') << endl;
  }

  void placeBatteries() {

    for (int i = 0; i < numberOfBatteries; i++) {
      // if there is a batter already there repeat until there is not
      int x = Helper.randNum(0, 9);
      int y = Helper.randNum(0, 9);

      if (map[x][y] == 'B') {
        i--;
      }
      map[x][y] = 'B';
    }
  }

  // ALERT: THIS IS THE ISSUE
  void placeRobot() {
    if (botPlaced) {
      return;
    }
    int x = Helper.randNum(0, 9);
    int y = Helper.randNum(0, 9);

    // If there is a battery there, place the robot somewhere else
    if (map[x][y] == 'B') {
      placeRobot();
    } else {
      robotX = y;
      robotY = x;
      // cout << "Robot X: " << (robotX) << endl;
      // cout << "Robot Y: " << (robotY) << endl;
      botPlaced = true;
      return;
    }
  }

  int getRobotX() { return robotX; }
  int getRobotY() { return robotY; }
};

class Genes {
private:
  // gets states from robot
  // [North, South, Eash, West]
  int genes[4];

  // Linked these states to the following actions
  // N = goNorth(), S = goSouth(), E = goEast(), W = goWest()
  char action;

public:
  Genes() {
    for (int i = 0; i < 4; i++) {
      genes[i] = Helper.randNum(0, 2);
    }
    mutateAction();
  }

  // Mutates the action to something random
  void mutateAction() {
    char dir = Helper.randNum(0, 3);
    // write a switch case for this
    switch (dir) {
    case 0:
      action = 'N';
      break;
    case 1:
      action = 'E';
      break;
    case 2:
      action = 'S';
      break;
    case 3:
      action = 'W';
      break;
    default:
      action = 'N'; // Default action
      break;
    }
  }

  // Mutates the given direction to something random
  void mutateDirection(int direction) {
    genes[direction] = Helper.randNum(0, 2);
  }

  // Mutates a random direction to something random
  void mutateDirection() {
    int index = Helper.randNum(0, 3);
    genes[index] = Helper.randNum(0, 2);
  }

  // Setters
  void setAction(char newAction) { action = newAction; }
  void setGene(int index, int value) { genes[index] = value; }

  // Getters
  char getAction() { return action; }

  // Calculates the score based on the given states
  int geneScore(int N, int E, int S, int W) {
    int score = 0;

    if (N == genes[0])
      score++;
    if (E == genes[1])
      score++;
    if (S == genes[2])
      score++;
    if (W == genes[3])
      score++;

    // change it so that if it is exact it will do plus 2, but if it is not
    // exact it will do plus 1 so if N is 2 and genes[0] is 2 then it will do
    // plus 2 but if N is 1 and genes[0] is 2 then it will do plus 1

    if (N == 1 && genes[0] == 2) {
      score++;
    }
    if (E == 1 && genes[1] == 2) {
      score++;
    }
    if (S == 1 && genes[2] == 2) {
      score++;
    }
    if (W == 1 && genes[3] == 2) {
      score++;
    }

    return score;
  }

  int &operator[](int index) { return genes[index]; }
};

class Robot : public Map {
private:
  // Each robot needs power to move
  int power;

  // Keeps track of how many batteries the robot has harvested
  int powerHarvested;

  // How much power the robot gets when it harvests a battery
  int batteryIncrease;

  // 0 = border
  // 1 = not border
  // 2 = battery
  int N;
  int E;
  int S;
  int W;

  // array size 16 of genes
  // each gene has 4 states and 1 action
  // [N, S, E, W, action]
  Genes genes[16];

public:
  Robot() {
    powerHarvested = 0;
    batteryIncrease = 5;
    power = 5;
  }

  bool botDead() {
    if (power <= 0) {
      return true;
    }
    return false;
  }

  void getStates() {
    N = 0;
    E = 0;
    S = 0;
    W = 0;
    if (robotY != 0) {
      N = map[robotY - 1][robotX] == 'B' ? 2 : 1;
    }
    if (robotX != 9) {
      E = map[robotY][robotX + 1] == 'B' ? 2 : 1;
    }
    if (robotY != 9) {
      S = map[robotY + 1][robotX] == 'B' ? 2 : 1;
    }
    if (robotX != 0) {
      W = map[robotY][robotX - 1] == 'B' ? 2 : 1;
    }
  }

  // outputs it like a compass so it looks like this:
  /*
  |---|---|---|
  |   | 2 |   |
  |---|---|---|
  | 1 | R | 1 |
  |---|---|---|
  |   | 2 |   |
  |---|---|---|
  */
  void printStates() {
    cout << "Getting states..." << endl;
    getStates();

    string output = "";
    output += "|---|---|---|\n";
    output += "|   | " + Helper.toBlue(to_string(N)) + " |   |\n";
    output += "|---|---|---|\n";
    output += "| " + Helper.toBlue(to_string(W)) + " | " + Helper.toRed("R") +
              " | " + Helper.toBlue(to_string(E)) + " |\n";
    output += "|---|---|---|\n";
    output += "|   | " + Helper.toBlue(to_string(S)) + " |   |\n";
    output += "|---|---|---|\n";
    cout << output;
  }

  // Prints the genes in the form:
  // N   E   S   W   A
  // 0   0   0   0   N
  // 0   0   0   1   E
  // 0   0   1   0   S
  // etc...
  void printGenes() {
    cout << "Printing genes..." << endl;
    cout << "        N   E   S   W   A" << endl;

    for (int i = 0; i < 16; i++) {
      cout << "Gene " << i << ": ";
      cout << genes[i][0] << "   ";
      cout << genes[i][1] << "   ";
      cout << genes[i][2] << "   ";
      cout << genes[i][3] << "   ";
      cout << genes[i].getAction() << "   ";
      cout << endl;
    }
  }

  // Prints a specific gene
  void printGene(int geneNumber) {
    cout << "Gene " << geneNumber << ": ";
    cout << genes[geneNumber][0] << "   ";
    cout << genes[geneNumber][1] << "   ";
    cout << genes[geneNumber][2] << "   ";
    cout << genes[geneNumber][3] << "   ";
    cout << genes[geneNumber].getAction() << "   ";
    cout << endl;
  }

  int findBestGene() {

    // Update the states
    // Pass the state to all genes to get score
    // Checks clockwise
    // Add them to a vector
    // Sort and return the highest score

    getStates();
    Vector scores;

    for (int i = 0; i < 16; i++) {
      scores.add(genes[i].geneScore(N, E, S, W));
    }
    return scores.indexOf(scores.max());
  }

  void executeBestGene() {

    // Method One of finding the best gene
    // Looks for an exact match
    /*
            getStates();
            int geneNumber = 15;

            for(int i = 0; i < 16; i++) {
                if(genes[i][0] == N && genes[i][1] == E && genes[i][2] == S &&
       genes[i][3] == W) { geneNumber = i; break;
                }
            }
            char action = genes[geneNumber].getAction();

    */

    // Method Two of finding the best gene
    // Looks for the highest score
    int geneNumber = findBestGene();
    char action = genes[geneNumber].getAction();

    // Decrease power
    power -= 1;

    // Clear the spot after move so there is not another battery in there
    map[robotY][robotX] = ' ';

    // Checks if it is along the border or not and executes the action if
    // possible
    if (N != 0 && action == 'N') {
      goNorth();
    } else if (E != 0 && action == 'E') {
      goEast();
    } else if (S != 0 && action == 'S') {
      goSouth();
    } else if (W != 0 && action == 'W') {
      goWest();
    }
  }

  // Runs the robot until it runs out of power
  void run() {
    while (!botDead()) {
      executeBestGene();
    }
  }

  void increasePower() {
    power += batteryIncrease;
    powerHarvested += 1;
  }

  void decreasePower() { power -= 1; }

  void setPower() { power = 5; }

  // All actions check the state ahead for a battery
  // Validation is done in other parts of the code

  void goNorth() {
    if (map[robotY - 1][robotX] == 'B') {
      increasePower();
    }
    // map[robotY][robotX] = ' ';
    robotY -= 1;
  }

  void goEast() {
    if (map[robotY][robotX + 1] == 'B') {
      increasePower();
    }
    // map[robotY][robotX] = ' ';
    robotX += 1;
  }

  void goSouth() {
    // Question: Why does this randomly fail???????
    if (map[robotY + 1][robotX] == 'B') {
      increasePower();
    }
    // map[robotY][robotX] = ' ';
    robotY += 1;
  }

  void goWest() {
    if (map[robotY][robotX - 1] == 'B') {
      increasePower();
    }
    // map[robotY][robotX] = ' ';
    robotX -= 1;
  }

  // Robot newRob = rob1 + rob2;
  Robot operator+(const Robot &otherRobot) {
    Robot newRobot;
    // Randomly select genes from each parent
    for (int i = 0; i < 16; i++) {
      newRobot.genes[i] = (rand() % 2 == 0) ? genes[i] : otherRobot.genes[i];
    }
    // 5 Percent chance that they will be chosen and something will be mutated
    newRobot.mutateGene();

    return newRobot;
  }

  // In 5% of the individual genes swapping there is an error - a mutation.
  // Randomly change one character on the gene the child has inherited
  // from it’s parent. Just generate another value and insert that value
  // over the old one.
  void mutateGene() {
    // 5% chance of mutation
    // 1/20 = 5%
    bool change = Helper.randNum(1, 20) == 1;
    if (change) {
      int geneNumber = Helper.randNum(0, 15);
      // 0 = direction
      // 1 = action

      if (Helper.randNum(0, 1)) {
        // mutate direction
        genes[geneNumber].mutateDirection();
      } else {
        // mutate action
        genes[geneNumber].mutateAction();
      }
    }
  }

  int getPowerHarvested() { return powerHarvested; }
  ~Robot() {
    // Code to release resources, if any
  }
};

class Evolution {
  // 1. Create robots
  // 2. Run each robot
  // 3. Sort the robots by power harvested
  // 4. Show the top robots
  // 5. Merge the top robots

private:
  // Default population size
  int size = 200;

  // Dynamically allocated array of robots
  Robot *robots;
  bool colorize = false;

public:
  // Constructor which creates the robots to default size
  Evolution() {
    srand(time(NULL));
    robots = new Robot[size];
  }

  // Constructor which creates the robot to the given size
  Evolution(int s) {
    srand(time(NULL));

    size = s;
    robots = new Robot[size];
  }

  void colorizeOutput(bool colorize) { this->colorize = colorize; }

  // Runs all the robots in the population
  void run() {
    for (int i = 0; i < size; i++) {
      robots[i].run();
    }
  }

  // Runs the robots for the given number of generations and outputs the fitness
  // score for every X generation
  void runGeneration(int numberOfGens, int everyOtherGen = 1) {
    // Colorize the change in fitness score
    int previousFitnessScore = 0;

    cout << "|" << setfill('-') << setw(41) << "" << setfill(' ') << "|"
         << endl;
    cout << "|   Generation Number   |"
         << "  Fitness Score  "
         << "|" << endl;
    cout << "|" << setfill('-') << setw(41) << "" << setfill(' ') << "|"
         << endl;

    run();
    for (int i = 0; i < numberOfGens; i++) {
      naturalSelection();
      run();
      if (i % everyOtherGen == 0) {

        if (colorize) {
          int currentScore = getFitness();
          // cout << "Current previous fitness score is: " <<
          // previousFitnessScore << endl;
          if (currentScore > previousFitnessScore) {
            cout << "| " << setw(30) << Helper.toGreen((to_string(i) + " "))
                 << " | " << setw(24) << Helper.toGreen(to_string(getFitness()))
                 << " |" << endl;
          } else if (currentScore < previousFitnessScore) {

            cout << "| " << setw(30) << Helper.toRed((to_string(i)) + " ")
                 << " | " << setw(24) << Helper.toRed(to_string(getFitness()))
                 << " |" << endl;
          } else {
            cout << "| " << setw(30) << Helper.toYellow((to_string(i)) + " ")
                 << " | " << setw(24)
                 << Helper.toYellow(to_string(getFitness())) << " |" << endl;
          }

          previousFitnessScore = getFitness();
          // cout << "| " << setw(21) << (to_string(i)+" ") << " | " << setw(15)
          // << getFitness() << " |" << endl;
        } else {
          cout << "| " << setw(21) << (to_string(i) + " ") << " | " << setw(15)
               << getFitness() << " |" << endl;
        }
      }
    }
    cout << "|" << setfill('-') << setw(41) << "" << setfill(' ') << "|"
         << endl;
  }

  // Sorts the robots by fitness score and deletes the bottom half
  // Then it adds the top half to the bottom half
  void naturalSelection() {
    sortRobots();
    deleteHalf();
    addHalf();
  }

  // Returns the average fitness score of the population
  double getFitness() {
    double totalPowerHarvested = 0.0;
    for (int i = 0; i < size; i++) {
      totalPowerHarvested += robots[i].getPowerHarvested();
    }
    // return average;
    return totalPowerHarvested / size;
  }

  // Sorts the robots by fitness score
  void sortRobots() {
    for (int i = 0; i < size - 1; i++) {
      for (int j = 0; j < size - i - 1; j++) {
        if (robots[j].getPowerHarvested() < robots[j + 1].getPowerHarvested()) {
          Robot temp = robots[j];
          robots[j] = robots[j + 1];
          robots[j + 1] = temp;
        }
      }
    }
  }

  // Prints the robots in the form:
  // Robot 0 Power Harvested: 100
  // Robot 1 Power Harvested: 99
  // etc...
  void printRobots() {
    for (int i = 0; i < size; i++) {
      cout << "Robot " << i
           << " Power Harvested: " << robots[i].getPowerHarvested() << endl;
    }
  }

  int getSize() { return size; }

  void deleteHalf() {
    size /= 2;
    Robot *temp = new Robot[size];

    for (int i = 0; i < size; i++) {
      temp[i] = robots[i];
    }

    delete[] robots;
    robots = temp;
  }

  // Makes children and adds it to the bottom half
  void addHalf() {
    const int unmoddedSize = size;
    for (int i = 0; i < unmoddedSize; i += 2) {
      for (int j = 0; j < 2; j++) {
        Robot temp = robots[i] + robots[i + 1]; // + operator
        addRobot(temp);
      }
    }
  }

  // Add robot
  void addRobot(Robot rob) {
    size++;
    Robot *temp = new Robot[size];
    for (int i = 0; i < size - 1; i++) {
      temp[i] = robots[i];
    }
    temp[size - 1] = rob;
    delete[] robots;
    robots = temp;
    temp = nullptr;
  }
};

void programGreeting() {
  cout << "Welcome to the AI Robot Project" << endl;
  cout << "We will represent evolution of robots in a 10x10 grid with 40 "
          "batteries"
       << endl;
  cout << "Each robot will have 5 units of power and 16 genes" << endl;
  cout << "The robots with the best genes will continue to live" << endl;
  cout << "Made by: Bilal Khan" << endl;
}

int main() {
  programGreeting();

  // Create the evolution
  // With 200 robots
  Evolution evo(200);

  // Colorize the ouput to show the change in fitness score
  // Red = decrease in fitness score
  // Green = increase in fitness score
  // Yellow = little to no change in fitness score
  evo.colorizeOutput(true);
  evo.runGeneration(201, 10);
  return 0;
}

/*
Sample Output:
Welcome to the AI Robot Project
We will represent evolution of robots in a 10x10 grid with 40 batteries
Each robot will have 5 units of power and 16 genes
The robots with the best genes will continue to live
Made by: Bilal Khan
|-----------------------------------------|
|   Generation Number   |  Fitness Score  |
|-----------------------------------------|
|                    0  |            2.67 |
|                   10  |           5.485 |
|                   20  |            8.18 |
|                   30  |          29.745 |
|                   40  |           34.18 |
|                   50  |          35.185 |
|                   60  |          34.765 |
|                   70  |           35.15 |
|                   80  |           34.98 |
|                   90  |          35.745 |
|                  100  |          35.105 |
|                  110  |           35.63 |
|                  120  |          35.075 |
|                  130  |           35.48 |
|                  140  |           35.53 |
|                  150  |          34.915 |
|                  160  |           35.31 |
|                  170  |              36 |
|                  180  |            34.5 |
|                  190  |           35.22 |
|                  200  |          35.695 |
|-----------------------------------------|
*/
