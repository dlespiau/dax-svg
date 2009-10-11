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

#ifndef __CASTET_TITLE_ELEMENT_H__
#define __CASTET_TITLE_ELEMENT_H__

#include <glib-object.h>

#include "castet-element.h"

G_BEGIN_DECLS

#define CASTET_TYPE_TITLE_ELEMENT castet_title_element_get_type()

#define CASTET_TITLE_ELEMENT(obj)                           \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 CASTET_TYPE_TITLE_ELEMENT, \
                                 CastetTitleElement))

#define CASTET_TITLE_ELEMENT_CLASS(klass)                   \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                      \
                              CASTET_TYPE_TITLE_ELEMENT,    \
                              CastetTitleElementClass))

#define CASTET_IS_TITLE_ELEMENT(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CASTET_TYPE_TITLE_ELEMENT))

#define CASTET_IS_TITLE_ELEMENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), CASTET_TYPE_TITLE_ELEMENT))

#define CASTET_TITLE_ELEMENT_GET_CLASS(obj)                 \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                CASTET_TYPE_TITLE_ELEMENT,  \
                                CastetTitleElementClass))

typedef struct _CastetTitleElement CastetTitleElement;
typedef struct _CastetTitleElementClass CastetTitleElementClass;
typedef struct _CastetTitleElementPrivate CastetTitleElementPrivate;

struct _CastetTitleElement
{
    CastetElement parent;

    CastetTitleElementPrivate *priv;
};

struct _CastetTitleElementClass
{
    CastetElementClass parent_class;
};

GType castet_title_element_get_type (void) G_GNUC_CONST;

CastetDomElement *castet_title_element_new (void);

G_END_DECLS

#endif /* __CASTET_TITLE_ELEMENT_H__ */
