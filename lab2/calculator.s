SYSEXIT = 1
EXIT_SUCCESS = 0
SYSWRITE = 4
STDOUT = 1

.section .data

num0: .float 0

num1: .float 2.25
num2: .float 3.125

num3: .float 490138290000002999043000009043094925.0
num4: .float 934820430318940819390328000009288207.3

num5: .float 2.2
num6: .float 3.4

p_inf: .long 0x7fff8000000000000000

.section .text

.global _start
_start:

# operations with default control word

# Brak wyjątku
fld num1
fdiv num2

# Dzielenie przez 0 (+Inf)
fld num1
fdiv num0

# +0
fld num1
fmul num0

# 0 podzielone przez 0 (NaN)
fld num0
fdiv num0

# 0 pomnożone przez +inf (NaN)
# fld num0
# fmul p_inf

fld num3
fmul num4



exit:
mov $SYSEXIT, %eax
mov $EXIT_SUCCESS, %ebx
int $0x80
