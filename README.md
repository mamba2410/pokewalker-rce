# pokewalker-rce

Remote code execution for the pokewalker via arduino mega.

Arduino/host on `Serial`.
Arduino/pokewalker on `Serial1`.

Right now idk the best way to deliver the payload with the arduino.
Either bake it into the sketch/binary or send over serial.
Can bake it in with 
```
echo "#ifndef PAYLOAD_H \n#define PAYLOAD_H\n" >> src/arduino/payload.h
xxd -i payload payload.bin >> src/arduino/payload.h
echo "#endif PAYLOAD_H /* PAYLOAD_H */" >> src/arduino/payload.h
```


## Usage

### Payload
Write payload assembly in `src/pw/` and assemble with 
```
make pw
```
Make sure to have build tools for `h8300-hitachi-elf`.


### Arduino
Not implemented yet, but probably going to be
```
make arduino
make upload
```
With `avrdude` toolchain installed.


## Further references

- [Original cracking article by Dmitry GR.](http://dmitry.gr/?r=05.Projects&proj=28.%20pokewalker)
- [Emulator by PoroCYon](https://git.titandemo.org/PoroCYon/pwemu/)
- [ROM dumper by PoroCYon](https://git.titandemo.org/PoroCYon/pokewalker-rom-dumper)
- [Pico-based hardware recreation by mamba2410](https://github.com/mamba2410/picowalke://github.com/mamba2410/picowalker)

