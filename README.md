# ppkMHD

## What is it ?

ppkMHD stands for Performance Portable Kokkos for Magnetohydrodynamics solvers.




## Dependencies

 * [Kokkos](https://github.com/kokkos/kokkos): for now (Feb 2017) it is required to use a version of kokkos that comes with kokkos.cmake (e.g. https://github.com/pkestene/kokkos branch develop_cmake)
 * a minimal sub-set of [boost](http://www.boost.org/); just all that is needed to build boost-mpi, i.e. system and serialization
 * (optionnal) if boost is not installed on your system, you can build this minimal subset using the following:
    1. Download [boost sources](http://www.boost.org/users/history/version_1_63_0.html) and untar 
    2. Configure boost
    ```shell
    ./bootstrap.sh --with-libraries=system,serialization --prefix=/home/pkestene/local/boost-1.63_mini
    ```
    3. Build and install
    ```shell
    ./b2 install
    ```
 * [boost-mpi](https://github.com/boostorg/mpi): it will be build inside ppkMHD

## Build

Env variable BOOST_ROOT is required is your build boost yourself (not the system boost).
