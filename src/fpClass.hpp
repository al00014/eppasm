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
#include "utils.hpp"
#pragma once

class DemogParam {
public:
  boost2D_ptr basepop;
  double *    births;
  boost3D_ptr Sx;
  boost3D_ptr netmigr;
  boost2D_ptr asfr;
  boost2D_ptr srb;
  boost2D_ptr birthslag;
  boost2D_ptr cumsurv;
  boost2D_ptr cumnetmigr;
  boost3D_ptr targetpop;
  boost2D_ptr entrantpop;
  const bool  flag_popadjust;
  DemogParam(const SEXP& fp);
};

class NaturalHistoryParam {
public:
  boost2D_ptr artmx_timerr;
  boost3D_ptr cd4_initdist;
  boost3D_ptr cd4_prog;
  boost3D_ptr cd4_mort;
  boost3D_ptr frr_cd4;
  boost4D_ptr art_mort;
  boost4D_ptr frr_art;
  NaturalHistoryParam(const SEXP& fp);
};

class ArtData {
public:
  boost2D_ptr art15plus_num;
  boost2D_ptr art15plus_isperc;
  const int    * artcd4elig_idx;  // NOTE: 1-based indexing
  const double * specpop_percelig;
  const double * pw_artelig;
  const double   who34percelig;
  const double * art_dropout;
  const double * median_cd4init;
  const int * med_cd4init_cat;
  const int * med_cd4init_input;
  const int art_alloc_method;
  const double art_alloc_mxweight;
  const int scale_cd4_mort;
  ArtData(const SEXP& fp);
};

class RtrendParam {
public:
  const double * proj_steps;
  const double * rw_start;
  const double * rw_trans;
  const double * rlogistic_steps;
  const double * rw_steps;
  const double * n_rw;
  const double * rw_dk;
  const double * rw_knots;
  const int    * rw_idx;
  const double * n_param;
  const double * rw_transition;
  RtrendParam() {};
  void init_me(const SEXP& fp);
};

class IncidenceParam {
public:
  const int      eppmod; 
  const int      incidmod;
  boost3D_ptr    incrr_age;
  boost2D_ptr    circ_prop;
  boost2D_ptr    mat_m;
  boost2D_ptr    mat_f;
  boost2D_ptr    db_pr;
  const double   relinfectART;
  const double * incrr_sex;
  double         circ_incid_rr;
  const double * incidinput;
  int            incidpopage;
  double         tsEpidemicStart; //ts_epidemic_start;
  double       * proj_steps;
  double         iota;
  const int    * logitiota;
  const double * rvec;
  double         rw_start;
  RtrendParam    rt;
  IncidenceParam(const SEXP& fp);
};

class PaediatricHivParam {
public:
  const double * verttrans_lag;
  const double * paedsurv_lag;
  const double   netmighivsurv;
  const double   netmig_hivprob;
  boost2D_ptr    entrantprev;
  boost2D_ptr    entrantartcov;
  boost3D_ptr    paedsurv_cd4dist;
  boost4D_ptr    paedsurv_artcd4dist;
  PaediatricHivParam(const SEXP& fp);
};

class AncParam {
public:
  const int    * ancsitedata;
  const int    * ancrt;
  const double * ancbias;
  const double * v_infl;
  const double * ancrtcens_vinfl;
  const double * ancrtsite_beta;
  const double * log_frr_adjust;
  AncParam(const SEXP& fp);
};

class Parameters {
public:
  DemogParam          dm;
  NaturalHistoryParam nh;
  ArtData             ad;
  IncidenceParam      ic;
  PaediatricHivParam  ph;
  Parameters(const SEXP& fp);
};

// Master parameters class
class StateSpace {
public:
  int          SIM_YEARS;
  int          year = 1; // simulation year
  int          MODEL;
  bool         MIX;
  SEXP         fp_ss;
  int          proj_start;
  int          PROJ_YEARS;
  int          AGE_START;
  int          hiv_steps_per_year;
  // double       time_epi_start;
  int          NG;
  int          pDS;
  int          m_idx;
  int          f_idx;
  int          hivn_idx;
  int          hivp_idx;
  int          pAG;
  double       ag_rate;
  boost1I_ptr  p_fert_idx;
  boost1I_ptr  p_age15to49_idx;
  boost1I_ptr  p_age15plus_idx;
  boost1D_ptr  h_ag_span;
  int          hAG;
  int          hDS;
  int          hTS;
  boost1I_ptr  ag_idx;
  boost1I_ptr  agfirst_idx;
  boost1I_ptr  aglast_idx;
  boost1I_ptr  h_fert_idx;
  boost1I_ptr  h_age15to49_idx;
  boost1I_ptr  h_age15plus_idx;
  double       DT;
  int          pDB;
  int          hDB;
  int          n_steps;
  int          tARTstart;
  // 
  int pAG_FERT, hAG_FERT, pAG_1549, hAG_1549, pAG_15plus, hAG_15plus;
  StateSpace(const SEXP& fp);
};