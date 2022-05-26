/* C17 - ISO/IEC 9899:2018 standard revision */

/*
 * A GIMP palette generator
 * Copyright (C) 2022 Márcio Silva <coadde@hyperbola.info>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern int errno;

enum {
    FALSE,
    TRUE
};

/*
 * All variables/properties and fuctions/methods
 * are created by structure style,
 * to set a better code style.
 */

/*
 * This structure (map) with "new" function pointers is required
 * to generate a new structure (class).
 */

/* This structure emulate a class interface. */
struct GPLGenC {
    struct {
        unsigned char bchn_lmin1;
        unsigned short headerl: 11;
        unsigned long long cpall: 33;
    } buffer_s;

    struct {
        struct {
            unsigned char typel;
            char *type_gp;
            unsigned char depthl: 2;
            unsigned char depth_a[3];
        } colour_s;

        struct {
            unsigned char titlel;
            char *title_gp;
            unsigned char columns;
        } pmap_s;

        struct {
            unsigned char authorl;
            char *author_gp;
            unsigned char yearsl;
            char *years_gp;
        } copyright_s;
    } data_s;

    struct {
        int code: 5;
        unsigned char valuel;
        char *value_gp;
    } error_s;

    struct Input {
        unsigned char typel;
        char type_g[1 << 8];
        unsigned char depthl;
        char depth_g[1 << 8];
        unsigned char titlel;
        char title_g[1 << 8];
        unsigned char columnsl;
        char columns_g[1 << 8];
        unsigned char authorl;
        char author_g[1 << 8];
        unsigned char yearsl;
        char years_g[1 << 8];
    } input_s;

    unsigned char *(*bchn_lgen_mp)(struct GPLGenC *, unsigned char);
    char *(*header_lgen_mp)(struct GPLGenC *);
    char *(*cpal_lgen_mp)(struct GPLGenC *);
    void (*finput_mp)(struct GPLGenC *, FILE *);
    void (*run_mp)(struct GPLGenC *, int, char *[]);
};


/* This function emulate a class with constructor. */
struct GPLGenC
GPLGenC(int argc, char *arg_gap[])
{
    unsigned char *
    bchn_lgen_f(struct GPLGenC *self, unsigned char byte) {
        if (byte<1 || byte>8)
            byte = 8;

        /*
         * C does not have a exponent/power operator.
         * 2 ** x == 1 << x
         * 2**x - 1 == (1 << x) - 1
         */
        unsigned char quantity = (1 << byte) - 1;
        unsigned char max = (1 << 8) - 1;

        /* Declare and add dynamic memory to the channel array. */
        unsigned char *channel_ap = malloc(sizeof(char) * (quantity + 1));
        memset(channel_ap, 0, sizeof(char) * (quantity + 1));

        /*
         * Round half away from zero division for integer calcutation:
         * https://www.calculator.net/rounding-calculator.html
         *
         * Note: // (floor division to)
         *       %/ (round division to)
         *       == (equal to)
         *       != (not equal to)
         *       x (dividend)
         *       y (divisor)
         *       k (result)
         *       r (remainder)
         *       f (remainder float)
         *       ==> (change to)
         *       -> (such as)
         *       && (and)
         *
         * Integers calculation:
         * x//y == k(floor)
         * x//y != k(round)
         * x&/y != k(floor)
         * x&/y == k(round)
         * x%y == r(int)
         * k(floor) != k(round)
         * (x + r(int))//y == k(round)
         * (x + x%y)//y == k(round)
         * (x + x%y)//y == k(round) ==> x + x%y == k(round)*y
         * x + x%y == k(round)*y ==> x%y == k(round)*y - x
         * x%y == x&/y*y - x
         *
         * ((+x) + (+r))//(+y) == +k(floor)+1
         *   -> +k >= +k(floor) + 0.5 (from x/y == k) && r > 0
         * ((+x) + (+r))//(+y) == +k(floor)+0
         *   -> +k <  +k(floor) + 0.5 (from x/y == k) && r > 0
         * ((-x) + (+r))//(-y) == +k(floor)+1
         *   -> +k >= +k(floor) + 0.5 (from x/y == k) && r > 0
         * ((-x) + (+r))//(-y) == +k(floor)+0
         *   -> +k <  +k(floor) + 0.5 (from x/y == k) && r > 0
         *
         * ((+x) + (-r))//(-y) == -k(floor)-1
         *   -> -k >= -k(floor) - 0.5 (from x/y == k) && r < 0
         * ((+x) + (-r))//(-y) == -k(floor)-0
         *   -> -k <  -k(floor) - 0.5 (from x/y == k) && r < 0
         * ((-x) + (-r))//(+y) == -k(floor)-1
         *   -> -k >= -k(floor) - 0.5 (from x/y == k) && r < 0
         * ((-x) + (-r))//(+y) == -k(floor)-0
         *   -> -k <  -k(floor) - 0.5 (from x/y == k) && r < 0
         *
         * ---
         * (x + x%y)//y == k(round)
         * ---
         *
         * Integers calcutation:  (x + x%y)/y
         */
        for (unsigned char index = 0; index <= quantity; index++) {
            /* Integers calcutation:  (x + x%y)/y */
            channel_ap[index] = (
                (index*max + index*max%quantity)/quantity
            );
        }

        /* Save the lenght and return the channel array. */
        self->buffer_s.bchn_lmin1 = quantity;
        byte, quantity = 0, 0;
        return channel_ap;
    }

    char *
    header_lgen_f(struct GPLGenC *self) {
        /* Default values. */
        struct {
            unsigned char authorl;
            char *author_gp;
            unsigned char columns;
            unsigned char columns_lmin1: 2;
            unsigned char *columns_gp;
            unsigned char titlel;
            char *title_gp;
            unsigned char yearsl;
            char *years_gp;
        } tmp_s = {
            self->data_s.copyright_s.authorl,
            self->data_s.copyright_s.author_gp,
            self->data_s.pmap_s.columns,
            0,
            NULL,
            self->data_s.pmap_s.titlel,
            self->data_s.pmap_s.title_gp,
            self->data_s.copyright_s.yearsl,
            self->data_s.copyright_s.years_gp
        };

        /* Set columns values. */
        if (tmp_s.columns < 10)
            tmp_s.columns_lmin1 = 1;
        else if (tmp_s.columns < 100)
            tmp_s.columns_lmin1 = 2;
        else
            tmp_s.columns_lmin1 = 3;

        /* Declare and add dynamic memory to the columns string. */
        tmp_s.columns_gp = (
            malloc(sizeof(char) * (tmp_s.columns_lmin1 + 1))
        );
        sprintf(tmp_s.columns_gp, "%hu", tmp_s.columns);

        /* Default string values. */
        struct {
            char line1_g[13];
            char line2a_g[7];
            char line3a_g[10];
            char line5a_g[14];
            char line5c_g[5];
            char line6a_g[36];
            char line6b_g[44];
            char line7a_g[40];
            char line7b_g[36];
            char line8a_g[13];
            char line8b_g[52];
            char line9a_g[34];
            char line9b_g[43];
            char line10_g[34];
            char line11_g[56];
            char newline_g[2];
            char n_sing_g[2];
            unsigned short lenght: 11;
        } str_s = {
            "GIMP Palette",
            "Name: ",
            "Columns: ",
            "# Written in ",
            " by ",
            "# To the extent possible under law,",
            " the author(s) have dedicated all copyright",
            "# and related and neighboring rights to",
            " this software to the public domain",
            "# worldwide.",
            " This software is distributed without any warranty.",
            "# You should have received a copy",
            " of the CC0 Public Domain Dedication along",
            "# with this software. If not, see",
            "# <https://creativecommons.org/publicdomain/zero/1.0/>.",
            {'\n', '\0'},
            {'#', '\0'},
            0
        };

        /* Declare the lenght of the header string. */
        str_s.lenght = (
            sizeof(str_s.line1_g)/sizeof(char) - 1
              + sizeof(str_s.newline_g)/sizeof(char) - 1
              + sizeof(str_s.line2a_g)/sizeof(char) - 1
              + (unsigned short)tmp_s.titlel - 1
              + sizeof(str_s.newline_g)/sizeof(char) - 1
              + sizeof(str_s.line3a_g)/sizeof(char) - 1
              + (unsigned short)tmp_s.columns_lmin1
              + sizeof(str_s.newline_g)/sizeof(char) - 1
              + sizeof(str_s.n_sing_g)/sizeof(char) - 1
              + sizeof(str_s.newline_g)/sizeof(char) - 1
              + sizeof(str_s.line5a_g)/sizeof(char) - 1
              + (unsigned short)tmp_s.yearsl - 1
              + sizeof(str_s.line5c_g)/sizeof(char) - 1
              + (unsigned short)tmp_s.authorl - 1
              + sizeof(str_s.newline_g)/sizeof(char) - 1
              + sizeof(str_s.line6a_g)/sizeof(char) - 1
              + sizeof(str_s.line6b_g)/sizeof(char) - 1
              + sizeof(str_s.newline_g)/sizeof(char) - 1
              + sizeof(str_s.line7b_g)/sizeof(char) - 1
              + sizeof(str_s.line7a_g)/sizeof(char) - 1
              + sizeof(str_s.newline_g)/sizeof(char) - 1
              + sizeof(str_s.line8b_g)/sizeof(char) - 1
              + sizeof(str_s.line8a_g)/sizeof(char) - 1
              + sizeof(str_s.newline_g)/sizeof(char) - 1
              + sizeof(str_s.line9b_g)/sizeof(char) - 1
              + sizeof(str_s.line9a_g)/sizeof(char) - 1
              + sizeof(str_s.newline_g)/sizeof(char) - 1
              + sizeof(str_s.line10_g)/sizeof(char) - 1
              + sizeof(str_s.newline_g)/sizeof(char) - 1
              + sizeof(str_s.line11_g)/sizeof(char) - 1
              + sizeof(str_s.newline_g)/sizeof(char) - 1
              + sizeof(str_s.n_sing_g)/sizeof(char) - 1
              + sizeof(str_s.newline_g)/sizeof(char)
        );

        /* Declare and add dynamic memory to the header string. */
        char *str_gp = malloc(sizeof(char) * str_s.lenght);
        memset(str_gp, '\0', sizeof(char) * str_s.lenght);

        /* Copy all strings to the header string. */
        strcat(str_gp, str_s.line1_g);
        strcat(str_gp, str_s.newline_g);
        strcat(str_gp, str_s.line2a_g);
        strcat(str_gp, tmp_s.title_gp);
        strcat(str_gp, str_s.newline_g);
        strcat(str_gp, str_s.line3a_g);
        strcat(str_gp, tmp_s.columns_gp);
        strcat(str_gp, str_s.newline_g);
        strcat(str_gp, str_s.n_sing_g);
        strcat(str_gp, str_s.newline_g);
        strcat(str_gp, str_s.line5a_g);
        strcat(str_gp, tmp_s.years_gp);
        strcat(str_gp, str_s.line5c_g);
        strcat(str_gp, tmp_s.author_gp);
        strcat(str_gp, str_s.newline_g);
        strcat(str_gp, str_s.line6a_g);
        strcat(str_gp, str_s.line6b_g);
        strcat(str_gp, str_s.newline_g);
        strcat(str_gp, str_s.line7a_g);
        strcat(str_gp, str_s.line7b_g);
        strcat(str_gp, str_s.newline_g);
        strcat(str_gp, str_s.line8a_g);
        strcat(str_gp, str_s.line8b_g);
        strcat(str_gp, str_s.newline_g);
        strcat(str_gp, str_s.line9a_g);
        strcat(str_gp, str_s.line9b_g);
        strcat(str_gp, str_s.newline_g);
        strcat(str_gp, str_s.line10_g);
        strcat(str_gp, str_s.newline_g);
        strcat(str_gp, str_s.line11_g);
        strcat(str_gp, str_s.newline_g);
        strcat(str_gp, str_s.n_sing_g);
        strcat(str_gp, str_s.newline_g);
        str_gp[str_s.lenght - 1] = '\0';

        /* Save the lenght of the header string. */
        self->buffer_s.headerl = str_s.lenght;

        /*
         * Free unused memory.
         * 0 == '\0' == NULL
         */
        memset(
            self->data_s.copyright_s.author_gp,
            '\0',
            sizeof(char) * self->data_s.copyright_s.authorl
        );
        memset(
            self->data_s.pmap_s.title_gp,
            '\0',
            sizeof(char) * self->data_s.pmap_s.titlel
        );
        memset(
            self->data_s.copyright_s.years_gp,
            '\0',
            sizeof(char) * self->data_s.copyright_s.yearsl
        );
        memset(
            tmp_s.columns_gp,
            '\0',
            sizeof(char) * (tmp_s.columns_lmin1 + 1)
        );
        self->data_s.copyright_s.authorl = 0;
        self->data_s.pmap_s.titlel = 0;
        self->data_s.copyright_s.yearsl = 0;
        free(self->data_s.copyright_s.author_gp);
        free(self->data_s.pmap_s.title_gp);
        free(self->data_s.copyright_s.years_gp);
        free(tmp_s.columns_gp);
        self->data_s.copyright_s.author_gp = NULL;
        self->data_s.pmap_s.title_gp = NULL;
        self->data_s.copyright_s.years_gp = NULL;

        /*
         * Clear all unused data.
         * 0 == '\0' == NULL
         */
        memset(&tmp_s, '\0', sizeof(tmp_s));
        memset(&str_s, '\0', sizeof(str_s));
        self->data_s.pmap_s.columns = 0;

        /* Return the header string. */
        return str_gp;
    }

    char *
    cpal_lgen_f(struct GPLGenC *self) {
        struct {
            unsigned char channelsl: 2;
            unsigned char *channels_ap;
            unsigned char typel;
            char *type_gp;
            unsigned short headerl: 11;
            char *header_gp;
            unsigned long long pmapl: 33;
            char *pmap_gp;
        } tmp_s = {
            self->data_s.colour_s.depthl,
            self->data_s.colour_s.depth_a,
            self->data_s.colour_s.typel,
            self->data_s.colour_s.type_gp,
            self->buffer_s.headerl,
            self->header_lgen_mp(self),
            0,
            NULL
        };

        if (strcmp(tmp_s.type_gp, "g") == 0
              || strcmp(tmp_s.type_gp, "gr") == 0
              || strcmp(tmp_s.type_gp, "gry") == 0
              || strcmp(tmp_s.type_gp, "gray") == 0
              || strcmp(tmp_s.type_gp, "grey") == 0) {
            /*
             * Default channel values.
             * 0 == '\0' == NULL
             */
            struct {
                unsigned char *key_ap;
                unsigned char key_lmin1;
                char spaces_g[6];
                unsigned char key_g[4];
            } channels_s;
            memset(&channels_s, '\0', sizeof(channels_s));

            /* Set key colour lenght and array. */
            channels_s.key_ap = (
                self->bchn_lgen_mp(self, tmp_s.channels_ap[0])
            );

            /* Set lenght generated by bchn_lgen_mp(). */
            channels_s.key_lmin1 = self->buffer_s.bchn_lmin1;

            /*
             * Line lenght is:
             * 4-1 + 2-1 + 4-1 + 2-1 + 4-1 + 2-1 + 2-1 + 2-1 == 14
             * tmp_s.pmap_gp lenght is:
             * sizeof(char) * (keys*line + \0)
             */
            tmp_s.pmapl = (
                (channels_s.key_lmin1 + 1)*14 + 1
            );

            /* Declare and add dynamic memory to the pmap string. */
            tmp_s.pmap_gp = malloc(sizeof(char) * tmp_s.pmapl);
            memset(tmp_s.pmap_gp, '\0', sizeof(char) * tmp_s.pmapl);

            for (unsigned char key_index = 0;
                  key_index <= (channels_s.key_lmin1 + 1)-1;
                  key_index++) {
                if (channels_s.key_ap[key_index] <= 9)
                    strcpy(channels_s.spaces_g, "  %hu");
                else if (channels_s.key_ap[key_index] <= 99)
                    strcpy(channels_s.spaces_g, " %hu\0");
                else
                    strcpy(channels_s.spaces_g, "%hu\0\0");

                sprintf(
                    channels_s.key_g,
                    channels_s.spaces_g,
                    channels_s.key_ap[key_index]
                );

                strcat(tmp_s.pmap_gp, channels_s.key_g);
                strcat(tmp_s.pmap_gp, " ");
                strcat(tmp_s.pmap_gp, channels_s.key_g);
                strcat(tmp_s.pmap_gp, " ");
                strcat(tmp_s.pmap_gp, channels_s.key_g);
                strcat(tmp_s.pmap_gp, "\t");
                strcat(tmp_s.pmap_gp, "#");
                strcat(tmp_s.pmap_gp, "\n");
            }

            /*
             * Free unused channels memory.
             * 0 == '\0' == NULL
             */
            memset(
                channels_s.key_ap,
                0,
                sizeof(char) * (channels_s.key_lmin1 + 1)
            );
            free(channels_s.key_ap);

            /*
             * Clear all unused channels data.
             * 0 == '\0' == NULL
             */
            memset(&channels_s, '\0', sizeof(channels_s));
        } else {
            /*
             * Default channel values.
             * 0 == '\0' == NULL
             */
            struct {
                unsigned char *red_ap;
                unsigned char *green_ap;
                unsigned char *blue_ap;
                unsigned char red_lmin1;
                unsigned char green_lmin1;
                unsigned char blue_lmin1;
                char spaces_g[6];
                unsigned char red_g[4];
                unsigned char green_g[4];
                unsigned char blue_g[4];
            } channels_s;
            memset(&channels_s, '\0', sizeof(channels_s));

            /* Set blue colour lenght and array. */
            channels_s.blue_ap = (
                self->bchn_lgen_mp(self, tmp_s.channels_ap[2])
            );

            /* Set lenght generated by bchn_lgen_mp(). */
            channels_s.blue_lmin1 = self->buffer_s.bchn_lmin1;

            /* Set green colour lenght and array. */
            channels_s.green_ap = (
                self->bchn_lgen_mp(self, tmp_s.channels_ap[1])
            );

            /* Set lenght generated by bchn_lgen_mp(). */
            channels_s.green_lmin1 = self->buffer_s.bchn_lmin1;

            /* Set red colour lenght and array. */
            channels_s.red_ap = (
                self->bchn_lgen_mp(self, tmp_s.channels_ap[0])
            );

            /* Set lenght generated by bchn_lgen_mp(). */
            channels_s.red_lmin1 = self->buffer_s.bchn_lmin1;

            /*
             * Line lenght is:
             * 4-1 + 2-1 + 4-1 + 2-1 + 4-1 + 2-1 + 2-1 + 2-1 == 14
             * tmp_s.pmap_gp lenght is:
             * sizeof(char) * (reds*greens*blues*line + \0)
             */
            tmp_s.pmapl = (
                (
                    (channels_s.red_lmin1 + 1)
                      * (channels_s.green_lmin1 + 1)
                      * (channels_s.blue_lmin1 + 1)
                      * 14
                ) + 1
            );

            /* Declare and add dynamic memory to the pmap string. */
            tmp_s.pmap_gp = malloc(sizeof(char) * tmp_s.pmapl);
            memset(tmp_s.pmap_gp, '\0', sizeof(char) * tmp_s.pmapl);

            for (unsigned char blue_index = 0;
                  blue_index <= (channels_s.blue_lmin1 + 1)-1;
                  blue_index++) {
                for (unsigned char green_index = 0;
                      green_index <= (channels_s.green_lmin1 + 1)-1;
                      green_index++) {
                    for (unsigned char red_index = 0;
                           red_index <= (channels_s.red_lmin1 + 1)-1;
                           red_index++) {

                        if (channels_s.blue_ap[blue_index] <= 9)
                            strcpy(channels_s.spaces_g, "  %hu");
                        else if (channels_s.blue_ap[blue_index] <= 99)
                            strcpy(channels_s.spaces_g, " %hu\0");
                        else
                            strcpy(channels_s.spaces_g, "%hu\0\0");

                        sprintf(
                            channels_s.blue_g,
                            channels_s.spaces_g,
                            channels_s.blue_ap[blue_index]
                        );

                        if (channels_s.green_ap[green_index] <= 9)
                            strcpy(channels_s.spaces_g, "  %hu");
                        else if (channels_s.green_ap[green_index] <= 99)
                            strcpy(channels_s.spaces_g, " %hu\0");
                        else
                            strcpy(channels_s.spaces_g, "%hu\0\0");

                        sprintf(
                            channels_s.green_g,
                            channels_s.spaces_g,
                            channels_s.green_ap[green_index]
                        );

                        if (channels_s.red_ap[red_index] <= 9)
                            strcpy(channels_s.spaces_g, "  %hu");
                        else if (channels_s.red_ap[red_index] <= 99)
                            strcpy(channels_s.spaces_g, " %hu\0");
                        else
                            strcpy(channels_s.spaces_g, "%hu\0\0");

                        sprintf(
                            channels_s.red_g,
                            channels_s.spaces_g,
                            channels_s.red_ap[red_index]
                        );

                        strcat(tmp_s.pmap_gp, channels_s.red_g);
                        strcat(tmp_s.pmap_gp, " ");
                        strcat(tmp_s.pmap_gp, channels_s.green_g);
                        strcat(tmp_s.pmap_gp, " ");
                        strcat(tmp_s.pmap_gp, channels_s.blue_g);
                        strcat(tmp_s.pmap_gp, "\t");
                        strcat(tmp_s.pmap_gp, "#");
                        strcat(tmp_s.pmap_gp, "\n");
                    }
                }
            }

            /*
             * Free unused channels memory.
             * 0 == '\0' == NULL
             */
            memset(
                channels_s.blue_ap,
                0,
                sizeof(char) * (channels_s.blue_lmin1 + 1)
            );
            memset(
                channels_s.green_ap,
                0,
                sizeof(char) * (channels_s.green_lmin1 + 1)
            );
            memset(
                channels_s.red_ap,
                0,
                sizeof(char) * (channels_s.red_lmin1 + 1)
            );
            free(channels_s.blue_ap);
            free(channels_s.green_ap);
            free(channels_s.red_ap);

            /*
             * Clear all unused channels data.
             * 0 == '\0' == NULL
             */
            memset(&channels_s, '\0', sizeof(channels_s));
        }
        tmp_s.pmap_gp[tmp_s.pmapl - 1] = '\0';

        /* Save the cpal lenght. */
        self->buffer_s.cpall = (
            self->buffer_s.headerl + tmp_s.pmapl - 1
        );

        /* Declare and add dynamic memory to the cpal string. */
        char *cpal_gp = (
            malloc(sizeof(char) * self->buffer_s.cpall)
        );
        memset(cpal_gp, '\0', sizeof(char) * self->buffer_s.cpall);

        /* Merge the palette string to the header string. */
        strcat(cpal_gp, tmp_s.header_gp);
        strcat(cpal_gp, tmp_s.pmap_gp);

        /*
         * Free unused memory.
         * 0 == '\0' == NULL
         */
        memset(
            self->data_s.colour_s.depth_a,
            0,
            self->data_s.colour_s.depthl
        );
        memset(
            self->data_s.colour_s.type_gp,
            '\0',
            self->data_s.colour_s.typel
        );
        memset(
            tmp_s.header_gp,
            '\0',
            self->buffer_s.headerl
        );
        memset(
            tmp_s.pmap_gp,
            '\0',
            tmp_s.pmapl
        );
        self->data_s.colour_s.depthl = 0;
        self->data_s.colour_s.typel = 0;
        self->buffer_s.headerl = 0;
        free(self->data_s.colour_s.type_gp);
        free(tmp_s.header_gp);
        free(tmp_s.pmap_gp);
        self->data_s.colour_s.type_gp = NULL;

        /*
         * Clear all unused data.
         * 0 == '\0' == NULL
         */
        memset(&tmp_s, '\0', sizeof(tmp_s));

        /* Return the palette string. */
        return cpal_gp;
    }

    void
    finput_f(struct GPLGenC *self, FILE *input_lp) {
        struct {
            char character;
            unsigned char keyl: 3;
            char key_g[8];
            unsigned char valuel;
            char value_g[1 << 8];
            /* 0 is a key type and 1 is a value type. */
            unsigned char type: 1;
            /* 1 to ignore characters. */
            unsigned char ignore: 1;
        } buffer_s;
        memset(&buffer_s, '\0', sizeof(buffer_s));

        while (! feof(input_lp)) {
            if (! buffer_s.type) {
                if (buffer_s.keyl < 7) {
                    if (! buffer_s.ignore) {
                        buffer_s.key_g[buffer_s.keyl] = fgetc(input_lp);

                        switch (buffer_s.key_g[buffer_s.keyl]) {
                            case ' ':
                                buffer_s.key_g[buffer_s.keyl] = '\0';
                                buffer_s.keyl = 7;
                                break;
                            case '\r':
                            case '\n':
                                memset(
                                    buffer_s.key_g,
                                    '\0',
                                    sizeof(buffer_s.key_g)
                                );
                                buffer_s.keyl = 0;
                                break;
                            case '\xff':
                                buffer_s.key_g[buffer_s.keyl] = '\0';
                                break;
                            default:
                                if (buffer_s.keyl == 6)
                                    buffer_s.character = '=';

                                buffer_s.keyl = ++buffer_s.keyl;
                        }
                    } else {
                        buffer_s.character = fgetc(input_lp);

                        switch (buffer_s.character) {
                            case '\r':
                            case '\n':
                                buffer_s.character = '\0';
                                buffer_s.ignore = FALSE;
                                break;
                        }
                    }
                } else {
                    buffer_s.key_g[buffer_s.keyl] = '\0';

                    if (strcmp(buffer_s.key_g, "type") == 0
                          || strcmp(buffer_s.key_g, "depth") == 0
                          || strcmp(buffer_s.key_g, "columns") == 0
                          || strcmp(buffer_s.key_g, "title") == 0
                          || strcmp(buffer_s.key_g, "author") == 0
                          || strcmp(buffer_s.key_g, "years") == 0) {
                        /* Set as value type. */
                        buffer_s.type = TRUE;
                    } else {
                        memset(buffer_s.key_g, '\0', sizeof(buffer_s.key_g));
                        buffer_s.keyl = 0;
                        buffer_s.ignore = TRUE;
                    }
                }
            } else {
                if (buffer_s.valuel < (1 << 8)-1) {
                    if (! buffer_s.ignore) {
                        buffer_s.value_g[buffer_s.valuel] = fgetc(input_lp);

                        switch(buffer_s.value_g[buffer_s.valuel]) {
                            case '=':
                                buffer_s.character = '=';
                                buffer_s.value_g[buffer_s.valuel] = '\0';
                                break;
                            case ' ':
                                if (buffer_s.character == '=') {
                                    buffer_s.character = '\0';
                                    buffer_s.value_g[buffer_s.valuel] = '\0';
                                } else
                                    buffer_s.valuel = ++buffer_s.valuel;

                                break;
                            case '\r':
                            case '\n':
                                buffer_s.value_g[buffer_s.valuel] = '\0';
                                buffer_s.valuel = (1 << 8) - 1;
                                buffer_s.value_g[buffer_s.valuel] = '\0';
                                break;
                            case '\xff':
                                buffer_s.value_g[buffer_s.valuel] = '\0';
                                break;
                            default:
                                if (buffer_s.valuel == (1 << 8) - 2)
                                    buffer_s.character = '!';

                                buffer_s.valuel = ++buffer_s.valuel;
                        }
                    }
                } else {
                    buffer_s.value_g[buffer_s.valuel] = '\0';

                    if (buffer_s.character == '!') {
                        buffer_s.character = '\0';
                        buffer_s.ignore = TRUE;
                    }

                    if (! buffer_s.ignore) {

                        if (strcmp(buffer_s.key_g, "type") == 0) {
                            strcpy(self->input_s.type_g, buffer_s.value_g);
                            //self->input_s.typel = buffer_s.valuel;
                            self->input_s.typel = strlen(buffer_s.value_g) + 1;
                        } else if (strcmp(buffer_s.key_g, "depth") == 0) {
                            strcpy(self->input_s.depth_g, buffer_s.value_g);
                            //self->input_s.depthl = buffer_s.valuel;
                            self->input_s.depthl = strlen(buffer_s.value_g) + 1;
                        } else if (strcmp(buffer_s.key_g, "columns") == 0) {
                            strcpy(self->input_s.columns_g, buffer_s.value_g);
                            //self->input_s.columnsl = buffer_s.valuel;
                            self->input_s.columnsl = strlen(buffer_s.value_g) + 1;
                        } else if (strcmp(buffer_s.key_g, "title") == 0) {
                            strcpy(self->input_s.title_g, buffer_s.value_g);
                            //self->input_s.titlel = buffer_s.valuel;
                            self->input_s.titlel = strlen(buffer_s.value_g) + 1;
                        } else if (strcmp(buffer_s.key_g, "author") == 0) {
                            strcpy(self->input_s.author_g, buffer_s.value_g);
                            //self->input_s.authorl = buffer_s.valuel;
                            self->input_s.authorl = strlen(buffer_s.value_g) + 1;
                        } else if (strcmp(buffer_s.key_g, "years") == 0) {
                            strcpy(self->input_s.years_g, buffer_s.value_g);
                            //self->input_s.yearsl = buffer_s.valuel;
                            self->input_s.yearsl = strlen(buffer_s.value_g) + 1;
                        }

                        memset(
                            buffer_s.key_g,
                            '\0',
                            sizeof(buffer_s.key_g)
                        );
                        buffer_s.keyl = 0;
                        memset(
                            buffer_s.value_g,
                            '\0',
                            sizeof(buffer_s.value_g)
                        );
                        buffer_s.valuel = 0;
                        /* Set as key type. */
                        buffer_s.type = FALSE;
                    } else {
                        buffer_s.character = fgetc(input_lp);

                        switch(buffer_s.character) {
                            case '\r':
                            case '\n':
                                buffer_s.character = '\0';
                                buffer_s.ignore = FALSE;
                                break;
                        }
                    }
                }
            }
        }
    }

    void
    run_f(struct GPLGenC *self, int argc, char *arg_gap[]) {
        /* Default values. */
        struct {
            unsigned char typel: 3;
            char type_g[4];
            unsigned char depthl: 2;
            char depth_a[3];
            unsigned char titlel: 4;
            char title_g[13];
            unsigned char columns: 5;
            unsigned char authorl: 4;
            char author_g[10];
            unsigned char yearsl: 3;
            char years_g[5];
            char code: 1;
            unsigned char valuel: 3;
            char value_g[5];
            time_t tm;
            struct tm *tm_sp;
            char *input_gp;
            char *output_gp;
            FILE *file_lp;
            struct Input *config_sp;
            char *cpal_gp;
            unsigned char isdigit: 1;
        } tmp_s = {
            4,
            "rgb",
            3,
            {3, 3, 2},
            13,
            "GIMP Palette",
            16,
            10,
            "anonymous",
            5,
            "\0\0\0\0",
            0,
            5,
            "none",
            time(NULL),
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            FALSE
        };

        /* Add year value to tmp_s.years_g. */
        tmp_s.tm_sp = localtime(&tmp_s.tm);
        sprintf(tmp_s.years_g, "%hu", tmp_s.tm_sp->tm_year + 1900);

        /* Add dynamic memories to a new structure. */
        self->data_s.colour_s.type_gp = (
            malloc(sizeof(char) * tmp_s.typel)
        );
        self->data_s.pmap_s.title_gp = (
            malloc(sizeof(char) * tmp_s.titlel)
        );
        self->data_s.copyright_s.author_gp = (
            malloc(sizeof(char) * tmp_s.authorl)
        );
        self->data_s.copyright_s.years_gp = (
            malloc(sizeof(char) * tmp_s.yearsl)
        );
        self->error_s.value_gp = (
            malloc(sizeof(char) * tmp_s.valuel)
        );
        memset(
            self->data_s.colour_s.type_gp,
            '\0',
            sizeof(char) * tmp_s.typel
        );
        memset(
            self->data_s.pmap_s.title_gp,
            '\0',
            sizeof(char) * tmp_s.titlel
        );
        memset(
            self->data_s.copyright_s.author_gp,
            '\0',
            sizeof(char) * tmp_s.authorl
        );
        memset(
            self->data_s.copyright_s.years_gp,
            '\0',
            sizeof(char) * tmp_s.yearsl
        );
        memset(
            self->error_s.value_gp,
            '\0',
            sizeof(char) * tmp_s.valuel
        );

        /* Add default values to a new structure. */
        self->data_s.colour_s.typel = tmp_s.typel;
        strcpy(self->data_s.colour_s.type_gp, tmp_s.type_g);
        self->data_s.colour_s.depthl = tmp_s.depthl;
        memcpy(self->data_s.colour_s.depth_a, tmp_s.depth_a, tmp_s.depthl);
        self->data_s.pmap_s.titlel = tmp_s.titlel;
        strcpy(self->data_s.pmap_s.title_gp, tmp_s.title_g);
        self->data_s.pmap_s.columns = tmp_s.columns;
        self->data_s.copyright_s.authorl = tmp_s.authorl;
        strcpy(self->data_s.copyright_s.author_gp, tmp_s.author_g);
        self->data_s.copyright_s.yearsl = tmp_s.yearsl;
        strcpy(self->data_s.copyright_s.years_gp, tmp_s.years_g);
        self->error_s.code = tmp_s.code;
        self->error_s.valuel = tmp_s.valuel;
        strcpy(self->error_s.value_gp, tmp_s.value_g);

        /* Add all functions to a new structure. */
        self->bchn_lgen_mp = &bchn_lgen_f;
        self->header_lgen_mp = &header_lgen_f;
        self->cpal_lgen_mp = &cpal_lgen_f;
        self->finput_mp = &finput_f;
        self->run_mp = &run_f;

        /* This file is in arg_gap[0]. */

        if (argc > 1)
            tmp_s.input_gp = arg_gap[1];

        if (tmp_s.input_gp) {
            tmp_s.file_lp = fopen(tmp_s.input_gp, "r");

            if (tmp_s.file_lp) {
                self->finput_mp(self, tmp_s.file_lp);
                tmp_s.config_sp = &self->input_s;
                fclose(tmp_s.file_lp);
            } else {
                self->error_s.code = errno;
                self->error_s.valuel = strlen(tmp_s.input_gp) + 1;
                self->error_s.value_gp = tmp_s.input_gp;
            }
            tmp_s.file_lp = NULL;
        }
        tmp_s.input_gp = NULL;

        if (tmp_s.config_sp) {
            if (tmp_s.config_sp->type_g) {
                strcpy(
                    self->data_s.colour_s.type_gp,
                    tmp_s.config_sp->type_g
                );

                memset(
                    tmp_s.config_sp->type_g,
                    '\0',
                    sizeof(char) * tmp_s.config_sp->typel
                );
                tmp_s.config_sp->typel = 0;
            }

            if (tmp_s.config_sp->depth_g) {
                struct {
                    char *data_ap;
                    unsigned char lenght: 3;
                } depth_s = {
                    malloc(sizeof(char)),
                    1
                };
                depth_s.data_ap[depth_s.lenght - 1] = (
                    self->data_s.colour_s.depth_a[depth_s.lenght - 1]
                );

                for (unsigned char index = 0;
                      index < tmp_s.config_sp->depthl;
                      index++) {
                    if ((index == 0
                          || index == 2
                          || index == tmp_s.config_sp->depthl - 2)
                          && isdigit(tmp_s.config_sp->depth_g[index])) {
                        depth_s.lenght = index/2 + 1;

                        depth_s.data_ap[depth_s.lenght - 1] = (
                            strtoul(
                                &tmp_s.config_sp->depth_g[index],
                                NULL,
                                10
                            )
                        );

                        if (index < tmp_s.config_sp->depthl - 2) {
                            depth_s.data_ap = (
                                realloc(
                                    depth_s.data_ap,
                                    sizeof(char) * depth_s.lenght + 1
                                )
                            );
                            depth_s.data_ap[depth_s.lenght] = (
                                self->data_s.colour_s.depth_a[depth_s.lenght]
                            );
                        }
                    }

                    if ((index == 1 || index == 3)
                          && tmp_s.config_sp->depth_g[index] != ' ') {
                        break;
                    }
                }

                memcpy(self->data_s.colour_s.depth_a, depth_s.data_ap, depth_s.lenght);

                memset(depth_s.data_ap, 0, sizeof(char) * depth_s.lenght);
                free(depth_s.data_ap);
                memset(&depth_s, 0, sizeof(depth_s));

                memset(
                    tmp_s.config_sp->depth_g,
                    '\0',
                    sizeof(char) * tmp_s.config_sp->depthl
                );
                tmp_s.config_sp->depthl = 0;
            }

            if (tmp_s.config_sp->title_g) {
                strcpy(
                    self->data_s.pmap_s.title_gp,
                    tmp_s.config_sp->title_g
                );

                memset(
                    tmp_s.config_sp->title_g,
                    '\0',
                    sizeof(char) * tmp_s.config_sp->titlel
                );
                tmp_s.config_sp->titlel = 0;
            }

            if (tmp_s.config_sp->columns_g) {
                for (unsigned char index = 0;
                      index < tmp_s.config_sp->columnsl - 1;
                      index++)
                    if (! isdigit(tmp_s.config_sp->columns_g[index]))
                        break;
                    else if (index + 1 == tmp_s.config_sp->columnsl - 1)
                        tmp_s.isdigit = TRUE;

                if (tmp_s.isdigit
                      && strtoul(tmp_s.config_sp->columns_g, NULL, 10) >= 0)
                    self->data_s.pmap_s.columns = (
                        strtoul(tmp_s.config_sp->columns_g, NULL, 10)
                    );

                memset(
                    tmp_s.config_sp->columns_g,
                    '\0',
                    sizeof(char) * tmp_s.config_sp->columnsl
                );
                tmp_s.config_sp->columnsl = 0;
                tmp_s.isdigit = FALSE;
            }

            if (tmp_s.config_sp->author_g) {
                /*
                 * Error in test/input0: malloc(): invalid size (unsorted).
                 * Error in test/input1: works, but eat the last '\n' in header string.
                 */
                /*
                strcpy(
                    self->data_s.copyright_s.author_gp,
                    tmp_s.config_sp->author_g
                );
                */

                memset(
                    tmp_s.config_sp->author_g,
                    '\0',
                    sizeof(char) * tmp_s.config_sp->authorl
                );
                tmp_s.config_sp->authorl = 0;
            }

            if (tmp_s.config_sp->years_g) {
                for (unsigned char index = 0;
                      index < tmp_s.config_sp->yearsl - 1;
                      index++)
                    if (! isdigit(tmp_s.config_sp->years_g[index]))
                        break;
                    else if (index + 1 == tmp_s.config_sp->yearsl - 1)
                        tmp_s.isdigit = TRUE;

                /*
                 * Error in test/input0: works, but eat the last '\n' in header string.
                 * OK in test/input1.
                 */
                /*
                if (tmp_s.isdigit
                      && strtoul(tmp_s.config_sp->years_g, NULL, 10) > 999
                      && strtoul(tmp_s.config_sp->years_g, NULL, 10)
                        < strtoul(tmp_s.years_g, NULL, 10)
                      || ! tmp_s.isdigit)
                    strcpy(
                        self->data_s.copyright_s.years_gp,
                        tmp_s.config_sp->years_g
                    );
                else if (tmp_s.isdigit);
                else
                    strcpy(
                        self->data_s.copyright_s.years_gp,
                        tmp_s.config_sp->years_g
                    );
                */

                memset(
                    tmp_s.config_sp->years_g,
                    '\0',
                    sizeof(char) * tmp_s.config_sp->yearsl
                );
                tmp_s.config_sp->yearsl = 0;
                tmp_s.isdigit = FALSE;
            }
        }

        tmp_s.cpal_gp = self->cpal_lgen_mp(self);
        if (argc > 2)
            tmp_s.output_gp = arg_gap[2];

        if (tmp_s.output_gp) {
            tmp_s.file_lp = fopen(tmp_s.output_gp, "w");
            if (tmp_s.file_lp) {
                fwrite(
                    tmp_s.cpal_gp,
                    sizeof(char),
                    self->buffer_s.cpall,
                    tmp_s.file_lp
                );
                fclose(tmp_s.file_lp);
            } else {
                self->error_s.code = errno;
                self->error_s.valuel = strlen(tmp_s.output_gp) + 1;
                self->error_s.value_gp = tmp_s.output_gp;
                printf("%s\n", tmp_s.cpal_gp);
            }
            tmp_s.file_lp = NULL;
        } else {
            printf("%s\n", tmp_s.cpal_gp);
        }
        tmp_s.output_gp = NULL;

        if (self->error_s.code) {
            struct {
                char code;
                unsigned char valuel;
                char *value_gp;
            } error_s = {
                self->error_s.code,
                self->error_s.valuel,
                self->error_s.value_gp
            };

            /*
             * Commons errors are:
             * ENOENT (2) and EACCES (13)
             */
            fprintf(
                stderr,
                "[Error %hhu]: %s: %s\n",
                error_s.code,
                error_s.value_gp,
                strerror(tmp_s.code)
            );

            /*
             * Free unused error memory.
             * 0 == '\0' == NULL
             */
            memset(
                self->error_s.value_gp,
                '\0',
                sizeof(char) * self->error_s.valuel
            );
            memset(
                error_s.value_gp,
                '\0',
                sizeof(char) * tmp_s.valuel
            );
            self->error_s.code = 0;
            self->error_s.valuel = 0;
            free(self->error_s.value_gp);
            self->error_s.value_gp = NULL;

            /*
             * Clear all unused error data.
             * 0 == '\0' == NULL
             */
            memset(&error_s, '\0', sizeof(error_s));
        }

        /*
         * Free unused memory.
         * 0 == '\0' == NULL
         */
        memset(
            tmp_s.tm_sp,
            0,
            sizeof(*tmp_s.tm_sp)
        );
        /*memset(
            tmp_s.config_sp,
            '\0',
            sizeof(char) * tmp_s.configl
        );*/
        memset(
            tmp_s.cpal_gp,
            '\0',
            sizeof(char) * self->buffer_s.cpall
        );
        self->buffer_s.cpall = 0;
        free(tmp_s.cpal_gp);

        /*
         * Clear all unused data.
         * 0 == '\0' == NULL
         */
        memset(&tmp_s, '\0', sizeof(tmp_s));
    }

    /*
     * This variable definition is required
     * to generate a new structure (class).
     */
    struct GPLGenC structure;

    /*
     * Run this structure function
     * to emulate the class constructor method.
     */
    run_f(&structure, argc, arg_gap);

    /* Return to a new structure as class object. */
    return structure;
}


/* Initial C function. */
int
main(int argc, char *arg_gap[])
{
    GPLGenC(argc, arg_gap);
}
