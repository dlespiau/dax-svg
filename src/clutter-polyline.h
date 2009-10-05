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

#ifndef __CLUTTER_POLYLINE_H__
#define __CLUTTER_POLYGON_H__

#include <glib-object.h>
#include <clutter/clutter.h>

G_BEGIN_DECLS

#define CLUTTER_TYPE_POLYLINE clutter_polyline_get_type()

#define CLUTTER_POLYLINE(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), CLUTTER_TYPE_POLYLINE, ClutterPolyline))

#define CLUTTER_POLYLINE_CLASS(klass)                   \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              CLUTTER_TYPE_POLYLINE,    \
                              ClutterPolylineClass))

#define CLUTTER_IS_POLYLINE(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CLUTTER_TYPE_POLYLINE))

#define CLUTTER_IS_POLYLINE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), CLUTTER_TYPE_POLYLINE))

#define CLUTTER_POLYLINE_GET_CLASS(obj)                 \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                CLUTTER_TYPE_POLYLINE,  \
                                ClutterPolylineClass))

typedef struct _ClutterPolyline        ClutterPolyline;
typedef struct _ClutterPolylineClass   ClutterPolylineClass;
typedef struct _ClutterPolylinePrivate ClutterPolylinePrivate;

struct _ClutterPolyline
{
    ClutterActor parent;
    ClutterPolylinePrivate  *priv;
};

struct _ClutterPolylineClass
{
    ClutterActorClass parent_class;
};

GType clutter_polyline_get_type (void) G_GNUC_CONST;

ClutterActor *clutter_polyline_new            (void);

G_END_DECLS

#endif
