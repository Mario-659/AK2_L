SYSEXIT = 1
EXIT_SUCCESS = 0
SYSWRITE = 4
STDOUT = 1

.section .data
liczba1:
    	.long 0x10304008, 0x701100FF # , 0x45100020, 0x18570030
        liczba1_len_bytes =(.-liczba1)
        liczba1_len_words = liczba1_len_bytes/4     
liczba2:
        .long 0xF040500C, 0x00220026, 0xf21000CB, 0x04520031
        liczba2_len_bytes =(.-liczba2)
        liczba2_len_words = liczba2_len_bytes/4

.section .text

.global _start
_start:

clc
pushf
mov $liczba1_len_words, %esi  # esi and edi as iterators
mov $liczba2_len_words, %edi

cmp %edi, %esi                # compare len of two numbers
jz  add_numbers_equal         # jump if len is equal
ja  num_1_bigger              # jump if first num's len is bigger
jb  num_2_bigger              # jump if second num's len is bigger

add_numbers_equal:
popf
dec %esi
movl liczba1(, %esi, 4), %eax
adcl liczba2(, %esi, 4), %eax
pushl %eax
pushf
cmp  $0, %esi
jz   end
jmp  add_numbers_equal

num_1_bigger:
popf
dec %esi
dec %edi
movl liczba1(, %esi, 4), %eax
adcl liczba2(, %edi, 4), %eax
pushl %eax
pushf
cmp  $0, %edi
jz   push_num_1
jmp  num_1_bigger

push_num_1:
popf
dec %esi
movl liczba1(, %esi, 4), %eax
adcl $0, %eax
pushl %eax
pushf
cmp $0, %esi
jz end
jmp push_num_1

num_2_bigger:
popf
dec %esi
dec %edi
movl liczba1(, %esi, 4), %eax
adcl liczba2(, %edi, 4), %eax
pushl %eax
pushf
cmp  $0, %esi
jz   push_num_2
jmp  num_2_bigger

push_num_2:
popf
dec %edi
movl liczba2(, %edi, 4), %eax
adcl $0, %eax
pushl %eax
pushf
cmp $0, %edi
jz end
jmp push_num_2

end:     # push 1 if last addidtion set carry flag
popf
jnc exit
pushl $1

exit:
mov $SYSEXIT, %eax
mov $EXIT_SUCCESS, %ebx
int $0x80
