import FWCore.ParameterSet.Config as cms

charmTagJetTags = cms.EDProducer(
   "JetTagProducer",
   jetTagComputer = cms.string('charmTagsComputer')
   tagInfos = cms.VInputTag(
      cms.InputTag('pfImpactParameterTagInfos'),
      cms.InputTag('pfInclusiveSecondaryVertexFinderCtagLTagInfos'),
      cms.InputTag('softPFMuonsTagInfos'),
      cms.InputTag('softPFElectronsTagInfos'),
      )
)
