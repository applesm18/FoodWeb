#include "organism.h"

void IdentifyApexPredators(Org* list, int size)
{
    int i, j, k;
    printf("Apex Predators:\n");
    
    for(i = 0; i < size; i++)
    {
        int eats = 0;
        for(j = 0; j < size; j++)
        {
            for(k = 0; k < list[j].numPrey; k++)
            {
                if(list[j].prey[k] == i) break;
            }

            if(k < list[j].numPrey) break;            
        }

        if(j == size)//this is IdentifyApexPredators
        {
            printf("%s\n", list[i].name);
        }
    }
}

void IdentifyProducers(Org* list, int size)
{
    int i, j, k;
    printf("Producers:\n");
    for(i = 0; i < size; i++)
    {
        if(list[i].numPrey == 0) //this is IdentifyProducers
        {
            printf("%s\n", list[i].name);
        }
    }
}

void IdentifytheMostFlexibleEaters(Org* list, int size)
{
    printf("Most Flexible Eaters:\n");
    int i,most;
    most = 0;
    for(i = most + 1; i < size; i++)
    {
        if(list[most].numPrey < list[i].numPrey) most = i;
    }

    printf("%s\n", list[most].name);
}

void IdentifytheTastiestFood(Org* list, int size)
{
    printf(" \n");
    int i, j, k, most;
    most = 0;
    int most_eaten = 0;
    for(j = 0; j < size; j++)
    {
        if(j == most) continue;
        for(i = 0; i < list[most].numPrey; i++)
        {
            if(list[most].prey[i] == most) most_eaten++;
        }
    }


    for(i = most + 1; i < size; i++)
    {
        int eaten = 0;
        for(k = 0; k < size; k++)
        {
            if(k == i) continue;
            for(j = 0; j < list[most].numPrey; j++)
            {
                if(list[most].prey[j] == i) eaten++;
            }
        }

        if(most_eaten < eaten)
        {
            most_eaten = eaten;
            most = i;
        }
    }

    printf("%s\n", list[most]);
}

int  Heights(Org* list, int size,  int index)
{
    if(list[index].numPrey == 0) return 0;

    int* hs = (int*)malloc(sizeof(int) * list[index].numPrey);
    memset(hs, 0, sizeof(int) * list[index].numPrey);
    int i;
    int maxh = 0;
    for(i = 0; i < list[index].numPrey; i++)
    {
        hs[i] = Heights(list, size, list[index].prey[i]);

        if(maxh < hs[i]) maxh = hs[i];
    }
    free(hs);

    return maxh + 1;
}

void FoodWebHeights(Org* list, int size)
{
    printf("Food Web Heights:\n");
    int i;
    for(i = 0; i < size; i++)
    {
        printf("%s:  ", list[i].name);
        printf("%d\n", Heights(list, size, i));
    }
}

bool IsProducer(Org* list, int size, int index)
{
    return (list[index].numPrey == 0);
}

bool IsHerbivore(Org* list, int size, int index)
{
    int i;
    for(i = 0; i < list[index].numPrey; i++)
    {
        if(!IsProducer(list, size, list[index].prey[i])) return false;
    }

    return true;
}

bool IsOmnivore(Org* list, int size, int index)
{
    int i;
    int producer = 0;
    int nonproducer = 0;
    for(i = 0; i < list[index].numPrey; i++)
    {
        if(!IsProducer(list, size, list[index].prey[i]))
        {
            nonproducer++;
        }
        else{
            producer++;
        }
    }

    if(producer > 0 && nonproducer >0) return true;

    return false;
}

bool IsCarnivore(Org* list, int size, int index)
{
    int i;
    for(i = 0; i<list[index].numPrey; i++)
    {
        if(IsProducer(list, size, list[index].prey[i])) return false;
    }

    if(list[index].numPrey == 0) return false;
    return true;
}


void CategorizeOrganismsbyVoreType(Org* list, int size)
{
    int i;
    printf("\n");
    printf("Vore Types:\n");

    printf("    Producers:\n");
    for(i = 0; i < size; i++)
    {
        if(IsProducer(list, size, i)) printf("      %s\n", list[i].name);
    }

    printf("    Herbivores:\n");
    for(i = 0; i < size; i++)
    {
        if(IsHerbivore(list, size, i)) printf("     %s\n", list[i].name);
    }

    printf("    Omnivores:\n");
    for(i = 0; i < size; i++)
    {
        if(IsOmnivore(list, size, i)) printf("      %s\n", list[i].name);
    }


    printf("    Carnivores:\n");
    for(i = 0; i < size; i++)
    {
        if(IsCarnivore(list, size, i)) printf("     %s\n", list[i].name);
    }

}

