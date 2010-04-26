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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __DAX_ELEMENT_SCRIPT_H__
#define __DAX_ELEMENT_SCRIPT_H__

#include <glib-object.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_ELEMENT_SCRIPT dax_element_script_get_type()

#define DAX_ELEMENT_SCRIPT(obj)                             \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 DAX_TYPE_ELEMENT_SCRIPT,   \
                                 DaxElementScript))

#define DAX_ELEMENT_SCRIPT_CLASS(klass)                 \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              DAX_TYPE_ELEMENT_SCRIPT,  \
                              DaxElementScriptClass))

#define DAX_IS_ELEMENT_SCRIPT(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_ELEMENT_SCRIPT))

#define DAX_IS_ELEMENT_SCRIPT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_ELEMENT_SCRIPT))

#define DAX_ELEMENT_SCRIPT_GET_CLASS(obj)                   \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_ELEMENT_SCRIPT,    \
                                DaxElementScriptClass))

typedef struct _DaxElementScript DaxElementScript;
typedef struct _DaxElementScriptClass DaxElementScriptClass;
typedef struct _DaxElementScriptPrivate DaxElementScriptPrivate;

#define DAX_SCRIPT_TYPE_OVERRIDE_TRANSFORM

typedef enum /*< prefix=DAX_SCRIPT_TYPE >*/
{
    DAX_SCRIPT_TYPE_ECMASCRIPT,
    DAX_SCRIPT_TYPE_JAVA_ARCHIVE
} DaxScriptType;

struct _DaxElementScript
{
    DaxElement parent;

    DaxElementScriptPrivate *priv;
};

struct _DaxElementScriptClass
{
    DaxElementClass parent_class;
};

GType               dax_element_script_get_type          (void) G_GNUC_CONST;

DaxDomElement *     dax_element_script_new               (void);
DaxScriptType       dax_element_script_get_script_type   (const DaxElementScript *script);
const gchar *       dax_element_script_get_code          (const DaxElementScript *script);

G_END_DECLS

#endif /* __DAX_ELEMENT_SCRIPT_H__ */
