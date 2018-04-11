#```
set +x
CNL=/home/john/cnl
echo ${CNL}

mkdir -p /tmp/ws/cnl
cd /tmp/ws/cnl
cmake -DCMAKE_INSTALL_PREFIX=/tmp/ws ${CNL}
cmake --build . -- install

mkdir -p /tmp/ws/tau
cd /tmp/ws/tau
cmake -DCMAKE_PREFIX_PATH=/tmp/ws -DCMAKE_MODULE_PATH=/tmp/ws ${CNL}/src/examples/tau
cmake --build .

./Tau

#```
