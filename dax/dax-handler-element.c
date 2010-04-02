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

#include "dax-internals.h"
#include "dax-private.h"
#include "dax-enum-types.h"
#include "dax-paramspec.h"
#include "dax-js-context.h"
#include "dax-xml-event-listener.h"
#include "dax-dom-text.h"
#include "dax-script-element.h"
#include "dax-handler-element.h"

static void dax_xml_event_listener_init (DaxXmlEventListenerIface *iface);

G_DEFINE_TYPE_WITH_CODE (DaxHandlerElement,
                         dax_handler_element,
                         DAX_TYPE_ELEMENT,
                         G_IMPLEMENT_INTERFACE (DAX_TYPE_XML_EVENT_LISTENER,
                                                dax_xml_event_listener_init))

#define HANDLER_ELEMENT_PRIVATE(o)                                  \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                          \
                                      DAX_TYPE_HANDLER_ELEMENT,  \
                                      DaxHandlerElementPrivate))

enum
{
    PROP_0,

    PROP_TYPE,
    PROP_EVENT_TYPE,
};

struct _DaxHandlerElementPrivate
{
    DaxJsContext *js_context;

    DaxScriptType type;
    DaxXmlEventType event_type;
};

/*
 * DaxXmlEventListener implementation
 */

static void
dax_handler_element_handle_event (DaxXmlEventListener *listener,
                                     DaxXmlEvent         *xml_event)
{
    DaxHandlerElement *handler = DAX_HANDLER_ELEMENT (listener);
    DaxHandlerElementPrivate *priv = handler->priv;
    DaxDomElement *target;
    DaxJsObject *event;
    gchar *code;

    target = dax_handler_element_get_target (handler);
    code = dax_handler_element_get_code (handler);
    event = dax_js_context_new_object_from_xml_event (priv->js_context,
                                                         xml_event);
    dax_js_context_eval (priv->js_context,
                            code,
                            strlen (code),
                            "svg",
                            NULL,
                            NULL);

    dax_js_context_call_function (priv->js_context,
                                     "__dax_handler",
                                     "o",
                                     event);
}

static void
dax_xml_event_listener_init (DaxXmlEventListenerIface *iface)
{
    iface->handle_event = dax_handler_element_handle_event;
}

/*
 * GObject overloading
 */

static void
dax_handler_element_get_property (GObject    *object,
                                     guint       property_id,
                                     GValue     *value,
                                     GParamSpec *pspec)
{
    DaxHandlerElement *self = DAX_HANDLER_ELEMENT (object);
    DaxHandlerElementPrivate *priv = self->priv;

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
dax_handler_element_set_property (GObject      *object,
                                     guint         property_id,
                                     const GValue *value,
                                     GParamSpec   *pspec)
{
    DaxHandlerElement *self = DAX_HANDLER_ELEMENT (object);
    DaxHandlerElementPrivate *priv = self->priv;

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
dax_handler_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_handler_element_parent_class)->dispose (object);
}

static void
dax_handler_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_handler_element_parent_class)->finalize (object);
}

static void
dax_handler_element_class_init (DaxHandlerElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxHandlerElementPrivate));

    object_class->get_property = dax_handler_element_get_property;
    object_class->set_property = dax_handler_element_set_property;
    object_class->dispose = dax_handler_element_dispose;
    object_class->finalize = dax_handler_element_finalize;

    pspec = dax_param_spec_enum ("type",
                                    "Type",
                                    "Identifies the programming language used",
                                    DAX_TYPE_SCRIPT_TYPE,
                                    DAX_SCRIPT_TYPE_ECMASCRIPT,
                                    DAX_PARAM_READWRITE,
                                    DAX_PARAM_NONE,
                                    svg_ns);
    g_object_class_install_property (object_class, PROP_TYPE, pspec);

    pspec = dax_param_spec_enum ("event",
                                    "Event",
                                    "The name of the event to handle",
                                    DAX_TYPE_XML_EVENT_TYPE,
                                    DAX_XML_EVENT_TYPE_DEFAULT,
                                    DAX_PARAM_READWRITE,
                                    DAX_PARAM_NONE,
                                    xmlevents_ns);
    g_object_class_install_property (object_class, PROP_EVENT_TYPE, pspec);
}

static void
dax_handler_element_init (DaxHandlerElement *self)
{
    DaxHandlerElementPrivate *priv;

    self->priv = priv = HANDLER_ELEMENT_PRIVATE (self);

    priv->js_context = dax_js_context_get_default ();
}

DaxDomElement *
dax_handler_element_new (void)
{
    return g_object_new (DAX_TYPE_HANDLER_ELEMENT, NULL);
}

DaxDomElement *
dax_handler_element_get_target (DaxHandlerElement *handler)
{
    DaxDomNode *element_node;

    g_return_val_if_fail (DAX_IS_HANDLER_ELEMENT (handler), NULL);

    /* FIXME: the target can also be defined with ev:target */
    element_node = dax_dom_node_get_parent_node (DAX_DOM_NODE (handler));

    return DAX_DOM_ELEMENT (element_node);
}

gchar *
dax_handler_element_get_code (const DaxHandlerElement *handler)
{
    DaxDomNode *text;

    g_return_val_if_fail (DAX_IS_HANDLER_ELEMENT (handler), NULL);

    text = dax_dom_node_get_first_child (DAX_DOM_NODE (handler));
    if (text && DAX_IS_DOM_TEXT (text)) {
        DaxDomCharacterData *char_data = DAX_DOM_CHARACTER_DATA (text);
        return g_strdup_printf ("function __dax_handler(event) {"
                                    "let evt=event;"
                                    "%s"
                                 "}",
                                dax_dom_character_data_get_data (char_data));
    }

    return NULL;
}
