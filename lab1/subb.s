SYSEXIT = 1
EXIT_SUCCESS = 0
SYSWRITE = 4
STDOUT = 1

.section .data
liczba1:
    	.long 0x47AE, 0xE49261E5
        liczba1_len_bytes =(.-liczba1)
        liczba1_len_words = liczba1_len_bytes/4     
liczba2:
        .long 0xffffffff, 0xffffffff
        liczba2_len_bytes =(.-liczba2)
        liczba2_len_words = liczba2_len_bytes/4

.section .text

.global _start
_start:

clc
pushf
mov $liczba1_len_words, %esi     # esi as iterator
mov $liczba2_len_words, %edi

cmp %edi, %esi                   # compare len of two numbers
jz  subb_numbers_equal           # jump if len is equal
jmp exit                         # jump if len is unequal

subb_numbers_equal:
popf
dec %esi
movl liczba1(, %esi, 4), %eax
sbbl liczba2(, %esi, 4), %eax
pushl %eax
pushf
cmp  $0, %esi
jz   end
jmp  subb_numbers_equal

end:
popf
# xor %eax, %eax  # clear eax
# sbbl $0, %eax
jmp exit

exit:
mov $SYSEXIT, %eax
mov $EXIT_SUCCESS, %ebx
int $0x80
