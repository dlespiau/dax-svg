/*
 * Castet - Load and draw SVG
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

#include "castet-internals.h"
#include "castet-enum-types.h"
#include "castet-private.h"
#include "castet-animate-element.h"

G_DEFINE_TYPE (CastetAnimateElement,
               castet_animate_element,
               CASTET_TYPE_ELEMENT)

#define ANIMATE_ELEMENT_PRIVATE(o)                              \
    (G_TYPE_INSTANCE_GET_PRIVATE ((o),                          \
                                  CASTET_TYPE_ANIMATE_ELEMENT,  \
                                  CastetAnimateElementPrivate))

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

struct _CastetAnimateElementPrivate
{
    CastetAnimationAttributeType attribute_type;
    gchar *attribute_name;
    gchar *from;
    gchar *to;
    CastetDuration *duration;
    CastetRepeatCount *repeat_count;
};

static void
castet_animate_element_set_duration (CastetAnimateElement *self,
                                     CastetDuration       *duration)
{
    CastetAnimateElementPrivate *priv = self->priv;

    if (priv->duration)
        castet_duration_free (priv->duration);
    priv->duration = castet_duration_copy (duration);
}

static void
castet_animate_element_set_repeat_count (CastetAnimateElement *self,
                                         CastetRepeatCount    *count)
{
    CastetAnimateElementPrivate *priv = self->priv;

    if (priv->repeat_count)
        castet_repeat_count_free (priv->repeat_count);
    priv->repeat_count = castet_repeat_count_copy (count);
}

/*
 * GObject overloading
 */

static void
castet_animate_element_get_property (GObject    *object,
                                     guint       property_id,
                                     GValue     *value,
                                     GParamSpec *pspec)
{
    CastetAnimateElement *self = CASTET_ANIMATE_ELEMENT (object);
    CastetAnimateElementPrivate *priv = self->priv;

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
castet_animate_element_set_property (GObject      *object,
                                     guint         property_id,
                                     const GValue *value,
                                     GParamSpec   *pspec)
{
    CastetAnimateElement *self = CASTET_ANIMATE_ELEMENT (object);
    CastetAnimateElementPrivate *priv = self->priv;

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
        castet_animate_element_set_duration (self, g_value_get_boxed (value));
        break;
    case PROP_REPEAT_COUNT:
        castet_animate_element_set_repeat_count (self,
                                                 g_value_get_boxed (value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
castet_animate_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_animate_element_parent_class)->dispose (object);
}

static void
castet_animate_element_finalize (GObject *object)
{
    CastetAnimateElement *self = CASTET_ANIMATE_ELEMENT (object);
    CastetAnimateElementPrivate *priv = self->priv;

    g_free (priv->attribute_name);
    g_free (priv->from);
    g_free (priv->to);
    castet_duration_free (priv->duration);

    G_OBJECT_CLASS (castet_animate_element_parent_class)->finalize (object);
}

static void
castet_animate_element_class_init (CastetAnimateElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (CastetAnimateElementPrivate));

    object_class->get_property = castet_animate_element_get_property;
    object_class->set_property = castet_animate_element_set_property;
    object_class->dispose = castet_animate_element_dispose;
    object_class->finalize = castet_animate_element_finalize;

    pspec = castet_param_spec_enum ("attributeType",
                                    "Attribute type",
                                    "The namespace in which the target "
                                    "attribute and its associated values are "
                                    "defined",
                                    CASTET_TYPE_ANIMATION_ATTRIBUTE_TYPE,
                                    CASTET_ANIMATION_ATTRIBUTE_TYPE_AUTO,
                                    CASTET_PARAM_READWRITE,
                                    CASTET_PARAM_NONE,
                                    svg_ns);
    g_object_class_install_property (object_class, PROP_ATTRIBUTE_TYPE, pspec);

    pspec = g_param_spec_string ("attributeName",
                                 "Attribute name",
                                 "The name of the animated attribute",
                                 NULL,
                                 CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_ATTRIBUTE_NAME, pspec);

    pspec = g_param_spec_string ("from",
                                 "From",
                                 "The starting value of the animation",
                                 NULL,
                                 CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_FROM, pspec);

    pspec = g_param_spec_string ("to",
                                 "To",
                                 "The ending value of the animation",
                                 NULL,
                                 CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_TO, pspec);

    pspec = g_param_spec_boxed ("dur",
                                "Duration",
                                "The simple duration",
                                CASTET_TYPE_DURATION,
                                CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_DURATION, pspec);

    pspec = g_param_spec_boxed ("repeatCount",
                                "Repeat count",
                                "The number of iterations of the animation",
                                CASTET_TYPE_REPEAT_COUNT,
                                CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_REPEAT_COUNT, pspec);
}

static void
castet_animate_element_init (CastetAnimateElement *self)
{
    self->priv = ANIMATE_ELEMENT_PRIVATE (self);
}

CastetDomElement *
castet_animate_element_new (void)
{
    return g_object_new (CASTET_TYPE_ANIMATE_ELEMENT, NULL);
}

CastetDuration *
castet_animate_element_get_duration (const CastetAnimateElement *self)
{
    g_return_val_if_fail (CASTET_IS_ANIMATE_ELEMENT (self), NULL);

    return self->priv->duration;
}

const gchar *
castet_animate_element_get_attribute_name (const CastetAnimateElement *self)
{
    g_return_val_if_fail (CASTET_IS_ANIMATE_ELEMENT (self), NULL);

    return self->priv->attribute_name;
}

const gchar *
castet_animate_element_get_from (const CastetAnimateElement *self)
{
    g_return_val_if_fail (CASTET_IS_ANIMATE_ELEMENT (self), NULL);

    return self->priv->from;
}

const gchar *
castet_animate_element_get_to (const CastetAnimateElement *self)
{
    g_return_val_if_fail (CASTET_IS_ANIMATE_ELEMENT (self), NULL);

    return self->priv->to;
}

const CastetRepeatCount *
castet_animate_element_get_repeat_count (const CastetAnimateElement *self)
{
    g_return_val_if_fail (CASTET_IS_ANIMATE_ELEMENT (self), NULL);

    return self->priv->repeat_count;
}

CastetDomElement *
castet_animate_element_get_target (const CastetAnimateElement *self)
{
    CastetDomNode *node;

    g_return_val_if_fail (CASTET_IS_ANIMATE_ELEMENT (self), NULL);

    node = CASTET_DOM_NODE (self);

    return CASTET_DOM_ELEMENT (node->parent_node);
}
