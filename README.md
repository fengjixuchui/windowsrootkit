# windowsrootkit <img src="https://forthebadge.com/images/badges/made-with-c-plus-plus.svg" /> <img src="https://forthebadge.com/images/badges/built-with-love.svg" />
### Combination of my code and other's code, adapted to a specific standard.

- Tested on ***Windows x86_64 1909***, local.

- Tested on ***VMWare Player 15, machine: Windows x86_64 1909***, local: Ubuntu 19.10.


## Getting started
- @cristeigabriel or any of the people listed below are not responsible for any action that's to happen due to this project being published. The code is distributed for learning purposes.


windowsrootkit is a rootkit that's supposed to be compiled as a DLL (Dynamic Library Link) and to be injected within any process that can handle it. It can hook WinApi functions, keylog, take screenshots, and more to come... It is **NOT** set up to send data to a server, and won't be, to avoid possible cyber attacks made by people with no knowledge.


## Why should I use this over any other rootkit?
Due to it's nature, which is, being an injected DLL, unless there's a 'counter attack' on the functions used to inject it into any module, there's no way, whatsoever, to find out if the malware is indeed running, thus, it makes actions like spying, or just general hacking, easier. Currently, no anti-virus detects these actions. This CAN be embed into any DLL project that runs on the C++17 language standard. It also provides an obfuscation engine out of the box (that's not meant to take the place of any other obfuscation tool, it's just to make reverse engineering harder by default, or general detection). It also has cryptology tools, i.e. a heavily obfuscated version of XOR encryption [compile time], a FNV-1a algorithm that works with multiple datatypes [run time].


## Projects that made it possible
- APIHook by @Zer0Mem0ry
- Heavily Vectorized XOR by @JustasMasiulis
- Obfusion by @kgretzky

## Licensing
- windowsrootkit is licensed under the GPL v2 license. Read more about it online.
