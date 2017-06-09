from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'MKjob'
config.General.workArea = 'v5'
config.General.transferLogs = True 
config.General.transferOutputs = True 

config.section_("Data")
#config.Data.inputDataset = '/SingleMuon/Run2016H-ZMu-PromptReco-v2/RAW-RECO'
#config.Data.inputDataset = '/SingleMuon/Run2016H-PromptReco-v2/AOD'
config.Data.inputDataset = '/ExpressPhysics/Run2017A-Express-v1/FEVT'

#config.Data.lumiMask='Cert_271036-284044_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt'
config.Data.lumiMask='JSON.txt'

#config.Data.runRange = '282000-283000'
config.Data.runRange = '295606'

config.Data.useParent = False 
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 5 #number of files per jobs
config.Data.totalUnits =  -1 #number of event
config.Data.outLFNDirBase = '/store/user/konec/test/'
config.Data.publication = False 
#config.Data.outputDatasetTag = 'CRAB3_tutorial_May2015_Data_analysis'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'makeTree.py'
#config.JobType.disableAutomaticOutputCollection = True
config.JobType.outputFiles = ['omtfTree.root', 'omtfHelper.root']

config.section_("Site")
#config.Site.whitelist = ['T3_CH_CERNCAF']
#config.Site.whitelist = ['T2_CH_CERN']
config.Site.storageSite = 'T2_PL_Swierk'
#config.Site.blacklist = ['T2_KR_*','T2_CN_*','T2_BR_*','T2_US_Florida','T2_US_UCSD']
