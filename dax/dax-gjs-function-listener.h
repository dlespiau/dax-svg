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

#if !defined(__DAX_H_INSIDE__) && !defined(DAX_COMPILATION)
#error "Only <dax/dax.h> can be included directly."
#endif

#ifndef __DAX_JS_FUNCTION_LISTENER_H__
#define __DAX_JS_FUNCTION_LISTENER_H__

#include <glib-object.h>

#include "dax-gjs-context.h"

G_BEGIN_DECLS

#define DAX_TYPE_JS_FUNCTION_LISTENER dax_js_function_listener_get_type()

#define DAX_JS_FUNCTION_LISTENER(obj)                           \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                         \
                                 DAX_TYPE_JS_FUNCTION_LISTENER, \
                                 DaxJsFunctionListener))

#define DAX_JS_FUNCTION_LISTENER_CLASS(klass)                   \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                          \
                              DAX_TYPE_JS_FUNCTION_LISTENER,    \
                              DaxJsFunctionListenerClass))

#define DAX_IS_JS_FUNCTION_LISTENER(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_JS_FUNCTION_LISTENER))

#define DAX_IS_JS_FUNCTION_LISTENER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_JS_FUNCTION_LISTENER))

#define DAX_JS_FUNCTION_LISTENER_GET_CLASS(obj)                 \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                          \
                                DAX_TYPE_JS_FUNCTION_LISTENER,  \
                                DaxJsFunctionListenerClass))

typedef struct _DaxJsFunctionListener DaxJsFunctionListener;
typedef struct _DaxJsFunctionListenerClass DaxJsFunctionListenerClass;
typedef struct _DaxJsFunctionListenerPrivate DaxJsFunctionListenerPrivate;

struct _DaxJsFunctionListener
{
    GObject parent;

    DaxJsFunctionListenerPrivate *priv;
};

struct _DaxJsFunctionListenerClass
{
    GObjectClass parent_class;
};

GType                   dax_js_function_listener_get_type   (void) G_GNUC_CONST;

DaxJsFunctionListener * dax_js_function_listener_new        (DaxJsContext  *context,
                                                             DaxJsFunction *function);

G_END_DECLS

#endif /* __DAX_JS_FUNCTION_LISTENER_H__ */
