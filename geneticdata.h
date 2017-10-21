#ifndef GENETICDATA_H
#define GENETICDATA_H

#include <vector>
#include <deque>
#include <stdint.h>
#include <stddef.h>
using namespace std;

#define NCMD 5

class GeneticBase
{
public:
    int8_t  *data;
    uint8_t size;

    GeneticBase() {data=NULL;}
    GeneticBase(int8_t *_data, uint8_t _size) : data(_data),size(_size) {}
};

class GeneticData
{
public:

    deque<GeneticBase*> data;

    GeneticData()
    {

    }

};

class GeneticLowlevelGrammar
{
protected:
    vector<char*> cmdnm;
    vector<int> cmdsz;
public :


    GeneticLowlevelGrammar()
    {
    }

    void initLLcmdsz ()
    {
        cmdnm.push_back("cmdend   ");
        cmdsz.push_back(0);
        cmdnm.push_back("cmdifmv  ");
        cmdsz.push_back(2);

    }

    int TranslateLLCode(GeneticBase *gb, int idcmd, int idcell)
    {
        switch (gb->data[0])
        {
            case 0://end transcription
            {
                return 256;
            }
            break;

            case 1://test and branch instruction
            {
                if (idcell<*(gb->data+1))
                    return *(gb->data+2);
                else
                    return 1;
            }
            break;

            default: //high level command
                return 1;
            break;
        }

    }

};

#endif // GENETICDATA_H
