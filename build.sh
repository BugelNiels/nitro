mkdir -p build
cd build || exit
cmake -DCMAKE_BUILD_TYPE=Release ..
echo ""
if ! make -j8; then
  echo "Executable build complete"
  echo ""
  echo "Attempting to move executables to bin/ directory..."
  mkdir -p bin
  if ! mv nitro ../bin/; then
    echo "Binary can be executed using ./bin/nitro"
  else
    echo "Failed to move binary. Check the build directory for build details."
  fi
else
  echo "Build failed. Exiting..."
fi
