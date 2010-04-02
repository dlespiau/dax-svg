/*
 * Dax - Load and draw SVG
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
/* dax-title-element.c */

#include "dax-title-element.h"

G_DEFINE_TYPE (DaxTitleElement, dax_title_element, DAX_TYPE_ELEMENT)

#define TITLE_ELEMENT_PRIVATE(o)                                    \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                          \
                                      DAX_TYPE_TITLE_ELEMENT,    \
                                      DaxTitleElementPrivate))

struct _DaxTitleElementPrivate
{
};

static void
dax_title_element_get_property (GObject    *object,
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
dax_title_element_set_property (GObject      *object,
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
dax_title_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_title_element_parent_class)->dispose (object);
}

static void
dax_title_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_title_element_parent_class)->finalize (object);
}

static void
dax_title_element_class_init (DaxTitleElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    /* g_type_class_add_private (klass, sizeof (DaxTitleElementPrivate)); */

    object_class->get_property = dax_title_element_get_property;
    object_class->set_property = dax_title_element_set_property;
    object_class->dispose = dax_title_element_dispose;
    object_class->finalize = dax_title_element_finalize;
}

static void
dax_title_element_init (DaxTitleElement *self)
{
    /* self->priv = TITLE_ELEMENT_PRIVATE (self); */
}

DaxDomElement *
dax_title_element_new (void)
{
    return g_object_new (DAX_TYPE_TITLE_ELEMENT, NULL);
}
