/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009, 2010 Intel Corporation.
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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "dax-internals.h"
#include "dax-enum-types.h"
#include "dax-private.h"
#include "dax-paramspec.h"
#include "dax-animate-element.h"

G_DEFINE_TYPE (DaxAnimateElement,
               dax_animate_element,
               DAX_TYPE_ELEMENT)

#define ANIMATE_ELEMENT_PRIVATE(o)                              \
    (G_TYPE_INSTANCE_GET_PRIVATE ((o),                          \
                                  DAX_TYPE_ANIMATE_ELEMENT,  \
                                  DaxAnimateElementPrivate))

enum
{
    PROP_0,

    PROP_ATTRIBUTE_TYPE,
    PROP_ATTRIBUTE_NAME,
    PROP_FROM,
    PROP_TO,
    PROP_DURATION,
    PROP_REPEAT_COUNT
};

struct _DaxAnimateElementPrivate
{
    DaxAnimationAttributeType attribute_type;
    gchar *attribute_name;
    gchar *from;
    gchar *to;
    DaxDuration *duration;
    DaxRepeatCount *repeat_count;
};

static void
dax_animate_element_set_duration (DaxAnimateElement *self,
                                     DaxDuration       *duration)
{
    DaxAnimateElementPrivate *priv = self->priv;

    if (priv->duration)
        dax_duration_free (priv->duration);
    priv->duration = dax_duration_copy (duration);
}

static void
dax_animate_element_set_repeat_count (DaxAnimateElement *self,
                                         DaxRepeatCount    *count)
{
    DaxAnimateElementPrivate *priv = self->priv;

    if (priv->repeat_count)
        dax_repeat_count_free (priv->repeat_count);
    priv->repeat_count = dax_repeat_count_copy (count);
}

/*
 * GObject overloading
 */

static void
dax_animate_element_get_property (GObject    *object,
                                     guint       property_id,
                                     GValue     *value,
                                     GParamSpec *pspec)
{
    DaxAnimateElement *self = DAX_ANIMATE_ELEMENT (object);
    DaxAnimateElementPrivate *priv = self->priv;

    switch (property_id) {
    case PROP_ATTRIBUTE_TYPE:
        g_value_set_enum (value, priv->attribute_type);
        break;
    case PROP_ATTRIBUTE_NAME:
        g_value_set_string (value, priv->attribute_name);
        break;
    case PROP_FROM:
        g_value_set_string (value, priv->from);
        break;
    case PROP_TO:
        g_value_set_string (value, priv->to);
        break;
    case PROP_DURATION:
        g_value_set_boxed (value, priv->duration);
        break;
    case PROP_REPEAT_COUNT:
        g_value_set_boxed (value, priv->repeat_count);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_animate_element_set_property (GObject      *object,
                                     guint         property_id,
                                     const GValue *value,
                                     GParamSpec   *pspec)
{
    DaxAnimateElement *self = DAX_ANIMATE_ELEMENT (object);
    DaxAnimateElementPrivate *priv = self->priv;

    switch (property_id)
    {
    case PROP_ATTRIBUTE_TYPE:
        priv->attribute_type = g_value_get_enum (value);
        break;
    case PROP_ATTRIBUTE_NAME:
        priv->attribute_name = g_strdup (g_value_get_string (value));
        break;
    case PROP_FROM:
        priv->from = g_strdup (g_value_get_string (value));
        break;
    case PROP_TO:
        priv->to = g_strdup (g_value_get_string (value));
        break;
    case PROP_DURATION:
        dax_animate_element_set_duration (self, g_value_get_boxed (value));
        break;
    case PROP_REPEAT_COUNT:
        dax_animate_element_set_repeat_count (self,
                                                 g_value_get_boxed (value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_animate_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_animate_element_parent_class)->dispose (object);
}

static void
dax_animate_element_finalize (GObject *object)
{
    DaxAnimateElement *self = DAX_ANIMATE_ELEMENT (object);
    DaxAnimateElementPrivate *priv = self->priv;

    g_free (priv->attribute_name);
    g_free (priv->from);
    g_free (priv->to);
    dax_duration_free (priv->duration);

    G_OBJECT_CLASS (dax_animate_element_parent_class)->finalize (object);
}

static void
dax_animate_element_class_init (DaxAnimateElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxAnimateElementPrivate));

    object_class->get_property = dax_animate_element_get_property;
    object_class->set_property = dax_animate_element_set_property;
    object_class->dispose = dax_animate_element_dispose;
    object_class->finalize = dax_animate_element_finalize;

    pspec = dax_param_spec_enum ("attributeType",
                                    "Attribute type",
                                    "The namespace in which the target "
                                    "attribute and its associated values are "
                                    "defined",
                                    DAX_TYPE_ANIMATION_ATTRIBUTE_TYPE,
                                    DAX_ANIMATION_ATTRIBUTE_TYPE_AUTO,
                                    DAX_PARAM_READWRITE,
                                    DAX_PARAM_NONE,
                                    svg_ns);
    g_object_class_install_property (object_class, PROP_ATTRIBUTE_TYPE, pspec);

    pspec = g_param_spec_string ("attributeName",
                                 "Attribute name",
                                 "The name of the animated attribute",
                                 NULL,
                                 DAX_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_ATTRIBUTE_NAME, pspec);

    pspec = g_param_spec_string ("from",
                                 "From",
                                 "The starting value of the animation",
                                 NULL,
                                 DAX_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_FROM, pspec);

    pspec = g_param_spec_string ("to",
                                 "To",
                                 "The ending value of the animation",
                                 NULL,
                                 DAX_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_TO, pspec);

    pspec = g_param_spec_boxed ("dur",
                                "Duration",
                                "The simple duration",
                                DAX_TYPE_DURATION,
                                DAX_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_DURATION, pspec);

    pspec = g_param_spec_boxed ("repeatCount",
                                "Repeat count",
                                "The number of iterations of the animation",
                                DAX_TYPE_REPEAT_COUNT,
                                DAX_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_REPEAT_COUNT, pspec);
}

static void
dax_animate_element_init (DaxAnimateElement *self)
{
    self->priv = ANIMATE_ELEMENT_PRIVATE (self);
}

DaxDomElement *
dax_animate_element_new (void)
{
    return g_object_new (DAX_TYPE_ANIMATE_ELEMENT, NULL);
}

DaxDuration *
dax_animate_element_get_duration (const DaxAnimateElement *self)
{
    g_return_val_if_fail (DAX_IS_ANIMATE_ELEMENT (self), NULL);

    return self->priv->duration;
}

const gchar *
dax_animate_element_get_attribute_name (const DaxAnimateElement *self)
{
    g_return_val_if_fail (DAX_IS_ANIMATE_ELEMENT (self), NULL);

    return self->priv->attribute_name;
}

const gchar *
dax_animate_element_get_from (const DaxAnimateElement *self)
{
    g_return_val_if_fail (DAX_IS_ANIMATE_ELEMENT (self), NULL);

    return self->priv->from;
}

const gchar *
dax_animate_element_get_to (const DaxAnimateElement *self)
{
    g_return_val_if_fail (DAX_IS_ANIMATE_ELEMENT (self), NULL);

    return self->priv->to;
}

const DaxRepeatCount *
dax_animate_element_get_repeat_count (const DaxAnimateElement *self)
{
    g_return_val_if_fail (DAX_IS_ANIMATE_ELEMENT (self), NULL);

    return self->priv->repeat_count;
}

DaxDomElement *
dax_animate_element_get_target (const DaxAnimateElement *self)
{
    DaxDomNode *node;

    g_return_val_if_fail (DAX_IS_ANIMATE_ELEMENT (self), NULL);

    node = DAX_DOM_NODE (self);

    return DAX_DOM_ELEMENT (node->parent_node);
}
