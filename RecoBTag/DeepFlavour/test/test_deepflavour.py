# This configuration is an example that recalibrates the slimmedJets from MiniAOD
# and adds a new userfloat "oldJetMass" and an additional b-tag discriminator to them

## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *
## switch to uncheduled mode
process.options.allowUnscheduled = cms.untracked.bool(True)
#process.Tracer = cms.Service("Tracer")

## uncomment the following line to update different jet collections
## and add them to the event content
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection

updateJetCollection(
   process,
   jetSource = cms.InputTag('slimmedJets'),
   jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
   btagDiscriminators = [
      'pfCombinedSecondaryVertexV2BJetTags',
      'pfDeepCSVJetTags:probudsg', 
      'pfDeepCSVJetTags:probb', 
      'pfDeepCSVJetTags:probc', 
      'pfDeepCSVJetTags:probbb', 
      'pfDeepFlavourJetTags:probb',
      'pfDeepFlavourJetTags:probbb',
      'pfDeepFlavourJetTags:problepb',
      'pfDeepFlavourJetTags:probc',
      'pfDeepFlavourJetTags:probuds',
      'pfDeepFlavourJetTags:probg',
      ] ## to add discriminators
)

## ------------------------------------------------------
#  In addition you usually want to change the following
#  parameters:
## ------------------------------------------------------
#
#   process.GlobalTag.globaltag =  ...    ##  (according to https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions)
#                                         ##
from PhysicsTools.PatAlgos.patInputFiles_cff import filesRelValTTbarPileUpMINIAODSIM
process.source.fileNames = filesRelValTTbarPileUpMINIAODSIM
#                                         ##
process.maxEvents.input = 100
#                                         ##
from Configuration.EventContent.EventContent_cff import MINIAODSIMEventContent
process.out.outputCommands = cms.untracked.vstring('drop *')
process.out.outputCommands.append('keep *_selectedUpdatedPatJets*_*_*')
process.out.outputCommands.append('drop *_selectedUpdatedPatJets*_caloTowers_*')
process.out.outputCommands.append('drop *_selectedUpdatedPatJets*_genJets_*')
process.out.outputCommands.append('drop *_selectedUpdatedPatJets*_pfCandidates_*')
#                                         ##
process.out.fileName = 'patTuple_updateJets_fromMiniAOD.root'
#                                         ##
#   process.options.wantSummary = False   ##  (to suppress the long output at the end of the job)
