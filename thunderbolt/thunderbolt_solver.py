from pwn import *
from Crypto.Util.number import long_to_bytes

flag_ls = []
for _ in range(5):
    io = remote('crypto.2021.chall.actf.co', 21603)
    io.recvuntil(': ')
    io.sendline('A'*40000)
    flag_ls.append(long_to_bytes(int(io.recvline()[-111:-1], 16)))
    io.close()

for flag in flag_ls:
    print(flag)


