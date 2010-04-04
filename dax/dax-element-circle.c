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

#include <clutter/clutter.h>

#include "dax-internals.h"
#include "dax-private.h"
#include "dax-paramspec.h"
#include "dax-element-circle.h"

G_DEFINE_TYPE (DaxElementCircle, dax_element_circle, DAX_TYPE_ELEMENT)

#define ELEMENT_CIRCLE_PRIVATE(o)                               \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      DAX_TYPE_ELEMENT_CIRCLE,  \
                                      DaxElementCirclePrivate))

enum
{
    PROP_0,

    PROP_CX,
    PROP_CY,
    PROP_RADIUS
};

struct _DaxElementCirclePrivate
{
    ClutterUnits *cx;
    ClutterUnits *cy;
    ClutterUnits *radius;
};

static void
dax_element_circle_set_cx (DaxElementCircle   *self,
                           const ClutterUnits *cx)
{
    DaxElementCirclePrivate *priv;

    priv = self->priv;
    if (priv->cx)
        clutter_units_free (priv->cx);

    priv->cx = clutter_units_copy (cx);
}

static void
dax_element_circle_set_cy (DaxElementCircle   *self,
                           const ClutterUnits *cy)
{
    DaxElementCirclePrivate *priv;

    priv = self->priv;
    if (priv->cy)
        clutter_units_free (priv->cy);

    priv->cy = clutter_units_copy (cy);
}

static void
dax_element_circle_set_radius (DaxElementCircle   *self,
                               const ClutterUnits *radius)
{
    DaxElementCirclePrivate *priv;

    priv = self->priv;
    if (priv->radius)
        clutter_units_free (priv->radius);

    priv->radius = clutter_units_copy (radius);
}

/*
 * GObject overloading
 */

static void
dax_element_circle_get_property (GObject    *object,
                                 guint       property_id,
                                 GValue     *value,
                                 GParamSpec *pspec)
{
    DaxElementCircle *self = DAX_ELEMENT_CIRCLE (object);
    DaxElementCirclePrivate *priv = self->priv;

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
dax_element_circle_set_property (GObject      *object,
                                 guint         property_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
    DaxElementCircle *self = DAX_ELEMENT_CIRCLE (object);

    switch (property_id)
    {
    case PROP_CX:
        dax_element_circle_set_cx (self, clutter_value_get_units (value));
        break;
    case PROP_CY:
        dax_element_circle_set_cy (self, clutter_value_get_units (value));
        break;
    case PROP_RADIUS:
        dax_element_circle_set_radius (self,
                                          clutter_value_get_units (value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_circle_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_element_circle_parent_class)->dispose (object);
}

static void
dax_element_circle_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_element_circle_parent_class)->finalize (object);
}

static void
dax_element_circle_class_init (DaxElementCircleClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxElementCirclePrivate));

    object_class->get_property = dax_element_circle_get_property;
    object_class->set_property = dax_element_circle_set_property;
    object_class->dispose = dax_element_circle_dispose;
    object_class->finalize = dax_element_circle_finalize;

    pspec = dax_param_spec_boxed ("cx",
                                  "cx",
                                  "The x coordinate of the center "
                                  "of the circle",
                                  CLUTTER_TYPE_UNITS,
                                  DAX_PARAM_READWRITE,
                                  DAX_PARAM_NONE,
                                  svg_ns);
    g_object_class_install_property (object_class, PROP_CX, pspec);

    pspec = dax_param_spec_boxed ("cy",
                                  "cy",
                                  "The y coordinate of the center "
                                  "of the circle",
                                  CLUTTER_TYPE_UNITS,
                                  DAX_PARAM_READWRITE,
                                  DAX_PARAM_NONE,
                                  svg_ns);
    g_object_class_install_property (object_class, PROP_CY, pspec);

    pspec = dax_param_spec_boxed ("r",
                                  "Radius",
                                  "The radius of the circle",
                                  CLUTTER_TYPE_UNITS,
                                  DAX_PARAM_READWRITE,
                                  DAX_PARAM_NONE,
                                  svg_ns);
    g_object_class_install_property (object_class, PROP_RADIUS, pspec);
}

static void
dax_element_circle_init (DaxElementCircle *self)
{
    DaxElementCirclePrivate *priv;
    ClutterUnits zero;

    self->priv = priv = ELEMENT_CIRCLE_PRIVATE (self);

    clutter_units_from_pixels (&zero, 0.0f);
    priv->cx = clutter_units_copy (&zero);
    priv->cy = clutter_units_copy (&zero);
    priv->radius = clutter_units_copy (&zero);
}

DaxDomElement *
dax_element_circle_new (void)
{
    return g_object_new (DAX_TYPE_ELEMENT_CIRCLE, NULL);
}

ClutterUnits *
dax_element_circle_get_cx (const DaxElementCircle *circle)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_CIRCLE (circle), NULL);

    return circle->priv->cx;
}

ClutterUnits *
dax_element_circle_get_cy (const DaxElementCircle *circle)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_CIRCLE (circle), NULL);

    return circle->priv->cy;
}

ClutterUnits *
dax_element_circle_get_r (const DaxElementCircle *circle)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_CIRCLE (circle), NULL);

    return circle->priv->radius;
}
