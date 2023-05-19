#include "tema.h"

int readNumberOfTeams(char *filePathInput)
{
    FILE* file;
    file = fopen(filePathInput, "rt");
    if(file == NULL){
        puts("Eroare");
        exit(1);
    }

    int numberOfTeams = 0;
    fscanf(file, "%d", &numberOfTeams);
    fgetc(file);
    fclose(file);
    return numberOfTeams;
}

void task1(List** head, char *filePathInput, char* filePathOutput)
{
    FILE* file;
    file = fopen(filePathInput, "rt");
    if(file == NULL){
        puts("Eroare");
        exit(1);
    }

    int numberOfTeams = 0;
    fscanf(file, "%d", &numberOfTeams);
    fgetc(file);
    for(int i = 0; i < numberOfTeams; i++){
        int nrPlayers;
        fscanf(file, "%d ", &nrPlayers);
        char teamName[50];
        fgets(teamName, sizeof(teamName), file);
        teamName[strcspn(teamName, "\n\r")] = '\0';
        
        Player* players = (Player*)malloc(nrPlayers * sizeof(Player));

        for(int j = 0; j < nrPlayers; j++){
            char firstName[30];
            char secondName[30];
            int points;

            fscanf(file, "%s %s %d", firstName, secondName, &points);
            fgetc(file);

            players[j].firstName = strdup(firstName);
            players[j].secondName = strdup(secondName);
            players[j].points = points;
        }

        addAtBeginning(head, nrPlayers, teamName, players);
    }
    //printTeams(*head);
    writeTeamsToFile(*head, filePathOutput);
}

List* createNewNode(int nrPlayers, char* teamName, Player* players)
{
    List* newNode = (List*)malloc(sizeof(List));
    newNode->team.nrPlayers = nrPlayers;
    newNode->team.teamName = strdup(teamName);
    newNode->team.players = players;
    newNode->next = NULL;
    return newNode;
}

void printList(List* head)
{
    List* currentNode = head;
    while (currentNode != NULL) {
        printf("%d %s\n",/*currentNode->team.averageTeam,*/ currentNode->team.nrPlayers, currentNode->team.teamName);
        for (int i = 0; i < currentNode->team.nrPlayers; i++) {
            Player player = currentNode->team.players[i];
            printf("%s %s %d\n", player.firstName, player.secondName, player.points);
        }
        printf("\n");
        currentNode = currentNode->next;
    }
}

void freeList(List* head)
{
    List* currentNode = head;
    while (currentNode != NULL) {
        free(currentNode->team.teamName);
        for (int i = 0; i < currentNode->team.nrPlayers; i++) {
            free(currentNode->team.players[i].firstName);
            free(currentNode->team.players[i].secondName);
        }
        free(currentNode->team.players);
        List* temp = currentNode;
        currentNode = currentNode->next;
        free(temp);
    }
}

void printTeams(List *head)
{
    List* currentNode = head;
    while (currentNode != NULL) {
        printf("%s\n", currentNode->team.teamName);
        currentNode = currentNode->next;
    }
}

void writeTeamsToFile(List *head, char* filePathOutput)
{
    FILE *file = fopen(filePathOutput, "wt");
    if(file == NULL){
        printf("Eroare");
        exit(1);
    }

    List* currentNode = head;
    while (currentNode != NULL) {
        fprintf(file, "%s\n", currentNode->team.teamName);
        currentNode = currentNode->next;
    }

    fclose(file);
}

void addAtBeginning(List** head, int nrPlayers, char* teamName, Player* players)
{
    List* newNode = createNewNode(nrPlayers, teamName, players);
    newNode->next = *head;
    *head = newNode;
}

void averageTeams(int numberOfTeams, List* head)
{
    List* currentNode = head;
    for(int i = 0; i < numberOfTeams; i++){
        int totalPoints = 0;
        for(int j = 0; j < currentNode->team.nrPlayers; j++){
            totalPoints += currentNode->team.players[j].points;
        }     
        currentNode->team.averageTeam = (float)totalPoints / currentNode->team.nrPlayers;

        currentNode = currentNode->next;   
    }
}

int detN(int numberOfTeams)
{
    int N = 1;
    while(N < numberOfTeams){
        N = N * 2;
    }
    if(N > numberOfTeams){
        N = N / 2;
    }
    return N;
}

void task2(List** head, int N, int numberOfTeams, char* filePathOutput)
{
    averageTeams(numberOfTeams, *head);
    while(numberOfTeams > N){
        List *currentNode = *head;
        List *lowestTeam = *head;
        while(currentNode != NULL){
            if(currentNode->team.averageTeam < lowestTeam->team.averageTeam){
                lowestTeam = currentNode;
            }
            currentNode = currentNode->next;
        }
        if(lowestTeam == *head){
            *head = lowestTeam->next;
        }
        else{
            List *prevNode = *head;
            while(prevNode->next != lowestTeam){
                prevNode = prevNode->next;
            }
            prevNode->next = lowestTeam->next;
        }
        numberOfTeams--;
    }
    writeTeamsToFile(*head, filePathOutput);
}

Queue* createQueue()
{
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

int isQueueEmpty(Queue* queue)
{
    return (queue->front == NULL);
}

void deQueue(Queue* queue)
{
    if(isQueueEmpty(queue)){
        return;
    }

    QueueNode* temp = queue->front;
    queue->front = queue->front->next;    
    if(queue->front == NULL){
        queue->rear = NULL;
    }
    free(temp);
}

void freeQueue(Queue* queue)
{
    while(!isQueueEmpty(queue)){
        deQueue(queue);
    }
    free(queue);
}

void enQueue(Queue* queue, List* team1, List* team2)
{
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->team1 = team1;
    newNode->team2 = team2;
    newNode->next = NULL;
    if(isQueueEmpty(queue)){
        queue->front = newNode;
        queue->rear = newNode;
    }
    else{
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

void createMatchQueue(List* head, Queue* queue)
{
    List* currentNode = head;
    while(currentNode != NULL && currentNode->next != NULL){
        enQueue(queue, currentNode, currentNode->next);
        currentNode = currentNode->next->next;
    }
}

void printQueue(Queue* queue)
{
    QueueNode* currentNode = queue->front;
    while(currentNode != NULL){
        printf("Match: %s vs %s\n", currentNode->team1->team.teamName, currentNode->team2->team.teamName);
        currentNode = currentNode->next;
    }   
}

void writeQueueToFile(Queue* queue, char* filePathOutput)
{
    FILE* file = fopen(filePathOutput, "wt");
    if(file == NULL){
        puts("Eroare");
        exit(1);
    }
    QueueNode *currentNode = queue->front;
    while(currentNode != NULL){
        fprintf(file, "%-33s-%33s\n", currentNode->team1->team.teamName, currentNode->team2->team.teamName);
            currentNode = currentNode->next;
    }
    fclose(file);
}