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

#include <string.h>

#include "castet-internals.h"
#include "castet-enum-types.h"
#include "castet-js-context.h"
#include "castet-xml-event-listener.h"
#include "castet-dom-text.h"
#include "castet-script-element.h"
#include "castet-handler-element.h"

static void castet_xml_event_listener_init (CastetXmlEventListenerIface *iface);

G_DEFINE_TYPE_WITH_CODE (CastetHandlerElement,
                         castet_handler_element,
                         CASTET_TYPE_ELEMENT,
                         G_IMPLEMENT_INTERFACE (CASTET_TYPE_XML_EVENT_LISTENER,
                                                castet_xml_event_listener_init))

#define HANDLER_ELEMENT_PRIVATE(o)                                  \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                          \
                                      CASTET_TYPE_HANDLER_ELEMENT,  \
                                      CastetHandlerElementPrivate))

enum
{
    PROP_0,

    PROP_TYPE,
    PROP_EVENT_TYPE,
};

struct _CastetHandlerElementPrivate
{
    CastetJsContext *js_context;

    CastetScriptType type;
    CastetXmlEventType event_type;
};

/*
 * CastetXmlEventListener implementation
 */

static void
castet_handler_element_handle_event (CastetXmlEventListener *listener,
                                     CastetXmlEvent         *xml_event)
{
    CastetHandlerElement *handler = CASTET_HANDLER_ELEMENT (listener);
    CastetHandlerElementPrivate *priv = handler->priv;
    CastetDomElement *target;
    CastetJsObject *event;
    gchar *code;

    target = castet_handler_element_get_target (handler);
    code = castet_handler_element_get_code (handler);
    event = castet_js_context_new_object_from_xml_event (priv->js_context,
                                                         xml_event);
    castet_js_context_eval (priv->js_context,
                            code,
                            strlen (code),
                            "svg",
                            NULL,
                            NULL);

    castet_js_context_call_function (priv->js_context,
                                     "__castet_handler",
                                     "o",
                                     event);
}

static void
castet_xml_event_listener_init (CastetXmlEventListenerIface *iface)
{
    iface->handle_event = castet_handler_element_handle_event;
}

/*
 * GObject overloading
 */

static void
castet_handler_element_get_property (GObject    *object,
                                     guint       property_id,
                                     GValue     *value,
                                     GParamSpec *pspec)
{
    CastetHandlerElement *self = CASTET_HANDLER_ELEMENT (object);
    CastetHandlerElementPrivate *priv = self->priv;

    switch (property_id)
    {
    case PROP_TYPE:
        g_value_set_enum (value, priv->type);
        break;
    case PROP_EVENT_TYPE:
        g_value_set_enum (value, priv->event_type);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
castet_handler_element_set_property (GObject      *object,
                                     guint         property_id,
                                     const GValue *value,
                                     GParamSpec   *pspec)
{
    CastetHandlerElement *self = CASTET_HANDLER_ELEMENT (object);
    CastetHandlerElementPrivate *priv = self->priv;

    switch (property_id)
    {
    case PROP_TYPE:
        priv->type = g_value_get_enum (value);
        break;
    case PROP_EVENT_TYPE:
        priv->event_type = g_value_get_enum (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
castet_handler_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_handler_element_parent_class)->dispose (object);
}

static void
castet_handler_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_handler_element_parent_class)->finalize (object);
}

static void
castet_handler_element_class_init (CastetHandlerElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (CastetHandlerElementPrivate));

    object_class->get_property = castet_handler_element_get_property;
    object_class->set_property = castet_handler_element_set_property;
    object_class->dispose = castet_handler_element_dispose;
    object_class->finalize = castet_handler_element_finalize;

    pspec = g_param_spec_enum ("type",
                               "Type",
                               "Identifies the programming language used",
                               CASTET_TYPE_SCRIPT_TYPE,
                               CASTET_SCRIPT_TYPE_DEFAULT,
                               CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_TYPE, pspec);

    pspec = g_param_spec_enum ("event",
                               "Event",
                               "The name of the event to handle",
                               CASTET_TYPE_XML_EVENT_TYPE,
                               CASTET_XML_EVENT_TYPE_DEFAULT,
                               CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_EVENT_TYPE, pspec);
}

static void
castet_handler_element_init (CastetHandlerElement *self)
{
    CastetHandlerElementPrivate *priv;

    self->priv = priv = HANDLER_ELEMENT_PRIVATE (self);

    priv->js_context = castet_js_context_get_default ();
}

CastetDomElement *
castet_handler_element_new (void)
{
    return g_object_new (CASTET_TYPE_HANDLER_ELEMENT, NULL);
}

CastetDomElement *
castet_handler_element_get_target (CastetHandlerElement *handler)
{
    CastetDomNode *element_node;

    g_return_val_if_fail (CASTET_IS_HANDLER_ELEMENT (handler), NULL);

    /* FIXME: the target can also be defined with ev:target */
    element_node = castet_dom_node_get_parent_node (CASTET_DOM_NODE (handler));

    return CASTET_DOM_ELEMENT (element_node);
}

gchar *
castet_handler_element_get_code (const CastetHandlerElement *handler)
{
    CastetDomNode *text;
    static gchar script_template[] = "function __castet_handler(event) {"
                                        "let evt=event;"
                                        "%s"
                                     "}";
#if 0
    static gchar script_template[] = "let evt=event;" "%s";
#endif

    g_return_val_if_fail (CASTET_IS_HANDLER_ELEMENT (handler), NULL);

    text = castet_dom_node_get_first_child (CASTET_DOM_NODE (handler));
    if (text && CASTET_IS_DOM_TEXT (text)) {
        CastetDomCharacterData *char_data = CASTET_DOM_CHARACTER_DATA (text);
        return g_strdup_printf (script_template,
                                castet_dom_character_data_get_data (char_data));
    }

    return NULL;
}
