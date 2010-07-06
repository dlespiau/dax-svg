/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009, 2010 Intel Corporation.
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

#include "dax-internals.h"
#include "dax-types.h"
#include "dax-utils.h"

#include "dax-element-path.h"

G_DEFINE_TYPE (DaxElementPath, dax_element_path, DAX_TYPE_ELEMENT)

#define ELEMENT_PATH_PRIVATE(o)                                 \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      DAX_TYPE_ELEMENT_PATH,    \
                                      DaxElementPathPrivate))

enum
{
    PROP_0,

    PROP_PATH,
    PROP_TRANSFORM
};

struct _DaxElementPathPrivate
{
    ClutterPath2D *path;
    DaxMatrix *transform;
};

static void
dax_element_path_get_property (GObject    *object,
                               guint       property_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
    DaxElementPath *self = DAX_ELEMENT_PATH (object);
    DaxElementPathPrivate *priv = self->priv;

    switch (property_id)
    {
    case PROP_PATH:
        g_value_set_object (value, priv->path);
        break;
    case PROP_TRANSFORM:
        g_value_set_boxed (value, priv->transform);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_element_path_set_property (GObject      *object,
                               guint         property_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
    DaxElementPath *self = DAX_ELEMENT_PATH (object);
    DaxElementPathPrivate *priv = self->priv;

    switch (property_id)
    {
    case PROP_PATH:
        if (priv->path)
            g_object_unref (priv->path);
        priv->path = g_object_ref (g_value_get_object (value));
        break;
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
dax_element_path_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_element_path_parent_class)->dispose (object);
}

static void
dax_element_path_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_element_path_parent_class)->finalize (object);
}

static void
dax_element_path_class_init (DaxElementPathClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxElementPathPrivate));

    object_class->get_property = dax_element_path_get_property;
    object_class->set_property = dax_element_path_set_property;
    object_class->dispose = dax_element_path_dispose;
    object_class->finalize = dax_element_path_finalize;

    pspec = g_param_spec_object ("d",
                                "Path data",
                                "", /* FIXME */
                                CLUTTER_TYPE_PATH_2D,
                                DAX_GPARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_PATH, pspec);

    _dax_utils_install_properties (object_class,
                                   _DAX_PROP_TRANSFORM, PROP_TRANSFORM,
                                   0);
}

static void
dax_element_path_init (DaxElementPath *self)
{
    self->priv = ELEMENT_PATH_PRIVATE (self);
}

DaxDomElement *
dax_element_path_new (void)
{
    return g_object_new (DAX_TYPE_ELEMENT_PATH, NULL);
}

ClutterPath2D *
dax_element_path_get_path (DaxElementPath *self)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_PATH (self), NULL);

    return g_object_ref (self->priv->path);
}

const DaxMatrix *
dax_element_path_get_transform (DaxElementPath *path)
{
    g_return_val_if_fail (DAX_IS_ELEMENT_PATH (path), NULL);

    return path->priv->transform;
}
