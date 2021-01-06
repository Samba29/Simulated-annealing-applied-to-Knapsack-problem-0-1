// Author: Mbono Samba Dieudonne
// Date: University of Yaounde 1, 04/01/2021
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M 2 // Represent a number of contraint
#define ITERMAX 29861
#define TEMPERATUREINIT 10000
int N = 4; //represent a number of item of our knapsack problem
int Evaluate(int Solution[N], int itemValues[N]);
int BestItemToAdd(int solution[N], int constraint[N][M], int valueConstraint[N], int valuesItem[N], int phase); // A implementer
int VerifyConstraint(int constraint[N][M], int valueConstraint[M], int solution[N]);
int TabuContain(int tabuList[N][N], int solution[N]);
int KnapsackProblemSolveByTabuSearch();
int EvaluateNumberVariable();
int CreateIndex(int solution[N]);
int SearchInIndex(int index[100], int solution[N]);

int main()
{
    N = EvaluateNumberVariable();
    SimulatedAnnealing();
}

void SimulatedAnnealing()
{
    int valuesItem[N];
    int valueConstraint[M];
    int solution[N];
    int constraint[N][M];
    int nbIter = 0;
    int bestSolution[N];
    FILE *fichier = NULL;

    float temperature = TEMPERATUREINIT;
    int numberItemWithoutMove = 0;
    const float alpha = 0.99;
    const int Boltzman = 0.5;

    int neightbor[N];
    float delta = 0.0;
    int acceptanceProbability = 0;
    int changeSolution = 0;
    for(int i = 0; i < N; i++)
        {
            solution[i] = 0;
            bestSolution[i] = 0;
            valuesItem[i] = 0;
        }
    for(int j = 0; j < M; j++)
    {
        valueConstraint[j] = 0;
        for(int i = 0; i < N; i++)
        {
            constraint[i][j] = 0;
        }
    }
    //Add read instruction
    fichier = fopen("TEST.csv","r");
    if(fichier != NULL)
    {
        int pos = 1;
        int i = 0;
        char chaine[100];
        while(fgets(chaine, 1000,fichier) != NULL)
        {
            int temp[N];
            char* pch;
            pch = strtok(chaine, ";");
            int i = 0;
            if(pos == 1) // We take a weight of contraint
            {
                while(pch != NULL)
                {
                    constraint[i][1] = atoi(pch);
                    i+=1;
                    pch = strtok(NULL, " ;");
                }
            }
            if(pos ==2 )//value of each item
            {
                while(pch != NULL)
                {
                    valuesItem[i] = atoi(pch);
                    i+=1;
                    pch = strtok(NULL, " ;");
                }
            }
            if(pos==3)
            {
                valueConstraint[1] =  atoi(pch);
            }
            pos+=1;
        }
    }

    do
    {
        GenerateSolution(neightbor, constraint, valueConstraint);
        delta = Evaluate(neightbor, valuesItem) - Evaluate(solution, valuesItem);
        printf("\n delta: %f \n", delta);
        changeSolution = 0;
        float r;
        printf(" \n la temperature est de: %f, la meilleur solution vaut: %d, c'est:", temperature, Evaluate(bestSolution, valuesItem));
        for(int i = 0; i< N; i++)
            {
                printf("%d", bestSolution[i]);
            }
        if(delta >= 0)
        {
            changeSolution = 1;
            if(Evaluate(bestSolution, valuesItem) <= Evaluate(neightbor, valuesItem))
            {
                for(int i = 0; i < N; i++)
                {
                    bestSolution[i] = neightbor[i];
                }
            }
        }
        else
        {
            acceptanceProbability =  exp(-delta/temperature);
            r = rand()%1000;
            r/=1000;
            if(r <= acceptanceProbability)
                changeSolution = 1;
        }
        if(changeSolution = 1)
        {
            for(int i = 0; i < N; i++)
            {
                solution[i] = neightbor[i];
            }
        }
        else
        {
            numberItemWithoutMove+=1;
        }
        temperature = temperature*alpha - 0.000001;
        nbIter+=1;
    }
    while(temperature >= 0 && nbIter != ITERMAX);
    printf("La valeur optimale est: %d et a ete atteinte en %d iterations", Evaluate(bestSolution, valuesItem), nbIter);
    printf("\nAppuyez sur n'importe quelle bouton pour sortir");
    scanf("%d",NULL);
    return 1;
}

int Evaluate(int Solution[N], int itemValues[N])
{
    int sum = 0;
    for(int i = 0; i < N; i++)
    {
        if (Solution[i] == 1)
            sum += Solution[i]*itemValues[i];
    }
    return sum;
}
int VerifyConstraint(int constraint[N][M], int valueConstraint[M], int solution[N])
{
    for(int j = 0; j < M; j++)
    {
        int valueCol = 0;
        for(int i = 0; i < N; i++)
        {
            valueCol+= constraint[i][j]*solution[i];
        }
        if(valueCol > valueConstraint[j])
        {
            // on suppose que les contraintes sont toutes mises sous la forme AX <= B
            return 0;
        }

    }
    return 1;
}
int EvaluateNumberVariable()
{
    FILE* file = NULL;
    int row = 1;
    char separator = 'a';
    file = fopen("TEST.csv", "r");
    if(file != NULL)
    {
        while(separator != '\n')
        {
            separator = fgetc(file);
            if(separator == ';')
                row+=1;
        }
    }
    return row;
}

void GenerateSolution(int* solution, int constraint[N][M], int valueConstraint[M])
{
    do
    {
        float probality = 0.0;
        for(int i = 0; i < N; i++)
        {
         probality = rand()%100;
         //printf("\n probabilit�: %f", probality);
         if(probality >= 50.0)
         {
            solution[i] = (solution[i] == 0)? 1 : 0;
         }
        }
    }
    while(VerifyConstraint(constraint, valueConstraint, solution) != 1);
}
