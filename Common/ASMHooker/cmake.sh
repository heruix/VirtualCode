#/bin/sh

cd ./obj
cmake -DCMAKE_TOOLCHAIN_FILE=$NDK_HOME/build/cmake/android.toolchain.cmake -DCMAKE_BUILD_TYPE=release -DANDROID_ABI=$NDK_ABI -DANDROID_PLATFORM=android-24 ../whale
