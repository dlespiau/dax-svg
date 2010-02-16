/*
 * Castet - Load and draw SVG
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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "castet-internals.h"
#include "castet-paramspec.h"

/*
 * Custom GParamSpec
 *
 * Castet needs custom GParamSpec to add a few features needed for SVG, namely:
 *   * differenciation between properties and attributes,
 *   * handling of inheritance.
 *   * specify if the parameter is animatable
 */

/*
 * CastetParamSpecEnum is derived From glib's gobject/gparamspecs.c
 *
 * Copyright (C) 1997-1999, 2000-2001 Tim Janik and Red Hat, Inc.
 * LGPL v2.1 (it's my option to choose 2.1)
 */
static void
param_enum_init (GParamSpec *pspec)
{
    CastetParamSpecEnum *espec = CASTET_PARAM_SPEC_ENUM (pspec);

    espec->enum_class = NULL;
    espec->default_value = 0;
}

static void
param_enum_finalize (GParamSpec *pspec)
{
    CastetParamSpecEnum *espec = CASTET_PARAM_SPEC_ENUM (pspec);
    GParamSpecClass *parent_class;

    parent_class = g_type_class_peek (g_type_parent (CASTET_TYPE_PARAM_ENUM));

    if (espec->enum_class) {
        g_type_class_unref (espec->enum_class);
        espec->enum_class = NULL;
    }

    parent_class->finalize (pspec);
}

static void
param_enum_set_default (GParamSpec *pspec,
                        GValue     *value)
{
    value->data[0].v_long = CASTET_PARAM_SPEC_ENUM (pspec)->default_value;
}

static gboolean
param_enum_validate (GParamSpec *pspec,
                     GValue     *value)
{
    CastetParamSpecEnum *espec = CASTET_PARAM_SPEC_ENUM (pspec);
    glong oval = value->data[0].v_long;

    if (!espec->enum_class ||
        !g_enum_get_value (espec->enum_class, value->data[0].v_long))
        value->data[0].v_long = espec->default_value;

    return value->data[0].v_long != oval;
}

static gint
param_long_values_cmp (GParamSpec   *pspec,
                       const GValue *value1,
                       const GValue *value2)
{
    if (value1->data[0].v_long < value2->data[0].v_long)
        return -1;
    else
        return value1->data[0].v_long > value2->data[0].v_long;
}

GType
castet_param_enum_get_type (void)
{
    static volatile gsize castet_param_enum_type__volatile = 0;

    if (g_once_init_enter (&castet_param_enum_type__volatile)) {
        GType type;
        static const GParamSpecTypeInfo pspec_info = {
            sizeof (CastetParamSpecEnum),   /* instance_size */
            16,                             /* n_preallocs */
            param_enum_init,                /* instance_init */
            G_TYPE_ENUM,                    /* value_type */
            param_enum_finalize,            /* finalize */
            param_enum_set_default,         /* value_set_default */
            param_enum_validate,            /* value_validate */
            param_long_values_cmp,          /* values_cmp */
        };
        type = g_param_type_register_static (ISS("CastetParamEnum"),
                                             &pspec_info);
        g_once_init_leave (&castet_param_enum_type__volatile, type);
    }

    return castet_param_enum_type__volatile;
}

/**
 * castet_param_spec_enum:
 * @name: canonical name of the property specified
 * @nick: nick name for the property specified
 * @blurb: description of the property specified
 * @enum_type: a #GType derived from %G_TYPE_ENUM
 * @default_value: default value for the property specified
 * @g_flags: flags for the underlying GParamSpec property
 * @castet_flags: flags specific to Castet
 * @namespace_uri: the namespace URI for the parameter. The string given has to
 *                 be an interned string.
 *
 * Creates a new #GParamSpecEnum instance specifying a %G_TYPE_ENUM
 * property.
 *
 * See g_param_spec_internal() for details on property names.
 *
 * Returns: a newly created parameter specification
 */

GParamSpec*
castet_param_spec_enum (const gchar      *name,
                        const gchar      *nick,
                        const gchar      *blurb,
                        GType             enum_type,
                        gint              default_value,
                        GParamFlags       g_flags,
                        CastetParamFlags  castet_flags,
                        const char       *namespace_uri)
{
    CastetParamSpecEnum *castet_enum_spec;
    GEnumClass *enum_class;

    g_return_val_if_fail (G_TYPE_IS_ENUM (enum_type), NULL);

    enum_class = g_type_class_ref (enum_type);

    g_return_val_if_fail (g_enum_get_value (enum_class, default_value) != NULL,
                          NULL);

    castet_enum_spec = g_param_spec_internal (CASTET_TYPE_PARAM_ENUM,
                                              name,
                                              nick,
                                              blurb,
                                              g_flags);

    castet_enum_spec->enum_class = enum_class;
    castet_enum_spec->default_value = default_value;
    castet_enum_spec->flags = castet_flags;
    castet_enum_spec->namespace_uri = namespace_uri;
    G_PARAM_SPEC (castet_enum_spec)->value_type = enum_type;

    return G_PARAM_SPEC (castet_enum_spec);
}
