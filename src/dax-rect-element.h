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

#ifndef __DAX_RECT_ELEMENT_H__
#define __DAX_RECT_ELEMENT_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_RECT_ELEMENT dax_rect_element_get_type()

#define DAX_RECT_ELEMENT(obj)                            \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 DAX_TYPE_RECT_ELEMENT,  \
                                 DaxRectElement))

#define DAX_RECT_ELEMENT_CLASS(klass)                \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              DAX_TYPE_RECT_ELEMENT, \
                              DaxRectElementClass))

#define DAX_IS_RECT_ELEMENT(obj)     \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 DAX_TYPE_RECT_ELEMENT))

#define DAX_IS_RECT_ELEMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),      \
                              DAX_TYPE_RECT_ELEMENT))

#define DAX_RECT_ELEMENT_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_RECT_ELEMENT,   \
                                DaxRectElementClass))

typedef struct _DaxRectElement DaxRectElement;
typedef struct _DaxRectElementClass DaxRectElementClass;
typedef struct _DaxRectElementPrivate DaxRectElementPrivate;

struct _DaxRectElement
{
    DaxElement parent;

    DaxRectElementPrivate *priv;
};

struct _DaxRectElementClass
{
    DaxElementClass parent_class;
};

GType dax_rect_element_get_type (void) G_GNUC_CONST;

DaxDomElement *dax_rect_element_new       (void);

void              dax_rect_element_set_x         (DaxRectElement  *self,
                                                     const ClutterUnits *x);
void              dax_rect_element_set_y         (DaxRectElement  *self,
                                                     const ClutterUnits *y);
void              dax_rect_element_set_width     (DaxRectElement  *self,
                                                     const ClutterUnits *width);
void              dax_rect_element_set_height    (DaxRectElement  *self,
                                                     const ClutterUnits *height);
void              dax_rect_element_set_rx        (DaxRectElement  *self,
                                                     const ClutterUnits *rx);
void              dax_rect_element_set_ry        (DaxRectElement  *self,
                                                     const ClutterUnits *ry);

gfloat            dax_rect_element_get_x_px      (DaxRectElement  *self);
gfloat            dax_rect_element_get_y_px      (DaxRectElement  *self);
gfloat            dax_rect_element_get_width_px  (DaxRectElement  *self);
gfloat            dax_rect_element_get_height_px (DaxRectElement  *self);
gfloat            dax_rect_element_set_rx_px     (DaxRectElement  *self);
gfloat            dax_rect_element_set_ry_px     (DaxRectElement  *self);

G_END_DECLS

#endif /* __DAX_RECT_ELEMENT_H__ */
