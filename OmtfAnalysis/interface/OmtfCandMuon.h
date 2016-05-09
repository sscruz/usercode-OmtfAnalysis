#ifndef UserCode_OmtfAnalysis_OmtfCandMuon_H
#define UserCode_OmtfAnalysis_OmtfCandMuon_H

#include <ostream>

struct OmtfCandMuon {
    uint32_t pt      : BITS_PT;
    uint32_t quality : BITS_QUA;
    signed int  eta  : BITS_ETA;
    uint32_t halo    : BITS_HALO;
    signed int phi   : BITS_PHI;
    uint32_t charge  : BITS_CHARGE;
    uint32_t valCh   : BITS_VALID;
    uint32_t track   : BITS_TRACK;
    friend std::ostream & operator<< (std::ostream &out, const OmtfCandMuon &o) {
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
#endif
