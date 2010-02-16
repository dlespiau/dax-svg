/*
 * Castet - Load and draw SVG
 *
 * Copyright © 2009, 2010 Intel Corporation.
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

#ifndef __CASTET_TYPES_H__
#define __CASTET_TYPES_H__

#include <glib-object.h>

G_BEGIN_DECLS

/*
 * Animation types
 */

/*
 * CastetDuration
 */

#define CASTET_TYPE_DURATION        (castet_duration_get_type ())
#define CASTET_VALUE_HOLDS_DURATION (G_VALUE_HOLDS ((x), CASTET_TYPE_DURATION))

typedef enum /*< skip >*/
{
    CASTET_DURATION_S,
    CASTET_DURATION_MS
} CastetDurationType;

typedef struct _CastetDuration CastetDuration;

struct _CastetDuration
{
    CastetDurationType unit_type;

    gfloat value;

    gfloat ms;
    guint ms_set;
};

GType               castet_duration_get_type        (void) G_GNUC_CONST;
CastetDurationType  castet_duration_get_unit_type   (const CastetDuration *dur);
gfloat              castet_duration_get_unit_value  (const CastetDuration *dur);

CastetDuration *    castet_duration_copy            (const CastetDuration *dur);
void                castet_duration_free            (CastetDuration *dur);

void                castet_duration_from_s          (CastetDuration *dur,
                                                     gfloat          seconds);
void                castet_duration_from_ms         (CastetDuration *dur,
                                                     gfloat          ms);

gfloat              castet_duration_to_ms           (CastetDuration *dur);

gboolean            castet_duration_from_string     (CastetDuration *dur,
                                                     const gchar    *string);
gchar *             castet_duration_to_string       (const CastetDuration *dur);

/*
 * CastetAnimationAttributeType
 */

#define CASTET_ANIMATION_ATTRIBUTE_TYPE_CASE_SENSITIVE  1
#define CASTET_ANIMATION_ATTRIBUTE_TYPE_DEFAULT         \
    CASTET_ANIMATION_ATTRIBUTE_TYPE_AUTO

typedef enum _CastetAnimationAttributeType
{
    CASTET_ANIMATION_ATTRIBUTE_TYPE_AUTO,
    CASTET_ANIMATION_ATTRIBUTE_TYPE_CSS,    /*< nick=CSS >*/
    CASTET_ANIMATION_ATTRIBUTE_TYPE_XML     /*< nick=XML >*/
} CastetAnimationAttributeType;

/*
 * CastetRepeatCount
 */

#define CASTET_TYPE_REPEAT_COUNT        (castet_repeat_count_get_type ())
#define CASTET_VALUE_HOLDS_REPEAT_COUNT \
    (G_VALUE_HOLDS ((x), CASTET_TYPE_REPEAT_COUNT))

typedef struct _CastetRepeatCount CastetRepeatCount;

struct _CastetRepeatCount
{
    gfloat value;
};

GType               castet_repeat_count_get_type        (void) G_GNUC_CONST;

CastetRepeatCount * castet_repeat_count_copy            (const CastetRepeatCount *count);
void                castet_repeat_count_free            (CastetRepeatCount *count);

void                castet_repeat_count_from_number     (CastetRepeatCount *count,
                                                         gfloat             ms);

gboolean            castet_repeat_count_is_indefinite   (const CastetRepeatCount *count);
gfloat              castet_repeat_count_get_value       (const CastetRepeatCount *count);

gboolean            castet_repeat_count_from_string     (CastetRepeatCount *count,
                                                         const gchar       *string);
gchar *             castet_repeat_count_to_string       (const CastetRepeatCount *count);

G_END_DECLS

#endif /* __CASTET_TYPES_H__ */
