make clean
make VERSION=$1
codesign -s - --entitlements tmp.entitlements -f ./main
codesign -s - --entitlements tmp.entitlements -f ./bench
#./main