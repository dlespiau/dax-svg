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

#ifndef __DAX_DOM_ELEMENT_H__
#define __DAX_DOM_ELEMENT_H__

#include <glib-object.h>

#include "dax-dom-forward.h"
#include "dax-dom-node.h"

G_BEGIN_DECLS

#define DAX_TYPE_DOM_ELEMENT dax_dom_element_get_type()

#define DAX_DOM_ELEMENT(obj)                             \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 DAX_TYPE_DOM_ELEMENT,   \
                                 DaxDomElement))

#define DAX_DOM_ELEMENT_CLASS(klass)                 \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              DAX_TYPE_DOM_ELEMENT,  \
                              DaxDomElementClass))

#define DAX_IS_DOM_ELEMENT(obj)      \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 DAX_TYPE_DOM_ELEMENT))

#define DAX_IS_DOM_ELEMENT_CLASS(klass)  \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),      \
                              DAX_TYPE_DOM_ELEMENT))

#define DAX_DOM_ELEMENT_GET_CLASS(obj)                   \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_DOM_ELEMENT,    \
                                DaxDomElementClass))

typedef struct _DaxDomElementClass DaxDomElementClass;
typedef struct _DaxDomElementPrivate DaxDomElementPrivate;

struct _DaxDomElement
{
    DaxDomNode parent;

    DaxDomElementPrivate *priv;
};

struct _DaxDomElementClass
{
    DaxDomNodeClass parent_class;

    const gchar *(*get_attribute)   (DaxDomElement *self,
                                     const gchar      *name);

    void         (*set_attribute)   (DaxDomElement  *self,
                                     const gchar       *name,
                                     const gchar       *value,
                                     GError           **err);
};

GType dax_dom_element_get_type (void) G_GNUC_CONST;

DaxDomElement *dax_dom_element_new (void);

#define dax_dom_element_get_attribute_ns \
    dax_dom_element_getAttributeNS
#define dax_dom_element_set_attribute_ns \
    dax_dom_element_setAttributeNS
#define dax_dom_element_get_attribute \
    dax_dom_element_getAttribute
#define dax_dom_element_set_attribute \
    dax_dom_element_setAttribute

const gchar *dax_dom_element_getAttributeNS  (DaxDomElement  *self,
                                                 const gchar       *ns,
                                                 const gchar       *name,
                                                 GError           **err);
void         dax_dom_element_setAttributeNS  (DaxDomElement  *self,
                                                 const gchar       *ns,
                                                 const gchar       *name,
                                                 const gchar       *value,
                                                 GError           **err);
const gchar *dax_dom_element_getAttribute    (DaxDomElement *self,
                                                 const gchar      *name);
void         dax_dom_element_setAttribute    (DaxDomElement  *self,
                                                 const gchar       *name,
                                                 const gchar       *value,
                                                 GError           **err);
G_END_DECLS

#endif /* __DAX_DOM_ELEMENT_H__ */
