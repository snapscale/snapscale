<div align=center><img height="150" src="https://github.com/snapscale/snapscale/blob/master/snapscalelogo.png"/></div>
<br>
<br>
<a href="#chinese">点击此处直达中文版 </a>
<br>
<br>
<br>
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

## Resources

1. [Website](https://snapscale.org/)
2. [Whitepaper](./SnapScale%20Whitepaper%20-%20EN.pdf)
3. [Discord](https://discord.gg/wKkjUnQ/)
4. [License](https://github.com/snapscale/snapscale/blob/master/LICENSE)
5. [Contribution](https://github.com/snapscale/snapscale/blob/master/CONTRIBUTING.md)

## Documentation

1. [Nodeos](https://github.com/SnapScale/SnapScale/tree/master/docs/01_nodeos)
    - [Usage](https://github.com/SnapScale/SnapScale/tree/master/docs/01_nodeos/02_usage)
    - [Replays](https://github.com/SnapScale/SnapScale/tree/master/docs/01_nodeos/04_replays)
    - [<font color='red'>Chain API Reference</font>](http://eosio.github.io/eos/latest/nodeos/plugins/chain_api_plugin/api-reference/index)
    - [Troubleshooting](https://github.com/SnapScale/SnapScale/tree/master/docs/01_nodeos/08_troubleshooting)
2. [Cleos](https://github.com/SnapScale/SnapScale/tree/master/docs/02_cleos)
3. [Keosd](https://github.com/SnapScale/SnapScale/tree/master/docs/03_keosd)
4. [Consortium Walkthrough](https://github.com/snapscale/snapscale/blob/master/docs/consortium_walkthrough.md)

See other dependencies:

5. [SnapScale Explorer](https://github.com/snapscale/snapscale.explorer/blob/2.0.0/README.md)
6. [Run on Docker](https://github.com/snapscale/snapscale/tree/master/docker/README.md)
7. [Contracts](https://github.com/snapscale/snapscale.contracts/README.md)
8. [Contracts Development Tools](https://github.com/snapscale/snapscale.cdt/blob/master/README.md) 

## Disclaimer

SnapScale is neither launching nor operating any initial public blockchains based upon the SnapScale software. This release refers only to version 1.0 of our open source software. We caution those who wish to use blockchains built on SnapScale to carefully vet the companies and organizations launching blockchains based on SnapScale before disclosing any private keys to their derivative software.

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
git clone https://github.com/snapscale/snapscale.git

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

## Getting Started
Instructions detailing the process of getting the software, building it, running a simple test network that produces blocks, account creation and uploading a sample contract to the blockchain can be found in the [Getting Started](https://developers.eos.io/welcome/latest/getting-started) walkthrough.

## License

SnapScale is released under the Apache 2.0 license and is offered “AS IS” without warranty of any kind, express or implied. Any security provided by the SnapScale software depends in part on how it is used, configured, and deployed. SnapScale is built upon many third-party libraries such as WABT (Apache License) and WAVM (BSD 3-clause) which are also provided “AS IS” without warranty of any kind.  

## Important

See [LICENSE](./LICENSE) for copyright and license terms.

All repositories and other materials are provided subject to the terms of this [IMPORTANT](./IMPORTANT.md) notice and you must familiarize yourself with its terms.  The notice contains important information, limitations and restrictions relating to our software, publications, trademarks, third-party resources, and forward-looking statements.  By accessing any of our repositories and other materials, you accept and agree to the terms of the notice.


---
<a id="chinese"></a> <br><br>
<div align=center><img height="150" src="https://github.com/snapscale/snapscale/blob/master/snapscalelogo.png"/></div>
<br>
<br>
边缘计算已经成为大多数物联网（IoT）解决方案中不可或缺的技术。随着IoT设备变得更加智能以及自动化，创建分布式的IoT网络对于维持信任以及释放边缘技术的潜力至关重要。SnapScale 旨在用区块链赋能边缘计算，并在网络边缘为分布式IoT网络创建协议。



# SnapScale - 基于多接入边缘计算（MEC）服务器部署分布式应用程序
SnapScale 是面向边缘的新型分布式账本协议，使企业能够在MEC上快速构建和部署高性能和高安全的区块链应用程序。

SnapScale 基于 [EOSIO](https://github.com/EOSIO/eos) 的源代码搭建，并会根据EOS版本的迭代进行更新。

**基于EOSIO 的功能改进和增强**:
- **分布式Certificate Authority (CA) 证书** - 使用智能合约发行数字证书，支持多重签名;
- **节点许可** - 使用智能合约的节点许可功能，确保只有授权的节点才能加入网络;
- **HTTP 许可** -  确保只有授权的客户端才能与节点交流;
- **链上黑名单** - 使用智能合约维护黑名单，确保黑名单用户永久无法通过P2P或HTTP与节点进行交流;
- **UTXO** - 增加了UTXO交易模型，支持余额模型和UTXO模型之间相互进行转账操作;

## 资源

1. [网站](https://snapscale.org/)
2. [白皮书](./SnapScale%20%E6%8A%80%E6%9C%AF%E7%99%BD%E7%9A%AE%E4%B9%A6.pdf)
3. [Discord 社区](https://discord.gg/wKkjUnQ/)
4. [协议许可](https://github.com/snapscale/snapscale/blob/master/LICENSE)
5. [代码贡献](https://github.com/snapscale/snapscale/blob/master/CONTRIBUTING.md)

## 文档

1. [Nodeos](https://github.com/SnapScale/SnapScale/tree/master/docs/01_nodeos)
    - [Usage](https://github.com/SnapScale/SnapScale/tree/master/docs/01_nodeos/02_usage)
    - [Replays](https://github.com/SnapScale/SnapScale/tree/master/docs/01_nodeos/04_replays)
    - [<font color='red'>Chain API Reference</font>](http://eosio.github.io/eos/latest/nodeos/plugins/chain_api_plugin/api-reference/index)
    - [Troubleshooting](https://github.com/SnapScale/SnapScale/tree/master/docs/01_nodeos/08_troubleshooting)
2. [Cleos](https://github.com/SnapScale/SnapScale/tree/master/docs/02_cleos)
3. [Keosd](https://github.com/SnapScale/SnapScale/tree/master/docs/03_keosd)
4. [联盟链入门](https://github.com/snapscale/snapscale/blob/master/docs/consortium_walkthrough.md)

其他依存文件:

5. [SnapScale 浏览器](https://github.com/snapscale/snapscale.explorer/blob/2.0.0/README.md)
6. [在 Docker 上部署](https://github.com/snapscale/snapscale/tree/master/docker/README.md)
7. [智能合约](https://github.com/snapscale/snapscale.contracts/README.md)
8. [智能合约部署工具](https://github.com/snapscale/snapscale.cdt/blob/master/README.md) 

## 免责声明

SnapScale 既不启动也不运行任何基于SnapScale 软件的初始公共区块链。此版本仅引用我们开源软件的1.0版。在此，我们告诫那些希望使用基于SnapScale 构建的区块链的企业和机构在向其衍生软件披露任何私钥之前，请仔细审查基于SnapScale 进行区块链开发的公司和组织。


## 支持的操作系统

目前，SnapScale支持以下操作系统:  

1. Ubuntu 16.04
2. Ubuntu 18.04
3. MacOS 10.14 (Mojave)



**注意：您或许可以在其他基于Unix的操作系统上安装SnapScale。不过，此操作目前未得到官方支持。**

---

## 构建源码

```sh
git clone https://github.com/snapscale/snapscale.git

cd SnapScale

git submodule update --init --recursive

./scripts/eosio_build.sh
```

**注意：如果您使用我们的脚本来构建或安装SnapScale，请在使用预构建二进制包之前运行 [卸载脚本](#uninstall-script) 。**

## 软件安装 

```sh
./scripts/eosio_install.sh
```

## 软件卸载

To uninstall the EOSIO built/installed binaries and dependencies, run:

```sh
./scripts/eosio_uninstall.sh
```

## 快速入门
您可以在[快速入门](https://developers.eos.io/welcome/latest/getting-started)中找到关于如何获取软件、构建软件，以及简单搭建可以创建账户并将智能合约上传至区块链的测试网。

## 许可协议

SnapScale 遵循Apache 2.0 开源协议发布，按“原样”提供，没有任何明示或暗示的担保。SnapScale 软件提供的任何安全性部分取决于它的使用，配置和部署方式。 SnapScale 建立在许多第三方库上，如Binaryen（Apache许可证）和WAVM（BSD 3-clause），它们也是“按现状”提供的，没有任何形式的保证。


## 重要事项

有关版权和许可条款，请参考[许可协议](./LICENSE) 。

我们提供的所有信息都受限于本[重要通知](./IMPORTANT.md) 您必须熟悉其中的条款。该通知包含了关于我们的软件、发布、商标、第三方资源和前瞻性声明的重要信息、条件和限制。您获取任何材料的时候就意味着您接受并同意该通知的条款。
