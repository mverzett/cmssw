import FWCore.ParameterSet.Config as cms

deepFlavourJetTags = cms.EDProducer(
	'DeepFlavourJetTagsProducer',
	src = cms.InputTag('deepNNTagInfos'),
	NNConfig = cms.FileInPath('RecoBTag/Combined/data/DeepFlavourNoSL.json')
	)
