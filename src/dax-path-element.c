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

#include "dax-internals.h"
#include "dax-path-element.h"

G_DEFINE_TYPE (DaxPathElement, dax_path_element, DAX_TYPE_ELEMENT)

#define PATH_ELEMENT_PRIVATE(o)                                 \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      DAX_TYPE_PATH_ELEMENT, \
                                      DaxPathElementPrivate))

enum
{
    PROP_0,

    PROP_PATH
};

struct _DaxPathElementPrivate
{
    ClutterPath *path;
};

static void
dax_path_element_get_property (GObject    *object,
                                  guint       property_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
    DaxPathElement *self = DAX_PATH_ELEMENT (object);
    DaxPathElementPrivate *priv = self->priv;

    switch (property_id)
    {
    case PROP_PATH:
        g_value_set_object (value, priv->path);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_path_element_set_property (GObject      *object,
                                  guint         property_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
    DaxPathElement *self = DAX_PATH_ELEMENT (object);
    DaxPathElementPrivate *priv = self->priv;

    switch (property_id)
    {
    case PROP_PATH:
        if (priv->path)
            g_object_unref (priv->path);
        priv->path = g_object_ref (g_value_get_object (value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_path_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_path_element_parent_class)->dispose (object);
}

static void
dax_path_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (dax_path_element_parent_class)->finalize (object);
}

static void
dax_path_element_class_init (DaxPathElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxPathElementPrivate));

    object_class->get_property = dax_path_element_get_property;
    object_class->set_property = dax_path_element_set_property;
    object_class->dispose = dax_path_element_dispose;
    object_class->finalize = dax_path_element_finalize;

    pspec = g_param_spec_object ("d",
                                "Path data",
                                "", /* FIXME */
                                CLUTTER_TYPE_PATH,
                                DAX_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_PATH, pspec);
}

static void
dax_path_element_init (DaxPathElement *self)
{
    self->priv = PATH_ELEMENT_PRIVATE (self);
}

DaxDomElement *
dax_path_element_new (void)
{
    return g_object_new (DAX_TYPE_PATH_ELEMENT, NULL);
}

ClutterPath *
dax_path_element_get_path (DaxPathElement *self)
{
    g_return_val_if_fail (DAX_IS_PATH_ELEMENT (self), NULL);

    return g_object_ref (self->priv->path);
}
