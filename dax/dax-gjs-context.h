/*
 * Dax - Load and draw SVG
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
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined(__DAX_H_INSIDE__) && !defined(DAX_COMPILATION)
#error "Only <dax/dax.h> can be included directly."
#endif

#ifndef __DAX_GJS_CONTEXT_H__
#define __DAX_GJS_CONTEXT_H__

#include <glib-object.h>

#include "dax-dom.h"

#include "dax-xml-event.h"

G_BEGIN_DECLS

#define DAX_TYPE_JS_CONTEXT dax_js_context_get_type()

#define DAX_JS_CONTEXT(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), DAX_TYPE_JS_CONTEXT, DaxJsContext))

#define DAX_JS_CONTEXT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), DAX_TYPE_JS_CONTEXT, DaxJsContextClass))

#define DAX_IS_JS_CONTEXT(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_JS_CONTEXT))

#define DAX_IS_JS_CONTEXT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_JS_CONTEXT))

#define DAX_JS_CONTEXT_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), DAX_TYPE_JS_CONTEXT, DaxJsContextClass))

typedef struct _DaxJsContextClass DaxJsContextClass;
typedef struct _DaxJsContextPrivate DaxJsContextPrivate;

typedef void DaxJsObject;
typedef void DaxJsFunction;

struct _DaxJsContext
{
    GObject parent;

    DaxJsContextPrivate *priv;
};

struct _DaxJsContextClass
{
    GObjectClass parent_class;
};

GType           dax_js_context_get_type             (void) G_GNUC_CONST;

DaxJsContext*   dax_js_context_new                  (void);
void *          dax_js_context_get_gjs_context      (DaxJsContext *context);
void *          dax_js_context_get_native_context   (DaxJsContext *context);
gboolean        dax_js_context_eval                 (DaxJsContext  *context,
                                                     const char    *script,
                                                     gssize         length,
                                                     const char    *file,
                                                     gint          *retval,
                                                     GError       **error);

gboolean        dax_js_context_call_function    (DaxJsContext *context,
                                                 const char   *name,
                                                 const char   *nb_args,
                                                 ...);

DaxJsObject*    dax_js_context_new_object_from_gobject   (DaxJsContext *context,
                                                          GObject     *object);
DaxJsObject*    dax_js_context_new_object_from_xml_event (DaxJsContext *context,
                                                          DaxXmlEvent  *event);

gboolean        dax_js_context_add_global_object    (DaxJsContext *context,
                                                     const gchar  *name,
                                                     DaxJsObject  *object);

G_END_DECLS

#endif /* __DAX_GJS_CONTEXT_H__ */
