/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __DAX_ELEMENT_RECT_H__
#define __DAX_ELEMENT_RECT_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_ELEMENT_RECT dax_element_rect_get_type()

#define DAX_ELEMENT_RECT(obj)                            \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 DAX_TYPE_ELEMENT_RECT,  \
                                 DaxElementRect))

#define DAX_ELEMENT_RECT_CLASS(klass)                \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              DAX_TYPE_ELEMENT_RECT, \
                              DaxElementRectClass))

#define DAX_IS_ELEMENT_RECT(obj)     \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 DAX_TYPE_ELEMENT_RECT))

#define DAX_IS_ELEMENT_RECT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),      \
                              DAX_TYPE_ELEMENT_RECT))

#define DAX_ELEMENT_RECT_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_ELEMENT_RECT,   \
                                DaxElementRectClass))

typedef struct _DaxElementRect DaxElementRect;
typedef struct _DaxElementRectClass DaxElementRectClass;
typedef struct _DaxElementRectPrivate DaxElementRectPrivate;

struct _DaxElementRect
{
    DaxElement parent;

    DaxElementRectPrivate *priv;
};

struct _DaxElementRectClass
{
    DaxElementClass parent_class;
};

GType dax_element_rect_get_type (void) G_GNUC_CONST;

DaxDomElement *dax_element_rect_new       (void);

void              dax_element_rect_set_x         (DaxElementRect  *self,
                                                     const ClutterUnits *x);
void              dax_element_rect_set_y         (DaxElementRect  *self,
                                                     const ClutterUnits *y);
void              dax_element_rect_set_width     (DaxElementRect  *self,
                                                     const ClutterUnits *width);
void              dax_element_rect_set_height    (DaxElementRect  *self,
                                                     const ClutterUnits *height);
void              dax_element_rect_set_rx        (DaxElementRect  *self,
                                                     const ClutterUnits *rx);
void              dax_element_rect_set_ry        (DaxElementRect  *self,
                                                     const ClutterUnits *ry);

gfloat            dax_element_rect_get_x_px      (DaxElementRect  *self);
gfloat            dax_element_rect_get_y_px      (DaxElementRect  *self);
gfloat            dax_element_rect_get_width_px  (DaxElementRect  *self);
gfloat            dax_element_rect_get_height_px (DaxElementRect  *self);
gfloat            dax_element_rect_set_rx_px     (DaxElementRect  *self);
gfloat            dax_element_rect_set_ry_px     (DaxElementRect  *self);

G_END_DECLS

#endif /* __DAX_ELEMENT_RECT_H__ */
