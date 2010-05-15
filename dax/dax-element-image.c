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

#include "dax-dom.h"

#include "dax-cache.h"
#include "dax-paramspec.h"
#include "dax-private.h"
#include "dax-types.h"
#include "dax-utils.h"

#include "dax-element-image.h"

G_DEFINE_TYPE (DaxElementImage, dax_element_image, DAX_TYPE_ELEMENT)

#define ELEMENT_IMAGE_PRIVATE(o)                                \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      DAX_TYPE_ELEMENT_IMAGE,   \
                                      DaxElementImagePrivate))

enum
{
    PROP_0,

    PROP_X,
    PROP_Y,
    PROP_WIDTH,
    PROP_HEIGHT,
    PROP_PAR,
    PROP_HREF,
    PROP_TYPE
};

struct _DaxElementImagePrivate
{
    DaxCacheEntry *cached_file;

    /* properties */
    ClutterUnits *x;
    ClutterUnits *y;
    ClutterUnits *width;
    ClutterUnits *height;
    DaxPreserveAspectRatio *par;
    gchar *href;
    gchar *type;
};

static void
dax_element_image_set_x (DaxElementImage    *image,
                         const ClutterUnits *x)
{
    DaxElementImagePrivate *priv = image->priv;

    if (priv->x)
        clutter_units_free (priv->x);

    priv->x = clutter_units_copy (x);
}

static void
dax_element_image_set_y (DaxElementImage    *image,
                         const ClutterUnits *y)
{
    DaxElementImagePrivate *priv = image->priv;

    if (priv->y)
        clutter_units_free (priv->y);

    priv->y = clutter_units_copy (y);
}

static void
dax_element_image_set_width (DaxElementImage    *image,
                             const ClutterUnits *width)
{
    DaxElementImagePrivate *priv = image->priv;

    if (priv->width)
        clutter_units_free (priv->width);

    priv->width = clutter_units_copy (width);
}

static void
dax_element_image_set_height (DaxElementImage    *image,
                              const ClutterUnits *height)
{
    DaxElementImagePrivate *priv = image->priv;

    if (priv->height)
        clutter_units_free (priv->height);

    priv->height = clutter_units_copy (height);
}

static void
dax_element_image_set_par (DaxElementImage              *image,
                           const DaxPreserveAspectRatio *par)
{
    DaxElementImagePrivate *priv = image->priv;

    if (priv->par)
        dax_preserve_aspect_ratio_free (priv->par);

    priv->par = dax_preserve_aspect_ratio_copy (par);
}

/*
 * DaxDomElement implementation
 */

static void
on_cache_entry_ready (DaxCacheEntry   *entry,
                      GParamSpec      *pspec,
                      DaxDomElement   *element)
{
    dax_dom_element_set_loaded (element, TRUE);
}

static void
dax_element_image_parsed (DaxDomElement *element)
{
    DaxElementImage *image = (DaxElementImage *) element;
    DaxElementImagePrivate *priv = image->priv;
    DaxCache *cache;

    cache = dax_cache_get_default ();
    priv->cached_file =
        dax_cache_get_entry_for_href (cache, element, priv->href);

    if (!dax_cache_entry_is_ready (priv->cached_file)) {
        dax_dom_element_set_loaded (element, FALSE);
        g_signal_connect (priv->cached_file, "notify::ready",
                          G_CALLBACK (on_cache_entry_ready), element);
    }
}

/*
 * GObject implementation
 */

static void
dax_element_image_get_property (GObject    *object,
                                guint       property_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
    DaxElementImage *image = (DaxElementImage *) object;
    DaxElementImagePrivate *priv = image->priv;

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
    case PROP_PAR:
        g_value_set_boxed (value, priv->par);
        break;
    case PROP_HREF:
        g_value_set_string (value, priv->href);
        break;
    case PROP_TYPE:
        g_value_set_string (value, priv->type);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_image_set_property (GObject      *object,
                                guint         property_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
    DaxElementImage *image = (DaxElementImage *) object;
    DaxElementImagePrivate *priv = image->priv;

    switch (property_id)
    {
    case PROP_X:
        dax_element_image_set_x (image, clutter_value_get_units (value));
        break;
    case PROP_Y:
        dax_element_image_set_y (image, clutter_value_get_units (value));
        break;
    case PROP_WIDTH:
        dax_element_image_set_width (image, clutter_value_get_units (value));
        break;
    case PROP_HEIGHT:
        dax_element_image_set_height (image, clutter_value_get_units (value));
        break;
    case PROP_PAR:
        dax_element_image_set_par (image, g_value_get_boxed (value));
        break;
    case PROP_HREF:
        g_free (priv->href);
        priv->href = g_value_dup_string (value);
        break;
    case PROP_TYPE:
        g_free (priv->type);
        priv->type = g_value_dup_string (value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_image_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_element_image_parent_class)->dispose (object);
}

static void
dax_element_image_finalize (GObject *object)
{
    DaxElementImage *image = (DaxElementImage *) object;
    DaxElementImagePrivate *priv = image->priv;

    clutter_units_free (priv->x);
    clutter_units_free (priv->y);
    clutter_units_free (priv->width);
    clutter_units_free (priv->height);

    dax_preserve_aspect_ratio_free (priv->par);

    g_free (priv->href);
    g_free (priv->type);

    G_OBJECT_CLASS (dax_element_image_parent_class)->finalize (object);
}

static void
dax_element_image_class_init (DaxElementImageClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    DaxDomElementClass *dom_element_class = DAX_DOM_ELEMENT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxElementImagePrivate));

    object_class->get_property = dax_element_image_get_property;
    object_class->set_property = dax_element_image_set_property;
    object_class->dispose = dax_element_image_dispose;
    object_class->finalize = dax_element_image_finalize;

    dom_element_class->parsed = dax_element_image_parsed;

    pspec = dax_param_spec_boxed ("x",
                                  "x",
                                  "The x-axis coordinate of one corner of the "
                                  "rectangular region",
                                  CLUTTER_TYPE_UNITS,
                                  DAX_GPARAM_READWRITE,
                                  DAX_PARAM_ANIMATABLE,
                                  svg_ns);
    g_object_class_install_property (object_class, PROP_X, pspec);

    pspec = dax_param_spec_boxed ("y",
                                  "y",
                                  "The y-axis coordinate of one corner of the "
                                  "rectangular region",
                                  CLUTTER_TYPE_UNITS,
                                  DAX_GPARAM_READWRITE,
                                  DAX_PARAM_ANIMATABLE,
                                  svg_ns);
    g_object_class_install_property (object_class, PROP_Y, pspec);

    pspec = dax_param_spec_boxed ("width",
                                  "width",
                                  "The width of the rectangular region",
                                  CLUTTER_TYPE_UNITS,
                                  DAX_GPARAM_READWRITE,
                                  DAX_PARAM_ANIMATABLE,
                                  svg_ns);
    g_object_class_install_property (object_class, PROP_WIDTH, pspec);

    pspec = dax_param_spec_boxed ("height",
                                  "height",
                                  "The height of the rectangular region",
                                  CLUTTER_TYPE_UNITS,
                                  DAX_GPARAM_READWRITE,
                                  DAX_PARAM_ANIMATABLE,
                                  svg_ns);
    g_object_class_install_property (object_class, PROP_HEIGHT, pspec);

    pspec = dax_param_spec_boxed ("preserveAspectRatio",
                                  "preserveAspectRatio",
                                  "Whether or not to force uniform scaling",
                                  DAX_TYPE_PRESERVE_ASPECT_RATIO,
                                  DAX_GPARAM_READWRITE,
                                  DAX_PARAM_ANIMATABLE,
                                  svg_ns);
    g_object_class_install_property (object_class, PROP_PAR, pspec);

    pspec = dax_param_spec_string ("href",
                                   "href",
                                   "An IRI reference to the image",
                                   NULL,
                                   DAX_GPARAM_READWRITE,
                                   DAX_PARAM_ANIMATABLE,
                                   xlink_ns);
    g_object_class_install_property (object_class, PROP_HREF, pspec);

    pspec = dax_param_spec_string ("type",
                                   "type",
                                   "A hint about the expected Internet Media "
                                   "Type of the raster image",
                                   NULL,
                                   DAX_GPARAM_READWRITE,
                                   DAX_PARAM_ANIMATABLE,
                                   svg_ns);
    g_object_class_install_property (object_class, PROP_TYPE, pspec);
}

static void
dax_element_image_init (DaxElementImage *self)
{
    DaxElementImagePrivate *priv;
    DaxPreserveAspectRatio par;
    ClutterUnits zero;

    self->priv = priv = ELEMENT_IMAGE_PRIVATE (self);

    clutter_units_from_pixels (&zero, 0.0f);
    priv->x = clutter_units_copy (&zero);
    priv->y = clutter_units_copy (&zero);
    priv->width = clutter_units_copy (&zero);
    priv->height = clutter_units_copy (&zero);

    dax_preserve_aspect_ratio_set_default (&par);
    priv->par = dax_preserve_aspect_ratio_copy (&par);
}

DaxDomElement *
dax_element_image_new (void)
{
    return g_object_new (DAX_TYPE_ELEMENT_IMAGE, NULL);
}

/*
 * Accessors
 */

ClutterUnits *
dax_element_image_get_x (const DaxElementImage *image)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_IMAGE (image), NULL);

    return image->priv->x;
}

ClutterUnits *
dax_element_image_get_y (const DaxElementImage *image)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_IMAGE (image), NULL);

    return image->priv->y;
}

ClutterUnits *
dax_element_image_get_width (const DaxElementImage *image)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_IMAGE (image), NULL);

    return image->priv->width;
}

ClutterUnits *
dax_element_image_get_height (const DaxElementImage *image)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_IMAGE (image), NULL);

    return image->priv->height;
}

const DaxCacheEntry *
dax_element_image_get_cache_entry (const DaxElementImage *image)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_IMAGE (image), NULL);

    return image->priv->cached_file;
}
