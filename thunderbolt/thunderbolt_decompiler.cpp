#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <cstdio>

int main(void)
{
  std::byte bVar1;
  size_t input_len;
  FILE *f;
  size_t flag_len;
  void *__ptr;
  void *rand_total_len;
  std::byte *__ptr_01;
  int ret;
  std::byte *pbVar6;
  int total_len;
  char *input_str;
  size_t n;
  undefined rand_16 [16];
  char flag [264];
  
  setvbuf(stdout,(char *)0x0,2,0);
  input_str = (char *)0x0;
  printf("Enter a string to encrypt: ");
  n = 0;
  getline(&input_str,&n,stdin);
  /* not include '\n' , 4*/
  input_len = strcspn(input_str,"\n");
  f = fopen("flag","rb");
  if (f == (FILE *)0x0) {
    puts("Could not open flag file.");
    ret = 1;
  }
  else {
    fgets(flag,0x100,f);
    /* not include '\n' , 14*/
    flag_len = strcspn(flag,"\n");
    total_len = (int)flag_len + (int)input_len;
    flag[(int)flag_len] = '\0';
    __ptr = realloc(input_str,(long)(total_len + 1));
    /* 18 */
    flag_len = SEXT48(total_len);
    strcpy((char *)((long)(int)input_len + (long)__ptr),flag);
    f = fopen("/dev/urandom","rb");

    /* rand_16 is in address 0x7fffffffdf00 */
    fread(rand_16,0x10,1,f);
    rand_total_len = malloc(flag_len);

    /* rand_total_len is in address 0x55555555af10 */
    fread(rand_total_len,flag_len,1,f);

    fgen(rand_total_len,rand_16,flag_len,0x10);
    __ptr_01 = (std::byte *)malloc(flag_len);
    enc(__ptr,__ptr_01,flag_len);

    /* cipher is in address 0x55555555af30*/
    if (0 < total_len) {
      pbVar6 = __ptr_01;
      do {
        bVar1 = *pbVar6;
        pbVar6 = pbVar6 + 1;
        printf("%02x",(ulong)bVar1);
      } while (pbVar6 != __ptr_01 + (ulong)(total_len - 1) + 1);
    }
    putchar(10);
    free(rand_total_len);
    free(__ptr);
    free(__ptr_01);
    ret = 0;
  }
  return ret;
}


/* rand_total_len is in address 0x55555555af10 */
/* rand_16 is in address 0x7fffffffdf00 */
/* s is in address 0x5555555584c0 */
/* f is in address 0x5555555580c0 ~ 0x5555555584c0 */
void fgen(long rand_total_len,long rand_16,ulong total_len,ulong 16)

{
  int iVar1;
  long i;
  ulong uVar3;
  uint uVar4;
  ulong uVar5;
  ulong i;
  ulong uVar7;
  
  /* 0x5555555580c0 : 00000000 */
  /* 0x5555555580c4 : 00000001 */
  /* 0x5555555580c8 : 00000002 */
  /* 0x5555555580cc : 00000003 */
  /* ... */
  i = 0;
  do {
    *(int *)(f + i * 4) = (int)i;
    i = i + 1;
  } while (i != 0x100);

  /* (+39, ebp) uVar3 = s*/
  uVar3 = (ulong)s;


  i = 0;
  do {
    /* (+53, ebx) uVar7 = i & 0xff*/
    uVar7 = i & 0xff;
    uVar5 = i + 1;
    /* (+76, eax) iVar1 = rand_total_len[i%total_len] + uVar3 + f[i & 0xff] */
    iVar1 = (uint)*(byte *)(rand_total_len + i % total_len) + (int)uVar3 + *(uint *)(f + uVar7 * 4);
    /* (+79, rdx) uVar4 = 0 */
    uVar4 = (uint)(iVar1 >> 0x1f) >> 0x18;
    /* (+91, rax) uVar3 = old_f[iVar1 & 0xff] */
    uVar3 = SEXT48(*(int *)(f + (long)(int)((iVar1 + uVar4 & 0xff) - uVar4) * 4));
    /* (+95, r8d) uVar4 = old_f[i & 0xff] ^ f[old_f[iVar1 & 0xff]] */
    uVar4 = *(uint *)(f + uVar7 * 4) ^ *(uint *)(f + uVar3 * 4);
    /* (+108) f[i & 0xff] = old_f[i & 0xff] ^ f[old_f[iVar1 & 0xff]] */
    *(uint *)(f + uVar7 * 4) = uVar4;
    /* (+112, r8d) uVar4 = old_f[i & 0xff] ^ f[old_f[iVar1 & 0xff]] ^ f[old_f[iVar1 & 0xff]] = old_f[i & 0xff] */
    uVar4 = uVar4 ^ *(uint *)(f + uVar3 * 4);
    /* (+116) f[old_f[iVar1 & 0xff]] = old_f[i & 0xff] */
    *(uint *)(f + uVar3 * 4) = uVar4;
    /* new_f[i & 0xff] = old_f[i & 0xff] ^ f[old_f[iVar1 & 0xff]] ^ old_f[i & 0xff] = f[old_f[iVar1 & 0xff]] */
    *(uint *)(f + uVar7 * 4) = *(uint *)(f + uVar7 * 4) ^ uVar4;
    i = uVar5;
  } while (uVar5 != 0x300);


  i = 0;
  do {
    /* uVar5 = i & 0xff*/
    uVar5 = i & 0xff;
    uVar7 = i + 1;
    /* iVar1 = rand_16[i%16] + uVar3 + f[i & 0xff] */
    iVar1 = (uint)*(byte *)(rand_16 + i % 16) + (int)uVar3 + *(uint *)(f + uVar5 * 4);
    uVar4 = (uint)(iVar1 >> 0x1f) >> 0x18;
    /* uVar3 = old_f[iVar1 & 0xff] */
    s = *(uint *)(f + (long)(int)((iVar1 + uVar4 & 0xff) - uVar4) * 4);
    uVar3 = SEXT48((int)s);
    /* uVar4 = old_f[i & 0xff] ^ f[old_f[iVar1 & 0xff]] */
    uVar4 = *(uint *)(f + uVar5 * 4) ^ *(uint *)(f + uVar3 * 4);
    /* f[i & 0xff] = old_f[i & 0xff] ^ f[old_f[iVar1 & 0xff]] */
    *(uint *)(f + uVar5 * 4) = uVar4;
    /* uVar4 = old_f[i & 0xff] ^ f[old_f[iVar1 & 0xff]] ^ f[old_f[iVar1 & 0xff]] = old_f[i & 0xff] */
    uVar4 = uVar4 ^ *(uint *)(f + uVar3 * 4);
    /* f[old_f[iVar1 & 0xff]] = old_f[i & 0xff] */
    *(uint *)(f + uVar3 * 4) = uVar4;
    /* new_f[i & 0xff] = old_f[i & 0xff] ^ f[old_f[iVar1 & 0xff]] ^ old_f[i & 0xff] = f[old_f[iVar1 & 0xff]] */
    *(uint *)(f + uVar5 * 4) = *(uint *)(f + uVar5 * 4) ^ uVar4;
    i = uVar7;
  } while (uVar7 != 0x300);
  return;
}




/* __ptr : total_len + 1 の長さを確保したポインタ 0x5555555592a0 : 1234actf...*/
/* __ptr_01 : total_len の長さを確保したポインタ 0x55555555af30 : */
/* s is in address 0x5555555584c0 */
/* f is in address 0x5555555580c0 ~ 0x5555555584c0 */
/* 0x5555555580c0 : 00000000 */
/* 0x5555555580c4 : 00000001 */
/* 0x5555555580c8 : 00000002 */
/* 0x5555555580cc : 00000003 */
/* ... */
void enc(long __ptr,long __ptr_01,long total_len)

{
  uint uVar1;
  int iVar2;
  long _j;
  ulong uVar4;
  uint j;
  long i;
  uint uVar7;
  
  if (total_len != 0) {
    uVar4 = (ulong)s;
    i = 0;
    j = 0;
    do {
      _j = (long)(int)j;
      /* (+46, eax) iVar2 = s + f[_j] */
      iVar2 = (int)uVar4 + *(int *)(f + _j * 4);
      j = (j + 1) & 0xff;
      /* (+60, r11d) uVar7 = 0 (?) */
      uVar7 = (uint)(iVar2 >> 0x1f) >> 0x18;
      /* (+75, rax) s = f[iVar2 & 0xff] */
      s = *(uint *)(f + (long)(int)((iVar2 + uVar7 & 0xff) - uVar7) * 4);
      /* (+92, eax) eax = f[f[s]] + 1 */
      /* (+102, r11d) uVar7 = 0 (?) */
      uVar7 = (uint)(*(int *)(f + (long)*(int *)(f + (long)(int)s * 4) * 4) + 1 >> 0x1f) >> 0x18;
      /* (+124) __ptr_01[i] = f[(f[f[s]] + 1) & 0xff] ^ __ptr[i] */
      *(byte *)(__ptr_01 + i) =
           (byte)*(undefined4 *)(
                                  f + (long)(int)(
                                                  (
                                                    *(int *)(
                                                      f + (long)*(int *)(f + (long)(int)s * 4) * 4
                                                    ) + 1 +uVar7 & 0xff
                                                  ) - uVar7
                                                ) * 4
                                  ) ^ *(byte *)(__ptr + i);
      /* (+128, rbx) uVar4 = s(23) */
      uVar4 = SEXT48((int)s);
      i++;
      /* (+139, r11d) uVar7 = old_f[_j] (e5)*/
      uVar7 = *(uint *)(f + _j * 4);
      /* uVar1 = old_f[s] (74) */
      uVar1 = *(uint *)(f + uVar4 * 4);
      /* (+150) f[_j] = old_f[_j] ^ old_f[s] (91)*/
      *(uint *)(f + _j * 4) = uVar7 ^ uVar1;
      /* (+154 r11d) uVar7 = ord_f[_j] ^ old_f[s] * old_f[s] = uVal7 (const.) */
      uVar7 = uVar7 ^ uVar1 ^ *(uint *)(f + uVar4 * 4);
      /* f[s] = old_f[_j] */
      *(uint *)(f + uVar4 * 4) = uVar7;
      /* new_f[_j] = f[_j] ^ ord_f[_j] = old_f[s] */
      *(uint *)(f + _j * 4) = *(uint *)(f + _j * 4) ^ uVar7;
    } while (total_len != i);
    return;
  }
  return;
}

/* i = 0 */
/* (+75, rax) s = f[(s + f[i]) & 0xff] */
/* (+124) __ptr_01[i] = f[(f[f[s]] + 1) & 0xff] ^ __ptr[i] */
/* f[s], f[i] = f[i], f[s] */

