/* dax-element-text.h */
/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
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

#if !defined(__DAX_H_INSIDE__) && !defined(DAX_COMPILATION)
#error "Only <dax/dax.h> can be included directly."
#endif

#ifndef __DAX_ELEMENT_TEXT_H__
#define __DAX_ELEMENT_TEXT_H__

#include <glib-object.h>

#include "dax-element.h"

G_BEGIN_DECLS

#define DAX_TYPE_ELEMENT_TEXT dax_element_text_get_type()

#define DAX_ELEMENT_TEXT(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), DAX_TYPE_ELEMENT_TEXT, DaxElementText))

#define DAX_ELEMENT_TEXT_CLASS(klass)                   \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              DAX_TYPE_ELEMENT_TEXT,    \
                              DaxElementTextClass))

#define DAX_IS_ELEMENT_TEXT(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_ELEMENT_TEXT))

#define DAX_IS_ELEMENT_TEXT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_ELEMENT_TEXT))

#define DAX_ELEMENT_TEXT_GET_CLASS(obj)                 \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                  \
                                DAX_TYPE_ELEMENT_TEXT,  \
                                DaxElementTextClass))

typedef struct _DaxElementText DaxElementText;
typedef struct _DaxElementTextClass DaxElementTextClass;
typedef struct _DaxElementTextPrivate DaxElementTextPrivate;

#define DAX_TEXT_EDITABLE_DEFAULT       DAX_TEXT_EDITABLE_NONE
typedef enum
{
    DAX_TEXT_EDITABLE_NONE,
    DAX_TEXT_EDITABLE_SIMPLE
} DaxTextEditable;

struct _DaxElementText
{
    DaxElement parent;

    DaxElementTextPrivate *priv;
};

struct _DaxElementTextClass
{
    DaxElementClass parent_class;
};

GType           dax_element_text_get_type       (void) G_GNUC_CONST;

DaxDomElement * dax_element_text_new            (void);
GArray *        dax_element_text_get_x          (const DaxElementText *text);
GArray *        dax_element_text_get_y          (const DaxElementText *text);
DaxTextEditable dax_element_text_get_editable   (const DaxElementText *text);
const GArray *  dax_element_text_get_rotation   (const DaxElementText *text);
gchar *         dax_element_text_get_text       (const DaxElementText *text);

G_END_DECLS

#endif /* __DAX_ELEMENT_TEXT_H__ */
