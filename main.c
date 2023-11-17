// #include "organism.h"
#include "task2.h"

void main(int argc, const char * argv[])
{
    if(argc < 1) return;

//Task1
    int websize = 0;
    Org* list = buildWeb(&websize);
    printWeb(list, websize);

//Task 2
    IdentifyApexPredators(list, websize);
    IdentifyProducers(list, websize);
    IdentifytheTastiestFood(list, websize);
    FoodWebHeights(list, websize);
    CategorizeOrganismsbyVoreType(list, websize);

//Task 3
    int delenum = 3;
    extinction(&list, &websize, delenum);
    printWeb(list, websize);
}

