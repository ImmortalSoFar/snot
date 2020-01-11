#ifndef SNMENU_H
#define SNMENU_H
#include <curses.h>
#include <menu.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

enum eMenu {
    eMenuRoot,
    eMenuFile,
    eMenuEdit,
    eMenuMax,
    eCmdNew = eMenuMax,
    eCmdOpen,
    eCmdSave,
    eCmdFind
};

struct tagCursMenu
{
    WINDOW *my_menu_win;
    MENU    *my_Menu;
    ITEM    **m_items;
};

struct  tagScreen
{
    struct tagCursMenu  *pMenu [eMenuMax];
};

struct tagOption
{
    const char      *pszName;
    int              iCmd;
};

struct tagScreen *menu_begin ();
void menu_end (struct tagScreen *pScr);
int menu_process (struct tagCursMenu *pMenu);

#endif // SNMENU_H
