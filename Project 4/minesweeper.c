#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <io.h>
#include <fcntl.h>

#define N 35
#define _O_U16TEXT 0x20000


int M[N][N];
int dM[N][N];
int start=1;
int flags=0;

int build_matrix(int M[][N], int a, int b, int mines){
    int *p=&M[0][0];
    // printf("no");
    for (int i=0; i<a; i++){
        for (int j=0; j<b; j++){
            *(p + i*N + j)=0;
            // printf("%d ", M[i][j]);
        }
        // printf("\n");
    }
    // printf("mo");
    int mc=0;
    int i, j;
    while (mc<mines){
        i = rand()%(a);
        j = rand()%(b);
        if ( M[i][j] != 9){
            M[i][j] = 9;
            mc++;
        }
    }
    // printf("yo");
    return M[a][b];
}

int build_zero(int dM[][N], int a, int b, int mines){
    int *p=&dM[0][0];
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            *(p + i*N + j)=-1;
        }
    }
    // return dM[N][N];
}

int alive_neighbours(int M[][N], int a, int b, int i, int j){
    int sum=0;
    if (M[i][j]==9){
        sum=81;
    }
    else if ((i==0) && (j==0)){
        sum+=(M[0][1]);
        sum+=(M[1][0]);
        sum+=(M[1][1]);
    }
    else if ((i==0) && (j==(b-1))){
        sum+=(M[0][b-2]);
        sum+=(M[1][b-1]);
        sum+=(M[1][b-2]);
    }
    else if ((i==(a-1)) && (j==0)){  
        sum+=(M[a-2][0]);
        sum+=(M[a-1][1]);
        sum+=(M[a-2][1]);
    }
    else if ((i==(a-1)) && (j==(b-1))){
        sum+=(M[a-2][b-1]);
        sum+=(M[a-1][b-2]);
        sum+=(M[a-2][b-2]);
    }
    else if (i==0 && j>0 && (j<(b-1))){
        sum+=(M[0][j-1]);
        sum+=(M[0][j+1]);
        sum+=(M[1][j-1]);
        sum+=(M[1][j]);
        sum+=(M[1][j+1]);
    }
    else if (i==(a-1) && j>0 && (j<(b-1))){
        sum+=(M[a-1][j-1]);
        sum+=(M[a-1][j+1]);
        sum+=(M[a-2][j-1]);
        sum+=(M[a-2][j]);
        sum+=(M[a-2][j+1]);
    }
    else if (j==0 && i>0 && (i<(a-1))){
        sum+=(M[i-1][0]);
        sum+=(M[i+1][0]);
        sum+=(M[i-1][1]);
        sum+=(M[i][1]);
        sum+=(M[i+1][1]);
    }
    else if (j==(b-1) && i>0 && (i<(a-1))){
        sum+=(M[i-1][b-1]);
        sum+=(M[i+1][b-1]);
        sum+=(M[i-1][b-2]);
        sum+=(M[i][b-2]);
        sum+=(M[i+1][b-2]);
    }
    else if (i>0 && (i<(a-1)) && j>0 && (j<(b-1))){
        sum+=(M[i-1][j-1]);
        sum+=(M[i-1][j]);
        sum+=(M[i-1][j+1]);
        sum+=(M[i][j-1]);
        sum+=(M[i][j+1]);
        sum+=(M[i+1][j-1]);
        sum+=(M[i+1][j]);
        sum+=(M[i+1][j+1]);
    }
    return (sum/9);
}

void printMatrix(int dM[][N], int a, int b, int mines) {
    int *p=&dM[0][0];
    wprintf(L"\n");

    wprintf(L"Mines: %d\n", mines);
    wprintf(L"You have flagged %d potential mines till now.\n", flags);

    for (int i=0; i<a; i++){
        wprintf(L"\t\t");
        if (i==0){
            wprintf(L"     ");
            for (int k=0; k<b; k++){
                wprintf(L" %2d",k);
            }
            wprintf(L"\n");
            wprintf(L"\t\t");

            wprintf(L"     ——");
            for (int k=0; k<b; k++){
                wprintf(L"———");
            }
            wprintf(L"\n");
            wprintf(L"\t\t");

        }
        for (int j=0; j<b; j++){
            if (j==0){
                wprintf(L" %2d | ",i);
            }

            _setmode(_fileno(stdout), _O_U16TEXT);
            if (*(p + i*N + j)==-1){
                wprintf(L" \x25A0 ");
                // printf("%d ", *(p + i*N + j));
            }            
            
            else if (*(p + i*N + j)==9){
                //this is supposed to be a skull
                wprintf(L" \x2620 ");
            }

            else if (*(p + i*N + j)==0){
                // this is a ballot box
                wprintf(L" \x2610 ");

                // printf("# ");
                //this is a white flag
                // wprintf(L" \x2690 ");

                //this is a cross in a box
                // wprintf(L" \x2327");

                //this is a cross
                // wprintf(L" \x2573");
                // printf("%d ", *(p + i*N + j));
            }
            else if (*(p + i*N + j)==10){
                wprintf(L" \x2691 ");
            }

            else{
                wprintf(L" %d ", dM[i][j]);
            }
        }
        wprintf(L"\n");
        // wprintf(L"dwssq");
    }
    
    return;
}

int Flag(int a, int b, int mines, int ei, int ej){

    if (dM[ei][ej]==-1){
        dM[ei][ej]=10;
        flags++;
    }

    else if (dM[ei][ej]==10){
        wprintf(L"\nFlag removed! \n");
        flags=flags-1;
    }

    else{
        wprintf(L"\n The selected area has already been explored!\nChoose different coordinates.\n");
    }

    printMatrix(dM, a, b, mines);
}

void reportneighbours(int a, int b, int mines, int ei, int ej){
    if (dM[ei][ej]==-1){
        int an;
        an=alive_neighbours(M, a, b, ei, ej);
        dM[ei][ej]=an;
        if (dM[ei][ej]==0){
            if ((ei==0) && (ej==0)){
                reportneighbours(a, b, mines, 0, 1);
                reportneighbours(a, b, mines, 1, 0);
                reportneighbours(a, b, mines, 1, 1);
            }
            else if ((ei==0) && (ej==(b-1))){
                reportneighbours(a, b, mines, 0, b-2);
                reportneighbours(a, b, mines, 1, b-1);
                reportneighbours(a, b, mines, 1, b-2);
            }
            else if ((ei==(a-1)) && (ej==0)){
                reportneighbours(a, b, mines, a-2, 0);
                reportneighbours(a, b, mines, a-1, 1);
                reportneighbours(a, b, mines, a-2, 1);
            }
            else if ((ei==(a-1)) && (ej==(b-1))){
                reportneighbours(a, b, mines, a-2, a-1);
                reportneighbours(a, b, mines, a-1, a-2);
                reportneighbours(a, b, mines, a-2, a-2);
            }
            else if (ei==0 && ej>0 && (ej<(b-1))){
                reportneighbours(a, b, mines, 0, ej-1);
                reportneighbours(a, b, mines, 0, ej+1);
                reportneighbours(a, b, mines, 1, ej-1);
                reportneighbours(a, b, mines, 1, ej);
                reportneighbours(a, b, mines, 1, ej+1);
            }
            else if (ei==(a-1) && ej>0 && (ej<(b-1))){
                reportneighbours(a, b, mines, a-1, ej-1);
                reportneighbours(a, b, mines, a-1, ej+1);
                reportneighbours(a, b, mines, a-2, ej-1);
                reportneighbours(a, b, mines, a-2, ej);
                reportneighbours(a, b, mines, a-2, ej+1);
            }
            else if (ej==0 && ei>0 && (ei<(a-1))){
                reportneighbours(a, b, mines, ei-1, 0);
                reportneighbours(a, b, mines, ei+1, 0);
                reportneighbours(a, b, mines, ei-1, 1);
                reportneighbours(a, b, mines, ei, 1);
                reportneighbours(a, b, mines, ei+1, 1);
            }
            else if (ej==(b-1) && ei>0 && (ei<(a-1))){
                reportneighbours(a, b, mines, ei-1, b-1);
                reportneighbours(a, b, mines, ei+1, b-1);
                reportneighbours(a, b, mines, ei-1, b-2);
                reportneighbours(a, b, mines, ei, b-2);
                reportneighbours(a, b, mines, ei+1, b-2);
            }
            else if (ei>0 && (ei<(a-1)) && ej>0 && (ej<(b-1))){
                reportneighbours(a, b, mines, ei-1, ej-1);
                reportneighbours(a, b, mines, ei-1, ej);
                reportneighbours(a, b, mines, ei-1, ej+1);
                reportneighbours(a, b, mines, ei, ej-1);
                reportneighbours(a, b, mines, ei, ej+1);
                reportneighbours(a, b, mines, ei+1, ej-1);
                reportneighbours(a, b, mines, ei+1, ej);
                reportneighbours(a, b, mines, ei+1, ej+1);
            }
        }
    }

    else if (M[ei][ej]==9 && dM[ei][ej]!=10){
        // dM[ei][ej]==9;
        Flag(a, b, mines, ei, ej);
        // wprintf(L"\nBomb!!  Bomb!!  Bomb!!  Bomb!!  Bomb!! \n");
        // wprintf(L"\nBLAST!!\n");
        printMatrix(dM, a, b, mines);
    }
}

int flag_neighbours(int a, int b, int ei, int ej){
    int fs=0;
    if ((ei==0) && (ej==0)){
        if (dM[0][1]==10){
            fs++;
        }
        if (dM[1][0]==10){
            fs++;
        }
        if (dM[1][1]==10){
            fs++;
        }
    }
    else if ((ei==0) && (ej==(b-1))){
        if (dM[0][b-2]==10){
            fs++;
        }
        if (dM[1][b-1]==10){
            fs++;
        }
        if (dM[1][b-2]==10){
            fs++;
        }
    }
    else if ((ei==(a-1)) && (ej==0)){
        if (dM[a-2][0]==10){
            fs++;
        }
        if (dM[a-1][1]==10){
            fs++;
        }
        if (dM[a-2][1]==10){
            fs++;
        }
    }
    else if ((ei==(a-1)) && (ej==(b-1))){
        if (dM[a-2][b-1]==10){
            fs++;
        }
        if (dM[a-1][b-2]==10){
            fs++;
        }
        if (dM[a-2][b-2]==10){
            fs++;
        }
    }
    else if (ei==0 && ej>0 && (ej<(b-1))){
        if (dM[0][ej-1]==10){
            fs++;
        }
        if (dM[0][ej+1]==10){
            fs++;
        }
        if (dM[1][ej-1]==10){
            fs++;
        }
        if (dM[1][ej]==10){
            fs++;
        }
        if (dM[1][ej+1]==10){
            fs++;
        }
    }
    else if (ei==(a-1) && ej>0 && (ej<(b-1))){
        if (dM[a-1][ej-1]==10){
            fs++;
        }
        if (dM[a-1][ej+1]==10){
            fs++;
        }
        if (dM[a-2][ej-1]==10){
            fs++;
        }
        if (dM[a-2][ej]==10){
            fs++;
        }
        if (dM[a-2][ej+1]==10){
            fs++;
        }
    }
    else if (ej==0 && ei>0 && (ei<(a-1))){
        if (dM[ei-1][0]==10){
            fs++;
        }
        if (dM[ei+1][0]==10){
            fs++;
        }
        if (dM[ei-1][1]==10){
            fs++;
        }
        if (dM[ei][1]==10){
            fs++;
        }
        if (dM[ei+1][1]==10){
            fs++;
        }
    }
    else if (ej==(b-1) && ei>0 && (ei<(a-1))){
        if (dM[ei-1][b-1]==10){
            fs++;
        }
        if (dM[ei+1][b-1]==10){
            fs++;
        }
        if (dM[ei-1][b-2]==10){
            fs++;
        }
        if (dM[ei][b-2]==10){
            fs++;
        }
        if (dM[ei+1][b-2]==10){
            fs++;
        }
    }
    else if (ei>0 && (ei<(a-1)) && ej>0 && (ej<(b-1))){
        if (dM[ei-1][ej-1]==10){
            fs++;
        }
        if (dM[ei-1][ej]==10){
            fs++;
        }
        if (dM[ei-1][ej+1]==10){
            fs++;
        }
        if (dM[ei][ej-1]==10){
            fs++;
        }
        if (dM[ei][ej+1]==10){
            fs++;
        }
        if (dM[ei+1][ej-1]==10){
            fs++;
        }
        if (dM[ei+1][ej]==10){
            fs++;
        }
        if (dM[ei+1][ej+1]==10){
            fs++;
        }
    }
    
    return fs;
}

int Mine(int dM[][N], int a, int b, int mines, int ei, int ej){
    
    if (M[ei][ej]==9){
        dM[ei][ej]==9;
        wprintf(L"\nBomb!!  Bomb!!  Bomb!!  Bomb!!  Bomb!! \n");
        wprintf(L"\nBLAST!!\n");
        reportneighbours(a, b, mines, ei, ej);
        printMatrix(dM, a, b, mines);
        wprintf(L"\n\t\tMine Collapsed!! You lose!!\n");
        start=0;
    }
    else if (dM[ei][ej]!=-1 && dM[ei][ej]!=10){
        if ((flag_neighbours(a, b, ei, ej)) == (dM[ei][ej])){

            if ((ei==0) && (ej==0)){
                reportneighbours(a, b, mines, 0, 1);
                reportneighbours(a, b, mines, 1, 0);
                reportneighbours(a, b, mines, 1, 1);
            }
            else if ((ei==0) && (ej==(b-1))){
                reportneighbours(a, b, mines, 0, b-2);
                reportneighbours(a, b, mines, 1, b-1);
                reportneighbours(a, b, mines, 1, b-2);
            }
            else if ((ei==(a-1)) && (ej==0)){
                reportneighbours(a, b, mines, a-2, 0);
                reportneighbours(a, b, mines, a-1, 1);
                reportneighbours(a, b, mines, a-2, 1);
            }
            else if ((ei==(a-1)) && (ej==(b-1))){
                reportneighbours(a, b, mines, a-2, a-1);
                reportneighbours(a, b, mines, a-1, a-2);
                reportneighbours(a, b, mines, a-2, a-2);
            }
            else if (ei==0 && ej>0 && (ej<(b-1))){
                reportneighbours(a, b, mines, 0, ej-1);
                reportneighbours(a, b, mines, 0, ej+1);
                reportneighbours(a, b, mines, 1, ej-1);
                reportneighbours(a, b, mines, 1, ej);
                reportneighbours(a, b, mines, 1, ej+1);
            }
            else if (ei==(a-1) && ej>0 && (ej<(b-1))){
                reportneighbours(a, b, mines, a-1, ej-1);
                reportneighbours(a, b, mines, a-1, ej+1);
                reportneighbours(a, b, mines, a-2, ej-1);
                reportneighbours(a, b, mines, a-2, ej);
                reportneighbours(a, b, mines, a-2, ej+1);
            }
            else if (ej==0 && ei>0 && (ei<(a-1))){
                reportneighbours(a, b, mines, ei-1, 0);
                reportneighbours(a, b, mines, ei+1, 0);
                reportneighbours(a, b, mines, ei-1, 1);
                reportneighbours(a, b, mines, ei, 1);
                reportneighbours(a, b, mines, ei+1, 1);
            }
            else if (ej==(b-1) && ei>0 && (ei<(a-1))){
                reportneighbours(a, b, mines, ei-1, b-1);
                reportneighbours(a, b, mines, ei+1, b-1);
                reportneighbours(a, b, mines, ei-1, b-2);
                reportneighbours(a, b, mines, ei, b-2);
                reportneighbours(a, b, mines, ei+1, b-2);
            }
            else if (ei>0 && (ei<(a-1)) && ej>0 && (ej<(b-1))){
                reportneighbours(a, b, mines, ei-1, ej-1);
                reportneighbours(a, b, mines, ei-1, ej);
                reportneighbours(a, b, mines, ei-1, ej+1);
                reportneighbours(a, b, mines, ei, ej-1);
                reportneighbours(a, b, mines, ei, ej+1);
                reportneighbours(a, b, mines, ei+1, ej-1);
                reportneighbours(a, b, mines, ei+1, ej);
                reportneighbours(a, b, mines, ei+1, ej+1);

            // reportneighbours(a, b, mines, ei, ej);
            printMatrix(dM, a, b, mines);
            }
        }
    }
    else if (dM[ei][ej]!=10){
        reportneighbours(a, b, mines, ei, ej);
        printMatrix(dM, a, b, mines);
        int *p=&dM[0][0];
        int unexplored=0;
        for (int i=0; i<a; i++){
            for (int j=0; j<b; j++){
                if (*(p + i*N + j)==-1){
                    unexplored++;
                }
            }
        }
        if (unexplored==0 || (unexplored==mines)){
            wprintf(L"\nYou win!!\n");
            start=0;
        }
    }
}

void Scout_mines_in_row(int a, int b, int mines, int ei, int ej){
    int ans=0, r=0, l=0;
    for (int x=0; x<b; x++){
        if (M[ei][x]==9){
            ans++;
            if (x<ej){
                l++;
            }
            else if (x>ej){
                r++;
            }
        }
    }
    wprintf(L"\nThere are %d mines in Row %d\n", ans, ei);
    wprintf(L"\nMines towards Right: %d \nMines towards Left: %d\n", r, l);
    
}

void Scout_mines_in_column(int a, int b, int mines, int ei, int ej){
    int ans=0, u=0, d=0;
    for (int y=0; y<b; y++){
        if (M[y][ej]==9){
            ans++;
            // wprintf(L"r %d c %d",ej,ej);
            if (y<ei){
                u++;
            }
            else if (y>ei){
                d++;
            }
        }
    }
    wprintf(L"\nThere are %d mines in Column %d\n", ans, ej);
    wprintf(L"\nMines Above: %d \nMines Below: %d\n", u, d);
}

int main(){
    srand(time(0));
    int a,b;
    printf("# Serious Warning! #\n# Keep grid dimesions between 5 and 30 for each side #");
    printf("\na: vertical length   b: horizontal length\n");
    printf("\nEnter the dimensions of mine-field (a b): ");
    scanf("%d %d", &a, &b);
    int mines;
    printf("\nIf you want number of mines to be automatic, give input as 0.");
    printf("\nEnter the number of mines [max: (1-a)*(1-b)]: ");
    scanf("%d", &mines);
    
    printf("\n#If you give an absurd data anywhere, I will know!\n");
    printf("Otherwise, it's your game dude, enjoy!\n");

    if ( (a<5) || (b<5) || (a>30) || (b>30) ){
        printf("Invalid grid dimensions!\n");
        return 0;
    }
    else if (mines<0){
        printf("Really? You want number of mines to be negative?\n");
        return 0;
    }
    else if (mines > ((a-1)*(b-1))){
        printf("Number of mines more than allowed value");
        return 0;
    }
    else if (mines==0){
        int m;
        m=(((12.6)*(a*b))/100)+1;
        mines=m;
    }

    // int M[N][N];

    // printf("ju");
    M[a][b]=build_matrix(M, a, b, mines);
    // printMatrix(M, a, b, mines);
    // wprintf(L"\n");
    
    // int dM[N][N];
    build_zero(dM, a, b, mines);
    printMatrix(dM, a, b, mines);
    // wprintf("yo")

    while (start){
        int ei, ej, action;
        wprintf(L"Select the active field (vertical   horizontal): ");
        scanf("%d %d", &ei, &ej);
        wprintf(L"Actions available :- \nMine        (1)       Scout_mines_in_row    (3)\nFlag/Unflag (2)       Scout_mines_in_column (4)  : ");
        scanf("%d", &action);

        switch (action){
            case 1:
                dM[a][b]=Mine(dM, a, b, mines, ei, ej);
                break;

            case 2:
                Flag(a, b, mines, ei, ej);
                break;

            case 3:
                Scout_mines_in_row(a, b, mines, ei, ej);
                break;

            case 4:
                Scout_mines_in_column(a, b, mines, ei, ej);
                break;
                
            default:
                break;
            }

    }
}

