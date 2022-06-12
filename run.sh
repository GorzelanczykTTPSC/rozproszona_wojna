#!/bin/bash
# pierwszy argument - liczba procesów
# drugi argument - liczba doków
mpiCC *.cpp -o wojna
mpirun -oversubscribe -np $1 ./wojna $2
