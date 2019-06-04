
#pragma once

#include <map>
#include <set>
#include "Descriptor.h"
#include "DirectorySignature.h"
#include "DirSourceEntry.h"
#include "NetworkStatusEntry.h"

class RelayNetworkStatusConsensus :public Descriptor{
public:
	virtual int getNetworkStatusVersion() = 0;
	virtual shared_ptr<std::string> getConsensusFlavor() = 0;
	virtual int getConsensusMethod() = 0;
	virtual long getValidAfterMillis() = 0;
	virtual long getFreshUntilMillis() = 0;
	virtual long getValidUntilMillis() = 0;
	virtual long getVoteSeconds() = 0;
	virtual long getDistSeconds() = 0;
	virtual shared_ptr<std::vector<std::string>> getRecommendedServerVersions() = 0;
	virtual shared_ptr<std::vector<std::string>> getRecommendedClientVersions() = 0;
	virtual shared_ptr<std::map<std::string, std::set<long>>> getRecommendedClientProtocols() = 0;
	virtual shared_ptr<std::map<std::string, std::set<long>>> getRecommendedRelayProtocols() = 0;
	virtual shared_ptr<std::map<std::string, std::set<long>>> getRequiredClientProtocols() = 0;
	virtual shared_ptr<std::map<std::string, std::set<long>>> getRequiredRelayProtocols() = 0;
	virtual shared_ptr<std::vector<std::string>> getPackageLines() = 0;
	virtual shared_ptr<std::set<std::string>> getKnownFlags() = 0;
	virtual shared_ptr<std::map<std::string, int>> getConsensusParams() = 0;
	virtual int getSharedRandPreviousNumReveals() = 0;
	virtual shared_ptr<std::string> getSharedRandPreviousValue() = 0;
	virtual int getSharedRandCurrentNumReveals() = 0;
	virtual shared_ptr<std::string> getSharedRandCurrentValue() = 0;
	virtual shared_ptr<std::map<std::string, DirSourceEntry>> getDirSourceEntries() = 0;
	virtual shared_ptr<std::map<std::string, NetworkStatusEntry>> getStatusEntries() = 0;
	virtual bool containsStatusEntry(shared_ptr<std::string> fingerprint) = 0;
	virtual shared_ptr<NetworkStatusEntry> getStatusEntry(shared_ptr<std::string> fingerprint) = 0;
	virtual shared_ptr<std::vector<DirectorySignature>> getSignatures() = 0;
	virtual shared_ptr<std::map<std::string, int>> getBandwidthWeights() = 0;
	virtual shared_ptr<std::string> getDigestSha1Hex() = 0;
};