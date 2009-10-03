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
#include "castet-rect-element.h"

G_DEFINE_TYPE (CastetRectElement, castet_rect_element, CASTET_TYPE_ELEMENT)

#define RECT_ELEMENT_PRIVATE(o)                                 \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      CASTET_TYPE_RECT_ELEMENT, \
                                      CastetRectElementPrivate))

enum {
    PROP_0,

    PROP_X,
    PROP_Y,
    PROP_WIDTH,
    PROP_HEIGHT,
    PROP_RX,
    PROP_RY,
};

struct _CastetRectElementPrivate
{
    ClutterUnits *x;
    ClutterUnits *y;
    ClutterUnits *width;
    ClutterUnits *height;
    ClutterUnits *rx;
    ClutterUnits *ry;
};

void
castet_rect_element_set_x (CastetRectElement  *self,
                           const ClutterUnits *x)
{
    CastetRectElementPrivate *priv;

    g_return_if_fail (CASTET_IS_RECT_ELEMENT (self));

    priv = self->priv;
    if (priv->x)
        clutter_units_free (priv->x);

    priv->x = clutter_units_copy (x);
}

void
castet_rect_element_set_y (CastetRectElement  *self,
                           const ClutterUnits *y)
{
    CastetRectElementPrivate *priv;

    g_return_if_fail (CASTET_IS_RECT_ELEMENT (self));

    priv = self->priv;
    if (priv->y)
        clutter_units_free (priv->y);

    priv->y = clutter_units_copy (y);
}

void
castet_rect_element_set_width (CastetRectElement  *self,
                               const ClutterUnits *width)
{
    CastetRectElementPrivate *priv;

    g_return_if_fail (CASTET_IS_RECT_ELEMENT (self));

    priv = self->priv;
    if (priv->width)
        clutter_units_free (priv->width);

    priv->width = clutter_units_copy (width);
}

void
castet_rect_element_set_height (CastetRectElement  *self,
                                const ClutterUnits *height)
{
    CastetRectElementPrivate *priv;

    g_return_if_fail (CASTET_IS_RECT_ELEMENT (self));

    priv = self->priv;
    if (priv->height)
        clutter_units_free (priv->height);

    priv->height = clutter_units_copy (height);
}

void
castet_rect_element_set_rx (CastetRectElement  *self,
                            const ClutterUnits *rx)
{
    CastetRectElementPrivate *priv;

    g_return_if_fail (CASTET_IS_RECT_ELEMENT (self));

    priv = self->priv;
    if (priv->rx)
        clutter_units_free (priv->rx);

    priv->rx = clutter_units_copy (rx);
}

void
castet_rect_element_set_ry (CastetRectElement  *self,
                            const ClutterUnits *ry)
{
    CastetRectElementPrivate *priv;

    g_return_if_fail (CASTET_IS_RECT_ELEMENT (self));

    priv = self->priv;
    if (priv->ry)
        clutter_units_free (priv->ry);

    priv->ry = clutter_units_copy (ry);
}

/*
 * GObject overloading
 */

static void
castet_rect_element_get_property (GObject    *object,
                                  guint       property_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
    CastetRectElement *rect = CASTET_RECT_ELEMENT (object);
    CastetRectElementPrivate *priv = rect->priv;

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
castet_rect_element_set_property (GObject      *object,
                                  guint         property_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
    CastetRectElement *rect = CASTET_RECT_ELEMENT (object);

    switch (property_id)
    {
    case PROP_X:
        castet_rect_element_set_x (rect, clutter_value_get_units (value));
        break;
    case PROP_Y:
        castet_rect_element_set_y (rect, clutter_value_get_units (value));
        break;
    case PROP_WIDTH:
        castet_rect_element_set_width (rect, clutter_value_get_units (value));
        break;
    case PROP_HEIGHT:
        castet_rect_element_set_height (rect, clutter_value_get_units (value));
        break;
    case PROP_RX:
        castet_rect_element_set_rx (rect, clutter_value_get_units (value));
        break;
    case PROP_RY:
        castet_rect_element_set_ry (rect, clutter_value_get_units (value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
castet_rect_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_rect_element_parent_class)->dispose (object);
}

static void
castet_rect_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_rect_element_parent_class)->finalize (object);
}

static void
castet_rect_element_class_init (CastetRectElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (CastetRectElementPrivate));

    object_class->get_property = castet_rect_element_get_property;
    object_class->set_property = castet_rect_element_set_property;
    object_class->dispose = castet_rect_element_dispose;
    object_class->finalize = castet_rect_element_finalize;

    pspec = clutter_param_spec_units ("x",
                                      "x",
                                      "", /* FIXME */
                                      CLUTTER_UNIT_PIXEL,
                                      0.0f, G_MAXFLOAT,
                                      0.0f,
                                      CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_X, pspec);

    pspec = clutter_param_spec_units ("y",
                                      "y",
                                      "", /* FIXME */
                                      CLUTTER_UNIT_PIXEL,
                                      0.0f, G_MAXFLOAT,
                                      0.0f,
                                      CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_Y, pspec);

    pspec = clutter_param_spec_units ("width",
                                      "width",
                                      "", /* FIXME */
                                      CLUTTER_UNIT_PIXEL,
                                      0.0f, G_MAXFLOAT,
                                      0.0f,
                                      CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_WIDTH, pspec);

    pspec = clutter_param_spec_units ("height",
                                      "height",
                                      "", /* FIXME */
                                      CLUTTER_UNIT_PIXEL,
                                      0.0f, G_MAXFLOAT,
                                      0.0f,
                                      CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_HEIGHT, pspec);

    pspec = clutter_param_spec_units ("rx",
                                      "rx",
                                      "", /* FIXME */
                                      CLUTTER_UNIT_PIXEL,
                                      0.0f, G_MAXFLOAT,
                                      0.0f,
                                      CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_RX, pspec);

    pspec = clutter_param_spec_units ("ry",
                                      "ry",
                                      "", /* FIXME */
                                      CLUTTER_UNIT_PIXEL,
                                      0.0f, G_MAXFLOAT,
                                      0.0f,
                                      CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_RY, pspec);
}

static void
castet_rect_element_init (CastetRectElement *self)
{
    CastetRectElementPrivate *priv;

    self->priv = priv = RECT_ELEMENT_PRIVATE (self);

    priv->x = g_slice_new0 (ClutterUnits);
    priv->y = g_slice_new0 (ClutterUnits);
    priv->width = g_slice_new0 (ClutterUnits);
    priv->height = g_slice_new0 (ClutterUnits);
    priv->rx = g_slice_new0 (ClutterUnits);
    priv->ry = g_slice_new0 (ClutterUnits);
}

CastetDomElement *
castet_rect_element_new (void)
{
    return g_object_new (CASTET_TYPE_RECT_ELEMENT, NULL);
}
