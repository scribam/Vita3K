#!/bin/bash -ex

docker run --user $(id -u):$(id -g) -v $(pwd):/Vita3K -v "$HOME/.ccache":/root/.ccache gcc:8 /bin/bash -ex /Vita3K/.travis/linux/docker.sh || exit $?
