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

#ifndef __CASTET_SCRIPT_ELEMENT_H__
#define __CASTET_SCRIPT_ELEMENT_H__

#include <glib-object.h>

#include "castet-element.h"

G_BEGIN_DECLS

#define CASTET_TYPE_SCRIPT_ELEMENT castet_script_element_get_type()

#define CASTET_SCRIPT_ELEMENT(obj)                              \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                         \
                                 CASTET_TYPE_SCRIPT_ELEMENT,    \
                                 CastetScriptElement))

#define CASTET_SCRIPT_ELEMENT_CLASS(klass)                  \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                      \
                              CASTET_TYPE_SCRIPT_ELEMENT,   \
                              CastetScriptElementClass))

#define CASTET_IS_SCRIPT_ELEMENT(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CASTET_TYPE_SCRIPT_ELEMENT))

#define CASTET_IS_SCRIPT_ELEMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), CASTET_TYPE_SCRIPT_ELEMENT))

#define CASTET_SCRIPT_ELEMENT_GET_CLASS(obj)                \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                CASTET_TYPE_SCRIPT_ELEMENT, \
                                CastetScriptElementClass))

typedef struct _CastetScriptElement CastetScriptElement;
typedef struct _CastetScriptElementClass CastetScriptElementClass;
typedef struct _CastetScriptElementPrivate CastetScriptElementPrivate;

#define CASTET_SCRIPT_TYPE_DEFAULT      CASTET_SCRIPT_TYPE_ECMASCRIPT

typedef enum /*< prefix=CASTET_SCRIPT_TYPE >*/
{
    CASTET_SCRIPT_TYPE_ECMASCRIPT,      /*< nick=application/ecmascript >*/
    CASTET_SCRIPT_TYPE_JAVA_ARCHIVE     /*< nick=application/java-archive >*/
} CastetScriptType;

struct _CastetScriptElement
{
    CastetElement parent;

    CastetScriptElementPrivate *priv;
};

struct _CastetScriptElementClass
{
    CastetElementClass parent_class;
};

GType               castet_script_element_get_type          (void) G_GNUC_CONST;

CastetDomElement *  castet_script_element_new               (void);
CastetScriptType    castet_script_element_get_script_type   (const CastetScriptElement *script);
const gchar *       castet_script_element_get_code          (const CastetScriptElement *script);

G_END_DECLS

#endif /* __CASTET_SCRIPT_ELEMENT_H__ */
