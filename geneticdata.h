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
    GeneticData() {}
};

class GeneticLowlevelGrammar
{
protected:
    vector<char*> cmdnm;
    vector<int> cmdsz;
public :


    GeneticLowlevelGrammar();

    void initLLcmds ();
    int TranslateLLCode(GeneticBase *gb, int idcmd, int idcell);
};

#endif // GENETICDATA_H
