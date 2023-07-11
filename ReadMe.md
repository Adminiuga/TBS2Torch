# Thunderboard Sense 2 Torch Light

## What is this?

This is a Zigbee Light running on [Thunderboard Sense 2](https://www.silabs.com/documents/public/user-guides/ug309-sltb004a-user-guide.pdf) *"the ultimate multi-sensor, multiprotocol cloud inspiration kit"*. This is also my toy project in an attempt to learn the Zigbee Application Framework. The project is based on Zigbee 3 Light example project, with some addion like hall sensor, IMU sensor etc to provide some interface for the light and device control.

## But why is it the Torch Light?

Well, my kids like playing Minecraft, I like playing Minecraft and when I saw this <img align="right" width="100" height="100" src="Docs/Media/minecraft-torch-toy.png">
online, I just couldn't resist the impulsive buying and ordered a few. Now, I don't have anything against those lights, but when all you other lights in the house are controlled by [Home Assistant](https://www.home-assistant.io/) Zigbee integration, pressing the button to turn the lights on or off just didn't feel right and I knew from the begining, that I wanted these lights to be Zigbee. However, at this moment I've decided against cutting those lights open and instead decided to 3d print the [Minecraft Torch Nightlight][3d Source Torch Light] with a custom [torch head](3D%20Models/TBS2-Torch-head.stl) torch head to hold the TBS2 board. In the future, I need to find a replacement for TBS2. [BRD2601b](https://www.silabs.com/documents/public/user-guides/ug524-brd2601b-user-guide.pdf) is a tad expensive and supports Matter. Or maybe just build some custom board, because for now I just cannot find any other board that at least has an IMU and a microphone.

## Installation

### Installation using JLink Mass Storage Device

For the very 1st time, you would need to install both the boot loader and the application. For updates, you either could download just the app update or use the OTA imager for the over-the-air update.

Head over to [TBS2-Torch Release](https://github.com/Adminiuga/TBS2Torch/releases), find the a release with the bootloader, download the bootloader artifact and copy `app_btl-combined.s37` to `TB004` Thunderboard Sense 2 mass storage device. Once copied, the device will reboot.

Next, download the application artifact archive and copy `TBS2Torch.hex` to `TB004` mass storage device.

### Installation using Simplicity Commander

Alternatively, if you have [Simplicity Commander][Simplicity Commander] installed, you can use the simplicity commander to flash the same files. At least once, you would need to flash the `app_btl-combined.s37`, for the upgrades, just flash the `TBS2Torch.gbl` or `TBS2Torch.hex`


```bash
  slc generate \
      --project-file="/src/TBS2Torch2/TBS2Torch2.slcp" \
      --export-destination=/build/TBS2-Torch \
      --copy-proj-sources --new-project --force \
      --configuration=""
```

Then build it with

```bash
make -f TBS2Torch2.Makefile release
```

[Reference Table]: #
[3d Source Torch Light]: https://www.thingiverse.com/thing:2002669 "3D Torch Light"
[Simplicity Commander]: https://community.silabs.com/s/article/simplicity-commander?language=en_US "Simplicity Commander"
