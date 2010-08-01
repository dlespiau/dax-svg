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

#ifndef __DAX_TRAVERSER_LOAD_H__
#define __DAX_TRAVERSER_LOAD_H__

#include <glib-object.h>

#include "dax-traverser.h"

G_BEGIN_DECLS

#define DAX_TYPE_TRAVERSER_LOAD dax_traverser_load_get_type()

#define DAX_TRAVERSER_LOAD(obj)                             \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 DAX_TYPE_TRAVERSER_LOAD,   \
                                 DaxTraverserLoad))

#define DAX_TRAVERSER_LOAD_CLASS(klass)                 \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                  \
                              DAX_TYPE_TRAVERSER_LOAD,  \
                              DaxTraverserLoadClass))

#define DAX_IS_TRAVERSER_LOAD(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DAX_TYPE_TRAVERSER_LOAD))

#define DAX_IS_TRAVERSER_LOAD_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), DAX_TYPE_TRAVERSER_LOAD))

#define DAX_TRAVERSER_LOAD_GET_CLASS(obj)                   \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                DAX_TYPE_TRAVERSER_LOAD,    \
                                DaxTraverserLoadClass))

typedef struct _DaxTraverserLoad DaxTraverserLoad;
typedef struct _DaxTraverserLoadClass DaxTraverserLoadClass;


struct _DaxTraverserLoad
{
    DaxTraverser parent;
};

struct _DaxTraverserLoadClass
{
    DaxTraverserClass parent_class;
};

GType               dax_traverser_load_get_type     (void) G_GNUC_CONST;

DaxTraverser *      dax_traverser_load_new          (DaxDomNode *root);

G_END_DECLS

#endif /* __DAX_TRAVERSER_LOAD_H__ */
