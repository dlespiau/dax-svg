/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009, 2010 Intel Corporation.
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

#include "dax-types.h"
#include "dax-utils.h"

#include "dax-element-g.h"

G_DEFINE_TYPE (DaxElementG, dax_element_g, DAX_TYPE_ELEMENT)

#define ELEMENT_G_PRIVATE(o)                                \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      DAX_TYPE_ELEMENT_G,   \
                                      DaxElementGPrivate))

enum
{
    PROP_0,
    PROP_TRANSFORM
};

struct _DaxElementGPrivate
{
    DaxMatrix *transform;
};

static void
dax_element_g_get_property (GObject    *object,
                            guint       property_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
    DaxElementG *g = DAX_ELEMENT_G (object);
    DaxElementGPrivate *priv = g->priv;

    switch (property_id)
    {
    case PROP_TRANSFORM:
        g_value_set_boxed (value, priv->transform);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_g_set_property (GObject      *object,
                            guint         property_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
    DaxElementG *g = DAX_ELEMENT_G (object);
    DaxElementGPrivate *priv = g->priv;

    switch (property_id)
    {
    case PROP_TRANSFORM:
        if (priv->transform)
            dax_matrix_free (priv->transform);
        priv->transform = dax_matrix_deep_copy (g_value_get_boxed (value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_g_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_element_g_parent_class)->dispose (object);
}

static void
dax_element_g_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_element_g_parent_class)->finalize (object);
}

static void
dax_element_g_class_init (DaxElementGClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (DaxElementGPrivate));

    object_class->get_property = dax_element_g_get_property;
    object_class->set_property = dax_element_g_set_property;
    object_class->dispose = dax_element_g_dispose;
    object_class->finalize =  dax_element_g_finalize;

    _dax_utils_install_properties (object_class,
                                   _DAX_PROP_TRANSFORM, PROP_TRANSFORM,
                                   0);
}

static void
dax_element_g_init (DaxElementG *self)
{
    self->priv = ELEMENT_G_PRIVATE (self);
}

DaxDomElement *
dax_element_g_new (void)
{
    return g_object_new (DAX_TYPE_ELEMENT_G, NULL);
}

const DaxMatrix *
dax_element_g_get_transform (DaxElementG *g)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_G (g), NULL);

    return g->priv->transform;
}
