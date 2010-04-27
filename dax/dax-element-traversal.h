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

#if !defined(__DAX_H_INSIDE__) && !defined(DAX_COMPILATION)
#error "Only <dax/dax.h> can be included directly."
#endif

#ifndef __DAX_ELEMENT_TRAVERSAL_H__
#define __DAX_ELEMENT_TRAVERSAL_H__

#include <glib-object.h>

G_BEGIN_DECLS

#include "dax-dom-forward.h"
#include "dax-dom-element.h"

#define DAX_TYPE_ELEMENT_TRAVERSAL (dax_element_traversal_get_type())

#define DAX_ELEMENT_TRAVERSAL(obj)                           \
    (G_TYPE_CHECK_INSTANCE_CAST((obj),                          \
                                DAX_TYPE_ELEMENT_TRAVERSAL,  \
                                DaxElementTraversal))

#define DAX_IS_ELEMENT_TRAVERSAL(obj)    \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj),      \
                                DAX_TYPE_ELEMENT_TRAVERSAL))

#define DAX_ELEMENT_TRAVERSAL_GET_INTERFACE(obj)                 \
    (G_TYPE_INSTANCE_GET_INTERFACE((obj),                           \
                                   DAX_TYPE_ELEMENT_TRAVERSAL,   \
                                   DaxElementTraversalIface))

typedef struct _DaxElementTraversal           DaxElementTraversal;
typedef struct _DaxElementTraversalIface      DaxElementTraversalIface;

/**
 * DaxElementTraversalIface:
 *
 * #DaxElementTraversal interface
 */
struct _DaxElementTraversalIface
{
    /*< private >*/
    GTypeInterface parent_iface;

    DaxDomElement *(* get_first_element_child)(DaxElementTraversal *self);
    DaxDomElement *(* get_last_element_child)(DaxElementTraversal *self);
    DaxDomElement *(* get_next_element_sibling)(DaxElementTraversal *self);
    DaxDomElement *(* get_previous_element_sibling)(DaxElementTraversal *self);
    gulong (* get_child_element_count)(DaxElementTraversal *self);
};

DaxDomElement*
dax_element_traversal_get_first_element_child     (DaxElementTraversal *self);
DaxDomElement*
dax_element_traversal_get_last_element_child      (DaxElementTraversal *self);
DaxDomElement*
dax_element_traversal_get_next_element_sibling    (DaxElementTraversal *self);
DaxDomElement*
dax_element_traversal_get_previous_element_sibling(DaxElementTraversal *self);
gulong
dax_element_traversal_get_child_element_count     (DaxElementTraversal *self);

GType       dax_element_traversal_get_type    (void) G_GNUC_CONST;

G_END_DECLS

#endif /* __DAX_ELEMENT_TRAVERSAL_H__ */
