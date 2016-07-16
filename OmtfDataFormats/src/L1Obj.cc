#include "UserCode/OmtfDataFormats/interface/L1Obj.h"
#include <bitset>


L1Obj::L1Obj() : pt(0),eta(0),phi(0),disc(0), bx(0),q(0), hits(0), charge(0), type(NONE) {};

std::ostream & operator<< (std::ostream &out, const L1Obj &o)
{
  out<<"L1Obj: ";
  switch (o.type) {
    case L1Obj::RPCb:     { out <<"RPCb    "; break; }
    case L1Obj::RPCf:     { out <<"RPCf    "; break; }
    case L1Obj::DT:       { out <<"DT      "; break; }
    case L1Obj::CSC:      { out <<"CSC     "; break; }
    case L1Obj::GMT:      { out <<"GMT     "; break; }
    case L1Obj::RPCb_emu: { out <<"RPCb_emu"; break; }
    case L1Obj::RPCf_emu: { out <<"RPCf_emu"; break; }
    case L1Obj::GMT_emu:  { out <<"GMT_emu "; break; }
    case L1Obj::OMTF   :  { out <<"OMTF    "; break; }
    case L1Obj::OMTF_emu: { out <<"OMTF_emu"; break; }
    case L1Obj::BMTF   :  { out <<"BMTF    "; break; }
    case L1Obj::EMTF   :  { out <<"EMTF    "; break; }
    case L1Obj::NONE   :  { out <<"NONE    "; break; }
    default: out <<"Unknown";
  };
  out <<" ip: " << o.iProcessor << "  pt: ";
  if (o.charge==1) out <<"-"; else if (o.charge==0) out <<"+";
  out <<o.pt<<", eta: "<<o.eta<<", phi: "<<o.phi<<", q: "<<o.q<<", bx: "<<o.bx
      <<" track: "<< std::bitset<29>(o.hits);
  return out;
}


ClassImp(L1Obj)
