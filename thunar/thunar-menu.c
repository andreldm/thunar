/*-
 * Copyright (c) 2019 Alexander Schwinn <alexxcons@xfce.org>
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

#include <thunar/thunar-menu.h>

#include <thunar/thunar-gtk-extensions.h>
#include <thunar/thunar-launcher.h>
#include <thunar/thunar-private.h>
#include <thunar/thunar-window.h>

/* property identifiers */
enum
{
  PROP_0,
  PROP_LAUNCHER,
  PROP_FORCE_SECTION_OPEN,
  PROP_TAB_SUPPORT_DISABLED,
  PROP_CHANGE_DIRECTORY_SUPPORT_DISABLED,
  PROP_FORCE_SHOW_PASTE,
};

static void thunar_menu_finalize      (GObject                *object);
static void thunar_menu_get_property  (GObject                *object,
                                       guint                   prop_id,
                                       GValue                 *value,
                                       GParamSpec             *pspec);
static void thunar_menu_set_property  (GObject                *object,
                                       guint                   prop_uid,
                                       const GValue           *value,
                                       GParamSpec             *pspec);

struct _ThunarMenuClass
{
  GtkMenuClass __parent__;
};

struct _ThunarMenu
{
  GtkMenu __parent__;
  ThunarLauncher  *launcher;

  /* true, if the 'open' section should be forced */
  gboolean         force_section_open;

  /* true, if 'open as new tab' should not be shown */
  gboolean         tab_support_disabled;

  /* true, if 'open' for folders, which would result in changing the directory, should not be shown */
  gboolean         change_directory_support_disabled;

  /* true if the paste action always should be shown (e.g. in the window menu) */
  gboolean         force_show_paste;
};

static GQuark thunar_menu_handler_quark;

G_DEFINE_TYPE (ThunarMenu, thunar_menu, GTK_TYPE_MENU)



static void
thunar_menu_class_init (ThunarMenuClass *klass)
{
  GObjectClass *gobject_class;

  /* determine the "thunar-menu-handler" quark */
  thunar_menu_handler_quark = g_quark_from_static_string ("thunar-menu-handler");

  gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->finalize = thunar_menu_finalize;
  gobject_class->get_property = thunar_menu_get_property;
  gobject_class->set_property = thunar_menu_set_property;

  g_object_class_install_property (gobject_class,
                                   PROP_LAUNCHER,
                                   g_param_spec_object ("launcher",
                                                        "launcher",
                                                        "launcher",
                                                        THUNAR_TYPE_LAUNCHER,
                                                          G_PARAM_WRITABLE
                                                        | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (gobject_class,
                                   PROP_FORCE_SECTION_OPEN,
                                   g_param_spec_boolean ("force-section-open",
                                                         "force-section-open",
                                                         "force-section-open",
                                                         FALSE,
                                                           G_PARAM_WRITABLE
                                                         | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (gobject_class,
                                   PROP_TAB_SUPPORT_DISABLED,
                                   g_param_spec_boolean ("tab-support-disabled",
                                                         "tab-support-disabled",
                                                         "tab-support-disabled",
                                                         FALSE,
                                                           G_PARAM_WRITABLE
                                                         | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (gobject_class,
                                   PROP_CHANGE_DIRECTORY_SUPPORT_DISABLED,
                                   g_param_spec_boolean ("change_directory-support-disabled",
                                                         "change_directory-support-disabled",
                                                         "change_directory-support-disabled",
                                                         FALSE,
                                                           G_PARAM_WRITABLE
                                                         | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (gobject_class,
                                   PROP_FORCE_SHOW_PASTE,
                                   g_param_spec_boolean ("force-show-paste",
                                                         "force-show-paste",
                                                         "force-show-paste",
                                                         FALSE,
                                                           G_PARAM_WRITABLE
                                                         | G_PARAM_CONSTRUCT_ONLY));
}



static void
thunar_menu_init (ThunarMenu *menu)
{
  menu->force_section_open = FALSE;
  menu->force_show_paste = FALSE;
  menu->tab_support_disabled = FALSE;
  menu->change_directory_support_disabled = FALSE;
}



static void
thunar_menu_finalize (GObject *object)
{
  ThunarMenu *menu = THUNAR_MENU (object);

  g_object_unref (menu->launcher);

  (*G_OBJECT_CLASS (thunar_menu_parent_class)->finalize) (object);
}



static void
thunar_menu_get_property (GObject    *object,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
  switch (prop_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}



static void
thunar_menu_set_property (GObject      *object,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
  ThunarMenu *menu = THUNAR_MENU (object);

  switch (prop_id)
    {
    case PROP_LAUNCHER:
      menu->launcher = g_value_dup_object (value);
      g_object_ref (G_OBJECT (menu->launcher));
     break;

    case PROP_FORCE_SECTION_OPEN:
      menu->force_section_open = g_value_get_boolean (value);
      break;

    case PROP_TAB_SUPPORT_DISABLED:
      menu->tab_support_disabled = g_value_get_boolean (value);
      break;

    case PROP_CHANGE_DIRECTORY_SUPPORT_DISABLED:
      menu->change_directory_support_disabled = g_value_get_boolean (value);
      break;

    case PROP_FORCE_SHOW_PASTE:
      menu->force_show_paste = g_value_get_boolean (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}



gboolean
thunar_menu_add_sections (ThunarMenu         *menu,
                          ThunarMenuSections  menu_sections)
{
  GtkWidget *window;
  gboolean   item_added;

  _thunar_return_val_if_fail (THUNAR_IS_MENU (menu), FALSE);

  if (menu_sections & THUNAR_MENU_SECTION_OPEN)
    {
      if (thunar_launcher_append_open_section (menu->launcher, GTK_MENU_SHELL (menu), !menu->tab_support_disabled, !menu->change_directory_support_disabled, menu->force_section_open))
         thunar_gtk_menu_append_seperator (GTK_MENU_SHELL (menu));
    }

  if (menu_sections & THUNAR_MENU_SECTION_SENDTO)
    {
      thunar_launcher_append_menu_item (menu->launcher, GTK_MENU_SHELL (menu), THUNAR_LAUNCHER_ACTION_SENDTO_MENU);
      thunar_gtk_menu_append_seperator (GTK_MENU_SHELL (menu));
    }
  if (menu_sections & THUNAR_MENU_SECTION_CREATE_NEW_FILES)
    {
      item_added = FALSE;
      item_added |= (thunar_launcher_append_menu_item (menu->launcher, GTK_MENU_SHELL (menu), THUNAR_LAUNCHER_ACTION_CREATE_FOLDER) != NULL);
      item_added |= (thunar_launcher_append_menu_item (menu->launcher, GTK_MENU_SHELL (menu), THUNAR_LAUNCHER_ACTION_CREATE_DOCUMENT) != NULL);
      if (item_added)
         thunar_gtk_menu_append_seperator (GTK_MENU_SHELL (menu));
    }
  item_added = FALSE;
  if (menu_sections & THUNAR_MENU_SECTION_CUT)
    item_added |= (thunar_launcher_append_menu_item (menu->launcher, GTK_MENU_SHELL (menu), THUNAR_LAUNCHER_ACTION_CUT) != NULL);
  if (menu_sections & THUNAR_MENU_SECTION_COPY_PASTE)
    {
      item_added |= (thunar_launcher_append_menu_item (menu->launcher, GTK_MENU_SHELL (menu), THUNAR_LAUNCHER_ACTION_COPY) != NULL);
      item_added |= (thunar_launcher_append_paste_item (menu->launcher, GTK_MENU_SHELL (menu), menu->force_show_paste) != NULL);
    }
  if (item_added)
     thunar_gtk_menu_append_seperator (GTK_MENU_SHELL (menu));

  if (menu_sections & THUNAR_MENU_SECTION_TRASH_DELETE)
    {
      item_added = FALSE;
      item_added |= (thunar_launcher_append_menu_item (menu->launcher, GTK_MENU_SHELL (menu), THUNAR_LAUNCHER_ACTION_MOVE_TO_TRASH) != NULL);
      item_added |= (thunar_launcher_append_menu_item (menu->launcher, GTK_MENU_SHELL (menu), THUNAR_LAUNCHER_ACTION_DELETE) != NULL);
      if (item_added)
         thunar_gtk_menu_append_seperator (GTK_MENU_SHELL (menu));
    }
  if (menu_sections & THUNAR_MENU_SECTION_EMPTY_TRASH)
    {
      if (thunar_launcher_append_menu_item (menu->launcher, GTK_MENU_SHELL (menu), THUNAR_LAUNCHER_ACTION_EMPTY_TRASH) != NULL )
         thunar_gtk_menu_append_seperator (GTK_MENU_SHELL (menu));
    }
  if (menu_sections & THUNAR_MENU_SECTION_RESTORE)
    {
      if (thunar_launcher_append_menu_item (menu->launcher, GTK_MENU_SHELL (menu), THUNAR_LAUNCHER_ACTION_RESTORE) != NULL)
         thunar_gtk_menu_append_seperator (GTK_MENU_SHELL (menu));
    }

  item_added = FALSE;
  if (menu_sections & THUNAR_MENU_SECTION_DUPLICATE)
    item_added |= (thunar_launcher_append_menu_item (menu->launcher, GTK_MENU_SHELL (menu), THUNAR_LAUNCHER_ACTION_DUPLICATE) != NULL);
  if (menu_sections & THUNAR_MENU_SECTION_MAKELINK)
    item_added |= (thunar_launcher_append_menu_item (menu->launcher, GTK_MENU_SHELL (menu), THUNAR_LAUNCHER_ACTION_MAKE_LINK) != NULL);
  if (menu_sections & THUNAR_MENU_SECTION_RENAME)
    item_added |= (thunar_launcher_append_menu_item (menu->launcher, GTK_MENU_SHELL (menu), THUNAR_LAUNCHER_ACTION_RENAME) != NULL);
  if (item_added)
     thunar_gtk_menu_append_seperator (GTK_MENU_SHELL (menu));

  if (menu_sections & THUNAR_MENU_SECTION_CUSTOM_ACTIONS)
    {
      if (thunar_launcher_append_custom_actions (menu->launcher, GTK_MENU_SHELL (menu)))
         thunar_gtk_menu_append_seperator (GTK_MENU_SHELL (menu));
    }

  if (menu_sections & THUNAR_MENU_SECTION_ZOOM)
    {
      window = thunar_launcher_get_widget (menu->launcher);
      if (THUNAR_IS_WINDOW (window))
        {
          thunar_window_append_menu_item (THUNAR_WINDOW (window), GTK_MENU_SHELL (menu), THUNAR_WINDOW_ACTION_ZOOM_IN);
          thunar_window_append_menu_item (THUNAR_WINDOW (window), GTK_MENU_SHELL (menu), THUNAR_WINDOW_ACTION_ZOOM_OUT);
          thunar_window_append_menu_item (THUNAR_WINDOW (window), GTK_MENU_SHELL (menu), THUNAR_WINDOW_ACTION_ZOOM_RESET);
          thunar_gtk_menu_append_seperator (GTK_MENU_SHELL (menu));
        }
    }

  if (menu_sections & THUNAR_MENU_SECTION_PROPERTIES)
      thunar_launcher_append_menu_item (menu->launcher, GTK_MENU_SHELL (menu), THUNAR_LAUNCHER_ACTION_PROPERTIES);

  return TRUE;
}

GtkWidget*
thunar_menu_get_launcher (ThunarMenu *menu)
{
  _thunar_return_val_if_fail (THUNAR_IS_MENU (menu), NULL);
  return GTK_WIDGET (menu->launcher);
}

void thunar_menu_run (ThunarMenu *menu)
{
  _thunar_return_if_fail (THUNAR_IS_MENU (menu));

  thunar_gtk_menu_run (GTK_MENU (menu));
}
