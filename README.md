<<<<<<< HEAD
# Compilers-Course-Homework-1
=======

Here's how this is supposed to work

```
mkdir build
cd build
cmake ../hw1 -DCMAKE_VERBOSE_MAKEFILE=1
make -j`nproc`
```

For Mac users installed flex w "brew":

```
mkdir build
cd build
cmake ../ -D FL_LIBRARY=/opt/homebrew/opt/flex/lib/libfl.a -D FL_INCLUDE_DIR=/opt/homebrew/opt/flex/include -D BISON_EXECUTABLE=/opt/homebrew/opt/bison/bin/bison
make -j`nproc`
```
>>>>>>> b7e366a (Initial commit)
