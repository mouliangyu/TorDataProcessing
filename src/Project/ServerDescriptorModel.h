#pragma once
#include<string>
#include<vector>

class ServerDescriptorModel
{
public:
	ServerDescriptorModel();
	~ServerDescriptorModel();

	//"router" nickname address ORPort SOCKSPort DirPort NL
	std::string NickName;
	std::string Address;
	std::string ORPort;
	std::string SOCKSPort;
	std::string DirPort;

	std::string IdentityED25519;

	std::string MasterKeyED25519;

	//"bandwidth" bandwidth-avg bandwidth-burst bandwidth-observed NL
	int BindwidthAvg;
	int BindwidthBurst;
	int BindwidthObserved;

	std::string Platform;

	std::string Published;

	std::string Fingerprint;

	bool Hibernating;
	
	long Uptime;

	std::string OnionKey;

	std::string OnionKeyCrosscert;

	std::string NTorOnionKey;

	int NTorOnionKeyCrosscertSign;
	std::string NTorOnionKeyCrosscert;

	std::string SigningKey;

	std::vector<std::string> Accept;

	std::vector<std::string> Reject;

	std::string IPV6Policy;

	std::string RouterSigED25519;

	std::string RouterSignature;

	std::string Contact;

	std::string BridgeDistributionRequest;

	std::vector<std::string> Family;

	std::string ReadHistory;
	std::string WriteHistory;

	bool EventDNS;

	bool CachesExtraInfo;

	std::string ExtraInfoDigest;

	bool HiddenServiceDir;

	std::string Protocols;

	bool AllowSingleHopExits;

	std::vector<std::string> ORAddress;

	bool TunnelledDirServer;

	std::string Proto;

public:
	const static std::string KEY_ROUTER;
	const static std::string KEY_OBJ_IDENTITY_ED25519;
	const static std::string KEY_MASTER_KEY_ED25519;
	const static std::string KEY_BANDWIDTH;
	const static std::string KEY_PLATFORM;
	const static std::string KEY_PUBLISHED;
	const static std::string KEY_FINGERPRINT;
	const static std::string KEY_HIBERNATING;
	const static std::string KEY_UPTIME;
	const static std::string KEY_OBJ_ONION_KEY;
	const static std::string KEY_OBJ_ONION_KEY_CROSSCERT;
	const static std::string KEY_NTOR_ONION_KEY;
	const static std::string KEY_OBJ_NTOR_ONION_KEY_CROSSCERT;
	const static std::string KEY_OBJ_SIGNING_KEY;
	const static std::string KEY_ACCEPT;
	const static std::string KEY_REJECT;
	const static std::string KEY_IPV6_POLICY;
	const static std::string KEY_ROUTER_SIG_ED25519;
	const static std::string KEY_OBJ_ROUTER_SIGNATURE;
	const static std::string KEY_CONTACT;
	const static std::string KEY_BRIDGE_DISTRIBUTION_REQUEST;
	const static std::string KEY_FAMILY;
	const static std::string KEY_READ_HISTORY;
	const static std::string KEY_WRITE_HISTORY;
	const static std::string KEY_EVENTDNS;
	const static std::string KEY_CACHES_EXTRA_INFO;
	const static std::string KEY_EXTRA_INFO_DIGEST;
	const static std::string KEY_HIDDEN_SERVICE_DIR;
	const static std::string KEY_PROTOCOLS;
	const static std::string KEY_ALLOW_SINGLE_HOP_EXITS;
	const static std::string KEY_OR_ADDRESS;
	const static std::string KEY_TUNNELLED_DIR_SERVER;
	const static std::string KEY_PROTO;
};

