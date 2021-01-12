

#include "..\\include\\global.h"
#include "..\\include\\MKxxx.h"
#include "..\\include\\hal_config.h"
#include "..\\include\\hal_watchdog.h"
#include "..\\include\\hal_dma.h"
#include "..\\include\\hal_enet.h"





void hal_int_enable(void)
{
    asm(" CPSIE i");
}
void hal_int_disable(void)
{
    asm(" CPSID i");
}
void hal_relocation(void)
{
    #define R_VTOR *(volatile uint*)0xE000ED08
    R_VTOR = VEC_RELOAD;
    #undef R_VTOR
}
void hal_isr_register(uint isr_no, FUNC_VOID func)
{
    
}

void __iar_program_start(void);

void entry(void)
{
    hal_int_disable();
    hal_watchdog_close();
    __iar_program_start();
    while(1)
        ;
}
static uint _get_int_num(void)
{
    int r = 0;
    asm(" mrs r0, psr");
    r &= 0x1ff;
    return r;
}
void isr_default(void)
{
    printf("isr_default,%d\n",_get_int_num());
}
void isr_dma0(void)
{
    //static int i= 0;
    void app_isr_dma0(void);
    app_isr_dma0();
    hal_dma_clear_int(0);
    //printf("isr_dma0,%d\n", i++);
}

#pragma location = ".intvec"
const FUNC_VOID __vector_table[256+4] =
{
    (FUNC_VOID)0x20003ffc,      // 0 sp
    entry,                      // 1 pc entry
    isr_default,                // 2
    isr_default,                // 3
    isr_default,                // 4
    isr_default,                // 5
    isr_default,                // 6
    isr_default,                // 7
    isr_default,                // 8
    isr_default,                // 9
    isr_default,                // 10
    isr_default,                // 11
    isr_default,                // 12
    isr_default,                // 13
    isr_default,                // 14
    isr_default,                // 15
    isr_dma0,                   // 16(irq 0)
    isr_default,                // 17(irq 1)
    isr_default,                // 18(irq 2)
    isr_default,                // 19(irq 3)
    isr_default,                // 20(irq 4)
    isr_default,                // 21(irq 5)
    isr_default,                // 22(irq 6)
    isr_default,                // 23(irq 7)
    isr_default,                // 24(irq 8)
    isr_default,                // 25(irq 9)
    isr_default,                // 26(irq 10)
    isr_default,                // 27(irq 11)
    isr_default,                // 28(irq 12)
    isr_default,                // 29(irq 13)
    isr_default,                // 30(irq 14)
    isr_default,                // 31(irq 15)
    isr_default,                // 32(irq 16)
    isr_default,                // 33(irq 17)
    isr_default,                // 34(irq 18)
    isr_default,                // 35(irq 19)
    isr_default,                // 36(irq 20)
    isr_default,                // 37(irq 21)
    isr_default,                // 38(irq 22)
    isr_default,                // 39(irq 23)
    isr_default,                // 40(irq 24)
    isr_default,                // 41(irq 25)
    isr_default,                // 42(irq 26)
    isr_default,                // 43(irq 27)
    isr_default,                // 44(irq 28)
    isr_default,                // 45(irq 29)
    isr_default,                // 46(irq 30)
    isr_default,                // 47(irq 31)
    isr_default,                // 48(irq 32)
    isr_default,                // 49(irq 33)
    isr_default,                // 50(irq 34)
    isr_default,                // 51(irq 35)
    isr_default,                // 52(irq 36)
    isr_default,                // 53(irq 37)
    isr_default,                // 54(irq 38)
    isr_default,                // 55(irq 39)
    isr_default,                // 56(irq 40)
    isr_default,                // 57(irq 41)
    isr_default,                // 58(irq 42)
    isr_default,                // 59(irq 43)
    isr_default,                // 60(irq 44)
    isr_default,                // 61(irq 45)
    isr_default,                // 62(irq 46)
    isr_default,                // 63(irq 47)
    isr_default,                // 64(irq 48)
    isr_default,                // 65(irq 49)
    isr_default,                // 66(irq 50)
    isr_default,                // 67(irq 51)
    isr_default,                // 68(irq 52)
    isr_default,                // 69(irq 53)
    isr_default,                // 70(irq 54)
    isr_default,                // 71(irq 55)
    isr_default,                // 72(irq 56)
    isr_default,                // 73(irq 57)
    isr_default,                // 74(irq 58)
    isr_default,                // 75(irq 59)
    isr_default,                // 76(irq 60)
    isr_default,                // 77(irq 61)
    isr_default,                // 78(irq 62)
    isr_default,                // 79(irq 63)
    isr_default,                // 80(irq 64)
    isr_default,                // 81(irq 65)
    isr_default,                // 82(irq 66)
    isr_default,                // 83(irq 67)
    isr_default,                // 84(irq 68)
    isr_default,                // 85(irq 69)
    isr_default,                // 86(irq 70)
    isr_default,                // 87(irq 71)
    isr_default,                // 88(irq 72)
    isr_default,                // 89(irq 73)
    isr_default,                // 90(irq 74)
    isr_default,                // 91(irq 75)
    isr_default,                // 92(irq 76)
    isr_default,                // 93(irq 77)
    isr_default,                // 94(irq 78)
    isr_default,                // 95(irq 79)
    isr_default,                // 96(irq 80)
    isr_default,                // 97(irq 81)
    isr_default,                // 98(irq 82)
    isr_default,                // 99(irq 83)
    isr_default,                // 100(irq 84)
    isr_default,                // 101(irq 85)
    isr_default,                // 102(irq 86)
    isr_default,                // 103(irq 87)
    isr_default,                // 104(irq 88)
    isr_default,                // 105(irq 89)
    isr_default,                // 106(irq 90)
    isr_default,                // 107(irq 91)
    isr_default,                // 108(irq 92)
    isr_default,                // 109(irq 93)
    isr_default,                // 110(irq 94)
    isr_default,                // 111(irq 95)
    isr_default,                // 112(irq 96)
    isr_default,                // 113(irq 97)
    isr_default,                // 114(irq 98)
    isr_default,                // 115(irq 99)
    isr_default,                // 116(irq 100)
    isr_default,                // 117(irq 101)
    isr_default,                // 118(irq 102)
    isr_default,                // 119(irq 103)
    isr_default,                // 120(irq 104)
    isr_default,                // 121(irq 105)
    isr_default,                // 122(irq 106)
    isr_default,                // 123(irq 107)
    isr_default,                // 124(irq 108)
    isr_default,                // 125(irq 109)
    isr_default,                // 126(irq 110)
    isr_default,                // 127(irq 111)
    isr_default,                // 128(irq 112)
    isr_default,                // 129(irq 113)
    isr_default,                // 130(irq 114)
    isr_default,                // 131(irq 115)
    isr_default,                // 132(irq 116)
    isr_default,                // 133(irq 117)
    isr_default,                // 134(irq 118)
    isr_default,                // 135(irq 119)
    isr_default,                // 136(irq 120)
    isr_default,                // 137(irq 121)
    isr_default,                // 138(irq 122)
    isr_default,                // 139(irq 123)
    isr_default,                // 140(irq 124)
    isr_default,                // 141(irq 125)
    isr_default,                // 142(irq 126)
    isr_default,                // 143(irq 127)
    isr_default,                // 144(irq 128)
    isr_default,                // 145(irq 129)
    isr_default,                // 146(irq 130)
    isr_default,                // 147(irq 131)
    isr_default,                // 148(irq 132)
    isr_default,                // 149(irq 133)
    isr_default,                // 150(irq 134)
    isr_default,                // 151(irq 135)
    isr_default,                // 152(irq 136)
    isr_default,                // 153(irq 137)
    isr_default,                // 154(irq 138)
    isr_default,                // 155(irq 139)
    isr_default,                // 156(irq 140)
    isr_default,                // 157(irq 141)
    isr_default,                // 158(irq 142)
    isr_default,                // 159(irq 143)
    isr_default,                // 160(irq 144)
    isr_default,                // 161(irq 145)
    isr_default,                // 162(irq 146)
    isr_default,                // 163(irq 147)
    isr_default,                // 164(irq 148)
    isr_default,                // 165(irq 149)
    isr_default,                // 166(irq 150)
    isr_default,                // 167(irq 151)
    isr_default,                // 168(irq 152)
    isr_default,                // 169(irq 153)
    isr_default,                // 170(irq 154)
    isr_default,                // 171(irq 155)
    isr_default,                // 172(irq 156)
    isr_default,                // 173(irq 157)
    isr_default,                // 174(irq 158)
    isr_default,                // 175(irq 159)
    isr_default,                // 176(irq 160)
    isr_default,                // 177(irq 161)
    isr_default,                // 178(irq 162)
    isr_default,                // 179(irq 163)
    isr_default,                // 180(irq 164)
    isr_default,                // 181(irq 165)
    isr_default,                // 182(irq 166)
    isr_default,                // 183(irq 167)
    isr_default,                // 184(irq 168)
    isr_default,                // 185(irq 169)
    isr_default,                // 186(irq 170)
    isr_default,                // 187(irq 171)
    isr_default,                // 188(irq 172)
    isr_default,                // 189(irq 173)
    isr_default,                // 190(irq 174)
    isr_default,                // 191(irq 175)
    isr_default,                // 192(irq 176)
    isr_default,                // 193(irq 177)
    isr_default,                // 194(irq 178)
    isr_default,                // 195(irq 179)
    isr_default,                // 196(irq 180)
    isr_default,                // 197(irq 181)
    isr_default,                // 198(irq 182)
    isr_default,                // 199(irq 183)
    isr_default,                // 200(irq 184)
    isr_default,                // 201(irq 185)
    isr_default,                // 202(irq 186)
    isr_default,                // 203(irq 187)
    isr_default,                // 204(irq 188)
    isr_default,                // 205(irq 189)
    isr_default,                // 206(irq 190)
    isr_default,                // 207(irq 191)
    isr_default,                // 208(irq 192)
    isr_default,                // 209(irq 193)
    isr_default,                // 210(irq 194)
    isr_default,                // 211(irq 195)
    isr_default,                // 212(irq 196)
    isr_default,                // 213(irq 197)
    isr_default,                // 214(irq 198)
    isr_default,                // 215(irq 199)
    isr_default,                // 216(irq 200)
    isr_default,                // 217(irq 201)
    isr_default,                // 218(irq 202)
    isr_default,                // 219(irq 203)
    isr_default,                // 220(irq 204)
    isr_default,                // 221(irq 205)
    isr_default,                // 222(irq 206)
    isr_default,                // 223(irq 207)
    isr_default,                // 224(irq 208)
    isr_default,                // 225(irq 209)
    isr_default,                // 226(irq 210)
    isr_default,                // 227(irq 211)
    isr_default,                // 228(irq 212)
    isr_default,                // 229(irq 213)
    isr_default,                // 230(irq 214)
    isr_default,                // 231(irq 215)
    isr_default,                // 232(irq 216)
    isr_default,                // 233(irq 217)
    isr_default,                // 234(irq 218)
    isr_default,                // 235(irq 219)
    isr_default,                // 236(irq 220)
    isr_default,                // 237(irq 221)
    isr_default,                // 238(irq 222)
    isr_default,                // 239(irq 223)
    isr_default,                // 240(irq 224)
    isr_default,                // 241(irq 225)
    isr_default,                // 242(irq 226)
    isr_default,                // 243(irq 227)
    isr_default,                // 244(irq 228)
    isr_default,                // 245(irq 229)
    isr_default,                // 246(irq 230)
    isr_default,                // 247(irq 231)
    isr_default,                // 248(irq 232)
    isr_default,                // 249(irq 233)
    isr_default,                // 250(irq 234)
    isr_default,                // 251(irq 235)
    isr_default,                // 252(irq 236)
    isr_default,                // 253(irq 237)
    isr_default,                // 254(irq 238)
    isr_default,                // 255(irq 239)
    (FUNC_VOID)0xffffffff,
    (FUNC_VOID)0xffffffff,
    (FUNC_VOID)0xffffffff,
    (FUNC_VOID)0xfffffffe
};



