#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#define N 12

void build_matrix(int C[][N]){
    int *p=&C[0][0];
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            *(p + i*N + j)=(rand() % 5);
            if (*(p + i*N + j)!=1){
                (*(p + i*N + j)=0);
            }
        }
    }
}

int build_zero(int newC[][N]){
    int *p=&newC[0][0];
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            *(p + i*N + j)=0;
        }
    }
    return newC[N][N];
}

int alive_neighbours(int C[][N], int i, int j){
    int sum=0;
    if ((i==0) && (j==0)){
        sum+=(C[0][1]);
        sum+=(C[1][0]);
        sum+=(C[1][1]);
    }
    else if ((i==0) && (j==(N-1))){
        sum+=(C[0][N-2]);
        sum+=(C[1][N-1]);
        sum+=(C[1][N-2]);
    }
    else if ((i==(N-1)) && (j==0)){  
        sum+=(C[N-2][0]);
        sum+=(C[N-1][1]);
        sum+=(C[N-2][1]);
    }
    else if ((i==(N-1)) && (j==(N-1))){
        sum+=(C[N-2][N-1]);
        sum+=(C[N-1][N-2]);
        sum+=(C[N-2][N-2]);
    }
    else if (i==0 && j>0 && (j<(N-1))){
        sum+=(C[0][j-1]);
        sum+=(C[0][j+1]);
        sum+=(C[1][j-1]);
        sum+=(C[1][j]);
        sum+=(C[1][j+1]);
    }
    else if (i==(N-1) && j>0 && (j<(N-1))){
        sum+=(C[N-1][j-1]);
        sum+=(C[N-1][j+1]);
        sum+=(C[N-2][j-1]);
        sum+=(C[N-2][j]);
        sum+=(C[N-2][j+1]);
    }
    else if (j==0 && i>0 && (i<(N-1))){
        sum+=(C[i-1][0]);
        sum+=(C[i+1][0]);
        sum+=(C[i-1][1]);
        sum+=(C[i][1]);
        sum+=(C[i+1][1]);
    }
    else if (j==(N-1) && i>0 && (i<(N-1))){
        sum+=(C[i-1][N-1]);
        sum+=(C[i+1][N-1]);
        sum+=(C[i-1][N-2]);
        sum+=(C[i][N-2]);
        sum+=(C[i+1][N-2]);
    }
    else{
        sum+=(C[i-1][j-1]);
        sum+=(C[i-1][j]);
        sum+=(C[i-1][j+1]);
        sum+=(C[i][j-1]);
        sum+=(C[i][j+1]);
        sum+=(C[i+1][j-1]);
        sum+=(C[i+1][j]);
        sum+=(C[i+1][j+1]);
    }
    return sum;
}

void printMatrix(int C[][N]) {
    int *p=&C[0][0];
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            if (*(p + i*N + j)==1){
                printf("# ");
                // printf("%d ", *(p + i*N + j));
            }
            else{
                printf(". ");
                // printf("%d ", *(p + i*N + j));
            }
        }
        printf("\n");
    }
    return;
}

int conway_game(int C[][N]) {
    int newC[N][N];
    newC[N][N]=build_zero(newC);
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            int an;
            an=alive_neighbours(C, i, j);
            if ((C[i][j]==0)){
                if (an==3){
                    newC[i][j]=1;
                }
                else{
                    newC[i][j]=0;
                }
            }
            else if (C[i][j]==1){
                if (an==2 || an==3){
                    newC[i][j]=1;
                }
                else{
                    newC[i][j]=0;
                }
            }
        }
        printf("\n");
    }
    printMatrix(newC);
    Sleep(1000);
    system("cls");
    conway_game(newC);
}

int main(){
    srand(time(0));
    int C[N][N];

    build_matrix(C);
    printMatrix(C);
    conway_game(C);
}

