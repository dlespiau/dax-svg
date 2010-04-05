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

#include "dax-element-tspan.h"

G_DEFINE_TYPE (DaxElementTspan, dax_element_tspan, DAX_TYPE_ELEMENT)

#define ELEMENT_TSPAN_PRIVATE(o)                        \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o),                    \
                                DAX_TYPE_ELEMENT_TSPAN, \
                                DaxElementTspanPrivate))

struct _DaxElementTspanPrivate
{
};


static void
dax_element_tspan_get_property (GObject    *object,
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
dax_element_tspan_set_property (GObject      *object,
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
dax_element_tspan_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_element_tspan_parent_class)->dispose (object);
}

static void
dax_element_tspan_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_element_tspan_parent_class)->finalize (object);
}

static void
dax_element_tspan_class_init (DaxElementTspanClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

#if 0
    g_type_class_add_private (klass, sizeof (DaxElementTspanPrivate));
#endif

    object_class->get_property = dax_element_tspan_get_property;
    object_class->set_property = dax_element_tspan_set_property;
    object_class->dispose = dax_element_tspan_dispose;
    object_class->finalize = dax_element_tspan_finalize;
}

static void
dax_element_tspan_init (DaxElementTspan *self)
{
#if 0
    self->priv = ELEMENT_TSPAN_PRIVATE (self);
#endif
}

DaxDomElement *
dax_element_tspan_new (void)
{
    return g_object_new (DAX_TYPE_ELEMENT_TSPAN, NULL);
}
