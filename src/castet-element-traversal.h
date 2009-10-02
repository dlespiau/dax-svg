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

#ifndef __CASTET_ELEMENT_TRAVERSAL_H__
#define __CASTET_ELEMENT_TRAVERSAL_H__

#include <glib-object.h>

G_BEGIN_DECLS

#include "castet-dom-forward.h"
#include "castet-dom-element.h"

#define CASTET_TYPE_ELEMENT_TRAVERSAL (castet_element_traversal_get_type())

#define CASTET_ELEMENT_TRAVERSAL(obj)                           \
    (G_TYPE_CHECK_INSTANCE_CAST((obj),                          \
                                CASTET_TYPE_ELEMENT_TRAVERSAL,  \
                                CastetElementTraversal))

#define CASTET_IS_ELEMENT_TRAVERSAL(obj)    \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj),      \
                                CASTET_TYPE_ELEMENT_TRAVERSAL))

#define CASTET_ELEMENT_TRAVERSAL_GET_INTERFACE(obj)                 \
    (G_TYPE_INSTANCE_GET_INTERFACE((obj),                           \
                                   CASTET_TYPE_ELEMENT_TRAVERSAL,   \
                                   CastetElementTraversalIface))

typedef struct _CastetElementTraversal           CastetElementTraversal;
typedef struct _CastetElementTraversalIface      CastetElementTraversalIface;

/**
 * CastetElementTraversalIface:
 *
 * #CastetElementTraversal interface
 */
struct _CastetElementTraversalIface
{
    /*< private >*/
    GTypeInterface parent_iface;

    CastetDomElement *(* get_first_element_child)(CastetElementTraversal *self);
    CastetDomElement *(* get_last_element_child)(CastetElementTraversal *self);
    CastetDomElement *(* get_next_element_sibling)(CastetElementTraversal *self);
    CastetDomElement *(* get_previous_element_sibling)(CastetElementTraversal *self);
    gulong (* get_child_element_count)(CastetElementTraversal *self);
};

CastetDomElement*
castet_element_traversal_get_first_element_child     (CastetElementTraversal *self);
CastetDomElement*
castet_element_traversal_get_last_element_child      (CastetElementTraversal *self);
CastetDomElement*
castet_element_traversal_get_next_element_sibling    (CastetElementTraversal *self);
CastetDomElement*
castet_element_traversal_get_previous_element_sibling(CastetElementTraversal *self);
gulong
castet_element_traversal_get_child_element_count     (CastetElementTraversal *self);

GType       castet_element_traversal_get_type    (void) G_GNUC_CONST;

G_END_DECLS

#endif /* __CASTET_ELEMENT_TRAVERSAL_H__ */
