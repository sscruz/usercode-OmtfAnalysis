#include "DataFormats/Common/interface/Wrapper.h"
#include "UserCode/OmtfDataFormats/interface/EventData.h"
#include "UserCode/OmtfDataFormats/interface/EventObj.h"
#include "UserCode/OmtfDataFormats/interface/TrackObj.h"
#include "UserCode/OmtfDataFormats/interface/MuonObj.h"
#include "UserCode/OmtfDataFormats/interface/L1Obj.h"
#include "UserCode/OmtfDataFormats/interface/L1ObjColl.h"
#include "UserCode/OmtfDataFormats/interface/TriggerMenuResultObj.h"


namespace {
  struct dictionary {
    EventData aEvData;
    EventObj aEvt;
    TrackObj aTrack; 
    MuonObj  aMuon;
    L1Obj aL1;
    L1ObjColl aL1Coll;
    TriggerMenuResultObj aMenuRes;
  };
}
