import FWCore.ParameterSet.Config as cms
import os
import sys
import commands

process = cms.Process("Analysis")
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1))


import sys
samp = sys.argv[2] 
print 'getting sample', samp
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

sampleDict = {
    'SingleNu140' : '/eos/cms/store/group/phys_muon/sesanche/nov22/SingleNu140.root'   ,
    'SingleNu200' : '/eos/cms/store/group/phys_muon/sesanche/nov22/SingleNu200.root'   , 
    'SingleNuNoPU' : '/eos/cms/store/group/phys_muon/sesanche/nov22/SingleNuNoPU.root' , 
    'SingleMuNoPU' : '/eos/cms/store/group/phys_muon/sesanche/nov22/SingleMu_FlatPt-2to100_NoPile.root' , 
    'SingleMu140' : '/eos/cms/store/group/phys_muon/sesanche/nov22/SingleMu_FlatPt-2to100_PU140.root',
    'SingleMu200' : '/eos/cms/store/group/phys_muon/sesanche/nov22/SingleMu_FlatPt-2to100_PU200.root',
}




setattr(process, 'omtfAnalysis%s'%samp, cms.EDAnalyzer("OmtfTreeAnalysis",
                                                       histoFileName = cms.string("omtfAnalysis_%s.root"%samp),
  treeFileNames = cms.vstring( sampleDict[samp] ),
  filterByAnaEvent = cms.bool(True),
  anaEvent = cms.PSet(
        skipRuns = cms.vuint32(),
  ),

  anaSecMuSel = cms.PSet(
    triggMuon = cms.PSet (
##     requireCharge      = cms.int32(1),
      requireEtaSign     = cms.int32(-1),
##      requireOutsideOMTF  = cms.bool(True),
      requireTight        = cms.bool(True),
      requireUnique       = cms.bool(True),
      requireHLT          = cms.bool(True),
      requireIsoForHLTIso = cms.bool(True),
      minAcceptMuPtVal    = cms.double(20.),
      minMatchStations    = cms.uint32(2),
      maxMuEtaVal         = cms.double(2.4),
      minAcceptL1PtVal    = cms.double(18.),
      maxL1DeltaR         = cms.double(0.3),
      minAccepL1Q         = cms.int32(12), 
    ),
    probeMuon = cms.PSet (
##      requireCharge    = cms.int32(-1),
      requireEtaSign   = cms.int32( 1),
##      requireInsideOMTF = cms.bool(True),
      requireUnique     = cms.bool(True),
      requireLoose      = cms.bool(True),
      requireMedium     = cms.bool(True),
      requireTight      = cms.bool(False),
      requireTkIso      = cms.bool(False),
      requirePfIso      = cms.bool(False),
      minTrgMuDeltaR    = cms.double(1.0),
      maxMuEtaVal       = cms.double(2.4),
    ),
  ), 

  filterByAnaMuonDistribution = cms.bool(False),
  anaMuonDistribution = cms.PSet (
    requireUnique  = cms.bool(True),
    requireGlobal  = cms.bool(True),
    requireInner   = cms.bool(False),
    requireOuter   = cms.bool(False),
    requireLoose   = cms.bool(True),
    requireMedium  = cms.bool(True),
    requireTight   = cms.bool(False),
    requireTkIso   = cms.bool(False),
    requirePFIso   = cms.bool(False),
    chi2Norm  = cms.double(2.),
    ptMin     = cms.double(4.0),
    etaMax    = cms.double(2.4),
    minNumberOfMatchedStations = cms.uint32(2),
    minNumberTkHits = cms.uint32(6),
    minNumberRpcHits = cms.uint32(0),
    minNumberDtCscHits = cms.uint32(0),
    minNumberRpcDtCscHits = cms.uint32(2),
  ),

  filterByAnaMenu = cms.bool(False),
  anaMenu = cms.PSet( #OR of conditions for L1 and separately for HLT
    acceptL1_OtherThanMu             = cms.bool(True),
    acceptL1_Mu                      = cms.bool(True),
    acceptL1_Names                   = cms.vstring("L1_ZeroBias","L1_ZeroBias_copy",
                                                   "L1_IsolatedBunch","L1_FirstCollisionInOrbit"),
    acceptHLT_OtherThanMuPhysics     = cms.bool(False),
    acceptHLT_L1                     = cms.bool(False),
    acceptHLT_Physics                = cms.bool(False),
    acceptHLT_Mu                     = cms.bool(False),
    acceptHLT_ZeroBias               = cms.bool(True),
    acceptHLT_Names                  = cms.vstring(
        "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ",
        ),
  ),

  anaDataEmul =  cms.PSet(bxMin=cms.int32(-3),bxMax=cms.int32(4)),
  anaEff =  cms.PSet(),     
  anaGenEff = cms.PSet(),                                  
  anaTime = cms.PSet(),
  anaSynch = cms.PSet(),
  anaDiMu = cms.PSet(),
)
)



process.p = cms.Path(getattr(process,'omtfAnalysis%s'%samp))
