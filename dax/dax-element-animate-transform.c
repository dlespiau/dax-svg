/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
 *
 * Authored by: Øyvind Kolås <pippin@linux.intel.com>
 *              Damien Lespiau <damien.lespiau@intel.com>
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

#include "dax-enum-types.h"
#include "dax-paramspec.h"
#include "dax-private.h"
#include "dax-types.h"

#include "dax-element-animate-transform.h"

G_DEFINE_TYPE (DaxElementAnimateTransform,
               dax_element_animate_transform,
               DAX_TYPE_ELEMENT_ANIMATION)

#define ELEMENT_ANIMATE_TRANSFORM_PRIVATE(o)                            \
    (G_TYPE_INSTANCE_GET_PRIVATE ((o),                                  \
                                  DAX_TYPE_ELEMENT_ANIMATE_TRANSFORM,   \
                                  DaxElementAnimateTransformPrivate))

enum
{
    PROP_0,

    PROP_TYPE
};

struct _DaxElementAnimateTransformPrivate
{
    DaxAnimateTransformType type;
};

/*
 * GObject implementation
 */

static void
dax_element_animate_transform_get_property (GObject    *object,
                                            guint       property_id,
                                            GValue     *value,
                                            GParamSpec *pspec)
{
    DaxElementAnimateTransform *transform;
    DaxElementAnimateTransformPrivate *priv;
    
    transform = DAX_ELEMENT_ANIMATE_TRANSFORM (object);
    priv = transform->priv;

    switch (property_id)
    {
    case PROP_TYPE:
        g_value_set_enum (value, priv->type);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_animate_transform_set_property (GObject      *object,
                                            guint         property_id,
                                            const GValue *value,
                                            GParamSpec   *pspec)
{
    DaxElementAnimateTransform *transform;
    DaxElementAnimateTransformPrivate *priv;
    
    transform = DAX_ELEMENT_ANIMATE_TRANSFORM (object);
    priv = transform->priv;

    switch (property_id)
    {
    case PROP_TYPE:
        priv->type = g_value_get_enum (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_animate_transform_dispose (GObject *object)
{
    GObjectClass *object_class;

    object_class = G_OBJECT_CLASS (dax_element_animate_transform_parent_class);
    object_class->dispose (object);
}

static void
dax_element_animate_transform_finalize (GObject *object)
{
    GObjectClass *object_class;

    object_class = G_OBJECT_CLASS (dax_element_animate_transform_parent_class);
    object_class->finalize (object);
}

static void
dax_element_animate_transform_class_init (DaxElementAnimateTransformClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass,
                              sizeof (DaxElementAnimateTransformPrivate));

    object_class->get_property = dax_element_animate_transform_get_property;
    object_class->set_property = dax_element_animate_transform_set_property;
    object_class->dispose = dax_element_animate_transform_dispose;
    object_class->finalize = dax_element_animate_transform_finalize;

    pspec = dax_param_spec_enum ("type",
                                 "Type",
                                 "Type of transformation",
                                 DAX_TYPE_ANIMATE_TRANSFORM_TYPE,
                                 DAX_ANIMATE_TRANSFORM_TYPE_DEFAULT,
                                 DAX_GPARAM_READWRITE,
                                 DAX_PARAM_NONE,
                                 svg_ns);
    g_object_class_install_property (object_class, PROP_TYPE, pspec);
}

static void
dax_element_animate_transform_init (DaxElementAnimateTransform *self)
{
    self->priv = ELEMENT_ANIMATE_TRANSFORM_PRIVATE (self);
}

DaxDomElement *
dax_element_animate_transform_new (void)
{
    return g_object_new (DAX_TYPE_ELEMENT_ANIMATE_TRANSFORM, NULL);
}

DaxAnimateTransformType
dax_element_animate_transform_get_matrix_type (DaxElementAnimateTransform *at)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_ANIMATE_TRANSFORM (at),
                          DAX_ANIMATE_TRANSFORM_TYPE_DEFAULT);

    return at->priv->type;
}
