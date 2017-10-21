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

    GeneticData();

};

class GeneticLowlevelGrammar
{
public :

    GeneticLowlevelGrammar()
    {
    }

    int TranslateLLCode(GeneticBase *gb, int idcmd)
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
                if (idcmd<*(gb->data+1))
                    return idcmd+*(gb->data+2);
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
