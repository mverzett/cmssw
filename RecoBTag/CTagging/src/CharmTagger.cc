#include "DataFormats/BTauReco/interface/CandSoftLeptonTagInfo.h"
#include "DataFormats/BTauReco/interface/CandIPTagInfo.h"

#include "RecoBTag/CTagging/interface/CharmTagger.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

CharmTagger::CharmTagger(const edm::ParameterSet & configuration):
	variables_(configuration.getParameter<vpset>("variables"))
{
	uses("seTagInfos");

	edm::FileInPath weight_file=configuration.getParameter<edm::FileInPath>("weightFile");
	mvaID_.reset(new TMVAEvaluator());
	
	std::vector<std::string> variable_names(variables_.size());
	for(auto &var : variables_) {
		variable_names.push_back(
			var.getParameter<std::string>("name")
			);
	}
	std::vector<std::string> spectators;
	
	mvaID_->initialize("Color:Silent:Error", "BDT", weight_file.fullPath(), variable_names, spectators);
}


/// b-tag a jet based on track-to-jet parameters in the extened info collection
float CharmTagger::discriminator(const TagInfoHelper & tagInfo) const {
  // default value, used if there are no leptons associated to this jet
  const reco::CandIPTagInfo & ip_info = tagInfo.get<reco::CandIPTagInfo>("pfImpactParameterTagInfos");
	const reco::CandIPTagInfo & sv_info = tagInfo.get<reco::CandIPTagInfo>("pfInclusiveSecondaryVertexFinderCtagLTagInfos");
	const reco::CandSoftLeptonTagInfo& softel_info = tagInfo.get<reco::CandSoftLeptonTagInfo>("softPFMuonsTagInfos");
	const reco::CandSoftLeptonTagInfo& softmu_info = tagInfo.get<reco::CandSoftLeptonTagInfo>("softPFElectronsTagInfos");
	
  // TMVAEvaluator is not thread safe
 	std::lock_guard<std::mutex> lock(mutex_);
	ip_info.hasProbabilities();
	sv_info.hasProbabilities();
	softel_info.leptons();
	softmu_info.leptons();

	// Loop over input variables
	std::map<std::string, float> inputs;
	//for(){}

	//get the MVA output
	float tag = mvaID_->evaluate(inputs);
	return tag;
}
