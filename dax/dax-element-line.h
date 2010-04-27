/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
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

#if !defined(__DAX_H_INSIDE__) && !defined(DAX_COMPILATION)
#error "Only <dax/dax.h> can be included directly."
#endif

#ifndef __DAX_ELEMENT_LINE_H__
#define __DAX_ELEMENT_LINE_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_ELEMENT_LINE dax_element_line_get_type()

#define DAX_ELEMENT_LINE(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), DAX_TYPE_ELEMENT_LINE, DaxElementLine))

#define DAX_ELEMENT_LINE_CLASS(klass)                   \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              DAX_TYPE_ELEMENT_LINE,    \
                              DaxElementLineClass))

#define DAX_IS_ELEMENT_LINE(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_ELEMENT_LINE))

#define DAX_IS_ELEMENT_LINE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_ELEMENT_LINE))

#define DAX_ELEMENT_LINE_GET_CLASS(obj)                 \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                DAX_TYPE_ELEMENT_LINE,  \
                                DaxElementLineClass))

typedef struct _DaxElementLine DaxElementLine;
typedef struct _DaxElementLineClass DaxElementLineClass;
typedef struct _DaxElementLinePrivate DaxElementLinePrivate;

struct _DaxElementLine
{
    DaxElement parent;

    DaxElementLinePrivate *priv;
};

struct _DaxElementLineClass
{
    DaxElementClass parent_class;
};

GType               dax_element_line_get_type   (void) G_GNUC_CONST;

DaxDomElement *     dax_element_line_new        (void);

ClutterUnits *      dax_element_line_get_x1     (const DaxElementLine *line);
ClutterUnits *      dax_element_line_get_y1     (const DaxElementLine *line);
ClutterUnits *      dax_element_line_get_x2     (const DaxElementLine *line);
ClutterUnits *      dax_element_line_get_y2     (const DaxElementLine *line);

G_END_DECLS

#endif /* __DAX_ELEMENT_LINE_H__ */
