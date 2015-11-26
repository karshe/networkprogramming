/***********************************************
*** WINSOCK BASED TCP CLIENT/SERVER APPLICATION
*** https://github.com/karshe
***********************************************/

// HEADER FILES
#include <stdio.h>

/* SERVER APP */
int ServerApp();
/* CLIENT APP */
int ClientApp();


/* MAIN - Hello_Winsock */
int main(){

    /** VARIABLES **/
    int choice;

    printf("HELLO WINSOCK!\n");

    do{
        printf("SELECT BELOW (1-3)\n");
        printf("1. SERVER \t 2. CLIENT \t 3.EXIT\n");
        scanf_s("%d", &choice);

        if(choice == 1){
            ServerApp();
        }else if(choice == 2){
            ClientApp();
        }

    }while(choice != 3);


    return 0;
}
