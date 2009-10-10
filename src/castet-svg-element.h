/*
 * Castet - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
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

#ifndef __CASTET_SVG_ELEMENT_H__
#define __CASTET_SVG_ELEMENT_H__

#include <glib-object.h>

#include "castet-element.h"

G_BEGIN_DECLS

#define CASTET_TYPE_SVG_ELEMENT castet_svg_element_get_type()

#define CASTET_SVG_ELEMENT(obj)                             \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 CASTET_TYPE_SVG_ELEMENT,   \
                                 CastetSvgElement))

#define CASTET_SVG_ELEMENT_CLASS(klass)                 \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              CASTET_TYPE_SVG_ELEMENT,  \
                              CastetSvgElementClass))

#define CASTET_IS_SVG_ELEMENT(obj)      \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 CASTET_TYPE_SVG_ELEMENT))

#define CASTET_IS_SVG_ELEMENT_CLASS(klass)  \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),      \
                              CASTET_TYPE_SVG_ELEMENT))

#define CASTET_SVG_ELEMENT_GET_CLASS(obj)                   \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                CASTET_TYPE_SVG_ELEMENT,    \
                                CastetSvgElementClass))

typedef struct _CastetSvgElement CastetSvgElement;
typedef struct _CastetSvgElementClass CastetSvgElementClass;
typedef struct _CastetSvgElementPrivate CastetSvgElementPrivate;

#define CASTET_SVG_VERSION_DEFAULT      CASTET_SVG_VERSION_1_2
typedef enum
{
    CASTET_SVG_VERSION_1_0,     /*< nick=1.0 >*/
    CASTET_SVG_VERSION_1_1,     /*< nick=1.1 >*/
    CASTET_SVG_VERSION_1_2,     /*< nick=1.2 >*/
} CastetSvgVersion;

#define CASTET_SVG_BASE_PROFILE_DEFAULT     CASTET_SVG_BASE_PROFILE_NONE
typedef enum
{
    CASTET_SVG_BASE_PROFILE_NONE,
    CASTET_SVG_BASE_PROFILE_FULL,
    CASTET_SVG_BASE_PROFILE_BASIC,
    CASTET_SVG_BASE_PROFILE_TINY
} CastetSvgBaseProfile;

struct _CastetSvgElement
{
    CastetElement parent;

    CastetSvgElementPrivate *priv;
};

struct _CastetSvgElementClass
{
    CastetElementClass parent_class;
};

GType castet_svg_element_get_type (void) G_GNUC_CONST;

CastetDomElement *castet_svg_element_new (void);

G_END_DECLS

#endif /* __CASTET_SVG_ELEMENT_H__ */
