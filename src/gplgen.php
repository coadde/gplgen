#!/usr/bin/env php
<?php
// PHP 7.4

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

define('YEAR', date("Y"));

class GPLGenC {
    private $data_a = [
        'colour_a' => [
            'type' => 'rgb',
            'depth' => [3, 3, 2]
        ],
        'pmap_a' => [
            'title' => 'GIMP Palette',
            'columns' => 16
        ],
        'copyright_a' => [
            'author' => 'anonymous',
            'years' => YEAR
        ],
        'error_a' => [
            'code' => 0,
            'value' => 'none'
        ]
    ];

    final private function bchn_lgen_m($byte = 8) {
        if (! is_numeric($byte)
              or (is_numeric($byte) and floatval($byte) < 1)
              or (is_numeric($byte) and floatval($byte) > 8))
            $byte = 8;
        elseif (is_float($byte)
              or (is_string($byte)
              and is_numeric($byte)
              and strpos($byte, '.')))
            $byte = intval($byte);

        $size = 2**$byte - 1;

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
         * (x + r(int)) // y == k(round)
         * (x + x%y) // y == k(round)
         * (x + x%y) // y == k(round) ==> x+x%y == k(round)*y
         * x+x%y == k(round)*y ==> x%y == k(round)*y - x
         * x%y == x&/y*y - x
         *
         * ((+x)+(+r)) // (+y) == +k(floor)+1
         *   -> +k >= +k(floor)+0.5 (from x/y == k) && r > 0
         * ((+x)+(+r)) // (+y) == +k(floor)+0
         *   -> +k <  +k(floor)+0.5 (from x/y == k) && r > 0
         * ((-x)+(+r)) // (-y) == +k(floor)+1
         *   -> +k >= +k(floor)+0.5 (from x/y == k) && r > 0
         * ((-x)+(+r)) // (-y) == +k(floor)+0
         *   -> +k <  +k(floor)+0.5 (from x/y == k) && r > 0
         *
         * ((+x)+(-r)) // (-y) == -k(floor)-1
         *   -> -k >= -k(floor)-0.5 (from x/y == k) && r < 0
         * ((+x)+(-r)) // (-y) == -k(floor)-0
         *   -> -k <  -k(floor)-0.5 (from x/y == k) && r < 0
         * ((-x)+(-r)) // (+y) == -k(floor)-1
         *   -> -k >= -k(floor)-0.5 (from x/y == k) && r < 0
         * ((-x)+(-r)) // (+y) == -k(floor)-0
         *   -> -k <  -k(floor)-0.5 (from x/y == k) && r < 0
         *
         * ---
         * (x + x%y) // y == k(round)
         * ---
         *
         * Integers calcutation:  floor((x + x%y) / y))
         */
        $channel_a = [];
        for ($index = 0; $index <= $size; $index++) {
            // Float calcutation:  round(x / y, 0, PHP_ROUND_HALF_UP)
            // $channel_a[$index] = \
            //   round($index * 255 / $size, 0, PHP_ROUND_HALF_UP);

            // Integers calcutation:  floor((x + x%y) / y))
            $channel_a[$index] = (
                floor(($index*255 + $index*255%$size) / $size)
            );
       }

        unset($index, $size);

        return $channel_a;
    }

    final private function header_lgen_m() {
        $author = $this->data_a['copyright_a']['author'];
        $columns = $this->data_a['pmap_a']['columns'];
        $title = $this->data_a['pmap_a']['title'];
        $years = $this->data_a['copyright_a']['years'];

        return (
            'GIMP Palette' . PHP_EOL
              . 'Name: ' . $title . PHP_EOL
              . 'Columns: ' . $columns . PHP_EOL
              . '#' . PHP_EOL
              . '# Written in ' . $years . ' by ' . $author . PHP_EOL
              . '# To the extent possible under law,'
              . ' the author(s) have dedicated all copyright' . PHP_EOL
              . '# and related and neighboring rights to'
              . ' this software to the public domain' . PHP_EOL
              . '# worldwide.'
              . ' This software is distributed without any warranty.'
              . PHP_EOL
              . '# You should have received a copy'
              . ' of the CC0 Public Domain Dedication along' . PHP_EOL
              . '# with this software. If not, see' . PHP_EOL
              . '# <https://creativecommons.org/publicdomain/zero/1.0/>.'
              . PHP_EOL
              . '#' . PHP_EOL
        );
    }

    final private function cpal_lgen_m() {
        $channels = $this->data_a['colour_a']['depth'];
        $type = $this->data_a['colour_a']['type'];

        $palette_map = '';
        if ($type == 'g'
              or $type == 'gr'
              or $type == 'gry'
              or $type == 'gray'
              or $type == 'grey') {
            foreach ($this->bchn_lgen_m($channels[0]) as $key_index) {
                if ($key_index <= 9)
                    $key_channel = '  ' . $key_index;
                elseif ($key_index <= 99)
                    $key_channel = ' ' . $key_index;
                else
                    $key_channel = $key_index;

                $palette_map = (
                    $palette_map
                      . $key_channel . ' '
                      . $key_channel . ' '
                      . $key_channel . "\t"
                      . '#' . PHP_EOL
                );
            }

            unset($key_channel, $key_index);
        } else {
            foreach ($this->bchn_lgen_m($channels[2]) as $blue_index)
                foreach ($this->bchn_lgen_m($channels[1]) as $green_index)
                    foreach ($this->bchn_lgen_m($channels[0]) as $red_index) {
                        if ($blue_index <= 9)
                            $blue_channel = '  ' . $blue_index;
                        elseif ($blue_index <= 99)
                            $blue_channel = ' ' . $blue_index;
                        else
                            $blue_channel = $blue_index;

                        if ($green_index <= 9)
                            $green_channel = '  ' . $green_index;
                        elseif ($green_index <= 99)
                            $green_channel = ' ' . $green_index;
                        else
                            $green_channel = $green_index;

                        if ($red_index <= 9)
                            $red_channel = '  ' . $red_index;
                        elseif ($red_index <= 99)
                            $red_channel = ' ' . $red_index;
                        else
                            $red_channel = $red_index;

                        $palette_map = (
                            $palette_map
                              . $red_channel . ' '
                              . $green_channel . ' '
                              . $blue_channel . "\t"
                              . '#' . PHP_EOL
                        );
                    }

            unset($blue_channel, $blue_index);
            unset($green_channel, $green_index);
            unset($red_channel, $red_index);
        }

        return $this->header_lgen_m() . $palette_map;
    }

    final public function run_m($input = null, $output = null) {
        $config_a = null;
        if ($input)
            try {
                if (! is_file($input))
                    throw new Exception('', 2);
                elseif (! is_readable($input))
                    throw new Exception('', 13);
                else {
                    $file = fopen($input, 'rt');

                    $line = [];
                    static $index = 0;
                    while (! feof($file)) {
                        $line[] = str_replace(PHP_EOL, '', fgets($file));

                        $line_tmp = [];
                        foreach (preg_split(
                                  '/\s=\s+/',
                                  $line[$index]
                              ) as $line_index) {
                            $line_tmp[] = $line_index;
                    }
                    if (count($line_tmp) == 2)
                        $config_a[$line_tmp[0]] = $line_tmp[1];
                        $index++;
                    }
                    unset($index, $line, $line_index, $line_tmp);

                    fclose($file);
                    unset($file);
                }
            } catch (Exception $ex) {
                $this->data_a['error_a']['code'] = $ex->getCode();
                $this->data_a['error_a']['value'] = input;
                unset($ex);
            }

        if ($config_a) {
            if ($config_a['type'])
                $this->data_a['colour_a']['type'] = $config_a['type'];

            if ($config_a['depth']) {
                $depth_tmp = [];
                foreach (preg_split(
                          '/\s+/',
                          $config_a['depth'],
                          -1,
                          PREG_SPLIT_NO_EMPTY
                      ) as $depth_index)
                    if (is_numeric($depth_index))
                        $depth_tmp[] = intval($depth_index);
                    else
                        $depth_tmp[] = 8;

                unset($depth_index);

                $this->data_a['colour_a']['depth'] = $depth_tmp;
                unset($depth_tmp);
            }

            if ($config_a['title'])
                $this->data_a['pmap_a']['title'] = $config_a['title'];

            if ($config_a['columns']
                  and is_numeric($config_a['columns'])
                  and intval($config_a['columns']) >= 0)
                $this->data_a['pmap_a']['columns'] = (
                    intval($config_a['columns'])
                );

            if ($config_a['author'])
                $this->data_a['copyright_a']['author'] = $config_a['author'];

            if ($config_a['years'])
                if (is_numeric($config_a['years'])
                      and intval($config_a['years']) >= 1000
                      and intval($config_a['years']) < YEAR)
                    $this->data_a['copyright_a']['years'] = (
                        intval($config_a['years'])
                    );
                elseif (is_numeric($config_a['years']));
                else
                    $this->data_a['copyright_a']['years'] = $config_a['years'];

            unset($config_a);
        }

        if ($output)
            try {
                if (! is_writable($output))
                    throw new Exception('', 13);
                else {
                    $file = fopen($output, 'wt');
                    fwrite($file, $this->cpal_lgen_m());
                    fclose($file);
                    unset($file);
                }
            } catch (Exception $ex) {
                $this->data_a['error_a']['code'] = $ex->getCode();
                $this->data_a['error_a']['value'] = output;
                unset($ex);

                echo $this->cpal_lgen_m() . PHP_EOL;
            }
        else
            echo $this->cpal_lgen_m() . PHP_EOL;

        if ($this->data_a['error_a']['code'] != 0) {
            $code = $this->data_a['error_a']['code'];
            $value = $this->data_a['error_a']['value'];

            switch ($code) {
                case 2:
                    $message = (
                        'failed to open stream: No such file or directory'
                    );
                    break;
                case 13:
                    $message = 'failed to open stream: Permission denied';
                    break;
                default:
                    $message = 'unknown error';
            }

            fwrite(
                STDERR,
                '[Errno ' . strval($code) . ']: ' . strval($value)
                  . ': ' . $message . PHP_EOL
            );

            unset($code, $value);
        }
    }

    final public function __construct($input = null, $output = null) {
        $this->run_m($input, $output);
    }
}


$arg = [null, null];

// This file is in argv[0]

if (count($argv) > 1)
    $arg[0] = $argv[1];

if (count($argv) > 2)
    $arg[1] = $argv[2];

new GPLGenC($arg[0], $arg[1]);
