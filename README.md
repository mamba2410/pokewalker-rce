# pokewalker-rce

Remote code execution for the pokewalker via arduino mega.

Arduino/host on `Serial`.
Arduino/pokewalker on `Serial1`.

Currently, baking in the binary as a C header file, included in arduino.


## Usage

### Requirements

- linux
- Arduino mega (other boards with multiple hw serial ports will probably work)
- `h8300-hitachi-elf` binutils.
- `arduino-cli`

### Payload

Write payload assembly in `src/pw/` and assemble with 
```
make pw
make pw-header
```


### Arduino

A modular pw interaction code that sends payload in `src/arduino-send/payload.h` to pokewalker.
Adapted from [here](https://github.com/mamba2410/reverse-pokewalker)
```
make arduino
make upload
```

You may need to change the port in `Makefile` to upload to your board.



## Further references

- [Original cracking article by Dmitry GR.](http://dmitry.gr/?r=05.Projects&proj=28.%20pokewalker)
- [Emulator by PoroCYon](https://git.titandemo.org/PoroCYon/pwemu/)
- [ROM dumper by PoroCYon](https://git.titandemo.org/PoroCYon/pokewalker-rom-dumper)
- [Pico-based hardware recreation by mamba2410](https://github.com/mamba2410/picowalker)

