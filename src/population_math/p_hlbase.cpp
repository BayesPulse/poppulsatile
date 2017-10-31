// Baseline and halflife mcmc

double draw_proposal(double current_sd, double proposal_sd) 
{

  // Draw proposal values for b and hl
  //  via calculations.c
  rmvnorm(pmd, var, 2, subject->basehalf, 1);
  return Rf_rnorm(current_sd, proposal_sd);

}

bool within_parameter_support(double proposal, double min = 0, double max = 0)
{
  return proposal > min;
}

double posterior_function() 
{

}






void draw_bh(Subject_type *sublist,Common_parms *parms,Priors *priors,double **ts,
             int N,unsigned long *seed,double **var)
{

  /*this tells likelihood2 which subject's likelihood to calculate*/
  parms->subindex = 0;

  /*Loop thru all subjects*/
  while (subject != NULL){

    /*Calculate the current likelihood for this subject*/
    current_like = likelihood2(subject->list,ts,parms,N,subject->list,subject->basehalf[0]);

    /*Count number of pulses for this subject*/
    num_node= 0;
    new_node = subject->list->succ;
    while (new_node != NULL) {
      new_node = new_node->succ;
      num_node++;
    }

    /*Allocate memory*/
    pmd = (double *)calloc(2,sizeof(double));
    currentmc = (double **)calloc(num_node,sizeof(double *));
    for (i=0;i<num_node;i++)
      currentmc[i] = (double *)calloc(N,sizeof(double));

    /*Increase denominator of acceptance rate for b and hl*/
    ndelta++;

    /*Draw proposal values for b and hl*/
    rmvnorm(pmd,var,2,subject->basehalf,seed,1);

    /*Only proceed if we draw reasonable values*/
    if (pmd[0] > 0 && pmd[1] > 3) {

      /*Compute ratio of prior densities*/
      priorb_old = subject->basehalf[0] - priors->meanbh[0];
      priorb_old *= 0.5*priorb_old/(priors->varbh[0]*priors->varbh[0]);
      priorb_new = pmd[0]-priors->meanbh[0];
      priorb_new *= 0.5*priorb_new/(priors->varbh[0]*priors->varbh[0]);
      priorh_old = subject->basehalf[1] - priors->meanbh[1];
      priorh_old *= 0.5*priorh_old/(priors->varbh[1]*priors->varbh[1]);
      priorh_new = pmd[1]-priors->meanbh[1];
      priorh_new *= 0.5*priorh_new/(priors->varbh[1]*priors->varbh[1]);

      prior_ratio = priorb_old + priorh_old - priorb_new - priorh_new;

      /*Save current values of b and hl and set new current values equal to
        proposal values */
      for (k=0;k<2;k++) {
        currentmd[k] = subject->basehalf[k];
        subject->basehalf[k] = pmd[k];
      }

      /*Save current decay rate; calculate new current decay rate based on
        proposal value of hl */
      current_decay = subject->decay;
      subject->decay = log(2)/subject->basehalf[1];

      /*Save current mean_contrib for each pulse; calculate new current
        mean_contrib for each pulse based on proposed values of b and hl*/
      i = 0;
      subnode = subject->list->succ;
      while (subnode != NULL) {
        for (j=0;j<N;j++)
          currentmc[i][j] = subnode->mean_contrib[j];
        mean_contribution(subnode,ts,parms,N,subject->basehalf[1]);
        i++;
        subnode = subnode->succ;
      }

      /*Calculate proposed likelihood and then calculate likelihood ratio*/
      plikelihood = likelihood2(subject->list,ts,parms,N,subject->list,subject->basehalf[0]);
      like_ratio = plikelihood - current_like;

      /*Calculate log rho; set alpha equal to min(0,log rho) */
      alpha = (0 < (temp = (prior_ratio+like_ratio))) ? 0:temp;

      /*If log U < log rho, increase acceptance rate by 1  */
      if (log(kiss(seed)) < alpha) {
        adelta++;
      }

      /*Otherwise, we need to revert values back to their current state */
      else {

        /*Set b and hl back equal to current values*/
        subject->basehalf[0] = currentmd[0];
        subject->basehalf[1] = currentmd[1];

        /*Set mean_contrib back equal to current values for each pulse*/
        i = 0;
        subnode = subject->list->succ;
        while (subnode != NULL) {
          for (j=0;j<N;j++) {
            subnode->mean_contrib[j] = currentmc[i][j];
          }
          i++;
          subnode = subnode->succ;
        }

        /*Set decay rate back equal to current value*/
        subject->decay = current_decay;

      } /*End of if else statement*/

    }

    /*Go to next subject*/
    subject = subject->succ;
    parms->subindex++;

    /*Free memory */
    for (i=0;i<num_node;i++)
      free(currentmc[i]);
    free(currentmc);
    free(pmd);

  } /*End of loop through subjects*/

}