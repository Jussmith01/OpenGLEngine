#ifndef RANDOMGEN_C
#define RANDOMGEN_C

#include "../../../Headers/headerscpp.h"
#include "../../../Headers/headersogl.h"
#include <time.h>
#include <random>

//Included Shader Program
//#include "../EasyBMP_1.06/EasyBMP.h"

class RandomInteger
{
        std::default_random_engine generator;
        std::vector<int> array;
        int index;

        public:
        RandomInteger(){};
        RandomInteger(int w,int i){Setup(w,i);};

        void Setup(int w,int i)
        {
                time_t Time;
                time(&Time);
                int seedOffset=(int)Time;

                array.resize(w);

                int t = (int)glfwGetTime()+i;
                std::seed_seq seed = {seedOffset,t,i+100};
                seed.generate(array.begin(),array.end());//Seed the generator
                index = 0;
        };

        int GenRandInt(int low,int high)
        {
                generator.seed(array[index]);//Seed the generator
                std::uniform_int_distribution<int> distribution(low,high);//Setup the distribution
                int RN = (int)distribution(generator);//Denerate the random number
                //std::cout << "RandomNumber: " << RN << std::endl;
                ++index;//Increase seed offset
                return RN;
        };
};

class RandomRealVal
{
        std::default_random_engine generator;
        std::vector<int> array;
        int index;

        public:
        RandomRealVal(){};

        void Setup(int w,int i)
        {
                time_t Time;
                time(&Time);
                int seedOffset=(int)Time;

                array.resize(w);

                int t = (int)glfwGetTime()+i;
                std::seed_seq seed = {seedOffset,t,i+100};
                seed.generate(array.begin(),array.end());//Seed the generator
                index = 0;
        };

        int GenRandReal(int high,int low)
        {
                generator.seed(array[index]);//Seed the generator
                std::uniform_real_distribution<float> distribution(low,high);//Setup the distribution
                float RN = (float)distribution(generator);//Denerate the random number
                //std::cout << "RandomNumber: " << RN << std::endl;
                ++index;//Increase seed offset
                return RN;
        };
};

#endif
