#include <stdint.h>

#define TIM1_CR1      (0x40012C00 + 0x00)
#define TIM1_DIER     (0x40012C00 + 0x0C)
#define TIM1_SR       (0x40012C00 + 0x10)
#define TIM1_CCMR1    (0x40012C00 + 0x18)
#define TIM1_PSC      (0x40012C00 + 0x28)
#define TIM1_ARR      (0x40012C00 + 0x2C)
#define TIM1_CCR1     (0x40012C00 + 0x34)
#define NVIC_IPR0     (0xE000E400)
#define RCC_APBENR1   (0x40021000 + 0x3C)

#define GPIOA_MODER   (0x50000000 + 0x00)
#define GPIOA_OSPEEDR (0x50000000 + 0x08)
#define GPIOA_BSRR    (0x50000000 + 0x18)

// r/w /////////////////////////////////////////////////////
static inline uint32_t read32 (uintptr_t addr){ return *((uint32_t*)addr); }
static inline void     write32(uintptr_t addr, uint32_t val){ *((uint32_t*)addr)= val; }
static inline uint16_t read16 (uintptr_t addr){ return *((uint16_t*)addr); }
static inline void     write16(uintptr_t addr, uint16_t val){ *((uint16_t*)addr)= val; }
// lfsr113_Bits ////////////////////////////////////////////
uint32_t lfsr113_Bits (void) {
   static uint32_t z1 = 1234567, z2 = 3219231, z3 = 9182319, z4 = 9127938;
   uint32_t b;
   b  = ((z1 << 6) ^ z1) >> 13;
   z1 = ((z1 & 4294967294U) << 18) ^ b;
   b  = ((z2 << 2) ^ z2) >> 27;
   z2 = ((z2 & 4294967288U) << 2) ^ b;
   b  = ((z3 << 13) ^ z3) >> 21;
   z3 = ((z3 & 4294967280U) << 7) ^ b;
   b  = ((z4 << 3) ^ z4) >> 12;
   z4 = ((z4 & 4294967168U) << 13) ^ b;
   return (z1 ^ z2 ^ z3 ^ z4);
}

////////////////////////////////////////////////////////////
void     NVIC_EnableIRQ      (uint32_t irq){ write32(0xE000E100, 1 << irq); }
void     NVIC_ClearPendingIRQ(uint32_t irq){ write32(0xE000E280, 1 << irq); }
uint32_t NVIC_GetPendingIRQ  (uint32_t irq){ read32(0xE000E280) & (1 << irq); }
void     NVIC_SetPriority(uint32_t irq, uint32_t pri){
  uintptr_t base_addr = NVIC_IPR0 + (irq/4) * 4;
  uint32_t  ofst_bit  = (irq % 4) * 8;
  uint32_t  ofst_mask = ~(0xFF << ofst_bit);
  write32(base_addr,
          (read32(base_addr) & ofst_mask) | (pri << ofst_bit));
}
// tim1_irq ////////////////////////////////////////////////
void tim1_irq(){ // reload
  // next count
  // write16(TIM1_ARR, 480 + (lfsr113_Bits() % (48000-480)));
  // uint32_t val = lfsr113_Bits();

  static uint32_t val = 360;
  val += 360;
  if(48000 < val){ val= 360; }
  write16(TIM1_ARR, val);

  write16(TIM1_CCR1, val-48 );

  write32(0x50000400 + 0x18, 0x00000080); // PB7 on
  write32(GPIOA_BSRR, 0x00001000); // PA12 on

  write32(TIM1_SR, read32(TIM1_SR) & 0xFFFFFFFE); // clear flags
  NVIC_ClearPendingIRQ(13);
}
void tim1_cc_irq(){ // compare match
  write32(0x50000400 + 0x18, 0x00800000); // PB7 off
  write32(GPIOA_BSRR, 0x10000000); // PA12 off

  write32(TIM1_SR, read32(TIM1_SR) & 0xFFFFFFFD); // clear flags
  NVIC_ClearPendingIRQ(14);
}
// main ////////////////////////////////////////////////////
void main(){
  volatile uint32_t readback;

  write32( 0x40022000 + 0x00,
           (read32(0x40022000 + 0x00) & 0xFFFFFFF8) | 0x1 ); // FLASH_ACR, LAT=1
  do{ readback = read32(0x40022000 + 0x00); }
  while( (readback & 0x7) == 0x0 );

  write32( 0x40021000 + 0x00,
           read32( 0x40021000 + 0x00 ) & 0xFFFFC7FF ); // HSIDIV -> div1
  do{ readback = read32(0x40021000 + 0x00); }
  while( (readback & 0x00000400) == 0 );

  write32(0x40021000 + 0x34, 0x00000003); // enable portB and portA
  write32(RCC_APBENR1, (read32(RCC_APBENR1) | 0x08000000)); // DBG enable
  write32(0x40021000 + 0x40, 0x00000800); // enable tim1

  // PB7 -> output mode (2'b01)
  write32( 0x50000400 + 0x00,
           (read32(0x50000400 + 0x00) & 0xFFFF3FFF) | 0x00004000 );
  // todo: set as open-drain when led is connected
  //
  // pull up (2'b01)
  write32( 0x50000400 + 0x0C,
           (read32(0x50000400 + 0x0C) & 0xFFFF3FFF) | 0x00004000 );

  // PA12 -> output mode
  write32( GPIOA_MODER,
           (read32(GPIOA_MODER) & 0xFCFFFFFF) | 0x01000000) ;
  // PA12 -> very high speed
  write32( GPIOA_OSPEEDR,
           (read32(GPIOA_OSPEEDR) & 0xFCFFFFFF) | 0x03000000 );

  // setup interrupts //////////////////////////////////////
  NVIC_EnableIRQ(13); NVIC_SetPriority(13, 0x40);
  NVIC_EnableIRQ(14); NVIC_SetPriority(14, 0x40);

  // TIM1 //////////////////////////////////////////////////
  write16(TIM1_CR1,   (read16(TIM1_CR1)   & 0xFCFF) | 0x0080); // div1, ARR is buffered
  write16(TIM1_CR1,   (read16(TIM1_CR1)   & 0xFF7F) | 0x0080); // enable reload
  write16(TIM1_DIER,  (read16(TIM1_DIER)  & 0xFFFC) | 0x0003); // enable cc1ie, and uie
  write32(TIM1_CCMR1, (read32(TIM1_CCMR1) & 0xFFFFFFFC) | 0x00000000); // CC1S = output
  write32(TIM1_CCMR1, (read32(TIM1_CCMR1) & 0xFFFFFF8F) | 0x00000060); // OC1M = PWM mode 1
  //write32(TIM1_CCMR1, (read32(TIM1_CCMR1) & 0xFFFFFFF7) | 0x00000008); // OC1PE = Preload enable
  write16(TIM1_PSC,         0); // prescaler
  write16(TIM1_ARR,       480); // auto-reload
  write16(TIM1_CCR1,       48); // capture/compare
  write16(TIM1_CR1,   (read16(TIM1_CR1)   & 0xFFFE) | 0x0001); // // enable clock


  volatile uint32_t i;
  while(1){;}
}
