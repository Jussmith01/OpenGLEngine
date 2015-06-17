#include "boxclass.h"

Box::Box () {};

Box::Box (const Box &instance)
{
    SetBox(instance.TLj,instance.TLi,instance.TRj,instance.TRi,instance.BLj,instance.BLi,instance.BRj,instance.BRi);
};

Box::Box(int TLj,int TLi,int TRj,int TRi,int BLj,int BLi,int BRj,int BRi)
{
    SetBox(TLj,TLi,TRj,TRi,BLj,BLi,BRj,BRi);
};

void Box::SetBox(int TLj,int TLi,int TRj,int TRi,int BLj,int BLi,int BRj,int BRi)
{
    this->TLi=TLi;
    this->TLj=TLj;
    this->TRi=TRi;
    this->TRj=TRj;
    this->BLi=BLi;
    this->BLj=BLj;
    this->BRi=BRi;
    this->BRj=BRj;
};

void Box::SetMiddle(std::vector< std::vector<int> > &data,int RandomPeak)
{
    int TLp = data[TLi][TLj];
    int TRp = data[TRi][TRj];
    int BLp = data[BLi][BLj];
    int BRp = data[BRi][BRj];

    //std::cout << " TL: [" << TLi << "," << TLj << "] TR: [" << TRi << "," << TRj << "] BL: [" << BLi << "," << BLj << "] BR: [" << BRi << "," << BRj << "]\n";

    float avgval = ((TLp+TRp+BLp+BRp)/4) - RandomPeak;

    if (avgval < 0)
    {
        avgval=0;
    }
    if (avgval > 1000)
    {
        avgval=1000;
    }

    int i = (TLi + BRi)/2;
    int j = (TLj + BRj)/2;

    //std::cout << "MiddleIDX: [" << i << "," << j << "]\n";

    data[i][j] = avgval;
}

void Box::SetEdge(std::vector< std::vector<int> > &data,int RandomPeak)
{
    int TLp = data[TLi][TLj];
    int TRp = data[TRi][TRj];
    int BLp = data[BLi][BLj];
    int BRp = data[BRi][BRj];

    int top = (TLp+TRp)/2 - RandomPeak;
    if (top<0)
    {
        top=0;
    }
    if (top>1000)
    {
        top=1000;
    }

    int bottom = (BLp+BRp)/2 - RandomPeak;
    if (bottom<0)
    {
        bottom=0;
    }
    if (bottom>1000)
    {
        bottom=1000;
    }

    int left = (TLp+BLp)/2 - RandomPeak;
    if (left<0)
    {
        left=0;
    }
    if (left>1000)
    {
        left=1000;
    }

    int right = (TRp+BRp)/2 - RandomPeak;
    if (right<0)
    {
        right=0;
    }
    if (right>1000)
    {
        right=1000;
    }

    int i = (TLi+TRi)/2;
    int j = TRj;

    //std::cout << "Top [" << i << "," << j << "] = " << top << std::endl;
    data[i][j] = top;

    i = (BLi+BRi)/2;
    j = BRj;

    //std::cout << "Bottom [" << i << "," << j << "] = " << bottom << std::endl;
    data[i][j] = bottom;

    i = TLi;
    j = (TLj+BLj)/2;

    //std::cout << "Left [" << i << "," << j << "] = " << left << std::endl;
    data[i][j] = left;

    i = BRi;
    j = (TRj+BRj)/2;

    //std::cout << "Right [" << i << "," << j << "] = " << right << std::endl;
    data[i][j] = right;
    //std::cout << "\n";
};

bool Box::ProduceNewIdx(std::vector<Box> &Boxes)
{
    int Idxj[9];
    int Idxi[9];

    //std::cout << "CHK IDX:\n";
    //Boxes[0].PrintBoxIdx();

    int Mj = (TLj + BLj)/2;
    Idxj[0] = TLj;
    Idxj[1] = TLj;
    Idxj[2] = TLj;
    Idxj[3] = Mj;
    Idxj[4] = Mj;
    Idxj[5] = Mj;
    Idxj[6] = BLj;
    Idxj[7] = BLj;
    Idxj[8] = BLj;

    int Mi = (TLi + BRi)/2;
    Idxi[0] = TLi;
    Idxi[1] = Mi;
    Idxi[2] = TRi;
    Idxi[3] = TLi;
    Idxi[4] = Mi;
    Idxi[5] = TRi;
    Idxi[6] = TLi;
    Idxi[7] = Mi;
    Idxi[8] = TRi;

    /*for (int i=0;i<3;++i)
    {
            for (int j=0;j<3;++j)
            {
                    std::cout << "(" << Idxi[i*3+j] << ","<< Idxj[i*3+j] << ") ";
            }
            std::cout << std::endl;
    }*/

    Box box1(Idxj[0],Idxi[0],Idxj[1],Idxi[1],Idxj[3],Idxi[3],Idxj[4],Idxi[4]);

    Box box2(Idxj[1],Idxi[1],Idxj[2],Idxi[2],Idxj[4],Idxi[4],Idxj[5],Idxi[5]);

    Box box3(Idxj[3],Idxi[3],Idxj[4],Idxi[4],Idxj[6],Idxi[6],Idxj[7],Idxi[7]);

    Box box4(Idxj[4],Idxi[4],Idxj[5],Idxi[5],Idxj[7],Idxi[7],Idxj[8],Idxi[8]);

    Boxes.push_back(box1);
    Boxes.push_back(box2);
    Boxes.push_back(box3);
    Boxes.push_back(box4);

    bool chkfin;
    int dist = abs(Idxi[0] - Idxi[1]);
    //std::cout << "Dist: " << dist << std::endl;
    if (dist <= 2)
    {
        chkfin=true;
    }
    else
    {
        chkfin=false;
    }

    return chkfin;
};

void Box::PrintBoxIdx()
{
    std::cout << " TL: [" << TLi << "," << TLj << "] TR: [" << TRi << "," << TRj << "] BL: [" << BLi << "," << BLj << "] BR: [" << BRi << "," << BRj << "]\n";
};
