mkdir -p build
mkdir -p bin
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j8
echo ""
echo "Moving executables to bin/ directory..."
mv nitro ../bin/

echo ""
echo "Executable build complete"
