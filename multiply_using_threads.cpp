/*
Assignment-2
C Program to multiply two matrices using threads.
Submitted by : Ajeet Ujjwal (B14CS004)
*/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

pthread_t *threads;

int r1 , c1 , r2 , c2 , i , j , cur_row=0 , cur_col=0;
int **arr1 , **arr2 , **res;
char fname1[50] , fname2[50];

 /* Mutex for Locking. */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to multiply a given row of matrix to col of another matrix and store the sum in resultant matrix using thread
void *MultiplyUsingThread()

    {
            pthread_mutex_lock(&mutex);
            int sum=0,i;

            for(i=0;i<c1;i++)
                    {
                        sum+=arr1[cur_row][i]*arr2[i][cur_col];
                    }

            res[cur_row][cur_col]=sum;
            cur_col++;

            if(cur_col==c2)
                    {
                        cur_row++;
                        cur_col=0;
                    }
            pthread_mutex_unlock(&mutex);
            pthread_exit(0);

    }


// Allocate Memory to @-D matrices
allocate_memory(int ***arr,int row,int col)

    {
            int i;
            *arr = (int **)malloc(row * sizeof(int *));
            for (i=0; i<row; i++)
                 (*arr)[i] = (int *)malloc(col * sizeof(int));
    }


// Extract Matrix data from txt file fname and store in 2-D array
store_matrix(int r,int c,char *fname,int **arr)

    {
            int i,j;
            FILE* fp = fopen(fname,"r");
            for(i = 0; i < r; i++)
                    {
                        for(j = 0; j < c; j++)
                                {
                                    if (!fscanf(fp, "%d", &arr[i][j]))
                                        break;

                                }
                    }
            fclose(fp);

    }


int main()
{

    int choice=1,numberOfThreads;

    while(choice){

                    cur_row=0,cur_col=0;

                    printf("Enter the matrix 1 size : ");
                    scanf("%d %d",&r1,&c1);

                    printf("Enter the matrix 2 size : ");
                    scanf("%d %d",&r2,&c2);

                    printf("Enter the matrix 1 filename : ");
                    scanf("%s",fname1);

                    printf("Enter the matrix 2 filename : ");
                    scanf("%s",fname2);

                    printf("Enter the no of threads : ");
                    scanf("%d",&numberOfThreads);

                    if(c1 != r2)
                        {
                            printf("\nThese two matrices can not be multiplied !!!\n");
                            printf("\nDo you want to continue ? ( Press 1 to Continue , 0 to Exit ) \n");

                            scanf("%d",&choice);
                            continue;
                        }

                    // create dynamic thread array as specified by user
                    if(numberOfThreads>r1*c2)
                        numberOfThreads=r1*c2;
                    threads = (pthread_t *) malloc(sizeof(pthread_t) * numberOfThreads);

                    // allocate memory to all three matrices
                    allocate_memory(&arr1,r1,c1);
                    allocate_memory(&arr2,r2,c2);
                    allocate_memory(&res,r1,c2);

                    // store data of txt files in 2-D arrays
                    store_matrix(r1,c1,fname1,arr1);
                    store_matrix(r2,c2,fname2,arr2);




     /* Start Distributing the work. */


                    int count=0;
                    if(numberOfThreads>r1*c2)
                    numberOfThreads=r1*c2;
                    for (i = 0; i < r1*c2; i++) {

                            if(count<numberOfThreads)
                                    {
                                        pthread_create(&threads[count], NULL,MultiplyUsingThread,NULL);
                                        count++;
                                    }
                            else
                                    {
                                        count--;
                                        pthread_join(threads[count],NULL);
                                        pthread_create(&threads[count],NULL,MultiplyUsingThread,NULL);
                                        count++;
                                    }

                    }

                    for(i=0;i<numberOfThreads;i++)
                    pthread_join(threads[i],NULL);


                // Print Result
                    printf("\nMatrix  1  is  :  \n");

                    for(i=0;i<r1;i++){

                            printf("\n");
                                for(j=0;j<c1;j++)
                                    printf("%d   ",arr1[i][j]);
                    }

                    printf("\nMatrix  2  is  :  \n");

                    for(i=0;i<r2;i++){

                            printf("\n");
                                for(j=0;j<c2;j++)
                                printf("%d   ",arr2[i][j]);
                    }

                    //printf("\nNo of threads used  is  :  %d ",numberOfThreads);
                    //printf("\n");


                    printf("\nResult   is  :  \n");

                    for (i = 0; i < r1; i++) {
                        printf("\n");
                            for (j = 0; j < c2; j++)
                                printf("%d   ", res[i][j]);

                    }
                    printf("\nDo you want to continue ? ( Press 1 to Continue , 0 to Exit ) \n");

                    scanf("%d",&choice);

    }
}
