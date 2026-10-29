#ifndef PROTOCOLCONSTANTS_H
#define PROTOCOLCONSTANTS_H

namespace amnezia
{
    namespace protocols
    {
        namespace dns
        {
            constexpr char amneziaDnsIp[] = "172.29.172.254";
        }

        namespace openvpn
        {
            constexpr char defaultPort[] = "1194";
            constexpr char defaultTransportProto[] = "udp";
        }

        namespace xray
        {
            constexpr char defaultPort[] = "443";
            constexpr char defaultLocalAddr[] = "10.33.0.2";
            constexpr char defaultLocalListenAddr[] = "127.0.0.1";

            constexpr char defaultFingerprint[] = "chrome";

            constexpr char defaultXhttpMode[] = "Auto";
            constexpr char defaultXhttpUplinkMethod[] = "POST";
            constexpr char defaultXhttpSessionPlacement[] = "Path";
            constexpr char defaultXhttpSessionKey[] = "";
            constexpr char defaultXhttpSeqPlacement[] = "Path";
            constexpr char defaultXhttpUplinkDataPlacement[] = "Body";

            constexpr char defaultXhttpHost[] = "www.googletagmanager.com";
            constexpr char defaultXhttpUplinkChunkSize[] = "0";
            constexpr char defaultXhttpScMaxEachPostBytesMin[] = "1";
            constexpr char defaultXhttpScMaxEachPostBytesMax[] = "100";
            constexpr char defaultXhttpScMinPostsIntervalMsMin[] = "100";
            constexpr char defaultXhttpScMinPostsIntervalMsMax[] = "800";
            constexpr char defaultXhttpScStreamUpServerSecsMin[] = "1";
            constexpr char defaultXhttpScStreamUpServerSecsMax[] = "100";

            constexpr char defaultXPaddingPlacement[] = "Cookie";
            constexpr char defaultXPaddingMethod[] = "Repeat-x";

            constexpr char outbounds[] = "outbounds";
            constexpr char inbounds[] = "inbounds";
            constexpr char settings[] = "settings";
            constexpr char streamSettings[] = "streamSettings";
            constexpr char vnext[] = "vnext";
            constexpr char users[] = "users";
            constexpr char id[] = "id";
            constexpr char port[] = "port";
            constexpr char flow[] = "flow";
            constexpr char network[] = "network";
            constexpr char security[] = "security";
            constexpr char realitySettings[] = "realitySettings";
            constexpr char serverName[] = "serverName";
            constexpr char fingerprint[] = "fingerprint";
        }

        namespace wireguard
        {
            // Config file keys ([Interface] / [Peer] sections) - case-sensitive
            constexpr char PrivateKey[] = "PrivateKey";
            constexpr char Address[] = "Address";
            constexpr char PublicKey[] = "PublicKey";
            constexpr char PresharedKey[] = "PresharedKey";
            constexpr char PreSharedKey[] = "PreSharedKey";
            constexpr char AllowedIPs[] = "AllowedIPs";
            constexpr char Endpoint[] = "Endpoint";
            constexpr char PersistentKeepalive[] = "PersistentKeepalive";
            constexpr char MTU[] = "MTU";

            constexpr char defaultPort[] = "51820";

            constexpr char defaultMtu[] = "1376";
        }

        namespace awg
        {
            constexpr char defaultPort[] = "55424";
            constexpr char defaultMtu[] = "1376";

            constexpr char defaultSpecialJunk1[] = "<r 2><b 0x858000010001000000000669636c6f756403636f6d0000010001c00c000100010000105a00044d583737>";

            constexpr char awgV1_5[] = "1.5";
            constexpr char awgV2[] = "2";
            constexpr char awgV3[] = "3.1";

            constexpr char awgBoolOff[] = "off";
        }
    } // namespace protocols
}

#endif // PROTOCOLCONSTANTS_H
