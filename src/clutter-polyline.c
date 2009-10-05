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

#include "castet-knot-sequence.h"
#include "clutter-polyline.h"

#ifndef CLUTTER_PARAM_READWRITE
#define CLUTTER_PARAM_READWRITE \
    G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK |G_PARAM_STATIC_BLURB
#endif


G_DEFINE_TYPE (ClutterPolyline, clutter_polyline, CLUTTER_TYPE_ACTOR);

enum
{
    PROP_0,
    PROP_COLOR,
    PROP_KNOTS,
};

#define CLUTTER_POLYLINE_GET_PRIVATE(obj) \
    (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CLUTTER_TYPE_POLYLINE, ClutterPolylinePrivate))

struct _ClutterPolylinePrivate
{
    ClutterColor color;
    CastetKnotSequence *knots;
};

static void
clutter_polyline_pick (ClutterActor       *self,
                       const ClutterColor *color)
{
    ClutterPolyline          *polyline = CLUTTER_POLYLINE(self);
    ClutterPolylinePrivate *priv = polyline->priv;

    if (clutter_actor_should_pick_paint (self))
    {
        cogl_set_source_color4ub (color->red,
                                  color->green,
                                  color->blue,
                                  color->alpha);
        cogl_path_polyline (castet_knot_sequence_get_array (priv->knots),
                            castet_knot_sequence_get_size (priv->knots));
        cogl_path_fill();
    }
}

static void
clutter_polyline_paint (ClutterActor *self)
{
    ClutterPolyline        *polyline = CLUTTER_POLYLINE(self);
    ClutterPolylinePrivate *priv = polyline->priv;
    ClutterColor            tmp_col;

    tmp_col = priv->color;
    tmp_col.alpha = clutter_actor_get_paint_opacity (self)
        * priv->color.alpha
        / 255;

    cogl_set_source_color ((CoglColor*)&tmp_col);

    cogl_path_polyline (castet_knot_sequence_get_array (priv->knots),
                        castet_knot_sequence_get_size (priv->knots));
    cogl_path_fill();
}

/*
 * GObject overloading
 */

static void
clutter_polyline_get_property (GObject    *object,
                               guint       prop_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
    ClutterPolyline *polyline = CLUTTER_POLYLINE(object);
    ClutterPolylinePrivate *priv = polyline->priv;

    switch (prop_id)
    {
    case PROP_COLOR:
        g_value_set_boxed (value, &priv->color);
        break;
    case PROP_KNOTS:
        {
            g_value_set_object (value, priv->knots);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
clutter_polyline_set_property (GObject      *object,
                               guint         prop_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
    ClutterPolyline *polyline = CLUTTER_POLYLINE(object);
    ClutterPolylinePrivate *priv = polyline->priv;

    switch (prop_id)
    {
    case PROP_COLOR:
        {
            ClutterColor *color = g_value_get_boxed (value);
            priv->color = *color;
            clutter_actor_queue_redraw (CLUTTER_ACTOR (object));
        }
        break;
    case PROP_KNOTS:
        if (priv->knots)
            g_object_unref (priv->knots);
        priv->knots = g_value_get_object (value);
        clutter_actor_queue_redraw (CLUTTER_ACTOR (object));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
clutter_polyline_finalize (GObject *object)
{
    ClutterPolyline          *polyline = CLUTTER_POLYLINE(object);
    ClutterPolylinePrivate *priv  = polyline->priv;

    if (priv->knots)
        g_object_unref (priv->knots);

    G_OBJECT_CLASS (clutter_polyline_parent_class)->finalize (object);
}

static void
clutter_polyline_dispose (GObject *object)
{
    G_OBJECT_CLASS (clutter_polyline_parent_class)->dispose (object);
}

static void
clutter_polyline_class_init (ClutterPolylineClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (gobject_class, sizeof (ClutterPolylinePrivate));

    actor_class->paint        = clutter_polyline_paint;
    actor_class->pick         = clutter_polyline_pick;

    gobject_class->finalize     = clutter_polyline_finalize;
    gobject_class->dispose      = clutter_polyline_dispose;
    gobject_class->set_property = clutter_polyline_set_property;
    gobject_class->get_property = clutter_polyline_get_property;

    pspec = g_param_spec_boxed ("color",
                                "Color",
                                "The color of the polyline",
                                CLUTTER_TYPE_COLOR,
                                CLUTTER_PARAM_READWRITE);
    g_object_class_install_property (gobject_class, PROP_COLOR, pspec);

    pspec = g_param_spec_object ("knots",
                                 "Sequence of knots",
                                 "", /* FIXME */
                                 CASTET_TYPE_KNOT_SEQUENCE,
                                 CLUTTER_PARAM_READWRITE);
    g_object_class_install_property (gobject_class, PROP_KNOTS, pspec);
}

static void
clutter_polyline_init (ClutterPolyline *self)
{
    ClutterPolylinePrivate *priv;

    self->priv = priv = CLUTTER_POLYLINE_GET_PRIVATE (self);

    priv->color.red = 0xff;
    priv->color.green = 0xff;
    priv->color.blue = 0xff;
    priv->color.alpha = 0xff;
}

/**
 * clutter_polyline_new:
 *
 * Creates a new #ClutterActor with a rectangular polyline.
 *
 * Return value: a new #ClutterActor
 */
ClutterActor*
clutter_polyline_new (void)
{
    return g_object_new (CLUTTER_TYPE_POLYLINE, NULL);
}
