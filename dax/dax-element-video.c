/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
 *
 * Authored by: Damien Lespiau <damien.lespiau@intel.com>
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

#include "dax-paramspec.h"
#include "dax-private.h"
#include "dax-types.h"
#include "dax-utils.h"

#include "dax-element-video.h"

G_DEFINE_TYPE (DaxElementVideo, dax_element_video, DAX_TYPE_ELEMENT)

#define ELEMENT_VIDEO_PRIVATE(o)                                \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      DAX_TYPE_ELEMENT_VIDEO,   \
                                      DaxElementVideoPrivate))

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

struct _DaxElementVideoPrivate
{
    /* properties */
    ClutterUnits *x;
    ClutterUnits *y;
    ClutterUnits *width;
    ClutterUnits *height;
    DaxPreserveAspectRatio *par;
    gchar *href;
    gchar *type;

    gchar *uri;
};

static void
dax_element_video_set_x (DaxElementVideo    *video,
                         const ClutterUnits *x)
{
    DaxElementVideoPrivate *priv = video->priv;

    if (priv->x)
        clutter_units_free (priv->x);

    priv->x = clutter_units_copy (x);
}

static void
dax_element_video_set_y (DaxElementVideo    *video,
                         const ClutterUnits *y)
{
    DaxElementVideoPrivate *priv = video->priv;

    if (priv->y)
        clutter_units_free (priv->y);

    priv->y = clutter_units_copy (y);
}

static void
dax_element_video_set_width (DaxElementVideo    *video,
                             const ClutterUnits *width)
{
    DaxElementVideoPrivate *priv = video->priv;

    if (priv->width)
        clutter_units_free (priv->width);

    priv->width = clutter_units_copy (width);
}

static void
dax_element_video_set_height (DaxElementVideo    *video,
                              const ClutterUnits *height)
{
    DaxElementVideoPrivate *priv = video->priv;

    if (priv->height)
        clutter_units_free (priv->height);

    priv->height = clutter_units_copy (height);
}

static void
dax_element_video_set_par (DaxElementVideo              *video,
                           const DaxPreserveAspectRatio *par)
{
    DaxElementVideoPrivate *priv = video->priv;

    if (priv->par)
        dax_preserve_aspect_ratio_free (priv->par);

    priv->par = dax_preserve_aspect_ratio_copy (par);
}

/*
 * GObject implementation
 */

static void
dax_element_video_get_property (GObject    *object,
                                guint       property_id,
                                GValue     *value,
                                GParamSpec *pspec)
{
    DaxElementVideo *video = (DaxElementVideo *) object;
    DaxElementVideoPrivate *priv = video->priv;

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
dax_element_video_set_property (GObject      *object,
                                guint         property_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
    DaxElementVideo *video = (DaxElementVideo *) object;
    DaxElementVideoPrivate *priv = video->priv;

    switch (property_id)
    {
    case PROP_X:
        dax_element_video_set_x (video, clutter_value_get_units (value));
        break;
    case PROP_Y:
        dax_element_video_set_y (video, clutter_value_get_units (value));
        break;
    case PROP_WIDTH:
        dax_element_video_set_width (video, clutter_value_get_units (value));
        break;
    case PROP_HEIGHT:
        dax_element_video_set_height (video, clutter_value_get_units (value));
        break;
    case PROP_PAR:
        dax_element_video_set_par (video, g_value_get_boxed (value));
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
dax_element_video_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_element_video_parent_class)->dispose (object);
}

static void
dax_element_video_finalize (GObject *object)
{
    DaxElementVideo *video = (DaxElementVideo *) object;
    DaxElementVideoPrivate *priv = video->priv;

    clutter_units_free (priv->x);
    clutter_units_free (priv->y);
    clutter_units_free (priv->width);
    clutter_units_free (priv->height);

    dax_preserve_aspect_ratio_free (priv->par);

    g_free (priv->href);
    g_free (priv->type);
    g_free (priv->uri);

    G_OBJECT_CLASS (dax_element_video_parent_class)->finalize (object);
}

static void
dax_element_video_class_init (DaxElementVideoClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxElementVideoPrivate));

    object_class->get_property = dax_element_video_get_property;
    object_class->set_property = dax_element_video_set_property;
    object_class->dispose = dax_element_video_dispose;
    object_class->finalize = dax_element_video_finalize;

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
                                   "An IRI reference to the video content",
                                   NULL,
                                   DAX_GPARAM_READWRITE,
                                   DAX_PARAM_ANIMATABLE,
                                   xlink_ns);
    g_object_class_install_property (object_class, PROP_HREF, pspec);

    pspec = dax_param_spec_string ("type",
                                   "type",
                                   "The video format",
                                   NULL,
                                   DAX_GPARAM_READWRITE,
                                   DAX_PARAM_ANIMATABLE,
                                   svg_ns);
    g_object_class_install_property (object_class, PROP_TYPE, pspec);
}

static void
dax_element_video_init (DaxElementVideo *self)
{
    DaxElementVideoPrivate *priv;
    DaxPreserveAspectRatio par;
    ClutterUnits zero;

    self->priv = priv = ELEMENT_VIDEO_PRIVATE (self);

    clutter_units_from_pixels (&zero, 0.0f);
    priv->x = clutter_units_copy (&zero);
    priv->y = clutter_units_copy (&zero);
    priv->width = clutter_units_copy (&zero);
    priv->height = clutter_units_copy (&zero);

    dax_preserve_aspect_ratio_set_default (&par);
    priv->par = dax_preserve_aspect_ratio_copy (&par);
}

DaxDomElement *
dax_element_video_new (void)
{
    return g_object_new (DAX_TYPE_ELEMENT_VIDEO, NULL);
}

/*
 * Accessors
 */

ClutterUnits *
dax_element_video_get_x (DaxElementVideo *video)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_VIDEO (video), NULL);

    return video->priv->x;
}

ClutterUnits *
dax_element_video_get_y (DaxElementVideo *video)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_VIDEO (video), NULL);

    return video->priv->y;
}

ClutterUnits *
dax_element_video_get_width (DaxElementVideo *video)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_VIDEO (video), NULL);

    return video->priv->width;
}

ClutterUnits *
dax_element_video_get_height (DaxElementVideo *video)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_VIDEO (video), NULL);

    return video->priv->height;
}

const gchar *
dax_element_video_get_uri (DaxElementVideo *video)
{
    DaxElementVideoPrivate *priv;

    g_return_val_if_fail (DAX_IS_ELEMENT_VIDEO (video), NULL);
    priv = video->priv;

    if (priv->uri)
        return priv->uri;

    priv->uri = dax_dom_utils_get_uri_for_href (DAX_DOM_ELEMENT (video),
                                                priv->href);
    return priv->uri;
}
