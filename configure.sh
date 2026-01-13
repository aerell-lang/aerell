set -a # Enable automatic export
source .env
set +a # Disable automatic export

cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE -DCMAKE_INSTALL_PREFIX=$CMAKE_INSTALL_PREFIX