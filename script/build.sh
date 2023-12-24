#!/bin/bash

base_path=$(cd `dirname $0`; pwd)

root_path=$base_path/..
build_path=$root_path/build
lib_path=$root_path/third_party

function check_glm()
{
    #glm_hpp_content=`cat $lib_path/glm/CMakeLists.txt`
    #for line in $text; do
    #done
    glm_version_line=$(grep -Ew 'GLM_VERSION' $lib_path/glm/CMakeLists.txt)
    #echo "check_glm: $glm_version_line"
    if [[ "$glm_version_line" =~ \"([0-9]+).*\" ]];then 
        glm_version_match_str=${BASH_REMATCH[0]}
        glm_version_match=${glm_version_match_str//\"/}
        str_glm_version_specified=\"$1\"
        #echo "find version in glm: $glm_version_match in $str_glm_version_specified"
        #echo "str_glm_version: $str_glm_version_specified"
        if [[ $str_glm_version_specified == *"$glm_version_match"* ]]; then 
            #echo "glm version match!"
            echo "True"
            return 1
        else
            #echo "glm version does not match!"
            echo "False"
            return 0
        fi
    fi
    echo "False"
    return 0
}

function build_uni_io() {
    if [[ ! -d "$build_path" ]]; then 
        echo "create cmake build.."
        mkdir -p $build_path
    fi
    cmake  -S$root_path -B$build_path       \
        -DUSD_ROOT_DIR=$lib_path/usd        \
        -DBOOST_ROOT_DIR=$lib_path/boost    \
        -DTBB_ROOT_DIR=$lib_path/tbb        \
        -B$build_path -H$root_path
    cmake --build $build_path --config Release
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

#if [[ -d "$lib_path/glm" ]]; then 
#    no_update_glm=$(check_glm $glm_version)
#    echo "check glm $no_update_glm"
#    if  [[ "$no_update_glm" == "True" ]] ;then 
#        echo "glm lib exist!"
#    else
#        rm -rf $lib_path/glm
#        download_glm $glm_version
#    fi
#else
#     download_glm $glm_version
#fi

#download glm lib
#download_glm $glm_version

build_uni_io




