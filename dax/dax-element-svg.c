/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009, 2010 Intel Corporation.
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
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "dax-internals.h"
#include "dax-private.h"
#include "dax-enum-types.h"
#include "dax-paramspec.h"
#include "dax-element-svg.h"

G_DEFINE_TYPE (DaxElementSvg, dax_element_svg, DAX_TYPE_ELEMENT)

#define ELEMENT_SVG_PRIVATE(o)                              \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      DAX_TYPE_ELEMENT_SVG, \
                                      DaxElementSvgPrivate))

enum
{
    PROP_0,

    PROP_VERSION,
    PROP_BASE_PROFILE,
    PROP_WIDTH,
    PROP_HEIGHT,
    PROP_VIEW_BOX,
};

struct _DaxElementSvgPrivate
{
    DaxSvgVersion version;
    DaxSvgBaseProfile base_profile;
    ClutterUnits *width;
    ClutterUnits *height;
    GArray *view_box;
};

static void
dax_element_svg_set_width (DaxElementSvg      *self,
                           const ClutterUnits *width)
{
    DaxElementSvgPrivate *priv = self->priv;

    if (priv->width)
        clutter_units_free (priv->width);

    priv->width = clutter_units_copy (width);
}

static void
dax_element_svg_set_height (DaxElementSvg      *self,
                            const ClutterUnits *height)
{
    DaxElementSvgPrivate *priv = self->priv;

    if (priv->height)
        clutter_units_free (priv->height);

    priv->height = clutter_units_copy (height);
}

/*
 * GObject overloading
 */

static void
dax_element_svg_get_property (GObject    *object,
                              guint       property_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
    DaxElementSvg *self = DAX_ELEMENT_SVG (object);
    DaxElementSvgPrivate *priv = self->priv;

    switch (property_id)
    {
    case PROP_VERSION:
        g_value_set_enum (value, priv->version);
        break;
    case PROP_BASE_PROFILE:
        g_value_set_enum (value, priv->base_profile);
        break;
    case PROP_WIDTH:
        clutter_value_set_units (value, priv->width);
        break;
    case PROP_HEIGHT:
        clutter_value_set_units (value, priv->height);
        break;
    case PROP_VIEW_BOX:
        g_value_set_boxed (value, priv->view_box);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_svg_set_property (GObject      *object,
                              guint         property_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
    DaxElementSvg *self = DAX_ELEMENT_SVG (object);
    DaxElementSvgPrivate *priv = self->priv;

    switch (property_id)
    {
    case PROP_VERSION:
        priv->version = g_value_get_enum (value);
        break;
    case PROP_BASE_PROFILE:
        priv->base_profile = g_value_get_enum (value);
        break;
    case PROP_WIDTH:
        dax_element_svg_set_width (self, clutter_value_get_units (value));
        break;
    case PROP_HEIGHT:
        dax_element_svg_set_height (self, clutter_value_get_units (value));
        break;
    case PROP_VIEW_BOX:
        if (priv->view_box)
            g_array_free (priv->view_box, TRUE);
        priv->view_box = g_value_get_boxed (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_svg_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_element_svg_parent_class)->dispose (object);
}

static void
dax_element_svg_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_element_svg_parent_class)->finalize (object);
}

static void
dax_element_svg_class_init (DaxElementSvgClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxElementSvgPrivate));

    object_class->get_property = dax_element_svg_get_property;
    object_class->set_property = dax_element_svg_set_property;
    object_class->dispose = dax_element_svg_dispose;
    object_class->finalize = dax_element_svg_finalize;

    pspec = dax_param_spec_enum ("version",
                                 "Version",
                                 "The SVG language version to which this "
                                 "document fragment conforms",
                                 DAX_TYPE_SVG_VERSION,
                                 DAX_SVG_VERSION_1_2,
                                 DAX_GPARAM_READWRITE,
                                 DAX_PARAM_NONE,
                                 svg_ns);
    g_object_class_install_property (object_class, PROP_VERSION, pspec);

    pspec = dax_param_spec_enum ("baseProfile",
                                 "Base profile",
                                 "The minimum SVG language profile believed "
                                 "necessary to correctly render the content",
                                 DAX_TYPE_SVG_BASE_PROFILE,
                                 DAX_SVG_BASE_PROFILE_DEFAULT,
                                 DAX_GPARAM_READWRITE,
                                 DAX_PARAM_NONE,
                                 svg_ns);
    g_object_class_install_property (object_class, PROP_BASE_PROFILE, pspec);

    pspec = g_param_spec_boxed ("width",
                                "Width",
                                "The intrinsic width of the SVG document "
                                "fragment",
                                CLUTTER_TYPE_UNITS,
                                DAX_GPARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_WIDTH, pspec);

    pspec = g_param_spec_boxed ("height",
                                "Height",
                                "The intrinsic height of the SVG "
                                "document fragment",
                                CLUTTER_TYPE_UNITS,
                                DAX_GPARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_HEIGHT, pspec);

    pspec = dax_param_spec_array ("viewBox",
                                  "viewBox",
                                  "Rectangular region into which content must "
                                  "be fit",
                                  G_TYPE_FLOAT,
                                  4,
                                  DAX_GPARAM_READWRITE,
                                  DAX_PARAM_NONE,
                                  svg_ns);
    g_object_class_install_property (object_class, PROP_VIEW_BOX, pspec);
}

static void
dax_element_svg_init (DaxElementSvg *self)
{
    self->priv = ELEMENT_SVG_PRIVATE (self);
}

DaxDomElement *
dax_element_svg_new (void)
{
    return g_object_new (DAX_TYPE_ELEMENT_SVG, NULL);
}

ClutterUnits *
dax_element_svg_get_width (DaxElementSvg *svg)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_SVG (svg), NULL);

    return svg->priv->width;
}

ClutterUnits *
dax_element_svg_get_height (DaxElementSvg *svg)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_SVG (svg), NULL);

    return svg->priv->height;
}
