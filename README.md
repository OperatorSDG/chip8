# Emulating: CHIP-8 - The classic CHIP-8 for the COSMAC VIP by Joseph Weisbecker, 1977
[Timendus/chip8-test-suite](https://github.com/Timendus/chip8-test-suite)
[Chip8 Opcode List - Gulark Schumann](https://chip8.gulrak.net/#quirk5)

## To-Do

- [x] Load and display ROMs
- [x] Implement 00E0 (clear screen)
- [ ] Implement full opcode set
    - [x] 00E0 - CLS
    - [x] 00EE - RET
    - [ ] 0nnn - SYS addr
    - [x] 1nnn - JP addr
    - [x] 2nnn - CALL addr
    - [x] 3xkk - SE Vx, byte
    - [x] 4xkk - SNE Vx, byte
    - [x] 5xy0 - SE Vx, Vy
    - [x] 6xkk - LD Vx, byte
    - [x] 7xkk - ADD Vx, byte
    - [x] 8xy0 - LD Vx, Vy
    - [x] 8xy1 - OR Vx, Vy 
        - [ ] Quirk5
    - [x] 8xy2 - AND Vx, Vy 
        - [ ] Quirk5
    - [x] 8xy3 - XOR Vx, Vy 
        - [ ] Quirk5
    - [x] 8xy4 - ADD Vx, Vy
    - [x] 8xy5 - SUB Vx, Vy
    - [x] 8xy6 - SHR Vx {, Vy} 
        - [ ] Quirk6
    - [x] 8xy7 - SUBN Vx, Vy
    - [x] 8xyE - SHL Vx {, Vy} 
        - [ ] Quirk6
    - [X] 9xy0 - SNE Vx, Vy
    - [x] Annn - LD I, addr
    - [ ] Bnnn - JP V0, addr
    - [ ] Cxkk - RND Vx, byte
    - [x] Dxyn - DRW Vx, Vy, nibble 
        - [ ] Quirk7
        - [ ] Quirk8 
        - [ ] Quirk9 
        - [ ] Quirk10
    - [x] Ex9E - SKP Vx
    - [x] ExA1 - SKNP Vx
    - [ ] Fx07 - LD Vx, DT
    - [x] Fx0A - LD Vx, K
    - [X] Fx15 - LD DT, Vx
    - [ ] Fx18 - LD ST, Vx
    - [X] Fx1E - ADD I, Vx
    - [ ] Fx29 - LD F, Vx
    - [X] Fx33 - LD B, Vx
    - [X] Fx55 - LD I, Vx - Quirk12[ ]
    - [X] Fx65 - LD Vx, I - Quirk12[ ]
- [ ] Add keypad input support
- [ ] Add sound support
- [ ] Write tests for CPU instructions
- [X] Add cycle timing for real-time emulation
