export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

BINUTILS_VER=2.29.1
GCC_VER=7.2.0

mkdir $HOME/src

cd $HOME/src

# Download binutils and GCC and extract
echo 'downloading binutils'
wget -qO- ftp://ftp.gnu.org/pub/gnu/binutils/binutils-$BINUTILS_VER.tar.gz | tar xz
echo 'downloading gcc'
wget -qO- ftp://ftp.gnu.org/pub/gnu/gcc/gcc-$GCC_VER/gcc-$GCC_VER.tar.gz | tar xz

# If you wish to build these packages as part of binutils:
#mv isl-x.y.z binutils-x.y.z/isl
#mv cloog-x.y.z binutils-x.y.z/cloog
# But reconsider: You should just get the development packages from your OS.
echo 'building binutils'
mkdir build-binutils
cd build-binutils
../binutils-$BINUTILS_VER/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -s
make install

cd $HOME/src

# If you wish to build these packages as part of gcc:
#mv libiconv-x.y.z gcc-x.y.z/libiconv # Mac OS X users
#mv gmp-x.y.z gcc-x.y.z/gmp
#mv mpfr-x.y.z gcc-x.y.z/mpfr
#mv mpc-x.y.z gcc-x.y.z/mpc
#mv isl-x.y.z gcc-x.y.z/isl
#mv cloog-x.y.z gcc-x.y.z/cloog
# But reconsider: You should just get the development packages from your OS.

# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH
echo 'building gcc'
mkdir build-gcc
cd build-gcc
../gcc-$GCC_VER/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc -s
make all-target-libgcc -s
make install-gcc
make install-target-libgcc
