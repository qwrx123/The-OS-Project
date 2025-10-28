# The OS Project

## Setup Instructions

### Windows

1. Open PowerShell command prompt with Admin privilege and run the following commands:

```powershell
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
wsl --set-default-version 2
wsl --install -d Debian
```

2. Run Debian

### Linux

#### 1. Download Prerequisite Programs

**Debian:**
```bash
sudo apt install build-essential
```

**Arch:**
```bash
sudo pacman -S base-devel
```

#### 2. Download Cross-Compiling Toolchain

**Debian:**

1. Go to [ARM GNU Toolchain Downloads](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
2. Find x86_64 Linux hosted cross toolchains and copy the link to AArch64 bare-metal target
3. Run the following commands (replace `<target>` with the downloaded filename):

```bash
wget <target>
sudo tar -xf --no-same-owner <target>
sudo rm <target>/{*manifest.txt,licence.txt}
sudo cp -rv <target>/* /usr
sudo rm -rf <target>
```

**Arch:**
```bash
yay -S aarch64-none-elf-toolchain
```

**Gentoo:**

See the [Gentoo Crossdev Wiki](https://wiki.gentoo.org/wiki/Crossdev) for instructions.