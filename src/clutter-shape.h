/*
 * Castet - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
 *
 * Authored by: Øyvind Kolås <pippin@linux.intel.com>
 *              Damien Lespiau <damien.lespiau@intel.com>
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

#ifndef __CLUTTER_SHAPE_H__
#define __CLUTTER_SHAPE_H__

#include <glib-object.h>
#include <clutter/clutter.h>

G_BEGIN_DECLS

#define CLUTTER_TYPE_SHAPE clutter_shape_get_type()

#define CLUTTER_SHAPE(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), CLUTTER_TYPE_SHAPE, ClutterShape))

#define CLUTTER_SHAPE_CLASS(klass)                  \
    (G_TYPE_CHECK_CLASS_CAST ((klass),              \
                              CLUTTER_TYPE_SHAPE,   \
                              ClutterShapeClass))

#define CLUTTER_IS_SHAPE(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CLUTTER_TYPE_SHAPE))

#define CLUTTER_IS_SHAPE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), CLUTTER_TYPE_SHAPE))

#define CLUTTER_SHAPE_GET_CLASS(obj)                \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),              \
                                CLUTTER_TYPE_SHAPE, \
                                ClutterShapeClass))

typedef struct _ClutterShape        ClutterShape;
typedef struct _ClutterShapeClass   ClutterShapeClass;
typedef struct _ClutterShapePrivate ClutterShapePrivate;

struct _ClutterShape
{
    ClutterActor parent;
    ClutterShapePrivate  *priv;
};

struct _ClutterShapeClass
{
    ClutterActorClass parent_class;
};

GType clutter_shape_get_type (void) G_GNUC_CONST;

ClutterActor *clutter_shape_new            (void);

G_END_DECLS

#endif
