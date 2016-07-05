import FWCore.ParameterSet.Config as cms

shallowClusters = cms.EDProducer(
   "ShallowClustersProducer",
   Clusters=cms.InputTag("siStripClusters"),
   ProcessedRawDigis = cms.InputTag("siStripProcessedRawDigis"),
   Prefix=cms.string("cluster"),
   CommonMode = cms.InputTag("siStripDigis", "CommonMode"),
   computeCommonMode = cms.bool(False)
   )

