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
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * SECTION: dax-element-traversal
 * @short_description: FIXME
 *
 * This interface implements the Element Traversal Specification
 * http://www.w3.org/TR/ElementTraversal
 */

#include "dax-element-traversal.h"

DaxDomElement*
dax_element_traversal_get_first_element_child (DaxElementTraversal *self)
{
    DaxElementTraversalIface *iface;

    g_return_val_if_fail(DAX_IS_ELEMENT_TRAVERSAL(self), NULL);

    iface = DAX_ELEMENT_TRAVERSAL_GET_INTERFACE(self);

    if (iface->get_first_element_child)
        return iface->get_first_element_child(self);

    return NULL;
}

DaxDomElement*
dax_element_traversal_get_last_element_child (DaxElementTraversal *self)
{
    DaxElementTraversalIface *iface;

    g_return_val_if_fail(DAX_IS_ELEMENT_TRAVERSAL(self), NULL);

    iface = DAX_ELEMENT_TRAVERSAL_GET_INTERFACE(self);

    if (iface->get_last_element_child)
        return iface->get_last_element_child(self);

    return NULL;
}

DaxDomElement*
dax_element_traversal_get_next_element_sibling (DaxElementTraversal *self)
{
    DaxElementTraversalIface *iface;

    g_return_val_if_fail(DAX_IS_ELEMENT_TRAVERSAL(self), NULL);

    iface = DAX_ELEMENT_TRAVERSAL_GET_INTERFACE(self);

    if (iface->get_next_element_sibling)
        return iface->get_next_element_sibling(self);

    return NULL;
}

DaxDomElement*
dax_element_traversal_get_previous_element_sibling(DaxElementTraversal *self)
{
    DaxElementTraversalIface *iface;

    g_return_val_if_fail(DAX_IS_ELEMENT_TRAVERSAL(self), NULL);

    iface = DAX_ELEMENT_TRAVERSAL_GET_INTERFACE(self);

    if (iface->get_previous_element_sibling)
        return iface->get_previous_element_sibling(self);

    return NULL;
}

gulong
dax_element_traversal_get_child_element_count(DaxElementTraversal *self)
{
    DaxElementTraversalIface *iface;

    g_return_val_if_fail(DAX_IS_ELEMENT_TRAVERSAL(self), 0);

    iface = DAX_ELEMENT_TRAVERSAL_GET_INTERFACE(self);

    if (iface->get_child_element_count)
        return iface->get_child_element_count(self);

    return 0;
}


/*
 * Interface management
 */

GType dax_element_traversal_get_type(void)
{
    static GType element_traversal_type = 0;

    if (!element_traversal_type)
        element_traversal_type =
            g_type_register_static_simple(G_TYPE_INTERFACE,
                                          "DaxElementTraversal",
                                          sizeof(DaxElementTraversalIface),
                                          NULL, 0, NULL, 0);

    return element_traversal_type;
}

