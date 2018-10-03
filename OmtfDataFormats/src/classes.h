#include "DataFormats/Common/interface/Wrapper.h"
#include "UserCode/OmtfDataFormats/interface/EventData.h"
#include "UserCode/OmtfDataFormats/interface/EventObj.h"
#include "UserCode/OmtfDataFormats/interface/TrackObj.h"
#include "UserCode/OmtfDataFormats/interface/MuonObj.h"
#include "UserCode/OmtfDataFormats/interface/MuonObjColl.h"
#include "UserCode/OmtfDataFormats/interface/L1Obj.h"
#include "UserCode/OmtfDataFormats/interface/L1ObjColl.h"
#include "UserCode/OmtfDataFormats/interface/TriggerMenuResultObj.h"
#include "UserCode/OmtfDataFormats/interface/SynchroCountsObj.h"
#include "UserCode/OmtfDataFormats/interface/SynchroCountsObjVect.h"
#include "UserCode/OmtfDataFormats/interface/GenObj.h"
#include "UserCode/OmtfDataFormats/interface/GenObjColl.h"


namespace {
  struct dictionary {
    EventData aEvData;
    EventObj aEvt;
    TrackObj aTrack; 
    MuonObj  aMuon;
    MuonObjColl  aMuons;
    L1Obj aL1;
    L1ObjColl aL1Coll;
    TriggerMenuResultObj aMenuRes;
    SynchroCountsObj aSynch;
    SynchroCountsObjVect aSynchVec;
    GenObj aGen;
    GenObjColl aGenColl;
  };
}
