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

#ifndef __CASTET_PATH_ELEMENT_H__
#define __CASTET_PATH_ELEMENT_H__

#include <glib-object.h>
#include <clutter/clutter.h>

#include "castet-element.h"

G_BEGIN_DECLS

#define CASTET_TYPE_PATH_ELEMENT castet_path_element_get_type()

#define CASTET_PATH_ELEMENT(obj)        \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                 CASTET_TYPE_PATH_ELEMENT, CastetPathElement))

#define CASTET_PATH_ELEMENT_CLASS(klass)    \
    (G_TYPE_CHECK_CLASS_CAST ((klass),      \
                              CASTET_TYPE_PATH_ELEMENT, CastetPathElementClass))

#define CASTET_IS_PATH_ELEMENT(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CASTET_TYPE_PATH_ELEMENT))

#define CASTET_IS_PATH_ELEMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), CASTET_TYPE_PATH_ELEMENT))

#define CASTET_PATH_ELEMENT_GET_CLASS(obj)                  \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                CASTET_TYPE_PATH_ELEMENT,   \
                                CastetPathElementClass))

typedef struct _CastetPathElement CastetPathElement;
typedef struct _CastetPathElementClass CastetPathElementClass;
typedef struct _CastetPathElementPrivate CastetPathElementPrivate;

struct _CastetPathElement
{
    CastetElement parent;

    CastetPathElementPrivate *priv;
};

struct _CastetPathElementClass
{
    CastetElementClass parent_class;
};

GType               castet_path_element_get_type    (void) G_GNUC_CONST;

CastetDomElement   *castet_path_element_new         (void);
ClutterPath        *castet_path_element_get_path    (CastetPathElement *self);

G_END_DECLS

#endif /* __CASTET_PATH_ELEMENT_H__ */
