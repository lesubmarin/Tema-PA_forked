#include "tema.h"

int main(int argc, char *argv[])
{
    List* head = NULL;
    // Determin fisierele pe care le voi folosi
    char *filePath_C_in = argv[1];
    char *filePathInput = argv[2];
    char *filePathOutput = argv[3];

    FILE* tasks;
    tasks = fopen(filePath_C_in, "rt");
    if(tasks == NULL){
        puts("Eroare");
        exit(1);
    }
    int task[4];
    int numberOfTasks = 5;
    // Citesc numarul de echipe
    int numberOfTeams = readNumberOfTeams(filePathInput);
    // Determin acel N
    int n = detN(numberOfTeams);
    // Determin numarul de runde
    int numberOfRounds = detNumberOfRounds(numberOfTeams);
    // Creez un vector pentru taskuri
    for(int i = 0; i < numberOfTasks; i++){
        fscanf(tasks, "%d ", &task[i]);
    }
    // Incep crearea cozii initiale
    Queue* matchQueue = createQueue();
    // In variabila voi numara rundele
    int counter = 1;
    BSTList* headBST = (BSTList*)malloc(sizeof(BSTList));
    // In functie de pozitiile pe care se afla cifra 1 in fisierul c.in, apelez taskurile corespunzator
    for(int i = 0; i < numberOfTasks; i++){
        if(task[i] == 1){
            if(i == 0){
                task1(&head, filePathInput, filePathOutput);
            }
            if(i == 1){
                task2(&head, n, numberOfTeams, filePathOutput);
            }
            if(i == 2){
                task3(&head, filePathOutput, &matchQueue, &counter, numberOfTeams, numberOfRounds, &headBST);
            }
            if(i == 3){
                task4(headBST, filePathOutput);
            }
        }
    } 
    freeQueue(matchQueue);
    freeList(head);
    return 0;
}
