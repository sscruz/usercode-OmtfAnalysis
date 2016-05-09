#ifndef UserCode_OmtfAnalysis_OmtfGmtData_H
#define UserCode_OmtfAnalysis_OmtfGmtData_H

#include <ostream>
#include <iomanip>
#include <vector>
#include <iostream>
#include <bitset>

struct OmtfGmtData {

  static const unsigned int BITS_PT = 9;
  static const unsigned int BITS_QUA = 4;
  static const unsigned int BITS_ETA = 9;
  static const unsigned int BITS_HALO = 1;
  static const unsigned int BITS_PHI = 8;
  static const unsigned int BITS_BCN0 = 1;

  static const unsigned int BITS_CHARGE = 1;
  static const unsigned int BITS_VALID = 1;
  static const unsigned int BITS_TRACK = 29;
  static const unsigned int BITS_SYNCH_ERR = 1;

  static const unsigned int BITS_DATA = BITS_PT
                                            + BITS_QUA
                                            + BITS_ETA
                                            + BITS_HALO
                                            + BITS_PHI
                                            + BITS_BCN0
                                            + BITS_CHARGE
                                            + BITS_VALID
                                            + BITS_TRACK
                                            + BITS_SYNCH_ERR;

  struct Synch {
    uint32_t bc0      : BITS_BCN0;
    uint32_t synchErr : BITS_SYNCH_ERR;
    uint32_t bcn      : 2*(BITS_BCN0+BITS_SYNCH_ERR);
  };

  struct CandMuon {
    uint32_t pt      : BITS_PT;
    uint32_t quality : BITS_QUA;
    signed int  eta  : BITS_ETA;
    uint32_t halo    : BITS_HALO;
    signed int phi   : BITS_PHI;
    uint32_t charge  : BITS_CHARGE;
    uint32_t valCh   : BITS_VALID;
    uint32_t track   : BITS_TRACK;
    friend std::ostream & operator<< (std::ostream &out, const OmtfGmtData::CandMuon &o) {
      std::string charge = (o.charge==0) ? "+" : "-";
      out << "CandMuon: "<<std::dec
          <<" q: "<< std::setw(1)<<o.quality
          <<" pt: "<<std::setw(3)<<o.pt
          <<" charge:"<<charge
          <<" valCh: "<<std::setw(1)<<o.valCh
          <<" phi: "<<std::setw(6)<<o.phi
          <<" eta: "<<std::setw(6)<<o.eta
          <<" track: "<< std::bitset<29>(o.track) ;
      return out;
    }
  };

  Synch synch;
  std::vector<CandMuon> muons;

};

#endif
