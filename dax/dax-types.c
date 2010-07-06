/*
 * Dax - Load and draw SVG
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
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>

#include "dax-affine.h"
#include "dax-enum-types.h"
#include "dax-internals.h"
#include "dax-utils.h"

#include "dax-types.h"

/*
 * DaxMatrix
 */

static DaxElementaryMatrix *
dax_elementary_matrix_new (DaxMatrixType type,
                           float         param0,
                           float         param1,
                           float         param2)
{
    DaxElementaryMatrix *em;

    em = g_slice_new (DaxElementaryMatrix);
    em->type = type;
    em->params[0] = param0;
    em->params[1] = param1;
    em->params[2] = param2;

    return em;
}

static void
dax_elementary_matrix_free (DaxElementaryMatrix *em)
{
    g_slice_free (DaxElementaryMatrix, em);
}

static DaxElementaryMatrix *
dax_elementary_matrix_copy (DaxElementaryMatrix *em)
{
    return g_slice_dup (DaxElementaryMatrix, em);
}

static void
dax_matrix_prepend_elementary (DaxMatrix           *matrix,
                               DaxElementaryMatrix *em)
{
    matrix->elementary_matrices = g_list_prepend (matrix->elementary_matrices,
                                                  em);
}

DaxMatrix *
dax_matrix_deep_copy (const DaxMatrix *matrix)
{
    DaxMatrix *new_matrix;
    GList *cur;

    if (matrix == NULL)
        return NULL;

    new_matrix = g_slice_dup (DaxMatrix, matrix);
    new_matrix->elementary_matrices = NULL;
    new_matrix->refcount = 1;
    for (cur = matrix->elementary_matrices; cur; cur = g_list_next (cur)) {
        DaxElementaryMatrix *em;

        em = dax_elementary_matrix_copy (cur->data);
        dax_matrix_prepend_elementary (new_matrix, em);
    }

    new_matrix->elementary_matrices =
        g_list_reverse (new_matrix->elementary_matrices);

    return new_matrix;
}

DaxMatrix *
dax_matrix_copy (DaxMatrix *matrix)
{
    if (matrix == NULL)
        return NULL;

    g_atomic_int_inc (&matrix->refcount);

    return matrix;
}

static void
dax_matrix_free_elementary_matrices (DaxMatrix *matrix)
{
    GList *cur;

    if (matrix->elementary_matrices == NULL)
        return;

    for (cur = matrix->elementary_matrices; cur; cur = g_list_next (cur))
        dax_elementary_matrix_free (cur->data);
    g_list_free (matrix->elementary_matrices);
    matrix->elementary_matrices = NULL;
}

static void
dax_matrix_destroy (DaxMatrix *matrix)
{
    dax_matrix_free_elementary_matrices (matrix);
    g_slice_free (DaxMatrix, matrix);
}

void
dax_matrix_free (DaxMatrix *matrix)
{
    if (matrix == NULL)
        return;

    if (g_atomic_int_dec_and_test (&matrix->refcount))
        dax_matrix_destroy (matrix);
}

void
dax_matrix_from_array (DaxMatrix *matrix,
                       double     src[6])
{
    g_return_if_fail (matrix != NULL);

    matrix->refcount = 1;
    matrix->elementary_matrices = NULL;
    memcpy (matrix->affine, src, 6 * sizeof (double));
}

double *
dax_matrix_get_affine (DaxMatrix *matrix)
{
    g_return_val_if_fail (matrix != NULL, NULL);

    return matrix->affine;
}

/*
 * Parse an SVG transform string into an affine matrix. Reference: SVG
 * working draft dated 1999-07-06, section 8.5. Return TRUE on
 * success.
 *
 * Taken from libsrvg, srvg-styles.c, LGPLv2+,
 * Copyright (C) 2000 Eazel, Inc.
 * Copyright (C) 2002 Dom Lachowicz <cinamod@hotmail.com>
 *
 * I made some changes to the original function. We now populate a GList of
 * elementary transformation that the resulting matrix can be decomposed into.
 * This is useful when, say, we want to interpolate between "rotate(0)
 * scale(1)" and "rotate(90) scale(10)"
 */
static gboolean
dax_parse_transform (DaxMatrix  *matrix,
                     const char *src)
{
    int idx;
    char keyword[32];
    double args[6];
    int n_args;
    guint key_len;
    double tmp_affine[6];
    double *dst = matrix->affine;
    DaxElementaryMatrix *em;

    _dax_affine_identity (dst);

    idx = 0;
    while (src[idx]) {
        /* skip initial whitespace */
        while (g_ascii_isspace (src[idx]))
            idx++;

        if (src[idx] == '\0')
            break;

        /* parse keyword */
        for (key_len = 0; key_len < sizeof (keyword); key_len++) {
            char c;

            c = src[idx];
            if (g_ascii_isalpha (c) || c == '-')
                keyword[key_len] = src[idx++];
            else
                break;
        }
        if (key_len >= sizeof (keyword))
            return FALSE;
        keyword[key_len] = '\0';

        /* skip whitespace */
        while (g_ascii_isspace (src[idx]))
            idx++;

        if (src[idx] != '(')
            return FALSE;
        idx++;

        for (n_args = 0;; n_args++) {
            char c;
            char *end_ptr;

            /* skip whitespace */
            while (g_ascii_isspace (src[idx]))
                idx++;
            c = src[idx];
            if (g_ascii_isdigit (c) || c == '+' || c == '-' || c == '.') {
                if (n_args == sizeof (args) / sizeof (args[0]))
                    return FALSE;       /* too many args */
                args[n_args] = g_ascii_strtod (src + idx, &end_ptr);
                idx = end_ptr - src;

                while (g_ascii_isspace (src[idx]))
                    idx++;

                /* skip optional comma */
                if (src[idx] == ',')
                    idx++;
            } else if (c == ')')
                break;
            else
                return FALSE;
        }
        idx++;

        /* ok, have parsed keyword and args, now modify the transform */
        if (!strcmp (keyword, "matrix")) {
            if (n_args != 6)
                return FALSE;
            em = dax_elementary_matrix_new (DAX_MATRIX_TYPE_GENERIC, 0, 0, 0);
            dax_matrix_prepend_elementary (matrix, em);
            _dax_affine_multiply (dst, args, dst);
        } else if (!strcmp (keyword, "translate")) {
            if (n_args == 1)
                args[1] = 0;
            else if (n_args != 2)
                return FALSE;
            em = dax_elementary_matrix_new (DAX_MATRIX_TYPE_TRANSLATE,
                                            args[0],
                                            args[1],
                                            0);
            dax_matrix_prepend_elementary (matrix, em);
            _dax_affine_translate (tmp_affine, args[0], args[1]);
            _dax_affine_multiply (dst, tmp_affine, dst);
        } else if (!strcmp (keyword, "scale")) {
            if (n_args == 1)
                args[1] = args[0];
            else if (n_args != 2)
                return FALSE;
            em = dax_elementary_matrix_new (DAX_MATRIX_TYPE_SCALE,
                                            args[0],
                                            args[1],
                                            0);
            dax_matrix_prepend_elementary (matrix, em);
            _dax_affine_scale (tmp_affine, args[0], args[1]);
            _dax_affine_multiply (dst, tmp_affine, dst);
        } else if (!strcmp (keyword, "rotate")) {
            if (n_args == 1) {
                em = dax_elementary_matrix_new (DAX_MATRIX_TYPE_ROTATE,
                                                args[0],
                                                0,
                                                0);
                dax_matrix_prepend_elementary (matrix, em);
                _dax_affine_rotate (tmp_affine, args[0]);
                _dax_affine_multiply (dst, tmp_affine, dst);
            } else if (n_args == 3) {
                em = dax_elementary_matrix_new (DAX_MATRIX_TYPE_ROTATE_AROUND,
                                                args[0],
                                                args[1],
                                                args[2]);
                dax_matrix_prepend_elementary (matrix, em);
                _dax_affine_translate (tmp_affine, args[1], args[2]);
                _dax_affine_multiply (dst, tmp_affine, dst);

                _dax_affine_rotate (tmp_affine, args[0]);
                _dax_affine_multiply (dst, tmp_affine, dst);

                _dax_affine_translate (tmp_affine, -args[1], -args[2]);
                _dax_affine_multiply (dst, tmp_affine, dst);
            } else
                return FALSE;
        } else if (!strcmp (keyword, "skewX")) {
            if (n_args != 1)
                return FALSE;
            em = dax_elementary_matrix_new (DAX_MATRIX_TYPE_SKEW_X,
                                            args[0],
                                            0,
                                            0);
            dax_matrix_prepend_elementary (matrix, em);
            _dax_affine_shear (tmp_affine, args[0]);
            _dax_affine_multiply (dst, tmp_affine, dst);
        } else if (!strcmp (keyword, "skewY")) {
            if (n_args != 1)
                return FALSE;
            em = dax_elementary_matrix_new (DAX_MATRIX_TYPE_SKEW_X,
                                            args[0],
                                            0,
                                            0);
            dax_matrix_prepend_elementary (matrix, em);
            _dax_affine_shear (tmp_affine, args[0]);
            /* transpose the affine, given that we know [1] is zero */
            tmp_affine[1] = tmp_affine[2];
            tmp_affine[2] = 0;
            _dax_affine_multiply (dst, tmp_affine, dst);
        } else
            return FALSE;       /* unknown keyword */
    }
    matrix->elementary_matrices = g_list_reverse (matrix->elementary_matrices);
    return TRUE;
}

gboolean
dax_matrix_from_string (DaxMatrix   *matrix,
                        const gchar *string)
{
    gboolean res;

    g_return_val_if_fail (matrix != NULL, FALSE);
    g_return_val_if_fail (matrix != NULL, FALSE);

    matrix->refcount = 1;
    matrix->elementary_matrices = NULL;
    res = dax_parse_transform (matrix, string);
    if (res == FALSE)
        dax_matrix_free_elementary_matrices (matrix);

    return res;
}

gchar *
dax_matrix_to_string (const DaxMatrix *matrix)
{
    g_return_val_if_fail (matrix != NULL, NULL);

    return g_strdup_printf ("[%.02lf %.02lf %.02lf %.02lf %.02lf %.02lf]",
                            matrix->affine[0], matrix->affine[1],
                            matrix->affine[2], matrix->affine[3],
                            matrix->affine[4], matrix->affine[5]);
}

void
dax_matrix_transform_point (const DaxMatrix    *m,
                            const ClutterPoint *point,
                            ClutterPoint       *out)
{
    out->x = m->affine[0] * point->x + m->affine[2] * point->y + m->affine[4];
    out->y = m->affine[1] * point->x + m->affine[3] * point->y + m->affine[5];
}

static void
_transform_matrix_string (const GValue *src,
                          GValue       *dest)
{
    gchar *string = dax_matrix_to_string (src->data[0].v_pointer);

    g_value_take_string (dest, string);
}

static void
_transform_string_matrix (const GValue *src,
                          GValue       *dest)
{
    DaxMatrix matrix;

    dax_matrix_from_string (&matrix, g_value_get_string (src));

    dest->data[0].v_pointer = dax_matrix_deep_copy (&matrix);
}

static gboolean
dax_matrices_can_be_interpolated (DaxMatrix *a,
                                  DaxMatrix *b)
{
    GList *cur_a, *cur_b;

    if (g_list_length (a->elementary_matrices) !=
        g_list_length (b->elementary_matrices) &&
        g_list_length (a->elementary_matrices) != 0)
    {
        return FALSE;
    }

    for (cur_a = a->elementary_matrices, cur_b = b->elementary_matrices;
         cur_a;
         cur_a = g_list_next (cur_a), cur_b = g_list_next (cur_b))
    {
        DaxElementaryMatrix *em_a = cur_a->data;
        DaxElementaryMatrix *em_b = cur_b->data;

        if (em_a->type != em_b->type)
            return FALSE;
    }

    return TRUE;
}

static gboolean
dax_matrix_progress (const GValue *a,
                     const GValue *b,
                     gdouble       progress,
                     GValue       *retval)
{
    DaxMatrix *matrix_a = g_value_get_boxed (a);
    DaxMatrix *matrix_b = g_value_get_boxed (b);
    DaxElementaryMatrix *em_a;
    DaxElementaryMatrix *em_b;
    DaxMatrix matrix, *result;
    double tmp_affine[6], result_affine[6];

    if (dax_matrices_can_be_interpolated (matrix_a, matrix_b) == FALSE) {
        /* FIXME: more useful message... */
        g_warning ("Can not interpolate between those two matrices");
        return FALSE;
    }

    _dax_affine_identity (result_affine);

    /* FIXME: should really support interpolating between two lists of
     * similar elementary matrices */
    em_a = matrix_a->elementary_matrices->data;
    em_b = matrix_b->elementary_matrices->data;

    switch (em_b->type) {
    case DAX_MATRIX_TYPE_ROTATE:
        {
            float angle, angle_a, angle_b;

            angle_a = em_a->params[0];
            angle_b = em_b->params[0];
            angle = progress * (angle_b - angle_a) + angle_a;

            _dax_affine_rotate (tmp_affine, angle);
            _dax_affine_multiply (result_affine, tmp_affine, result_affine);
        }
        break;
    case DAX_MATRIX_TYPE_GENERIC:
    case DAX_MATRIX_TYPE_ROTATE_AROUND:
    case DAX_MATRIX_TYPE_TRANSLATE:
    case DAX_MATRIX_TYPE_SCALE:
    case DAX_MATRIX_TYPE_SKEW_X:
    case DAX_MATRIX_TYPE_SKEW_Y:
    default:
        g_assert_not_reached ();
    }

    dax_matrix_from_array (&matrix, result_affine);
    result = dax_matrix_deep_copy (&matrix);
    g_value_set_boxed (retval, result);

    return TRUE;
}

GType
dax_matrix_get_type (void)
{
    static volatile gsize dax_matrix_type__volatile = 0;

    if (g_once_init_enter (&dax_matrix_type__volatile)) {
        GType dax_matrix_type =
            g_boxed_type_register_static (I_("DaxMatrix"),
                                          (GBoxedCopyFunc)dax_matrix_copy,
                                          (GBoxedFreeFunc)dax_matrix_free);

        g_value_register_transform_func (dax_matrix_type,
                                         G_TYPE_STRING,
                                         _transform_matrix_string);
        g_value_register_transform_func (G_TYPE_STRING,
                                         dax_matrix_type,
                                         _transform_string_matrix);
        clutter_interval_register_progress_func (dax_matrix_type,
                                                 dax_matrix_progress);
        g_once_init_leave (&dax_matrix_type__volatile,
                           dax_matrix_type);
    }

    return dax_matrix_type__volatile;
}

/*
 * DaxDuration
 */

DaxDurationType
dax_duration_get_unit_type (const DaxDuration *dur)
{
    g_return_val_if_fail (dur, DAX_DURATION_MS);

    return dur->unit_type;
}

gfloat
dax_duration_get_unit_value (const DaxDuration *dur)
{
    g_return_val_if_fail (dur, 0.0f);

    return dur->value;
}

DaxDuration *
dax_duration_copy (const DaxDuration *dur)
{
    if (dur == NULL)
        return NULL;

    return g_slice_dup (DaxDuration, dur);
}

void
dax_duration_free (DaxDuration *dur)
{
    if (dur == NULL)
        return;

    g_slice_free (DaxDuration, dur);
}

void
dax_duration_from_s (DaxDuration *dur,
                     gfloat       seconds)
{
    g_return_if_fail (dur != NULL);

    dur->unit_type = DAX_DURATION_S;
    dur->value = seconds;
    dur->ms = seconds * 1000;
    dur->ms_set = TRUE;
}

void
dax_duration_from_ms (DaxDuration *dur,
                      gfloat       ms)
{
    g_return_if_fail (dur != NULL);

    dur->unit_type = DAX_DURATION_MS;
    dur->value = ms;
    dur->ms = ms;
    dur->ms_set = TRUE;
}

gfloat
dax_duration_to_ms (DaxDuration *dur)
{
    g_return_val_if_fail (dur != NULL, 0.0);

    if (dur->ms_set)
        return dur->ms;

    switch (dur->unit_type) {
    case DAX_DURATION_S:
        dur->ms = dur->value * 1000;
        break;
    case DAX_DURATION_MS:
        dur->ms = dur->value;
        break;
    default:
        g_assert_not_reached ();
    }

    dur->ms_set = TRUE;

    return dur->ms;
}

gboolean
dax_duration_from_string (DaxDuration *dur,
                          const gchar *string)
{
    DaxDurationType unit_type;
    gfloat value;
    gchar *str = (gchar *)string;

    g_return_val_if_fail (dur != NULL, FALSE);
    g_return_val_if_fail (str != NULL, FALSE);

    while (g_ascii_isspace (*str))
        str++;

    if (_dax_utils_parse_simple_float (&str, &value) == FALSE)
        return FALSE;

    while (g_ascii_isspace (*str))
        str++;

    /* default to seconds (SVG Tiny 1.2 clock-value p. 226) */
    if (*str == '\0')
        unit_type = DAX_DURATION_S;
    else if (*str == 's') {
        unit_type = DAX_DURATION_S;
        str++;
    } else if (strncmp (str, "ms", 2) == 0) {
        unit_type = DAX_DURATION_MS;
        str += 2;
    } else
        return FALSE;

    /* ensure the unit is only followed by white space */
    while (g_ascii_isspace (*str))
        str++;
    if (*str != '\0')
        return FALSE;

    dur->unit_type = unit_type;
    dur->value = value;
    dur->ms_set = FALSE;

    return TRUE;
}

static const gchar *
dax_duration_type_name (DaxDurationType unit_type)
{
    switch (unit_type) {
    case DAX_DURATION_MS:
        return "ms";
    case DAX_DURATION_S:
        return "s";
    default:
        g_assert_not_reached ();
        return "<invalid>";
    }
}

gchar *
dax_duration_to_string (const DaxDuration *dur)
{
    g_return_val_if_fail (dur != NULL, NULL);

    return g_strdup_printf ("%.02f%s",
                            dur->value,
                            dax_duration_type_name (dur->unit_type));
}

static void
_transform_duration_string (const GValue *src,
                            GValue       *dest)
{
    gchar *string = dax_duration_to_string (src->data[0].v_pointer);

    g_value_take_string (dest, string);
}

static void
_transform_string_duration (const GValue *src,
                            GValue       *dest)
{
    DaxDuration duration = { DAX_DURATION_MS, 0.0f };

    dax_duration_from_string (&duration, g_value_get_string (src));

    dest->data[0].v_pointer = dax_duration_copy (&duration);
}

GType
dax_duration_get_type (void)
{
    static volatile gsize dax_duration_type__volatile = 0;

    if (g_once_init_enter (&dax_duration_type__volatile)) {
        GType dax_duration_type =
            g_boxed_type_register_static (I_("DaxDuration"),
                                          (GBoxedCopyFunc)dax_duration_copy,
                                          (GBoxedFreeFunc)dax_duration_free);

        g_value_register_transform_func (dax_duration_type, G_TYPE_STRING,
                                         _transform_duration_string);
        g_value_register_transform_func (G_TYPE_STRING, dax_duration_type,
                                         _transform_string_duration);
        g_once_init_leave (&dax_duration_type__volatile,
                           dax_duration_type);
    }

    return dax_duration_type__volatile;
}

/*
 * DaxRepeatCount
 */

DaxRepeatCount *
dax_repeat_count_copy (const DaxRepeatCount *count)
{
    if (count == NULL)
        return NULL;

    return g_slice_dup (DaxRepeatCount, count);
}

void
dax_repeat_count_free (DaxRepeatCount *count)
{
    if (count == NULL)
        return;

    g_slice_free (DaxRepeatCount, count);
}

void
dax_repeat_count_from_number (DaxRepeatCount *count,
                              gfloat          nb)
{
    g_return_if_fail (count != NULL);

    count->value = nb;
}

gboolean
dax_repeat_count_is_indefinite (const DaxRepeatCount *count)
{
    g_return_val_if_fail (count != NULL, 0.0f);

    return count->value == 0.0f;
}

gfloat
dax_repeat_count_get_value (const DaxRepeatCount *count)
{
    g_return_val_if_fail (count != NULL, 0.0f);

    return count->value;
}

gboolean
dax_repeat_count_from_string (DaxRepeatCount *count,
                              const gchar    *string)
{
    gfloat value;
    gchar *str = (gchar *)string;

    g_return_val_if_fail (count != NULL, FALSE);
    g_return_val_if_fail (count != NULL, FALSE);

    while (g_ascii_isspace (*str))
        str++;

    if (strncmp (str, "indefinite", 10) == 0) {
        value = 0.0f;
        str += 10;
    } else if (_dax_utils_parse_simple_float (&str, &value)) {
        /* value and str are updated by parse_simple_float () */
    } else
        return FALSE;

    /* ensure the unit is only followed by white space */
    while (g_ascii_isspace (*str))
        str++;
    if (*str != '\0')
        return FALSE;

    count->value = value;

    return TRUE;
}

gchar *
dax_repeat_count_to_string (const DaxRepeatCount *count)
{
    static char indefinite[] = "indefinite";

    g_return_val_if_fail (count != NULL, indefinite);

    if (count->value == 0.0f)
        return g_strdup (indefinite);

    return g_strdup_printf ("%.02f", count->value);
}

static void
_transform_repeat_count_string (const GValue *src,
                                GValue       *dest)
{
    gchar *string = dax_repeat_count_to_string (src->data[0].v_pointer);

    g_value_take_string (dest, string);
}

static void
_transform_string_repeat_count (const GValue *src,
                                GValue       *dest)
{
    DaxRepeatCount repeat_count = { 0.0f };

    dax_repeat_count_from_string (&repeat_count, g_value_get_string (src));

    dest->data[0].v_pointer = dax_repeat_count_copy (&repeat_count);
}

GType
dax_repeat_count_get_type (void)
{
    static volatile gsize dax_repeat_count_type__volatile = 0;

    if (g_once_init_enter (&dax_repeat_count_type__volatile)) {
        GType dax_repeat_count_type =
            g_boxed_type_register_static (I_("DaxRepeatCount"),
                                          (GBoxedCopyFunc)dax_repeat_count_copy,
                                          (GBoxedFreeFunc)dax_repeat_count_free);

        g_value_register_transform_func (dax_repeat_count_type,
                                         G_TYPE_STRING,
                                         _transform_repeat_count_string);
        g_value_register_transform_func (G_TYPE_STRING,
                                         dax_repeat_count_type,
                                         _transform_string_repeat_count);
        g_once_init_leave (&dax_repeat_count_type__volatile,
                           dax_repeat_count_type);
    }

    return dax_repeat_count_type__volatile;
}

/*
 * DaxPreserveAspectRatio
 */

DaxPreserveAspectRatio *
dax_preserve_aspect_ratio_copy (const DaxPreserveAspectRatio *ar)
{
    if (ar == NULL)
        return NULL;

    return g_slice_dup (DaxPreserveAspectRatio, ar);
}

void
dax_preserve_aspect_ratio_free (DaxPreserveAspectRatio *ar)
{
    if (ar == NULL)
        return;

    g_slice_free (DaxPreserveAspectRatio, ar);
}

gboolean
dax_preserve_aspect_ratio_from_string (DaxPreserveAspectRatio *ar,
                                       const gchar            *string)
{
    gchar old_char, *str, *string_start = g_strdup (string);
    gchar *enum_start;
    gboolean ret = TRUE;

    g_return_val_if_fail (ar != NULL, FALSE);
    g_return_val_if_fail (string != NULL, FALSE);

    str = string_start;
    ar->flags = 0;

    while (g_ascii_isspace (*str))
        str++;

    if (g_str_has_prefix (str, "defer")) {
        ar->flags |= DAX_PRESERVE_ASPECT_RATIO_FLAG_DEFER;
        str += 5;
        while (g_ascii_isspace (*str))
            str++;
    }

    enum_start = str;
    while (*str && !g_ascii_isspace (*str))
        str++;
    old_char = *str;
    *str = '\0';
    if (!dax_string_to_enum (DAX_TYPE_PRESERVE_ASPECT_RATIO_ALIGN,
                             enum_start,
                             (gint *)&ar->align))
    {
        g_warning (G_STRLOC ": could not parse align value \"%s\", using "
                   "default", enum_start);
        ar->align = DAX_PRESERVE_ASPECT_RATIO_ALIGN_DEFAULT;
    }
    *str = old_char;

    while (g_ascii_isspace (*str))
        str++;

    if (g_str_has_prefix (str, "meet")) {
        ar->flags |= DAX_PRESERVE_ASPECT_RATIO_FLAG_MEET;
        str += 4;
        while (g_ascii_isspace (*str))
            str++;
    }

    /* trailing spaces are OK, but not extra characters */
    if (*str != '\0')
        ret = FALSE;

    g_free (string_start);

    return ret;
}

gchar *
dax_preserve_aspect_ratio_to_string (DaxPreserveAspectRatio *ar)
{
    GString *str;

    str = g_string_new (NULL);
    if (ar->flags & DAX_PRESERVE_ASPECT_RATIO_FLAG_DEFER)
        g_string_append (str, "defer ");
    g_string_append (str,
                     dax_enum_to_string (DAX_TYPE_PRESERVE_ASPECT_RATIO_ALIGN,
                                         ar->align));
    if (ar->flags & DAX_PRESERVE_ASPECT_RATIO_FLAG_MEET)
        g_string_append (str, " meet");

    return g_string_free (str, FALSE);
}

static void
_transform_preserve_ar_string (const GValue *src,
                               GValue       *dest)
{
    gchar *string =
        dax_preserve_aspect_ratio_to_string (src->data[0].v_pointer);

    g_value_take_string (dest, string);
}

static void
_transform_string_preserve_ar(const GValue *src,
                               GValue       *dest)
{
    DaxPreserveAspectRatio ar = { 0, 0.0f };

    dax_preserve_aspect_ratio_from_string (&ar, g_value_get_string (src));

    dest->data[0].v_pointer = dax_preserve_aspect_ratio_copy (&ar);
}

GType
dax_preserve_aspect_ratio_get_type (void)
{
    static volatile gsize dax_preserve_ar_type__volatile = 0;

    if (g_once_init_enter (&dax_preserve_ar_type__volatile)) {
        GType dax_preserve_ar_type =
            g_boxed_type_register_static (I_("DaxPreserveAspectRatio"),
                                          (GBoxedCopyFunc)dax_preserve_aspect_ratio_copy,
                                          (GBoxedFreeFunc)dax_preserve_aspect_ratio_free);

        g_value_register_transform_func (dax_preserve_ar_type,
                                         G_TYPE_STRING,
                                         _transform_preserve_ar_string);
        g_value_register_transform_func (G_TYPE_STRING,
                                         dax_preserve_ar_type,
                                         _transform_string_preserve_ar);
        g_once_init_leave (&dax_preserve_ar_type__volatile,
                           dax_preserve_ar_type);
    }

    return dax_preserve_ar_type__volatile;
}

void
dax_preserve_aspect_ratio_set_default (DaxPreserveAspectRatio *par)
{
    g_return_if_fail (par);

    par->flags = 0;
    par->align = DAX_PRESERVE_ASPECT_RATIO_ALIGN_DEFAULT;
}

gboolean
dax_preserve_aspect_ratio_has_defer (DaxPreserveAspectRatio *ar)
{
    return ar->flags & DAX_PRESERVE_ASPECT_RATIO_FLAG_DEFER;
}

gboolean
dax_preserve_aspect_ratio_has_meet (DaxPreserveAspectRatio *ar)
{
    return ar->flags & DAX_PRESERVE_ASPECT_RATIO_FLAG_MEET;
}
