
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

// #define LOG

#define MAGIC_FOOD_WEB "Food Web Predators & Prey:"
#define MAGIC_EAT "eats"
#define MAXNAME 20
typedef struct Org_struct {
char name[MAXNAME];
int* prey; //dynamic array of indices
int numPrey;
} Org;

int fsizefromflename(const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1; 
}

int _fsizefromstd(FILE *st_in) {
    struct stat st;
    if (fstat(fileno(st_in), &st) == 0)
        return st.st_size;
    return -1;
}

bool check_margic(char* strmagic)
{
    return (strnicmp(strmagic, MAGIC_FOOD_WEB, strlen(MAGIC_FOOD_WEB)) == 0);
}

int getfindindex(Org* list_org, int size, char* name)
{
    int i = 0; 
    for(i = 0; i < size; i++)
    {
        if(stricmp(list_org[i].name, name) == 0) return i;
    }

    return -1;
}

Org* buildWeb(int* websize)
{
    Org* list_org;
    FILE* fp = stdin;
    char ** relationships;
    int linenum = 0;
    if(fp)
    {
        long size = _fsizefromstd(fp);
        char* line = (char*)malloc(size);
        memset(line, 0, size);
        int readsize = 0;
        relationships = (char**)malloc(size);
        while(fgets(line, size, stdin) && readsize < size)
        {
            *relationships = (char*)malloc(strlen(line)+1);
            memset(*relationships, 0, strlen(line)+1);
            strncpy(*relationships, line, strlen(line));
            #ifdef LOG
            printf("line = %s\n", *relationships);
            #endif
            memset(line, 0, size);
            relationships++;
            linenum++;
            readsize += strlen(line)+1;
        }

        fclose(fp);
        #ifdef LOG
        printf("filesize = %d\n", size);
        #endif

        free(line);
    }
    else{
        return NULL;
    }

    relationships = relationships-linenum;
    #ifdef LOG
    printf("getmargic of relationships[0] = %s", *relationships);
    #endif

    if(!check_margic(*relationships))
    {
        for(int i = 0; i < linenum; i++)
        {
            free(*relationships--);
        }
        return NULL;
    }
    relationships++;

    #ifdef LOG
    printf("check margic ok!!!\n");
    #endif

    list_org = (Org*)malloc(sizeof(Org)*(linenum-1));
    //make organisms
    for(int i = 0; i < linenum-1; i++)
    {
        char* lnode;
        char* relative = *relationships;
        int parselen = 0;
        #ifdef LOG
        printf("relationships = %s\n", relative);
        #endif
        lnode = (char*)malloc(strlen(*relationships));
        memset(lnode, 0, strlen(*relationships));
        int lnodelen = strlen(*relationships);

        while(sscanf(relative, "%s", lnode) && parselen < strlen(*relationships)-1)
        {
            relative += strlen(lnode) + 1;
            parselen += strlen(lnode) + 1;

            if(lnode[strlen(lnode)-1] == ',')lnode[strlen(lnode)-1] = 0x00;
            #ifdef LOG
            printf("lnode = %s\n", lnode);
            #endif

            memset(list_org[i].name, 0, MAXNAME);
            strncpy(list_org[i].name, lnode, strlen(lnode) > MAXNAME ? MAXNAME : strlen(lnode));

            memset(lnode, 0, lnodelen);
            break;
        }
        relationships++;
        free(lnode);
    }

    #ifdef LOG
    printf("make organisms list \n");
    #endif

    #ifdef LOG
    for(int i = 0; i < linenum-1; i++)
    {
        printf("list_org[%d].name = %s \n", i, list_org[i].name);        
    }
    #endif

    #ifdef LOG
    printf("parse predator-prey relationships start!!!\n");
    #endif

    //parse predator-prey relationships
    relationships -= linenum-1;
    for(int i = 0; i < linenum-1; i++)
    {
        char* lnode;
        char* relative = *relationships;
        int parselen = 0;
        #ifdef LOG
        printf("relationships = %s\n", relative);
        #endif
        lnode = (char*)malloc(strlen(*relationships));
        memset(lnode, 0, strlen(*relationships));
        int lnodelen = strlen(*relationships);
        int eats = 0;
        int *prey;
        prey = (int*)malloc(strlen(*relationships)*sizeof(int));
        int numPrey = 0;
        memset(prey, 0, sizeof(int)*strlen(*relationships));

        while(sscanf(relative, "%s", lnode) && parselen < strlen(*relationships)-1)
        {
            relative += strlen(lnode) + 1;
            parselen += strlen(lnode) + 1;

            if(lnode[strlen(lnode)-1] == ',')lnode[strlen(lnode)-1] = 0x00;
            #ifdef LOG
            printf("lnode = %s\n", lnode);
            #endif
            if(strnicmp(lnode, MAGIC_EAT, strlen(MAGIC_EAT)) == 0 && eats == 0)
            {
                eats = 1;
            }
            else{
                if(eats == 1)
                {
                    #ifdef LOG
                    printf("lnode is prey = %s \n", lnode);
                    #endif
                    int index = getfindindex(list_org, linenum-1, lnode);
                    prey[numPrey++] = index;
                }
            }
           

            memset(lnode, 0, lnodelen);            
        }

        list_org[i].prey = (int*)malloc(numPrey * sizeof(int));
        memset(list_org[i].prey, 0, numPrey*sizeof(int));

        for(int j = 0; j < numPrey; j++)
        {
            list_org[i].prey[j] = prey[j];
        }
        list_org[i].numPrey = numPrey;

        free(lnode);
        free(prey);

        relationships++;
    }

    for(int i = 0; i < linenum; i++)
    {
        free(*relationships--);
    }

    *websize = linenum -1;
    
    #ifdef LOG
    printf("buidWeb End websize = %d\n", *websize);
    #endif
    return list_org;
    
}


Org* extinction(Org** oldlist, int* size, int del_index)
{
    int websize = *size;
    Org* list = *oldlist;
    Org* newlist = (Org*)malloc(sizeof(Org) * (websize-1));
    int i,j,cn;

    cn = 0;
    for(i = 0; i < websize; i++)
    {
        if(i == del_index) continue;

        memset(newlist[cn].name, 0, MAXNAME);
        strncpy(newlist[cn].name, list[i].name, strlen(list[i].name));
       
        int numPrey = 0;
        int* prey = (int*)malloc(list[i].numPrey * sizeof(int));

        for(j = 0; j < list[i].numPrey; j++)
        {
            if(list[i].prey[j] == del_index) continue;

            if(list[i].prey[j] > del_index)
            {
                prey[numPrey] = list[i].prey[j] - 1;
                numPrey++;
            }
            else{
                prey[numPrey] = list[i].prey[j];
                numPrey++;
            }
        }

        newlist[cn].numPrey = numPrey;
        newlist[cn].prey = (int*)malloc(sizeof(int)*numPrey);
        for(j = 0; j < numPrey; j++) 
        {
            newlist[cn].prey[j] = prey[j];
        }
        cn++;

        free(prey);
    }

    for(i = 0; i < websize; i++)
    {
        if(list[i].numPrey > 0) free(list[i].prey);
    }
    free(list);

   
    *size = websize - 1;
    *oldlist = newlist;

    return newlist;
}


void printWeb(Org* list, int size)
{
    printf("\n\n\n");
    printf("printWeb start size = %d \n", size);
    int i;
    for(i = 0; i < size; i++)
    {
        printf("\n\n");
        printf("Org Index = %d\n", i);
        printf("name = %s\n", list[i].name);
        printf("numPrey = %d \n", list[i].numPrey);

        for(int j = 0; j < list[i].numPrey; j++)
        {
            printf("prey[%d] = %d\n", j, list[i].prey[j]);
        }
        printf("Org end\n");
    }

    printf("printWeb End!!!\n");
}