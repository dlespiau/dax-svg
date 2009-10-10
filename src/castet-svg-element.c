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
#include "castet-enum-types.h"
#include "castet-svg-element.h"

G_DEFINE_TYPE (CastetSvgElement, castet_svg_element, CASTET_TYPE_ELEMENT)

#define SVG_ELEMENT_PRIVATE(o)                                  \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      CASTET_TYPE_SVG_ELEMENT,  \
                                      CastetSvgElementPrivate))

enum
{
    PROP_0,

    PROP_VERSION,
    PROP_BASE_PROFILE,
    PROP_WIDTH,
    PROP_HEIGHT,
};

struct _CastetSvgElementPrivate
{
    CastetSvgVersion version;
    CastetSvgBaseProfile base_profile;
    ClutterUnits *width;
    ClutterUnits *height;
};

static void
castet_svg_element_set_width (CastetSvgElement   *self,
                              const ClutterUnits *width)
{
    CastetSvgElementPrivate *priv = self->priv;

    if (priv->width)
        clutter_units_free (priv->width);

    priv->width = clutter_units_copy (width);
}

static void
castet_svg_element_set_height (CastetSvgElement   *self,
                               const ClutterUnits *height)
{
    CastetSvgElementPrivate *priv = self->priv;

    if (priv->height)
        clutter_units_free (priv->height);

    priv->height = clutter_units_copy (height);
}

/*
 * GObject overloading
 */

static void
castet_svg_element_get_property (GObject    *object,
                                 guint       property_id,
                                 GValue     *value,
                                 GParamSpec *pspec)
{
    CastetSvgElement *self = CASTET_SVG_ELEMENT (object);
    CastetSvgElementPrivate *priv = self->priv;

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
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
castet_svg_element_set_property (GObject      *object,
                                 guint         property_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
    CastetSvgElement *self = CASTET_SVG_ELEMENT (object);
    CastetSvgElementPrivate *priv = self->priv;

    switch (property_id)
    {
    case PROP_VERSION:
        priv->version = g_value_get_enum (value);
        break;
    case PROP_BASE_PROFILE:
        priv->base_profile = g_value_get_enum (value);
        break;
    case PROP_WIDTH:
        castet_svg_element_set_width (self, clutter_value_get_units (value));
        break;
    case PROP_HEIGHT:
        castet_svg_element_set_height (self, clutter_value_get_units (value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
castet_svg_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_svg_element_parent_class)->dispose (object);
}

static void
castet_svg_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_svg_element_parent_class)->finalize (object);
}

static void
castet_svg_element_class_init (CastetSvgElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (CastetSvgElementPrivate));

    object_class->get_property = castet_svg_element_get_property;
    object_class->set_property = castet_svg_element_set_property;
    object_class->dispose = castet_svg_element_dispose;
    object_class->finalize = castet_svg_element_finalize;

    pspec = g_param_spec_enum ("version",
                               "Version",
                               "The SVG language version to which this "
                               "document fragment conforms",
                               CASTET_TYPE_SVG_VERSION,
                               CASTET_SVG_VERSION_DEFAULT,
                               CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_VERSION, pspec);

    pspec = g_param_spec_enum ("baseProfile",
                               "Base profile",
                               "The minimum SVG language profile believed "
                               "necessary to correctly render the content",
                               CASTET_TYPE_SVG_BASE_PROFILE,
                               CASTET_SVG_BASE_PROFILE_DEFAULT,
                               CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_BASE_PROFILE, pspec);

    pspec = g_param_spec_boxed ("width",
                                "Width",
                                "The intrinsic width of the SVG document "
                                "fragment",
                                CLUTTER_TYPE_UNITS,
                                CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_WIDTH, pspec);

    pspec = g_param_spec_boxed ("height",
                                "Height",
                                "The intrinsic height of the SVG "
                                "document fragment",
                                CLUTTER_TYPE_UNITS,
                                CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_HEIGHT, pspec);
}

static void
castet_svg_element_init (CastetSvgElement *self)
{
    self->priv = SVG_ELEMENT_PRIVATE (self);
}

CastetDomElement *
castet_svg_element_new (void)
{
    return g_object_new (CASTET_TYPE_SVG_ELEMENT, NULL);
}
