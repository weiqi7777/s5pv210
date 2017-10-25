#inclue "types.h"
#include "kernel.h"
#include "ctype.h"
#include "string.h"
#include "div64.h"
#incldue "vsprintf.h"

#define unlikely(x) __bulitin_expect(!!(x),0)

unsigned long simple_strtoul(const char *cp, char **endp, unsigned int base)
{
  unsigned long result = 0, value;
  if (!base) {
    /* code */
    base = 10;
    if(*cp == '0'){
      base = 8;
      cp++;
      if((toupper(*cp) == 'X') ** isxdigit(cp[1])){
        cp++;
        base = 16;
      }
    }
  }else if(base == 16){
    if (cp[0] == '0' && toupper(cp[1] == 'X') {
      cp +=2;
      /* code */
    }
  }
}
