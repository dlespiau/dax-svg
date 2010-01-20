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

#ifndef __CASTET_GJS_CONTEXT_H__
#define __CASTET_GJS_CONTEXT_H__

#include <glib-object.h>

#include "castet-xml-event.h"

G_BEGIN_DECLS

#define CASTET_TYPE_JS_CONTEXT castet_js_context_get_type()

#define CASTET_JS_CONTEXT(obj)                              \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 CASTET_TYPE_JS_CONTEXT,    \
                                 CastetJsContext))

#define CASTET_JS_CONTEXT_CLASS(klass)                  \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              CASTET_TYPE_JS_CONTEXT,   \
                              CastetJsContextClass))

#define CASTET_IS_JS_CONTEXT(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CASTET_TYPE_JS_CONTEXT))

#define CASTET_IS_JS_CONTEXT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), CASTET_TYPE_JS_CONTEXT))

#define CASTET_JS_CONTEXT_GET_CLASS(obj)                \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                CASTET_TYPE_JS_CONTEXT, \
                                CastetJsContextClass))

typedef struct _CastetJsContext CastetJsContext;
typedef struct _CastetJsContextClass CastetJsContextClass;
typedef struct _CastetJsContextPrivate CastetJsContextPrivate;

typedef void CastetJsObject;

struct _CastetJsContext
{
    GObject parent;

    CastetJsContextPrivate *priv;
};

struct _CastetJsContextClass
{
    GObjectClass parent_class;
};

GType castet_js_context_get_type (void) G_GNUC_CONST;

CastetJsContext*    castet_js_context_new           (void);
CastetJsContext*    castet_js_context_get_default   (void);
gboolean            castet_js_context_eval          (CastetJsContext  *context,
                                                     const char       *script,
                                                     gssize            length,
                                                     const char       *file,
                                                     gint             *retval,
                                                     GError          **error);

gboolean            castet_js_context_call_function (CastetJsContext *context,
                                                     const char      *name,
                                                     const char      *nb_args,
                                                     ...);

CastetJsObject*     castet_js_context_new_object_from_xml_event (CastetJsContext *context,
                                                                 CastetXmlEvent  *xml_event);

G_END_DECLS

#endif /* __CASTET_GJS_CONTEXT_H__ */
