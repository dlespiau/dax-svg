/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "dox-internals.h"

#include "dox-navigator.h"

G_DEFINE_TYPE (DoxNavigator, dox_navigator, G_TYPE_OBJECT)

#define NAVIGATOR_PRIVATE(o)                                \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      DOX_TYPE_NAVIGATOR,   \
                                      DoxNavigatorPrivate))

enum
{
    PROP_0,

    PROP_USER_AGENT
};

struct _DoxNavigatorPrivate
{
    gchar *user_agent;
};

static void
dox_navigator_get_property (GObject    *object,
                            guint       property_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
    DoxNavigator *navigator = DOX_NAVIGATOR (object);
    DoxNavigatorPrivate *priv = navigator->priv;

    switch (property_id)
    {
    case PROP_USER_AGENT:
        g_value_set_string (value, priv->user_agent);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dox_navigator_set_property (GObject      *object,
                            guint         property_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
    DoxNavigator *navigator = DOX_NAVIGATOR (object);
    DoxNavigatorPrivate *priv = navigator->priv;

    switch (property_id)
    {
    case PROP_USER_AGENT:
        g_free (priv->user_agent);
        priv->user_agent = g_value_dup_string (value);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dox_navigator_dispose (GObject *object)
{
    G_OBJECT_CLASS (dox_navigator_parent_class)->dispose (object);
}

static void
dox_navigator_finalize (GObject *object)
{
    DoxNavigator *navigator = DOX_NAVIGATOR (object);
    DoxNavigatorPrivate *priv = navigator->priv;

    g_free (priv->user_agent);

    G_OBJECT_CLASS (dox_navigator_parent_class)->finalize (object);
}

static void
dox_navigator_class_init (DoxNavigatorClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DoxNavigatorPrivate));

    object_class->get_property = dox_navigator_get_property;
    object_class->set_property = dox_navigator_set_property;
    object_class->dispose = dox_navigator_dispose;
    object_class->finalize = dox_navigator_finalize;

    pspec = g_param_spec_string ("user-agent",
                                 "User agent",
                                 "Name of the HTTP user agent",
                                 "Dox " PACKAGE_VERSION,
                                 DOX_GPARAM_READWRITE | G_PARAM_CONSTRUCT);
    g_object_class_install_property (object_class, PROP_USER_AGENT, pspec);
}

static void
dox_navigator_init (DoxNavigator *self)
{
    DoxNavigatorPrivate *priv;

    self->priv = priv = NAVIGATOR_PRIVATE (self);
}

DoxNavigator *
dox_navigator_new (void)
{
    return g_object_new (DOX_TYPE_NAVIGATOR, NULL);
}

DoxNavigator *
dox_navigator_get_default (void)
{
    static DoxNavigator *navigator = NULL;

    if (G_UNLIKELY (navigator == NULL)) {
        navigator = dox_navigator_new ();
    }

    return navigator;
}

void
dox_navigator_set_user_agent (DoxNavigator *navigator,
                              const gchar  *user_agent)
{
    DoxNavigatorPrivate *priv;

    g_return_if_fail (DOX_IS_NAVIGATOR (navigator));
    priv = navigator->priv;

    g_free (priv->user_agent);
    priv->user_agent = g_strdup (user_agent);

    g_object_notify (G_OBJECT (navigator), "user-agent");
}

const gchar *
dox_navigator_get_user_agent (DoxNavigator *navigator)
{
    g_return_val_if_fail (DOX_IS_NAVIGATOR (navigator), NULL);

    return navigator->priv->user_agent;
}
