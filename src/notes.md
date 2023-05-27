Raspberry Pi 4 Model B Rev 1.1

## Compile

`g++ -o main -I/usr/include/python3.7 main.cpp module.cpp kernel.cpp -lwiringPi -lpython3.7m`

## Stop program

`ctrl + z` or `ctrl + c`

## GPIO pins

`gpio readall`

`pinout`