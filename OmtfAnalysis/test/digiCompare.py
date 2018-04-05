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
#  'file:/afs/cern.ch/work/k/konec/data/runs/run281214-Cosmics_E0D61D8B-E27F-E611-AF32-FA163E368DD1.root',
#   'file:/afs/cern.ch/work/k/konec/data/runs/run283999-Cosmics_14208324-0A9B-E611-B786-02163E01437C.root',
#'/store/express/PARun2016C/ExpressCosmics/FEVT/Express-v1/000/286/180/00000/5A22E4C8-D7B6-E611-AA6A-FA163EFEB709.root',
#'/store/express/PARun2016C/ExpressCosmics/FEVT/Express-v1/000/286/026/00000/02E95F89-29B5-E611-8A83-02163E0141AF.root',
#285915
#'/store/express/PARun2016C/ExpressCosmics/FEVT/Express-v1/000/285/915/00000/469656FF-7CB3-E611-9F00-FA163E0F1BFC.root',
#'/store/express/PARun2016C/ExpressCosmics/FEVT/Express-v1/000/285/915/00000/7274B4B6-7BB3-E611-8999-02163E0119D7.root',
#'/store/express/PARun2016C/ExpressCosmics/FEVT/Express-v1/000/285/915/00000/88F833C3-7DB3-E611-880B-02163E0145E5.root',
#'/store/express/PARun2016C/ExpressCosmics/FEVT/Express-v1/000/285/915/00000/B830A0AF-7BB3-E611-8CA0-FA163E99DA7B.root',
#'/store/express/PARun2016C/ExpressCosmics/FEVT/Express-v1/000/285/915/00000/E8BDEAFB-7CB3-E611-8790-FA163E6337E8.root',
#  'file:/afs/cern.ch/work/k/konec/data/runs/run285915-CosmicsPA-C057B6C6-7EB3-E611-8B51-02163E0133A2.root',
#'file:/afs/cern.ch/work/k/konec/data/runs/run286180-CosmicsPA-2481BCC7-D7B6-E611-9A30-FA163EB8B34E.root',
#'file:/afs/cern.ch/work/k/konec/data/runs/run299149-Express-0484DE01-CC69-E711-B649-02163E013816.root'
'root://eoscms.cern.ch//eos/cms/tier0/store/data/Run2017F/SingleMuon/RAW/v1/000/306/459/00000/C6F9F5A2-9DC5-E711-BF94-02163E01A609.root'
                                  ),
#skipEvents =  cms.untracked.uint32(66)
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
#from Configuration.AlCa.GlobalTag import GlobalTag
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag.globaltag = '100X_mcRun2_asymptotic_v2'
process.GlobalTag.globaltag = '100X_dataRun2_v1'

#
# message logger
#
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.MessageLogger.cerr.threshold = cms.untracked.string('DEBUG')
process.MessageLogger.debugModules = cms.untracked.vstring()
#process.MessageLogger.debugModules.append('muonRPCDigis')
#process.MessageLogger.debugModules.append('omtfStage2Digis')
process.options = cms.untracked.PSet( wantSummary=cms.untracked.bool(False))

process.omtfStage2Digis = cms.EDProducer("OmtfUnpacker",
  inputLabel = cms.InputTag('rawDataCollector'),
)

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
 
#  srcDTPh_BMTF = cms.InputTag('bmtfDigis'),
#  srcDTTh_BMTF = cms.InputTag('bmtfDigis'),
  srcDTPh_BMTF = cms.InputTag('twinMuxStage2Digis','PhIn'),
  srcDTTh_BMTF = cms.InputTag('twinMuxStage2Digis','ThIn'),
  srcDTPh_OMTF = cms.InputTag('omtfStage2Digis'),
  srcDTTh_OMTF = cms.InputTag('omtfStage2Digis'),
#  srcDTPh_OMTF = cms.InputTag('omtfStage2Digis2','OmtfUnpacker2'),
#  srcDTTh_OMTF = cms.InputTag('omtfStage2Digis2','OmtfUnpacker2'),
)

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

###OMTF emulator configuration
process.omtfEmulator = cms.EDProducer("L1TMuonOverlapTrackProducer",
#  srcDTPh =  cms.InputTag('simTwinMuxDigis'),
#  srcDTTh =  cms.InputTag('simTwinMuxDigis'),
#  srcDTPh =  cms.InputTag('twinMuxStage2Digis','PhIn'),
#  srcDTTh =  cms.InputTag('twinMuxStage2Digis','ThIn'),
#  srcDTPh =  cms.InputTag('bmtfDigis'),
#  srcDTTh =  cms.InputTag('bmtfDigis'),
#  srcDTPh =  cms.InputTag('bmtfDigis:PhiDigis'),
#  srcDTTh =  cms.InputTag('bmtfDigis:TheDigis'),
#  srcDTPh = cms.InputTag('simDtTriggerPrimitiveDigis'),
#  srcDTTh = cms.InputTag('simDtTriggerPrimitiveDigis'),
   srcDTPh = cms.InputTag('omtfStage2Digis'),
   srcDTTh = cms.InputTag('omtfStage2Digis'),
#  srcCSC = cms.InputTag('csctfDigis'),
#  srcCSC = cms.InputTag('emtfStage2Digis'),
#  srcCSC = cms.InputTag('muonCSCDigis','MuonCSCCorrelatedLCTDigi'),
#  srcCSC = cms.InputTag('simCscTriggerPrimitiveDigis','MPCSORTED'),
   srcCSC = cms.InputTag('omtfStage2Digis'),
#  srcRPC = cms.InputTag('muonRPCDigis'),
#  srcRPC = cms.InputTag('simMuonRPCDigis'),
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
  dropCSCPrimitives = cms.bool(False),
)


process.raw2digi_step = cms.Path(process.muonRPCDigis+process.csctfDigis+process.bmtfDigis+process.emtfStage2Digis+process.twinMuxStage2Digis+process.gmtStage2Digis+process.omtfStage2Digis)
#process.omtf_step = cms.Path(process.omtfEmulator)
process.omtf_step = cms.Path(process.omtfEmulator+process.digiComapre)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.schedule = cms.Schedule(process.raw2digi_step,  process.omtf_step, process.endjob_step)

#print process.dumpPython();
print process.schedule



