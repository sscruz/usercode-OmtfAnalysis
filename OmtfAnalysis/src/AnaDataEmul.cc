#include "UserCode/OmtfAnalysis/interface/AnaDataEmul.h"

#include <cmath>
#include <bitset>
#include <string>

#include "TObjArray.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TAxis.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "UserCode/OmtfDataFormats/interface/L1ObjColl.h"
#include "L1Trigger/L1TMuonOverlap/interface/OmtfName.h"

namespace {
  TH1D *hDataEmulCompare; 
  TH2D *hDataEmulCompareComb;
  TH2D *hDataEmulPt, *hDataEmulPhi, *hDataEmulEta;
  TH2D *hDataEmulNotAgree;

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

  OmtfName makeName(const L1Obj & obj) { return OmtfName(obj.iProcessor, obj.position); }

   


}

AnaDataEmul::AnaDataEmul(const edm::ParameterSet& cfg)
{
}

void AnaDataEmul::init(TObjArray& histos)
{
  hDataEmulCompare = new TH1D("hDataEmulCompare","hDataEmulCompare",7, 0.5,7.5); histos.Add(hDataEmulCompare);
  hDataEmulCompare->GetXaxis()->SetBinLabel(1,"Agree");
  hDataEmulCompare->GetXaxis()->SetBinLabel(2,"Almost Agree");
  hDataEmulCompare->GetXaxis()->SetBinLabel(3,"Rather Agree");
  hDataEmulCompare->GetXaxis()->SetBinLabel(4,"Not Agree");
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

  hDataEmulNotAgree = new TH2D("hDataEmulNotAgree","hDataEmulNotAgree",13,-6.5, 6.5, nEtaBins, 0.5, nEtaBins+0.5); histos.Add(hDataEmulNotAgree);
  for (unsigned int iProc = 0; iProc  <=5; iProc++) {
    for (int endcap = -1; endcap <=1; endcap+=2) {
      OmtfName board(iProc,endcap);  
      hDataEmulNotAgree->GetXaxis()->SetBinLabel( board+7, board.name().c_str());
    }
  }
  for (unsigned int ibin=1; ibin <= nEtaBins; ibin++) {
    int etaCode = etaBinVal[ibin-1];
    std::string slabel = std::to_string( etaCode );
    hDataEmulNotAgree->GetYaxis()->SetBinLabel( ibin, slabel.c_str());
  }
  
  

}

void AnaDataEmul::run(L1ObjColl * coll)
{
  if ( !*coll ) return;

  const L1Obj * data = 0;
  L1ObjColl dataColl =  coll->selectByType(L1Obj::OMTF);
  if (dataColl.getL1Objs().size()!=0) data = &(dataColl.getL1Objs().front());

  const L1Obj * emul = 0;
  L1ObjColl emulColl =  coll->selectByType(L1Obj::OMTF_emu);
  emul = bestMatch(data, emulColl);
  if (emul && emul->q==0) return;

  bool unique = data && emul && (dataColl.getL1Objs().size() == 1) && (emulColl.getL1Objs().size() == 1);

  bool lowQuality = false;
  if (data && data->q == 4 ) lowQuality = true; 
  if (emul && emul->q == 4 ) lowQuality = true; 

  DIFF diff = compare(data, emul);   
  if (data && emul && (dataColl.getL1Objs().size() != emulColl.getL1Objs().size()) ) diff = sizeDiff;

  if (!lowQuality) hDataEmulCompare->Fill(diff);

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
  if(!lowQuality) hDataEmulCompareComb->Fill(diff, layerComb); 
  if(unique && diff==notAgree) hDataEmulNotAgree->Fill( OmtfName(emul->iProcessor, emul->position), code2HistoBin(abs(emul->eta)) ); 

//  if (!dt && !csc && rpc && diff!=agree) std::cout <<" ***** RPC only, not agree  "<< std::endl;
  if (rpc && diff!=agree ) std::cout << "NOT agree, dt: "<< dt <<", csc: "<< csc <<", rpcB: "<< hasRpcHitsB(hits)<<", rpcE: "<<hasRpcHitsE(hits) << std::endl; 

 if (unique) {
   hDataEmulPt->Fill( code2pt(data->pt), code2pt(emul->pt) );
   hDataEmulPhi->Fill(data->phi, emul->phi);
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
            && data->charge == emul->charge) diff = agree; 
    else if (    abs(data->pt-emul->pt) <= 2 
              && abs(data->phi == emul->phi) <= 2 
              && data->charge == emul->charge) diff = almostAgree; 
    else if (     static_cast<double>(abs(data->pt - emul->pt))/static_cast<double>(data->pt+emul->pt) <= 0.15   // difference by 15%
              && abs(data->phi == emul->phi) <= 5) diff = ratherAgree; 
    else diff = notAgree;
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
  DIFF best = notAgree;
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

