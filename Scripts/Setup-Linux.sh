#!/bin/bash

pushd ..
Vendor/Binaries/Premake/Linux/premake5 --cc=clang --file=Build.lua gmake2
popd
echo "Premake Makefiles generated. You can now run: make config=debug"
