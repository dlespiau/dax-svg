/*
 * Castet - Load and draw SVG
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

#include "castet-dom-text.h"

G_DEFINE_TYPE (CastetDomText, castet_dom_text, CASTET_TYPE_DOM_CHARACTER_DATA)

#define DOM_TEXT_PRIVATE(o)                                 \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      CASTET_TYPE_DOM_TEXT, \
                                      CastetDomTextPrivate))

struct _CastetDomTextPrivate
{
};

static void
castet_dom_text_get_property (GObject    *object,
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
castet_dom_text_set_property (GObject      *object,
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
castet_dom_text_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_dom_text_parent_class)->dispose (object);
}

static void
castet_dom_text_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_dom_text_parent_class)->finalize (object);
}

static void
castet_dom_text_class_init (CastetDomTextClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    /* g_type_class_add_private (klass, sizeof (CastetDomTextPrivate)); */

    object_class->get_property = castet_dom_text_get_property;
    object_class->set_property = castet_dom_text_set_property;
    object_class->dispose = castet_dom_text_dispose;
    object_class->finalize = castet_dom_text_finalize;
}

static void
castet_dom_text_init (CastetDomText *self)
{
    /* self->priv = DOM_TEXT_PRIVATE (self); */
}

CastetDomText *
castet_dom_text_new (void)
{
    return g_object_new (CASTET_TYPE_DOM_TEXT, NULL);
}
