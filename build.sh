make main
./main ./example.nux
gcc example.nux.o -o example
echo "Running example.nux..."
./example