#!/bin/sh
# POSIX Shell

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

bchn_lgen_f() {
  if [ -z $1 ] || [ $(expr $1 : '-*[0-9][0-9]*$') = 0 ] \
      || [ $1 -lt 1 ] || [ $1 -gt 8 ]; then
    _p1=8
  else
    _p1=$1
  fi

  _cs=$((2**${_p1} - 1))
  unset _p1

  _cm=
  _ci=$((0))
  while [ ${_ci} -le ${_cs} ]; do

    _cm+=$(((${_ci}*255 + ${_cs} / 2) / ${_cs}))' '
    _ci=$((${_ci} + 1))
  done
  unset _ci _cs

  printf '%s\n' ${_cm} | sed 's/ $//'
  unset _cm
}

header_lgen_f() {
  if [ -z $2 ] || [ $(expr $2 : '-*[0-9][0-9]*$') = 0 ] \
      || [ $2 -lt 0 ]; then
    _p2=16
  else
    _p2=$2
  fi

  if [ -z $4 ]; then
    _p4=$(date '+%Y')
  else
    if [ $(expr $2 : '-*[0-9][0-9]*$') -ge 4 ] \
        && [ $2 -ge $(date '+%Y') ]; then
      _p4=$(date '+%Y')
    else
      _p4="$4"
    fi
  fi

  printf 'GIMP Palette\n'
  printf 'Name: %s\n' "${1:-GIMP Palette}"
  printf 'Columns: %s\n' ${_p2}
  printf '#\n'
  printf '# Written in %s by %s\n' "${_p4}" "${3:-anonymous}"
  printf '#\n'
  printf '# To the extent possible under law,'
  printf   ' the author(s) have dedicated all copyright\n'
  printf '# and related'
  printf   ' and neighboring rights to this software to the public domain\n'
  printf '# worldwide. This software is distributed without any warranty.\n'
  printf '#\n'
  printf '# You should have received a copy'
  printf   ' of the CC0 Public Domain Dedication along\n'
  printf '# with this software. If not, see\n'
  printf '# <https://creativecommons.org/publicdomain/zero/1.0/>.\n'
  printf '#\n'

  unset _p2 _4
}

cpal_lgen_f() {
  if [ $1 = 'g' ] || [ $1 = 'gr' ] || [ $1 = 'gry' ] \
      || [ $1 = 'gray' ] || [ $1 = 'grey' ]; then
    header_lgen_f "$3" $4 "$5" "$6"

    for _kib in $(bchn_lgen_f $2); do
      if [ ${_kib} -le 9 ]; then
        _kbv='  '${_kib}
      elif [ ${_kib} -le 99 ]; then
        _kbv=' '${_kib}
      else
        _kbv=${_kib}
      fi

      printf "${_kbv} ${_kbv} ${_kbv}\t#\n"
    done
    unset _kbv _kib
  else
    header_lgen_f "$4" $5 "$6" "$7"

    for _bib in $(bchn_lgen_f $3); do
      for _gib in $(bchn_lgen_f $2); do
        for _rib in $(bchn_lgen_f $1); do
          if [ ${_bib} -le 9 ]; then
            _bbv='  '${_bib}
          elif [ ${_bib} -le 99 ]; then
            _bbv=' '${_bib}
          else
            _bbv=${_bib}
          fi

          if [ ${_gib} -le 9 ]; then
            _gbv='  '${_gib}
          elif [ ${_gib} -le 99 ]; then
            _gbv=' '${_gib}
          else
            _gbv=${_gib}
          fi

          if [ ${_rib} -le 9 ]; then
            _rbv='  '${_rib}
          elif [ ${_rib} -le 99 ]; then
            _rbv=' '${_rib}
          else
            _rbv=${_rib}
          fi

          printf "${_rbv} ${_gbv} ${_bbv}\t#\n"
        done
      done
    done
    unset _bbv _bib _gbv _gib _rbv _rib
  fi
}

gplgen_f() {
  if [ -f "$1" ]; then
    _fv="$1"
  else
    _fv=
  fi

  _ifs=${IFS}
  IFS='
'
  for _iv in $(cat "${_fv}"); do
    case ${_iv} in
    'type ='*) _ctv=$(printf '%s\n' ${_iv} | sed 's/type = //');;
    'depth ='*) _cdv=$(printf '%s\n' ${_iv} | sed 's/depth = //');;
    'columns ='*) _pcv=$(printf '%s\n' ${_iv} | sed 's/columns = //');;
    'title ='*) _ptv=$(printf '%s\n' ${_iv} | sed 's/title = //');;
    'author ='*) _cav=$(printf '%s\n' ${_iv} | sed 's/author = //');;
    'years ='*) _cyv=$(printf '%s\n' ${_iv} | sed 's/years = //');;
    esac
  done
  IFS=${_ifs}
  unset _fv _ifs _iv

  cpal_lgen_f ${_ctv} ${_cdv} "${_ptv}" ${_pcv} "${_cav}" "${_cyv}" > "$2"
  unset _cav _cdv _ctv _cyv _pcv _ptv
}

gplgen_f "$1" "$2"
