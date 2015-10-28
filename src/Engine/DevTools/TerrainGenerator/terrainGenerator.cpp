#include "terrainGenerator.h"
#include <omp.h>
#include <math.h>

//******************************************//
//             Allocate Data                //
//******************************************//
/*
Allocate height data.
*/
void TerrainGeneration::AllocateData(int size)
{
    int h = size;
    int w = size;

    long long int memSize = h*w*sizeof(int);
    Console::cPrint(tools::appendStrings("Required Memory for Height Data: ",memSize/(float)(1024*1024),"MB"));

    if(!HeightData.empty())
    {
        for (int i=0; i<(int)HeightData.size(); ++i)
        {
            HeightData[i].clear();
        }
        HeightData.clear();
    }

    HeightData.resize(h);

    for (int i=0; i<h; ++i)
    {
        HeightData[i].resize(w);
    }
};

//******************************************//
//          Generate Height Data            //
//******************************************//
/*
Generate the height data based on the mid-point formula.
*/
void TerrainGeneration::GenerateTerrainData(int terrainSize)
{

    int h = terrainSize;
    int w = h;

    AllocateData(terrainSize);

    //Choose 4 random pixel colors to initialize
    RandomInteger RandC(4,clock());

    //HeightData[  0  ][  0  ] = RandC.GenRandInt(50,100);
    //HeightData[  0  ][ w-1 ] = RandC.GenRandInt(50,100);
    //HeightData[ h-1 ][  0  ] = RandC.GenRandInt(50,100);
    //HeightData[ h-1 ][ w-1 ] = RandC.GenRandInt(50,100);

    // Max is 1000, Min is 0
    HeightData[  0  ][  0  ] = heightVariation.x;
    HeightData[  0  ][ w-1 ] = heightVariation.x;
    HeightData[ h-1 ][  0  ] = heightVariation.x;
    HeightData[ h-1 ][ w-1 ] = heightVariation.x;

    //std::cout << "Beginning Fra\n";
    Console::cPrint("Computing Height Data...");
    //std::cout << "[" << HeightData[0][0] << "," << HeightData[0][w-1] << "]" << std::endl;
    //std::cout << "[" << HeightData[h-1][0] << "," << HeightData[h-1][w-1] << "]" << std::endl;
    std::vector<Box> Boxes;

    Box box1(0,0,0,w-1,h-1,0,h-1,w-1);
    Boxes.push_back(box1);

    bool chkfin=false;
    int cycle=0;
    //RandomInt RI(pow(2,16),100+cycle);

    //****************************************************************
    //Calculate Initial Height map via midpoint displacement algorithm
    //****************************************************************
    while (!chkfin)
    {
        ++cycle;
        Console::cPrint(tools::appendStrings(" Cycle: ",cycle));
        int N = Boxes.size();
        //RandomInt RI(N,100+cycle);

        int hvlow=heightVariation.y;
        int hvhigh=heightVariation.z;

        RandomInteger RI(6*N,clock());
        for (int k = 0; k<(int)N; ++k)
        {
            //std::cout << "   Box: " << k << std::endl;
            int RandomMPeak = RI.GenRandInt(-hvlow/cycle,hvhigh/cycle);
            int RandomEPeak = RI.GenRandInt(-hvlow/cycle,hvhigh/cycle);
            //int RandomPeak = 0;
            Boxes[k].SetMiddle(HeightData,RandomMPeak);
            Boxes[k].SetEdge(HeightData,RandomEPeak);
        }

        //Boxes[0].PrintBoxIdx();
        std::vector<Box> cpBoxes = Boxes;
        //cpBoxes[0].PrintBoxIdx();
        Boxes.clear();

        for  (int l = 0; l<(int)cpBoxes.size(); ++l)
        {
            chkfin = cpBoxes[l].ProduceNewIdx(Boxes);
            //std::cout << "chkfin: " << chkfin << std::endl;
        }

        //std::cout << "NUMBOXES: " << Boxes.size() << std::endl;
    }

    for (int k = 0; k<(int)Boxes.size(); ++k)
    {
        Boxes[k].SetMiddle(HeightData,0);
        Boxes[k].SetEdge(HeightData,0);
    }

    /*for (int i=0;i<terrainSize;++i)
    {
        std::cout << "[";
        for (int j=0;j<terrainSize;++j)
        {
            std::cout << HeightData[i][j] << ",";
        }
        std::cout << "]" << std::endl;
    }*/

    //******************************
    //Blur image For Smoothness
    //******************************
    Console::cPrint("Running Height Map Blur Cycles...");
    //std::cout << "Running Height Map Blur Cycles..." << "\n";
    for (int l=0; l<NSmooth; ++l)
    {
        std::vector< std::vector<int> > TempData = HeightData;
        //std::cout << "   Cycle: " << l << "\n";
        Console::cPrint(tools::appendStrings("   Cycle: ",l));

        #pragma omp parallel for
        for (int j = 0; j<h; ++j)
        {
            for (int i = 0; i<w; ++i)
            {
                int NewVal = AverageHeights(i,j,TempData,terrainSize);

                HeightData[i][j] = NewVal;
            }
        }
        #pragma omp barrier
    }
};

//******************************************//
//          Average the heights            //
//******************************************//
/*
Average the heights, this helps smooth the terrain
after generation.
*/
int TerrainGeneration::AverageHeights(int i,int j, std::vector< std::vector<int> > &data,int size)
{
    int h = size;
    int w = h;

    int im1 = i-1;
    int ip1 = i+1;
    int jm1 = j-1;
    int jp1 = j+1;

    //Periodic Conditions
    if (im1 < 0)
    {
        im1=w-1;
    }

    if (ip1 > w-1)
    {
        ip1=0;
    }

    if (jm1 < 0)
    {
        jm1=h-1;
    }

    if (jp1 > h-1)
    {
        jp1=0;
    }

    std::vector<int> NPix;

    NPix.resize(8);

    NPix[0] = data[im1][jm1];
    NPix[1] = data[i][jm1];
    NPix[2] = data[ip1][jm1];
    NPix[3] = data[im1][j];
    NPix[4] = data[ip1][j];
    NPix[5] = data[im1][jp1];
    NPix[6] = data[i][jp1];
    NPix[7] = data[ip1][jp1];

    //Calculate Average
    int SUM = 0;
    for (int k=0; k<8; ++k)
    {
        SUM += NPix[k];
    }

    int AVG = SUM/8;

    return AVG;
};

//*********************************************
//              Setup Verticies
//*********************************************
/*
Setup the verticies/tex coords based on size and
data stored in height data.
*/
void TerrainGeneration::SetupVerts()
{
    int h = terrainSize;
    int w = terrainSize;
    int sd = pow(4,subdiv);

    Console::cPrint("Allocating Verts Memory...");
    verts.resize(w*h);

    Console::cPrint("Determining Max/Min Heights...");
    RecalculateMaxMinHeights();

    Console::cPrint("Calculating Verts...");
    RecalculateVerticies();
    //HeightData.clear(); // Done with height data

    Console::cPrint("Calculating Normals...");
    RecalculateNormals();

    Console::cPrint("Calculating Indicies...");

    if (!AccessIdxs().empty())
    {
        AccessIdxs().clear();
    }

    if (!AccessPositions().empty())
    {
        AccessPositions().clear();
    }

    AccessPositions().resize(sd);

    int N=(h/sqrt(sd))+1; // Length of each mesh
    Console::cPrint(tools::appendStrings(" Width of Mesh: ",N));

    SetupMeshSubInfo(sqrt(sd),N,sizeScale);

    long long int memreq=(N-1)*(N-1)*6;
    Console::cPrint(tools::appendStrings(" Idx. Mem. Req.: ",(sd*memreq)/(1024*1024),"MB"));

    AccessIdxs().resize(sd);
    for (int i=0; i<sd; ++i)
    {
        AccessIdxs()[i].resize(memreq);
    }

    Console::cPrint(" Allocating Mesh Verts Memory...");
    AccessMeshVerts().resize(sd);
    for (int i=0; i<sd; ++i)
    {
        AccessMeshVerts()[i].resize(N*N);
    }

    Console::cPrint(" Indexing Mesh Verts...");

    #pragma omp parallel for firstprivate(sd,N)
    for (int m=0; m<(int)sqrt(sd); ++m)
    {
        for (int n=0; n<(int)sqrt(sd); ++n)
        {
            int sdIdx=n+m*(int)sqrt(sd);
            //std::cout << "SubDivisions: m: " << m << " n: " << n << " IDX: " << sdIdx << std::endl;

            long int it=0;
            // Index the verts for each new mesh
            for (int i=0; i<(N-1); ++i)
            {
                for (int j=0; j<(N-1); ++j)
                {
                    int idx1 = j+(i)*N;
                    int idx2 = (j+(i)*N)+1;
                    int idx3 = j+(i+1)*N;
                    int idx4 = (j+1)+(i+1)*N;

                    //std::cout << " QUAD: {" << idx1 << "," << idx3 << "," << idx2 << "}"
                    //                << ",{" << idx2 << "," << idx3 << "," << idx4 << "}\n";

                    AccessIdxs()[sdIdx][it+0]=idx1;
                    AccessIdxs()[sdIdx][it+1]=idx3;
                    AccessIdxs()[sdIdx][it+2]=idx2;

                    AccessIdxs()[sdIdx][it+3]=idx2;
                    AccessIdxs()[sdIdx][it+4]=idx3;
                    AccessIdxs()[sdIdx][it+5]=idx4;

                    it+=6;
                }
            }

            // Store the verts based on the index
            for (int i=0; i<N; ++i)
            {
                for (int j=0; j<N; ++j)
                {
                    int Nidx=j+(i)*N;
                    int ic=i+(N-1)*m;
                    int jc=j+(N-1)*n;
                    int idx1=jc+ic*h;
                    //std::cout << " VERT: {" << idx1 << ""<< Nidx << "}\n";
                    AccessMeshVerts()[sdIdx][Nidx]=verts[idx1];
                }
            }

            // Compute the center of each new mesh
            double fw=(w-1)*sizeScale; // full width of terrain
            double dx=fw/(double)sqrt(sd); // Width of a single mesh

            double x=n*dx+dx/2.0-(fw/2.0);
            double y=m*dx+dx/2.0-(fw/2.0);

            AccessPositions()[sdIdx]=glm::vec2(x,y);

            //std::cout << "DISTANCE(" << sdIdx << "): {" << positions.back().x << "," << positions.back().y << "}" << std::endl;
        }
        //std::cout << "\n";
    }
    #pragma omp barrier

    double fw=(w-1)*sizeScale; // full width of terrain
    double dx=fw/(double)sqrt(sd); // Width of a single mesh
    meshwidth = dx/2.0;

    //verts.clear(); // Done with normal verts
    Console::cPrint(" Terrain Successfully Setup!",true);
};

//*********************************************
//            Recalculate Normals
//*********************************************
/*
Recalculate the normals based on new vertex positions.

Begins by calculating vectors from each center
vertex to each nearest neighbor, then calculating
the cross product of each of these vectors. Averaging
these vectors gives a "smoothed" normal based on the
slope.
*/
void TerrainGeneration::RecalculateNormals()
{
    int h = terrainSize;
    int w = terrainSize;

    //std::cout << "Beginning Normals...\n";
    //Console::cPrint(" Calculating Normals...");

    #pragma omp parallel for shared(h,w)
    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            //std::cout << "BEGINNING NORMAL (" << i << "," << j << ")\n";
            int im1=i-1;
            int jm1=j-1;
            int ip1=i+1;
            int jp1=j+1;

            glm::vec3 dP1;
            if (!(im1<0))
            {
                dP1 = verts[j+im1*w].position - verts[j+i*w].position;
            }

            glm::vec3 dP2;
            if (!(im1<0) && !(jm1<0))
            {
                dP2 = verts[jm1+im1*w].position - verts[j+i*w].position;
            }

            glm::vec3 dP3;
            if (!(jm1<0))
            {
                dP3 = verts[jm1+i*w].position - verts[j+i*w].position;
            }

            glm::vec3 dP4;
            if (!(jm1<0) && !(ip1>=h))
            {
                dP4 = verts[jm1+ip1*w].position - verts[j+i*w].position;
            }

            glm::vec3 dP5;
            if (!(ip1>=h))
            {
                dP5 = verts[j+ip1*w].position - verts[j+i*w].position;
            }

            glm::vec3 dP6;
            if (!(jp1>=w) && !(ip1>=h))
            {
                dP6 = verts[jp1+ip1*w].position - verts[j+i*w].position;
            }

            glm::vec3 dP7;
            if (!(jp1>=w))
            {
                dP7 = verts[jp1+i*w].position - verts[j+i*w].position;
            }

            glm::vec3 dP8;
            if (!(jp1>=w) && !(im1<0))
            {
                dP8 = verts[jp1+im1*w].position - verts[j+i*w].position;
            }

            glm::vec3 cP1 = -glm::normalize(glm::cross(dP3,dP1));
            glm::vec3 cP2 = -glm::normalize(glm::cross(dP4,dP2));
            glm::vec3 cP3 = -glm::normalize(glm::cross(dP5,dP3));
            glm::vec3 cP4 = -glm::normalize(glm::cross(dP6,dP4));
            glm::vec3 cP5 = -glm::normalize(glm::cross(dP7,dP5));
            glm::vec3 cP6 = -glm::normalize(glm::cross(dP8,dP6));
            glm::vec3 cP7 = -glm::normalize(glm::cross(dP1,dP7));
            glm::vec3 cP8 = -glm::normalize(glm::cross(dP2,dP8));

            if (i-1 < 0)
            {
                cP1=glm::vec3(0.0f);
                cP2=glm::vec3(0.0f);
                cP6=glm::vec3(0.0f);
                cP7=glm::vec3(0.0f);
                cP8=glm::vec3(0.0f);
            }

            if (j-1 < 0)
            {
                cP1=glm::vec3(0.0f);
                cP2=glm::vec3(0.0f);
                cP3=glm::vec3(0.0f);
                cP4=glm::vec3(0.0f);
                cP8=glm::vec3(0.0f);
            }

            if (j+1 >= w)
            {
                cP4=glm::vec3(0.0f);
                cP5=glm::vec3(0.0f);
                cP6=glm::vec3(0.0f);
                cP7=glm::vec3(0.0f);
                cP8=glm::vec3(0.0f);
            }

            if (i+1 >= h)
            {
                cP2=glm::vec3(0.0f);
                cP3=glm::vec3(0.0f);
                cP4=glm::vec3(0.0f);
                cP5=glm::vec3(0.0f);
                cP6=glm::vec3(0.0f);
            }

            glm::vec3 Normal=glm::normalize(cP1+cP2+cP3+cP4+cP5+cP6+cP7+cP8);

            verts[j+i*w].normal=Normal;
            //std::cout << "NORMAL (" << i << "," << j << "): [" << verts[j+i*w].normal.x << ","<< verts[j+i*w].normal.y << "," <<  verts[j+i*w].normal.z << "]" << std::endl;
        }
    }
    #pragma omp barrier
};

//*********************************************
//            Recalculate Verticies
//*********************************************
/*
Recalculate Verticies based on new heightdata
*/
void TerrainGeneration::RecalculateVerticies()
{
    int h = terrainSize;
    int w = terrainSize;

    // Calculate the shift of the mesh (Used to shift 0,0 to center)
    float shift=(float)sizeScale*(h-1)/2.0f;

    float midpoint=(lowShift+GetRelHeight().x)/2.0f;

    AccessRelHeight().x=heightMult*(GetRelHeight().x-midpoint);

    float sScale = sizeScale;
    float hMult = heightMult;

    #pragma omp parallel for firstprivate(h,w,sScale,shift,midpoint,hMult)
    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            float Height = HeightData[i][j];

            verts[j+i*w].position.x = j*sScale-shift;
            verts[j+i*w].position.y = (float)hMult*(Height-midpoint);
            verts[j+i*w].position.z = i*sScale-shift;

            verts[j+i*w].texture.x = (float)j;
            verts[j+i*w].texture.y = (float)i;
        }
    }
    #pragma omp barrier
};

//*********************************************
//         Calculate Height Shifts
//*********************************************
/*
Calculate the maximum and minimum heights
*/
void TerrainGeneration::RecalculateMaxMinHeights()
{
    int h = terrainSize;
    int w = terrainSize;

    AccessRelHeight().x=0.0f;
    lowShift=1.0E30;

    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            float Height = HeightData[i][j];

            if (Height>GetRelHeight().x)
            {
                AccessRelHeight().x=Height;
                highShift=Height;
            }

            if (Height<lowShift)
            {
                lowShift=Height;
            }
        }
    }
};

//*********************************************
//        Setup Terrain Creation Parameters
//*********************************************
/*
Setup Terrain Creation Parameters
1) terrainSize
2) heightVariation
*/
void TerrainGeneration::SetupTerrainCreationParameters(int terrainSize,int NSmooth,glm::ivec3 heightVariation)
{
    this->terrainSize=terrainSize;
    this->NSmooth=NSmooth;
    this->heightVariation=heightVariation;
};

//*********************************************
//        Setup Terrain Modify Parameters
//*********************************************
/*
Setup Terrain Parameters
1) heightMult
2) relativeHeight
*/
void TerrainGeneration::SetupTerrainModifyParameters(float heightMult,glm::vec3 relheight)
{
    this->heightMult=heightMult;
    AccessRelHeight() = glm::vec4(GetRelHeight().x,relheight.x,relheight.y,relheight.z);
};

//*********************************************
//          Raise/Lower Terrain
//*********************************************
/*
Set Relative Height Data on GPU.
*/
void TerrainGeneration::modifyElevation(int func,float eff,InputStruct &input,RTSCamera &camera)
{
    double x,y;
    input.ReturnMousePos(x,y); // Get mouse screen position

    generalTimer.start_point(); // Start the timer *TESTING*

    double sphererad = sqrt(meshwidth*meshwidth+meshwidth*meshwidth); // Radius of the sphere

    // Initialize variables to be set
    int mesh=-1;
    int poly=-1;
    glm::vec3 baryPos(1.0E7);
    glm::vec3 avgPos;
    bool found=false;

    // Search all meshes for click
    for(int m=0; m<int(AccessIdxs().size()); ++m)
    {
        if (!found)
        {
            // Declare thread variables for private use
            //glm::vec3 dir = camera.cameraDir;
            glm::vec3 pos = camera.cameraPos;
            glm::vec3 ray = camera.Cursor3DRay;

            glm::vec3 spherecent = glm::vec3(AccessPositions()[m].x,0.0f,AccessPositions()[m].y);

            // Determine if click occurs within mesh sphere
            if (glmtools::DetermineSphereIntersection(pos,ray,spherecent,sphererad))
            {
                // If click occurs within mesh sphere, search mesh for clicked triangle
                #pragma omp parallel for default(shared) firstprivate(m,pos,ray)
                for(int i=0; i<int(AccessIdxs()[m].size())/3; ++i)
                {
                    int idx1 = AccessIdxs()[m][i*3];
                    int idx2 = AccessIdxs()[m][i*3+1];
                    int idx3 = AccessIdxs()[m][i*3+2];

                    glm::vec3 n1 = AccessMeshVerts()[m][idx1].normal;
                    glm::vec3 n2 = AccessMeshVerts()[m][idx2].normal;
                    glm::vec3 n3 = AccessMeshVerts()[m][idx3].normal;

                    glm::vec3 na = glm::normalize(n1+n2+n3);

                    // Determine if the polygon is facing the ray
                    if (glm::dot(-glm::normalize(ray),na) > 0)
                    {
                        glm::vec3 v1 = AccessMeshVerts()[m][idx1].position;
                        glm::vec3 v2 = AccessMeshVerts()[m][idx2].position;
                        glm::vec3 v3 = AccessMeshVerts()[m][idx3].position;

                        glm::vec3 baryPostmp;

                        // Determine if searched triangle intersects click ray
                        if (glmtools::DetermineTriangleIntersection(pos,ray,v1,v2,v3,baryPostmp))
                        {
                            #pragma omp critical
                            {
                                if (baryPostmp.z < baryPos.z)
                                {
                                    baryPos=baryPostmp;
                                    mesh=m;
                                    poly=i;

                                    avgPos=v1+v2+v3;
                                    avgPos*=0.3333333;
                                    found=true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Modify the height data
    if (found)
    {
        switch(func)
        {
            case 0: {ModifyHeightData(avgPos,0,eff);break;}
            case 1: {ModifyHeightData(avgPos,1,eff);break;}
            case 2: {LevelHeightData(avgPos,eff);break;}
        }

        // Recalculate the data based on height changes
        RecalculateData();
        //****************
        // TESTING THINGSsds
        //****************
        Console::cPrint(tools::appendStrings("Intersect Mesh(",mesh,") Polygon(",poly,")"));
        Console::cPrint(tools::appendStrings("Bary Position [",baryPos.x,",",baryPos.y,",",baryPos.z,"]"));
    } else {
        Console::cPrint("No intersection detected!");
    }


    generalTimer.end_point(); // End the *TESTING* timer
    Console::cPrint(generalTimer.get_generic_print_string("modifyElevation "));
};

//*********************************************
//            Modify Height Data
//*********************************************
/*

*/
void TerrainGeneration::ModifyHeightData(glm::vec3 point,int updown,float eff)
{
    int h = terrainSize;
    int w = terrainSize;

    float hmult=5.0f;

    float lShift=lowShift;
    float hShift=highShift;

    #pragma omp parallel for firstprivate(h,w,point,hmult,eff,lShift,hShift)
    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            float R = glm::length(verts[j+i*w].position-point);
            if (R < 50.0)
            {
                float nM = hmult/(pow(R/4.0,eff)+1.0);
                float nY = 0.0;

                switch (updown)
                {
                    case 0: {nY = HeightData[i][j] - nM;break;}
                    case 1: {nY = HeightData[i][j] + nM;break;}
                }
                //std::cout << "*TEST* Length: " << R << " nM: " << nM << " nY: " << nY << " nYo: " << HeightData[i][j] << std::endl;

                if (!(std::round(nY)>hShift) && !(std::round(nY)<lShift))
                {
                    //std::cout << "*TEST* Length: " << R << " nM: " << nM << " nY: " << nY << " nYcorr: " << std::round(nY) << " nYo: " << HeightData[i][j] << "\n";
                    HeightData[i][j] = std::round(nY);
                }
            }
        }
        //std::cout << " "<< std::endl;
    }
    //std::cout << "|-------------------------------|"<< std::endl;
    #pragma omp barrier
};

//*********************************************
//            Modify Height Data
//*********************************************
/*

*/
void TerrainGeneration::LevelHeightData(glm::vec3 point,float eff)
{
    int h = terrainSize;
    int w = terrainSize;

    //float hmult=5.0f;

    //float lShift=lowShift;
    //float hShift=highShift;

    std::vector< std::pair<int,int> > idx;
    std::vector< double > dists;
    std::pair< int,int > avgh;

    avgh.first=0.0;
    avgh.second=0;

    //#pragma omp parallel for firstprivate(h,w,point,hmult,eff)
    for (int i=0; i<h; ++i)
    {
        for (int j=0; j<w; ++j)
        {
            glm::vec3 v1=verts[j+i*w].position;
            float R = glm::length(v1-point);
            if (R < 100.0)
            {
                avgh.first+=HeightData[i][j]; // Add heights
                ++avgh.second; // Count heights

                std::pair<int,int> wkidx; // Build element
                wkidx.first=i; // Element index
                wkidx.second=j; // Element index

                idx.push_back(wkidx); // Save pair

                dists.push_back(R);
            }
        }
        //std::cout << " "<< std::endl;
    }

    int avgHeight = std::round(avgh.first/(float)avgh.second);
    for (auto&& el : idx)
    {
        //std::cout << "Working!!\n";
        int cY = HeightData[el.first][el.second];
        float scale=(avgHeight-cY)*0.05;

        HeightData[el.first][el.second]=std::round(cY+scale);
    }

    //std::cout << "|-------------------------------|"<< std::endl;
    //#pragma omp barrier
};
