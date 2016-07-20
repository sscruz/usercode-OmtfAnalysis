import FWCore.ParameterSet.Config as cms
import commands
import os

#from Configuration.StandardSequences.Eras import eras
#process = cms.Process('OmtfTree',eras.Run2_2016)

process = cms.Process('OmtfTree')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#
# For processing single files insert lines with 'file:/PATH/FILE.root'
# alernatively one can use 'root://xrootd.unl.edu//store/express/Run2015A.....root'
# or                       '/store/express/Run2015A/ExpressPhysics/FEVT/...root'
# (there is 255 file limit though). Can be empty for crab.
#
process.source = cms.Source("PoolSource", 
fileNames = cms.untracked.vstring(
#  'root://cms-xrd-global.cern.ch//store/data/Run2016D/SingleMuon/RAW-RECO/ZMu-PromptReco-v2/000/276/811/00000/44797745-794C-E611-A7F9-02163E011CC9.root',
#  'file:/afs/cern.ch/work/k/konec/data/runs/run276811-SM_ZMu_Prompt-300_44797745-794C-E611-A7F9-02163E011CC9.root',
#  'file:/afs/cern.ch/work/k/konec/data/runs/run275963-Express-150_24AE8B98-743E-E611-ABC2-02163E0138E3.root',
   'file:/afs/cern.ch/work/k/konec/data/runs/run276870-Express-500_F8CB4B4B-6C4B-E611-ADEA-02163E01429D.root',
#'/store/data/Run2016C/ZeroBias/RECO/PromptReco-v2/000/276/097/00000/54FC3E14-0141-E611-882B-02163E014116.root'
#'/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/963/00000/846A13FA-733E-E611-BC5C-02163E011D84.root',
#'/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/963/00000/426B9BF4-733E-E611-BD17-02163E011B2D.root',
#'file:/afs/cern.ch/work/k/konec/data/runs/run275963-Express-150_24AE8B98-743E-E611-ABC2-02163E0138E3.root', 
#'/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/963/00000/60BF23FA-733E-E611-9DB7-02163E0126B2.root',
#'/store/express/Run2016C/ExpressPhysics/FEVT/Express-v2/000/275/963/00000/76B0FAEE-733E-E611-9E7F-02163E0119E8.root',
                                  ),
#skipEvents =  cms.untracked.uint32(1037)
)

#
# import of standard configurations
#
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
#process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('EventFilter.L1TRawToDigi.bmtfDigis_cfi')
process.load('EventFilter.L1TRawToDigi.emtfStage2Digis_cfi')
process.load('EventFilter.L1TRawToDigi.gmtStage2Digis_cfi')
process.load('EventFilter.L1TXRawToDigi.twinMuxStage2Digis_cfi')

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
#from Configuration.AlCa.GlobalTag import GlobalTag
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')



#
# message logger
#
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.debugModules.append('rpcunpacker')
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.MessageLogger.suppressWarning  = cms.untracked.vstring('Geometry', 'AfterSource','L1T','L1GlobalTriggerRawToDigi')
process.options = cms.untracked.PSet( wantSummary=cms.untracked.bool(False))



#OMTF ESProducer. Fills CondFormats from XML files.
process.omtfParamsSource = cms.ESSource( "EmptyESSource",
     recordName = cms.string('L1TMuonOverlapParamsRcd'),
    iovIsRunNotTime = cms.bool(True),
    firstValid = cms.vuint32(1)
)

process.omtfParams = cms.ESProducer( "L1TMuonOverlapParamsESProducer",
     patternsXMLFiles = cms.VPSet( cms.PSet(patternsXMLFile = cms.FileInPath("L1Trigger/L1TMuon/data/omtf_config/Patterns_0x0003.xml")),),
     configXMLFile = cms.FileInPath("L1Trigger/L1TMuon/data/omtf_config/hwToLogicLayer_0x0003.xml"),
)

###OMTF emulator configuration
process.omtfEmulator = cms.EDProducer("L1TMuonOverlapTrackProducer",
#  srcDTPh =  cms.InputTag('simTwinMuxDigis'),
#  srcDTTh =  cms.InputTag('simTwinMuxDigis'),
#  srcDTPh =  cms.InputTag('twinMuxStage2Digis'),
#  srcDTTh =  cms.InputTag('twinMuxStage2Digis'),
  srcDTPh =  cms.InputTag('bmtfDigis'),
  srcDTTh =  cms.InputTag('bmtfDigis'),
#  srcDTPh =  cms.InputTag('bmtfDigis:PhiDigis'),
#  srcDTTh =  cms.InputTag('bmtfDigis:TheDigis'),
#  srcDTPh = cms.InputTag('simDtTriggerPrimitiveDigis'),
#  srcDTTh = cms.InputTag('simDtTriggerPrimitiveDigis'),
   srcCSC = cms.InputTag('csctfDigis'),
#  srcCSC = cms.InputTag('emtfStage2Digis'),
#  srcCSC = cms.InputTag('muonCSCDigis','MuonCSCCorrelatedLCTDigi'),
#  srcCSC = cms.InputTag('simCscTriggerPrimitiveDigis','MPCSORTED'),
  srcRPC = cms.InputTag('muonRPCDigis'),
#  srcRPC = cms.InputTag('simMuonRPCDigis'),
#  dumpResultToXML = cms.bool(True),
  dumpResultToXML = cms.bool(False),
  dumpDetailedResultToXML = cms.bool(False),
  XMLDumpFileName = cms.string("TestEvents.xml"),
  dumpGPToXML = cms.bool(True),
  readEventsFromXML = cms.bool(False),
  eventsXMLFiles = cms.vstring("TestEvents.xml"),
  dropRPCPrimitives = cms.bool(False),
  dropDTPrimitives = cms.bool(False),
  dropCSCPrimitives = cms.bool(False),
)

process.raw2digi_step = cms.Path(process.muonRPCDigis+process.csctfDigis+process.bmtfDigis+process.emtfStage2Digis+process.twinMuxStage2Digis+process.gmtStage2Digis)
process.omtf_step = cms.Path(process.omtfEmulator)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.schedule = cms.Schedule(process.raw2digi_step, process.omtf_step, process.endjob_step)

#process.raw2digi_step = cms.Path(process.bmtfDigis+process.emtfStage2Digis+process.gmtStage2Digis)
#process.endjob_step = cms.EndPath(process.endOfProcess)
#process.schedule = cms.Schedule(process.raw2digi_step, process.endjob_step)

#
# re-emulate L1T
#
#from L1Trigger.Configuration.customiseReEmul import L1TReEmulFromRAW
#process = L1TReEmulFromRAW(process)
#process.L1TReEmul.remove(process.simCaloStage2Digis)
#process.L1TReEmul.remove(process.simCaloStage2Layer1Digis)
#process.L1TReEmul.remove(process.simGmtCaloSumDigis)
#process.L1TReEmul.remove(process.simGmtStage2Digis)
#process.L1TReEmul.remove(process.simEcalTriggerPrimitiveDigis)

#process.simOmtfDigis.srcDTPh = cms.InputTag('BMTFStage2Digis')
#process.simOmtfDigis.srcDTTh = cms.InputTag('BMTFStage2Digis')
#process.simOmtfDigis.dumpResultToXML = cms.bool(True)

process.omtfTree = cms.EDAnalyzer("OmtfTreeMaker",
  histoFileName = cms.string("omtfHelper.root"),
  treeFileName = cms.string("omtfTree.root"),

  menuInspector = cms.PSet(),
  
  l1ObjMaker = cms.PSet(
    omtfEmulSrc = cms.InputTag('omtfEmulator','OMTF'),
    omtfDataSrc = cms.InputTag('gmtStage2Digis','OMTF'),
    emtfDataSrc = cms.InputTag('gmtStage2Digis','EMTF'),
    bmtfDataSrc = cms.InputTag('gmtStage2Digis','BMTF'),
     gmtDataSrc = cms.InputTag('gmtStage2Digis','Muon'),
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
    checkChi2NotLoose = cms.bool(True),
    maxChi2Mu = cms.double(2.),
    maxChi2Sa = cms.double(2.),
    maxChi2Tk = cms.double(2.),
    minNumberTrackerHits = cms.int32(6),
    minNumberRpcHits = cms.int32(0),
    minNumberDtCscHits = cms.int32(0),
    minNumberOfMatchedStations = cms.int32(0),
    deltaPhiUnique = cms.double(1.0),
    deltaEtaUnique = cms.double(0.5),
    minPtUnique = cms.double(2.0),
    looseUnique = cms.bool(True)
  ),
)

process.OmtfTree = cms.Path(process.omtfTree)
process.schedule.append(process.OmtfTree)

#print process.dumpPython();
print process.schedule



