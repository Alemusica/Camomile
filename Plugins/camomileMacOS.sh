#!/bin/bash

CamomileInst=Camomile
CamomileFx=CamomileFx
VstExtension=vst
Vst3Extension=vst3
AuExtension=component
ThisPath="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

################################################################################
#           Install all the plugins from ./Builds to a destination             #
################################################################################
install_plugin() {
    local InstallationPath
    if [ "$2" == "$VstExtension" ]; then
        InstallationPath=$3/VST
    elif [ "$2" == "$Vst3Extension" ]; then
        InstallationPath=$3/VST3
    elif [ "$2" == "$AuExtension" ]; then
        InstallationPath=$3/Components
    else
        echo -e "\033[31m"$PluginName.$PluginExtension" extension not recognized\033[0m"
        return
    fi

    if [ -d $InstallationPath/$1.$2 ]; then
        rm -rf $InstallationPath/$1.$2
    fi

    cp -rf $ThisPath/Builds/$1.$2 $InstallationPath/$1.$2
    echo $PluginName.$PluginExtension" in "$InstallationPath
}

install_all_plugins() {
    if [ -d $1 ]; then
        echo  -e "\033[1;30mInstalling Plugins to "$1"\033[0m"
        for Plugin in $ThisPath/Builds/*
        do
            local PluginName=$(basename "$Plugin")
            local PluginExtension="${PluginName##*.}"
            local PluginName="${PluginName%.*}"
            install_plugin $PluginName $PluginExtension $1
        done
        echo -e "\033[1;30mFinished\033[0m"
    else
        echo -e "\033[31m"$1" invalid path\033[0m"
    fi
}

################################################################################
#                       Clean all the plugins from ./Builds                    #
################################################################################

clean_plugin() {
    if [ "$2" == "$VstExtension" ] || [ "$2" == "$Vst3Extension" ] || [ "$2" == "$AuExtension" ]; then
        rm -rf $ThisPath/Builds/$1.$2
        echo $PluginName.$PluginExtension
    else
        echo -e "\033[31m"$PluginName.$PluginExtension" extension not recognized\033[0m"
        return
    fi
}

clean_all_plugins() {
    echo  -e "\033[1;30mCleaning Plugins\033[0m"
    for Plugin in $ThisPath/Builds/*
    do
        local PluginName=$(basename "$Plugin")
        local PluginExtension="${PluginName##*.}"
        local PluginName="${PluginName%.*}"
        clean_plugin $PluginName $PluginExtension

    done
    echo -e "\033[1;30mFinished\033[0m"
}

################################################################################
#                       Generate all the plugins from ./Builds                    #
################################################################################

generate_plugin_vst() {
    if [ -d $ThisPath/Camomile/$1.$VstExtension ]; then
        if [ -d $ThisPath/Builds/$3.$VstExtension ]; then
            rm -rf $ThisPath/Builds/$3.$VstExtension
        fi

        cp -rf $ThisPath/Camomile/$1.$VstExtension/ $ThisPath/Builds/$3.$VstExtension
        cp -rf $2/$3/ $ThisPath/Builds/$3.$VstExtension/Contents/Resources
        echo -n $VstExtension " "
    else
        echo -n -e "\033[2;30m"$VstExtension" \033[0m"
    fi
}

generate_plugin_vst3() {
    if [ -d $ThisPath/Camomile/$1.$Vst3Extension ]; then
        if [ -d $ThisPath/Builds/$3.$Vst3Extension ]; then
            rm -rf $ThisPath/Builds/$3.$Vst3Extension
        fi

        cp -rf $ThisPath/Camomile/$1.$Vst3Extension/ $ThisPath/Builds/$3.$Vst3Extension
        cp -rf $2/$3/ $ThisPath/Builds/$3.$Vst3Extension/Contents/Resources
        echo -n $Vst3Extension " "
    else
        echo -n -e "\033[2;30m"$Vst3Extension" \033[0m"
    fi
}

au_get_plugin_code() {
    while IFS='' read -r line || [[ -n "$line" ]]; do
        local wline=($line)
        if [ "${wline[0]}" == "code" ]; then
            echo ${wline[1]}
            return
        fi
    done < "$1"
    echo "0"
}

au_get_plugin_author() {
    while IFS='' read -r line || [[ -n "$line" ]]; do
        local wline=($line)
        if [ "${wline[0]}" == "author" ]; then
            echo ${wline[1]}
            return
        fi
    done < "$1"
    echo "0"
}

generate_plugin_au() {
    if [ ! -f $2/$3/$3.txt ]; then
        echo -n -e "\033[31m(No config file) \033[0m"
        return
    fi

    local code=$(au_get_plugin_code $2/$3/$3.txt)
    if [ "$code" == "0" ]; then
        echo -n -e "\033[31mNo plugin code defined\033[0m"
        return
    fi
    code=${code::4}
    if [ -d $ThisPath/Camomile/$1.$AuExtension ]; then
        if [ -d $ThisPath/Builds/$3.$AuExtension ]; then
            rm -rf $ThisPath/Builds/$3.$AuExtension
        fi

        cp -rf $ThisPath/Camomile/$1.$AuExtension/ $ThisPath/Builds/$3.$AuExtension
        cp -rf $2/$3/ $ThisPath/Builds/$3.$AuExtension/Contents/Resources
        plutil -replace AudioComponents.name -string $3 $ThisPath/Builds/$3.$AuExtension/Contents/Info.plist
        plutil -replace AudioComponents.subtype -string $code $ThisPath/Builds/$3.$AuExtension/Contents/Info.plist
        echo -n $AuExtension
    else
        echo -n -e "\033[2;30m"$AuExtension"\033[0m"
    fi
}

generate_plugins() {
    echo -n $PluginName "("
    generate_plugin_vst $CamomileName $PatchesPath $PluginName
    generate_plugin_vst3 $CamomileName $PatchesPath $PluginName
    generate_plugin_au $CamomileName $PatchesPath $PluginName
    echo ")"
}

generate_all_plugins() {
    local CamomileName
    local PatchesPath
    if [ "$1" == "Effects" ]; then
        CamomileName=$CamomileFx
        PatchesPath=$ThisPath/Effects
    elif [ "$1" == "Instruments" ]; then
        CamomileName=$CamomileInst
        PatchesPath=$ThisPath/Instruments
    else
        echo -e "\033[31mWrong type of plugins\033[0m"
    fi


    if [ ! -d $PatchesPath ]; then
        echo -e "\033[31m"$2" wrong arguments\033[0m"
        return
    fi

    echo  -e "\033[1;30mGenerating Effects Plugins\033[0m"
    for Patch in $PatchesPath/*
    do
      if [ -d $Patch ]; then
          local PluginName=$(basename "$Patch")
          local PluginExtension="${PluginName##*.}"
          local PluginName="${PluginName%.*}"
          generate_plugins $CamomileName $PatchesPath $PluginName
      fi
    done
    echo -e "\033[1;30mFinished\033[0m"
}

################################################################################
#                                   Main method                                #
################################################################################

if [ ! -d $ThisPath/Builds ]; then
    mkdir $ThisPath/Builds
fi

if [ "$1" == "install" ]; then
    if [ -z "$2" ]; then
        install_all_plugins $HOME/Library/Audio/Plug-Ins
    else
        install_all_plugins $2
    fi
elif [ "$1" == "generate" ]; then
    if [ "$2" == "effects" ]; then
        generate_all_plugins Effects
    elif [ "$2" == "instruments" ]; then
        generate_all_plugins Instruments
    elif [ -z "$2" ]; then
        generate_all_plugins Effects
        generate_all_plugins Instruments
    else
        echo -e "\033[31m"$2" wrong arguments\033[0m"
    fi
elif [ "$1" == "clean" ]; then
    clean_all_plugins
elif [ -z "$1" ]; then
    echo -e "\033[31mArguments required\033[0m"
else
    echo -e "\033[31m"$1" wrong arguments\033[0m"
fi
