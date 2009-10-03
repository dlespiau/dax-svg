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

#include "castet-dom-element.h"

G_DEFINE_ABSTRACT_TYPE (CastetDomElement,
                        castet_dom_element,
                        CASTET_TYPE_DOM_NODE)

#define DOM_ELEMENT_PRIVATE(o)                                  \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      CASTET_TYPE_DOM_ELEMENT,  \
                                      CastetDomElementPrivate))

struct _CastetDomElementPrivate
{
};

const gchar *
castet_dom_element_get_attribute(CastetDomElement *self,
                                 const gchar      *name)
{
    CastetDomElementClass *klass = CASTET_DOM_ELEMENT_GET_CLASS(self);

    g_return_val_if_fail(klass != NULL, NULL);

    if (klass->get_attribute)
        return klass->get_attribute(self, name);

    return NULL;
}

void
castet_dom_element_set_attribute (CastetDomElement  *self,
                                  const char        *name,
                                  const char        *value,
                                  GError           **err)
{
    CastetDomElementClass *klass = CASTET_DOM_ELEMENT_GET_CLASS(self);

    g_return_if_fail(klass != NULL);

    if (klass->set_attribute)
        klass->set_attribute(self, name, value, err);
}

/*
 * GObject overloading
 */

static void
castet_dom_element_get_property (GObject    *object,
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
castet_dom_element_set_property (GObject      *object,
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
castet_dom_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_dom_element_parent_class)->dispose (object);
}

static void
castet_dom_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_dom_element_parent_class)->finalize (object);
}

static void
castet_dom_element_class_init (CastetDomElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    /* g_type_class_add_private (klass, sizeof (CastetDomElementPrivate)); */

    object_class->get_property = castet_dom_element_get_property;
    object_class->set_property = castet_dom_element_set_property;
    object_class->dispose = castet_dom_element_dispose;
    object_class->finalize = castet_dom_element_finalize;
}

static void
castet_dom_element_init (CastetDomElement *self)
{
    /* self->priv = DOM_ELEMENT_PRIVATE (self); */
}

CastetDomElement *
castet_dom_element_new (void)
{
    return g_object_new (CASTET_TYPE_DOM_ELEMENT, NULL);
}
