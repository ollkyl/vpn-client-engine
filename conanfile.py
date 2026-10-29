from conan import ConanFile


class VpnEngine(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "VirtualBuildEnv", "CMakeConfigDeps"

    def requirements(self):
        self.requires("awg-windows/3.1.20260814")
        self.requires("tap-windows6/9.27.0")
        self.requires("win-split-tunnel/1.2.5.0")
        self.requires("wintun/0.14.1")

        self.requires("amnezia-xray-bindings/1.4.0")
        self.requires("tun2socks/2.6.0")
        self.requires("openvpn/2.7.0")
        self.requires("v2ray-rules-dat/202603162227")

        self.requires("openssl/3.6.2")
        self.requires("zlib/1.3.2")
