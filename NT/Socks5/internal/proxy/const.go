package proxy

// Socks5 consts

// Connect to SOCKS server: VER:NMETHODS:METHODS
// SOCKS server select method: VER:METHOD
// SOCKS Request: VER:CMD:RSV:ATYP:DST.ADDR:DST.PORT
// SOCKS server Replies: VER:REP:RSV:ATYP:BND.ADDR:BND.PORT
const (
	// Version (VER)
	SOCKS_VERSION byte = 0x05

	// Supported authentication methods (METHODS)
	SOCKS_AUTH_NO_REQUIRED           byte = 0x00
	SOCKS_AUTH_NO_ACCEPTABLE_METHODS byte = 0xFF

	// Supported command (CMD)
	SOCKS_CMD_CONNECT       byte = 0x01
	SOCKS_CMD_BIND          byte = 0x02
	SOCKS_CMD_UDP_ASSOCIATE byte = 0x03

	// Reserved (RSV)
	SOCKS_RESERVED byte = 0x00

	// Address type of following address (ATYP)
	SOCKS_ATYP_IPV4 byte = 0x01
	SOCKS_ATYP_DOMAINNAME byte = 0x03
	SOCKS_ATYP_IPV6 byte = 0x04

	// Replies (REP)
	SOCKS_REPLY_SUCCEEDED                         byte = 0x00
	SOCKS_REPLY_GENERAL_SOCKS_SERVER_FAILURE      byte = 0x01
	SOCKS_REPLY_CONNECTION_NOT_ALLOWED_BY_RULESET byte = 0x02
	SOCKS_REPLY_NETWORK_UNREACHABLE               byte = 0x03
	SOCKS_REPLY_HOST_UNREACHABLE                  byte = 0x04
	SOCKS_REPLY_CONNECTION_REFUSED                byte = 0x05
	SOCKS_REPLY_TTL_EXPIRED                       byte = 0x06
	SOCKS_REPLY_COMMAND_NOT_SUPPORTED             byte = 0x07
	SOCKS_REPLY_ADDRESS_TYPE_NOT_SUPPORTED        byte = 0x08
)
