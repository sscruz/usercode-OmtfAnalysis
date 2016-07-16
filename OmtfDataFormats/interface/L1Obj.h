#ifndef L1Obj_H
#define L1Obj_H
#include "TObject.h"
#include <iostream>
#include <math.h>

class L1Obj : public TObject {

public:
  
  enum TYPE { NONE, RPCb, RPCf, DT, CSC, GMT, RPCb_emu, RPCf_emu, GMT_emu, OMTF, OMTF_emu, BMTF, EMTF };

  int pt, eta, phi;
  int disc;
  int   bx, q, hits, charge, refLayer;
  TYPE  type;
  int   iProcessor, position;

  L1Obj();

  bool isValid() const { return type!=NONE && pt>0;}
  operator bool() const { return isValid(); }

  double ptValue() const { return (pt-1.)/2.; }
  double etaValue() const { return eta/240.*2.26; }
  double phiValue() const {
    if (type==OMTF || type==OMTF_emu || type==EMTF) 
    return ( (15.+iProcessor*60.)/360. + phi/576. ) *2*M_PI;  
    else if (type==BMTF) return ( (-15.+iProcessor*30.)/360. + phi/576. ) *2*M_PI;
    else return 9999.;
  }
  int chargeValue() const { return pow(-1,charge); }

  ClassDef(L1Obj,4)
};


std::ostream & operator<< (std::ostream &out, const L1Obj &o);

#endif
