sycl_planning
=============

This project uses the [SYCL](https://www.khronos.org/sycl/) framework to plan robot trajectories on heterogeneous hardware.

Building
--------

This project is currently only developed using intels [DPC++](https://software.intel.com/content/www/us/en/develop/tools/oneapi/components/dpc-compiler.html) SYCL implementation.
If you want to build the code documentation as well, you need to have a recent [doxygen](https://www.doxygen.nl) version installed as well.

```console
> cd <build_dir>
> source /opt/intel/oneapi/setvars.sh  # Adjust to custom install path as needed
> cmake <sycl_planning dir> \
    -DBUILD_TESTS=<ON/OFF> \
    -DBUILD_DOC=<ON/OFF>
> cmake --build .
```

All tests are managed using CTest.
If you selected ```-DBUILD_TESTS=ON```, you can run them using

```console
> ctest
```

If you selected ```-DBUILD_DOC=ON```, you can now find all generated documentation in ```<build_dir>/doc```.
