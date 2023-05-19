#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#define DIM 256

typedef struct Node{
	char *val;
	struct Node *next;
}Node;

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
void writeQueueToFile(Queue* queue, char* filePathOutput);


