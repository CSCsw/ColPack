echo "* autoreconf --force --install" && autoreconf --force --install && \
echo "* ./configure --prefix=`pwd`/build --exec-prefix=`pwd`/build  --enable-examples" && ./configure --prefix=`pwd`/build --exec-prefix=`pwd`/build  --enable-examples  && \
#./configure --prefix=`pwd`/build --exec-prefix=`pwd`/build  --enable-examples --enable-openmp && \
echo "* make clean" && make clean && \
echo "* make -j EXTRA_FLAGS="-O5 -DCOLPACK_DEBUG_LEVEL=0 "" && make -j EXTRA_FLAGS="-O5 -DCOLPACK_DEBUG_LEVEL=0 "
