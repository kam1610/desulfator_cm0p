extern void tim1_irq();
extern void tim1_cc_irq();

// unhandled_exception /////////////////////////////////////
static void unhandled_exception(void) {
    for(;;);
}
////////////////////////////////////////////////////////////
void NMI_Handler()       { unhandled_exception(); }
void HardFault_Handler() { unhandled_exception(); }
void SVCall_Handler()    { unhandled_exception(); }
void PendSV_Handler()    { unhandled_exception(); }
void SysTick_Handler()   { unhandled_exception(); }
void WWDG_IRQ()          { unhandled_exception(); }
void RTC_IRQ()           { unhandled_exception(); }
void FLASH_IRQ()         { unhandled_exception(); }
void RCC_IRQ()           { unhandled_exception(); }
void EXTI0_1_IRQ()       { unhandled_exception(); }
void EXTI2_3_IRQ()       { unhandled_exception(); }
void EXTI4_15_IRQ()      { unhandled_exception(); }
void DMA1_Ch1_IRQ()      { unhandled_exception(); }
void DMA1_Ch2_3_IRQ()    { unhandled_exception(); }
void DMAMUX_IRQ()        { unhandled_exception(); }
void ADC_IRQ()           { unhandled_exception(); }
void TIM1_IRQ()          { tim1_irq(); }
void TIM1_CC_IRQ()       { tim1_cc_irq(); }
void TIM3_IRQ()          { unhandled_exception(); }
void TIM14_IRQ()         { unhandled_exception(); }
void TIM16_IRQ()         { unhandled_exception(); }
void TIM17_IRQ()         { unhandled_exception(); }
void I2C1_IRQ()          { unhandled_exception(); }
void SPI1_IRQ()          { unhandled_exception(); }
void USART1_IRQ()        { unhandled_exception(); }
void USART2_IRQ()        { unhandled_exception(); }
