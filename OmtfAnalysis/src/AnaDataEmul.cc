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
  TH1D *hDataEmulBxD, *hDataEmulBxE;
  TH2D *hDataEmulCheckProb;

  TH2D *hDataEmulDistributionData, *hDataEmulDistributionEmul;

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
  : debug(false), bxMin(cfg.getParameter<int>("bxMin")), bxMax(cfg.getParameter<int>("bxMax"))
{
}

std::string AnaDataEmul::matchName(const MATCH & diff) const
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

std::string AnaDataEmul::problName(const PROBL & diff) const
{
  switch (diff) {
    case (ok)          : return "OK"; break;
    case (wrong_size)  : return "wrong_size"; break;
    case (wrong_board) : return "wrong_board"; break;
    case (wrong_hits)  : return "wrong_hits"; break;
    case (wrong_kine)  : return "wrong_kine"; break;
    default:       return "unknown";
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
//  hDataEmulIssue->GetXaxis()->SetBinLabel(4,"phi (> #pm 1)");
  hDataEmulIssue->GetXaxis()->SetBinLabel(4,"phi");
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

  hDataEmulDistributionData = new TH2D("hDataEmulDistributionData"," hDataEmulDistributionData",13,-6.5, 6.5, 18, -0.5, 17.5); histos.Add( hDataEmulDistributionData); 
  hDataEmulDistributionEmul = new TH2D("hDataEmulDistributionEmul"," hDataEmulDistributionEmul",13,-6.5, 6.5, 18, -0.5, 17.5); histos.Add( hDataEmulDistributionEmul); 
  hDataEmulNotAgreeEta = new TH2D("hDataEmulNotAgreeEta","hDataEmulNotAgreeEta",13,-6.5, 6.5, nEtaBins, 0.5, nEtaBins+0.5); histos.Add(hDataEmulNotAgreeEta);
  hDataEmulNotAgreePhi = new TH2D("hDataEmulNotAgreePhi","hDataEmulNotAgreePhi",13,-6.5, 6.5, 24, -15, 105.); histos.Add(hDataEmulNotAgreePhi);
  for (unsigned int iProc = 0; iProc  <=5; iProc++) {
    for (int endcap = -1; endcap <=1; endcap+=2) {
      OmtfName board(iProc,endcap);  
      hDataEmulNotAgreeEta->GetXaxis()->SetBinLabel( board+7, board.name().c_str());
      hDataEmulNotAgreePhi->GetXaxis()->SetBinLabel( board+7, board.name().c_str());
      hDataEmulDistributionData->GetXaxis()->SetBinLabel( board+7, board.name().c_str());
      hDataEmulDistributionEmul->GetXaxis()->SetBinLabel( board+7, board.name().c_str());
    }
  }
  for (unsigned int ibin=1; ibin <= nEtaBins; ibin++) {
    int etaCode = etaBinVal[ibin-1];
    std::string slabel = std::to_string( etaCode );
    hDataEmulNotAgreeEta->GetYaxis()->SetBinLabel( ibin, slabel.c_str());
  }
  
  hDataEmulBxD = new TH1D("hDataEmulBxD","hDataEmulBx",10,-4.5,5.5); histos.Add(hDataEmulBxD);
  hDataEmulBxE = new TH1D("hDataEmulBxE","hDataEmulBx",10,-4.5,5.5); histos.Add(hDataEmulBxE);

  hDataEmulCheckProb = new TH2D("hDataEmulCheckProb","hDataEmulCheckProb",8, -3.5,4.5, 5, -0.5, 4.5); histos.Add(hDataEmulCheckProb); 
  {
    TAxis* y = hDataEmulCheckProb->GetYaxis();
           y->SetBinLabel(y->FindBin(ok),         problName(ok).c_str());
           y->SetBinLabel(y->FindBin(wrong_size), problName(wrong_size).c_str());
           y->SetBinLabel(y->FindBin(wrong_board),problName(wrong_board).c_str());
           y->SetBinLabel(y->FindBin(wrong_hits), problName(wrong_hits).c_str());
           y->SetBinLabel(y->FindBin(wrong_kine), problName(wrong_kine).c_str());
    hDataEmulCheckProb->GetXaxis()->SetNdivisions(110);
  }

}

void AnaDataEmul::run(EventObj* event, L1ObjColl * coll)
{
  if ( !*coll ) return;
  

  //
  // BX distribution
  //
  {
    L1ObjColl cD = coll->selectByType(L1Obj::OMTF); 
    L1ObjColl cE = coll->selectByType(L1Obj::OMTF_emu); 

    for (const auto & obj : cD.getL1Objs()) hDataEmulBxD->Fill(obj.bx);
    for (const auto & obj : cE.getL1Objs()) hDataEmulBxE->Fill(obj.bx); 
    
  }

  //
  // position districution
  //
  {
  std::vector<L1Obj> vdata = coll->selectByType(L1Obj::OMTF).selectByBx(); 
  for (const auto & obj : vdata) {
    std::bitset<18> hitLayers(obj.hits);
    for (unsigned int hitLayer=0; hitLayer<18;hitLayer++) if(hitLayers[hitLayer]) hDataEmulDistributionData->Fill(OmtfName(obj.iProcessor, obj.position),hitLayer);  
  }

  std::vector<L1Obj> vemul = coll->selectByType(L1Obj::OMTF_emu).selectByBx(); 
  for (const auto & obj : vemul) {
    std::bitset<18> hitLayers(obj.hits);
    for (unsigned int hitLayer=0; hitLayer<18;hitLayer++) if(hitLayers[hitLayer]) hDataEmulDistributionEmul->Fill(OmtfName(obj.iProcessor, obj.position),hitLayer);  
  }
  }

  for (int bx = bxMin; bx <= bxMax; bx++) {

    L1ObjColl dataColl = coll->selectByType(L1Obj::OMTF).selectByBx(bx,bx);
    L1ObjColl emulColl = coll->selectByType(L1Obj::OMTF_emu).selectByBx(bx,bx);
    std::vector<L1Obj> vdata = dataColl;
    
    // check problems
    if (dataColl.getL1Objs().size() || emulColl.getL1Objs().size()) {
      if (dataColl.getL1Objs().size() != emulColl.getL1Objs().size()) {
        hDataEmulCheckProb->Fill(bx,wrong_size);
      }
      else {
        for (const auto & data : dataColl.getL1Objs()) {
          PROBL prob = ok;
          for (const auto & emul : emulColl.getL1Objs()) {
            PROBL diff = checkProbl(data,emul);
            if (diff==ok) {prob=ok; break;}
            if (diff>prob)prob=diff;
          }
          hDataEmulCheckProb->Fill(bx,prob);
        }
      }
    }

    unsigned int idx=0;
    do { 
      const L1Obj * data = (vdata.size() > 0) ?  &(vdata[idx]) : 0;
      const L1Obj * emul = bestMatch(data, emulColl);
    
      if (!data && !emul) break;

    //  if (emul && ((emul->q & 0b01) !=0) ) return;
    //  if (emul && makeName(*emul).name()=="OMTFn4") return;  
    //  if (data && makeName(*data).name()=="OMTFn4") return;  
    //  if (emul && makeName(*emul).name()=="OMTFp1") return;  
    //  if (data && makeName(*data).name()=="OMTFp1") return;  
    //  if (emul && makeName(*emul).name()=="OMTFn1") return;  
    //  if (data && makeName(*data).name()=="OMTFn1") return;  
    
    
    //  bool lowQuality = false;
    //  if (data && (data->q == 4) ) lowQuality = true; 
    //  if (emul && (emul->q == 4) ) lowQuality = true; 
    
      MATCH diff = checkMatch(data, emul);   
      if (data && emul && (vdata.size() != emulColl.getL1Objs().size()) ) diff = sizeDiff;

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

      bool unique = data && emul && (vdata.size() == 1) && (emulColl.getL1Objs().size() == 1) && (makeName(*data)==makeName(*emul));
      if(unique && !(diff==agree) ) {
         hDataEmulNotAgreeEta->Fill( OmtfName(emul->iProcessor, emul->position), code2HistoBin(abs(emul->eta)) ); 
         hDataEmulNotAgreePhi->Fill( OmtfName(emul->iProcessor, emul->position), emul->phi ); 
      }
    
      if (debug && diff!=agree ) std::cout << "NOT agree("<<matchName(diff)<<"), dt: "<< dt <<", csc: "<< csc <<", rpcB: "<< hasRpcHitsB(hits)<<", rpcE: "<<hasRpcHitsE(hits) << std::endl <<*coll << std::endl; 
    
      if (unique) {
        hDataEmulPt->Fill( code2pt(data->pt), code2pt(emul->pt) );
        hDataEmulPhi->Fill(data->phi, emul->phi);
        hDataEmulIssue->Fill(1);
        if (data->hits != emul->hits)          hDataEmulIssue->Fill(2);
        if (data->pt != emul->pt)              hDataEmulIssue->Fill(3); 
        if ( abs(data->phi - emul->phi) !=0)   hDataEmulIssue->Fill(4); 
        if (data->eta != emul->eta)            hDataEmulIssue->Fill(5);
        if ( data->charge !=  emul->charge)    hDataEmulIssue->Fill(6);
//      if ( (data->q >>2) != (emul->q >>2) )  hDataEmulIssue->Fill(7);
        if ( (data->q    ) != (emul->q    ) )  hDataEmulIssue->Fill(7);
      }
      if(unique) { hDataEmulEta->Fill(code2HistoBin(abs(data->eta)), code2HistoBin(abs(emul->eta)) ); }
    
    } while ( ++idx < vdata.size() );
  }
}

AnaDataEmul::PROBL AnaDataEmul::checkProbl(const L1Obj & data, const L1Obj & emul)
{
  OmtfName dataBoard = makeName(data);
  OmtfName emulBoard = makeName(emul);
  if ( dataBoard != emulBoard) return wrong_board;
  if (data.hits != emul.hits) return wrong_hits;
  if (data.q != emul.q) return wrong_kine;
  if (data.charge != emul.charge) return wrong_kine;
  if (data.pt  != emul.pt ) return wrong_kine;
  if (data.phi != emul.phi) return wrong_kine;
  if (data.eta != emul.eta) return wrong_kine;
  return ok;
}

AnaDataEmul::MATCH AnaDataEmul::checkMatch(const L1Obj * data, const L1Obj * emul)
{

  MATCH diff = unknown;

  if (!data && !emul) return diff;

  if (data && !emul) diff = dataOnly;
  else if (!data && emul) diff = emulOnly;
  else {
    unsigned int hitsEmul = emul->hits;
    unsigned int hitsData = data->hits;
    if (    (makeName(*data)==makeName(*emul))
         && (hitsEmul == hitsData) 
         && (data->pt  == emul->pt) 
         && (data->phi == emul->phi) 
         && (data->eta == emul->eta) 
         && (data->q   == emul->q  ) 
         && (data->charge == emul->charge)
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
  MATCH best = disagree;
  if (!data) {
    if (emuColl.getL1Objs().size()!=0) emul = &(emuColl.getL1Objs().front());
  } else {
    OmtfName dataBoard = makeName(*data); 
    for (auto & l1Emu : emuColl.getL1Objs()) {
      const L1Obj & tmp = l1Emu;
      OmtfName emulBoard = makeName(tmp);
      if ( dataBoard != emulBoard) continue;
      MATCH diff = checkMatch(data, &tmp);
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
    std::cout <<" RUN: "<<run <<" D-E: "<< effAndErr
              <<" stat: "<<theRunMap.stat(run).first<<"/"<<theRunMap.stat(run).second<<std::endl;
    hGraphRun->SetPoint(iPoint, run, effAndErr.eff());
    hGraphRun->SetPointError(iPoint, 0., effAndErr.effErr());
  }
  std::cout << "Data-Emul summ.: " << theRunMap.effAndErr() << std::endl;
}

