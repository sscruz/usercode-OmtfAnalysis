#ifndef UserCode_OmtfAnalysis_OmtfAlgoHit_H
#define UserCode_OmtfAnalysis_OmtfAlgoHit_H

#include <sstream>
#include <iomanip>
#include <vector>
#include <iostream>
#include <bitset>

struct OmtfAlgoHit {
  enum ChamberType { dt, dt_b, csc, rpc };
  static const unsigned int NUM_LAYERS = 18;
  static const unsigned int NUM_INPUTS = 14;
  static const unsigned int BITS_PHI   = 11;
  static const unsigned int BITS_ETA   =  9;
  static const unsigned int BITS_VALID =  1;
  static const unsigned int BITS_HITS = BITS_VALID
                                      + BITS_ETA
                                      + BITS_PHI;
  unsigned int layer;                          // layer number: [0,17]
  unsigned int input;                          // hit in layer: [0,13]
    signed int phi             : BITS_PHI;       // hit phi: [-1024,1023]
  unsigned int eta             : BITS_ETA;     // eta bitpatern [0 0000000 0]
  unsigned int bx;
  bool valid;
  bool empty() const { return !(phi||eta); }
  ChamberType chamberType() const {
    if (layer <= 5 ) { if (layer%2) return dt_b; else return dt; }
    else if ( layer <= 9) return csc;
    else return rpc;
  }
  friend std::ostream & operator<< (std::ostream &out, const OmtfAlgoHit &o) {
    out <<" AlgoHit: "<<(o.valid ? "v" : " ")<<std::setfill(' ')<<std::dec
        <<"  layer: "<<std::setw(2)<<o.layer
        <<", input: "<<std::setw(2)<<o.input
        <<", phi: " <<std::setw(5)<<o.phi
        <<", eta: " <<std::setw(5)<<o.eta
        <<", bx: "<<std::setw(4)<<o.bx ;
    return out;
  }
};

struct OmtfAlgoMuon {
  OmtfAlgoMuon() : bestPattern(0), hitCount(0), layerHitVector(0), weight(0), bxOut(0), refHitPhi(0), phi(0), eta(0), valid(0) {}

  static const unsigned int BITS_BESTPATTERN = 8;
  static const unsigned int BITS_HITCOUNT = 5;
  static const unsigned int BITS_LAYERHITVECTOR = 19;
  static const unsigned int BITS_WEIGHT = 12;
  static const unsigned int BITS_BXOUT = 12;
  static const unsigned int BITS_REFHITPHI = 11;
  static const unsigned int BITS_PHI = 11;
  static const unsigned int BITS_ETA = 9;
  static const unsigned int BITS_VALID = 1;

  static const unsigned int BITS_MUON = BITS_BESTPATTERN
                                      + BITS_HITCOUNT
                                      + BITS_LAYERHITVECTOR
                                      + BITS_WEIGHT
                                      + BITS_BXOUT
                                      + BITS_REFHITPHI
                                      + BITS_PHI
                                      + BITS_ETA
                                      + BITS_VALID;


  uint32_t bestPattern    : BITS_BESTPATTERN;    //omtf_best_pat
  uint32_t hitCount       : BITS_HITCOUNT;       //omtf_fired_cnt
  uint32_t layerHitVector : BITS_LAYERHITVECTOR; //omtf_fired_vec
  uint32_t weight         : BITS_WEIGHT;         //omtf_weight_sum
  uint32_t bxOut          : BITS_BXOUT;          //omtf_bx_out - zgodnosc BX 4 struktur
  signed int refHitPhi    : BITS_REFHITPHI;      //omtf_ref_phi
  signed int  phi         : BITS_PHI;            //omtf_corr_ref_phi
  unsigned int eta        : BITS_ETA;
  uint32_t valid          : BITS_VALID;          //omtf_found

  friend std::ostream & operator<< (std::ostream &out, const OmtfAlgoMuon &o) {
    std::bitset<18> firedLayers(o.layerHitVector);//FIXME - check number of bits, fixes from WZAb
    std::ostringstream firedLayersOstr;
    for(unsigned int i = 0; i < firedLayers.size(); i++) { if(firedLayers[i]) firedLayersOstr<<i<<", "; }
    std::stringstream flayers;
    for (unsigned int i=0; i < firedLayers.size(); i++) {
      switch (i) { case(6): case (10): flayers<<" "; default: if(firedLayers[i])flayers<<i%10; else flayers<<".";}
    }
    out <<"AlgoMuon: "
       <<(o.valid ? "v" : " ")<<std::setfill(' ')<<std::dec
       <<"  pat: "<<std::setw(2)<<o.bestPattern
       <<", phi: " <<std::setw(5)<<o.phi
       <<", hitPhi: "<<std::setw(5)<< o.refHitPhi
       <<", eta:"<<std::setw(5)<<o.eta
       <<", hits: "<<std::setw(2)<<o.hitCount
       <<", weight: "<< std::setw(4)<<o.weight
       <<", ["<<flayers.str()<<"]"
       <<", bx: "<<std::setw(4)<<o.bxOut ;
    return out;
  }
};

#include "OmtfGmtData.h"


#endif
