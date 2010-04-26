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

#include <string.h>

#include <gio/gio.h>

#include "dax-dom.h"

#include "dax-debug.h"
#include "dax-enum-types.h"
#include "dax-internals.h"
#include "dax-js-context.h"
#include "dax-paramspec.h"
#include "dax-private.h"
#include "dax-utils.h"

#include "dax-element-script.h"

G_DEFINE_TYPE (DaxElementScript, dax_element_script, DAX_TYPE_ELEMENT)

#define ELEMENT_SCRIPT_PRIVATE(o)                               \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      DAX_TYPE_ELEMENT_SCRIPT,  \
                                      DaxElementScriptPrivate))

enum
{
    PROP_0,

    PROP_TYPE,
    PROP_HREF,
};

struct _DaxElementScriptPrivate
{
    DaxScriptType type;
    gchar *href;
    gchar *resolved_href;
};

static void
invalid_resolved_href (DaxElementScript *script)
{
    DaxElementScriptPrivate *priv = script->priv;

    if (priv->resolved_href == NULL)
        return;

    /* resolved_href can be href in the case href is an [UI]RI */
    if (priv->href == priv->resolved_href)
        g_free (priv->resolved_href);

    priv->resolved_href = NULL;
}

/*
 * DaxDomElement implementation
 */

static const gchar*
dax_element_script_resolve_href (DaxElementScript *script)
{
    DaxElementScriptPrivate *priv = script->priv;

    if (priv->resolved_href)
        return priv->resolved_href;

    if (_dax_utils_is_iri (priv->href)) {
        priv->resolved_href = priv->href;
    } else {
        const gchar *base_iri;
        GFile *base_file, *resolved_file;

        base_iri = dax_element_get_base_iri ((DaxElement *) script);
        base_file = g_file_new_for_uri (base_iri);

        resolved_file = g_file_resolve_relative_path (base_file, priv->href);
        priv->resolved_href = g_file_get_uri (resolved_file);

        g_object_unref (base_file);
        g_object_unref (resolved_file);
    }

    return priv->resolved_href;
}

static gchar *
dax_element_script_get_href_content (DaxElementScript *script,
                                     gsize            *size)
{
    gchar *filename, *script_content;
    const gchar *script_iri;
    GFile *script_file;
    GError *error = NULL;

    script_iri = dax_element_script_resolve_href (script);

    DAX_NOTE (LOADING, "loading script %s", script_iri);

    /* TODO: async loading, "download manager" with http support */

    if (!g_str_has_prefix (script_iri, "file://")) {
        g_warning ("can't load %s", script_iri);
        *size = 0;
        return NULL;
    }

    script_file = g_file_new_for_uri (script_iri);
    filename = g_file_get_path (script_file);

    if (!g_file_get_contents (filename, &script_content, size, &error)) {
        g_warning (G_STRLOC ": could not load %s: %s", filename,
                   error->message);
        *size = 0;
        script_content = NULL;
    }

    g_object_unref (script_file);
    g_free (filename);

    return script_content;
}

static void
dax_element_script_parsed (DaxDomElement *element)
{
    DaxElementScript *script = (DaxElementScript *) element;
    DaxElementScriptPrivate *priv = script->priv;
    DaxJsContext *js_context;
    GError *error = NULL;
    int retval;

    /* If a 'script' element has both an 'xlink:href' attribute and child
     * character data, the executable content for the script is retrieved from
     * the IRI of the 'xlink:href' attribute, and the child content is not
     * added to the scripting context - REC-SVGTiny12-20081222 p.205 */

    js_context = dax_js_context_get_default ();

    if (priv->href) {
        gchar *code;
        gsize size;

        code = dax_element_script_get_href_content (script, &size);
        if (code && !dax_js_context_eval (js_context,
                                          code,
                                          size,
                                          "svg",
                                          &retval,
                                          &error))
        {
            g_warning ("Error: %s", error->message);
        }
        g_free (code);

    } else {
        const gchar *code = NULL;
        DaxDomNode *text;
        gsize size = 0;

        text = dax_dom_node_get_first_child (DAX_DOM_NODE (script));
        if (text && DAX_IS_DOM_CHARACTER_DATA (text)) {
            DaxDomCharacterData *char_data = DAX_DOM_CHARACTER_DATA (text);
            code = dax_dom_character_data_get_data (char_data);
            size = strlen (code);
        }

        if (!dax_js_context_eval (js_context,
                                  code,
                                  size,
                                  "svg",
                                  &retval,
                                  &error))
        {
            g_warning ("Error: %s", error->message);
        }
    }

}

/*
 * GObject implementation
 */

static void
dax_element_script_get_property (GObject    *object,
                                 guint       property_id,
                                 GValue     *value,
                                 GParamSpec *pspec)
{
    DaxElementScript *script = DAX_ELEMENT_SCRIPT (object);
    DaxElementScriptPrivate *priv = script->priv;

    switch (property_id)
    {
    case PROP_TYPE:
        g_value_set_enum (value, priv->type);
        break;
    case PROP_HREF:
        invalid_resolved_href (script);     /* remove cached href */
        g_value_set_string (value, priv->href);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_script_set_property (GObject      *object,
                                 guint         property_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
    DaxElementScript *self = DAX_ELEMENT_SCRIPT (object);
    DaxElementScriptPrivate *priv = self->priv;

    switch (property_id)
    {
    case PROP_TYPE:
        priv->type = g_value_get_enum (value);
        break;
    case PROP_HREF:
        priv->href = g_value_dup_string (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_script_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_element_script_parent_class)->dispose (object);
}

static void
dax_element_script_finalize (GObject *object)
{
    DaxElementScript *script = (DaxElementScript *) object;
    DaxElementScriptPrivate *priv = script->priv;

    invalid_resolved_href (script);
    g_free (priv->href);

    G_OBJECT_CLASS (dax_element_script_parent_class)->finalize (object);
}

static void
dax_element_script_class_init (DaxElementScriptClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    DaxDomElementClass *dom_element_class = DAX_DOM_ELEMENT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxElementScriptPrivate));

    object_class->get_property = dax_element_script_get_property;
    object_class->set_property = dax_element_script_set_property;
    object_class->dispose = dax_element_script_dispose;
    object_class->finalize = dax_element_script_finalize;

    dom_element_class->parsed = dax_element_script_parsed;

    pspec = dax_param_spec_enum ("type",
                                 "Type",
                                 "Identifies the programming language used",
                                 DAX_TYPE_SCRIPT_TYPE,
                                 DAX_SCRIPT_TYPE_ECMASCRIPT,
                                 DAX_GPARAM_READWRITE,
                                 DAX_PARAM_NONE,
                                 svg_ns);
    g_object_class_install_property (object_class, PROP_TYPE, pspec);

    pspec = dax_param_spec_string ("href",
                                   "href",
                                   "An IRI reference to an external resource "
                                   "containing the script code",
                                   NULL,
                                   DAX_GPARAM_READWRITE,
                                   DAX_PARAM_NONE,
                                   xlink_ns);
    g_object_class_install_property (object_class, PROP_HREF, pspec);
}

static void
dax_element_script_init (DaxElementScript *self)
{
    self->priv = ELEMENT_SCRIPT_PRIVATE (self);
}

DaxDomElement *
dax_element_script_new (void)
{
    return g_object_new (DAX_TYPE_ELEMENT_SCRIPT, NULL);
}

DaxScriptType
dax_element_script_get_script_type (DaxElementScript *script)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_SCRIPT (script), 0);

    return script->priv->type;
}
