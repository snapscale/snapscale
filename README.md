
# SNAPSCALE - The Most Powerful Infrastructure for Decentralized Applications

[![Build status](https://badge.buildkite.com/370fe5c79410f7d695e4e34c500b4e86e3ac021c6b1f739e20.svg?branch=master)](https://buildkite.com/EOSIO/eosio)

Welcome to the SNAPSCALE source code repository! This software enables businesses to rapidly build and deploy high-performance and high-security blockchain-based applications.

Some of the groundbreaking features of SNAPSCALE include:

1. Free Rate Limited Transactions
1. Low Latency Block confirmation (0.5 seconds)
1. Low-overhead Byzantine Fault Tolerant Finality
1. Designed for optional high-overhead, low-latency BFT finality
1. Smart contract platform powered by WebAssembly
1. Designed for Sparse Header Light Client Validation
1. Scheduled Recurring Transactions
1. Time Delay Security
1. Hierarchical Role Based Permissions
1. Support for Biometric Hardware Secured Keys (e.g. Apple Secure Enclave)
1. Designed for Parallel Execution of Context Free Validation Logic
1. Designed for Inter Blockchain Communication

## Disclaimer

SNAPSCALE is neither launching nor operating any initial public blockchains based upon the SNAPSCALE software. This release refers only to version 1.0 of our open source software. We caution those who wish to use blockchains built on SNAPSCALE to carefully vet the companies and organizations launching blockchains based on SNAPSCALE before disclosing any private keys to their derivative software.

## Official Testnet

[<font color='red'>testnet.snapscal.io</font>](https://testnet.eos.io/)

## Supported Operating Systems

SNAPSCALE currently supports the following operating systems:  

1. Ubuntu 16.04
2. Ubuntu 18.04
3. MacOS 10.14 (Mojave)

---

**Note: It may be possible to install SNAPSCALE on other Unix-based operating systems. This is not officially supported, though.**

---

## Building the source

```sh
git clone https://github.com/xeniroio/snapscale

cd snapscale

git submodule update --init --recursive

./scripts/eosio_build.sh
```

**Note: If you used our scripts to build/install SNAPSCALE, please run the [Uninstall Script](#uninstall-script) before using our prebuilt binary packages.**

## Software Installation

```sh
./scripts/eosio_install.sh
```

## Software Uninstall

To uninstall the EOSIO built/installed binaries and dependencies, run:

```sh
./scripts/eosio_uninstall.sh
```

## Documentation
1. [Nodeos](https://github.com/snapscale/snapscale/tree/master/docs/01_nodeos)
    - [Usage](https://github.com/snapscale/snapscale/tree/master/docs/01_nodeos/02_usage)
    - [Replays](https://github.com/snapscale/snapscale/tree/master/docs/01_nodeos/04_replays)
    - [<font color='red'>Chain API Reference</font>](http://eosio.github.io/eos/latest/nodeos/plugins/chain_api_plugin/api-reference/index)
    - [Troubleshooting](https://github.com/snapscale/snapscale/tree/master/docs/01_nodeos/08_troubleshooting)
2. [Cleos](https://github.com/snapscale/snapscale/tree/master/docs/02_cleos)
3. [Keosd](https://github.com/snapscale/snapscale/tree/master/docs/03_keosd)

## Resources
1. [Website](https://snapscale.org/)
2. [White Paper]()

## Getting Started
Instructions detailing the process of getting the software, building it, running a simple test network that produces blocks, account creation and uploading a sample contract to the blockchain can be found in the [Getting Started](https://developers.eos.io/welcome/latest/getting-started) walkthrough.

## License

SNAPSCALE is released under the open source [MIT](./LICENSE) license and is offered “AS IS” without warranty of any kind, express or implied. Any security provided by the SNAPSCALE software depends in part on how it is used, configured, and deployed. SNAPSCALE is built upon many third-party libraries such as WABT (Apache License) and WAVM (BSD 3-clause) which are also provided “AS IS” without warranty of any kind. Without limiting the generality of the foregoing, Block.one makes no representation or guarantee that SNAPSCALE or any third-party libraries will perform as intended or will be free of errors, bugs or faulty code. Both may fail in large or small ways that could completely or partially limit functionality or compromise computer systems. If you use or implement SNAPSCALE, you do so at your own risk. In no event will Block.one be liable to any party for any damages whatsoever, even if it had been advised of the possibility of damage.  

## Important

See [LICENSE](./LICENSE) for copyright and license terms.

All repositories and other materials are provided subject to the terms of this [IMPORTANT](./IMPORTANT.md) notice and you must familiarize yourself with its terms.  The notice contains important information, limitations and restrictions relating to our software, publications, trademarks, third-party resources, and forward-looking statements.  By accessing any of our repositories and other materials, you accept and agree to the terms of the notice.
