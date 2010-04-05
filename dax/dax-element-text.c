/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
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

#include "dax-dom-character-data.h"
#include "dax-enum-types.h"
#include "dax-internals.h"
#include "dax-private.h"
#include "dax-paramspec.h"
#include "dax-element-text.h"

G_DEFINE_TYPE (DaxElementText, dax_element_text, DAX_TYPE_ELEMENT)

#define ELEMENT_TEXT_PRIVATE(o)                         \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o),                    \
                                DAX_TYPE_ELEMENT_TEXT,  \
                                DaxElementTextPrivate))

enum {
    PROP_0,

    PROP_X,
    PROP_Y,
    PROP_EDITABLE,
    PROP_ROTATE
};

struct _DaxElementTextPrivate
{
    GArray *x;
    GArray *y;
    DaxTextEditable editable;
    GArray *rotate;
};


static void
dax_element_text_get_property (GObject    *object,
                               guint       property_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
    DaxElementText *self = DAX_ELEMENT_TEXT (object);
    DaxElementTextPrivate *priv = self->priv;

    switch (property_id)
    {
    case PROP_X:
        g_value_set_boxed (value, priv->x);
        break;
    case PROP_Y:
        g_value_set_boxed (value, priv->y);
        break;
    case PROP_EDITABLE:
        g_value_set_enum (value, priv->editable);
        break;
    case PROP_ROTATE:
        g_value_set_boxed (value, priv->rotate);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_text_set_property (GObject      *object,
                               guint         property_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
    DaxElementText *self = DAX_ELEMENT_TEXT (object);
    DaxElementTextPrivate *priv = self->priv;

    switch (property_id)
    {
    case PROP_X:
        if (priv->x)
            g_array_free (priv->x, TRUE);
        priv->x = g_value_get_boxed (value);
        break;
    case PROP_Y:
        if (priv->y)
            g_array_free (priv->y, TRUE);
        priv->y = g_value_get_boxed (value);
        break;
    case PROP_EDITABLE:
         priv->editable = g_value_get_enum (value);
        break;
    case PROP_ROTATE:
        if (priv->rotate)
            g_array_free (priv->rotate, TRUE);
        priv->rotate = g_value_get_boxed (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_text_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_element_text_parent_class)->dispose (object);
}

static void
dax_element_text_finalize (GObject *object)
{
    DaxElementText *self = DAX_ELEMENT_TEXT (object);
    DaxElementTextPrivate *priv = self->priv;

    if (priv->x)
        g_array_free (priv->x, TRUE);
    if (priv->y)
        g_array_free (priv->y, TRUE);
    if (priv->rotate)
        g_array_free (priv->rotate, TRUE);

    G_OBJECT_CLASS (dax_element_text_parent_class)->finalize (object);
}

static void
dax_element_text_class_init (DaxElementTextClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxElementTextPrivate));

    object_class->get_property = dax_element_text_get_property;
    object_class->set_property = dax_element_text_set_property;
    object_class->dispose = dax_element_text_dispose;
    object_class->finalize = dax_element_text_finalize;

    pspec = dax_param_spec_array ("x",
                                  "x",
                                  "Absolute X coordinate of glyphs",
                                  CLUTTER_TYPE_UNITS,
                                  DAX_PARAM_SPEC_ARRAY_NOT_SIZED,
                                  DAX_PARAM_READWRITE,
                                  DAX_PARAM_NONE,
                                  svg_ns);
    g_object_class_install_property (object_class, PROP_X, pspec);

    pspec = dax_param_spec_array ("y",
                                  "y",
                                  "Absolute Y coordinate of glyphs",
                                  CLUTTER_TYPE_UNITS,
                                  DAX_PARAM_SPEC_ARRAY_NOT_SIZED,
                                  DAX_PARAM_READWRITE,
                                  DAX_PARAM_NONE,
                                  svg_ns);
    g_object_class_install_property (object_class, PROP_Y, pspec);

    pspec = dax_param_spec_enum ("editable",
                                 "Editable",
                                 "Whether the text can be edited",
                                 DAX_TYPE_TEXT_EDITABLE,
                                 DAX_TEXT_EDITABLE_DEFAULT,
                                 DAX_PARAM_READWRITE,
                                 DAX_PARAM_NONE,
                                 svg_ns);
    g_object_class_install_property (object_class, PROP_EDITABLE, pspec);

    pspec = dax_param_spec_array ("rotate",
                                  "Rotate",
                                  "Supplemental rotation of glyphs",
                                  G_TYPE_FLOAT,
                                  DAX_PARAM_SPEC_ARRAY_NOT_SIZED,
                                  DAX_PARAM_READWRITE,
                                  DAX_PARAM_NONE,
                                  svg_ns);
    g_object_class_install_property (object_class, PROP_ROTATE, pspec);
}

static void
dax_element_text_init (DaxElementText *self)
{
    self->priv = ELEMENT_TEXT_PRIVATE (self);
}

DaxDomElement *
dax_element_text_new (void)
{
    return g_object_new (DAX_TYPE_ELEMENT_TEXT, NULL);
}

GArray *
dax_element_text_get_x (const DaxElementText *text)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_TEXT (text), NULL);

    return text->priv->x;
}

GArray *
dax_element_text_get_y (const DaxElementText *text)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_TEXT (text), NULL);

    return text->priv->y;
}

DaxTextEditable
dax_element_text_get_editable (const DaxElementText *text)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_TEXT (text), DAX_TEXT_EDITABLE_NONE);

    return text->priv->editable;
}

const GArray *
dax_element_text_get_rotation (const DaxElementText *text)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_TEXT (text), NULL);

    return text->priv->rotate;
}

static void
dax_element_text_collect_cdata (DaxDomNode *node,
                                GString    *text)
{
    if (DAX_IS_DOM_CHARACTER_DATA (node)) {
        DaxDomCharacterData *cdata;

        cdata = (DaxDomCharacterData *) node;
        g_string_append (text, dax_dom_character_data_get_data (cdata));
    }
}

static void
dax_element_text_walk_tree (DaxDomNode *node,
                            GString    *text)
{
    dax_element_text_collect_cdata (node, text);
    node = node->first_child;
    while (node) {
        dax_element_text_walk_tree (node, text);
        node = node->next_sibling;
    }
}

/* FIXME: this totally ignores the <tspan> nodes */
gchar *
dax_element_text_get_text (const DaxElementText *text)
{
    DaxDomNode *text_node;
    GString *string;

    string = g_string_new (NULL);
    text_node = DAX_DOM_NODE (text);

    dax_element_text_walk_tree (text_node, string);

    return g_string_free (string, FALSE);
}
