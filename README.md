## Introduction
*Megumin* is a super-optimizer targeting AArch64 assemblies.  
Currently it only supports integral instructions and floating point instructions (partly).

### Dependency
See **megumin/3rd** for all third-party dependencies
| Dependency | Description |
| --- | --- |
| Z3 | Symbolic execution |
| thread-pool | Parallel optimization |
| argparse | Parsing user arguments |
| Catch2 | Unit tests |
| keystone | Compile AArch64 assembilies to machine code |
| abseil | Large integer arithmetic |

## Build
### On Linux
#### Clone repository
```bash
git clone https://github.com/wormtql/megumin.git --recursive
cd megumin
```
#### Build Z3
```bash
mkdir z3_build
mkdir z3_install
cd z3_build
cmake -DCMAKE_BUILD_TYPE=Release ../3rd/z3
cmake --build .
cmake -DCMAKE_INSTALL_PREFIX=<path_to_z3_install> -P cmake_install.cmake
```
#### Build keystone
```bash
./build_keystone.sh
```
#### Build megumin
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```
### On Windows
#### Clone repository
Same as Linux
#### Build Z3
- Download Z3 binaries
- Extract to `z3_install`
#### Build keystone
- Download binary distributions from [keystone](https://www.keystone-engine.org/)
- Extract to `keystone_install`
- Make sure the `keystone.dll` lies in `keystone_install/keystone.dll`
#### Build megumin
use either `cmake` or `Visual Studio` or `CLion` or `VS Code` to build

## Binaries
Use `xxx.exe --help` to see options
### auto-opt
Optimize a single file assemblies
| Parameter | Description |
| --- | --- |
| --input-file | Assembly file to be optimized |
| --correct-file | Store the optimize results |
| --error-file | Store errors |
| --time-per-opt | Time spent per basic block per launch (ms) |
| --thread-count | Specify how many threads to use |
### apply-opt
Apply optimize result to original assembly file
| Parameter | Description |
| --- | --- |
| --source-file | The original assembly file |
| --output-file | The output assembly file |
| --optimizations | The optimize result, should be the output of `auto-opt` |

