#include "DataFormats/Common/interface/Wrapper.h"
#include "UserCode/OmtfDataFormats/interface/EventObj.h"
#include "UserCode/OmtfDataFormats/interface/EventData.h"
#include "UserCode/OmtfDataFormats/interface/L1Obj.h"
#include "UserCode/OmtfDataFormats/interface/L1ObjColl.h"


namespace {
  struct dictionary {
    EventObj aEvt;
    L1Obj aObj;
    L1ObjColl aObjColl;
    EventData eventData;

  };
}
