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
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdarg.h>
#include <string.h>

#include <gjs/gjs.h>
#include <gjs/gi/object.h>

#include "dax-js-context.h"

G_DEFINE_TYPE (DaxJsContext, dax_js_context, G_TYPE_OBJECT)

#define JS_CONTEXT_PRIVATE(o)                               \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      DAX_TYPE_JS_CONTEXT,  \
                                      DaxJsContextPrivate))

struct _DaxJsContextPrivate
{
    GjsContext *gjs_context;
    JSContext *js_context;

    JSObject *xml_event_prototype;
};

static void
js_xml_event_finalize (JSContext *context,
                       JSObject  *obj)
{
    //g_message (G_STRLOC ": not implemented");
}

static struct JSClass js_xml_event_class = {
    "DaxXmlEvent",
    JSCLASS_HAS_PRIVATE,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_EnumerateStub,
    JS_ResolveStub,
    JS_ConvertStub,
    js_xml_event_finalize,
    JSCLASS_NO_OPTIONAL_MEMBERS,
};

static JSPropertySpec js_xml_event_proto_props[] = {
    { NULL }
};

static JSBool
js_xml_event_to_string (JSContext *context,
                        JSObject  *obj,
                        uintN      argc,
                        jsval     *argv,
                        jsval     *retval)
{
    g_message (G_STRLOC ": not implemented");

    *retval = JSVAL_VOID;

    return JS_TRUE;
}

static JSFunctionSpec js_xml_event_proto_funcs[] = {
    { "toString", js_xml_event_to_string, 0, 0}, /* debugging purpose */
    { NULL }
};

static void
dax_js_context_get_property (GObject    *object,
                             guint       property_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_js_context_set_property (GObject      *object,
                             guint         property_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_js_context_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_js_context_parent_class)->dispose (object);
}

static void
dax_js_context_finalize (GObject *object)
{
    DaxJsContext *context = DAX_JS_CONTEXT (object);
    DaxJsContextPrivate *priv = context->priv;

    g_object_unref (priv->gjs_context);

    G_OBJECT_CLASS (dax_js_context_parent_class)->finalize (object);
}

static void
dax_js_context_class_init (DaxJsContextClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (DaxJsContextPrivate));

    object_class->get_property = dax_js_context_get_property;
    object_class->set_property = dax_js_context_set_property;
    object_class->dispose = dax_js_context_dispose;
    object_class->finalize = dax_js_context_finalize;
}

static void
dax_js_context_init (DaxJsContext *self)
{
    DaxJsContextPrivate *priv;
    JSObject *xml_event_prototype;
    static const gchar *import = "const Dax = imports.gi.Dax";

    self->priv = priv = JS_CONTEXT_PRIVATE (self);

    priv->gjs_context = gjs_context_new ();
    priv->js_context =
        (JSContext *) gjs_context_get_native_context (priv->gjs_context);

    /* import Dax typelib */
    gjs_context_eval (priv->gjs_context, import, 26, "dax", NULL, NULL);

    xml_event_prototype = JS_InitClass (priv->js_context,
                                        JS_GetGlobalObject (priv->js_context),
                                        NULL,   /* parent proto */
                                        &js_xml_event_class,
                                        NULL, 0,
                                        js_xml_event_proto_props,
                                        js_xml_event_proto_funcs,
                                        NULL,
                                        NULL);

    g_assert (xml_event_prototype != NULL);
    priv->xml_event_prototype = xml_event_prototype;
}

DaxJsContext *
dax_js_context_new (void)
{
    return g_object_new (DAX_TYPE_JS_CONTEXT, NULL);
}

DaxJsContext *
dax_js_context_get_default (void)
{
    static DaxJsContext *context = NULL;

    if (G_UNLIKELY (context == NULL))
        context = dax_js_context_new ();

    return context;
}

void *
dax_js_context_get_gjs_context (DaxJsContext *context)
{
    g_return_val_if_fail (DAX_IS_JS_CONTEXT (context), NULL);

    return context->priv->gjs_context;
}

void *
dax_js_context_get_native_context (DaxJsContext *context)
{
    g_return_val_if_fail (DAX_IS_JS_CONTEXT (context), NULL);

    return context->priv->js_context;
}

gboolean
dax_js_context_eval (DaxJsContext  *context,
                     const char    *script,
                     gssize         length,
                     const char    *file,
                     gint          *retval,
                     GError       **error)
{
    DaxJsContextPrivate *priv;

    g_return_val_if_fail (DAX_IS_JS_CONTEXT (context), FALSE);

    priv = context->priv;
    return gjs_context_eval (priv->gjs_context,
                             script,
                             -1,
                             file,
                             retval,
                             error);
}

DaxJsObject*
dax_js_context_new_object_from_gobject (DaxJsContext *context,
                                        GObject      *object)
{
    g_return_val_if_fail (DAX_IS_JS_CONTEXT (context), NULL);

    return gjs_object_from_g_object (context->priv->js_context, object);
}

DaxJsObject *
dax_js_context_new_object_from_xml_event (DaxJsContext *context,
                                          DaxXmlEvent  *xml_event)
{
    DaxJsContextPrivate *priv;
    JSObject *event;
    JSObject *target;
    jsval event_jsval;
    JSBool retval;

    g_return_val_if_fail (DAX_IS_JS_CONTEXT (context), NULL);

    priv = context->priv;
    event = JS_NewObject (priv->js_context,
                          &js_xml_event_class,
                          priv->xml_event_prototype,
                          NULL);
    g_assert (event != NULL);

    target = gjs_object_from_g_object (priv->js_context,
                                       G_OBJECT (xml_event->any.target));
    g_assert (target != NULL);

    event_jsval = OBJECT_TO_JSVAL (target);
    retval = JS_SetProperty (priv->js_context,
                             event,
                             "target",
                             &event_jsval);
    g_assert (retval == JS_TRUE);

    return event;
}

/*
 * FIXME: * best way to make such a function work with 2 backends?
 *        * return value
 */

gboolean
dax_js_context_call_function (DaxJsContext *context,
                              const char   *name,
                              const char   *format,
                              ...)
{
    DaxJsContextPrivate *priv;
    va_list args;
    JSBool ok;
    jsval *argv;
    void *mark;
    jsval retval;

    g_return_val_if_fail (DAX_IS_JS_CONTEXT (context), FALSE);

    priv = context->priv;

    va_start (args, format);

    argv = JS_PushArgumentsVA (priv->js_context, &mark, format, args);
    if (G_UNLIKELY (argv == NULL)) {
        g_warning (G_STRLOC ": could not push arguments onto the stack");
        va_end (args);
        return FALSE;
    }

    ok = JS_CallFunctionName (priv->js_context,
                              JS_GetGlobalObject (priv->js_context),
                              name,
                              strlen (format),
                              argv,
                              &retval);
    JS_PopArguments (priv->js_context, mark);
    if (!ok)
        gjs_log_exception (priv->js_context, NULL);

    va_end (args);

    return ok;
}

gboolean
dax_js_context_add_global_object (DaxJsContext *context,
                                  const char   *name,
                                  DaxJsObject  *object)
{
    DaxJsContextPrivate *priv;
    JSObject *js_object = (JSObject *) object;
    jsval js_val;

    g_return_val_if_fail (DAX_IS_JS_CONTEXT (context), FALSE);

    priv = context->priv;
    js_val = OBJECT_TO_JSVAL (js_object);

    return JS_SetProperty (priv->js_context,
                           JS_GetGlobalObject (priv->js_context),
                           name,
                           &js_val);
}
