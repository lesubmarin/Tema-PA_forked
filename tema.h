#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#define DIM 256

struct Player
{
    char* firstName;
    char* secondName;
    int points;
};
typedef struct Player Player;

typedef struct Team{
    int nrPlayers;
    char* teamName; 
    Player *players;
    float averageTeam;
}Team;

typedef struct List{
    Team team;
    struct List *next;
}List;

typedef struct QueueNode{
    List* team1;
    List* team2;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue{
    QueueNode* front;
    QueueNode* rear;
}Queue;

typedef struct LosersStack{
    Team losers;
    struct LosersStack* next;
}LosersStack;

typedef struct WinnersStack{
    Team winners;
    struct WinnersStack* next;
}WinnersStack;

typedef struct BSTList{
    Team team;
    struct BSTList *next;
}BSTList;

typedef struct BST{
    Team team;
    struct BST *left, *right;
}BST;

List* createNewNode(int nrPlayers, char* teamName, Player* players);
void freeList(List* head);
void printList(List* head);
void printTeams(List *head);
void writeTeamsToFile(List *head, char* filePathExit);
void addAtBeginning(List** head, int nrPlayers, char* teamName, Player* players);
void task1(List** head, char *filePathInput, char* filePathOutput);
void averageTeams(int numberOfTeams, List* head);
int detN(int numberOfTeams);
void task2(List** head, int N, int numberOfTeams, char* filePathOutput);
int readNumberOfTeams(char *filePathInput);
void createMatchQueue(List* head, Queue* queue);
void enQueue(Queue* queue, List* team1, List* team2);
int isQueueEmpty(Queue* queue);
Queue* createQueue();
void deQueue(Queue* queue);
void printQueue(Queue* queue);
void freeQueue(Queue* queue);
void writeQueueToFile(Queue* queue, char* filePathOutput, List *head, int round);
void popWinners(WinnersStack** winnersTop, char* filePathOutput);
void popLosers(LosersStack** losersTop);
int isEmptyStackWinners(WinnersStack* winnersTop);
int isEmptyStackLosers(LosersStack* losersTop);
void pushWinners(WinnersStack** winnersTop, QueueNode* currentNode);
void pushLosers(LosersStack** losersTop, QueueNode* currentNode);
void printRoundWinners(int round, WinnersStack* winnersTop, char* filePathOutput);
int getSizeStackWinners(WinnersStack* winnersTop);
void createQueueWithWinners(Queue* queue, WinnersStack** winnersTop);
void clearLosersStack(LosersStack** top);
void enQueueWinners(Queue* queue, Team team1, Team team2);
List* convertWinnersStackToList(WinnersStack* winnersTop);
void task3(List** head, char* filePathOutput, Queue** matchQueue, int *counter, int numberOfTeams, int numberOfRounds, BSTList **headBST);
int detNumberOfRounds(int numberOfTeams);
void printInDescendingOrder(BST* root, char* filePathOutput);
void freeBST(BST* root);
BST* copyListToBST(BSTList* head);
BST* createBSTNode(Team team);
void insertBSTNode(BST** root, Team team);
int getNumberOfTeams(List* head);
BSTList* copyListToBSTList(List* head, char* filePathOutput);
void task4(BSTList* headBST, char* filePathOutput);