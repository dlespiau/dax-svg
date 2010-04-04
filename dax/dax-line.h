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

#ifndef __DAX_LINE_H__
#define __DAX_LINE_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_LINE dax_line_get_type()

#define DAX_LINE(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), DAX_TYPE_LINE, DaxLine))

#define DAX_LINE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), DAX_TYPE_LINE, DaxLineClass))

#define DAX_IS_LINE(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_LINE))

#define DAX_IS_LINE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_LINE))

#define DAX_LINE_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), DAX_TYPE_LINE, DaxLineClass))

typedef struct _DaxLine DaxLine;
typedef struct _DaxLineClass DaxLineClass;
typedef struct _DaxLinePrivate DaxLinePrivate;

struct _DaxLine
{
    DaxElement parent;

    DaxLinePrivate *priv;
};

struct _DaxLineClass
{
    DaxElementClass parent_class;
};

GType               dax_line_get_type   (void) G_GNUC_CONST;

DaxDomElement *     dax_line_new        (void);

ClutterUnits *      dax_line_get_x1     (const DaxLine *line);
ClutterUnits *      dax_line_get_y1     (const DaxLine *line);
ClutterUnits *      dax_line_get_x2     (const DaxLine *line);
ClutterUnits *      dax_line_get_y2     (const DaxLine *line);

G_END_DECLS

#endif /* __DAX_LINE_H__ */
