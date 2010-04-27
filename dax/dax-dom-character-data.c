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

#include "dax-dom-character-data.h"

G_DEFINE_TYPE (DaxDomCharacterData,
               dax_dom_character_data,
               DAX_TYPE_DOM_NODE)

#define DOM_CHARACTER_DATA_PRIVATE(o)                                   \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                              \
                                      DAX_TYPE_DOM_CHARACTER_DATA,   \
                                      DaxDomCharacterDataPrivate))

struct _DaxDomCharacterDataPrivate
{
    GString *data;
};

static void
dax_dom_character_data_get_property (GObject    *object,
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
dax_dom_character_data_set_property (GObject      *object,
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
dax_dom_character_data_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_dom_character_data_parent_class)->dispose (object);
}

static void
dax_dom_character_data_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_dom_character_data_parent_class)->finalize (object);
}

static void
dax_dom_character_data_class_init (DaxDomCharacterDataClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (DaxDomCharacterDataPrivate));

    object_class->get_property = dax_dom_character_data_get_property;
    object_class->set_property = dax_dom_character_data_set_property;
    object_class->dispose = dax_dom_character_data_dispose;
    object_class->finalize = dax_dom_character_data_finalize;
}

static void
dax_dom_character_data_init (DaxDomCharacterData *self)
{
    DaxDomCharacterDataPrivate *priv;

    self->priv = priv = DOM_CHARACTER_DATA_PRIVATE (self);

    priv->data = g_string_new ("");
}

DaxDomCharacterData *
dax_dom_character_data_new (void)
{
    return g_object_new (DAX_TYPE_DOM_CHARACTER_DATA, NULL);
}

void
dax_dom_character_data_set_data (DaxDomCharacterData *char_data,
                                    const gchar            *data)
{
    g_return_if_fail (DAX_IS_DOM_CHARACTER_DATA (char_data));

    g_string_assign (char_data->priv->data, data);
}

const gchar *
dax_dom_character_data_get_data (DaxDomCharacterData *char_data)
{
    g_return_val_if_fail (DAX_IS_DOM_CHARACTER_DATA (char_data), NULL);

    return char_data->priv->data->str;
}
