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

#include "castet-element.h"

G_DEFINE_ABSTRACT_TYPE (CastetElement, castet_element, CASTET_TYPE_DOM_ELEMENT)

#define ELEMENT_PRIVATE(o) \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o), CASTET_TYPE_ELEMENT, CastetElementPrivate))

struct _CastetElementPrivate
{
};

/*
 * CastetDomElement overloading
 */

static const gchar *
castet_element_get_attribute(CastetDomElement *self,
                             const gchar      *name)
{
#if 0
    GObjectClass *object_class= G_OBJECT_GET_CLASS (self);
    GParamSpec *pspec;
#endif
    return NULL;
}

static void
castet_element_set_attribute (CastetDomElement  *self,
                              const gchar       *name,
                              const gchar       *value,
                              GError           **err)
{
    GObjectClass *object_class= G_OBJECT_GET_CLASS (self);
    GParamSpec *pspec;
    GValue string_value = { 0, };
    GValue new_value = { 0, };

    pspec = g_object_class_find_property (object_class, name);
    if (pspec == NULL) {
        /* FIXME exception */
        g_message ("Unsupported attribute %s for %s",
                   name,
                   G_OBJECT_CLASS_NAME (object_class));
        return;
    }

    /* we don't want to duplicate the string here */
    g_value_init (&string_value, G_TYPE_STRING);
    g_value_set_static_string (&string_value, value);

    /* this GValue holds the new value of the property we want to set */
    g_value_init (&new_value, pspec->value_type);

    if (g_value_transform (&string_value, &new_value) == FALSE) {
        /* FIXME exception ? */
        g_warning ("Could not transform a string into a %s",
                   g_type_name (pspec->value_type));
        return;
    }

    g_object_set_property (G_OBJECT (self), name, &new_value);
}

/*
 * GObject overloading
 */

static void
castet_element_get_property (GObject    *object,
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
castet_element_set_property (GObject      *object,
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
castet_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_element_parent_class)->dispose (object);
}

static void
castet_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_element_parent_class)->finalize (object);
}

static void
castet_element_class_init (CastetElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    CastetDomElementClass *dom_element_class = CASTET_DOM_ELEMENT_CLASS (klass);

    /* g_type_class_add_private (klass, sizeof (CastetElementPrivate)); */

    object_class->get_property = castet_element_get_property;
    object_class->set_property = castet_element_set_property;
    object_class->dispose = castet_element_dispose;
    object_class->finalize = castet_element_finalize;

    dom_element_class->get_attribute = castet_element_get_attribute;
    dom_element_class->set_attribute = castet_element_set_attribute;
}

static void
castet_element_init (CastetElement *self)
{
    /* self->priv = ELEMENT_PRIVATE (self); */
}

CastetDomElement *
castet_element_new (void)
{
    return g_object_new (CASTET_TYPE_ELEMENT, NULL);
}
