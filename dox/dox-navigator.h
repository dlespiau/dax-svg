/*
 * Dox - Load and draw SVG
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

#ifndef __DOX_NAVIGATOR_H__
#define __DOX_NAVIGATOR_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define DOX_TYPE_NAVIGATOR dox_navigator_get_type()

#define DOX_NAVIGATOR(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), DOX_TYPE_NAVIGATOR, DoxNavigator))

#define DOX_NAVIGATOR_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), DOX_TYPE_NAVIGATOR, DoxNavigatorClass))

#define DOX_IS_NAVIGATOR(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DOX_TYPE_NAVIGATOR))

#define DOX_IS_NAVIGATOR_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DOX_TYPE_NAVIGATOR))

#define DOX_NAVIGATOR_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), DOX_TYPE_NAVIGATOR, DoxNavigatorClass))

typedef struct _DoxNavigator DoxNavigator;
typedef struct _DoxNavigatorClass DoxNavigatorClass;
typedef struct _DoxNavigatorPrivate DoxNavigatorPrivate;

struct _DoxNavigator
{
    GObject parent;

    DoxNavigatorPrivate *priv;
};

struct _DoxNavigatorClass
{
    GObjectClass parent_class;
};

GType dox_navigator_get_type (void) G_GNUC_CONST;

DoxNavigator *  dox_navigator_new (void);
DoxNavigator *  dox_navigator_get_default (void);

void            dox_navigator_set_user_agent (DoxNavigator *navigator,
                                              const gchar  *user_agent);
const gchar *   dox_navigator_get_user_agent (DoxNavigator *navigator);

G_END_DECLS

#endif /* __DOX_NAVIGATOR_H__ */
