#include <bci-interface/Utils/square-function.h>

#include <stdlib.h>
#include <iostream>

using namespace bciinterface;

int main(int argc, char * argv[])
{
    int frequency = 1;
    int screenFrequency = 60;
    if(argc > 2)
    {
        frequency = atoi(argv[1]);
        screenFrequency = atoi(argv[2]);
    }
    std::vector< std::pair<int,int> > seq;
    if(squarefunction(frequency, screenFrequency, seq))
    {
        std::cout << " [ ";
        for(unsigned int i = 0; i < seq.size(); ++i)
        {
            std::cout << " < " << seq[i].first << " , " << seq[i].second << " > ";
        }
        std::cout << " ] " << std::endl;
    }
    return 0;
}
