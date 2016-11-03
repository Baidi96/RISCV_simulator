# lab2
This is a try for ssh key
lab2 for computer architecture

This simulator consists of the following parts:
loading part: the initial part of simulator for loading an ELF file to the simulator. It takes the name of ELF file and returns the text section's address and load the section that should be loaded to the memory
exectuion part: this part runs a single instruction of the RISCV codes. This part will start from the beginning of code and run until it meets the syscall stands for EXIT in a loop.
show part: it controls how simulator shows the results to the users
