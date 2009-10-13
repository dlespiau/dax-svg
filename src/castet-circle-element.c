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

#include <clutter/clutter.h>

#include "castet-internals.h"
#include "castet-circle-element.h"

G_DEFINE_TYPE (CastetCircleElement, castet_circle_element, CASTET_TYPE_ELEMENT)

#define CIRCLE_ELEMENT_PRIVATE(o)                                   \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                          \
                                      CASTET_TYPE_CIRCLE_ELEMENT,   \
                                      CastetCircleElementPrivate))

enum
{
    PROP_0,

    PROP_CX,
    PROP_CY,
    PROP_RADIUS
};

struct _CastetCircleElementPrivate
{
    ClutterUnits *cx;
    ClutterUnits *cy;
    ClutterUnits *radius;
};

static void
castet_circle_element_set_cx (CastetCircleElement  *self,
                              const ClutterUnits   *cx)
{
    CastetCircleElementPrivate *priv;

    priv = self->priv;
    if (priv->cx)
        clutter_units_free (priv->cx);

    priv->cx = clutter_units_copy (cx);
}

static void
castet_circle_element_set_cy (CastetCircleElement  *self,
                              const ClutterUnits   *cy)
{
    CastetCircleElementPrivate *priv;

    priv = self->priv;
    if (priv->cy)
        clutter_units_free (priv->cy);

    priv->cy = clutter_units_copy (cy);
}

static void
castet_circle_element_set_radius (CastetCircleElement  *self,
                                  const ClutterUnits   *radius)
{
    CastetCircleElementPrivate *priv;

    priv = self->priv;
    if (priv->radius)
        clutter_units_free (priv->radius);

    priv->radius = clutter_units_copy (radius);
}

/*
 * GObject overloading
 */

static void
castet_circle_element_get_property (GObject    *object,
                                    guint       property_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
    CastetCircleElement *self = CASTET_CIRCLE_ELEMENT (object);
    CastetCircleElementPrivate *priv = self->priv;

    switch (property_id)
    {
    case PROP_CX:
        clutter_value_set_units (value, priv->cx);
        break;
    case PROP_CY:
        clutter_value_set_units (value, priv->cy);
        break;
    case PROP_RADIUS:
        clutter_value_set_units (value, priv->radius);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
castet_circle_element_set_property (GObject      *object,
                                    guint         property_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
    CastetCircleElement *self = CASTET_CIRCLE_ELEMENT (object);

    switch (property_id)
    {
    case PROP_CX:
        castet_circle_element_set_cx (self, clutter_value_get_units (value));
        break;
    case PROP_CY:
        castet_circle_element_set_cy (self, clutter_value_get_units (value));
        break;
    case PROP_RADIUS:
        castet_circle_element_set_radius (self,
                                          clutter_value_get_units (value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
castet_circle_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_circle_element_parent_class)->dispose (object);
}

static void
castet_circle_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_circle_element_parent_class)->finalize (object);
}

static void
castet_circle_element_class_init (CastetCircleElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (CastetCircleElementPrivate));

    object_class->get_property = castet_circle_element_get_property;
    object_class->set_property = castet_circle_element_set_property;
    object_class->dispose = castet_circle_element_dispose;
    object_class->finalize = castet_circle_element_finalize;

    pspec = g_param_spec_boxed ("cx",
                                "Center x",
                                "The x coordinate of the center "
                                "of the circle",
                                CLUTTER_TYPE_UNITS,
                                CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_CX, pspec);

    pspec = g_param_spec_boxed ("cy",
                                "Center y",
                                "The y coordinate of the center "
                                "of the circle",
                                CLUTTER_TYPE_UNITS,
                                CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_CY, pspec);

    pspec = g_param_spec_boxed ("r",
                                "Radius",
                                "The radius of the circle",
                                CLUTTER_TYPE_UNITS,
                                CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_RADIUS, pspec);
}

static void
castet_circle_element_init (CastetCircleElement *self)
{
    CastetCircleElementPrivate *priv;
    ClutterUnits zero;

    self->priv = priv = CIRCLE_ELEMENT_PRIVATE (self);

    clutter_units_from_pixels (&zero, 0.0f);
    priv->cx = clutter_units_copy (&zero);
    priv->cy = clutter_units_copy (&zero);
    priv->radius = clutter_units_copy (&zero);
}

CastetDomElement *
castet_circle_element_new (void)
{
    return g_object_new (CASTET_TYPE_CIRCLE_ELEMENT, NULL);
}

ClutterUnits *
castet_circle_element_get_cx (const CastetCircleElement *circle)
{
    g_return_val_if_fail (CASTET_IS_CIRCLE_ELEMENT (circle), NULL);

    return circle->priv->cx;
}

ClutterUnits *
castet_circle_element_get_cy (const CastetCircleElement *circle)
{
    g_return_val_if_fail (CASTET_IS_CIRCLE_ELEMENT (circle), NULL);

    return circle->priv->cy;
}

ClutterUnits *
castet_circle_element_get_r (const CastetCircleElement *circle)
{
    g_return_val_if_fail (CASTET_IS_CIRCLE_ELEMENT (circle), NULL);

    return circle->priv->radius;
}
