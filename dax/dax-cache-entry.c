/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2010 Intel Corporation.
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

#include <gio/gio.h>

#include "dax-cache.h"
#include "dax-internals.h"
#include "dax-utils.h"

#include "dax-cache-entry.h"

G_DEFINE_TYPE (DaxCacheEntry, dax_cache_entry, G_TYPE_OBJECT)

#define CACHE_ENTRY_PRIVATE(o)                              \
        (G_TYPE_INSTANCE_GET_PRIVATE ((o),                  \
                                      DAX_TYPE_CACHE_ENTRY, \
                                      DaxCacheEntryPrivate))

enum
{
    PROP_0,

    PROP_CACHE,
    PROP_URI,
    PROP_LOCAL_URI,
    PROP_READY
};

struct _DaxCacheEntryPrivate
{
    DaxCache *cache;        /* back pointer to the cache ths entry belongs to */
    gchar *uri;
    gchar *local_uri;
    gboolean ready;
};

static void
invalidate_local_uri (DaxCacheEntry *entry)
{
    DaxCacheEntryPrivate *priv = entry->priv;

    /* local_uri can be uri in the case of a file:// uri */
    if (priv->local_uri != priv->uri)
        g_free (priv->local_uri);

    priv->local_uri = NULL;
}

static void
dax_cache_entry_set_uri_internal (DaxCacheEntry *entry,
                                  const gchar   *uri)
{
    DaxCacheEntryPrivate *priv = entry->priv;

    invalidate_local_uri (entry);
    g_free (priv->uri);
    priv->uri = g_strdup (uri);

    if (g_str_has_prefix (uri, "file://")) {
        priv->ready = TRUE;
        priv->local_uri = priv->uri;
        g_object_notify ((GObject *) entry, "ready");
    } else {
        /* FIXME http:// uris could be useful */
        g_message (G_STRLOC ": Unsupported uri %s", uri);
    }
}

/*
 * GObject implementation
 */

static void
dax_cache_entry_get_property (GObject    *object,
                              guint       property_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
    DaxCacheEntry *entry = (DaxCacheEntry *) object;
    DaxCacheEntryPrivate *priv = entry->priv;

    switch (property_id)
    {
    case PROP_CACHE:
        g_value_set_object (value, priv->cache);
        break;
    case PROP_URI:
        g_value_set_string (value, priv->uri);
        break;
    case PROP_LOCAL_URI:
        g_value_set_string (value, priv->local_uri);
        break;
    case PROP_READY:
        g_value_set_boolean (value, priv->ready);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_cache_entry_set_property (GObject      *object,
                              guint         property_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
    DaxCacheEntry *entry = (DaxCacheEntry *) object;

    switch (property_id)
    {
    case PROP_CACHE:
        /* read only */
        break;
    case PROP_URI:
        dax_cache_entry_set_uri_internal (entry, g_value_get_string (value));
        break;
    case PROP_LOCAL_URI:
        /* ready only */
        break;
    case PROP_READY:
        /* read only */
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
dax_cache_entry_dispose (GObject *object)
{
    G_OBJECT_CLASS (dax_cache_entry_parent_class)->dispose (object);
}

static void
dax_cache_entry_finalize (GObject *object)
{
    DaxCacheEntry *entry = (DaxCacheEntry *) object;
    DaxCacheEntryPrivate *priv = entry->priv;

    invalidate_local_uri (entry);
    g_free (priv->uri);

    G_OBJECT_CLASS (dax_cache_entry_parent_class)->finalize (object);
}

static void
dax_cache_entry_class_init (DaxCacheEntryClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    g_type_class_add_private (klass, sizeof (DaxCacheEntryPrivate));

    object_class->get_property = dax_cache_entry_get_property;
    object_class->set_property = dax_cache_entry_set_property;
    object_class->dispose = dax_cache_entry_dispose;
    object_class->finalize = dax_cache_entry_finalize;

    pspec = g_param_spec_object ("cache",
                                 "Cache",
                                 "Cache this entry belongs to",
                                 DAX_TYPE_CACHE,
                                 DAX_GPARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
    g_object_class_install_property (object_class, PROP_CACHE, pspec);

    pspec = g_param_spec_string ("uri",
                                 "URI",
                                 "URI this entry caches",
                                 "",
                                 DAX_GPARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
    g_object_class_install_property (object_class, PROP_URI, pspec);

    pspec = g_param_spec_string ("local-uri",
                                 "Local URI",
                                 "On-disk file URI for this entry",
                                 "",
                                 DAX_GPARAM_READABLE);
    g_object_class_install_property (object_class, PROP_LOCAL_URI, pspec);

    pspec = g_param_spec_boolean ("ready",
                                  "Ready",
                                  "This entry is ready to be used",
                                  FALSE,
                                  DAX_GPARAM_READABLE);
    g_object_class_install_property (object_class, PROP_READY, pspec);
}

static void
dax_cache_entry_init (DaxCacheEntry *self)
{
    self->priv = CACHE_ENTRY_PRIVATE (self);
}

DaxCacheEntry *
_dax_cache_entry_new (DaxCache    *cache,
                      const gchar *uri)
{
    return g_object_new (DAX_TYPE_CACHE_ENTRY,
                         "cache", cache,
                         "uri", uri,
                         NULL);
}

/*
 * Accessors
 */

DaxCache *
dax_cache_entry_get_cache (DaxCacheEntry *entry)
{
    g_return_val_if_fail (DAX_IS_CACHE_ENTRY (entry), NULL);

    return entry->priv->cache;
}

const gchar *
dax_cache_entry_get_uri (DaxCacheEntry *entry)
{
    g_return_val_if_fail (DAX_IS_CACHE_ENTRY (entry), NULL);

    return entry->priv->uri;
}

void
dax_cache_entry_set_uri (DaxCacheEntry *entry,
                         const gchar   *uri)
{
    g_return_if_fail (DAX_IS_CACHE_ENTRY (entry));

    dax_cache_entry_set_uri_internal (entry, uri);

    g_object_notify ((GObject *) entry, "uri");
}

const gchar *
dax_cache_entry_get_local_uri (DaxCacheEntry *entry)
{
    g_return_val_if_fail (DAX_IS_CACHE_ENTRY (entry), NULL);

    return entry->priv->local_uri;
}

gchar *
dax_cache_entry_get_local_path (const DaxCacheEntry *entry)
{
    DaxCacheEntryPrivate *priv;
    GFile *file;
    gchar *path;

    g_return_val_if_fail (DAX_IS_CACHE_ENTRY (entry), NULL);
    priv = entry->priv;

    file = g_file_new_for_uri (priv->local_uri);
    path = g_file_get_path (file);
    g_object_unref (file);

    return path;
}

gboolean
dax_cache_entry_is_ready (DaxCacheEntry *entry)
{
    g_return_val_if_fail (DAX_IS_CACHE_ENTRY (entry), FALSE);

    return entry->priv->ready;
}
