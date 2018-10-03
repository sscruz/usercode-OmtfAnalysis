import FWCore.ParameterSet.Config as cms
import commands
import os

#from Configuration.StandardSequences.Eras import eras
#process = cms.Process('OmtfTree',eras.Run2_2016)

process = cms.Process('OmtfTree')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

#
# For processing single files insert lines with 'file:/PATH/FILE.root'
# alernatively one can use 'root://xrootd.unl.edu//store/express/Run2015A.....root'
# or                       '/store/express/Run2015A/ExpressPhysics/FEVT/...root'
# (there is 255 file limit though). Can be empty for crab.
#
process.source = cms.Source("PoolSource", 
fileNames = cms.untracked.vstring(
# 'file:/afs/cern.ch/work/k/konec/data/runs/run279931-Express-2000_82DB28E0-7171-E611-A67B-FA163EE0E3A6.root',
#'root://cms-xrd-global.cern.ch//store/express/Run2017B/ExpressPhysics/FEVT/Express-v2/000/298/853/00000/08861D5E-C966-E711-9E99-02163E019DA2.root'
#'/store/data/Run2017F/SingleMuon/RAW-RECO/ZMu-17Nov2017-v1/70002/BEB00326-8EE0-E711-BCEE-FA163E8B70D3.root',
#'/store/data/Run2017F/SingleMuon/RAW-RECO/ZMu-PromptReco-v1/000/306/155/00000/860779D7-FFC4-E711-962F-FA163ECB69D1.root'
#'/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/321/010/00000/0601C5EA-FF9A-E811-A8BA-FA163EB3A544.root',
#'/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/321/010/00000/06921998-039B-E811-AE79-FA163E400AA8.root' 
#'/store/express/Run2018D/ExpressPhysics/FEVT/Express-v1/000/321/879/00000/006B4D10-3EAA-E811-AEB0-FA163E6EDA8C.root'
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/321/887/00000/6C133D73-73AE-E811-8244-FA163E6C4BEA.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/321/908/00000/16F0E3D6-96AE-E811-BEF2-FA163E3AE75B.root'
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/321/908/00000/22972439-96AE-E811-88C2-FA163EF09AB5.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/321/908/00000/869A40B6-86AE-E811-8BD9-02163E013AC9.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/321/908/00000/9A7467B6-99AE-E811-8847-FA163EDE9AD6.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/321/908/00000/5E9D84C4-95AE-E811-B271-FA163E13E714.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/321/887/00000/F666B1E2-75AE-E811-A074-FA163EB1A399.root'
#
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/321/909/00000/7E72017E-9CAE-E811-910A-FA163EAE6D9D.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/321/909/00000/5022FC62-9AAE-E811-9A2C-FA163E7C6CDD.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/321/909/00000/E0368CBD-97AE-E811-A96A-FA163E72908D.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/321/909/00000/441CE6FB-92AE-E811-9C72-FA163E4CBFDB.root',

#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/322/106/00000/9E0C65F7-BFB2-E811-A781-FA163E76167C.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/322/106/00000/8E0346B2-B5B2-E811-8E3F-FA163EA1D1FE.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/322/106/00000/7C6BA8C5-C1B2-E811-ACC1-FA163E9D9A3E.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/322/106/00000/0ADE0F3E-BEB2-E811-A837-02163E00AE1A.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/322/106/00000/EA5DCF08-C0B2-E811-94FE-FA163E4C08DA.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/322/106/00000/2038CFE2-BCB2-E811-B9D2-FA163E79FE08.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/322/106/00000/70C8E558-B4B2-E811-9D82-02163E010BF2.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/322/106/00000/FA0FC587-BBB2-E811-AC15-FA163E3593E9.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/322/106/00000/F62761E9-BDB2-E811-A8C8-FA163E82E6AC.root',
#'root://eoscms.cern.ch//eos/cms/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/322/106/00000/20D62E93-BEB2-E811-B27D-02163E01A109.root',
 '/store/data/Run2018D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/321/909/00000/F076FDEF-99AE-E811-B313-FA163EDAE846.root'
                                  ),
#skipEvents =  cms.untracked.uint32(504)
#skipEvents =  cms.untracked.uint32(267)
#skipEvents =  cms.untracked.uint32(547)
)

#
# import of standard configurations
#
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
#process.load('Configuration.EventContent.EventContent_cff')
#process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
#process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('EventFilter.L1TRawToDigi.bmtfDigis_cfi')
process.load('EventFilter.L1TRawToDigi.emtfStage2Digis_cfi')
process.load('EventFilter.L1TRawToDigi.gmtStage2Digis_cfi')
process.load('EventFilter.L1TXRawToDigi.twinMuxStage2Digis_cfi')
process.load('EventFilter.L1TRawToDigi.omtfStage2Digis_cfi')
#process.load('EventFilter.L1TRawToDigi.omtfStage2Raw_cfi')
#process.load('EventFilter.L1TRawToDigi.caloLayer1Digis_cfi')
process.load('EventFilter.L1TRawToDigi.caloStage2Digis_cfi')
#process.load("CondTools/RPC/RPCLinkMap_sqlite_cff")


#
# set proper GlobalTag
#
#process.GlobalTag.globaltag = 'FT_R_53_V10::All' #rereco 2012ABC
#process.GlobalTag.globaltag = 'FT_R_53_V18::All' #rereco 2012ABC
#process.GlobalTag.globaltag = 'FT_R_53_V21::All' #rereco 2012D
#process.GlobalTag.globaltag  = 'GR_E_V46::All' #rereco
#process.GlobalTag.globaltag  = 'GR_P_V50::All' #rereco
#process.GlobalTag.globaltag  = 'GR_P_V54::All' #rereco
#process.GlobalTag.globaltag  = 'GR_P_V56::All' #rereco
#process.GlobalTag.globaltag  = '74X_dataRun2_Prompt_v0'
#process.GlobalTag.globaltag  = '74X_dataRun2_Express_v0'
#process.GlobalTag.globaltag  = 'auto:run2_data'
#process.GlobalTag.globaltag  = '80X_dataRun2_Prompt_v8'
from Configuration.AlCa.GlobalTag import GlobalTag
#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag.globaltag  = '92X_dataRun2_Prompt_v4'
#process.GlobalTag.globaltag = '100X_dataRun2_v1'
#process.GlobalTag.globaltag = '100X_dataRun2_Express_v3'
process.GlobalTag.globaltag = '101X_dataRun2_Express_v8'

#
# message logger
#
process.load('FWCore.MessageService.MessageLogger_cfi')
#process.MessageLogger.cerr.threshold = cms.untracked.string('DEBUG')
#process.MessageLogger.debugModules.append('muonRPCDigis')
#process.MessageLogger.debugModules.append('omtfStage2Digis')
#process.MessageLogger.debugModules.append('omtfStage2Raw')
#process.MessageLogger.debugModules.append('omtfStage2Digis2')
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.MessageLogger.suppressWarning  = cms.untracked.vstring('Geometry', 'AfterSource','L1T','L1GlobalTriggerRawToDigi')
process.options = cms.untracked.PSet( wantSummary=cms.untracked.bool(False))

process.digiComapre = cms.EDAnalyzer("OmtfDigiCompare",
  srcRPC_OMTF = cms.InputTag('omtfStage2Digis'),
#  srcRPC_OMTF = cms.InputTag('omtfStage2Digis2','OmtfUnpacker2'),
  srcRPC_PACT = cms.InputTag('muonRPCDigis'),

  srcCSC_OMTF = cms.InputTag('omtfStage2Digis'),
#  srcCSC_OMTF  = cms.InputTag('omtfStage2Digis2','OmtfUnpacker2'),
#  srcCSC_CSC = cms.InputTag('csctfDigis'),
  srcCSC_CSC = cms.InputTag('emtfStage2Digis'),

  srcOMTF_DATA = cms.InputTag('omtfStage2Digis'),
  srcOMTF_EMUL = cms.InputTag('gmtStage2Digis','OMTF'),
#  srcOMTF_EMUL = cms.InputTag('omtfEmulator','OMTF'),
#
#  srcDTPh_BMTF = cms.InputTag('bmtfDigis'),
#  srcDTTh_BMTF = cms.InputTag('bmtfDigis'),
  srcDTPh_BMTF = cms.InputTag('twinMuxStage2Digis','PhIn'),
  srcDTTh_BMTF = cms.InputTag('twinMuxStage2Digis','ThIn'),
  srcDTPh_OMTF = cms.InputTag('omtfStage2Digis'),
  srcDTTh_OMTF = cms.InputTag('omtfStage2Digis'),
#  srcDTPh_OMTF = cms.InputTag('omtfStage2Digis2','OmtfUnpacker2'),
#  srcDTTh_OMTF = cms.InputTag('omtfStage2Digis2','OmtfUnpacker2'),
)

process.omtfStage2Raw = cms.EDProducer("OmtfPacker",
  rpcInputLabel = cms.InputTag('omtfStage2Digis'),
  cscInputLabel = cms.InputTag('omtfStage2Digis'),
  dtPhInputLabel = cms.InputTag('omtfStage2Digis'),
  dtThInputLabel = cms.InputTag('omtfStage2Digis'),
)

process.omtfStage2Digis2 = cms.EDProducer("OmtfUnpacker",
  inputLabel = cms.InputTag('omtfStage2Raw'),
  useRpcConnectionFile = cms.bool(True),
  rpcConnectionFile = cms.string("CondTools/RPC/data/RPCOMTFLinkMapInput.txt"),
  outputTag = cms.string("OmtfUnpacker2"),
)

#
###OMTF emulator configuration
#
#OMTF ESProducer. Fills CondFormats from XML files.
process.omtfParamsSource = cms.ESSource( "EmptyESSource",
     recordName = cms.string('L1TMuonOverlapParamsRcd'),
    iovIsRunNotTime = cms.bool(True),
    firstValid = cms.vuint32(1)
)
process.omtfParams = cms.ESProducer( "L1TMuonOverlapParamsESProducer",
     patternsXMLFiles = cms.VPSet( cms.PSet(patternsXMLFile = cms.FileInPath("L1Trigger/L1TMuon/data/omtf_config/Patterns_0x0003.xml")),),
     configXMLFile = cms.FileInPath("L1Trigger/L1TMuon/data/omtf_config/hwToLogicLayer_0x0005.xml"),
)

import L1Trigger.L1TMuonOverlap.simOmtfDigis_cfi
process.omtfEmulator=L1Trigger.L1TMuonOverlap.simOmtfDigis_cfi.simOmtfDigis.clone() 
process.omtfEmulator.srcDTPh = cms.InputTag('omtfStage2Digis')
process.omtfEmulator.srcDTTh = cms.InputTag('omtfStage2Digis')
process.omtfEmulator.srcCSC = cms.InputTag('omtfStage2Digis')
process.omtfEmulator.srcRPC = cms.InputTag('omtfStage2Digis')
#process.omtfEmulator.dropRPCPrimitives = cms.bool(False)
#process.omtfEmulator.dropDTPrimitives = cms.bool(False)
#process.omtfEmulator.dropCSCPrimitives = cms.bool(False)
process.omtfEmulator.dumpResultToXML = cms.bool(True)
process.omtfEmulator.bxMin = cms.int32(-3)
process.omtfEmulator.bxMax = cms.int32(4)

#process.omtfEmulator = cms.EDProducer("L1TMuonOverlapTrackProducer",
###  srcDTPh = cms.InputTag('simDtTriggerPrimitiveDigis'),
###  srcDTTh = cms.InputTag('simDtTriggerPrimitiveDigis'),
###  srcDTPh =  cms.InputTag('simTwinMuxDigis'),
###  srcDTTh =  cms.InputTag('simTwinMuxDigis'),
###  srcDTPh =  cms.InputTag('twinMuxStage2Digis','PhIn'),
###  srcDTTh =  cms.InputTag('twinMuxStage2Digis','ThIn'),
###  srcDTPh =  cms.InputTag('bmtfDigis'),
###  srcDTTh =  cms.InputTag('bmtfDigis'),
#   srcDTPh = cms.InputTag('omtfStage2Digis'),
#   srcDTTh = cms.InputTag('omtfStage2Digis'),
###  srcCSC = cms.InputTag('csctfDigis'),
#   srcCSC = cms.InputTag('omtfStage2Digis'),
###  srcCSC = cms.InputTag('emtfStage2Digis'),
###  srcCSC = cms.InputTag('muonCSCDigis','MuonCSCCorrelatedLCTDigi'),
###  srcCSC = cms.InputTag('simCscTriggerPrimitiveDigis','MPCSORTED'),
###  srcRPC = cms.InputTag('simMuonRPCDigis'),
###  srcRPC = cms.InputTag('muonRPCDigis'),
#   srcRPC = cms.InputTag('omtfStage2Digis'),
##  dumpResultToXML = cms.bool(True),
#   dumpResultToXML = cms.bool(False),
#   dumpDetailedResultToXML = cms.bool(False),
#   XMLDumpFileName = cms.string("TestEvents.xml"),
###  dumpGPToXML = cms.bool(True),
###  readEventsFromXML = cms.bool(False),
###  eventsXMLFiles = cms.vstring("TestEvents.xml"),
#   dropRPCPrimitives = cms.bool(False),
#   dropDTPrimitives = cms.bool(False),
#   dropCSCPrimitives = cms.bool(False),
#   bxMin = cms.int32(-3),
#   bxMax = cms.int32(4)
#)

#
# reemulate GMT, with changed OMTF
#
process.emulGmtCaloSumDigis = cms.EDProducer('L1TMuonCaloSumProducer',
    caloStage2Layer2Label = cms.InputTag("caloStage2Digis",'CaloTower'),
)
process.emulGmtStage2Digis = cms.EDProducer('L1TMuonProducer',
    barrelTFInput  = cms.InputTag("gmtStage2Digis", "BMTF"),
    overlapTFInput = cms.InputTag("omtfEmulator", "OMTF"),
#    overlapTFInput = cms.InputTag("gmtStage2Digis", "OMTF"),
    forwardTFInput = cms.InputTag("gmtStage2Digis", "EMTF"),
    #triggerTowerInput = cms.InputTag("simGmtCaloSumDigis", "TriggerTower2x2s"),
    triggerTowerInput = cms.InputTag("emulGmtCaloSumDigis", "TriggerTowerSums"),
    autoBxRange = cms.bool(True), # if True the output BX range is calculated from the inputs and 'bxMin' and 'bxMax' are ignored
    bxMin = cms.int32(-3),
    bxMax = cms.int32(4),
    autoCancelMode = cms.bool(False), # if True the cancel out methods are configured depending on the FW version number and 'emtfCancelMode' is ignored
    emtfCancelMode = cms.string("coordinate") # 'tracks' or 'coordinate'
)


process.raw2digi_step = cms.Path(process.muonRPCDigis+process.csctfDigis+process.bmtfDigis+process.emtfStage2Digis+process.twinMuxStage2Digis+process.gmtStage2Digis+process.caloStage2Digis)
#process.raw2digi_step = cms.Path(process.muonRPCDigis)
process.omtf_step = cms.Path(process.omtfStage2Digis+process.omtfEmulator+process.emulGmtCaloSumDigis+process.emulGmtStage2Digis)
#process.omtf_step = cms.Path(process.omtfStage2Digis+process.omtfStage2Raw+process.omtfStage2Digis2+process.digiComapre+process.omtfEmulator)
#process.omtf_step = cms.Path(process.omtfStage2Digis+process.omtfStage2Raw+process.omtfStage2Digis2)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.schedule = cms.Schedule(process.raw2digi_step, process.omtf_step, process.endjob_step)

#
# OMTF tree 
#
process.omtfTree = cms.EDAnalyzer("OmtfTreeMaker",
  histoFileName = cms.string("omtfHelper.root"),
  treeFileName = cms.string("omtfTree.root"),

  menuInspector = cms.PSet( 
#    namesCheckHltMuMatch = cms.vstring(
#      "HLT_IsoMu22_v","HLT_IsoTkMu22_v","HLT_Mu50_v","HLT_TkMu50_v","HLT_Mu45_eta2p1_v",
#      "HLT_IsoMu22_eta2p1_v", "HLT_IsoMu24_v", "HLT_IsoMu27_v",
#      "HLT_IsoTkMu22_eta2p1_v", "HLT_IsoTkMu24_v", "HLT_IsoTkMu27_v",
#      "HLT_Mu55_v", "HLT_IsoMu24_eta2p1_v", "HLT_IsoTkMu24_eta2p1_v"
#    ),
    namesCheckHltMuMatch = cms.vstring(
      "HLT_IsoMu20_v","HLT_IsoMu24_v","HLT_IsoMu27_v"
    ),
  ),

   linkSynchroGrabber = cms.PSet(
     rawSynchroTag = cms.InputTag("muonRPCDigis"),
     writeHistograms = cms.untracked.bool(True),
     deltaR_MuonToDetUnit_cutoff = cms.double(0.3),
     checkInside = cms.bool(True),
     linkMonitorPSet = cms.PSet(
       useFirstHitOnly = cms.untracked.bool(True),
       dumpDelays = cms.untracked.bool(True) # set to True for LB delay plots
     ),
     synchroSelector = cms.PSet(
       checkRpcDetMatching_minPropagationQuality = cms.int32(0),
       checkRpcDetMatching_matchingScaleValue = cms.double(3),
       checkRpcDetMatching_matchingScaleAuto  = cms.bool(True),
       checkUniqueRecHitMatching_maxPull = cms.double(2.),
       checkUniqueRecHitMatching_maxDist = cms.double(5.)
     )
   ),
  
  l1ObjMaker = cms.PSet(
    omtfEmulSrc = cms.InputTag('omtfEmulator','OMTF'),
    omtfDataSrc = cms.InputTag('omtfStage2Digis'),
#   omtfDataSrc = cms.InputTag('gmtStage2Digis','OMTF'),
    emtfDataSrc = cms.InputTag('gmtStage2Digis','EMTF'),
    bmtfDataSrc = cms.InputTag('gmtStage2Digis','BMTF'),
    gmtDataSrc = cms.InputTag('gmtStage2Digis','Muon'),
    gmtEmulSrc = cms.InputTag('emulGmtStage2Digis',''),
  ),
  genObjectFinder = cms.PSet(
    genColl = cms.InputTag("genParticles")
  ),                                 
  closestTrackFinder = cms.PSet(
    trackColl = cms.InputTag("generalTracks")
  ),

  onlyBestMuEvents = cms.bool(False),
  bestMuonFinder = cms.PSet(
    muonColl = cms.InputTag("muons"),
    beamSpot = cms.InputTag("offlineBeamSpot"),
    requireInnerTrack = cms.bool(True),
    requireOuterTrack = cms.bool(False),
    requireGlobalTrack = cms.bool(False),
    requireLoose       = cms.bool(True),
    minPt = cms.double(3.),
    maxTIP = cms.double(0.2),
    maxAbsEta = cms.double(2.4),
    minNumberTrackerHits = cms.int32(6),
    minNumberRpcHits = cms.int32(0),
    minNumberDtCscHits = cms.int32(0),
    minNumberOfMatchedStations = cms.int32(0),
    cutTkIsoRel = cms.double(0.1),
    cutPFIsoRel = cms.double(0.15),
    deltaPhiUnique = cms.double(1.0),
    deltaEtaUnique = cms.double(0.5),
    minPtUnique = cms.double(2.0),
    looseUnique = cms.bool(True)
  ),
)

#
# refit Muon
#
process.load("TrackingTools.RecoGeometry.RecoGeometries_cff")
process.load("TrackingTools.TrackRefitter.TracksToTrajectories_cff")
#process.load("TrackingTools.TrackRefitter.globalMuonTrajectories_cff")
import TrackingTools.TrackRefitter.globalMuonTrajectories_cff
process.refittedMuons = TrackingTools.TrackRefitter.globalMuonTrajectories_cff.globalMuons.clone()

process.OmtfTree = cms.Path(process.refittedMuons*process.omtfTree)
process.schedule.append(process.OmtfTree)

#print process.dumpPython();
print process.schedule

