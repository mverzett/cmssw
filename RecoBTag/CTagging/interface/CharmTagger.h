#ifndef RecoBTag_CTagging_CharmTagger_h
#define RecoBTag_CTagging_CharmTagger_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/Utils/interface/TMVAEvaluator.h"
#include "RecoBTau/JetTagComputer/interface/JetTagComputer.h"
#include <mutex>
#include "FWCore/Utilities/interface/ESInputTag.h"

/** \class CharmTagger
 *  \author M. Verzetti, U. Rochester, N.Y.
 *  copied from ElectronTagger.h
 */

class CharmTagger : public JetTagComputer {
public:
  /// explicit ctor 
	CharmTagger(const edm::ParameterSet & );
  virtual float discriminator(const TagInfoHelper & tagInfo) const override;
	typedef std::vector<edm::ParameterSet> vpset;

private:
  mutable std::mutex mutex_;
	[[cms::thread_guard("mutex_")]] std::unique_ptr<TMVAEvaluator> mvaID_;
	edm::ESInputTag computer_label_;
	vpset variables_;
};

#endif
