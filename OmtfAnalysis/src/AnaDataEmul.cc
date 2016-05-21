#include "UserCode/OmtfAnalysis/interface/AnaDataEmul.h"

#include <cmath>
#include <bitset>

#include "TObjArray.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TAxis.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "UserCode/OmtfDataFormats/interface/L1ObjColl.h"

namespace {
  TH1D *hDataEmulCompare; 
  TH2D *hDataEmulCompareComb;
  TH2D *hDataEmulPt, *hDataEmulPhi, *hDataEmulEta;

  double ptBins[]={ 0., 0.1, 
                   1.5, 2., 2.5, 3., 3.5, 4., 4.5, 5., 6., 7., 8., 
                   10., 12., 14., 16., 18., 20., 25., 30., 35., 40., 45., 
                   50., 60., 70., 80., 90., 100., 120., 140., 
                   160};

  double code2pt(int pt) { return std::fmin( double(pt-1)/2., 159); } 

  template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

  int code2Bit (int code) { 
    int sign = sgn(code);
    switch (abs(code)) {
      case  73 : return sign*8;
      case  78 : return sign*7;
      case  85 : return sign*6;
      case  90 : return sign*5;
      case  94 : return sign*4;
      case  99 : return sign*3;
      case 103 : return sign*2;
      case 110 : return sign*1;
      case 115 : return sign*0;
      case  95 : return sign*10;
      default:  return sign*9;
    }
  } 


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
  //hDataEmulEta = new TH2D("hDataEmulEta","hDataEmulEta",100,50.,150.,100.,50.,150.); histos.Add(hDataEmulEta);
  hDataEmulEta = new TH2D("hDataEmulEta","hDataEmulEta",11,-0.5,10.5, 11.,-0.5 ,10.5); histos.Add(hDataEmulEta);
}

void AnaDataEmul::run(L1ObjColl * coll)
{
  if ( !*coll ) return;

  const L1Obj * data = 0;
  L1ObjColl dataColl =  coll->selectByType(L1Obj::OMTF);
  if (dataColl.getL1Objs().size()!=0) data = &(dataColl.getL1Objs().front());

  const L1Obj * emul = 0;
  L1ObjColl emulColl =  coll->selectByType(L1Obj::OMTF_emu);
  if (emulColl.getL1Objs().size()!=0) emul = &(emulColl.getL1Objs().front());

  bool unique = data && emul && (dataColl.getL1Objs().size() == emulColl.getL1Objs().size());

  unsigned int hits = 0;
  if (data || emul) {
    unsigned int hitsEmul = emul ? emul->hits >>1 : 0;
    unsigned int hitsData = data ? data->hits >>2 : 0;
    hits = (hitsEmul | hitsData) << 1; 
  }
  
  DIFF diff = compare(data, emul);   
  if (data && emul && (dataColl.getL1Objs().size() != emulColl.getL1Objs().size()) ) diff = sizeDiff;
  hDataEmulCompare->Fill(diff);
  if (diff==notAgree) std::cout <<"**** NOT Agree" << std::endl;

  bool  dt = hasDtHits(hits);
  bool csc = hasCscHits(hits);
  bool rpc = hasRpcHits(hits);
  if ( dt &&  csc &&  rpc) hDataEmulCompareComb-> Fill(diff,1);
  if ( dt &&  csc && !rpc) hDataEmulCompareComb-> Fill(diff,2);
  if ( dt && !csc &&  rpc) hDataEmulCompareComb-> Fill(diff,3);
  if (!dt &&  csc &&  rpc) hDataEmulCompareComb-> Fill(diff,4);
  if ( dt && !csc && !rpc) hDataEmulCompareComb-> Fill(diff,5);
  if (!dt &&  csc && !rpc) hDataEmulCompareComb-> Fill(diff,6);
  if (!dt && !csc &&  rpc) hDataEmulCompareComb-> Fill(diff,7);

 if (unique) {
   hDataEmulPt->Fill( code2pt(data->pt), code2pt(emul->pt) );
   hDataEmulPhi->Fill(data->phi, emul->phi);
   hDataEmulEta->Fill(code2Bit(abs(data->eta)), code2Bit(abs(emul->eta)));
   std::cout <<"emul->eta: " << emul->eta<<" "<< code2Bit(abs(emul->eta)) << std::endl;
 }
    
}

AnaDataEmul::DIFF AnaDataEmul::compare(const L1Obj * data, const L1Obj * emul)
{

  DIFF diff = unknown;

  if (!data && !emul) return diff;

  if (data && !emul) diff = dataOnly;
  else if (!data && emul) diff = emulOnly;
  else {
    unsigned int hitsEmul = emul->hits | 1<<17;
    unsigned int hitsData = data->hits | 1<<17;
    if (    (hitsEmul == hitsData) 
            && (data->pt == emul->pt) 
            && (data->phi == emul->phi) 
            && data->charge == emul->charge) diff = agree; 
    else if (    abs(data->pt-emul->pt) <= 2 
              && abs(data->phi == emul->phi) <= 2 
              && data->charge == emul->charge) diff = almostAgree; 
    else if (     static_cast<double>(abs(data->pt - emul->pt))/double(data->pt+emul->pt) <= 0.15 
              && abs(data->phi == emul->phi) <= 5) diff = ratherAgree; 
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

