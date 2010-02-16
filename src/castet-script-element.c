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
#include "castet-private.h"
#include "castet-enum-types.h"
#include "castet-paramspec.h"
#include "castet-dom-text.h"
#include "castet-script-element.h"

G_DEFINE_TYPE (CastetScriptElement, castet_script_element, CASTET_TYPE_ELEMENT)

#define SCRIPT_ELEMENT_PRIVATE(o)                                 \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                        \
                                      CASTET_TYPE_SCRIPT_ELEMENT, \
                                      CastetScriptElementPrivate))

enum
{
    PROP_0,

    PROP_TYPE
};

struct _CastetScriptElementPrivate
{
    CastetScriptType type;
};

static void
castet_script_element_get_property (GObject    *object,
                                    guint       property_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
    CastetScriptElement *self = CASTET_SCRIPT_ELEMENT (object);
    CastetScriptElementPrivate *priv = self->priv;

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
castet_script_element_set_property (GObject      *object,
                                    guint         property_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
    CastetScriptElement *self = CASTET_SCRIPT_ELEMENT (object);
    CastetScriptElementPrivate *priv = self->priv;

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
castet_script_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_script_element_parent_class)->dispose (object);
}

static void
castet_script_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_script_element_parent_class)->finalize (object);
}

static void
castet_script_element_class_init (CastetScriptElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (CastetScriptElementPrivate));

    object_class->get_property = castet_script_element_get_property;
    object_class->set_property = castet_script_element_set_property;
    object_class->dispose = castet_script_element_dispose;
    object_class->finalize = castet_script_element_finalize;

    pspec = castet_param_spec_enum ("type",
                                    "Type",
                                    "Identifies the programming language used",
                                    CASTET_TYPE_SCRIPT_TYPE,
                                    CASTET_SCRIPT_TYPE_DEFAULT,
                                    CASTET_PARAM_READWRITE,
                                    CASTET_PARAM_NONE,
                                    svg_ns);
    g_object_class_install_property (object_class, PROP_TYPE, pspec);
}

static void
castet_script_element_init (CastetScriptElement *self)
{
    self->priv = SCRIPT_ELEMENT_PRIVATE (self);
}

CastetDomElement *
castet_script_element_new (void)
{
    return g_object_new (CASTET_TYPE_SCRIPT_ELEMENT, NULL);
}

CastetScriptType
castet_script_element_get_script_type (const CastetScriptElement *script)
{
    g_return_val_if_fail (CASTET_IS_SCRIPT_ELEMENT (script), 0);

    return script->priv->type;
}

const gchar *
castet_script_element_get_code (const CastetScriptElement *script)
{
    CastetDomNode *text;

    g_return_val_if_fail (CASTET_IS_SCRIPT_ELEMENT (script), NULL);

    text = castet_dom_node_get_first_child (CASTET_DOM_NODE (script));
    if (text && CASTET_IS_DOM_TEXT (text)) {
        CastetDomCharacterData *char_data = CASTET_DOM_CHARACTER_DATA (text);
        return castet_dom_character_data_get_data (char_data);
    }

    return "";
}
