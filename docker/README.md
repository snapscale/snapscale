# Run in docker

Simple and fast setup of SnapScale on Docker is also available.

## Install Dependencies

- [Docker](https://docs.docker.com) Docker 18.09 or higher is required

## Docker Requirement

- At least 7GB RAM (Docker -> Preferences -> Advanced -> Memory -> 7GB or above)
- If the build below fails, make sure you've adjusted Docker Memory settings and try again.

## Build SnapScale image

```bash
git clone https://github.com/snapscale/snapscale.git
git submodule update --init --recursive
cd snapscale/docker
docker build . -t snapscale/snapscale
```

The above will build off the most recent commit to the master branch by default.


## Start single node docker container

```bash
docker run --name nodexst -p 8888:8888 -p 9876:9876 -td snapscale/snapscale boot.sh --league-public-key=<xxxxxx> --league-private-key=<yyyyyy> --signature-by-root=<zzzzz> --root-public-key=<uuuuuu>
```

By default, all data is stored in a container. It will be deleted if the container is deleted.


Alternately, you can directly mount host directory into the container, or use the docker volume.

```bash
docker run --name nodexst -v /path-to-data-dir:/home/user/data -p 8888:8888 -p 9876:9876 -td snapscale/snapscale boot.sh --league-public-key=<xxxxxx> --league-private-key=<yyyyyy> --signature-by-root=<zzzzz> --root-public-key=<uuuuuu>
```

## Get chain info

```bash
curl http://127.0.0.1:8888/v1/chain/get_info
```
