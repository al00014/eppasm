// Copyright (C) 2019  Kinh Nguyen

// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.

// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.

// You should have received a copy of the GNU General Public License along
// with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "Classes.h"

void artC::aging (boost2D ag_prob) {
  for (int sex = 0; sex < NG; sex++)
    for (int agr = 0; agr < hAG; agr++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        for (int dur = 0; dur < hTS; dur++)
          data[year][sex][agr][cd4][dur] = data[year-1][sex][agr][cd4][dur];
  double nARTup;
  for (int sex = 0; sex < NG; sex++)
    for (int agr = 0; agr < hAG-1; agr++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        for (int dur = 0; dur < hTS; dur++) {
          nARTup = data[year-1][sex][agr][cd4][dur] * ag_prob[sex][agr];
          data[year][sex][agr][cd4][dur]   -= nARTup;
          data[year][sex][agr+1][cd4][dur] += nARTup;
        }
  if (MODEL==2) {
    for (int sex = 0; sex < NG; sex++)
      for (int agr = 0; agr < hAG; agr++)
        for (int cd4 = 0; cd4 < hDS; cd4++)
          for (int dur = 0; dur < hTS; dur++)
            data_db[year][sex][agr][cd4][dur] = data_db[year-1][sex][agr][cd4][dur];
    for (int sex = 0; sex < NG; sex++)
      for (int agr = 0; agr < hAG - 1; agr++)
        for (int cd4 = 0; cd4 < hDS; cd4++)
          for (int dur = 0; dur < hTS; dur++)  {
            nARTup = data_db[year-1][sex][agr][cd4][dur] * ag_prob[sex][agr];
            data_db[year][sex][agr][cd4][dur]   -= nARTup;
            data_db[year][sex][agr+1][cd4][dur] += nARTup;
          }
  }
}

void artC::add_entrants (boost1D artYesNo) {
  if (MODEL==1)
    for (int sex = 0; sex < NG; sex++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        for (int dur = 0; dur < hTS; dur++)
          data[year][sex][0][cd4][dur] += 
            p.paedsurv_artcd4dist[year][sex][cd4][dur] * artYesNo[sex];
  if (MODEL==2) // add to virgin then debut
    for (int sex = 0; sex < NG; sex++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        for (int dur = 0; dur < hTS; dur++)
          data_db[year][sex][0][cd4][dur] += 
            p.paedsurv_artcd4dist[year][sex][cd4][dur] * artYesNo[sex];
}

void artC::sexual_debut () {
  double debut_art;
  for (int sex = 0; sex < NG; sex++)
    for (int agr = 0; agr < pDB; agr++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        for (int dur = 0; dur < hTS; dur++) {
          debut_art = data_db[year][sex][agr][cd4][dur] * p.db_pr[sex][agr];
          data[year][sex][agr][cd4][dur] += debut_art;
          data_db[year][sex][agr][cd4][dur] -= debut_art;
        }
}

void artC::deaths (boost2D survival_pr) {
  for (int sex = 0; sex < NG; sex++)
    for (int agr = 0; agr < hAG; agr++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        for (int dur = 0; dur < hTS; dur++)
          data[year][sex][agr][cd4][dur] *= survival_pr[sex][agr];
  if (MODEL==2)
    for (int sex = 0; sex < NG; sex++)
      for (int agr = 0; agr < hAG; agr++)
        for (int cd4 = 0; cd4 < hDS; cd4++)
          for (int dur = 0; dur < hTS; dur++)
            data_db[year][sex][agr][cd4][dur] *= survival_pr[sex][agr];
}

void artC::migration (boost2D migration_pr) {
  for (int sex = 0; sex < NG; sex++)
    for (int agr = 0; agr < hAG; agr++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        for (int dur = 0; dur < hTS; dur++)
          data[year][sex][agr][cd4][dur] *= migration_pr[sex][agr];
  if (MODEL==2)
    for (int sex = 0; sex < NG; sex++)
      for (int agr = 0; agr < hAG; agr++)
        for (int cd4 = 0; cd4 < hDS; cd4++)
          for (int dur = 0; dur < hTS; dur++)
            data_db[year][sex][agr][cd4][dur] *= migration_pr[sex][agr];
}

void artC::grad_progress () {
  zeroing(gradART); // reset gradient
  if (MODEL==2)
    zeroing(gradART_db); // reset gradient

  // progression and mortality (HARD CODED 6 months duration)
  for (int sex = 0; sex < NG; sex++)
    for (int agr = 0; agr < hAG; agr++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        for (int dur = 0; dur < hTS - 1; dur++) {
          gradART[sex][agr][cd4][dur]   -= 2.0 * data[year][sex][agr][cd4][dur];
          gradART[sex][agr][cd4][dur+1] += 2.0 * data[year][sex][agr][cd4][dur];
        }
  if (MODEL==2) {
    for (int sex = 0; sex < NG; sex++)
      for (int agr = 0; agr < hAG; agr++)
        for (int cd4 = 0; cd4 < hDS; cd4++)
          for (int dur = 0; dur < hTS - 1; dur++)
            gradART_db[sex][agr][cd4][dur] -=
              2.0 * data_db[year][sex][agr][cd4][dur];
    for (int sex = 0; sex < NG; sex++)
      for (int agr = 0; agr < hAG; agr++)
        for (int cd4 = 0; cd4 < hDS; cd4++)
          for (int dur = 1; dur < hTS; dur++)
            gradART_db[sex][agr][cd4][dur] +=
              2.0 * data_db[year][sex][agr][cd4][dur-1];
  }
  // ART mortality
  for (int sex = 0; sex < NG; sex++)
    for (int agr = 0; agr < hAG; agr++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        for (int dur = 0; dur < hTS; dur++) 
          gradART[sex][agr][cd4][dur] -= 
            p.art_mort[sex][agr][cd4][dur] * p.artmx_timerr[year][dur] * 
            data[year][sex][agr][cd4][dur];
  if (MODEL==2)
    for (int sex = 0; sex < NG; sex++)
      for (int agr = 0; agr < hAG; agr++)
        for (int cd4 = 0; cd4 < hDS; cd4++)
          for (int dur = 0; dur < hTS; dur++) 
            gradART_db[sex][agr][cd4][dur] -= 
              p.art_mort[sex][agr][cd4][dur] * p.artmx_timerr[year][dur] * 
              data_db[year][sex][agr][cd4][dur];
}

void artC::art_dropout (hivC& hivpop) {
  for (int sex = 0; sex < NG; sex++)
    for (int agr = 0; agr < hAG; agr++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        for (int dur = 0; dur < hTS; dur++)
          hivpop.grad[sex][agr][cd4] +=
            data[year][sex][agr][cd4][dur] * p.art_dropout[year];
  for (int sex = 0; sex < NG; sex++)
    for (int agr = 0; agr < hAG; agr++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        for (int dur = 0; dur < hTS; dur++)
          gradART[sex][agr][cd4][dur] -=
            data[year][sex][agr][cd4][dur] * p.art_dropout[year];
  if (MODEL==2) {
    for (int sex = 0; sex < NG; sex++)
      for (int agr = 0; agr < hAG; agr++)
        for (int cd4 = 0; cd4 < hDS; cd4++)
          for (int dur = 0; dur < hTS; dur++)
            hivpop.grad_db[sex][agr][cd4] +=
              data_db[year][sex][agr][cd4][dur] * p.art_dropout[year];
    for (int sex = 0; sex < NG; sex++)
      for (int agr = 0; agr < hAG; agr++)
        for (int cd4 = 0; cd4 < hDS; cd4++)
          for (int dur = 0; dur < hTS; dur++)
            gradART_db[sex][agr][cd4][dur] -=
              data_db[year][sex][agr][cd4][dur] * p.art_dropout[year];
  }
}

boost1D artC::current_on_art () {
  boost1D art_curr(extents[NG]);
  for (int sex = 0; sex < NG; sex++)
    for (int agr = 0; agr < hAG; agr++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        for (int dur = 0; dur < hTS; dur++)
          art_curr[sex] += (data[year][sex][agr][cd4][dur] + 
                               gradART[sex][agr][cd4][dur] * DT);
  if (MODEL==2)
    for (int sex = 0; sex < NG; sex++)
      for (int agr = 0; agr < hAG; agr++)
        for (int cd4 = 0; cd4 < hDS; cd4++)
          for (int dur = 0; dur < hTS; dur++)
            art_curr[sex] += (data_db[year][sex][agr][cd4][dur] + 
                                 gradART_db[sex][agr][cd4][dur] * DT);
  return art_curr;
}

void artC::grad_init (boost3D artinit) { // 7x9x2
  for (int sex = 0; sex < NG; sex++)
    for (int agr = 0; agr < hAG; agr++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        gradART[sex][agr][cd4][0] += artinit[sex][agr][cd4] / DT;
  for (int sex = 0; sex < NG; sex++)
    for (int agr = 0; agr < hAG; agr++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        for (int dur = 0; dur < hTS; dur++)
          data[year][sex][agr][cd4][dur] +=
            DT * gradART[sex][agr][cd4][dur];
}

void artC::grad_db_init (boost3D artinit_db) {
  for (int sex = 0; sex < NG; sex++)
    for (int agr = 0; agr < hAG; agr++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        gradART_db[sex][agr][cd4][0] += artinit_db[sex][agr][cd4] / DT;
  for (int sex = 0; sex < NG; sex++)
    for (int agr = 0; agr < hAG; agr++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        for (int dur = 0; dur < hTS; dur++)
          data_db[year][sex][agr][cd4][dur] +=
             DT * gradART_db[sex][agr][cd4][dur];
}

void artC::adjust_pop (boost2D adj_prob) {
  for (int sex = 0; sex < NG; sex++)
    for (int agr = 0; agr < hAG; agr++)
      for (int cd4 = 0; cd4 < hDS; cd4++)
        for (int dur = 0; dur < hTS; dur++)
          data[year][sex][agr][cd4][dur] *= adj_prob[sex][agr];
  if (MODEL==2)
    for (int sex = 0; sex < NG; sex++)
      for (int agr = 0; agr < hAG; agr++)
        for (int cd4 = 0; cd4 < hDS; cd4++)
          for (int dur = 0; dur < hTS; dur++)
            data_db[year][sex][agr][cd4][dur] *= adj_prob[sex][agr];
}