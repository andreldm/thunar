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

#ifndef __THUNAR_GTK_EXTENSIONS_H__
#define __THUNAR_GTK_EXTENSIONS_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS;


typedef enum
{
  THUNAR_GTK_MENU_ITEM,
  THUNAR_GTK_IMAGE_MENU_ITEM,
  THUNAR_GTK_CHECK_MENU_ITEM,
  THUNAR_GTK_RADIO_MENU_ITEM,
} ThunarGtkMenuItem;

typedef struct
{
  /* unique identifier for instances of this structure */
  guint             id;

  const gchar      *accel_path;
  const gchar      *default_accelerator;

  /* menu_item data is optional, only relevant if there exists a menu_item for that accelerator */
  ThunarGtkMenuItem menu_item_type;
  gchar            *menu_item_label_text;
  gchar            *menu_item_tooltip_text;
  const gchar      *menu_item_icon_name;

  GCallback       callback;
} ThunarGtkActionEntry;

void             thunar_gtk_label_set_a11y_relation           (GtkLabel           *label,
                                                               GtkWidget          *widget);
GtkWidget       *thunar_gtk_menu_item_new                  (const gchar        *label_text,
                                                               const gchar *tooltip_text,
                                                               const gchar *accel_path,
                                                               GCallback    callback,
                                                               GObject     *callback_param,
                                                               GtkMenuShell       *menu_to_append_item);
GtkWidget       *thunar_gtk_image_menu_item_new            (const gchar        *label_text,
                                                               const gchar        *tooltip_text,
                                                               const gchar        *accel_path,
                                                               GCallback           callback,
                                                               GObject            *callback_param,
                                                               GtkWidget          *image,
                                                               GtkMenuShell       *menu_to_append_item);
GtkWidget       *thunar_gtk_image_menu_item_new_from_icon_name (const gchar *label_text,
                                                                   const gchar *tooltip_text,
                                                                   const gchar *accel_path,
                                                                   GCallback    callback,
                                                                   GObject     *callback_param,
                                                                   const gchar *icon_name,
                                                                   GtkMenuShell       *menu_to_append_item);
GtkWidget       *thunar_gtk_menu_thunarx_menu_item_new     (GObject            *thunarx_menu_item,
                                                               GtkMenuShell  *menu_to_append_item);
GtkWidget       *thunar_gtk_check_menu_item_new            (const gchar        *label_text,
                                                               const gchar        *tooltip_text,
                                                               const gchar        *accel_path,
                                                               GCallback           callback,
                                                               GObject            *callback_param,
                                                               gboolean      active,
                                                               GtkMenuShell       *menu_to_append_item);
GtkWidget       *thunar_gtk_radio_menu_item_new            (const gchar        *label_text,
                                                               const gchar        *tooltip_text,
                                                               const gchar        *accel_path,
                                                               GCallback           callback,
                                                               GObject            *callback_param,
                                                               gboolean      active,
                                                               GtkMenuShell       *menu_to_append_item);
GtkWidget       *thunar_gtk_menu_item_new_from_action_entry(ThunarGtkActionEntry *action_entry,
                                                               GObject              *callback_param,
                                                               GtkMenuShell         *menu_to_append_item);
GtkWidget       *thunar_gtk_toggle_menu_item_new_from_action_entry(ThunarGtkActionEntry *action_entry,
                                                               GObject              *callback_param,
                                                               gboolean      active,
                                                               GtkMenuShell         *menu_to_append_item);
void             thunar_gtk_menu_append_seperator             (GtkMenuShell            *menu);
void             thunar_gtk_menu_run                          (GtkMenu            *menu);

void             thunar_gtk_menu_run_at_event                 (GtkMenu            *menu,
                                                               GdkEvent           *event);
GtkWidget       *thunar_gtk_tool_button_new_from_action_entry (GtkToolbar           *toolbar,
                                                                     ThunarGtkActionEntry *action_entry,
                                                                     GObject              *callback_param);
void             thunar_gtk_widget_set_tooltip                (GtkWidget          *widget,
                                                               const gchar        *format,
                                                               ...) G_GNUC_PRINTF (2, 3);

GMountOperation *thunar_gtk_mount_operation_new               (gpointer            parent);

GtkAccelGroup   *thunar_gtk_accel_group_append                (GtkAccelGroup              *accel_group,
                                                               const ThunarGtkActionEntry *action_entries,
                                                               guint                       n_action_entries,
                                                               gpointer                    callback_data);
ThunarGtkActionEntry* thunar_gtk_get_action_entry_by_id       (ThunarGtkActionEntry       *action_entries,
                                                               guint                       n_action_entries,
                                                               guint                       id);
void                  thunar_gtk_translate_action_entries     (ThunarGtkActionEntry       *action_entries,
                                                               guint                       n_action_entries);

G_GNUC_BEGIN_IGNORE_DEPRECATIONS

static inline int
thunar_gtk_widget_get_allocation_x (GtkWidget *widget)
{
    GtkAllocation allocation;

    gtk_widget_get_allocation (widget, &allocation);

    return allocation.x;
}

static inline int
thunar_gtk_widget_get_allocation_y (GtkWidget *widget)
{
    GtkAllocation allocation;

    gtk_widget_get_allocation (widget, &allocation);

    return allocation.y;
}

static inline int
thunar_gtk_widget_get_allocation_width (GtkWidget *widget)
{
    GtkAllocation allocation;

    gtk_widget_get_allocation (widget, &allocation);

    return allocation.width;
}

static inline int
thunar_gtk_widget_get_allocation_height (GtkWidget *widget)
{
    GtkAllocation allocation;

    gtk_widget_get_allocation (widget, &allocation);

    return allocation.height;
}

static inline int
thunar_gtk_widget_get_requisition_width (GtkWidget *widget)
{
    GtkRequisition requisition;

    gtk_widget_get_requisition (widget, &requisition);

    return requisition.width;
}

static inline int
thunar_gtk_widget_get_requisition_height (GtkWidget *widget)
{
    GtkRequisition requisition;

    gtk_widget_get_requisition (widget, &requisition);

    return requisition.height;
}

G_GNUC_END_IGNORE_DEPRECATIONS

G_END_DECLS;

#endif /* !__THUNAR_GTK_EXTENSIONS_H__ */
