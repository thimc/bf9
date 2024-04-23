# bf9
a brainfuck interpreter written in Plan 9 C

## Commands

| Character | Function                                                       |
| --------- | -----------                                                    |
| >         | Increases the data pointer by one                              |
| <         | Decreases the data pointer by one                              |
| +         | Increases the byte at the data pointer by one                  |
| -         | Decreases the byte at the data pointer by one                  |
| .         | Outputs the current byte at the data pointer                   |
| ,         | Reads one byte from stdin to the byte at the data pointer      |
| [         | If the byte at the data pointer is zero jump to the matching ] |
| ]         | If the bytes is nonzero jump back to the matching [            |

## Caveats
The memory array size is 65535 bytes.

## Usage
``bf9 <source file>``

## Bugs
Probably.

