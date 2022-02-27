#This script generates project files for linux wayland

ide="gmake2"

if ["$1"="gmake"]; then
    ide="gmake"
if ["$1"="gmake2"]; then
    ide="gmake2"
if ["$1"="codelite"]; then
    ide="codelite"
if ["$1"="clion"]; then
    ide="clion"

editor="true"

if ["$2"="true"]; then
    editor="true"
if ["$2"="false"]; then
    editor="false"

pushd ../../

./libs/premake/premake5-linux clean
./libs/premake/premake5-linux $ide --generateEditor=$editor --linuxWindowSys=wayland

popd

echo "GrizzlyBear $ide project files were generated. Ready for run."