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

#ifndef __DAX_CACHE_H__
#define __DAX_CACHE_H__

#include <glib-object.h>

#include "dax-dom-element.h"
#include "dax-cache-entry.h"

G_BEGIN_DECLS

#define DAX_TYPE_CACHE dax_cache_get_type()

#define DAX_CACHE(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), DAX_TYPE_CACHE, DaxCache))

#define DAX_CACHE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), DAX_TYPE_CACHE, DaxCacheClass))

#define DAX_IS_CACHE(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_CACHE))

#define DAX_IS_CACHE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),  DAX_TYPE_CACHE))

#define DAX_CACHE_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), DAX_TYPE_CACHE, DaxCacheClass))

typedef struct _DaxCacheClass DaxCacheClass;
typedef struct _DaxCachePrivate DaxCachePrivate;

struct _DaxCache
{
    GObject parent;

    DaxCachePrivate *priv;
};

struct _DaxCacheClass
{
    GObjectClass parent_class;
};

GType           dax_cache_get_type              (void) G_GNUC_CONST;

DaxCache *      dax_cache_new                   (void);
DaxCache *      dax_cache_get_default           (void);
DaxCacheEntry * dax_cache_get_entry_for_uri     (DaxCache      *cache,
                                                 const gchar   *uri);
DaxCacheEntry * dax_cache_get_entry_for_href    (DaxCache      *cache,
                                                 DaxDomElement *element,
                                                 const gchar   *href);

G_END_DECLS

#endif /* __DAX_CACHE_H__ */
