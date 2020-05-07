Edge computing has become a very important aspect of most IoT Solutions. As these IoT devices gain more intelligence and become more autonomous, creating a decentralized IoT network becomes crucial to maintain trust and unleash the potential of edge technology. The goal of SnapScale is to leverage blockchain to make edge computing more robust and create a protocol for a decentralized IoT network at the network edge.

# SnapScale – Deploying Decentralized Applications through MEC

SnapScale is a distributed ledger protocol for Multi-access Edge Computing (MEC).

This software enables businesses to rapidly build and deploy high-performance and high-security blockchain-based applications on MEC.

SnapScale is a fork of [EOSIO](https://github.com/EOSIO/eos) and is updated in line with EOS releases.

**Key enhancements over EOSIO**:
- **Decentralized CA** - use smart contracts to issue digital certificates, support issuance by multiple signing thresholds
- **Peer Permissioning** - node/peer permissioning using smart contracts, ensuring only known parties can join the network
- **HTTP Permissioning** - ensure that only authorized clients can interact with the node
- **On-chain Blacklists** - use smart contracts to maintain blacklists and ensure that perpetrators can no longer use P2P or HTTP to interact with nodes
- **UTXO** - add UTXO-transaction model, supporting cross-transactions between the balance model and the UTXO model


## Disclaimer

SnapScale is neither launching nor operating any initial public blockchains based upon the SnapScale software. This release refers only to version 1.0 of our open source software. We caution those who wish to use blockchains built on SnapScale to carefully vet the companies and organizations launching blockchains based on SnapScale before disclosing any private keys to their derivative software.

## Useful Links

[<font color='red'>testnet.snapscale.io</font>](https://testnet.snapscale.io/)


## Official Testnet

1. [Foundation website](https://snapscale.org/)
2. [Whitepaper](xxx)
3. [Discord](https://discord.gg/wKkjUnQ/)
4. [License](https://github.com/snapscale/snapscale/blob/master/LICENSE)
5. [Contributing](https://github.com/snapscale/snapscale/blob/master/CONTRIBUTING.md)

## Supported Operating Systems

SnapScale currently supports the following operating systems:  

1. Ubuntu 16.04
2. Ubuntu 18.04
3. MacOS 10.14 (Mojave)

---

**Note: It may be possible to install SnapScale on other Unix-based operating systems. This is not officially supported, though.**

---

## Building the source

```sh
git clone https://github.com/xeniroio/SnapScale

cd SnapScale

git submodule update --init --recursive

./scripts/eosio_build.sh
```

**Note: If you used our scripts to build/install SnapScale, please run the [Uninstall Script](#uninstall-script) before using our prebuilt binary packages.**

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
1. [Nodeos](https://github.com/SnapScale/SnapScale/tree/master/docs/01_nodeos)
    - [Usage](https://github.com/SnapScale/SnapScale/tree/master/docs/01_nodeos/02_usage)
    - [Replays](https://github.com/SnapScale/SnapScale/tree/master/docs/01_nodeos/04_replays)
    - [<font color='red'>Chain API Reference</font>](http://eosio.github.io/eos/latest/nodeos/plugins/chain_api_plugin/api-reference/index)
    - [Troubleshooting](https://github.com/SnapScale/SnapScale/tree/master/docs/01_nodeos/08_troubleshooting)
2. [Cleos](https://github.com/SnapScale/SnapScale/tree/master/docs/02_cleos)
3. [Keosd](https://github.com/SnapScale/SnapScale/tree/master/docs/03_keosd)
4. [Consortium](https://github.com/SnapScale/SnapScale/tree/master/docs/how_to_consortium.md)

## Resources
1. [Website](https://SnapScale.org/)
2. [White Paper]()

## Getting Started
Instructions detailing the process of getting the software, building it, running a simple test network that produces blocks, account creation and uploading a sample contract to the blockchain can be found in the [Getting Started](https://developers.eos.io/welcome/latest/getting-started) walkthrough.

## License

SnapScale is released under the Apache 2.0 license and is offered “AS IS” without warranty of any kind, express or implied. Any security provided by the SnapScale software depends in part on how it is used, configured, and deployed. SnapScale is built upon many third-party libraries such as WABT (Apache License) and WAVM (BSD 3-clause) which are also provided “AS IS” without warranty of any kind.  

## Important

See [LICENSE](./LICENSE) for copyright and license terms.

All repositories and other materials are provided subject to the terms of this [IMPORTANT](./IMPORTANT.md) notice and you must familiarize yourself with its terms.  The notice contains important information, limitations and restrictions relating to our software, publications, trademarks, third-party resources, and forward-looking statements.  By accessing any of our repositories and other materials, you accept and agree to the terms of the notice.
