#include "DataFormats/BTauReco/interface/CandSoftLeptonTagInfo.h"
#include "DataFormats/BTauReco/interface/CandIPTagInfo.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"

#include "RecoBTag/CTagging/interface/CharmTagger.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

CharmTagger::CharmTagger(const edm::ParameterSet & configuration):
	sl_computer_(configuration.getParameter<edm::ParameterSet>("slComputerCfg"))
{
	uses("seTagInfos");

	edm::FileInPath weight_file=configuration.getParameter<edm::FileInPath>("weightFile");
	mvaID_.reset(new TMVAEvaluator());
	
	vpset vars_def = configuration.getParameter<vpset>("variables");
	std::vector<std::string> variable_names(vars_def.size());
	variables_.reserve(vars_def.size());
	for(auto &var : vars_def) {
		variable_names.push_back(
			var.getParameter<std::string>("name")
			);

		MVAVar mva_var;
		mva_var.name = var.getParameter<std::string>("name");
		mva_var.id = reco::getTaggingVariableName(
			var.getParameter<std::string>("taggingVarName")
			);
		mva_var.has_index = var.existsAs<int>("idx") ;
		mva_var.index = mva_var.has_index ? var.getParameter<int>("idx") : 0;
		mva_var.default_value = var.getParameter<double>("default");

		variables_.push_back(mva_var);
	}
	std::vector<std::string> spectators;
	
	mvaID_->initialize("Color:Silent:Error", "BDT", weight_file.fullPath(), variable_names, spectators);
}


/// b-tag a jet based on track-to-jet parameters in the extened info collection
float CharmTagger::discriminator(const TagInfoHelper & tagInfo) const {
  // default value, used if there are no leptons associated to this jet
  const reco::CandIPTagInfo & ip_info = tagInfo.get<reco::CandIPTagInfo>("pfImpactParameterTagInfos");
	const reco::CandSecondaryVertexTagInfo & sv_info = tagInfo.get<reco::CandSecondaryVertexTagInfo>("pfInclusiveSecondaryVertexFinderCtagLTagInfos");
	const reco::CandSoftLeptonTagInfo& softel_info = tagInfo.get<reco::CandSoftLeptonTagInfo>("softPFMuonsTagInfos");
	const reco::CandSoftLeptonTagInfo& softmu_info = tagInfo.get<reco::CandSoftLeptonTagInfo>("softPFElectronsTagInfos");
	reco::TaggingVariableList vars = sl_computer_(ip_info, sv_info, softmu_info, softel_info);

	// Loop over input variables
	std::map<std::string, float> inputs;
	for(auto &mva_var : variables_){
		//vectorial tagging variable
		if(mva_var.has_index){
			std::vector<float> vals = vars.getList(mva_var.id, false);
			inputs[mva_var.name] = (vals.size() > mva_var.index) ? vals[mva_var.index] : mva_var.default_value;
		}
		//single value tagging var
		else {
			inputs[mva_var.name] = vars.get(mva_var.id, mva_var.default_value);
		}
	}

  // TMVAEvaluator is not thread safe
 	std::lock_guard<std::mutex> lock(mutex_);
	//get the MVA output
	float tag = mvaID_->evaluate(inputs);
	return tag;
}
