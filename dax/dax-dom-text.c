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
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "dax-dom-text.h"

G_DEFINE_TYPE (DaxDomText, dax_dom_text, DAX_TYPE_DOM_CHARACTER_DATA)

#define DOM_TEXT_PRIVATE(o)                                 \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      DAX_TYPE_DOM_TEXT, \
                                      DaxDomTextPrivate))

struct _DaxDomTextPrivate
{
};

static void
dax_dom_text_get_property (GObject    *object,
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
dax_dom_text_set_property (GObject      *object,
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
dax_dom_text_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_dom_text_parent_class)->dispose (object);
}

static void
dax_dom_text_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_dom_text_parent_class)->finalize (object);
}

static void
dax_dom_text_class_init (DaxDomTextClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    /* g_type_class_add_private (klass, sizeof (DaxDomTextPrivate)); */

    object_class->get_property = dax_dom_text_get_property;
    object_class->set_property = dax_dom_text_set_property;
    object_class->dispose = dax_dom_text_dispose;
    object_class->finalize = dax_dom_text_finalize;
}

static void
dax_dom_text_init (DaxDomText *self)
{
    /* self->priv = DOM_TEXT_PRIVATE (self); */
}

DaxDomText *
dax_dom_text_new (void)
{
    return g_object_new (DAX_TYPE_DOM_TEXT, NULL);
}
