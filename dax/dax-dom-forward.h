/*
 * Dax - Load and draw SVG
 *
 * Copyright © 2009 Intel Corporation.
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

#ifndef __DAX_DOM_FORWARD_H__
#define __DAX_DOM_FORWARD_H__

#include <glib.h>

G_BEGIN_DECLS

/*
 * There is a circular dependencies between DOM Classes, so let's just
 * declare a few here first.
 */
typedef struct _DaxDomNode            DaxDomNode;
typedef struct _DaxDomElement         DaxDomElement;
typedef struct _DaxDomDocument        DaxDomDocument;

G_END_DECLS

#endif /* __DAX_DOM_FORWARD_H__ */

