/* vi:set et ai sw=2 sts=2 ts=2: */
/*-
 * Copyright (c) 2005-2006 Benedikt Meurer <benny@xfce.org>
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

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif

#include <exo/exo.h>

#include <thunar/thunar-gtk-extensions.h>
#include <thunar/thunar-private.h>
#include <thunar/thunar-util.h>

#include <thunarx/thunarx.h>



/**
 * thunar_gtk_label_set_a11y_relation:
 * @label  : a #GtkLabel.
 * @widget : a #GtkWidget.
 *
 * Sets the %ATK_RELATION_LABEL_FOR relation on @label for @widget, which means
 * accessiblity tools will identify @label as descriptive item for the specified
 * @widget.
 **/
void
thunar_gtk_label_set_a11y_relation (GtkLabel  *label,
                                    GtkWidget *widget)
{
  AtkRelationSet *relations;
  AtkRelation    *relation;
  AtkObject      *object;

  _thunar_return_if_fail (GTK_IS_WIDGET (widget));
  _thunar_return_if_fail (GTK_IS_LABEL (label));

  object = gtk_widget_get_accessible (widget);
  relations = atk_object_ref_relation_set (gtk_widget_get_accessible (GTK_WIDGET (label)));
  relation = atk_relation_new (&object, 1, ATK_RELATION_LABEL_FOR);
  atk_relation_set_add (relations, relation);
  g_object_unref (G_OBJECT (relation));
}



GtkWidget*
thunar_gtk_menu_item_new (const gchar  *label_text,
                             const gchar  *tooltip_text,
                             const gchar  *accel_path,
                             GCallback     callback,
                             GObject      *callback_param,
                             gboolean      sensitive,
                             GtkMenuShell  *menu_to_append_item)
{
  GtkWidget *item;

  item = gtk_menu_item_new_with_mnemonic (label_text);
  if (tooltip_text != NULL)
    gtk_widget_set_tooltip_text (item, tooltip_text);
  gtk_widget_set_sensitive (item, sensitive);
  if (accel_path != NULL)
    gtk_menu_item_set_accel_path (GTK_MENU_ITEM (item), accel_path);
  if (callback != NULL)
    g_signal_connect_swapped (G_OBJECT (item), "activate", callback, callback_param);
  if (menu_to_append_item != NULL)
    gtk_menu_shell_append (menu_to_append_item, item);
  gtk_widget_show (item);
  return item;
}



GtkWidget*
thunar_gtk_image_menu_item_new_from_icon_name (const gchar *label_text,
                                                  const gchar *tooltip_text,
                                                  const gchar *accel_path,
                                                  GCallback    callback,
                                                  GObject     *callback_param,
                                                  const gchar *icon_name,
                                                  gboolean     sensitive,
                                                  GtkMenuShell  *menu_to_append_item)
{
  GtkWidget *image = NULL;

  if (icon_name != NULL)
    image = gtk_image_new_from_icon_name (icon_name, GTK_ICON_SIZE_MENU);
  return thunar_gtk_image_menu_item_new (label_text, tooltip_text, accel_path,
                                            callback, callback_param, image, sensitive, menu_to_append_item);
}



GtkWidget*
thunar_gtk_image_menu_item_new (const gchar *label_text,
                                   const gchar *tooltip_text,
                                   const gchar *accel_path,
                                   GCallback    callback,
                                   GObject     *callback_param,
                                   GtkWidget   *image,
                                   gboolean     sensitive,
                                   GtkMenuShell  *menu_to_append_item)
{
  GtkWidget *item;

  G_GNUC_BEGIN_IGNORE_DEPRECATIONS
  item = gtk_image_menu_item_new_with_mnemonic (label_text);
  G_GNUC_END_IGNORE_DEPRECATIONS
  if (tooltip_text != NULL)
    gtk_widget_set_tooltip_text (item, tooltip_text);
  gtk_widget_set_sensitive (item, sensitive);
  if (accel_path != NULL)
    gtk_menu_item_set_accel_path (GTK_MENU_ITEM (item), accel_path);
  if (callback != NULL)
    g_signal_connect_swapped (G_OBJECT (item), "activate", callback, callback_param);
  if (menu_to_append_item != NULL)
    gtk_menu_shell_append (menu_to_append_item, item);
  gtk_widget_show (item);

  if (image != NULL)
    {
      G_GNUC_BEGIN_IGNORE_DEPRECATIONS
      gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (item), image);
      G_GNUC_END_IGNORE_DEPRECATIONS
    }
  return item;
}



GtkWidget*
thunar_gtk_menu_thunarx_menu_item_new (GObject *thunarx_menu_item,
                                          GtkMenuShell  *menu_to_append_item)
{
  gchar        *label_text, *tooltip_text, *icon_name;
  gboolean      sensitive;
  GtkWidget    *gtk_menu_item;
  ThunarxMenu  *thunarx_menu;
  GList        *children;
  GList        *lp;
  GtkWidget    *submenu;

  g_return_val_if_fail (THUNARX_IS_MENU_ITEM (thunarx_menu_item), NULL);

  g_object_get (G_OBJECT (thunarx_menu_item),
                "label", &label_text,
                "tooltip", &tooltip_text,
                "icon", &icon_name,
                "sensitive", &sensitive,
                "menu", &thunarx_menu,
                NULL);

  gtk_menu_item = thunar_gtk_image_menu_item_new_from_icon_name (label_text, tooltip_text, NULL,
                                                                    G_CALLBACK (thunarx_menu_item_activate),
                                                                    G_OBJECT (thunarx_menu_item), icon_name, sensitive, menu_to_append_item);

  /* recursively add submenu items if any */
  if (thunarx_menu != NULL)
  {
    children = thunarx_menu_get_items (thunarx_menu);
    submenu = gtk_menu_new ();
    for (lp = children; lp != NULL; lp = lp->next)
      thunar_gtk_menu_thunarx_menu_item_new (lp->data, GTK_MENU_SHELL (submenu));
    gtk_menu_item_set_submenu (GTK_MENU_ITEM (gtk_menu_item), submenu);
    thunarx_menu_item_list_free (children);
  }

  g_free (label_text);
  g_free (tooltip_text);
  g_free (icon_name);

  return gtk_menu_item;
}



GtkWidget*
thunar_gtk_check_menu_item_new (const gchar  *label_text,
                                   const gchar  *tooltip_text,
                                   const gchar  *accel_path,
                                   GCallback     callback,
                                   GObject      *callback_param,
                                   gboolean      sensitive,
                                   gboolean      active,
                                   GtkMenuShell  *menu_to_append_item)
{
  GtkWidget *item;

  item = gtk_check_menu_item_new_with_mnemonic (label_text);
  if (tooltip_text != NULL)
    gtk_widget_set_tooltip_text (item, tooltip_text);
  gtk_widget_set_sensitive (item, sensitive);
  if (accel_path != NULL)
    gtk_menu_item_set_accel_path (GTK_MENU_ITEM (item), accel_path);
  gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (item), active);
  if (menu_to_append_item != NULL)
    gtk_menu_shell_append (menu_to_append_item, item);
  gtk_widget_show (item);

  if (callback != NULL)
    g_signal_connect_swapped (G_OBJECT (item), "toggled", callback, callback_param);

  return item;
}



GtkWidget*
thunar_gtk_radio_menu_item_new (const gchar  *label_text,
                                   const gchar  *tooltip_text,
                                   const gchar  *accel_path,
                                   GCallback     callback,
                                   GObject      *callback_param,
                                   gboolean      sensitive,
                                   gboolean      active,
                                   GtkMenuShell  *menu_to_append_item)
{
  GtkWidget *item;

  /* gtk_radio_menu_item seemsto have some glitch. gtk_check_menu_item_set_active as well activates the item. */
  /* So we use gtk_check_menu_item for now and draw it as radio */
  item = gtk_check_menu_item_new_with_mnemonic (label_text);
  gtk_check_menu_item_set_draw_as_radio (GTK_CHECK_MENU_ITEM (item), TRUE);
  if (tooltip_text != NULL)
    gtk_widget_set_tooltip_text (item, tooltip_text);
  gtk_widget_set_sensitive (item, sensitive);
  if (accel_path != NULL)
    gtk_menu_item_set_accel_path (GTK_MENU_ITEM (item), accel_path);
  gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (item), active);
  if (menu_to_append_item != NULL)
    gtk_menu_shell_append (menu_to_append_item, item);
  gtk_widget_show (item);

  if (callback != NULL)
    g_signal_connect_swapped (G_OBJECT (item), "toggled", callback, callback_param);

  return item;
}



GtkWidget*
thunar_gtk_menu_item_new_from_action_entry (ThunarGtkActionEntry *action_entry,
                                               GObject              *callback_param,
                                               gboolean              sensitive,
                                               gboolean              active,
                                               GtkMenuShell         *menu_to_append_item)
{
  if (action_entry->menu_item_type == THUNAR_GTK_IMAGE_MENU_ITEM)
    {
      return thunar_gtk_image_menu_item_new_from_icon_name (action_entry->menu_item_label_text, action_entry->menu_item_tooltip_text,
                                                               action_entry->accel_path, action_entry->callback,
                                                               callback_param, action_entry->menu_item_icon_name, sensitive, menu_to_append_item);
    }
  else if (action_entry->menu_item_type == THUNAR_GTK_CHECK_MENU_ITEM)
    {
      return thunar_gtk_check_menu_item_new (action_entry->menu_item_label_text, action_entry->menu_item_tooltip_text,
                                                action_entry->accel_path, action_entry->callback,
                                                callback_param, sensitive, active, menu_to_append_item);
    }
  else if (action_entry->menu_item_type == THUNAR_GTK_RADIO_MENU_ITEM)
    {
      return thunar_gtk_radio_menu_item_new (action_entry->menu_item_label_text, action_entry->menu_item_tooltip_text,
                                                action_entry->accel_path, action_entry->callback,
                                                callback_param, sensitive, active, menu_to_append_item);
    }
  else if (action_entry->menu_item_type == THUNAR_GTK_MENU_ITEM)
    {
      return thunar_gtk_menu_item_new (action_entry->menu_item_label_text, action_entry->menu_item_tooltip_text,
                                          action_entry->accel_path, action_entry->callback,
                                          callback_param, sensitive, menu_to_append_item);
    }
  else
    {
      g_warning ("thunar_gtk_menu_item_new: Unknown item_type");
      return NULL;
    }
}



void
thunar_gtk_menu_append_seperator (GtkMenuShell *menu)
{
  GtkWidget *item;

  _thunar_return_if_fail (GTK_IS_MENU_SHELL (menu));

  item = gtk_separator_menu_item_new ();
  gtk_menu_shell_append (menu, item);
  gtk_widget_show (item);
}



/**
 * thunar_gtk_menu_run:
 * @menu : a #GtkMenu.
 *
 * Conveniance wrapper for thunar_gtk_menu_run_at_event_pointer, to run a menu for the current event
 **/
void
thunar_gtk_menu_run (GtkMenu *menu)
{
  GdkEvent *event = gtk_get_current_event ();
  thunar_gtk_menu_run_at_event (menu, event);
  gdk_event_free (event);
}



#if GTK_CHECK_VERSION (3, 24, 8)
static void
moved_to_rect_cb (GdkWindow          *window,
                  const GdkRectangle *flipped_rect,
                  const GdkRectangle *final_rect,
                  gboolean            flipped_x,
                  gboolean            flipped_y,
                  GtkMenu            *menu)
{
    g_signal_emit_by_name (menu, "popped-up", 0, flipped_rect, final_rect, flipped_x, flipped_y);
    g_signal_stop_emission_by_name (window, "moved-to-rect");
}



static void
popup_menu_realized (GtkWidget *menu,
                     gpointer   user_data)
{
    GdkWindow *toplevel = gtk_widget_get_window (gtk_widget_get_toplevel (menu));
    g_signal_handlers_disconnect_by_func (toplevel, moved_to_rect_cb, menu);
    g_signal_connect (toplevel, "moved-to-rect", G_CALLBACK (moved_to_rect_cb), menu);
}
#endif



/**
 * thunar_gtk_menu_run_at_event:
 * @menu  : a #GtkMenu.
 * @event : a #GdkEvent which may be NULL if no previous event was stored.
 *
 * A simple wrapper around gtk_menu_popup_at_pointer(), which runs the @menu in a separate
 * main loop and returns only after the @menu was deactivated.
 *
 * This method automatically takes over the floating reference of @menu if any and
 * releases it on return. That means if you created the menu via gtk_menu_new() you'll
 * not need to take care of destroying the menu later.
 * 
 **/
void
thunar_gtk_menu_run_at_event (GtkMenu *menu,
                              GdkEvent *event)
{
  GMainLoop *loop;
  gulong     signal_id;

  _thunar_return_if_fail (GTK_IS_MENU (menu));

  /* take over the floating reference on the menu */
  g_object_ref_sink (G_OBJECT (menu));

  /* run an internal main loop */
  loop = g_main_loop_new (NULL, FALSE);
  signal_id = g_signal_connect_swapped (G_OBJECT (menu), "deactivate", G_CALLBACK (g_main_loop_quit), loop);

#if GTK_CHECK_VERSION (3, 24, 8)
    /* Workaround for incorrect popup menus size */
    g_signal_connect (G_OBJECT (menu), "realize", G_CALLBACK (popup_menu_realized), NULL);
    gtk_widget_realize (GTK_WIDGET (menu));
#endif

  gtk_menu_popup_at_pointer (menu, event);
  gtk_menu_reposition (menu);
  gtk_grab_add (GTK_WIDGET (menu));
  g_main_loop_run (loop);
  g_main_loop_unref (loop);
  gtk_grab_remove (GTK_WIDGET (menu));

  g_signal_handler_disconnect (G_OBJECT (menu), signal_id);

  /* release the menu reference */
  g_object_unref (G_OBJECT (menu));
}



/**
 * thunar_gtk_widget_set_tooltip:
 * @widget : a #GtkWidget for which to set the tooltip.
 * @format : a printf(3)-style format string.
 * @...    : additional arguments for @format.
 *
 * Sets the tooltip for the @widget to a string generated
 * from the @format and the additional arguments in @...<!--->.
 **/
void
thunar_gtk_widget_set_tooltip (GtkWidget   *widget,
                               const gchar *format,
                               ...)
{
  va_list  var_args;
  gchar   *tooltip;

  _thunar_return_if_fail (GTK_IS_WIDGET (widget));
  _thunar_return_if_fail (g_utf8_validate (format, -1, NULL));

  /* determine the tooltip */
  va_start (var_args, format);
  tooltip = g_strdup_vprintf (format, var_args);
  va_end (var_args);

  /* setup the tooltip for the widget */
  gtk_widget_set_tooltip_text (widget, tooltip);

  /* release the tooltip */
  g_free (tooltip);
}



/**
 * thunar_gtk_mount_operation_new:
 * @parent : a #GtkWindow or non-toplevel widget.
 *
 * Create a mount operation with some defaults.
 **/
GMountOperation *
thunar_gtk_mount_operation_new (gpointer parent)
{
  GMountOperation *operation;
  GdkScreen       *screen;
  GtkWindow       *window = NULL;

  screen = thunar_util_parse_parent (parent, &window);

  operation = gtk_mount_operation_new (window);
  g_mount_operation_set_password_save (G_MOUNT_OPERATION (operation), G_PASSWORD_SAVE_FOR_SESSION);
  if (window == NULL && screen != NULL)
    gtk_mount_operation_set_screen (GTK_MOUNT_OPERATION (operation), screen);

  return operation;
}



GtkAccelGroup*
thunar_gtk_accel_group_append (GtkAccelGroup *accel_group, const ThunarGtkActionEntry* action_entries, guint n_action_entries, gpointer callback_data)
{
  GtkAccelKey key;
  GClosure   *closure = NULL;

  for (size_t i = 0; i < n_action_entries; i++)
    {
      if (action_entries[i].accel_path == NULL || g_strcmp0 (action_entries[i].accel_path, "") == 0)
        continue;

      /* If the accel path was not loaded to the acel_map via file, we add the default key for it to the accel_map */
      if (gtk_accel_map_lookup_entry (action_entries[i].accel_path, &key) == FALSE)
        {
          gtk_accelerator_parse (action_entries[i].default_accelerator, &key.accel_key, &key.accel_mods);
          gtk_accel_map_add_entry (action_entries[i].accel_path, key.accel_key, key.accel_mods);
        }
      if (action_entries[i].callback != NULL)
        {
          closure = g_cclosure_new_swap (action_entries[i].callback, callback_data, NULL);
          gtk_accel_group_connect_by_path (accel_group, action_entries[i].accel_path, closure);
        }
    }
  return accel_group;
}



ThunarGtkActionEntry*
thunar_gtk_get_action_entry_by_id (ThunarGtkActionEntry *action_entries,
                                   guint                 n_action_entries,
                                   guint                 id)
{
  for (size_t i = 0; i <  n_action_entries; i++)
    {
      if( action_entries[i].id == id )
          return &(action_entries[i]);
    }
  g_error ("There is no action with the id '%i'.", id);
  return NULL;
}



void
thunar_gtk_translate_action_entries (ThunarGtkActionEntry *action_entries,
                                     guint                 n_action_entries)
{
  for (size_t i = 0; i <  n_action_entries; i++)
    {
      action_entries[i].menu_item_label_text = g_strdup( g_dgettext (NULL, action_entries[i].menu_item_label_text));
      action_entries[i].menu_item_tooltip_text = g_strdup( g_dgettext (NULL, action_entries[i].menu_item_tooltip_text));
    }
}



GtkWidget*
thunar_gtk_tool_button_new_from_action_entry (GtkToolbar           *toolbar,
                                                    ThunarGtkActionEntry *action_entry,
                                                    GObject              *callback_param,
                                                    gboolean              sensitive)
{
  GtkToolItem *tool_item;
  GtkWidget   *image;

  image = gtk_image_new_from_icon_name (action_entry->menu_item_icon_name, GTK_ICON_SIZE_LARGE_TOOLBAR);
  tool_item = gtk_tool_button_new (image, action_entry->menu_item_label_text);
  g_signal_connect_swapped (G_OBJECT (tool_item), "clicked", action_entry->callback, callback_param);
  gtk_widget_set_tooltip_text (GTK_WIDGET (tool_item), action_entry->menu_item_tooltip_text);
  gtk_widget_show (GTK_WIDGET (tool_item));
  gtk_widget_show (image);
  gtk_widget_set_sensitive (GTK_WIDGET (tool_item), sensitive);
  gtk_toolbar_insert (toolbar, tool_item, -1);
  return GTK_WIDGET (tool_item);
}
