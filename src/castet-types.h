/*
 * Castet - Load and draw SVG
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

#ifndef __CASTET_TYPES_H__
#define __CASTET_TYPES_H__

#include <glib-object.h>

#define CASTET_TYPE_KNOT_SEQUENCE   (castet_knot_sequence_get_type ())

G_BEGIN_DECLS

typedef struct _CastetKnotSequence CastetKnotSequence;

GType               castet_knot_sequence_get_type               (void) G_GNUC_CONST;
CastetKnotSequence *castet_knot_sequence_new_from_array         (gfloat *data,
                                                                 guint   nb_knots);
CastetKnotSequence *castet_knot_sequence_new_from_static_array  (gfloat *data,
                                                                 guint   nb_knots);
CastetKnotSequence *castet_knot_sequence_new_from_string        (const gchar *string);
CastetKnotSequence *castet_knot_sequence_copy                   (const CastetKnotSequence *seq);
CastetKnotSequence *castet_knot_sequence_deep_copy              (const CastetKnotSequence *seq);
void                castet_knot_sequence_free                   (CastetKnotSequence *box);

const guint         castet_knot_sequence_get_size               (const CastetKnotSequence *seq);
const gfloat       *castet_knot_sequence_get_array              (const CastetKnotSequence *seq);

G_END_DECLS

#endif /* __CASTET_TYPES_H__ */
