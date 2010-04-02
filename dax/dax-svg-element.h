/*
 * Dax - Load and draw SVG
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

#ifndef __DAX_SVG_ELEMENT_H__
#define __DAX_SVG_ELEMENT_H__

#include <glib-object.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_SVG_ELEMENT dax_svg_element_get_type()

#define DAX_SVG_ELEMENT(obj)                             \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 DAX_TYPE_SVG_ELEMENT,   \
                                 DaxSvgElement))

#define DAX_SVG_ELEMENT_CLASS(klass)                 \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              DAX_TYPE_SVG_ELEMENT,  \
                              DaxSvgElementClass))

#define DAX_IS_SVG_ELEMENT(obj)      \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 DAX_TYPE_SVG_ELEMENT))

#define DAX_IS_SVG_ELEMENT_CLASS(klass)  \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),      \
                              DAX_TYPE_SVG_ELEMENT))

#define DAX_SVG_ELEMENT_GET_CLASS(obj)                   \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_SVG_ELEMENT,    \
                                DaxSvgElementClass))

typedef struct _DaxSvgElement DaxSvgElement;
typedef struct _DaxSvgElementClass DaxSvgElementClass;
typedef struct _DaxSvgElementPrivate DaxSvgElementPrivate;

#define DAX_SVG_VERSION_OVERRIDE_TRANSFORM
typedef enum
{
    DAX_SVG_VERSION_1_0,
    DAX_SVG_VERSION_1_1,
    DAX_SVG_VERSION_1_2,
} DaxSvgVersion;

#define DAX_SVG_BASE_PROFILE_DEFAULT     DAX_SVG_BASE_PROFILE_NONE
typedef enum
{
    DAX_SVG_BASE_PROFILE_NONE,
    DAX_SVG_BASE_PROFILE_FULL,
    DAX_SVG_BASE_PROFILE_BASIC,
    DAX_SVG_BASE_PROFILE_TINY
} DaxSvgBaseProfile;

struct _DaxSvgElement
{
    DaxElement parent;

    DaxSvgElementPrivate *priv;
};

struct _DaxSvgElementClass
{
    DaxElementClass parent_class;
};

GType dax_svg_element_get_type (void) G_GNUC_CONST;

DaxDomElement *dax_svg_element_new (void);

G_END_DECLS

#endif /* __DAX_SVG_ELEMENT_H__ */
