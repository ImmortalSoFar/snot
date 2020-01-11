#include "snMenu.h"
#include <stdlib.h>
#include <string.h>

#define CTRLD 	4
struct tagOption g_optRoot [] =
{
    {"File", eMenuFile},
    {"Edit", eMenuEdit},
    {0, 0}
};

struct tagOption g_optFile [] =
{
    {"New", eCmdNew},
    {"Open", eCmdOpen},
    {"Save", eCmdSave},
    {0, 0}

};

struct tagOption g_optEdit [] =
{
    {"Find", eCmdFind},
    {0, 0}
};

struct tagOption *g_pOptions [eMenuMax] = {
    g_optRoot, g_optFile, g_optEdit
};

static struct tagCursMenu *create_menu (struct tagOption *pOpt, int iOffset);

struct tagScreen *menu_begin ()
{
    struct tagScreen    *pRet = malloc (sizeof (*pRet));

	/* Initialize curses */
	initscr();
	start_color();
        cbreak();
        noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);

	pRet->pMenu [eMenuRoot] = create_menu(g_optRoot, -1);
	pRet->pMenu [eMenuFile] = create_menu(g_optFile, 0);
	pRet->pMenu [eMenuEdit] = create_menu(g_optEdit, getmaxx(pRet->pMenu [eMenuFile]->my_menu_win));

	attron(COLOR_PAIR(2));
	mvprintw(LINES - 3, 0, "Use PageUp and PageDown to scroll");
	mvprintw(LINES - 2, 0, "Use Arrow Keys to navigate (F1 to Exit)");
	attroff(COLOR_PAIR(2));
	refresh();

	/* Post the menu */
	post_menu(pRet->pMenu [eMenuRoot]->my_Menu);
	wrefresh(pRet->pMenu [eMenuRoot]->my_menu_win);

	return pRet;
}

struct tagCursMenu *create_menu (struct tagOption *pOpt, int iOffset)
{
    int               n_choices = 0,
                        iMaxWidth = 0;
    struct tagOption    *pPos = pOpt;
    struct tagCursMenu  *pRet;
    int                  iPos = 1;
    short                i = 0;

    for  (pPos = pOpt; pPos->pszName; pPos++, n_choices++)
        if  (strlen (pPos->pszName) > iMaxWidth)
            iMaxWidth = strlen (pPos->pszName);

    pRet = malloc (sizeof (*pRet));
	/* Create items */
    pRet->m_items = (ITEM **)calloc(n_choices+1, sizeof(ITEM *));
    for  (pPos = pOpt; pPos->pszName; pPos++)
        pRet->m_items[i++] = new_item(pPos->pszName, pPos->pszName);
    pRet->m_items [i] = 0;

	/* Crate menu */
	pRet->my_Menu = new_menu((ITEM **)pRet->m_items);

	/* Set menu option not to show the description */
	menu_opts_off(pRet->my_Menu, O_SHOWDESC);

    if  (iOffset < 0)
    {
        iMaxWidth = COLS;
        n_choices = 1;
        iOffset = 0;
        iPos = 0;
    }
    else
    {
        iMaxWidth += 2;
        n_choices += 2; //
    }
	/* Create the window to be associated with the menu */
    pRet->my_menu_win = newwin(n_choices, iMaxWidth, iOffset, iPos);
    keypad(pRet->my_menu_win, TRUE);

	/* Set main window and sub window */
        set_menu_win(pRet->my_Menu, pRet->my_menu_win);
	set_menu_format(pRet->my_Menu, iMaxWidth, n_choices);
        scale_menu (pRet->my_Menu, &iMaxWidth, &n_choices);
        set_menu_sub(pRet->my_Menu, derwin(pRet->my_menu_win, iOffset, iPos, iMaxWidth, n_choices));
	set_menu_mark(pRet->my_Menu, " * ");

	/* Print a border around the main window and print a title */
	if  (iOffset >= 0)
        box(pRet->my_menu_win, 0, 0);

    return pRet;
}

void menu_end (struct tagScreen *pScr)
{
    struct tagCursMenu  **pMenu = pScr->pMenu;
    ITEM                **pItem;

    for  (int m = 0; m < eMenuMax; m++, pMenu++)
    {
        /* Unpost and free all the memory taken up */
        unpost_menu((*pMenu)->my_Menu);
        free_menu((*pMenu)->my_Menu);
        pItem = (*pMenu)->m_items;
        while (*pItem)
        {
            free_item(*pItem++);
        }
        free (*pMenu);
    }
	endwin();
}

int menu_process(struct tagCursMenu *pMenu)
{
    int    c;

    nodelay (pMenu->my_menu_win, FALSE);

  	while(true)
	{
        c = wgetch(pMenu->my_menu_win);
        switch(c)
        {
        	case KEY_DOWN:
				menu_driver(pMenu->my_Menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(pMenu->my_Menu, REQ_UP_ITEM);
				break;
			case KEY_LEFT:
				menu_driver(pMenu->my_Menu, REQ_LEFT_ITEM);
				break;
			case KEY_RIGHT:
				menu_driver(pMenu->my_Menu, REQ_RIGHT_ITEM);
				break;
			case KEY_NPAGE:
				menu_driver(pMenu->my_Menu, REQ_SCR_DPAGE);
				break;
			case KEY_PPAGE:
				menu_driver(pMenu->my_Menu, REQ_SCR_UPAGE);
				break;
            default:
                return c;
		}
        wrefresh(pMenu->my_menu_win);
	}
}
