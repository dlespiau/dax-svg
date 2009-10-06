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

#include "castet-internals.h"
#include "castet-path-element.h"

G_DEFINE_TYPE (CastetPathElement, castet_path_element, CASTET_TYPE_ELEMENT)

#define PATH_ELEMENT_PRIVATE(o)                                 \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                      \
                                      CASTET_TYPE_PATH_ELEMENT, \
                                      CastetPathElementPrivate))

enum
{
    PROP_0,

    PROP_PATH
};

struct _CastetPathElementPrivate
{
    ClutterPath *path;
};

static void
castet_path_element_get_property (GObject    *object,
                                  guint       property_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
    CastetPathElement *self = CASTET_PATH_ELEMENT (object);
    CastetPathElementPrivate *priv = self->priv;

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
castet_path_element_set_property (GObject      *object,
                                  guint         property_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
    CastetPathElement *self = CASTET_PATH_ELEMENT (object);
    CastetPathElementPrivate *priv = self->priv;

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
castet_path_element_dispose (GObject *object)
{
    G_OBJECT_CLASS (castet_path_element_parent_class)->dispose (object);
}

static void
castet_path_element_finalize (GObject *object)
{
    G_OBJECT_CLASS (castet_path_element_parent_class)->finalize (object);
}

static void
castet_path_element_class_init (CastetPathElementClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (CastetPathElementPrivate));

    object_class->get_property = castet_path_element_get_property;
    object_class->set_property = castet_path_element_set_property;
    object_class->dispose = castet_path_element_dispose;
    object_class->finalize = castet_path_element_finalize;

    pspec = g_param_spec_object ("d",
                                "Path data",
                                "", /* FIXME */
                                CLUTTER_TYPE_PATH,
                                CASTET_PARAM_READWRITE);
    g_object_class_install_property (object_class, PROP_PATH, pspec);
}

static void
castet_path_element_init (CastetPathElement *self)
{
    self->priv = PATH_ELEMENT_PRIVATE (self);
}

CastetDomElement *
castet_path_element_new (void)
{
    return g_object_new (CASTET_TYPE_PATH_ELEMENT, NULL);
}

ClutterPath *
castet_path_element_get_path (CastetPathElement *self)
{
    g_return_val_if_fail (CASTET_IS_PATH_ELEMENT (self), NULL);

    return g_object_ref (self->priv->path);
}
