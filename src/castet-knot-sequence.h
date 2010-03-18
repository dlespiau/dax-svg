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

#ifndef __CASTET_KNOT_SEQUENCE_H__
#define __CASTET_KNOT_SEQUENCE_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define CASTET_TYPE_KNOT_SEQUENCE castet_knot_sequence_get_type()

#define CASTET_KNOT_SEQUENCE(obj)                           \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj),                     \
                                 CASTET_TYPE_KNOT_SEQUENCE, \
                                 CastetKnotSequence))

#define CASTET_KNOT_SEQUENCE_CLASS(klass)                   \
    (G_TYPE_CHECK_CLASS_CAST ((klass),                      \
                              CASTET_TYPE_KNOT_SEQUENCE,    \
                              CastetKnotSequenceClass))

#define CASTET_IS_KNOT_SEQUENCE(obj)    \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 CASTET_TYPE_KNOT_SEQUENCE))

#define CASTET_IS_KNOT_SEQUENCE_CLASS(klass)    \
    (G_TYPE_CHECK_CLASS_TYPE ((klass),          \
                              CASTET_TYPE_KNOT_SEQUENCE))

#define CASTET_KNOT_SEQUENCE_GET_CLASS(obj)                 \
    (G_TYPE_INSTANCE_GET_CLASS ((obj),                      \
                                CASTET_TYPE_KNOT_SEQUENCE,  \
                                CastetKnotSequenceClass))

typedef struct _CastetKnotSequence CastetKnotSequence;
typedef struct _CastetKnotSequenceClass CastetKnotSequenceClass;
typedef struct _CastetKnotSequencePrivate CastetKnotSequencePrivate;

struct _CastetKnotSequence
{
    GObject parent;

    CastetKnotSequencePrivate *priv;
};

struct _CastetKnotSequenceClass
{
    GObjectClass parent_class;
};

GType               castet_knot_sequence_get_type               (void) G_GNUC_CONST;

CastetKnotSequence *castet_knot_sequence_new                    (void);
CastetKnotSequence *castet_knot_sequence_new_from_static_array  (gfloat *data,
                                                                 guint   nb_knots);
CastetKnotSequence *castet_knot_sequence_new_from_string        (const gchar *string);
gchar *             castet_knot_sequence_to_string              (const CastetKnotSequence *seq);

guint               castet_knot_sequence_get_size               (const CastetKnotSequence *seq);
const gfloat *      castet_knot_sequence_get_array              (const CastetKnotSequence *seq);

G_END_DECLS

#endif /* __CASTET_KNOT_SEQUENCE_H__ */
