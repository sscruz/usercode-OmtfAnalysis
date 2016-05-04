#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/L1RpcTriggerAnalysis/interface/EventData.h"
#include "DataFormats/L1RpcTriggerAnalysis/interface/L1Obj.h"
#include "DataFormats/L1RpcTriggerAnalysis/interface/L1ObjColl.h"


namespace {
  struct dictionary {

    L1Obj aObj;
    L1ObjColl aObjColl;
    EventData eventData;

  };
}
