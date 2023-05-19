#include "tema.h"

int main(int argc, char *argv[])
{
    List* head = NULL;
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
    int numberOfTeams = readNumberOfTeams(filePathInput);
    int n = detN(numberOfTeams);
    for(int i = 0; i < numberOfTasks; i++){
        fscanf(tasks, "%d ", &task[i]);
    }
    for(int i = 0; i < numberOfTasks; i++){
        if(task[i] == 1){
            if(i == 0){
                task1(&head, filePathInput, filePathOutput);
            }
            if(i == 1){
                task2(&head, n, numberOfTeams, filePathOutput);
            }
        }
    }
    Queue* matchQueue = createQueue();
    createMatchQueue(head, matchQueue);
    //printQueue(matchQueue);
    writeQueueToFile(matchQueue, filePathOutput);
    freeQueue(matchQueue);
    freeList(head);
    return 0;
}