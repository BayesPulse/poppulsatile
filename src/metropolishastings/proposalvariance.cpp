#include <Rcpp.h>
#include <RcppArmadillo.h>
#include <cmath>
#include "proposalvariance.h"

//
// proposalvariance.cpp
//   Methods for the ProposalVariance class
//
// Author: Matt Mulvahill
// Created: 10/13/17
// Notes:
//   Outstanding questions:
//    - How to handle the two different adjustpv() functions?
//    - Where does this class get implemented?
//    - draw_proposal() takes SD's - straighten this out.
//


// Constructor
void ProposalVariance::ProposalVariance(double initial_pv,
                                        int adjust_at_iter = 500,
                                        int max_iters = 25000); {

  pv             = initial_pv;
  adjust_at_iter = adjust_at_iter;
  max_iters      = max_iters;
  accept_ct      = 0;
  iter_ct        = 0;
  ratio          = 0;

}

// destructor -- need to define here? - nothing so far
ProposalVariance::~ProposalVariance() { }


// Acceptance adjustment routine for one-parameter modified MH
// args: x   = current acceptance rate
//       *X  = current proposal variance
// returns: none -- updates internally
void MarginalProposalVariance::adjustpv(double x, double *X, double target_pv = 0.35)
{

  double y = 1.0 + 1000.0 * pow(target_pv, 3);
  if (y < 0.9) {
    y = 0.9;
    *X *= y;
  } else if (y > 1.1) {
    y = 1.1;
    *X *= y;
  }

}


// Acceptance adjustment routine for two-parameter modified MH
//   used previously for baseline and halflife
// args: [desired?] corr correlation between the two variances
// returns: none -- updates internally
void JointProposalVariance::adjustpv(double corr, double target_pv = 0.25)
{

  // y - new diagonal elements of proposal variance-covariance matrix based on
  // inputs
  double y = 1.0 + 1000.0 * pow(get_ratio() - target_pv, 3);

  if (y < .90) {

    y = .90;
    pv[0][0] *= y;
    pv[1][1] *= y;
    pv[0][1]  = pv[1][0] = corr * sqrt(pv[0][0] * pv[1][1]);

  } else if (y > 1.1) {

    y = 1.1;
    pv[0][0] *= y;
    pv[1][1] *= y;
    pv[0][1]  = pv[1][0] = corr * sqrt(pv[0][0] * pv[1][1]);

  }

}
