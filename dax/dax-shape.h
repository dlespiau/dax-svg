/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
 *
 * Authored by: Damien Lespiau <damien.lespiau@intel.com>
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
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __DAX_SHAPE_H__
#define __DAX_SHAPE_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "clutter-shape.h"
#include "dax-types.h"

G_BEGIN_DECLS

#define DAX_TYPE_SHAPE dax_shape_get_type()

#define DAX_SHAPE(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),  DAX_TYPE_SHAPE, DaxShape))

#define DAX_SHAPE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), DAX_TYPE_SHAPE, DaxShapeClass))

#define DAX_IS_SHAPE(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_SHAPE))

#define DAX_IS_SHAPE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_SHAPE))

#define DAX_SHAPE_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), DAX_TYPE_SHAPE, DaxShapeClass))

typedef struct _DaxShape DaxShape;
typedef struct _DaxShapeClass DaxShapeClass;
typedef struct _DaxShapePrivate DaxShapePrivate;

struct _DaxShape
{
    ClutterShape parent;

    DaxShapePrivate *priv;
};

struct _DaxShapeClass
{
    ClutterShapeClass parent_class;
};

GType           dax_shape_get_type      (void) G_GNUC_CONST;

ClutterActor *  dax_shape_new           (void);
void            dax_shape_set_matrix    (DaxShape        *self,
                                         const DaxMatrix *matrix);

G_END_DECLS

#endif /* __DAX_SHAPE_H__ */
