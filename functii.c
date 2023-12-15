#include "tema.h"

int readNumberOfTeams(char *filePathInput)
{
    // Deschid fisierul in modul "rt" (citire text)
    FILE* file;
    file = fopen(filePathInput, "rt");
    // Verific daca fisierul s-a deschis cum trebuie
    if(file == NULL){
        puts("Eroare");
        exit(1);
    }

    int numberOfTeams = 0;
    // Citesc numarul de echipe din fisier 
    fscanf(file, "%d", &numberOfTeams);
    // Citesc urmatorul caracter pentru a trece peste "\n"
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
    // Parcurg fiecare echipa
    for(int i = 0; i < numberOfTeams; i++){
        int nrPlayers;
        // Citesc numarul de jucatori 
        fscanf(file, "%d ", &nrPlayers);
        char teamName[50];
        // Citesc numele echipei
        fgets(teamName, sizeof(teamName), file);
        // Elimin caracterul newline de la sfarsitul numelui echipei
        teamName[strcspn(teamName, "\n\r")] = '\0';
        // Aloc memorie pentru un vector de jucatori
        Player* players = (Player*)malloc(nrPlayers * sizeof(Player));
        for(int j = 0; j < nrPlayers; j++){
            char firstName[30];
            char secondName[30];
            int points;
            // Citesc numele, prenumele si punctele fiecarui jucator
            fscanf(file, "%s %s %d", firstName, secondName, &points);
            fgetc(file);
            // Aloca memorie si copiaza numele jucatorilor in structura Player
            players[j].firstName = strdup(firstName);
            players[j].secondName = strdup(secondName);
            players[j].points = points;
        }
        // Adauga la inceputul listei
        addAtBeginning(head, nrPlayers, teamName, players);
    }
    // Scrie echipele in fisierul de iesire
    writeTeamsToFile(*head, filePathOutput);
}

List* createNewNode(int nrPlayers, char* teamName, Player* players)
{
    // Aloc memorie pentru un nou nod
    List* newNode = (List*)malloc(sizeof(List));
    // Seteaza numarul de jucatori pentru echipa din noul nod
    newNode->team.nrPlayers = nrPlayers;
    // Seteaza numele echipei in noul nod, folosind strdup pentru a aloca memorie
    newNode->team.teamName = strdup(teamName);
    // Seteaza vectorul de jucatori
    newNode->team.players = players;
    newNode->next = NULL;
    return newNode;
}

void printList(List* head)
{
    List* currentNode = head;
    while (currentNode != NULL) {
        printf("%d %s\n", currentNode->team.nrPlayers, currentNode->team.teamName);
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
    // Parcurg lista si eliberez memoria pentru fiecare nod
    while (currentNode != NULL) {
        free(currentNode->team.teamName);
        for (int i = 0; i < currentNode->team.nrPlayers; i++) {
            free(currentNode->team.players[i].firstName);
            free(currentNode->team.players[i].secondName);
        }
        free(currentNode->team.players);
        // Salvez adresa nodului curent intr-un nod temporar pentru a nu pierde referinta
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
    // Deschide fisierul de iesire in modul "wt" (scriere text)
    FILE *file = fopen(filePathOutput, "wt");
    if(file == NULL){
        printf("Eroare");
        exit(-1);
    }

    List* currentNode = head;
    while (currentNode != NULL) {
        int length = strlen(currentNode->team.teamName);
        // Verific daca ultimul caracter este ' ' si il inlocuiesc cu '\0'
        if(currentNode->team.teamName[length - 1] == ' '){
            currentNode->team.teamName[length - 1] = '\0';
        }
        fprintf(file, "%s\n", currentNode->team.teamName);
        currentNode = currentNode->next;
    }

    fclose(file);
}

void addAtBeginning(List** head, int nrPlayers, char* teamName, Player* players)
{
    // Creez un nod nou folosind functia de mai devreme
    List* newNode = createNewNode(nrPlayers, teamName, players);
    // Setez pointerul catre urmatorul nod al nodului nou creat pentru a fi nodul curent al listei
    newNode->next = *head;
    // Nodul nou creat devine capul listei
    *head = newNode;
}

void averageTeams(int numberOfTeams, List* head)
{
    List* currentNode = head;
    for(int i = 0; i < numberOfTeams; i++){
        int totalPoints = 0;
        // Retin totalul de puncte al unei echipe
        for(int j = 0; j < currentNode->team.nrPlayers; j++){
            totalPoints += currentNode->team.players[j].points;
        }     
        // Fac media aritmetica pentru a afla media echipei 
        currentNode->team.averageTeam = (float)totalPoints / currentNode->team.nrPlayers;

        currentNode = currentNode->next;   
    }
}

int detN(int numberOfTeams)
{
    // Determin acel N despre care e vorba in problema
    int N = 1;
    // Pentru a fi putere a lui 2, il inmultesc pentru pana cand este mai mare ca numarul de echipe
    while(N < numberOfTeams){
        N = N * 2;
    }
    // N va trece in final de numarul de echipe, de aceea trebuie redusa o putere a lui 2
    if(N > numberOfTeams){
        N = N / 2;
    }
    return N;
}

void task2(List** head, int N, int numberOfTeams, char* filePathOutput)
{
    // Calculez media punctelor pe echipa
    averageTeams(numberOfTeams, *head);
    // Elimin echipe pana cand numarul de echipe este egal cu N
    while(numberOfTeams > N){
        List *currentNode = *head;
        // Initializez un pointer catre echipa cu cel mai mic punctaj 
        List *lowestTeam = *head;
        // Parcurg lista si gasesc echipa cu cel mai mic punctaj 
        while(currentNode != NULL){
            if(currentNode->team.averageTeam < lowestTeam->team.averageTeam){
                lowestTeam = currentNode;
            }
            currentNode = currentNode->next;
        }
        // Daca echipa cu cel mai mic punctaj este capul listei, actualizeaza capul la urmatorul nod
        if(lowestTeam == *head){
            *head = lowestTeam->next;
        }
        else{
            // Gasesc nodul precedent echipei cu cel mai mic punctaj si actualizez referintele
            List *prevNode = *head;
            while(prevNode->next != lowestTeam){
                prevNode = prevNode->next;
            }
            prevNode->next = lowestTeam->next;
        }
        // Scad numarul de echipe
        numberOfTeams--;
    }
    writeTeamsToFile(*head, filePathOutput);
}

int detNumberOfRounds(int numberOfTeams)
{
    // Determin numarul de runde necesare
    int numberOfRounds = 0;
    while(numberOfTeams != 0){
        numberOfTeams = numberOfTeams/2;
        numberOfRounds++;
    }
    // Scad 1 pentru ca desi numarul de echipe ajunge la 0, numarul de runde tot va creste cu 1
    return numberOfRounds - 1;
}

void task3(List** head, char* filePathOutput, Queue** matchQueue, int *counter, int numberOfTeams, int numberOfRounds, BSTList **headBST)
{
    // Creez coada pentru meciuri
    createMatchQueue(*head, *matchQueue);
    // Scriu echipele in fisierul de iesire
    writeTeamsToFile(*head, filePathOutput);
    // Se executa pana cand se atinge numarul maxim de runde
    while(*counter <= numberOfRounds){
        // Initializez un pointer catre nodul curent din caoda de meciuri
        QueueNode* currentNode = (*matchQueue)->front;
        LosersStack* losersTop = NULL;
        WinnersStack* winnersTop = NULL;
        // Parcurg lista pentru a trece in stive echipele invise si castigatoare
        for(int i = 0; i < numberOfTeams && currentNode != NULL; i++){
            pushLosers(&losersTop, currentNode);
            pushWinners(&winnersTop, currentNode);
            currentNode = currentNode->next;
        }
        // Scriu coada in fisierul de iesire
        writeQueueToFile(*matchQueue, filePathOutput, *head, *counter);
        // Scriu invingatorii rundei curentein fisierul de iesire
        printRoundWinners(*counter, winnersTop, filePathOutput);
        // Elimin invinsii rundei curente
        popLosers(&losersTop);
        // Convertesc stiva de invingatori intr-o lista
        List* headCopy = convertWinnersStackToList(winnersTop);
        // Gasesc numarul de echipe din lista curenta pentru taskul 4
        int count = getNumberOfTeams(headCopy);
        // Daca se gaseste lista cu ultimele 8 echipe, copiez acea lista intr-o lista BST
        if(count == 8){
            *headBST = copyListToBSTList(headCopy, filePathOutput);
        }
        // Cresc numarul de runde
        (*counter)++;
        // Incep sa creez o noua coada de meciuri folosind lista de invingatori
        *matchQueue = createQueue();
        createMatchQueue(headCopy, *matchQueue);
    }
}

void task4(BSTList* headBST, char* filePathOutput)
{
    // Deschid fisierul de iesire in modul "a" (adaugare)
    FILE* file = fopen(filePathOutput, "a");
    if(file == NULL){
        puts("Eroare");
        exit(-1);
    }
    // Copiez lista creata anterior intr-un BST
    BST* root = copyListToBST(headBST);
    fprintf(file, "\nTOP %d TEAMS:\n", 8);
    fclose(file);
    // Scrie in fisierul de iesire echipele in ordine descrescatoare
    printInDescendingOrder(root, filePathOutput);
    free(root);
}

BSTList* copyListToBSTList(List* head, char* filePathOutput) {
    // Initializez lista BST
    BSTList* headBSTList = NULL;
    BSTList* current = NULL;
    // Parcurg lista initiala de echipe
    while (head != NULL) {
        BSTList* newNode = (BSTList*)malloc(sizeof(BSTList));
        // Copiez informatiile echipe din nodul curent al listei initiale in noul nod al listei BST
        newNode->team = head->team;
        newNode->next = NULL;
        // Daca lista BST este goala, setez noul nod ca primul nod al listei BST si 
        // setez nodul curent la noul nod
        if (headBSTList == NULL) {
            headBSTList = newNode;
            current = headBSTList;
        } else {
            // Daca lista BST nu este goala, adaug noul nod la sfarsitul listei prin actualizarea 
            // legaturii a nodului curent 
            current->next = newNode;
            current = current->next;
        }

        head = head->next;
    }
    // Returnez capul listei BST
    return headBSTList;
}

void printInDescendingOrder(BST* root, char* filePathOutput) {
    FILE* file = fopen(filePathOutput, "a");
    if(file == NULL){
        puts("Eroare");
        exit(1);
    }
    // Scriu in inordine (modificat) echipele, in ordine descrescatoare
    if (root != NULL) {
        printInDescendingOrder(root->right, filePathOutput);
        fprintf(file, "%-34s-  %.2f\n", root->team.teamName, root->team.averageTeam);
        fclose(file);
        printInDescendingOrder(root->left, filePathOutput);
    }
}

void freeBST(BST* root) {
    // Eliberez memoria pentru subarborele stang, drept si numele echipei
    if (root != NULL) {
        freeBST(root->left);
        freeBST(root->right);
        free(root->team.teamName);
        free(root);
    }
}

BST* copyListToBST(BSTList* head) {
    BST* root = NULL;
    BSTList* currentNode = head;

    while (currentNode != NULL) {
        // Creez un noua variabila de tipul Team pentru a stoca informatiile 
        Team team;
        // Copiaza numele echipei si punctele echipei
        team.teamName = strdup(currentNode->team.teamName);
        team.averageTeam = currentNode->team.averageTeam;
        // Inserez noul nod in BST
        insertBSTNode(&root, team);

        currentNode = currentNode->next;
    }

    return root;
}

BST* createBSTNode(Team team) {
    BST* newNode = (BST*)malloc(sizeof(BST));
    // Setez valorile din noul nod
    newNode->team = team;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void insertBSTNode(BST** root, Team team) {
    if (*root == NULL) {
        BST* newNode = createBSTNode(team);
        // Setez nodul creat drept radacina 
        *root = newNode;
    } else {
        // Compar punctajele echipelor 
        // Apelez functia recursiv pentru subarborele stang sau drept
        if (team.averageTeam < (*root)->team.averageTeam) {
            insertBSTNode(&((*root)->left), team);
        } else if(team.averageTeam > (*root)->team.averageTeam){
            insertBSTNode(&((*root)->right), team);
        } else {
            // Daca numele echipei este mai mare lexicografic, apelez pentru subarborele drept
            if(strcmp(team.teamName, (*root)->team.teamName) > 0){
                insertBSTNode(&((*root)->right), team);
            }
            // Invers pentru stang
            else{
                insertBSTNode(&((*root)->left), team);
            }
        }
    }
}

int getNumberOfTeams(List* head) {
    int count = 0;
    List* currentNode = head;
    // Cat timp lista contine echipe, count se incrementeaza si returneaza in final numarul de echipe
    while (currentNode != NULL) {
        count++;
        currentNode = currentNode->next;
    }

    return count;
}

Queue* createQueue()
{
    // Aloc memorie pentru inceputul unei cozi
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

int isQueueEmpty(Queue* queue)
{
    // Verific daca coada este goala 
    return (queue->front == NULL);
}

void deQueue(Queue* queue)
{
    // Verific daca coada este goala
    if(isQueueEmpty(queue)){
        return;
    }
    // Setez nodul din fata intr-o variabila temporara
    QueueNode* temp = queue->front;
    // Mut pointerul din fata la urmatorul nod
    queue->front = queue->front->next;    
    if(queue->front == NULL){
        // Daca pointerul din fata e null, actualizez si pe cel din spate
        queue->rear = NULL;
    }
    free(temp);
}

void freeQueue(Queue* queue)
{
    // Eliberez toate elementele din coada pana cand devine goala
    while(!isQueueEmpty(queue)){
        deQueue(queue);
    }
    free(queue);
}

void enQueue(Queue* queue, List* team1, List* team2)
{
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    // Initializez valorile echipei pentru noul nod
    newNode->team1 = team1;
    newNode->team2 = team2;
    newNode->next = NULL;
    if(isQueueEmpty(queue)){
        // Daca coada este goala, setez noul nod ca fiind primul si ultimul
        queue->front = newNode;
        queue->rear = newNode;
    }
    else{
        // Adaug noul nod la sfarsitul cozii
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

void createMatchQueue(List* head, Queue* queue)
{
    List* currentNode = head;
    // Parcurg lista pana la sfarsit sau pana cand nu mai sunt echipe suficiente pentru un meci
    while(currentNode != NULL && currentNode->next != NULL){
        enQueue(queue, currentNode, currentNode->next);
        currentNode = currentNode->next->next;
    }
}

void printQueue(Queue* queue)
{
    // Scrie pe ecran meciurile dintre echipe
    QueueNode* currentNode = queue->front;
    while(currentNode != NULL){
        printf("Match: %s vs %s\n", currentNode->team1->team.teamName, currentNode->team2->team.teamName);
        currentNode = currentNode->next;
    }   
}

void writeQueueToFile(Queue* queue, char* filePathOutput, List *head, int round)
{
    FILE* file = fopen(filePathOutput, "a");
    if(file == NULL){
        puts("Eroare");
        exit(1);
    }
    QueueNode *currentNode = queue->front;
    fprintf(file, "\n--- ROUND NO:%d\n", round);
    // Scrie in fisierul de iesire meciurile dintre echipe
    while(currentNode != NULL){
        fprintf(file, "%-33s-%33s\n", currentNode->team1->team.teamName, currentNode->team2->team.teamName);
            currentNode = currentNode->next;
    }
    fclose(file);
}

void pushLosers(LosersStack** losersTop, QueueNode* currentNode)
{
    LosersStack* newNodeInLosers = (LosersStack*)malloc(sizeof(LosersStack));
    // Verific care echipa are punctajul mai mic si copiez valoarea in stiva pierzatorilor
    if(currentNode->team1->team.averageTeam < currentNode->team2->team.averageTeam){
        newNodeInLosers->losers.teamName = currentNode->team1->team.teamName;
    }
    else {
        newNodeInLosers->losers.teamName = currentNode->team2->team.teamName;
    }
    // Adaug noul nod in varful stivei
    newNodeInLosers->next = *losersTop;
    *losersTop = newNodeInLosers; 
}

void pushWinners(WinnersStack** winnersTop, QueueNode* currentNode)
{
    WinnersStack* newNodeInWinners = (WinnersStack*)malloc(sizeof(WinnersStack));
    // Verific care echipa are punctajul mai mare si copiez valoarea in stiva castigatorilor
    if(currentNode->team1->team.averageTeam > currentNode->team2->team.averageTeam){
        newNodeInWinners->winners.teamName = currentNode->team1->team.teamName;
        newNodeInWinners->winners.averageTeam = currentNode->team1->team.averageTeam + 1;
    }
    else {
        newNodeInWinners->winners.teamName = currentNode->team2->team.teamName;
        newNodeInWinners->winners.averageTeam = currentNode->team2->team.averageTeam + 1;
    }
    // Adaug noul nod in varful stivei
    newNodeInWinners->next = *winnersTop;
    *winnersTop = newNodeInWinners; 
}

int isEmptyStackLosers(LosersStack* losersTop)
{
    // Verific daca stiva de pierzatori este goala
    return losersTop == NULL;
}

int isEmptyStackWinners(WinnersStack* winnersTop)
{
    // Verific daca stiva de castigatori este goala
    return winnersTop == NULL;
}

void popLosers(LosersStack** losersTop)
{
    if(isEmptyStackLosers(*losersTop)){
        printf("Losers stack is empty");
        return;
    }
    // Golesc stiva de pierzatori
    while (*losersTop != NULL) {
        LosersStack* temp = *losersTop;
        *losersTop = (*losersTop)->next;
        free(temp);
    }
}

void printRoundWinners(int round, WinnersStack* winnersTop, char* filePathOutput) {
    FILE* file = fopen(filePathOutput, "a");
    if (file == NULL) {
        printf("Error opening the file.\n");
        exit(1);
    }
    
    fprintf(file, "\nWINNERS OF ROUND NO:%d\n", round);
    WinnersStack* current = winnersTop;
    // Afisez castigatorii rundei respective si punctele echipei
    while (current != NULL) {
        fprintf(file, "%-34s-  %.2f\n", current->winners.teamName, current->winners.averageTeam);
        current = current->next;
        
    }
    fclose(file);
}
void enQueueWinners(Queue* queue, Team team1, Team team2) {
    // Creez un nou nod pentru echipa 1
    List* newNode1 = (List*)malloc(sizeof(List));
    newNode1->team = team1;
    newNode1->next = NULL;
    // Creez un nou nod pentru echipa 2
    List* newNode2 = (List*)malloc(sizeof(List));
    newNode2->team = team2;
    newNode2->next = NULL;
    // Adaug echipele in coada de meciuri
    enQueue(queue, newNode1, newNode2);
}

List* convertWinnersStackToList(WinnersStack* winnersTop) {
    // Initializez capul si coada listei rezultate
    List* listHead = NULL;
    List* listTail = NULL;

    WinnersStack* currentWinner = winnersTop;
    // Parcurg stiva castigatorilor
    while (currentWinner != NULL) {
        List* newNode = (List*)malloc(sizeof(List));
        // Creez un nou nod pentru echipa castigatoare
        newNode->team = currentWinner->winners;
        newNode->next = NULL;
        // Verific daca lista este goala
        if (listHead == NULL) {
            listHead = newNode;
            listTail = newNode;
        } else {
            // Adaug nodul la coada
            listTail->next = newNode;
            listTail = newNode;
        }

        currentWinner = currentWinner->next;
    }
    return listHead;
}

