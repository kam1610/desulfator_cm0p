
CSRCS   = vector.c main.c 
ASRCS   = csr0.s
OBJS    = $(CSRCS:.c=.o) $(ASRCS:.s=.o) 
CFLAGS  = -O0 -g3 \
	        -mthumb \
	        -mcpu=cortex-m0plus \
	        -nostartfiles
CC      = arm-none-eabi-gcc
AS      = arm-none-eabi-as
ASFLAGS = -mthumb \
	        -mcpu=cortex-m0plus

all: desulfator.out

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.s.o:
	$(AS) $(ASFLAGS) -c $< -o $@

desulfator.out: $(OBJS)
	arm-none-eabi-ld \
	-L/usr/arm-none-eabi/lib  \
	-L/usr/arm-none-eabi/lib/thumb/v6-m/nofp \
	-L/usr/lib/gcc/arm-none-eabi/14.1.0 \
	-L/usr/lib/gcc/arm-none-eabi/14.1.0/thumb/v6-m/nofp \
	-L/usr/lib/gcc/arm-none-eabi/lib/thumb/v6-m/nofp \
	-L/usr/lib/gcc/lib\
	-static --script stm32c011j4m7.ld -nostdlib -Map=desulfator.map \
	$(OBJS) \
	-o $@ \
  --start-group -lgcc -lc --end-group

clean:
	rm desulfator.out $(OBJS)
