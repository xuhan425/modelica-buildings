# Compiling on Ubuntu 18.04 LTS

`OpenCL` headers and library are the prerequisite. Before compiling the codes by hitting `make FFD_OPENCL_LINUX`, one should make sure everything is all set.

## Install dependencies

`sudo apt updatesudo`
`apt install ocl-icd-opencl-dev`

### Debugging and further information

`which gcc` to figure out the version of gcc.

`echo | gcc -E -Wp,-v -` and find if `CL/cl.h` is in the searching path. See [this post ](https://stackoverflow.com/questions/17939930/finding-out-what-the-gcc-include-path-is) for detail.

`ldconfig -v 2>/dev/null | grep -v ^$'\t'` for dependent library search path. See [this post](https://stackoverflow.com/questions/9922949/how-to-print-the-ldlinker-search-path) for detail.

## Build

`build.sh` will build both the OPEN_CL device detection application as well as
the FFD engine application
