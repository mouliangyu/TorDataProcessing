#include "Key.h"

bool KeyFactory::sInitialized = false;
boost::unordered_map<std::string, Key> KeyFactory::sNameKeyHashMap = boost::unordered_map<std::string, Key>();

void KeyFactory::initialize() {
	if (sInitialized) {
		return;
	} else {
		sInitialized = true;
	}
	sNameKeyHashMap.clear();
	
	sNameKeyHashMap.insert(std::make_pair(("the-empty-key"), EMPTY));
	sNameKeyHashMap.insert(std::make_pair(("the-invalid-key"), INVALID));

	/* crypto keys */
	sNameKeyHashMap.insert(std::make_pair(("-----BEGIN"), CRYPTO_BEGIN));
	sNameKeyHashMap.insert(std::make_pair(("-----END"), CRYPTO_END));

	/* descriptor keys (in alphabetic order) */
	sNameKeyHashMap.insert(std::make_pair(("a"), A));
	sNameKeyHashMap.insert(std::make_pair(("accept"), ACCEPT));
	sNameKeyHashMap.insert(std::make_pair(("allow-single-hop-exits"), ALLOW_SINGLE_HOP_EXITS));
	sNameKeyHashMap.insert(std::make_pair(("bandwidth"), BANDWIDTH));
	sNameKeyHashMap.insert(std::make_pair(("bandwidth-weights"), BANDWIDTH_WEIGHTS));
	sNameKeyHashMap.insert(std::make_pair(("bridge-ips"), BRIDGE_IPS));
	sNameKeyHashMap.insert(std::make_pair(("bridge-ip-transports"), BRIDGE_IP_TRANSPORTS));
	sNameKeyHashMap.insert(std::make_pair(("bridge-ip-versions"), BRIDGE_IP_VERSIONS));
	sNameKeyHashMap.insert(std::make_pair(("bridge-pool-assignment"), BRIDGE_POOL_ASSIGNMENT));
	sNameKeyHashMap.insert(std::make_pair(("bridge-stats-end"), BRIDGE_STATS_END));
	sNameKeyHashMap.insert(std::make_pair(("caches-extra-info"), CACHES_EXTRA_INFO));
	sNameKeyHashMap.insert(std::make_pair(("cell-circuits-per-decile"), CELL_CIRCUITS_PER_DECILE));
	sNameKeyHashMap.insert(std::make_pair(("cell-processed-cells"), CELL_PROCESSED_CELLS));
	sNameKeyHashMap.insert(std::make_pair(("cell-queued-cells"), CELL_QUEUED_CELLS));
	sNameKeyHashMap.insert(std::make_pair(("cell-stats-end"), CELL_STATS_END));
	sNameKeyHashMap.insert(std::make_pair(("cell-time-in-queue"), CELL_TIME_IN_QUEUE));
	sNameKeyHashMap.insert(std::make_pair(("client-versions"), CLIENT_VERSIONS));
	sNameKeyHashMap.insert(std::make_pair(("conn-bi-direct"), CONN_BI_DIRECT));
	sNameKeyHashMap.insert(std::make_pair(("consensus-method"), CONSENSUS_METHOD));
	sNameKeyHashMap.insert(std::make_pair(("consensus-methods"), CONSENSUS_METHODS));
	sNameKeyHashMap.insert(std::make_pair(("contact"), CONTACT));
	sNameKeyHashMap.insert(std::make_pair(("dircacheport"), DIRCACHEPORT));
	sNameKeyHashMap.insert(std::make_pair(("directory-footer"), DIRECTORY_FOOTER));
	sNameKeyHashMap.insert(std::make_pair(("directory-signature"), DIRECTORY_SIGNATURE));
	sNameKeyHashMap.insert(std::make_pair(("dirreq-read-history"), DIRREQ_READ_HISTORY));
	sNameKeyHashMap.insert(std::make_pair(("dirreq-stats-end"), DIRREQ_STATS_END));
	sNameKeyHashMap.insert(std::make_pair(("dirreq-v2-direct-dl"), DIRREQ_V2_DIRECT_DL));
	sNameKeyHashMap.insert(std::make_pair(("dirreq-v2-ips"), DIRREQ_V2_IPS));
	sNameKeyHashMap.insert(std::make_pair(("dirreq-v2-reqs"), DIRREQ_V2_REQS));
	sNameKeyHashMap.insert(std::make_pair(("dirreq-v2-resp"), DIRREQ_V2_RESP));
	sNameKeyHashMap.insert(std::make_pair(("dirreq-v2-share"), DIRREQ_V2_SHARE));
	sNameKeyHashMap.insert(std::make_pair(("dirreq-v2-tunneled-dl"), DIRREQ_V2_TUNNELED_DL));
	sNameKeyHashMap.insert(std::make_pair(("dirreq-v3-direct-dl"), DIRREQ_V3_DIRECT_DL));
	sNameKeyHashMap.insert(std::make_pair(("dirreq-v3-ips"), DIRREQ_V3_IPS));
	sNameKeyHashMap.insert(std::make_pair(("dirreq-v3-reqs"), DIRREQ_V3_REQS));
	sNameKeyHashMap.insert(std::make_pair(("dirreq-v3-resp"), DIRREQ_V3_RESP));
	sNameKeyHashMap.insert(std::make_pair(("dirreq-v3-share"), DIRREQ_V3_SHARE));
	sNameKeyHashMap.insert(std::make_pair(("dirreq-v3-tunneled-dl"), DIRREQ_V3_TUNNELED_DL));
	sNameKeyHashMap.insert(std::make_pair(("dirreq-write-history"), DIRREQ_WRITE_HISTORY));
	sNameKeyHashMap.insert(std::make_pair(("dir-address"), DIR_ADDRESS));
	sNameKeyHashMap.insert(std::make_pair(("dir-identity-key"), DIR_IDENTITY_KEY));
	sNameKeyHashMap.insert(std::make_pair(("dir-key-certificate-version"), DIR_KEY_CERTIFICATE_VERSION));
	sNameKeyHashMap.insert(std::make_pair(("dir-key-certification"), DIR_KEY_CERTIFICATION));
	sNameKeyHashMap.insert(std::make_pair(("dir-key-crosscert"), DIR_KEY_CROSSCERT));
	sNameKeyHashMap.insert(std::make_pair(("dir-key-expires"), DIR_KEY_EXPIRES));
	sNameKeyHashMap.insert(std::make_pair(("dir-key-published"), DIR_KEY_PUBLISHED));
	sNameKeyHashMap.insert(std::make_pair(("dir-options"), DIR_OPTIONS));
	sNameKeyHashMap.insert(std::make_pair(("dir-signing-key"), DIR_SIGNING_KEY));
	sNameKeyHashMap.insert(std::make_pair(("dir-source"), DIR_SOURCE));
	sNameKeyHashMap.insert(std::make_pair(("entry-ips"), ENTRY_IPS));
	sNameKeyHashMap.insert(std::make_pair(("entry-stats-end"), ENTRY_STATS_END));
	sNameKeyHashMap.insert(std::make_pair(("eventdns"), EVENTDNS));
	sNameKeyHashMap.insert(std::make_pair(("exit-kibibytes-read"), EXIT_KIBIBYTES_READ));
	sNameKeyHashMap.insert(std::make_pair(("exit-kibibytes-written"), EXIT_KIBIBYTES_WRITTEN));
	sNameKeyHashMap.insert(std::make_pair(("exit-stats-end"), EXIT_STATS_END));
	sNameKeyHashMap.insert(std::make_pair(("exit-streams-opened"), EXIT_STREAMS_OPENED));
	sNameKeyHashMap.insert(std::make_pair(("extra-info"), EXTRA_INFO));
	sNameKeyHashMap.insert(std::make_pair(("extra-info-digest"), EXTRA_INFO_DIGEST));
	sNameKeyHashMap.insert(std::make_pair(("family"), FAMILY));
	sNameKeyHashMap.insert(std::make_pair(("fingerprint"), FINGERPRINT));
	sNameKeyHashMap.insert(std::make_pair(("flag-thresholds"), FLAG_THRESHOLDS));
	sNameKeyHashMap.insert(std::make_pair(("fresh-until"), FRESH_UNTIL));
	sNameKeyHashMap.insert(std::make_pair(("geoip6-db-digest"), GEOIP6_DB_DIGEST));
	sNameKeyHashMap.insert(std::make_pair(("geoip-client-origins"), GEOIP_CLIENT_ORIGINS));
	sNameKeyHashMap.insert(std::make_pair(("geoip-db-digest"), GEOIP_DB_DIGEST));
	sNameKeyHashMap.insert(std::make_pair(("geoip-start-time"), GEOIP_START_TIME));
	sNameKeyHashMap.insert(std::make_pair(("hibernating"), HIBERNATING));
	sNameKeyHashMap.insert(std::make_pair(("hidden-service-dir"), HIDDEN_SERVICE_DIR));
	sNameKeyHashMap.insert(std::make_pair(("hidserv-dir-onions-seen"), HIDSERV_DIR_ONIONS_SEEN));
	sNameKeyHashMap.insert(std::make_pair(("hidserv-rend-relayed-cells"), HIDSERV_REND_RELAYED_CELLS));
	sNameKeyHashMap.insert(std::make_pair(("hidserv-stats-end"), HIDSERV_STATS_END));
	sNameKeyHashMap.insert(std::make_pair(("id"), ID));
	sNameKeyHashMap.insert(std::make_pair(("identity-ed25519"), IDENTITY_ED25519));
	sNameKeyHashMap.insert(std::make_pair(("ipv6-policy"), IPV6_POLICY));
	sNameKeyHashMap.insert(std::make_pair(("known-flags"), KNOWN_FLAGS));
	sNameKeyHashMap.insert(std::make_pair(("legacy-dir-key"), LEGACY_DIR_KEY));
	sNameKeyHashMap.insert(std::make_pair(("legacy-key"), LEGACY_KEY));
	sNameKeyHashMap.insert(std::make_pair(("m"), M));
	sNameKeyHashMap.insert(std::make_pair(("master-key-ed25519"), MASTER_KEY_ED25519));
	sNameKeyHashMap.insert(std::make_pair(("network-status-version"), NETWORK_STATUS_VERSION));
	sNameKeyHashMap.insert(std::make_pair(("ntor-onion-key"), NTOR_ONION_KEY));
	sNameKeyHashMap.insert(std::make_pair(("ntor-onion-key-crosscert"), NTOR_ONION_KEY_CROSSCERT));
	sNameKeyHashMap.insert(std::make_pair(("onion-key"), ONION_KEY));
	sNameKeyHashMap.insert(std::make_pair(("onion-key-crosscert"), ONION_KEY_CROSSCERT));
	sNameKeyHashMap.insert(std::make_pair(("opt"), OPT));
	sNameKeyHashMap.insert(std::make_pair(("or-address"), OR_ADDRESS));
	sNameKeyHashMap.insert(std::make_pair(("p"), P));
	sNameKeyHashMap.insert(std::make_pair(("p6"), P6));
	sNameKeyHashMap.insert(std::make_pair(("package"), PACKAGE));
	sNameKeyHashMap.insert(std::make_pair(("padding-counts"), PADDING_COUNTS));
	sNameKeyHashMap.insert(std::make_pair(("params"), PARAMS));
	sNameKeyHashMap.insert(std::make_pair(("platform"), PLATFORM));
	sNameKeyHashMap.insert(std::make_pair(("pr"), PR));
	sNameKeyHashMap.insert(std::make_pair(("proto"), PROTO));
	sNameKeyHashMap.insert(std::make_pair(("protocols"), PROTOCOLS));
	sNameKeyHashMap.insert(std::make_pair(("published"), PUBLISHED));
	sNameKeyHashMap.insert(std::make_pair(("r"), R));
	sNameKeyHashMap.insert(std::make_pair(("read-history"), READ_HISTORY));
	sNameKeyHashMap.insert(std::make_pair(("recommended-client-protocols"), RECOMMENDED_CLIENT_PROTOCOLS));
	sNameKeyHashMap.insert(std::make_pair(("recommended-relay-protocols"), RECOMMENDED_RELAY_PROTOCOLS));
	sNameKeyHashMap.insert(std::make_pair(("recommended-software"), RECOMMENDED_SOFTWARE));
	sNameKeyHashMap.insert(std::make_pair(("reject"), REJECT));
	sNameKeyHashMap.insert(std::make_pair(("required-client-protocols"), REQUIRED_CLIENT_PROTOCOLS));
	sNameKeyHashMap.insert(std::make_pair(("required-relay-protocols"), REQUIRED_RELAY_PROTOCOLS));
	sNameKeyHashMap.insert(std::make_pair(("router"), ROUTER));
	sNameKeyHashMap.insert(std::make_pair(("router-digest"), ROUTER_DIGEST));
	sNameKeyHashMap.insert(std::make_pair(("router-digest-sha256"), ROUTER_DIGEST_SHA256));
	sNameKeyHashMap.insert(std::make_pair(("router-signature"), ROUTER_SIGNATURE));
	sNameKeyHashMap.insert(std::make_pair(("router-sig-ed25519"), ROUTER_SIG_ED25519));
	sNameKeyHashMap.insert(std::make_pair(("router-status"), ROUTER_STATUS));
	sNameKeyHashMap.insert(std::make_pair(("running-routers"), RUNNING_ROUTERS));
	sNameKeyHashMap.insert(std::make_pair(("s"), S));
	sNameKeyHashMap.insert(std::make_pair(("server-versions"), SERVER_VERSIONS));
	sNameKeyHashMap.insert(std::make_pair(("shared-rand-commit"), SHARED_RAND_COMMIT));
	sNameKeyHashMap.insert(std::make_pair(("shared-rand-current-value"), SHARED_RAND_CURRENT_VALUE));
	sNameKeyHashMap.insert(std::make_pair(("shared-rand-participate"), SHARED_RAND_PARTICIPATE));
	sNameKeyHashMap.insert(std::make_pair(("shared-rand-previous-value"), SHARED_RAND_PREVIOUS_VALUE));
	sNameKeyHashMap.insert(std::make_pair(("signed-directory"), SIGNED_DIRECTORY));
	sNameKeyHashMap.insert(std::make_pair(("signing-key"), SIGNING_KEY));
	sNameKeyHashMap.insert(std::make_pair(("transport"), TRANSPORT));
	sNameKeyHashMap.insert(std::make_pair(("tunnelled-dir-server"), TUNNELLED_DIR_SERVER));
	sNameKeyHashMap.insert(std::make_pair(("uptime"), UPTIME));
	sNameKeyHashMap.insert(std::make_pair(("v"), V));
	sNameKeyHashMap.insert(std::make_pair(("valid-after"), VALID_AFTER));
	sNameKeyHashMap.insert(std::make_pair(("valid-until"), VALID_UNTIL));
	sNameKeyHashMap.insert(std::make_pair(("vote-digest"), VOTE_DIGEST));
	sNameKeyHashMap.insert(std::make_pair(("vote-status"), VOTE_STATUS));
	sNameKeyHashMap.insert(std::make_pair(("voting-delay"), VOTING_DELAY));
	sNameKeyHashMap.insert(std::make_pair(("w"), W));
	sNameKeyHashMap.insert(std::make_pair(("write-history"), WRITE_HISTORY));
}

Key KeyFactory::fromString(const std::string & str) {
	initialize();
	return sNameKeyHashMap.find(str)->second;
}
