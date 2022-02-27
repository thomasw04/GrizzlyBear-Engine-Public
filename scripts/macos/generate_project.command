#!/bin/bash
cd "$( dirname "$0" )"

echo "Enter project name: "
read prjName
echo "Enter start mode (editor, standalone): "
read staMode

pushd ../
libs/premake/premake5-macOS clean
libs/premake/premake5-macOS xcode4 --projectName=$prjName --startMode=$staMode
popd

echo Grizzly Bear Engine generated and ready for run.