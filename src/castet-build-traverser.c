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

#include "castet-build-traverser.h"

G_DEFINE_TYPE (CastetBuildTraverser,
               castet_build_traverser,
               CASTET_TYPE_TRAVERSER)

#define BUILD_TRAVERSER_PRIVATE(o)                                  \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                          \
                                      CASTET_TYPE_BUILD_TRAVERSER,  \
                                      CastetBuildTraverserPrivate))

struct _CastetBuildTraverserPrivate
{
    ClutterContainer *container;
};

static void
castet_build_traverser_traverse_rect (CastetTraverser   *traverser,
                                      CastetRectElement *node)
{
    CastetBuildTraverser *build = CASTET_BUILD_TRAVERSER (traverser);
    CastetBuildTraverserPrivate *priv = build->priv;
    ClutterActor *rectangle;
    ClutterColor color;
    ClutterGeometry geom;

    clutter_color_from_hls (&color,
                            g_random_double_range (0.0, 360.0), 0.5, 0.5);
    rectangle = clutter_rectangle_new_with_color (&color);
    geom.x = castet_rect_element_get_x_px (node);
    geom.y = castet_rect_element_get_y_px (node);
    geom.width = castet_rect_element_get_width_px (node);
    geom.height = castet_rect_element_get_height_px (node);
    clutter_actor_set_geometry (rectangle, &geom);

    clutter_container_add_actor (priv->container, rectangle);
}

static void
castet_build_traverser_traverse_polyline (CastetTraverser       *traverser,
                                          CastetPolylineElement *node)
{
}

/*
 * GObject overloading
 */

static void
castet_build_traverser_get_property (GObject    *object,
                                     guint       property_id,
                                     GValue     *value,
                                     GParamSpec *pspec)
{
    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
castet_build_traverser_set_property (GObject      *object,
                                     guint         property_id,
                                     const GValue *value,
                                     GParamSpec   *pspec)
{
    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
castet_build_traverser_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_build_traverser_parent_class)->dispose (object);
}

static void
castet_build_traverser_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_build_traverser_parent_class)->finalize (object);
}

static void
castet_build_traverser_class_init (CastetBuildTraverserClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    CastetTraverserClass *traverser_class = CASTET_TRAVERSER_CLASS (klass);

    g_type_class_add_private (klass, sizeof (CastetBuildTraverserPrivate));

    object_class->get_property = castet_build_traverser_get_property;
    object_class->set_property = castet_build_traverser_set_property;
    object_class->dispose = castet_build_traverser_dispose;
    object_class->finalize = castet_build_traverser_finalize;

    traverser_class->traverse_rect = castet_build_traverser_traverse_rect;
    traverser_class->traverse_polyline =
        castet_build_traverser_traverse_polyline;
}

static void
castet_build_traverser_init (CastetBuildTraverser *self)
{
    self->priv = BUILD_TRAVERSER_PRIVATE (self);
}

CastetTraverser *
castet_build_traverser_new (CastetDomNode    *node,
                            ClutterContainer *container)
{
    CastetTraverser *traverser;

    traverser = g_object_new (CASTET_TYPE_BUILD_TRAVERSER, NULL);
    castet_traverser_set_root (traverser, node);
    castet_build_traverser_set_container (CASTET_BUILD_TRAVERSER (traverser),
                                          container);

    return traverser;
}

void
castet_build_traverser_set_container (CastetBuildTraverser *self,
                                      ClutterContainer     *container)
{
    CastetBuildTraverserPrivate *priv;

    g_return_if_fail (CASTET_IS_BUILD_TRAVERSER (self));
    priv = self->priv;

    if (priv->container)
        g_object_unref (priv->container);
    priv->container = g_object_ref (container);
}
