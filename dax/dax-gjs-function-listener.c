/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009, 2010 Intel Corporation.
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

#include <gjs/gjs.h>

#include "dax-internals.h"
#include "dax-debug.h"

#include "dax-gjs-context.h"
#include "dax-gjs-function-listener.h"

static void dax_xml_event_listener_init (DaxXmlEventListenerIface *iface);

G_DEFINE_TYPE_WITH_CODE (DaxJsFunctionListener,
                         dax_js_function_listener,
                         G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (DAX_TYPE_XML_EVENT_LISTENER,
                                                dax_xml_event_listener_init))

#define JS_FUNCTION_LISTENER_PRIVATE(o)                                 \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                              \
                                      DAX_TYPE_JS_FUNCTION_LISTENER,    \
                                      DaxJsFunctionListenerPrivate))

enum
{
    PROP_0,

    PROP_FUNCTION
};

struct _DaxJsFunctionListenerPrivate
{
    DaxJsContext *js_context;
    JSContext *native_context;
    jsval function;
};

static JSFunction *
dax_js_function_listener_get_function (DaxJsFunctionListener *listener)
{
    DaxJsFunctionListenerPrivate *priv = listener->priv;

    return (JSFunction *) JSVAL_TO_OBJECT (priv->function);
}

static void
dax_js_function_listener_set_function (DaxJsFunctionListener *listener,
                                       DaxJsFunction         *function)
{
    DaxJsFunctionListenerPrivate *priv = listener->priv;
    jsval in_function;
    JSBool ret;

    in_function = OBJECT_TO_JSVAL (function);
    ret = JS_ConvertValue (priv->native_context,
                           in_function,
                           JSTYPE_FUNCTION,
                           &priv->function);
    if (G_UNLIKELY (ret == JS_FALSE)) {
        g_warning (G_STRLOC ": could not convert pointer to a JS function");
        priv->function = JSVAL_NULL;
    }
}

/*
 * DaxXmlEventListener implementation
 */

static void
dax_js_function_listener_handle_event (DaxXmlEventListener *listener,
                                       DaxXmlEvent         *xml_event)
{
    DaxJsFunctionListener *func_listener = DAX_JS_FUNCTION_LISTENER (listener);
    DaxJsFunctionListenerPrivate *priv = func_listener->priv;
    JSObject *event;
    jsval argv[1], ret_val;
    JSBool ret;

    event = dax_js_context_new_object_from_xml_event (priv->js_context,
                                                      xml_event);
    argv[0] = OBJECT_TO_JSVAL (event);

#if 0
    /* FIXME: UTF-16 string to display in the debug message */
    DAX_NOTE (SCRIPT, "calling %s",
              JS_GetFunctionId ((JSFunction *) priv->function));
#endif

    ret = JS_CallFunctionValue (priv->native_context,
                                JS_GetGlobalObject (priv->native_context),
                                priv->function,
                                1, argv,
                                &ret_val);
    if (G_UNLIKELY (ret == JS_FALSE))
        g_warning (G_STRLOC ": error when calling listener");
}

static void
dax_xml_event_listener_init (DaxXmlEventListenerIface *iface)
{
    iface->handle_event = dax_js_function_listener_handle_event;
}

/*
 * GObject implementation
 */

static void
dax_js_function_listener_get_property (GObject    *object,
                                       guint       property_id,
                                       GValue     *value,
                                       GParamSpec *pspec)
{
    DaxJsFunctionListener *listener = (DaxJsFunctionListener *) object;

    switch (property_id)
    {
    case PROP_FUNCTION:
        g_value_set_pointer (value,
                             dax_js_function_listener_get_function (listener));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_js_function_listener_set_property (GObject      *object,
                                       guint         property_id,
                                       const GValue *value,
                                       GParamSpec   *pspec)
{
    DaxJsFunctionListener *listener = (DaxJsFunctionListener *) object;

    switch (property_id)
    {
    case PROP_FUNCTION:
        dax_js_function_listener_set_function (listener,
                                               g_value_get_pointer (value));
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_js_function_listener_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_js_function_listener_parent_class)->dispose (object);
}

static void
dax_js_function_listener_class_init (DaxJsFunctionListenerClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxJsFunctionListenerPrivate));

    object_class->get_property = dax_js_function_listener_get_property;
    object_class->set_property = dax_js_function_listener_set_property;
    object_class->dispose = dax_js_function_listener_dispose;

    pspec = g_param_spec_pointer ("function",
                                  "Function",
                                  "JS callback of the listener",
                                  DAX_GPARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_FUNCTION, pspec);
}

static void
dax_js_function_listener_init (DaxJsFunctionListener *self)
{
    DaxJsFunctionListenerPrivate *priv;

    self->priv = priv = JS_FUNCTION_LISTENER_PRIVATE (self);

    priv->js_context = dax_js_context_get_default ();
    priv->native_context = dax_js_context_get_native_context (priv->js_context);
}

DaxJsFunctionListener *
dax_js_function_listener_new (DaxJsFunction *function)
{
    return g_object_new (DAX_TYPE_JS_FUNCTION_LISTENER,
                         "function", function,
                         NULL);
}
