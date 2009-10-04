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

#include "castet-internals.h"
#include "castet-knot-sequence.h"
#include "castet-polyline-element.h"

G_DEFINE_TYPE (CastetPolylineElement,
               castet_polyline_element,
               CASTET_TYPE_ELEMENT)

#define POLYLINE_ELEMENT_PRIVATE(o)                             \
    (G_TYPE_INSTANCE_GET_PRIVATE ((o),                          \
                                  CASTET_TYPE_POLYLINE_ELEMENT, \
                                  CastetPolylineElementPrivate))

enum {
    PROP_0,

    PROP_POINTS
};

struct _CastetPolylineElementPrivate
{
    CastetKnotSequence *knots;
};

static void
castet_polyline_element_get_property (GObject    *object,
                                      guint       property_id,
                                      GValue     *value,
                                      GParamSpec *pspec)
{
    CastetPolylineElement *polyline = CASTET_POLYLINE_ELEMENT (object);
    CastetPolylineElementPrivate *priv = polyline->priv;

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
castet_polyline_element_set_property (GObject      *object,
                                      guint         property_id,
                                      const GValue *value,
                                      GParamSpec   *pspec)
{
    CastetPolylineElement *polyline = CASTET_POLYLINE_ELEMENT (object);
    CastetPolylineElementPrivate *priv = polyline->priv;

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
castet_polyline_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_polyline_element_parent_class)->dispose (object);
}

static void
castet_polyline_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_polyline_element_parent_class)->finalize (object);
}

static void
castet_polyline_element_class_init (CastetPolylineElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass,
                              sizeof (CastetPolylineElementPrivate));

    object_class->get_property = castet_polyline_element_get_property;
    object_class->set_property = castet_polyline_element_set_property;
    object_class->dispose = castet_polyline_element_dispose;
    object_class->finalize = castet_polyline_element_finalize;

    pspec = g_param_spec_object ("points",
                                 "Points",
                                 "", /* FIXME */
                                 CASTET_TYPE_KNOT_SEQUENCE,
                                 CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_POINTS, pspec);
}

static void
castet_polyline_element_init (CastetPolylineElement *self)
{
    CastetPolylineElementPrivate *priv;

    self->priv = priv = POLYLINE_ELEMENT_PRIVATE (self);

    priv->knots = castet_knot_sequence_new ();
}

CastetDomElement *
castet_polyline_element_new (void)
{
    return g_object_new (CASTET_TYPE_POLYLINE_ELEMENT, NULL);
}
