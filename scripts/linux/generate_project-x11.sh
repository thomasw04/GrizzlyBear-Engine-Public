#!/bin/bash
#This script generates project files for linux x11

ide="gmake2"

if [ $1 = 'gmake' ]; then
    ide="gmake"
fi

if [ "$1" = "gmake2" ]; then
    ide="gmake2"
fi

if [ "$1" = "codelite" ]; then
    ide="codelite"
fi

if [ "$1" = "clion" ]; then
    ide="clion"
fi

editor="true"

if [ "$2" = "true" ]; then
    editor="true"
fi

if [ "$2" = "false" ]; then
    editor="false"
fi


pushd ../../

./libs/premake/premake5-linux clean
./libs/premake/premake5-linux $ide --generateEditor=$editor --linuxWindowSys=x11

popd

echo "GrizzlyBear $ide project files were generated. Ready for run."

