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

#include "dax-internals.h"
#include "dax-knot-sequence.h"
#include "dax-polyline-element.h"

G_DEFINE_TYPE (DaxPolylineElement,
               dax_polyline_element,
               DAX_TYPE_ELEMENT)

#define POLYLINE_ELEMENT_PRIVATE(o)                             \
    (G_TYPE_INSTANCE_GET_PRIVATE ((o),                          \
                                  DAX_TYPE_POLYLINE_ELEMENT, \
                                  DaxPolylineElementPrivate))

enum {
    PROP_0,

    PROP_POINTS
};

struct _DaxPolylineElementPrivate
{
    DaxKnotSequence *knots;
};

static void
dax_polyline_element_get_property (GObject    *object,
                                      guint       property_id,
                                      GValue     *value,
                                      GParamSpec *pspec)
{
    DaxPolylineElement *polyline = DAX_POLYLINE_ELEMENT (object);
    DaxPolylineElementPrivate *priv = polyline->priv;

    switch (property_id)
    {
    case PROP_POINTS:
        g_value_set_object (value, priv->knots);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_polyline_element_set_property (GObject      *object,
                                      guint         property_id,
                                      const GValue *value,
                                      GParamSpec   *pspec)
{
    DaxPolylineElement *polyline = DAX_POLYLINE_ELEMENT (object);
    DaxPolylineElementPrivate *priv = polyline->priv;

    switch (property_id)
    {
    case PROP_POINTS:
        if (priv->knots)
            g_object_unref (priv->knots);
        priv->knots = g_value_get_object (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_polyline_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_polyline_element_parent_class)->dispose (object);
}

static void
dax_polyline_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_polyline_element_parent_class)->finalize (object);
}

static void
dax_polyline_element_class_init (DaxPolylineElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass,
                              sizeof (DaxPolylineElementPrivate));

    object_class->get_property = dax_polyline_element_get_property;
    object_class->set_property = dax_polyline_element_set_property;
    object_class->dispose = dax_polyline_element_dispose;
    object_class->finalize = dax_polyline_element_finalize;

    pspec = g_param_spec_object ("points",
                                 "Points",
                                 "", /* FIXME */
                                 DAX_TYPE_KNOT_SEQUENCE,
                                 DAX_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_POINTS, pspec);
}

static void
dax_polyline_element_init (DaxPolylineElement *self)
{
    DaxPolylineElementPrivate *priv;

    self->priv = priv = POLYLINE_ELEMENT_PRIVATE (self);

    priv->knots = dax_knot_sequence_new ();
}

DaxDomElement *
dax_polyline_element_new (void)
{
    return g_object_new (DAX_TYPE_POLYLINE_ELEMENT, NULL);
}
