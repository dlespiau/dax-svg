/*
 * Castet - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
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
#include "castet-debug.h"
#include "castet-svg-element.h"
#include "castet-element.h"

G_DEFINE_ABSTRACT_TYPE (CastetElement, castet_element, CASTET_TYPE_DOM_ELEMENT)

#define ELEMENT_PRIVATE(o)                                  \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      CASTET_TYPE_ELEMENT,  \
                                      CastetElementPrivate))

enum
{
    PROP_0,

    PROP_FILL,
    PROP_FILL_OPACITY,
    PROP_STROKE,
};

struct _CastetElementPrivate
{
    ClutterColor *fill;
    gfloat fill_opacity;
    ClutterColor *stroke;
};

/*
 * CastetDomElement overloading
 */

static const gchar *
castet_element_get_attribute(CastetDomElement *self,
                             const gchar      *name)
{
#if 0
    GObjectClass *object_class= G_OBJECT_GET_CLASS (self);
    GParamSpec *pspec;
#endif
    return NULL;
}

static void
castet_element_set_attribute (CastetDomElement  *self,
                              const gchar       *name,
                              const gchar       *value,
                              GError           **err)
{
    GObjectClass *object_class= G_OBJECT_GET_CLASS (self);
    GParamSpec *pspec;
    GValue string_value = { 0, };
    GValue new_value = { 0, };

    pspec = g_object_class_find_property (object_class, name);
    if (pspec == NULL) {
        /* FIXME exception */
        g_message ("Unsupported attribute %s for %s",
                   name,
                   G_OBJECT_CLASS_NAME (object_class));
        return;
    }

    /* we don't want to duplicate the string here */
    g_value_init (&string_value, G_TYPE_STRING);
    g_value_set_static_string (&string_value, value);

    /* this GValue holds the new value of the property we want to set */
    g_value_init (&new_value, pspec->value_type);

    if (g_value_transform (&string_value, &new_value) == FALSE) {
        /* FIXME exception ? */
        g_warning ("Could not transform a string into a %s",
                   g_type_name (pspec->value_type));
        return;
    }

    CASTET_NOTE (PARSING, "set %s to %s on %s",
                 name,
                 value,
                 G_OBJECT_TYPE_NAME (self));
    g_object_set_property (G_OBJECT (self), name, &new_value);
}

/*
 * GObject overloading
 */

static void
castet_element_get_property (GObject    *object,
                             guint       property_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
    CastetElement *element = CASTET_ELEMENT (object);
    CastetElementPrivate *priv = element->priv;

    switch (property_id)
    {
    case PROP_FILL:
        clutter_value_set_color (value, priv->fill);
        break;
    case PROP_STROKE:
        clutter_value_set_color (value, priv->stroke);
        break;
    case PROP_FILL_OPACITY:
        g_value_set_float (value, priv->fill_opacity);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
castet_element_set_property (GObject      *object,
                             guint         property_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
    CastetElement *element = CASTET_ELEMENT (object);
    CastetElementPrivate *priv = element->priv;

    switch (property_id)
    {
    case PROP_FILL:
    {
        const ClutterColor *color;
        color = clutter_value_get_color (value);
        priv->fill = clutter_color_copy (color);
    }
        break;
    case PROP_STROKE:
    {
        const ClutterColor *color;
        color = clutter_value_get_color (value);
        priv->stroke = clutter_color_copy (color);
    }
        break;
    case PROP_FILL_OPACITY:
        priv->fill_opacity = g_value_get_float (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
castet_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_element_parent_class)->dispose (object);
}

static void
castet_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_element_parent_class)->finalize (object);
}

static void
castet_element_class_init (CastetElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    CastetDomElementClass *dom_element_class = CASTET_DOM_ELEMENT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (CastetElementPrivate));

    object_class->get_property = castet_element_get_property;
    object_class->set_property = castet_element_set_property;
    object_class->dispose = castet_element_dispose;
    object_class->finalize = castet_element_finalize;

    dom_element_class->get_attribute = castet_element_get_attribute;
    dom_element_class->set_attribute = castet_element_set_attribute;

    pspec = g_param_spec_boxed ("fill",
                                "Fill color",
                                "The fill color of the element",
                                CLUTTER_TYPE_COLOR,
                                CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_FILL, pspec);

    pspec = g_param_spec_boxed ("stroke",
                                "Stoke color",
                                "The color of the outline of the element",
                                CLUTTER_TYPE_COLOR,
                                CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_STROKE, pspec);

    pspec = g_param_spec_float ("fill-opacity",
                                "Fill opacity",
                                "The opacity of the painting operation used to "
                                "paint the interior of the element",
                                0.0f, 1.0f,
                                1.0f,
                                CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_FILL_OPACITY, pspec);
}

static void
castet_element_init (CastetElement *self)
{
    self->priv = ELEMENT_PRIVATE (self);
}

CastetDomElement *
castet_element_new (void)
{
    return g_object_new (CASTET_TYPE_ELEMENT, NULL);
}

const ClutterColor *
castet_element_get_fill_color (CastetElement *element)
{
    CastetElementPrivate *priv;
    CastetDomNode *parent;

    g_return_val_if_fail (CASTET_IS_ELEMENT (element), NULL);

    priv = element->priv;
    if (priv->fill)
        return priv->fill;

    /* casting here as g_return_val_if_fail has already checked for the type */
    parent = ((CastetDomNode  *)element)->parent_node;
    if (CASTET_IS_SVG_ELEMENT (parent))
        return NULL;

    return castet_element_get_fill_color (CASTET_ELEMENT (parent));
}

const ClutterColor *
castet_element_get_stroke_color (CastetElement *element)
{
    CastetElementPrivate *priv;
    CastetDomNode *parent;

    g_return_val_if_fail (CASTET_IS_ELEMENT (element), NULL);

    priv = element->priv;
    if (priv->stroke)
        return priv->stroke;

    /* casting here as g_return_val_if_fail has already checked for the type */
    parent = ((CastetDomNode  *)element)->parent_node;
    if (CASTET_IS_SVG_ELEMENT (parent))
        return NULL;

    return castet_element_get_stroke_color (CASTET_ELEMENT (parent));
}

gfloat
castet_element_get_fill_opacity (CastetElement *element)
{
    g_return_val_if_fail (CASTET_IS_ELEMENT (element), 1.0f);

    return element->priv->fill_opacity;
}
