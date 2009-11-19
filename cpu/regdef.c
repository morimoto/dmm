/************************************************************************


                              regs.def

This file needs "regs.def" file which define START, DEF, END for register.

2008/10 morimoto
************************************************************************/
#include <stdio.h>
#define START(a)  printf("#define %s_START %d\n" , #a , i);
#define DEF(a,b)  printf("#define %s %d\n"       , #a , i); i++;
#define END(a)    printf("#define %s_END %d\n"   , #a , i);

int main( void )
{
    int i = 0;
    #include "regs.def"

    printf( "#define REGMAX   REG_END\n" );
    printf( "#define REGUNDEF 0xFFFFFFFF\n" );

    return 0;
}
