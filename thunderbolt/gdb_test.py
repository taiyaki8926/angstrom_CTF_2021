import gdb
import sys

_input = 'A' * 10000
flag = 'actf{hogehoge_fugafuga_1234567890}'

#_input = '1234'
#flag = 'actf{hogehoge}'

with open('gdb_in.txt', 'w') as f:
    f.write(_input + '\n')

with open('flag', 'w') as f:
    f.write(flag + '\n')

m = [ord(i) for i in (_input + flag)]

def make_ptr_00(__ptr_00):
    l = __ptr_00.split('\n')[:-1]
    f = ''
    for _ in l:
        a = _.replace('\t', '').split(':')[1].replace('0x', '')
        assert(len(a) == 32)
        f = a[16:] + a[:16] + f
    f = [int(f[i*2:i*2+2], 16) for i in range(len(f)//2)][::-1][:len(m)]
    assert(len(f)==len(m))
    return f

def make_local_148(rand_16):
    l = rand_16.split('\n')[:-1]
    f = ''
    for _ in l:
        a = _.replace('\t', '').split(':')[1].replace('0x', '')
        assert(len(a) == 32)
        f = a[16:] + a[:16] + f
    f = [int(f[i*2:i*2+2], 16) for i in range(len(f)//2)][::-1][:len(m)]
    #print(f)
    assert(len(f)==16)
    return f

def make_f_s(f_raw):
    f_ls = f_raw.split('\n')
    l, s = f_ls[:-2], f_ls[-2]
    f = ''
    for _ in l:
        a = _.replace('\t', '').split(':')[1].replace('0x', '')
        assert(len(a) == 32)
        f =  a[16:] + a[:16] + f
    assert(len(f) == 256*8)
    f = [int(f[i*8:i*8+8], 16) for i in range(256)][::-1]
    s = int(s[-2:], 16)
    #print(f, s)
    return f, s

'''
def fgen1(rand_total, r9, f_result):
    f = [i for i in range(256)]    
    uVar3 = 0
    mid_f = [0]*256
    for i in range(0x300):
        uVar7 = i & 0xff
        iVar1 = rand_total[i%len(rand_total)] + uVar3 + f[i & 0xff]
        uVar3 = f[iVar1 & 0xff]
        f[f[iVar1 & 0xff]], f[i & 0xff] = f[i & 0xff], f[f[iVar1 & 0xff]]
        if i == r9 - 1: # r9-1
            if (f_result == f):
                print(True)
            else:
                wrong = []
                for i in range(256):
                    if f[i] != f_result[i]:
                        wrong.append([i, f[i], f_result[i]])
                print("diff : ", wrong)
                print("calc : ", f)
                print("ans  : ", f_result)
                sys.exit()
            #print([hex(_)[2:] for _ in f])
    
    return f, mid_f
'''


def fgen(__ptr_00, local_148, ptr_len):
    s = 0
    f = [0] * 0x100
    lVar2 = 0
    while True:
        f[lVar2] = lVar2
        lVar2 +=1
        if lVar2 == 0x100:
            break
    
    uVar3 = s
    i = 0
    while True:
        l = len(set(f))
        uVar7 = i & 0xff
        uVar5 = i + 1
        iVar1 = (__ptr_00[i % ptr_len]) + uVar3 + f[uVar7]
        uVar4 = (iVar1 >> 0x1f) >> 0x18
        uVar3 = f[(iVar1 + uVar4 & 0xff) - uVar4]
        uVar4 = f[uVar7] ^ f[uVar3] # uVar4 = 0
        f[uVar7] = uVar4 # f[i] = 0
        uVar4 = uVar4 ^ f[uVar3] # uVar4 = 0
        f[uVar3] = uVar4 # f[i] = 0
        f[uVar7] = f[uVar7] ^ uVar4 # f[i] = 0
        i = uVar5
        if len(set(f)) != l:
            assert(uVar3 == uVar7)
            #lack_num.append(uVar3)
        if uVar5 == 0x300:
            break
    
    print('--------')
    i = 0
    while True:
        l = len(set(f))
        uVar5 = i & 0xff
        uVar7 = i + 1
        iVar1 = local_148[i % 16] + uVar3 + f[uVar5]
        uVar4 = (iVar1 >> 0x1f) >> 0x18
        s = f[(iVar1 + uVar4 & 0xff) - uVar4]
        uVar3 = s
        uVar4 = f[uVar5] ^ f[uVar3]
        f[uVar5] = uVar4
        uVar4 = uVar4 ^ f[uVar3]
        f[uVar3] = uVar4
        f[uVar5] = f[uVar5] ^ uVar4
        i = uVar7
        if len(set(f)) != l:
            assert(uVar3 == uVar5)
            #lack_num.append(uVar3)
        if uVar7 == 0x300:
            break

    return f, s

def enc(m, c, len_m, f, s):
    if len_m != 0:
        uVar4 = s
        i = 0
        uVar5 = 0
        while True:
            l = len(set(f))
            lVar3 = uVar5
            iVar2 = uVar4 + f[lVar3]
            uVar5 = uVar5 + 1 & 0xff
            uVar7 = (iVar2 >> 0x1f) >> 0x18
            assert(uVar7 == 0)
            s = f[(iVar2 + uVar7 & 0xff) - uVar7]
            uVar7 = (f[f[s]] + 1 >> 0x1f) >> 0x18
            assert(uVar7 == 0)
            # c[i] = f[(f[f[s]] + 1 + uVar7 & 0xff) - uVar7] ^ m[i]
            c[i] = f[f[f[s]] + 1 & 0xff] ^ m[i]
            uVar4 = s
            i += 1

            uVar7 = f[lVar3] # = f[i]
            uVar1 = f[uVar4] # = f[i]
            f[lVar3] = uVar7 ^ uVar1 # f[i] = 0
            uVar7 = uVar7 ^ uVar1 ^ f[uVar4] # uVar7 = 0
            f[uVar4] = uVar7
            f[lVar3] = f[lVar3] ^ uVar7
            if len(set(f)) != l:
                #assert(1==2)
                assert(lVar3 == uVar4)
                #lack_num.append(lVar3)
            if len_m == i:
                break
        return c
    print('enc error')
    return [-1]
'''
gdb.execute('file ./thunderbolt_chall')
gdb.execute('b main')
gdb.execute('r < gdb_in.txt')
gdb.execute('b *main+310')
gdb.execute('c')
if len(m) % 16 == 0:
    i = (len(m) // 16) * 2
else:
    i = (len(m) // 16 + 1) * 2

__ptr_00_raw = gdb.execute('x/'+str(i)+'gx $rdi', to_string=True)
local_148_raw = gdb.execute('x/2gx $rsi', to_string=True)
__ptr_00 = make_ptr_00(__ptr_00_raw)
local_148 = make_local_148(local_148_raw)
f, s = fgen(__ptr_00,local_148,len(m))

# result of fgen
gdb.execute('b *fgen+20')
gdb.execute('c')
f_addr = gdb.execute('p $rcx', to_string=True).split(' = ')[1]
gdb.execute('b *main+315')
gdb.execute('c')
f_raw = gdb.execute('x/129gx ' + f_addr, to_string=True)
_f, _s = make_f_s(f_raw)
assert(_f == f and _s == s)

#print(f)
c = [0]*len(m)
c = enc(m, c, len(m), f, s)
print(f)
# result of enc
gdb.execute('b *main+340')
gdb.execute('c')
c_raw = gdb.execute('x/' + str(i) + 'gx $r13', to_string=True)
_c = make_ptr_00(c_raw)
assert(_c == c)
print(c[-100:])
print('finish')
gdb.execute('q')
'''
