##!/bin/bash

#pushd ..
#Vendor/Binaries/Premake/Linux/premake5 --cc=clang --file=Build.lua gmake2
#popd
#echo "Premake Makefiles generated. You can now run: make config=debug"


#!/bin/bash
set -e

cd "$(dirname "$0")/.."

# Download Premake if not present
PREMAKE_PATH="Vendor/Binaries/Premake/Linux/premake5"
if [ ! -f "$PREMAKE_PATH" ]; then
    echo "Downloading Premake..."
    mkdir -p Vendor/Binaries/Premake/Linux
    curl -L https://github.com/premake/premake-core/releases/download/v5.0.0-beta2/premake-5.0.0-beta2-linux.tar.gz \
        | tar -xz -C Vendor/Binaries/Premake/Linux
fi

Vendor/Binaries/Premake/Linux/premake5 --cc=clang --file=Build.lua gmake2

make config=release

BINARY=$(find Binaries -path "*/Release/App/App" | head -1)


if [ -z "$CI" ]; then
    sudo install -m 755 "$BINARY" /usr/local/bin/grape
    echo "Installed! Try: grape -E <regex> <file.txt>"
else
    echo "Build successful! Running smoke test..."
    result=$(echo "hello world" | "$BINARY" -E "hello")
    if [ "$result" = "hello world" ]; then
        echo "Smoke test passed!"
    else
        echo "Smoke test failed!"
        exit 1
    fi
fi

