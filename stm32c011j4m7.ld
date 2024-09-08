ENTRY(Reset_Handler);
MEMORY {
    FLASH(rx) : ORIGIN = 0x0,        LENGTH = 0x4000 /* 16K */
    RAM(rwx)  : ORIGIN = 0x20000000, LENGTH = 0x1800 /*  6K */
}

SECTIONS {
    . = ORIGIN(FLASH);
    .text : {
        KEEP(* (.isr_vectors));
        . = ALIGN(4);
        __vec_end__ = .;
        /*. = 0xC0;*/ /* End of 32 vectors */
        * (.text);
        . = ALIGN(4);
        __end_text__ = .;
    } > FLASH

    __flash_sdata__ = .;

    .data : AT(__flash_sdata__){
        __data_start__ = .;
        * (.data);
        . = ALIGN(4);
    __data_end__ = .;
    } > RAM

    .bss : {
        __bss_start__ = .;
         * (.bss);
        . = ALIGN(4);
        _bss_end__ = .;
    }
    __stack_top = ORIGIN(RAM) + LENGTH(RAM);
}

_end = .;
PROVIDE(end = .);
