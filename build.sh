set -e
set -x
mkdir -p RPC_client/lib
mkdir -p RPC_server/lib

if [ ! -f protobuf/bin/protoc ]; then
	if [ ! -f protobuf-cpp-3.0.0.tar.gz ]; then
		wget https://github.com/google/protobuf/releases/download/v3.0.0/protobuf-cpp-3.0.0.tar.gz
	fi	

	tar zxvf protobuf-cpp-3.0.0.tar.gz
	cd protobuf-3.0.0

	./configure --prefix=`pwd`/../protobuf
	make -j2
	make install

	cd ../
fi

echo "export LD_LIBRARY_PATH=$PWD/protobuf/lib" >> ~/.bashrc
source ~/.bashrc
mkdir build
cd build
cmake ..
make
exit $?
