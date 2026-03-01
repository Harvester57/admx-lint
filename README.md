# ADMX/ADML lint utility

If you wish to check ADMX or ADML file simply run:
```bash
admx-lint --input_file filename.admx
```

## Building from Source (Debian Trixie)

To compile `admx-lint` statically on Debian Trixie, you must first install the necessary dependencies:

```bash
# Update repository
sudo apt-get update

# Install build tools and libraries
sudo apt-get install -y \
    cmake \
    build-essential \
    pkg-config \
    libxerces-c-dev \
    xsdcxx \
    libboost-program-options-dev \
    libcurl4-gnutls-dev \
    libicu-dev
```

Once the dependencies are installed, you can configure and build the project using CMake:

```bash
# Clone the repository
git clone https://github.com/Harvester57/admx-lint
cd admx-lint

# Configure CMake (using CMake 4.x or newer) and build
cmake -S . -B build
cmake --build build
```
Upon a successful build, the `admx-lint` executable will be placed in the `src/` directory within your `build/` folder.
