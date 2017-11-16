/*-
 * Copyright (c) 2017 Andre Miranda <andreldm@xfce.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n-lib.h>

#include <thunarx/thunarx-private.h>
#include <thunarx/thunarx-menu-item.h>
#include <thunarx/thunarx-menu.h>



#define THUNARX_MENU_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), THUNARX_TYPE_MENU, ThunarxMenuPrivate))

/**
 * SECTION: thunarx-menu
 * @short_description: The base class for menu items added to the context menus
 * @title: ThunarxMenu
 * @include: thunarx/thunarx.h
 *
 * The class for menu items that can be added to Thunar's context menus
 * by extensions implementing the #ThunarxMenuProvider, #ThunarxPreferencesProvider
 * or #ThunarxRenamerProvider interfaces. The items returned by extensions from
 * *_get_menu_items() methods are instances of this class or a derived class.
 */



static void thunarx_menu_finalize (GObject      *object);



struct _ThunarxMenuPrivate
{
  GList *items;
};



G_DEFINE_TYPE (ThunarxMenu, thunarx_menu, G_TYPE_OBJECT)



static void
thunarx_menu_class_init (ThunarxMenuClass *klass)
{
  GObjectClass *gobject_class;

  /* add our private data to the class type */
  g_type_class_add_private (klass, sizeof (ThunarxMenuPrivate));

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = thunarx_menu_finalize;
}



static void
thunarx_menu_init (ThunarxMenu *menu)
{
  menu->priv = THUNARX_MENU_GET_PRIVATE (menu);
  menu->priv->items = NULL;
}



static void
thunarx_menu_finalize (GObject *object)
{
  ThunarxMenu *menu = THUNARX_MENU (object);

  if (menu->priv->items)
    g_list_free (menu->priv->items);

  (*G_OBJECT_CLASS (thunarx_menu_parent_class)->finalize) (object);
}



/**
 * thunarx_menu_new:
 *
 * Creates a new menu that can be added to the toolbar or to a contextual menu.
 *
 * Returns: a newly created #ThunarxMenu
 */
ThunarxMenu *
thunarx_menu_new (void)
{
  return g_object_new (THUNARX_TYPE_MENU, NULL);
}



void
thunarx_menu_append_item (ThunarxMenu *menu, ThunarxMenuItem *item)
{
  g_return_if_fail (menu != NULL);
  g_return_if_fail (item != NULL);

  menu->priv->items = g_list_append (menu->priv->items, g_object_ref (item));
}



/**
 * thunarx_menu_get_items:
 * @menu: a #ThunarxMenu
 *
 * Returns: (element-type ThunarxMenuItem) (transfer full): the provided #ThunarxMenuItem list
 */
GList *
thunarx_menu_get_items (ThunarxMenu *menu)
{
  GList *items;

  g_return_val_if_fail (menu != NULL, NULL);

  items = g_list_copy (menu->priv->items);
  g_list_foreach (items, (GFunc) g_object_ref, NULL);

  return items;
}

/**
 * thunarx_menu_item_list_free:
 * @items: (element-type ThunarxMenuItem): a list of #ThunarxMenuItem
 */
void
thunarx_menu_item_list_free (GList *items)
{
  g_return_if_fail (items != NULL);

  g_list_foreach (items, (GFunc)g_object_unref, NULL);
  g_list_free (items);
}
