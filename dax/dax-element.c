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
#include "dax-debug.h"
#include "dax-private.h"
#include "dax-paramspec.h"
#include "dax-element-svg.h"
#include "dax-element.h"

G_DEFINE_ABSTRACT_TYPE (DaxElement, dax_element, DAX_TYPE_DOM_ELEMENT)

#define ELEMENT_PRIVATE(o)                                  \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      DAX_TYPE_ELEMENT,     \
                                      DaxElementPrivate))

enum
{
    PROP_0,

    PROP_FILL,
    PROP_FILL_OPACITY,
    PROP_STROKE,
};

struct _DaxElementPrivate
{
    ClutterColor *fill;
    gfloat fill_opacity;
    ClutterColor *stroke;
};

/*
 * DaxDomElement overloading
 */

static const gchar *
dax_element_get_attribute (DaxDomElement *self,
                           const gchar   *name)
{
#if 0
    GObjectClass *object_class= G_OBJECT_GET_CLASS (self);
    GParamSpec *pspec;
#endif
    return NULL;
}

static void
dax_element_set_attribute (DaxDomElement  *self,
                           const gchar    *name,
                           const gchar    *value,
                           GError        **err)
{
    GObjectClass *object_class= G_OBJECT_GET_CLASS (self);
    GParamSpec *pspec;
    GValue string_value = { 0, };
    GValue new_value = { 0, };
    gboolean success;

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

    success = g_value_transform (&string_value, &new_value);

    /* fallback to using the from_string() vfunc of DaxParamSpecClass */
    if (success == FALSE && DAX_IS_PARAM_SPEC_ARRAY (pspec)) {
        DaxParamSpecClass *pspec_klass;

        pspec_klass = DAX_PARAM_SPEC_GET_CLASS (pspec);
        success = pspec_klass->from_string (pspec, value, &new_value);
    }

    if (success == FALSE) {
        /* FIXME exception ? */
        g_warning ("Could not transform a string into a %s",
                   g_type_name (pspec->value_type));
        return;
    }

    DAX_NOTE (PARSING, "set %s to %s on %s",
              name,
              value,
              G_OBJECT_TYPE_NAME (self));

    g_object_set_property (G_OBJECT (self), name, &new_value);
}

/*
 * GObject overloading
 */

static void
dax_element_get_property (GObject    *object,
                          guint       property_id,
                          GValue     *value,
                          GParamSpec *pspec)
{
    DaxElement *element = DAX_ELEMENT (object);
    DaxElementPrivate *priv = element->priv;

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
dax_element_set_property (GObject      *object,
                          guint         property_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
    DaxElement *element = DAX_ELEMENT (object);
    DaxElementPrivate *priv = element->priv;

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
dax_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_element_parent_class)->dispose (object);
}

static void
dax_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_element_parent_class)->finalize (object);
}

static void
dax_element_class_init (DaxElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    DaxDomElementClass *dom_element_class = DAX_DOM_ELEMENT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxElementPrivate));

    object_class->get_property = dax_element_get_property;
    object_class->set_property = dax_element_set_property;
    object_class->dispose = dax_element_dispose;
    object_class->finalize = dax_element_finalize;

    dom_element_class->get_attribute = dax_element_get_attribute;
    dom_element_class->set_attribute = dax_element_set_attribute;

    pspec = g_param_spec_boxed ("fill",
                                "Fill color",
                                "The fill color of the element",
                                CLUTTER_TYPE_COLOR,
                                DAX_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_FILL, pspec);

    pspec = g_param_spec_boxed ("stroke",
                                "Stoke color",
                                "The color of the outline of the element",
                                CLUTTER_TYPE_COLOR,
                                DAX_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_STROKE, pspec);

    pspec = g_param_spec_float ("fill-opacity",
                                "Fill opacity",
                                "The opacity of the painting operation used to "
                                "paint the interior of the element",
                                0.0f, 1.0f,
                                1.0f,
                                DAX_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_FILL_OPACITY, pspec);
}

static void
dax_element_init (DaxElement *self)
{
    DaxDomNode *node = DAX_DOM_NODE (self);

    node->namespace_uri = svg_ns;
    self->priv = ELEMENT_PRIVATE (self);
}

DaxDomElement *
dax_element_new (void)
{
    return g_object_new (DAX_TYPE_ELEMENT, NULL);
}

const ClutterColor *
dax_element_get_fill_color (DaxElement *element)
{
    DaxElementPrivate *priv;
    DaxDomNode *parent;

    g_return_val_if_fail (DAX_IS_ELEMENT (element), NULL);

    priv = element->priv;
    if (priv->fill)
        return priv->fill;

    /* casting here as g_return_val_if_fail has already checked for the type */
    parent = ((DaxDomNode  *)element)->parent_node;
    if (DAX_IS_ELEMENT_SVG (parent))
        return NULL;

    return dax_element_get_fill_color (DAX_ELEMENT (parent));
}

const ClutterColor *
dax_element_get_stroke_color (DaxElement *element)
{
    DaxElementPrivate *priv;
    DaxDomNode *parent;

    g_return_val_if_fail (DAX_IS_ELEMENT (element), NULL);

    priv = element->priv;
    if (priv->stroke)
        return priv->stroke;

    /* casting here as g_return_val_if_fail has already checked for the type */
    parent = ((DaxDomNode  *)element)->parent_node;
    if (DAX_IS_ELEMENT_SVG (parent))
        return NULL;

    return dax_element_get_stroke_color (DAX_ELEMENT (parent));
}

gfloat
dax_element_get_fill_opacity (DaxElement *element)
{
    g_return_val_if_fail (DAX_IS_ELEMENT (element), 1.0f);

    return element->priv->fill_opacity;
}

/*
 * TraitAccess
 */
gfloat
dax_element_getFloatTrait (DaxElement *element,
                           const char *name)
{
    GObjectClass *object_class;
    GParamSpec *pspec;
    gfloat value = 0.0f;

    /* FIXME, ensure @name is a trait */

    object_class = G_OBJECT_GET_CLASS (element);
    pspec = g_object_class_find_property (object_class, name);
    if (pspec == NULL) {
        /* FIXME exception */
        g_message ("Unsupported attribute %s for %s",
                   name,
                   G_OBJECT_CLASS_NAME (object_class));
        return 0.0f;
    }

    if (G_PARAM_SPEC_TYPE (pspec) == G_TYPE_PARAM_FLOAT) {
        g_object_get (element, name, &value, NULL);
    } else if ((G_PARAM_SPEC_TYPE (pspec) == CLUTTER_TYPE_PARAM_UNITS) ||
               (G_PARAM_SPEC_TYPE (pspec) == G_TYPE_PARAM_BOXED)) {
        ClutterUnits *units;

        g_object_get (element, name, &units, NULL);
        value = clutter_units_to_pixels (units);
    }

    return value;
}

void
dax_element_setFloatTrait (DaxElement *element,
                           const char *name,
                           gfloat      value)
{
    GObjectClass *object_class;
    GParamSpec *pspec;

    object_class = G_OBJECT_GET_CLASS (element);
    pspec = g_object_class_find_property (object_class, name);
    if (pspec == NULL) {
        /* FIXME exception */
        g_message ("Unsupported attribute %s for %s",
                   name,
                   G_OBJECT_CLASS_NAME (object_class));
        return;
    }

    if (G_PARAM_SPEC_TYPE (pspec) == G_TYPE_PARAM_FLOAT) {
        g_object_set (element, name, value, NULL);
    } else if ((G_PARAM_SPEC_TYPE (pspec) == CLUTTER_TYPE_PARAM_UNITS) ||
               (G_PARAM_SPEC_TYPE (pspec) == G_TYPE_PARAM_BOXED)) {
        ClutterUnits units;

        clutter_units_from_pixels (&units, value);
        g_object_set (element, name, &units, NULL);
    }
}
