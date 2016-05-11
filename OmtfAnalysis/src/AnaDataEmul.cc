#include "UserCode/OmtfAnalysis/interface/AnaDataEmul.h"

#include <cmath>
#include <bitset>

#include "TObjArray.h"
#include "TH1D.h"
#include "TAxis.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "UserCode/OmtfDataFormats/interface/L1ObjColl.h"

namespace {
 TH1D *hDataEmulCompareAll, *hDataEmulCompareRpc, *hDataEmulCompareRpcCsc, *hDataEmulCompareCsc ;
}

AnaDataEmul::AnaDataEmul(const edm::ParameterSet& cfg)
{
}

void AnaDataEmul::init(TObjArray& histos)
{
  hDataEmulCompareAll = new TH1D("hDataEmulCompareAll","hDataEmulCompareAll",5, 0.5,5.5); histos.Add(hDataEmulCompareAll);
  hDataEmulCompareAll->GetXaxis()->SetBinLabel(1,"Agree");
  hDataEmulCompareAll->GetXaxis()->SetBinLabel(2,"Almost Agree");
  hDataEmulCompareAll->GetXaxis()->SetBinLabel(3,"Not Agree");
  hDataEmulCompareAll->GetXaxis()->SetBinLabel(4,"Data Only");
  hDataEmulCompareAll->GetXaxis()->SetBinLabel(5,"Emul Only");
  hDataEmulCompareRpc = new TH1D("hDataEmulCompareRpc","hDataEmulCompareRpc",5, 0.5,5.5); histos.Add(hDataEmulCompareRpc);
  hDataEmulCompareRpc->GetXaxis()->SetBinLabel(1,"Agree");
  hDataEmulCompareRpc->GetXaxis()->SetBinLabel(2,"Almost Agree");
  hDataEmulCompareRpc->GetXaxis()->SetBinLabel(3,"Not Agree");
  hDataEmulCompareRpc->GetXaxis()->SetBinLabel(4,"Data Only");
  hDataEmulCompareRpc->GetXaxis()->SetBinLabel(5,"Emul Only");
  hDataEmulCompareRpcCsc = new TH1D("hDataEmulCompareRpcCsc","hDataEmulCompareRpcCsc",5, 0.5,5.5); histos.Add(hDataEmulCompareRpcCsc);
  hDataEmulCompareRpcCsc->GetXaxis()->SetBinLabel(1,"Agree");
  hDataEmulCompareRpcCsc->GetXaxis()->SetBinLabel(2,"Almost Agree");
  hDataEmulCompareRpcCsc->GetXaxis()->SetBinLabel(3,"Not Agree");
  hDataEmulCompareRpcCsc->GetXaxis()->SetBinLabel(4,"Data Only");
  hDataEmulCompareRpcCsc->GetXaxis()->SetBinLabel(5,"Emul Only");
  hDataEmulCompareCsc = new TH1D("hDataEmulCompareCsc","hDataEmulCompareCsc",5, 0.5,5.5); histos.Add(hDataEmulCompareCsc);
  hDataEmulCompareCsc->GetXaxis()->SetBinLabel(1,"Agree");
  hDataEmulCompareCsc->GetXaxis()->SetBinLabel(2,"Almost Agree");
  hDataEmulCompareCsc->GetXaxis()->SetBinLabel(3,"Not Agree");
  hDataEmulCompareCsc->GetXaxis()->SetBinLabel(4,"Data Only");
  hDataEmulCompareCsc->GetXaxis()->SetBinLabel(5,"Emul Only");
  
}

void AnaDataEmul::run(L1ObjColl * coll)
{
  if ( !*coll ) return;

  const L1Obj * data = 0;
  L1ObjColl dataColl =  coll->selectByType(L1Obj::OMTF);
  if (dataColl.getL1Objs().size()==1) data = &(dataColl.getL1Objs().front());
  if (data && abs(data->eta)==115) return;

  const L1Obj * emul = 0;
  L1ObjColl emulColl =  coll->selectByType(L1Obj::OMTF_emu);
  if (emulColl.getL1Objs().size()==1) emul = &(emulColl.getL1Objs().front());
  if (emulColl.getL1Objs().size() > 1) return;

  unsigned int hits = 0;
  if (data || emul) {
    unsigned int hitsEmul = emul ? emul->hits >>1 : 0;
    unsigned int hitsData = data ? data->hits >>2 : 0;
    hits = (hitsEmul | hitsData) << 1; 
  }
  
  DIFF diff = compare(data, emul);   
  hDataEmulCompareAll->Fill(diff);

  bool  dt = hasDtHits(hits);
  bool csc = hasCscHits(hits);
  bool rpc = hasRpcHits(hits);
  if (rpc && !csc && !dt) hDataEmulCompareRpc->Fill(diff);
  if (rpc && csc && !dt) hDataEmulCompareRpcCsc->Fill(diff);
  if (!rpc && csc && !dt) hDataEmulCompareCsc->Fill(diff);
}

AnaDataEmul::DIFF AnaDataEmul::compare(const L1Obj * data, const L1Obj * emul)
{

  DIFF diff = unknown;

  if (!data && !emul) return diff;

  if (data && !emul) diff = dataOnly;
  else if (!data && emul) diff = emulOnly;
  else {
    unsigned int hitsEmul = emul->hits >>1;
    unsigned int hitsData = data->hits >>2;
    if (    (hitsEmul == hitsData) 
            && (data->pt == emul->pt) 
            && (data->phi == emul->phi) ) diff = agree; 
    else if (hitsEmul == hitsData)  diff = almostAgree;
    else diff = notAgree;
  }
  return diff;
}
bool AnaDataEmul::hasDtHits(unsigned int hitPattern) {
  std::bitset<29> hitBits(hitPattern);
  for (unsigned int i=0; i <= 5; i++) if (hitBits[i]) return true;
  return false; 
}
bool AnaDataEmul::hasCscHits(unsigned int hitPattern) {
  std::bitset<29> hitBits(hitPattern);
  for (unsigned int i=6; i <= 9; i++) if (hitBits[i]) return true;
  return false; 
}
bool AnaDataEmul::hasRpcHits(unsigned int hitPattern) {
  std::bitset<29> hitBits(hitPattern);
  for (unsigned int i=10; i <=17; i++) if (hitBits[i]) return true;
  return false; 
}

