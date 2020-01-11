#include <stdio.h>
#include <stdlib.h>
#include "snMenu.h"

int main()
{
    int        c;
    struct tagScreen    *pScr = menu_begin ();

    while ((c = menu_process (pScr->pMenu [eMenuRoot])))
    {
        if  (c == 27)
            break;

        switch (c)
        {
          case KEY_F(1):
            break;
        }
    }

    menu_end (pScr);
}
