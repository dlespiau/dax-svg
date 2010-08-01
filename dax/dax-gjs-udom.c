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
#include <gjs/gi/object.h>

#include "dax-dom.h"

#include "dax-udom-svg-timer.h"

#include "dax-gjs-context.h"
#include "dax-gjs-function-listener.h"
#include "dax-gjs-udom.h"

static JSBool
add_event_listener (JSContext *cx,
                    JSObject  *obj,
                    uintN      argc,
                    jsval     *argv,
                    jsval     *rval)
{
    DaxJsFunctionListener *listener;
    char *type;
    JSFunction *listener_func;
    JSBool use_capture;
    DaxXmlEventTarget *target;
    DaxDomNode *node;
    DaxDomDocument *document;
    DaxJsContext *js_context;

    if (!JS_ConvertArguments (cx, argc, argv,
                              "sfb", &type, &listener_func, &use_capture))
        {
            return JS_FALSE;
        }

    target = DAX_XML_EVENT_TARGET (gjs_g_object_from_object (cx, obj));
    node = DAX_DOM_NODE (target);
    document = node->owner_document;
    js_context = dax_dom_document_get_js_context (document);
    listener = dax_js_function_listener_new (js_context,
                                             (DaxJsFunction *) listener_func);

    dax_xml_event_target_add_event_listener (target,
                                             type,
                                             DAX_XML_EVENT_LISTENER (listener),
                                             use_capture);
    return JS_TRUE;
}

static JSFunctionSpec timer_functions[] = {
    JS_FS ("addEventListener", add_event_listener, 3, 0, 0),
    JS_FS_END
};

static JSBool
create_timer (JSContext *cx,
              JSObject  *obj,
              uintN      argc,
              jsval     *argv,
              jsval     *rval)
{
    int32 initial_interval, repeat_interval;
    DaxSvgTimer *timer;
    JSObject *js_timer;
    JSBool ret;

    if (!JS_ConvertArguments (cx, argc, argv,
                              "ii", &initial_interval, &repeat_interval))
        {
            return JS_FALSE;
        }

    timer = dax_svg_timer_new (initial_interval, repeat_interval);
    js_timer = gjs_object_from_g_object (cx, (GObject *) timer);
    *rval = OBJECT_TO_JSVAL (js_timer);

    ret = JS_DefineFunctions(cx, js_timer, timer_functions);
    if (G_UNLIKELY (ret == JS_FALSE)) {
        g_warning (G_STRLOC ": could not define functions on SVGTimer");
        return JS_FALSE;
    }

    return JS_TRUE;
}

static JSFunctionSpec svg_global_functions[] = {
    JS_FS ("createTimer", create_timer, 2, 0, 0),
    JS_FS_END
};

gboolean
_dax_js_udom_setup_document (DaxJsContext   *context,
                             DaxDomDocument *document)
{
    JSContext *js_context;
    DaxJsObject *js_object;
    JSBool ret;

    g_return_val_if_fail (DAX_IS_JS_CONTEXT (context), FALSE);
    g_return_val_if_fail (DAX_IS_DOM_DOCUMENT (document), FALSE);

    js_context = (JSContext *) dax_js_context_get_native_context (context);

    js_object = dax_js_context_new_object_from_gobject (context,
                                                        (GObject *) document);
    dax_js_context_add_global_object (context, "document", js_object);

    ret = JS_DefineFunctions(js_context,
                             JS_GetGlobalObject (js_context),
                             svg_global_functions);
    if (G_UNLIKELY (ret == JS_FALSE)) {
        g_warning (G_STRLOC ": could not define functions on SVGTimer");
        return FALSE;
    }

    return TRUE;
}

static JSFunctionSpec svg_event_target_functions[] = {
    JS_FS ("addEventListener", add_event_listener, 3, 0, 0),
    JS_FS_END
};

gboolean
_dax_js_udom_setup_element (DaxJsContext  *context,
                            DaxDomElement *element)
{
    JSContext *js_context;

    js_context = (JSContext *) dax_js_context_get_native_context (context);

    if (!JS_DefineFunctions(js_context,
                            JS_GetGlobalObject (js_context),
                            svg_event_target_functions))
    {
        return FALSE;
    }

    return TRUE;
}
