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

#include "dax-desc-element.h"

G_DEFINE_TYPE (DaxDescElement, dax_desc_element, DAX_TYPE_ELEMENT)

#define DESC_ELEMENT_PRIVATE(o)                                 \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      DAX_TYPE_DESC_ELEMENT, \
                                      DaxDescElementPrivate))

struct _DaxDescElementPrivate
{
};

static void
dax_desc_element_get_property (GObject    *object,
                                  guint       property_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_desc_element_set_property (GObject      *object,
                                  guint         property_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_desc_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_desc_element_parent_class)->dispose (object);
}

static void
dax_desc_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_desc_element_parent_class)->finalize (object);
}

static void
dax_desc_element_class_init (DaxDescElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    /* g_type_class_add_private (klass, sizeof (DaxDescElementPrivate)); */

    object_class->get_property = dax_desc_element_get_property;
    object_class->set_property = dax_desc_element_set_property;
    object_class->dispose = dax_desc_element_dispose;
    object_class->finalize = dax_desc_element_finalize;
}

static void
dax_desc_element_init (DaxDescElement *self)
{
    /* self->priv = DESC_ELEMENT_PRIVATE (self); */
}

DaxDomElement *
dax_desc_element_new (void)
{
    return g_object_new (DAX_TYPE_DESC_ELEMENT, NULL);
}
