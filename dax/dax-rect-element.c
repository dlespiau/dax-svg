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
#include "dax-rect-element.h"

G_DEFINE_TYPE (DaxRectElement, dax_rect_element, DAX_TYPE_ELEMENT)

#define RECT_ELEMENT_PRIVATE(o)                                 \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      DAX_TYPE_RECT_ELEMENT, \
                                      DaxRectElementPrivate))

enum {
    PROP_0,

    PROP_X,
    PROP_Y,
    PROP_WIDTH,
    PROP_HEIGHT,
    PROP_RX,
    PROP_RY,
};

struct _DaxRectElementPrivate
{
    ClutterUnits *x;
    ClutterUnits *y;
    ClutterUnits *width;
    ClutterUnits *height;
    ClutterUnits *rx;
    ClutterUnits *ry;
};

void
dax_rect_element_set_x (DaxRectElement  *self,
                           const ClutterUnits *x)
{
    DaxRectElementPrivate *priv;

    g_return_if_fail (DAX_IS_RECT_ELEMENT (self));

    priv = self->priv;
    if (priv->x)
        clutter_units_free (priv->x);

    priv->x = clutter_units_copy (x);
}

void
dax_rect_element_set_y (DaxRectElement  *self,
                           const ClutterUnits *y)
{
    DaxRectElementPrivate *priv;

    g_return_if_fail (DAX_IS_RECT_ELEMENT (self));

    priv = self->priv;
    if (priv->y)
        clutter_units_free (priv->y);

    priv->y = clutter_units_copy (y);
}

void
dax_rect_element_set_width (DaxRectElement  *self,
                               const ClutterUnits *width)
{
    DaxRectElementPrivate *priv;

    g_return_if_fail (DAX_IS_RECT_ELEMENT (self));

    priv = self->priv;
    if (priv->width)
        clutter_units_free (priv->width);

    priv->width = clutter_units_copy (width);
}

void
dax_rect_element_set_height (DaxRectElement  *self,
                                const ClutterUnits *height)
{
    DaxRectElementPrivate *priv;

    g_return_if_fail (DAX_IS_RECT_ELEMENT (self));

    priv = self->priv;
    if (priv->height)
        clutter_units_free (priv->height);

    priv->height = clutter_units_copy (height);
}

void
dax_rect_element_set_rx (DaxRectElement  *self,
                            const ClutterUnits *rx)
{
    DaxRectElementPrivate *priv;

    g_return_if_fail (DAX_IS_RECT_ELEMENT (self));

    priv = self->priv;
    if (priv->rx)
        clutter_units_free (priv->rx);

    priv->rx = clutter_units_copy (rx);
}

void
dax_rect_element_set_ry (DaxRectElement  *self,
                            const ClutterUnits *ry)
{
    DaxRectElementPrivate *priv;

    g_return_if_fail (DAX_IS_RECT_ELEMENT (self));

    priv = self->priv;
    if (priv->ry)
        clutter_units_free (priv->ry);

    priv->ry = clutter_units_copy (ry);
}

/*
 * GObject overloading
 */

static void
dax_rect_element_get_property (GObject    *object,
                                  guint       property_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
    DaxRectElement *rect = DAX_RECT_ELEMENT (object);
    DaxRectElementPrivate *priv = rect->priv;

    switch (property_id)
    {
    case PROP_X:
        clutter_value_set_units (value, priv->x);
        break;
    case PROP_Y:
        clutter_value_set_units (value, priv->y);
        break;
    case PROP_WIDTH:
        clutter_value_set_units (value, priv->width);
        break;
    case PROP_HEIGHT:
        clutter_value_set_units (value, priv->height);
        break;
    case PROP_RX:
        clutter_value_set_units (value, priv->rx);
        break;
    case PROP_RY:
        clutter_value_set_units (value, priv->ry);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_rect_element_set_property (GObject      *object,
                                  guint         property_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
    DaxRectElement *rect = DAX_RECT_ELEMENT (object);

    switch (property_id)
    {
    case PROP_X:
        dax_rect_element_set_x (rect, clutter_value_get_units (value));
        break;
    case PROP_Y:
        dax_rect_element_set_y (rect, clutter_value_get_units (value));
        break;
    case PROP_WIDTH:
        dax_rect_element_set_width (rect, clutter_value_get_units (value));
        break;
    case PROP_HEIGHT:
        dax_rect_element_set_height (rect, clutter_value_get_units (value));
        break;
    case PROP_RX:
        dax_rect_element_set_rx (rect, clutter_value_get_units (value));
        break;
    case PROP_RY:
        dax_rect_element_set_ry (rect, clutter_value_get_units (value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_rect_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_rect_element_parent_class)->dispose (object);
}

static void
dax_rect_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_rect_element_parent_class)->finalize (object);
}

static void
dax_rect_element_class_init (DaxRectElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxRectElementPrivate));

    object_class->get_property = dax_rect_element_get_property;
    object_class->set_property = dax_rect_element_set_property;
    object_class->dispose = dax_rect_element_dispose;
    object_class->finalize = dax_rect_element_finalize;

    pspec = clutter_param_spec_units ("x",
                                      "x",
                                      "", /* FIXME */
                                      CLUTTER_UNIT_PIXEL,
                                      0.0f, G_MAXFLOAT,
                                      0.0f,
                                      DAX_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_X, pspec);

    pspec = clutter_param_spec_units ("y",
                                      "y",
                                      "", /* FIXME */
                                      CLUTTER_UNIT_PIXEL,
                                      0.0f, G_MAXFLOAT,
                                      0.0f,
                                      DAX_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_Y, pspec);

    pspec = clutter_param_spec_units ("width",
                                      "width",
                                      "", /* FIXME */
                                      CLUTTER_UNIT_PIXEL,
                                      0.0f, G_MAXFLOAT,
                                      0.0f,
                                      DAX_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_WIDTH, pspec);

    pspec = clutter_param_spec_units ("height",
                                      "height",
                                      "", /* FIXME */
                                      CLUTTER_UNIT_PIXEL,
                                      0.0f, G_MAXFLOAT,
                                      0.0f,
                                      DAX_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_HEIGHT, pspec);

    pspec = clutter_param_spec_units ("rx",
                                      "rx",
                                      "", /* FIXME */
                                      CLUTTER_UNIT_PIXEL,
                                      0.0f, G_MAXFLOAT,
                                      0.0f,
                                      DAX_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_RX, pspec);

    pspec = clutter_param_spec_units ("ry",
                                      "ry",
                                      "", /* FIXME */
                                      CLUTTER_UNIT_PIXEL,
                                      0.0f, G_MAXFLOAT,
                                      0.0f,
                                      DAX_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_RY, pspec);
}

static void
dax_rect_element_init (DaxRectElement *self)
{
    DaxRectElementPrivate *priv;

    self->priv = priv = RECT_ELEMENT_PRIVATE (self);

    priv->x = g_slice_new0 (ClutterUnits);
    priv->y = g_slice_new0 (ClutterUnits);
    priv->width = g_slice_new0 (ClutterUnits);
    priv->height = g_slice_new0 (ClutterUnits);
    priv->rx = g_slice_new0 (ClutterUnits);
    priv->ry = g_slice_new0 (ClutterUnits);
}

DaxDomElement *
dax_rect_element_new (void)
{
    return g_object_new (DAX_TYPE_RECT_ELEMENT, NULL);
}

gfloat
dax_rect_element_get_x_px (DaxRectElement  *self)
{
    return clutter_units_to_pixels (self->priv->x);
}

gfloat
dax_rect_element_get_y_px (DaxRectElement  *self)
{
    return clutter_units_to_pixels (self->priv->y);
}

gfloat
dax_rect_element_get_width_px (DaxRectElement  *self)
{
    return clutter_units_to_pixels (self->priv->width);
}

gfloat
dax_rect_element_get_height_px (DaxRectElement *self)
{
    return clutter_units_to_pixels (self->priv->height);
}

gfloat
dax_rect_element_set_rx_px (DaxRectElement  *self)
{
    return clutter_units_to_pixels (self->priv->rx);
}

gfloat dax_rect_element_set_ry_px (DaxRectElement  *self)
{
    return clutter_units_to_pixels (self->priv->ry);
}
