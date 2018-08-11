import FWCore.ParameterSet.Config as cms
import commands
import os

#from Configuration.StandardSequences.Eras import eras
#process = cms.Process('OmtfTree',eras.Run2_2016)

process = cms.Process('OmtfTree')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

#
# For processing single files insert lines with 'file:/PATH/FILE.root'
# alernatively one can use 'root://xrootd.unl.edu//store/express/Run2015A.....root'
# or                       '/store/express/Run2015A/ExpressPhysics/FEVT/...root'
# (there is 255 file limit though). Can be empty for crab.
#
process.source = cms.Source("PoolSource", 
fileNames = cms.untracked.vstring(
# 'file:/afs/cern.ch/work/k/konec/data/runs/run279931-Express-2000_82DB28E0-7171-E611-A67B-FA163EE0E3A6.root',
# 'file:toto.root',
#'/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/278/017/00000/B69D41C7-4E58-E611-AC3F-02163E01412A.root',
#'/store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/277/982/00000/0627D80B-EB57-E611-9FC6-02163E011B6F.root',
#  'root://cms-xrd-global.cern.ch//store/express/Run2016F/ExpressPhysics/FEVT/Express-v1/000/277/981/00000/B6751BB0-EB57-E611-8249-FA163E180514.root',
#  'root://cms-xrd-global.cern.ch//store/data/Run2016D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/276/811/00000/44797745-794C-E611-A7F9-02163E011CC9.root',
#  'file:/afs/cern.ch/work/k/konec/data/runs/run276811-SM_ZMu_Prompt-300_44797745-794C-E611-A7F9-02163E011CC9.root',
#  'file:/afs/cern.ch/work/k/konec/data/runs/run275963-Express-150_24AE8B98-743E-E611-ABC2-02163E0138E3.root',
#   'file:/afs/cern.ch/work/k/konec/data/runs/run276870-Express-500_F8CB4B4B-6C4B-E611-ADEA-02163E01429D.root',
#'/store/data/Run2016C/ZeroBias/RECO/PromptReco-v2/000/276/097/00000/54FC3E14-0141-E611-882B-02163E014116.root'
#'/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/963/00000/846A13FA-733E-E611-BC5C-02163E011D84.root',
#'/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/963/00000/426B9BF4-733E-E611-BD17-02163E011B2D.root',
#'file:/afs/cern.ch/work/k/konec/data/runs/run275963-Express-150_24AE8B98-743E-E611-ABC2-02163E0138E3.root', 
#'/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/963/00000/60BF23FA-733E-E611-9DB7-02163E0126B2.root',
#'/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/963/00000/76B0FAEE-733E-E611-9E7F-02163E0119E8.root',
# 'file:/afs/cern.ch/work/k/konec/data/runs/run285090-ExpressPA-0347_043D8230-5EA7-E611-95E7-FA163E3BAEE0.root',
#'/store/express/PARun2016C/ExpressCosmics/FEVT/Express-v1/000/286/180/00000/5A22E4C8-D7B6-E611-AA6A-FA163EFEB709.root',
#'file:/afs/cern.ch/work/k/konec/data/runs/run284036-SM_ZMu_Prompt-14EE7A3D-639F-E611-81EE-02163E013441.root'
#'file:/afs/cern.ch/work/k/konec/data/runs/run286180-CosmicsPA-2481BCC7-D7B6-E611-9A30-FA163EB8B34E.root',
#'/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/085FEBB5-4645-E711-AB5E-02163E013902.root',
#'/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/08D7F942-4745-E711-86A8-02163E011825.root',
#'/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/08FA4A0C-4645-E711-B797-02163E01A295.root',
#'/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/0A59B107-4745-E711-B578-02163E01A46D.root',
#'/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/0C154D59-4645-E711-A5EB-02163E019C9F.root',
#'/store/express/Run2017A/ExpressPhysics/FEVT/Express-v1/000/295/606/00000/0CA922D7-4545-E711-9C67-02163E01A364.root',
#'file:/afs/cern.ch/work/k/konec/data/runs/run295606-Express-46C6640F-4745-E711-A67B-02163E0145A9.root',
#'root://cms-xrd-global.cern.ch//store/express/Run2017B/ExpressPhysics/FEVT/Express-v2/000/298/853/00000/08861D5E-C966-E711-9E99-02163E019DA2.root'
#'/store/express/Run2017B/ExpressCosmics/FEVT/Express-v2/000/299/188/00000/2C7573A8-586A-E711-84CA-02163E0128D1.root'
#'/store/express/Run2017B/ExpressCosmics/FEVT/Express-v2/000/299/189/00000/02B163AB-676A-E711-B742-02163E011E55.root'
#'/store/express/Run2017B/ExpressPhysics/FEVT/Express-v2/000/299/149/00000/00C34496-E669-E711-9E38-02163E01A3FB.root',
#'/store/express/Run2017B/ExpressPhysics/FEVT/Express-v2/000/299/149/00000/0006732A-E169-E711-8843-02163E0123FD.root',
#'/store/express/Run2017B/ExpressPhysics/FEVT/Express-v2/000/299/149/00000/0484DE01-CC69-E711-B649-02163E013816.root',
#'/store/express/Run2017B/ExpressPhysics/FEVT/Express-v1/000/297/435/00000/02CFFD58-DC58-E711-A2B4-02163E01A70A.root',
#'/store/express/Run2017B/ExpressPhysics/FEVT/Express-v1/000/297/435/00000/3212D13A-DC58-E711-A478-02163E019CC6.root',
#'/store/data/Run2017F/SingleMuon/RAW-RECO/ZMu-17Nov2017-v1/70002/BEB00326-8EE0-E711-BCEE-FA163E8B70D3.root'
#'/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/574/00000/0C7BF67A-B742-E811-A725-FA163ECF9759.root'
'/store/data/Run2017F/SingleMuon/RAW-RECO/ZMu-PromptReco-v1/000/306/155/00000/860779D7-FFC4-E711-962F-FA163ECB69D1.root'
#'/store/express/Run2018B/ExpressPhysics/FEVT/Express-v1/000/318/653/00000/D4D8D4AD-1279-E811-9A53-FA163EA563F7.root'
                                  ),
#skipEvents =  cms.untracked.uint32(29)
#skipEvents =  cms.untracked.uint32(264)
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
process.load('EventFilter.L1TRawToDigi.omtfStage2Raw_cfi')
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
process.GlobalTag.globaltag = '101X_dataRun2_Express_v7'

#
# message logger
#
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = cms.untracked.string('DEBUG')
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
     configXMLFile = cms.FileInPath("L1Trigger/L1TMuon/data/omtf_config/hwToLogicLayer_0x0004.xml"),
)
process.omtfEmulator = cms.EDProducer("L1TMuonOverlapTrackProducer",
#  srcDTPh = cms.InputTag('simDtTriggerPrimitiveDigis'),
#  srcDTTh = cms.InputTag('simDtTriggerPrimitiveDigis'),
#  srcDTPh =  cms.InputTag('simTwinMuxDigis'),
#  srcDTTh =  cms.InputTag('simTwinMuxDigis'),
#  srcDTPh =  cms.InputTag('twinMuxStage2Digis','PhIn'),
#  srcDTTh =  cms.InputTag('twinMuxStage2Digis','ThIn'),
#  srcDTPh =  cms.InputTag('bmtfDigis'),
#  srcDTTh =  cms.InputTag('bmtfDigis'),
   srcDTPh = cms.InputTag('omtfStage2Digis'),
   srcDTTh = cms.InputTag('omtfStage2Digis'),
#  srcCSC = cms.InputTag('csctfDigis'),
   srcCSC = cms.InputTag('omtfStage2Digis'),
#  srcCSC = cms.InputTag('emtfStage2Digis'),
#  srcCSC = cms.InputTag('muonCSCDigis','MuonCSCCorrelatedLCTDigi'),
#  srcCSC = cms.InputTag('simCscTriggerPrimitiveDigis','MPCSORTED'),
#  srcRPC = cms.InputTag('simMuonRPCDigis'),
#  srcRPC = cms.InputTag('muonRPCDigis'),
   srcRPC = cms.InputTag('omtfStage2Digis'),
#  dumpResultToXML = cms.bool(True),
  dumpResultToXML = cms.bool(False),
  dumpDetailedResultToXML = cms.bool(False),
  XMLDumpFileName = cms.string("TestEvents.xml"),
  dumpGPToXML = cms.bool(True),
  readEventsFromXML = cms.bool(False),
  eventsXMLFiles = cms.vstring("TestEvents.xml"),
  dropRPCPrimitives = cms.bool(False),
  dropDTPrimitives = cms.bool(False),
  dropCSCPrimitives = cms.bool(False)
)

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
    autoBxRange = cms.bool(False), # if True the output BX range is calculated from the inputs and 'bxMin' and 'bxMax' are ignored
    bxMin = cms.int32(-2),
    bxMax = cms.int32(2),
    autoCancelMode = cms.bool(True), # if True the cancel out methods are configured depending on the FW version number and 'emtfCancelMode' is ignored
    emtfCancelMode = cms.string("coordinate") # 'tracks' or 'coordinate'
)


process.raw2digi_step = cms.Path(process.muonRPCDigis+process.csctfDigis+process.bmtfDigis+process.emtfStage2Digis+process.twinMuxStage2Digis+process.gmtStage2Digis+process.caloStage2Digis)
#process.raw2digi_step = cms.Path()
process.omtf_step = cms.Path(process.omtfStage2Digis+process.omtfEmulator+process.emulGmtCaloSumDigis+process.emulGmtStage2Digis)
#process.omtf_step = cms.Path(process.omtfStage2Digis+process.digiComapre+process.omtfEmulator+process.omtfStage2Raw)
#process.omtf_step = cms.Path(process.omtfStage2Digis+process.omtfStage2Raw+process.omtfStage2Digis2+process.digiComapre+process.omtfEmulator)
#process.omtf_step = cms.Path(process.omtfStage2Digis+process.omtfStage2Raw+process.omtfStage2Digis2)
#process.omtf_step = cms.Path(process.omtfStage2Digis+process.digiComapre)
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
#process.OmtfTree = cms.Path(process.omtfTree)
process.schedule.append(process.OmtfTree)

#print process.dumpPython();
print process.schedule

