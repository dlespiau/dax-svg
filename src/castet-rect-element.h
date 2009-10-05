/*
 * Castet - Load and draw SVG
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

#ifndef __CASTET_RECT_ELEMENT_H__
#define __CASTET_RECT_ELEMENT_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "castet-element.h"

G_BEGIN_DECLS

#define CASTET_TYPE_RECT_ELEMENT castet_rect_element_get_type()

#define CASTET_RECT_ELEMENT(obj)                            \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 CASTET_TYPE_RECT_ELEMENT,  \
                                 CastetRectElement))

#define CASTET_RECT_ELEMENT_CLASS(klass)                \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              CASTET_TYPE_RECT_ELEMENT, \
                              CastetRectElementClass))

#define CASTET_IS_RECT_ELEMENT(obj)     \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 CASTET_TYPE_RECT_ELEMENT))

#define CASTET_IS_RECT_ELEMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),      \
                              CASTET_TYPE_RECT_ELEMENT))

#define CASTET_RECT_ELEMENT_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                CASTET_TYPE_RECT_ELEMENT,   \
                                CastetRectElementClass))

typedef struct _CastetRectElement CastetRectElement;
typedef struct _CastetRectElementClass CastetRectElementClass;
typedef struct _CastetRectElementPrivate CastetRectElementPrivate;

struct _CastetRectElement
{
    CastetElement parent;

    CastetRectElementPrivate *priv;
};

struct _CastetRectElementClass
{
    CastetElementClass parent_class;
};

GType castet_rect_element_get_type (void) G_GNUC_CONST;

CastetDomElement *castet_rect_element_new       (void);

void              castet_rect_element_set_x         (CastetRectElement  *self,
                                                     const ClutterUnits *x);
void              castet_rect_element_set_y         (CastetRectElement  *self,
                                                     const ClutterUnits *y);
void              castet_rect_element_set_width     (CastetRectElement  *self,
                                                     const ClutterUnits *width);
void              castet_rect_element_set_height    (CastetRectElement  *self,
                                                     const ClutterUnits *height);
void              castet_rect_element_set_rx        (CastetRectElement  *self,
                                                     const ClutterUnits *rx);
void              castet_rect_element_set_ry        (CastetRectElement  *self,
                                                     const ClutterUnits *ry);

gfloat            castet_rect_element_get_x_px      (CastetRectElement  *self);
gfloat            castet_rect_element_get_y_px      (CastetRectElement  *self);
gfloat            castet_rect_element_get_width_px  (CastetRectElement  *self);
gfloat            castet_rect_element_get_height_px (CastetRectElement  *self);
gfloat            castet_rect_element_set_rx_px     (CastetRectElement  *self);
gfloat            castet_rect_element_set_ry_px     (CastetRectElement  *self);

G_END_DECLS

#endif /* __CASTET_RECT_ELEMENT_H__ */
