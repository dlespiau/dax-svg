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

#ifndef __CASTET_HANDLER_ELEMENT_H__
#define __CASTET_HANDLER_ELEMENT_H__

#include <glib-object.h>

#include "castet-element.h"

G_BEGIN_DECLS

#define CASTET_TYPE_HANDLER_ELEMENT castet_handler_element_get_type()

#define CASTET_HANDLER_ELEMENT(obj)                             \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                         \
                                 CASTET_TYPE_HANDLER_ELEMENT,   \
                                 CastetHandlerElement))

#define CASTET_HANDLER_ELEMENT_CLASS(klass)                 \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                      \
                              CASTET_TYPE_HANDLER_ELEMENT,  \
                              CastetHandlerElementClass))

#define CASTET_IS_HANDLER_ELEMENT(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CASTET_TYPE_HANDLER_ELEMENT))

#define CASTET_IS_HANDLER_ELEMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), CASTET_TYPE_HANDLER_ELEMENT))

#define CASTET_HANDLER_ELEMENT_GET_CLASS(obj)                   \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                          \
                                CASTET_TYPE_HANDLER_ELEMENT,    \
                                CastetHandlerElementClass))

typedef struct _CastetHandlerElement CastetHandlerElement;
typedef struct _CastetHandlerElementClass CastetHandlerElementClass;
typedef struct _CastetHandlerElementPrivate CastetHandlerElementPrivate;

struct _CastetHandlerElement
{
    CastetElement parent;

    CastetHandlerElementPrivate *priv;
};

struct _CastetHandlerElementClass
{
    CastetElementClass parent_class;
};

GType               castet_handler_element_get_type     (void) G_GNUC_CONST;

CastetDomElement *  castet_handler_element_new          (void);
CastetDomElement *  castet_handler_element_get_target   (CastetHandlerElement *handler);
gchar *             castet_handler_element_get_code     (const CastetHandlerElement *handler);

G_END_DECLS

#endif /* __CASTET_HANDLER_ELEMENT_H__ */
