#!/bin/bash

base_path=$(cd `dirname $0`; pwd)

root_path=$base_path/..
build_path=$root_path/build
lib_path=$root_path/third_party


function build_uni_io() {
    if [[ ! -d "$build_path" ]]; then 
        echo "create cmake build.."
        mkdir -p $build_path
    fi
    cmake                                   \
        -DUSD_ROOT_DIR=$lib_path/usd        \
        -DBOOST_ROOT_DIR=$lib_path/boost    \
        -DTBB_ROOT_DIR=$lib_path/tbb        \
        -B$build_path -H$root_path
}

function download_glm() {
    echo "download glm version: $1"
    eigen_download_url=https://github.com/g-truc/glm/releases/download/$1/glm-$1.zip
    wget -O $build_path/glm.zip -P $build_path  $eigen_download_url
    if [ -f "$build_path/glm.zip" ] 
    then 
        unzip -d $lib_path/ $build_path/glm.zip
        echo "download glm success!"
    else
        echo "error: download glm failed!"
    fi
}

source $base_path/version.sh
glm_version=$GLM_VERSION

echo "glm verson: $glm_version"

if [[ -d "$lib_path/glm" ]]; then 
    echo "glm lib exist!"
fi

#download glm lib
download_glm $glm_version




