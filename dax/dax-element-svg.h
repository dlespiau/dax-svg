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

#ifndef __DAX_ELEMENT_SVG_H__
#define __DAX_ELEMENT_SVG_H__

#include <glib-object.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_ELEMENT_SVG dax_element_svg_get_type()

#define DAX_ELEMENT_SVG(obj)                             \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 DAX_TYPE_ELEMENT_SVG,   \
                                 DaxElementSvg))

#define DAX_ELEMENT_SVG_CLASS(klass)                 \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              DAX_TYPE_ELEMENT_SVG,  \
                              DaxElementSvgClass))

#define DAX_IS_ELEMENT_SVG(obj)      \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 DAX_TYPE_ELEMENT_SVG))

#define DAX_IS_ELEMENT_SVG_CLASS(klass)  \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),      \
                              DAX_TYPE_ELEMENT_SVG))

#define DAX_ELEMENT_SVG_GET_CLASS(obj)                   \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_ELEMENT_SVG,    \
                                DaxElementSvgClass))

typedef struct _DaxElementSvg DaxElementSvg;
typedef struct _DaxElementSvgClass DaxElementSvgClass;
typedef struct _DaxElementSvgPrivate DaxElementSvgPrivate;

#define DAX_SVG_VERSION_OVERRIDE_TRANSFORM
typedef enum
{
    DAX_SVG_VERSION_1_0,
    DAX_SVG_VERSION_1_1,
    DAX_SVG_VERSION_1_2
} DaxSvgVersion;

#define DAX_SVG_BASE_PROFILE_DEFAULT     DAX_SVG_BASE_PROFILE_NONE
typedef enum
{
    DAX_SVG_BASE_PROFILE_NONE,
    DAX_SVG_BASE_PROFILE_FULL,
    DAX_SVG_BASE_PROFILE_BASIC,
    DAX_SVG_BASE_PROFILE_TINY
} DaxSvgBaseProfile;

struct _DaxElementSvg
{
    DaxElement parent;

    DaxElementSvgPrivate *priv;
};

struct _DaxElementSvgClass
{
    DaxElementClass parent_class;
};

GType dax_element_svg_get_type (void) G_GNUC_CONST;

DaxDomElement *dax_element_svg_new (void);

G_END_DECLS

#endif /* __DAX_ELEMENT_SVG_H__ */
