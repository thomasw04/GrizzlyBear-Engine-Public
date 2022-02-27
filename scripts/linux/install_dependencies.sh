#Script to install libraries for the engine

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

pushd ../../

./libs/vcpkg/bootstrap-vcpkg.sh -disableMetrics

while IFS= read -r line; do
    echo "Trying to install $line..."
    ./libs/vcpkg/vcpkg install "$line:x64-linux"
done < packages.txt

pushd projects/LuaJit/luajit

make
make install PREFIX="${SCRIPTPATH}/../../projects/LuaJit/luajit/build"

popd

echo "Installed dependencies. Ready for run."

popd
