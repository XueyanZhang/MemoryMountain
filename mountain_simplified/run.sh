set -ex

g++ -O1 mountain_simplified.cpp -o mountain_simplified

./mountain_simplified &> data.csv

python3 graph.py data.csv
