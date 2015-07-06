import FWCore.ParameterSet.Config as cms

charmTagsComputer = cms.ESProducer(
   'CharmTaggerESProducer',
   weightFile = cms.FileInPath('RecoBTag/CTagging/data/c_vs_udsg.weight.xml'),
   variables = cms.VPSet(
      cms.PSet(name = cms.string('dummy'), default = cms.double(0))
      ),
   computer = cms.ESInputTag('combinedSecondaryVertexSoftLeptonComputer'),
   tagInfos = cms.VInputTag(
      cms.InputTag('pfImpactParameterTagInfos'),
      cms.InputTag('pfInclusiveSecondaryVertexFinderCtagLTagInfos'),
      cms.InputTag('softPFMuonsTagInfos'),
      cms.InputTag('softPFElectronsTagInfos'),
      )
   )
