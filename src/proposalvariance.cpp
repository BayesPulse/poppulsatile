//// [[Rcpp::depends(RcppArmadillo)]]
//#include <RcppArmadillo.h>
//#include <cmath>
#include "proposalvariance.h"
#include <cstdio>

////
//// proposalvariance.cpp
////   Methods for the ProposalVariance class
////
//// Author: Matt Mulvahill
//// Created: 10/13/17
//// Notes:
////   Outstanding questions:
////    - Where does this class get implemented? -- implementation handled here,
////    instantiation handled in either mcmc function or MMH classes?
////    - draw_proposal() takes SD's - straighten this out.
////    - are defined destructors needed?
//
//
//
//
////----------------------------------------------------------
//// Marginal PV class
////----------------------------------------------------------
//MarginalProposalVariance::MarginalProposalVariance(double initial_pv,
//                                                   int adjust_at_iter   = 500,
//                                                   int max_iters        = 25000,
//                                                   double target_ratio = 0.35)
//{
//
//  adjust_at_iter = adjust_at_iter;
//  max_iters      = max_iters;
//  accept_ct      = 0;
//  iter_ct        = 0;
//  target_ratio  = target_ratio;
//
//  initialize_proposals(initial_pv);
//
//}
//
//
//// Acceptance adjustment routine for one-parameter modified MH
//void ProposalVariance::adjustpv()
//{
//
//  double y = 1.0 + 1000.0 * pow(getratio() - target_ratio, 3);
//  if (y < 0.9)      set_proposals(pv * 0.9);
//  else if (y > 1.1) set_proposals(pv * 1.1);
//
//}
//
//void MarginalProposalVariance::initialize_proposals(double this_pv)
//{
//  set_proposals(this_pv);
//}
//
//void MarginalProposalVariance::set_proposals(double this_pv)
//{
//  pv = this_pv;
//  psd = sqrt(pv);
//}
//
//
//
//
//
////----------------------------------------------------------
//// Joint PV class
////----------------------------------------------------------
//
//// Constructor
//ProposalVariance::ProposalVariance(T initial_pv,
//                                   int adjust_at_iter   = 500,
//                                   int max_iters        = 25000,
//                                   double target_ratio = 0.25)
//{
//
//  adjust_at_iter = adjust_at_iter;
//  max_iters      = max_iters;
//  accept_ct      = 0;
//  iter_ct        = 0;
//  ratio          = 0;
//  target_ratio   = target_ratio;
//
//  // Create pv and psd objects;
//  initialize_proposals(initial_pv);
//
//}
//
//
//// adjustpv()
////
////   Acceptance adjustment routine for two-parameter modified MH used previously
////   for baseline and halflife
////   - args:
////      - corr correlation between the two variances
////   - returns: 
////      - none -- updates internally
//void ProposalVariance::adjustpv(double corr = -0.90)
//{
//
//  arma::mat mydiag(2, 2, arma::fill::eye);
//
//  // y - new diagonal elements of proposal variance-covariance matrix based on
//  // inputs
//  double y = 1.0 + 1000.0 * pow(getratio() - target_ratio, 3);
//
//  if (y < .90) {
//
//    y = .90;
//    pv  = pv % mydiag; // set off-diag to 0 with Schur/Hadamard multiplication
//    set_proposals(pv * y, corr);
//
//  } else if (y > 1.1) {
//
//    y = 1.1;
//    pv  = pv % mydiag; // set off-diag to 0 with Schur/Hadamard multiplication
//    set_proposals(pv * y, corr);
//
//  }
//
//}
//
//
//
//// initialize_pv()
////   create/set pv and psd objects (proposal var-covar and correlation matrices)
//void ProposalVariance::initialize_proposals(arma::vec initial_pv)
//{
//
//  arma::mat this_pv = arma::diagmat(initial_pv);
//  set_proposals(this_pv, -0.90);
//
//}
//
//
//void ProposalVariance::set_proposals(arma::mat this_pv, double corr)
//{
//
//  pv = this_pv;
//  pv(0, 1) = pv(1, 0) = calc_covariance(pv, corr);
//  psd = chol(pv);
//
//}
//
//
//double ProposalVariance::calc_covariance(arma::mat pv, double corr) 
//{
//
//  return corr * sqrt(pv(0, 0) * pv(1, 1));
//
//}
//
//
