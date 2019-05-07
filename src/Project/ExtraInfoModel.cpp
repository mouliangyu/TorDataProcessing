#include "ExtraInfoModel.h"

const std::string ExtraInfoModel::KEY_ROUTER = "router";
const std::string ExtraInfoModel::KEY_OBJ_IDENTITY_ED25519 = "identity-ed25519";
const std::string ExtraInfoModel::KEY_MASTER_KEY_ED25519 = "master-key-ed25519";
const std::string ExtraInfoModel::KEY_BANDWIDTH = "bandwidth";
const std::string ExtraInfoModel::KEY_PLATFORM = "platform";
const std::string ExtraInfoModel::KEY_PUBLISHED = "published";
const std::string ExtraInfoModel::KEY_FINGERPRINT = "fingerprint";
const std::string ExtraInfoModel::KEY_HIBERNATING = "hibernating";
const std::string ExtraInfoModel::KEY_UPTIME = "uptime";
const std::string ExtraInfoModel::KEY_OBJ_ONION_KEY = "onion-key";
const std::string ExtraInfoModel::KEY_OBJ_ONION_KEY_CROSSCERT = "onion-key-crosscert";
const std::string ExtraInfoModel::KEY_NTOR_ONION_KEY = "ntor-onion-key";
const std::string ExtraInfoModel::KEY_OBJ_NTOR_ONION_KEY_CROSSCERT = "ntor-onion-key-crosscert";
const std::string ExtraInfoModel::KEY_OBJ_SIGNING_KEY = "signing-key";
const std::string ExtraInfoModel::KEY_ACCEPT = "accept";
const std::string ExtraInfoModel::KEY_REJECT = "reject";
const std::string ExtraInfoModel::KEY_IPV6_POLICY = "ipv6-policy";
const std::string ExtraInfoModel::KEY_ROUTER_SIG_ED25519 = "router-sig-ed25519";
const std::string ExtraInfoModel::KEY_OBJ_ROUTER_SIGNATURE = "router-signature";
const std::string ExtraInfoModel::KEY_CONTACT = "contact";
const std::string ExtraInfoModel::KEY_BRIDGE_DISTRIBUTION_REQUEST = "bridge-distribution-request";
const std::string ExtraInfoModel::KEY_FAMILY = "family";
const std::string ExtraInfoModel::KEY_READ_HISTORY = "read-history";
const std::string ExtraInfoModel::KEY_WRITE_HISTORY = "write-history";
const std::string ExtraInfoModel::KEY_EVENTDNS = "eventdns";
const std::string ExtraInfoModel::KEY_CACHES_EXTRA_INFO = "caches-extra-info";
const std::string ExtraInfoModel::KEY_EXTRA_INFO_DIGEST = "extra-info-digest";
const std::string ExtraInfoModel::KEY_HIDDEN_SERVICE_DIR = "hidden-service-dir";
const std::string ExtraInfoModel::KEY_PROTOCOLS = "protocols";
const std::string ExtraInfoModel::KEY_ALLOW_SINGLE_HOP_EXITS = "allow-single-hop-exits";
const std::string ExtraInfoModel::KEY_OR_ADDRESS = "or-address";
const std::string ExtraInfoModel::KEY_TUNNELLED_DIR_SERVER = "tunnelled-dir-server";
const std::string ExtraInfoModel::KEY_PROTO = "proto";


ExtraInfoModel::ExtraInfoModel() {
}


ExtraInfoModel::~ExtraInfoModel() {
}
