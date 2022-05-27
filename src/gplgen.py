#!/usr/bin/env python
# Python 3.8

# A GIMP palette generator
# Copyright (C) 2022 Márcio Silva <coadde@hyperbola.info>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

from datetime import datetime
from sys import argv
from sys import stderr


class GPLGenC:
    __data_d = {
        'colour_d': {
            'type': 'rgb',
            'depth': (3, 3, 2)
        },
        'pmap_d': {
            'title': 'GIMP Palette',
            'columns': 16
        },
        'copyright_d': {
            'author': 'anonymous',
            'years': str(datetime.now().year)
        }
    }

    __error_d = {
        'code': 0,
        'value': 'none'
    }

    def __bchn_lgen_m(self, byte: int = 8) -> tuple:
        if ((type(byte) is str and not byte.isnumeric())
              or type(byte) is not (int or float or complex)
              or (type(byte) is (str or int or float or complex) and complex(byte) < 1)
              or (type(byte) is (str or int or float or complex) and complex(byte) > 8)):
            byte = 8
        elif type(byte) is (str or float or complex):
            byte = int(byte)

        quantity = 2**byte - 1
        max = 2**8 - 1

        # Round half away from zero division for integer calcutation:
        # https://www.calculator.net/rounding-calculator.html
        #
        # Note: // (floor division to)
        #       %/ (round division to)
        #       == (equal to)
        #       != (not equal to)
        #       x (dividend)
        #       y (divisor)
        #       k (result)
        #       r (remainder)
        #       f (remainder float)
        #       ==> (change to)
        #       -> (such as)
        #       && (and)
        #
        # Integers calculation:
        # x//y == k(floor)
        # x//y != k(round)
        # x&/y != k(floor)
        # x&/y == k(round)
        # x%y == r(int)
        # k(floor) != k(round)
        # (x + r(int)) // y == k(round)
        # (x + x%y) // y == k(round)
        # (x + x%y) // y == k(round) ==> x+x%y == k(round)*y
        # x+x%y == k(round)*y ==> x%y == k(round)*y - x
        # x%y == x&/y*y - x
        #
        # ((+x)+(+r)) // (+y) == +k(floor)+1
        #   -> +k >= +k(floor)+0.5 (from x/y == k) && r > 0
        # ((+x)+(+r)) // (+y) == +k(floor)+0
        #   -> +k <  +k(floor)+0.5 (from x/y == k) && r > 0
        # ((-x)+(+r)) // (-y) == +k(floor)+1
        #   -> +k >= +k(floor)+0.5 (from x/y == k) && r > 0
        # ((-x)+(+r)) // (-y) == +k(floor)+0
        #   -> +k <  +k(floor)+0.5 (from x/y == k) && r > 0
        #
        # ((+x)+(-r)) // (-y) == -k(floor)-1
        #   -> -k >= -k(floor)-0.5 (from x/y == k) && r < 0
        # ((+x)+(-r)) // (-y) == -k(floor)-0
        #   -> -k <  -k(floor)-0.5 (from x/y == k) && r < 0
        # ((-x)+(-r)) // (+y) == -k(floor)-1
        #   -> -k >= -k(floor)-0.5 (from x/y == k) && r < 0
        # ((-x)+(-r)) // (+y) == -k(floor)-0
        #   -> -k <  -k(floor)-0.5 (from x/y == k) && r < 0
        #
        # ---
        # (x + x%y) // y == k(round)
        # ---
        #
        # Integer calcutation:  (x + x%y) // y)
        channel_l = []
        for index in range(quantity + 1):
            # Float calcutation:  round(x / y)
            # channel_l.append(round(index * max / quantity))

            # Integer calcutation:  (x + x%y) // y)
            channel_l.append((index*max + index*max%quantity) // quantity)

        del index
        del max
        del quantity

        return tuple(channel_l)

    def __header_lgen_m(self) -> str:
        author = self.__data_d['copyright_d']['author']
        columns = self.__data_d['pmap_d']['columns']
        title = self.__data_d['pmap_d']['title']
        years = self.__data_d['copyright_d']['years']

        return '\n'.join((
            'GIMP Palette',
            f'Name: {title}',
            f'Columns: {columns}',
            '#',
            f'# Written in {years} by {author}',
            '#',
            ' '.join((
                '# To the extent possible under law,',
                'the author(s) have dedicated all copyright'
            )),
            ' '.join((
                '# and related and neighboring rights to',
                'this software to the public domain'
            )),
            '# worldwide. This software is distributed without any warranty.',
            '#',
            ' '.join((
                '# You should have received a copy',
                'of the CC0 Public Domain Dedication along'
            )),
            '# with this software. If not, see',
            '# <https://creativecommons.org/publicdomain/zero/1.0/>.',
            '#' + '\n'
        ))

    def __cpal_lgen_m(self) -> str:
        channels = self.__data_d['colour_d']['depth']
        type = self.__data_d['colour_d']['type']

        palette_map = ''
        if (type == 'g'
              or type == 'gr'
              or type == 'gry'
              or type == 'gray'
              or type == 'grey'):
            for key_index in self.__bchn_lgen_m(channels[0]):
                if key_index <= 9:
                    key_channel = '  ' + str(key_index)
                elif key_index <= 99:
                    key_channel = ' ' + str(key_index)
                else:
                    key_channel = str(key_index)

                palette_map = (
                    palette_map
                      + key_channel + ' '
                      + key_channel + ' '
                      + key_channel + '\t'
                      + '#' + '\n'
                )

            del key_channel
            del key_index
        else:
            for blue_index in self.__bchn_lgen_m(channels[2]):
                for green_index in self.__bchn_lgen_m(channels[1]):
                    for red_index in self.__bchn_lgen_m(channels[0]):
                        if blue_index <= 9:
                            blue_channel = '  ' + str(blue_index)
                        elif blue_index <= 99:
                            blue_channel = ' ' + str(blue_index)
                        else:
                            blue_channel = str(blue_index)

                        if green_index <= 9:
                            green_channel = '  ' + str(green_index)
                        elif green_index <= 99:
                            green_channel = ' ' + str(green_index)
                        else:
                            green_channel = str(green_index)

                        if red_index <= 9:
                            red_channel = '  ' + str(red_index)
                        elif red_index <= 99:
                            red_channel = ' ' + str(red_index)
                        else:
                            red_channel = str(red_index)

                        palette_map = (
                            palette_map
                              + red_channel + ' '
                              + green_channel + ' '
                              + blue_channel + '\t'
                              + '#' + '\n'
                        )

            del blue_channel
            del blue_index
            del green_channel
            del green_index
            del red_channel
            del red_index

        return self.__header_lgen_m() + palette_map

    def run_m(self, args: list = []) -> None:
        # This script is in args[0]
        input = None
        output = None

        if len(args) > 1:
            input = args[1]

        if len(args) > 2:
            output = args[2]

        config_d = None
        if input:
            config_s = set()

            try:
                with open(input, 'rt') as file:
                    for line in file:
                        if len(tuple(line[:-1].split(' = '))) == 2:
                            config_s.add(tuple(line[:-1].split(' = ')))

                    del line

                del file
            except FileNotFoundError:
                self.__error_d['code'] = 2
                self.__error_d['value'] = input
            except PermissionError:
                self.__error_d['code'] = 13
                self.__error_d['value'] = input

            config_d = dict(config_s)
            del config_s

        if config_d:
            if 'type' in config_d.keys():
                self.__data_d['colour_d']['type'] = config_d['type']

            if 'depth' in config_d.keys():
                depth_tmp = []
                for depth_index in config_d['depth'].split(' '):
                    if depth_index.isdigit():
                        depth_tmp.append(int(depth_index))
                    else:
                        depth_tmp.append(8)

                del depth_index

                self.__data_d['colour_d']['depth'] = tuple(depth_tmp)
                del depth_tmp

            if 'title' in config_d.keys():
                self.__data_d['pmap_d']['title'] = config_d['title']

            if ('columns' in config_d.keys()
                  and config_d['columns'].isdigit()
                  and int(config_d['columns']) >= 0):
                self.__data_d['pmap_d']['columns'] = (
                    int(config_d['columns'])
                )

            if 'author' in config_d.keys():
                self.__data_d['copyright_d']['author'] = config_d['author']

            if 'years' in config_d.keys():
                if (config_d['years'].isdigit()
                      and int(config_d['years']) > 999
                      and int(config_d['years']) < datetime.now().year):
                    self.__data_d['copyright_d']['years'] = (
                        int(config_d['years'])
                    )
                elif config_d['years'].isdigit():
                    pass
                else:
                    self.__data_d['copyright_d']['years'] = config_d['years']

        del config_d

        if output:
            try:
                with open(output, 'wt') as file:
                    file.write(self.__cpal_lgen_m())

                del file
            except PermissionError:
                self.__error_d['code'] = 13
                self.__error_d['value'] = output
                print (self.__cpal_lgen_m())

        else:
            print (self.__cpal_lgen_m())

        if self.__error_d['code'] != 0:
            code = self.__error_d['code']
            value = self.__error_d['value']

            if code == 2:
                name = 'FileNotFoundError';
                message = 'No such file or directory';
            elif code == 13:
                name = 'PermissionError';
                message = 'Permission denied';
            else:
                name = 'UnknownError';
                message = 'Unknown error';

            print (
                name + ': [Errno ' + str(code) + '] '
                  + message + ': ' + str(value),
                file=stderr
            )

    def __init__(self, args: list = []) -> None:

        self.run_m(args)


GPLGenC(argv)
