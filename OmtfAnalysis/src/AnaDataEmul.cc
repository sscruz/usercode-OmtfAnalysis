#include "UserCode/OmtfAnalysis/interface/AnaDataEmul.h"

#include <cmath>
#include <bitset>
#include <string>

#include "TObjArray.h"
#include "TGraphErrors.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TAxis.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "L1Trigger/L1TMuonOverlap/interface/OmtfName.h"
#include "UserCode/OmtfDataFormats/interface/EventObj.h"
#include "UserCode/OmtfDataFormats/interface/L1ObjColl.h"
#include "UserCode/OmtfAnalysis/interface/Utilities.h"

namespace {
  TH1D *hDataEmulCompare; 
  TH1D *hDataEmulIssue;
  TH2D *hDataEmulCompareComb;
  TH2D *hDataEmulPt, *hDataEmulPhi, *hDataEmulEta;
  TH2D *hDataEmulNotAgreeEta, *hDataEmulNotAgreePhi;

  double ptBins[]={ 0., 0.1, 
                   1.5, 2., 2.5, 3., 3.5, 4., 4.5, 5., 6., 7., 8., 
                   10., 12., 14., 16., 18., 20., 25., 30., 35., 40., 45., 
                   50., 60., 70., 80., 90., 100., 120., 140., 
                   160};

  const int nEtaBins = 14;
  int  etaBinVal[nEtaBins] = {73, 75, 78, 79, 85, 90, 92, 94, 95, 99, 103, 110, 115, 121}; 

  double code2pt(int pt) { return std::fmin( double(pt-1)/2., 159); } 

  template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

  int code2HistoBin (int code) { 
    int sign = sgn(code);
    switch (abs(code)) {
      case  73 : return sign*1;
      case  75 : return sign*2;
      case  78 : return sign*3;
      case  79 : return sign*4;
      case  85 : return sign*5;
      case  90 : return sign*6;
      case  92 : return sign*7;
      case  94 : return sign*8;
      case  95 : return sign*9;
      case  99 : return sign*10;
      case 103 : return sign*11;
      case 110 : return sign*12;
      case 115 : return sign*13;
      case 121 : return sign*14;
      default:  return sign*0;
    }
  } 



   


}

AnaDataEmul::AnaDataEmul(const edm::ParameterSet& cfg)
{
}

std::string AnaDataEmul::diffName(const DIFF & diff) const
{
  switch (diff) {
    case (agree) :       return "Agree"; break;
    case (almostAgree) : return "Almost Agree"; break;
    case (ratherAgree) : return "Rather Agree"; break;
    case (disagree) :    return "Disagree"; break;
    case (dataOnly) :    return "Data Only"; break;
    case (emulOnly) :    return "Emul Only"; break;
    case (sizeDiff) :    return "Size Diff"; break;
    default:             return "unknown";
  }
}

void AnaDataEmul::init(TObjArray& histos)
{
  hDataEmulCompare = new TH1D("hDataEmulCompare","hDataEmulCompare",7, 0.5,7.5); histos.Add(hDataEmulCompare);
  hDataEmulCompare->GetXaxis()->SetBinLabel(1,"Agree");
  hDataEmulCompare->GetXaxis()->SetBinLabel(2,"Almost Agree");
  hDataEmulCompare->GetXaxis()->SetBinLabel(3,"Rather Agree");
  hDataEmulCompare->GetXaxis()->SetBinLabel(4,"Disagree");
  hDataEmulCompare->GetXaxis()->SetBinLabel(5,"Data Only");
  hDataEmulCompare->GetXaxis()->SetBinLabel(6,"Emul Only");
  hDataEmulCompare->GetXaxis()->SetBinLabel(7,"Size Diff");
  
  hDataEmulCompareComb = new TH2D("hDataEmulCompareComb","hDataEmulCompareComb", 7, 0.5,7.5, 7, 0.5,7.5); 
  histos.Add(hDataEmulCompareComb);
  hDataEmulCompareComb->GetXaxis()->SetBinLabel(1,"Agree");
  hDataEmulCompareComb->GetXaxis()->SetBinLabel(2,"Almost Agree");
  hDataEmulCompareComb->GetXaxis()->SetBinLabel(3,"Rather Agree");
  hDataEmulCompareComb->GetXaxis()->SetBinLabel(4,"Not Agree");
  hDataEmulCompareComb->GetXaxis()->SetBinLabel(5,"Data Only");
  hDataEmulCompareComb->GetXaxis()->SetBinLabel(6,"Emul Only");
  hDataEmulCompareComb->GetXaxis()->SetBinLabel(7,"Size Diff");
  hDataEmulCompareComb->GetYaxis()->SetBinLabel(1,"Dt+Csc+Rpc");
  hDataEmulCompareComb->GetYaxis()->SetBinLabel(2,"Dt+Csc");
  hDataEmulCompareComb->GetYaxis()->SetBinLabel(3,"Dt+Rpc");
  hDataEmulCompareComb->GetYaxis()->SetBinLabel(4,"Csc+Rpc");
  hDataEmulCompareComb->GetYaxis()->SetBinLabel(5,"Dt Only");
  hDataEmulCompareComb->GetYaxis()->SetBinLabel(6,"Csc Only");
  hDataEmulCompareComb->GetYaxis()->SetBinLabel(7,"Rpc Only");

  hDataEmulPt = new TH2D("hDataEmulPt","hDataEmulPt",32, ptBins, 32, ptBins); histos.Add(hDataEmulPt);
  hDataEmulPhi = new TH2D("hDataEmulPhi","hDataEmulPhi",150,-30.,120.,150.,-30.,120.); histos.Add(hDataEmulPhi);

  hDataEmulIssue = new TH1D("hDataEmulIssue","hDataEmulIssue",7,  0.5, 7.5); histos.Add(hDataEmulIssue);
  hDataEmulIssue->GetXaxis()->SetBinLabel(1," ");
  hDataEmulIssue->GetXaxis()->SetBinLabel(2,"hits");
  hDataEmulIssue->GetXaxis()->SetBinLabel(3,"p_{T}");
  hDataEmulIssue->GetXaxis()->SetBinLabel(4,"phi (> #pm 1)");
  hDataEmulIssue->GetXaxis()->SetBinLabel(5,"eta");
  hDataEmulIssue->GetXaxis()->SetBinLabel(6,"charge");
  hDataEmulIssue->GetXaxis()->SetBinLabel(7,"quality");

//  double etaBins[nEtaBins+1]; 
//  for (unsigned int idx=0; idx<nEtaBins; idx++) etaBins[idx]=etaBinVal[idx]-0.1; 
//  etaBins[0]=etaBinVal[0]-1.; etaBins[nEtaBins]=etaBinVal[nEtaBins-1]+1.;
//  hDataEmulEta = new TH2D("hDataEmulEta","hDataEmulEta", nEtaBins, etaBins, nEtaBins, etaBins); histos.Add(hDataEmulEta);

  hDataEmulEta = new TH2D("hDataEmulEta","hDataEmulEta", nEtaBins, 0.5, nEtaBins+0.5, nEtaBins, 0.5, nEtaBins+0.5); histos.Add(hDataEmulEta);
  for (unsigned int ibin=1; ibin <= nEtaBins; ibin++) {
    int etaCode = etaBinVal[ibin-1];
    std::string slabel = std::to_string( etaCode );
    if (etaCode == 73) slabel += " (uflv) ";
    if (etaCode == 75) slabel += " (mid MB3) ";
    if (etaCode == 79) slabel += " (mid MB2) ";
    if (etaCode == 95) slabel += " (mid OMTF) ";
    if (etaCode == 92) slabel += " (mid MB1) ";
    if (etaCode == 115) slabel += " (oflv) ";
    if (etaCode == 121) slabel += " (foflv) ";
    hDataEmulEta->GetXaxis()->SetBinLabel( ibin, slabel.c_str());
    hDataEmulEta->GetYaxis()->SetBinLabel( ibin, slabel.c_str());
  }

  hDataEmulNotAgreeEta = new TH2D("hDataEmulNotAgreeEta","hDataEmulNotAgreeEta",13,-6.5, 6.5, nEtaBins, 0.5, nEtaBins+0.5); histos.Add(hDataEmulNotAgreeEta);
  hDataEmulNotAgreePhi = new TH2D("hDataEmulNotAgreePhi","hDataEmulNotAgreePhi",13,-6.5, 6.5, 24, -15, 105.); histos.Add(hDataEmulNotAgreePhi);
  for (unsigned int iProc = 0; iProc  <=5; iProc++) {
    for (int endcap = -1; endcap <=1; endcap+=2) {
      OmtfName board(iProc,endcap);  
      hDataEmulNotAgreeEta->GetXaxis()->SetBinLabel( board+7, board.name().c_str());
      hDataEmulNotAgreePhi->GetXaxis()->SetBinLabel( board+7, board.name().c_str());
    }
  }
  for (unsigned int ibin=1; ibin <= nEtaBins; ibin++) {
    int etaCode = etaBinVal[ibin-1];
    std::string slabel = std::to_string( etaCode );
    hDataEmulNotAgreeEta->GetYaxis()->SetBinLabel( ibin, slabel.c_str());
  }
  
  

}

void AnaDataEmul::run(EventObj* event, L1ObjColl * coll)
{
  if ( !*coll ) return;

  const L1Obj * data = 0;
  L1ObjColl dataColl =  coll->selectByType(L1Obj::OMTF);
  if (dataColl.getL1Objs().size()!=0) data = &(dataColl.getL1Objs().front());

  const L1Obj * emul = 0;
  L1ObjColl emulColl =  coll->selectByType(L1Obj::OMTF_emu);
  emul = bestMatch(data, emulColl);

  if (!data && !emul) return;
  if (emul && ((emul->q & 0b01) !=0) ) return;

//  if (emul && makeName(*emul).name()=="OMTFn4") return;  
//  if (data && makeName(*data).name()=="OMTFn4") return;  


  bool unique = data && emul && (dataColl.getL1Objs().size() == 1) && (emulColl.getL1Objs().size() == 1);

//  bool lowQuality = false;
//  if (data && (data->q == 4) ) lowQuality = true; 
//  if (emul && (emul->q == 4) ) lowQuality = true; 

  DIFF diff = compare(data, emul);   
  if (data && emul && (dataColl.getL1Objs().size() != emulColl.getL1Objs().size()) ) diff = sizeDiff;

  hDataEmulCompare->Fill(diff);
  theRunMap.addEvent(event->run, (diff==agree) ); 

  unsigned int hits = 0;
  if (emul) hits |= emul->hits;
  if (data) hits |= data->hits;
  unsigned int dt = hasDtHits(hits);
  unsigned int csc = hasCscHits(hits);
  unsigned int rpc = hasRpcHits(hits);

  int layerComb = 0;
  if ( dt &&  csc &&  rpc) layerComb = 1;
  if ( dt &&  csc && !rpc) layerComb = 2;
  if ( dt && !csc &&  rpc) layerComb = 3;
  if (!dt &&  csc &&  rpc) layerComb = 4;
  if ( dt && !csc && !rpc) layerComb = 5;
  if (!dt &&  csc && !rpc) layerComb = 6;
  if (!dt && !csc &&  rpc) layerComb = 7;
  hDataEmulCompareComb->Fill(diff, layerComb); 
  if(unique && !(diff==agree) ) {
     hDataEmulNotAgreeEta->Fill( OmtfName(emul->iProcessor, emul->position), code2HistoBin(abs(emul->eta)) ); 
     hDataEmulNotAgreePhi->Fill( OmtfName(emul->iProcessor, emul->position), emul->phi ); 
  }

  if (diff!=agree ) std::cout << "NOT agree("<<diffName(diff)<<"), dt: "<< dt <<", csc: "<< csc <<", rpcB: "<< hasRpcHitsB(hits)<<", rpcE: "<<hasRpcHitsE(hits) << std::endl; 

  if (unique) {
    hDataEmulPt->Fill( code2pt(data->pt), code2pt(emul->pt) );
    hDataEmulPhi->Fill(data->phi, emul->phi);
    hDataEmulIssue->Fill(1);
    if (data->hits != emul->hits)          hDataEmulIssue->Fill(2);
    if (data->pt != emul->pt)              hDataEmulIssue->Fill(3); 
    if ( abs(data->phi - emul->phi) > 1)   hDataEmulIssue->Fill(4); 
    if (data->eta != emul->eta)            hDataEmulIssue->Fill(5);
    if ( data->charge !=  emul->charge)   hDataEmulIssue->Fill(6);
    if ( (data->q >>2) != (emul->q >>2) )  hDataEmulIssue->Fill(7);
  }
  if(diff==agree) { hDataEmulEta->Fill(code2HistoBin(abs(data->eta)), code2HistoBin(abs(emul->eta)) ); }
    
}

AnaDataEmul::DIFF AnaDataEmul::compare(const L1Obj * data, const L1Obj * emul)
{

  DIFF diff = unknown;

  if (!data && !emul) return diff;

  if (data && !emul) diff = dataOnly;
  else if (!data && emul) diff = emulOnly;
  else {
    unsigned int hitsEmul = emul->hits;
    unsigned int hitsData = data->hits;
    if (    (hitsEmul == hitsData) 
            && (data->pt == emul->pt) 
            && (data->phi == emul->phi) 
            && data->charge == emul->charge
           ) diff = agree; 
    else if (    abs(data->pt-emul->pt) <= 2 
              && abs(data->phi - emul->phi) <= 1 
              && data->charge == emul->charge) diff = almostAgree; 
    else if (     static_cast<double>(abs(data->pt - emul->pt))/static_cast<double>(data->pt+emul->pt) <= 0.15   // difference by 15%
              && abs(data->phi - emul->phi) <= 5) diff = ratherAgree; 
    else diff = disagree;
  }
  return diff;
}
unsigned int AnaDataEmul::hasDtHits(unsigned int hitPattern) {
  unsigned int result = 0;
  std::bitset<29> hitBits(hitPattern);
  for (unsigned int i=0; i <= 5; i++) if (hitBits[i]) result++;
  return result; 
}
unsigned int AnaDataEmul::hasCscHits(unsigned int hitPattern) {
  unsigned int result = 0;
  std::bitset<29> hitBits(hitPattern);
  for (unsigned int i=6; i <= 9; i++) if (hitBits[i]) result++;
  return result; 
}
unsigned int AnaDataEmul::hasRpcHits(unsigned int hitPattern) {
  return hasRpcHitsB(hitPattern)+hasRpcHitsE(hitPattern);
}
unsigned int AnaDataEmul::hasRpcHitsB(unsigned int hitPattern) {
  unsigned int result = 0;
  std::bitset<29> hitBits(hitPattern);
  for (unsigned int i=10; i <=14; i++) if (hitBits[i])  result++;
  return result; 
}
unsigned int AnaDataEmul::hasRpcHitsE(unsigned int hitPattern) {
  unsigned int result = 0;
  std::bitset<29> hitBits(hitPattern);
  for (unsigned int i=15; i <=17; i++) if (hitBits[i])  result++;
  return result; 
}

const L1Obj * AnaDataEmul::bestMatch( const L1Obj * data, const L1ObjColl & emuColl)
{
  const L1Obj * emul = 0;
  DIFF best = disagree;
  if (!data) {
    if (emuColl.getL1Objs().size()!=0) emul = &(emuColl.getL1Objs().front());
  } else {
    OmtfName dataBoard = makeName(*data); 
    const std::vector<L1Obj> & l1EmuObjs = emuColl; 
    for (auto & l1Emu : l1EmuObjs) {
      const L1Obj & tmp = l1Emu;
      OmtfName emulBoard = makeName(tmp);
      if ( dataBoard != emulBoard) continue;
      DIFF diff = compare(data, &tmp);
      if (diff == agree) { emul = &tmp; best = agree; }
      else if (diff == ratherAgree && best != agree) { emul = &tmp; best = ratherAgree; }
      else if (best != agree && best != ratherAgree) { emul = &tmp;}
    }
  }
  return emul;
}

void AnaDataEmul::resume(TObjArray& histos)
{
  TGraphErrors * hGraphRun = new TGraphErrors();
  hGraphRun->SetName("hGraphDataEmulHistory");
  histos.Add(hGraphRun);
  std::vector<unsigned int> runs = theRunMap.runs();
  hGraphRun->Set(runs.size());
  for (unsigned int iPoint = 0; iPoint < runs.size(); iPoint++) {
    unsigned int run = runs[iPoint];
    RunEffMap::EffAndErr effAndErr = theRunMap.effAndErr(run); 
    hGraphRun->SetPoint(iPoint, run, effAndErr.eff());
    hGraphRun->SetPointError(iPoint, 0., effAndErr.effErr());
  }
}

