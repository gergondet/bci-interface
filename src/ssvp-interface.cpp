#include <iostream>
#include <ssvp-interface/square-function.h>

int main(int argc, char * argv[])
{
    std::vector< std::pair<int,int> > seq;
    squarefunction(45, 240, seq);
    std::cout << "[ ";
    for(int i = 0; i < seq.size(); ++i)
    {
        if(i < seq.size() - 1)
            std::cout << seq[i].first << " , " << seq[i].second << " , ";
        else
            std::cout << seq[i].first << " , " << seq[i].second;
    }
    std::cout << " ] " << std::endl;
    return 0;
}
