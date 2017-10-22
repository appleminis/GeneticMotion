#include "geneticdata.h"



GeneticLowlevelGrammar::GeneticLowlevelGrammar()
{
}

void GeneticLowlevelGrammar::initLLcmds ()
{
    cmdnm.push_back("cmdend   ");
    cmdsz.push_back(0);
    cmdnm.push_back("cmdifmv  ");
    cmdsz.push_back(2);

  //cmdnm.push_back("cmdfork  ");


}

int GeneticLowlevelGrammar::TranslateLLCode(GeneticBase *gb, int idcmd, int idcell)
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
