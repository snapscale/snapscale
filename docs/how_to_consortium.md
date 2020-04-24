# Guides to use consortium

## How to use the Snapscale Consortium as a node

Before starting the node, you need to configure the config.ini file as follows:

``` ini
root-public-key = XST71cZdtoJbJyC89sTpUCa14nMCKMv95QzWSZdUKo9FcbusJxy9A
league-public-key = XST5z8Yc9tUj1naSnYPf49XE644bRib6VP3yFM8w9SQ7SHKsrCSNQ
league-private-key = 5K8jggGd1iEqHVUXCHd3z3uvFHgCXvQbveRrM3wKDp3YvLL5f2T
signature-by-root = SIG_K1_KhiiSy1uukNfNKc1K1LpjuFPFjCxEM5yqQ3xYwTXRxiDMHPUNhVfcvfbRgyCZWGNuwXR8qQ25kPmCrWNPghiwofYynNB46
```

1. root-public-key：The public key of the key pair used by the alliance signature
2. league-public-key:Node's public key
3. league-private-key:Node's private key
4. signature-by-root:The node's signature information is the signature certificate issued by the alliance to this node

## How to use the Snapscale Consortium Chain as a user or developer

When accessing Snapscale alliance chain data as a client, you need to add token information to the http request in the following format:

```shell
"Authorization:eyJhbGciOiJIUzI1NiJ9.eyJleHAiOjE1ODU3MjcwNzAsImlhdCI6MTU4NTcyMzQ3MCwiaXNzIjoieHN0IiwicHVibGljIjoiWFNUNXo4WWM5dFVqMW5hU25ZUGY0OVhFNjQ0YlJpYjZWUDN5Rk04dzlTUTdTSEtzckNTTlEiLCJzaWduYXR1cmUiOiJTSUdfSzFfS2hpaVN5MXV1a05mTktjMUsxTHBqdUZQRmpDeEVNNXlxUTN4WXdUWFJ4aURNSFBVTmhWZmN2ZmJSZ3lDWldHTnV3WFI4cVEyNWtQbUNyV05QZ2hpd29mWXluTkI0NiJ9.lVSBsqsf7kTIlHLygYlmT0PxivYSPP3drD-rvHxwsHs"
```

For example, if you want to view information through cleos, the command is as follows:

```shell
cleos --header "Authorization:eyJhbGciOiJIUzI1NiJ9.eyJleHAiOjE1ODU3MjcwNzAsImlhdCI6MTU4NTcyMzQ3MCwiaXNzIjoieHN0IiwicHVibGljIjoiWFNUNXo4WWM5dFVqMW5hU25ZUGY0OVhFNjQ0YlJpYjZWUDN5Rk04dzlTUTdTSEtzckNTTlEiLCJzaWduYXR1cmUiOiJTSUdfSzFfS2hpaVN5MXV1a05mTktjMUsxTHBqdUZQRmpDeEVNNXlxUTN4WXdUWFJ4aURNSFBVTmhWZmN2ZmJSZ3lDWldHTnV3WFI4cVEyNWtQbUNyV05QZ2hpd29mWXluTkI0NiJ9.lVSBsqsf7kTIlHLygYlmT0PxivYSPP3drD-rvHxwsHs"  get info
```

For token generation, you can request the token generation from the wallet through the following command:

```shell
cleos wallet create_token --pubkey XST5z8Yc9tUj1naSnYPf49XE644bRib6VP3yFM8w9SQ7SHKsrCSNQ --signature SIG_K1_KhiiSy1uukNfNKc1K1LpjuFPFjCxEM5yqQ3xYwTXRxiDMHPUNhVfcvfbRgyCZWGNuwXR8qQ25kPmCrWNPghiwofYynNB46 --svr-pubkey XST83jABXUkPLcpeA5fbgKfokdd5YSP6k6n87wpYrApfW7rY9MqT6
```

The returned results are as follows:

```shell
"Authorization:eyJhbGciOiJIUzI1NiJ9.eyJleHAiOjE1ODY1MDU3NTAsImlhdCI6MTU4NjUwMjE1MCwiaXNzIjoieHN0IiwicHVibGljIjoiWFNUNXo4WWM5dFVqMW5hU25ZUGY0OVhFNjQ0YlJpYjZWUDN5Rk04dzlTUTdTSEtzckNTTlEiLCJzaWduYXR1cmUiOiJTSUdfSzFfS2hpaVN5MXV1a05mTktjMUsxTHBqdUZQRmpDeEVNNXlxUTN4WXdUWFJ4aURNSFBVTmhWZmN2ZmJSZ3lDWldHTnV3WFI4cVEyNWtQbUNyV05QZ2hpd29mWXluTkI0NiJ9.eV0QIMHkSOZ2GOODAgv5s9PLAWf9DFmsn7H06FTs0qY"
```

```shell
svr-pubkey  ：he public key that http request will send to the server
pubkey      ：The public key of the HTTP request sender
signature   ：Alliance signing certificate issued by sending alliance

Authorization：The token returned from the wallet

⚠️Note: To successfully generate the token, the wallet containing the pubkey public and private keys needs to be unlocked
```
