.section .isr_vectors

.align 2

.long __stack_top       /*             00 */
.long Reset_Handler     /*             04 */
.long NMI_Handler       /*             08 */
.long HardFault_Handler /*             0C */
.long 0                 /*    Reserved 10 */
.long 0                 /*    Reserved 14 */
.long 0                 /*    Reserved 18 */
.long 0                 /*    Reserved 1C */
.long 0                 /*    Reserved 20 */
.long 0                 /*    Reserved 24 */
.long 0                 /*    Reserved 28 */
.long SVCall_Handler    /*    Reserved 2C */
.long 0                 /*    Reserved 30 */
.long 0                 /*    Reserved 34 */
.long PendSV_Handler    /*    Reserved 38 */
.long SysTick_Handler   /*    Reserved 3C */
.long WWDG_IRQ          /* 00          40 */
.long 0                 /* 01 Reserved 44 */
.long RTC_IRQ           /* 02          48 */
.long FLASH_IRQ         /* 03          4C */
.long RCC_IRQ           /* 04          50 */
.long EXTI0_1_IRQ       /* 05          54 */
.long EXTI2_3_IRQ       /* 06          58 */  
.long EXTI4_15_IRQ      /* 07          5C */
.long 0                 /* 08 Reserved 60 */  
.long DMA1_Ch1_IRQ      /* 09          64 */
.long DMA1_Ch2_3_IRQ    /* 10          68 */  
.long DMAMUX_IRQ        /* 11          6C */
.long ADC_IRQ           /* 12          70 */  
.long TIM1_IRQ          /* 13          74 */
.long TIM1_CC_IRQ       /* 14          78 */  
.long 0                 /* 15 Reserved 7C */
.long TIM3_IRQ          /* 16          80 */  
.long 0                 /* 17 Reserved 84 */
.long 0                 /* 18 Reserved 88 */
.long TIM14_IRQ         /* 19          8C */  
.long 0                 /* 20 Reserved 90 */
.long TIM16_IRQ         /* 21          94 */  
.long TIM17_IRQ         /* 22          98 */
.long I2C1_IRQ          /* 23          9C */  
.long 0                 /* 24 Reserved A0 */
.long SPI1_IRQ          /* 25          A4 */  
.long 0                 /* 26 Reserved A8 */
.long USART1_IRQ        /* 27          AC */  
.long USART2_IRQ        /* 28          B0 */
.long 0                 /* 29 Reserved B4 */  
.long 0                 /* 30 Reserved B8 */
.long 0                 /* 31 Reserved BC */  

.text
.align

.thumb_func
.global Reset_Handler
Reset_Handler:
    b main
