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

#include "dax-internals.h"
#include "dax-enum-types.h"
#include "dax-private.h"
#include "dax-paramspec.h"

#include "dax-element-animation.h"

G_DEFINE_ABSTRACT_TYPE (DaxElementAnimation,
                        dax_element_animation,
                        DAX_TYPE_ELEMENT)

#define ELEMENT_ANIMATION_PRIVATE(o)                            \
    (G_TYPE_INSTANCE_GET_PRIVATE ((o),                          \
                                  DAX_TYPE_ELEMENT_ANIMATION,   \
                                  DaxElementAnimationPrivate))

enum
{
    PROP_0,

    PROP_ATTRIBUTE_TYPE,
    PROP_ATTRIBUTE_NAME,
    PROP_FROM,
    PROP_TO,
    PROP_DURATION,
    PROP_REPEAT_COUNT,
    PROP_HREF
};

struct _DaxElementAnimationPrivate
{
    DaxAnimationAttributeType attribute_type;
    gchar *attribute_name;
    gchar *from;
    gchar *to;
    DaxDuration *duration;
    DaxRepeatCount *repeat_count;
    gchar *href;
};

static void
dax_element_animation_set_duration (DaxElementAnimation *self,
                                    DaxDuration         *duration)
{
    DaxElementAnimationPrivate *priv = self->priv;

    if (priv->duration)
        dax_duration_free (priv->duration);
    priv->duration = dax_duration_copy (duration);
}

static void
dax_element_animation_set_repeat_count (DaxElementAnimation *self,
                                        DaxRepeatCount      *count)
{
    DaxElementAnimationPrivate *priv = self->priv;

    if (priv->repeat_count)
        dax_repeat_count_free (priv->repeat_count);
    priv->repeat_count = dax_repeat_count_copy (count);
}

/*
 * GObject overloading
 */

static void
dax_element_animation_get_property (GObject    *object,
                                    guint       property_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
    DaxElementAnimation *self = DAX_ELEMENT_ANIMATION (object);
    DaxElementAnimationPrivate *priv = self->priv;

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
    case PROP_HREF:
        g_value_set_string (value, priv->href);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_animation_set_property (GObject      *object,
                                    guint         property_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
    DaxElementAnimation *self = DAX_ELEMENT_ANIMATION (object);
    DaxElementAnimationPrivate *priv = self->priv;

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
        dax_element_animation_set_duration (self, g_value_get_boxed (value));
        break;
    case PROP_REPEAT_COUNT:
        dax_element_animation_set_repeat_count (self,
                                                 g_value_get_boxed (value));
        break;
    case PROP_HREF:
        g_free (priv->href);
        priv->href = g_value_dup_string (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_animation_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_element_animation_parent_class)->dispose (object);
}

static void
dax_element_animation_finalize (GObject *object)
{
    DaxElementAnimation *self = DAX_ELEMENT_ANIMATION (object);
    DaxElementAnimationPrivate *priv = self->priv;

    g_free (priv->attribute_name);
    g_free (priv->from);
    g_free (priv->to);
    dax_duration_free (priv->duration);
    g_free (priv->href);

    G_OBJECT_CLASS (dax_element_animation_parent_class)->finalize (object);
}

static void
dax_element_animation_class_init (DaxElementAnimationClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxElementAnimationPrivate));

    object_class->get_property = dax_element_animation_get_property;
    object_class->set_property = dax_element_animation_set_property;
    object_class->dispose = dax_element_animation_dispose;
    object_class->finalize = dax_element_animation_finalize;

    pspec = dax_param_spec_enum ("attributeType",
                                 "Attribute type",
                                 "The namespace in which the target "
                                 "attribute and its associated values are "
                                 "defined",
                                 DAX_TYPE_ANIMATION_ATTRIBUTE_TYPE,
                                 DAX_ANIMATION_ATTRIBUTE_TYPE_AUTO,
                                 DAX_GPARAM_READWRITE,
                                 DAX_PARAM_NONE,
                                 svg_ns);
    g_object_class_install_property (object_class, PROP_ATTRIBUTE_TYPE, pspec);

    pspec = g_param_spec_string ("attributeName",
                                 "Attribute name",
                                 "The name of the animationd attribute",
                                 NULL,
                                 DAX_GPARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_ATTRIBUTE_NAME, pspec);

    pspec = g_param_spec_string ("from",
                                 "From",
                                 "The starting value of the animation",
                                 NULL,
                                 DAX_GPARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_FROM, pspec);

    pspec = g_param_spec_string ("to",
                                 "To",
                                 "The ending value of the animation",
                                 NULL,
                                 DAX_GPARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_TO, pspec);

    pspec = g_param_spec_boxed ("dur",
                                "Duration",
                                "The simple duration",
                                DAX_TYPE_DURATION,
                                DAX_GPARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_DURATION, pspec);

    pspec = g_param_spec_boxed ("repeatCount",
                                "Repeat count",
                                "The number of iterations of the animation",
                                DAX_TYPE_REPEAT_COUNT,
                                DAX_GPARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_REPEAT_COUNT, pspec);

    pspec = dax_param_spec_string ("href",
                                   "href",
                                   "An IRI reference to the target of the "
                                   "animation",
                                   NULL,
                                   DAX_GPARAM_READWRITE,
                                   DAX_PARAM_ANIMATABLE,
                                   xlink_ns);
    g_object_class_install_property (object_class, PROP_HREF, pspec);

}

static void
dax_element_animation_init (DaxElementAnimation *self)
{
    self->priv = ELEMENT_ANIMATION_PRIVATE (self);
}

DaxDuration *
dax_element_animation_get_duration (DaxElementAnimation *self)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_ANIMATION (self), NULL);

    return self->priv->duration;
}

const gchar *
dax_element_animation_get_attribute_name (DaxElementAnimation *self)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_ANIMATION (self), NULL);

    return self->priv->attribute_name;
}

const gchar *
dax_element_animation_get_from (DaxElementAnimation *self)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_ANIMATION (self), NULL);

    return self->priv->from;
}

const gchar *
dax_element_animation_get_to (DaxElementAnimation *self)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_ANIMATION (self), NULL);

    return self->priv->to;
}

const DaxRepeatCount *
dax_element_animation_get_repeat_count (DaxElementAnimation *self)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_ANIMATION (self), NULL);

    return self->priv->repeat_count;
}

static DaxDomElement *
href_get_element (DaxDomNode  *node,
                  const gchar *href)
{
    if (href == NULL)
        return NULL;

    if (href[0] != '#')
        return NULL;

    return dax_dom_document_get_element_by_id (node->owner_document, href + 1);
}

DaxDomElement *
dax_element_animation_get_target (DaxElementAnimation *self)
{
    DaxElementAnimationPrivate *priv;
    DaxDomNode *node;
    DaxDomElement *target = NULL;

    g_return_val_if_fail (DAX_IS_ELEMENT_ANIMATION (self), NULL);
    priv = self->priv;
    node = DAX_DOM_NODE (self);

    /* SVG tiny p. 219: "If the 'xlink:href' attribute is not provided, then
     * the target element will be the immediate parent element of the current
     * animation element" */
    target = href_get_element (node, priv->href);
    if (target == NULL)
        target = DAX_DOM_ELEMENT (node->parent_node);

    return target;
}
