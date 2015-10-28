#ifndef TERGENBOX_C
#define TERGENBOX_C

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <math.h>

#include "randlib.h"

class Box
{
    int TLi;
    int TLj;
    int TRi;
    int TRj;
    int BLi;
    int BLj;
    int BRi;
    int BRj;

public:
    //Constructors
    Box();
    Box(const Box &instance);
    Box(int TLj,int TLi,int TRj,int TRi,int BLj,int BLi,int BRj,int BRi);

    //Box Functions
    void SetBox(int TLj,int TLi,int TRj,int TRi,int BLj,int BLi,int BRj,int BRi);
    void SetMiddle(std::vector< std::vector<int> > &Data,int RandomPeak);
    void SetEdge(std::vector< std::vector<int> > &Data,int RandomPeak);
    bool ProduceNewIdx(std::vector<Box> &Boxes);
    void PrintBoxIdx();
};

#endif
