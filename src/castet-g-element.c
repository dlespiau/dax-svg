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

#include "castet-g-element.h"

G_DEFINE_TYPE (CastetGElement, castet_g_element, CASTET_TYPE_ELEMENT)

#define G_ELEMENT_PRIVATE(o)                                    \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      CASTET_TYPE_G_ELEMENT,    \
                                      CastetGElementPrivate))

struct _CastetGElementPrivate
{
};

static void
castet_g_element_get_property (GObject    *object,
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
castet_g_element_set_property (GObject      *object,
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
castet_g_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_g_element_parent_class)->dispose (object);
}

static void
castet_g_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_g_element_parent_class)->finalize (object);
}

static void
castet_g_element_class_init (CastetGElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (CastetGElementPrivate));

    object_class->get_property = castet_g_element_get_property;
    object_class->set_property = castet_g_element_set_property;
    object_class->dispose = castet_g_element_dispose;
    object_class->finalize = castet_g_element_finalize;
}

static void
castet_g_element_init (CastetGElement *self)
{
    self->priv = G_ELEMENT_PRIVATE (self);
}

CastetGElement *
castet_g_element_new (void)
{
    return g_object_new (CASTET_TYPE_G_ELEMENT, NULL);
}
