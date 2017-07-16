#ifndef SynchroCountsObjVect_H
#define SynchroCountsObjVect_H

#include "TObject.h"
#include "UserCode/OmtfDataFormats/interface/SynchroCountsObj.h"

#include <vector>

struct SynchroCountsObjVect : public TObject {
  std::vector< SynchroCountsObj > data;
ClassDef(SynchroCountsObjVect,1)
};


#endif
