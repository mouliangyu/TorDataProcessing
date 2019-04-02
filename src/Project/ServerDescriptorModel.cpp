#include "ServerDescriptorModel.h"

const std::string ServerDescriptorModel::KEY_ROUTER = "router";
const std::string ServerDescriptorModel::KEY_OBJ_IDENTITY_ED25519 = "identity-ed25519";
const std::string ServerDescriptorModel::KEY_MASTER_KEY_ED25519 = "master-key-ed25519";
const std::string ServerDescriptorModel::KEY_BANDWIDTH = "bandwidth";
const std::string ServerDescriptorModel::KEY_PLATFORM = "platform";
const std::string ServerDescriptorModel::KEY_PUBLISHED = "published";
const std::string ServerDescriptorModel::KEY_FINGERPRINT = "fingerprint";
const std::string ServerDescriptorModel::KEY_HIBERNATING = "hibernating";
const std::string ServerDescriptorModel::KEY_UPTIME = "uptime";
const std::string ServerDescriptorModel::KEY_OBJ_ONION_KEY = "onion-key";
const std::string ServerDescriptorModel::KEY_OBJ_ONION_KEY_CROSSCERT = "onion-key-crosscert";
const std::string ServerDescriptorModel::KEY_NTOR_ONION_KEY = "ntor-onion-key";
const std::string ServerDescriptorModel::KEY_OBJ_NTOR_ONION_KEY_CROSSCERT = "ntor-onion-key-crosscert";
const std::string ServerDescriptorModel::KEY_OBJ_SIGNING_KEY = "signing-key";
const std::string ServerDescriptorModel::KEY_ACCEPT = "accept";
const std::string ServerDescriptorModel::KEY_REJECT = "reject";
const std::string ServerDescriptorModel::KEY_IPV6_POLICY = "ipv6-policy";
const std::string ServerDescriptorModel::KEY_ROUTER_SIG_ED25519 = "router-sig-ed25519";
const std::string ServerDescriptorModel::KEY_OBJ_ROUTER_SIGNATURE = "router-signature";
const std::string ServerDescriptorModel::KEY_CONTACT = "contact";
const std::string ServerDescriptorModel::KEY_BRIDGE_DISTRIBUTION_REQUEST = "bridge-distribution-request";
const std::string ServerDescriptorModel::KEY_FAMILY = "family";
const std::string ServerDescriptorModel::KEY_READ_HISTORY = "read-history";
const std::string ServerDescriptorModel::KEY_WRITE_HISTORY = "write-history";
const std::string ServerDescriptorModel::KEY_EVENTDNS = "eventdns";
const std::string ServerDescriptorModel::KEY_CACHES_EXTRA_INFO = "caches-extra-info";
const std::string ServerDescriptorModel::KEY_EXTRA_INFO_DIGEST = "extra-info-digest";
const std::string ServerDescriptorModel::KEY_HIDDEN_SERVICE_DIR = "hidden-service-dir";
const std::string ServerDescriptorModel::KEY_PROTOCOLS = "protocols";
const std::string ServerDescriptorModel::KEY_ALLOW_SINGLE_HOP_EXITS = "allow-single-hop-exits";
const std::string ServerDescriptorModel::KEY_OR_ADDRESS = "or-address";
const std::string ServerDescriptorModel::KEY_TUNNELLED_DIR_SERVER = "tunnelled-dir-server";
const std::string ServerDescriptorModel::KEY_PROTO = "proto";

ServerDescriptorModel::ServerDescriptorModel()
{
	BindwidthAvg = 0;
	BindwidthBurst = 0;
	BindwidthObserved = 0;

	Hibernating = false;
	Uptime = 0;

	NTorOnionKeyCrosscertSign = -1;
	EventDNS = false;
	CachesExtraInfo = false;
	HiddenServiceDir = false;
	AllowSingleHopExits = false;
	TunnelledDirServer = false;
}


ServerDescriptorModel::~ServerDescriptorModel()
{
}
