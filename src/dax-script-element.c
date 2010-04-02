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
#include "dax-private.h"
#include "dax-enum-types.h"
#include "dax-paramspec.h"
#include "dax-dom-text.h"
#include "dax-script-element.h"

G_DEFINE_TYPE (DaxScriptElement, dax_script_element, DAX_TYPE_ELEMENT)

#define SCRIPT_ELEMENT_PRIVATE(o)                                 \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                        \
                                      DAX_TYPE_SCRIPT_ELEMENT, \
                                      DaxScriptElementPrivate))

enum
{
    PROP_0,

    PROP_TYPE
};

struct _DaxScriptElementPrivate
{
    DaxScriptType type;
};

static void
dax_script_element_get_property (GObject    *object,
                                    guint       property_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
    DaxScriptElement *self = DAX_SCRIPT_ELEMENT (object);
    DaxScriptElementPrivate *priv = self->priv;

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
dax_script_element_set_property (GObject      *object,
                                    guint         property_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
    DaxScriptElement *self = DAX_SCRIPT_ELEMENT (object);
    DaxScriptElementPrivate *priv = self->priv;

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
dax_script_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_script_element_parent_class)->dispose (object);
}

static void
dax_script_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_script_element_parent_class)->finalize (object);
}

static void
dax_script_element_class_init (DaxScriptElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxScriptElementPrivate));

    object_class->get_property = dax_script_element_get_property;
    object_class->set_property = dax_script_element_set_property;
    object_class->dispose = dax_script_element_dispose;
    object_class->finalize = dax_script_element_finalize;

    pspec = dax_param_spec_enum ("type",
                                    "Type",
                                    "Identifies the programming language used",
                                    DAX_TYPE_SCRIPT_TYPE,
                                    DAX_SCRIPT_TYPE_ECMASCRIPT,
                                    DAX_PARAM_READWRITE,
                                    DAX_PARAM_NONE,
                                    svg_ns);
    g_object_class_install_property (object_class, PROP_TYPE, pspec);
}

static void
dax_script_element_init (DaxScriptElement *self)
{
    self->priv = SCRIPT_ELEMENT_PRIVATE (self);
}

DaxDomElement *
dax_script_element_new (void)
{
    return g_object_new (DAX_TYPE_SCRIPT_ELEMENT, NULL);
}

DaxScriptType
dax_script_element_get_script_type (const DaxScriptElement *script)
{
    g_return_val_if_fail (DAX_IS_SCRIPT_ELEMENT (script), 0);

    return script->priv->type;
}

const gchar *
dax_script_element_get_code (const DaxScriptElement *script)
{
    DaxDomNode *text;

    g_return_val_if_fail (DAX_IS_SCRIPT_ELEMENT (script), NULL);

    text = dax_dom_node_get_first_child (DAX_DOM_NODE (script));
    if (text && DAX_IS_DOM_TEXT (text)) {
        DaxDomCharacterData *char_data = DAX_DOM_CHARACTER_DATA (text);
        return dax_dom_character_data_get_data (char_data);
    }

    return "";
}
