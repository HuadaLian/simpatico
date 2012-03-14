#ifndef DPD_SOFT_PAIR_CPP
#define DPD_SOFT_PAIR_CPP

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010, David Morse (morse@cems.umn.edu)
* Distributed under the terms of the GNU General Public License.
*/

#include <iostream>
#include "DpdSoftPair.h"

namespace DdMd
{

   using namespace Util;

   /* 
   * Constructor.
   */
   DpdSoftPair::DpdSoftPair() 
    : maxPairCutoff_(0.0),
      nAtomType_(0),
      isInitialized_(false)
   {}
   
   /* 
   * Copy constructor.
   */
   DpdSoftPair::DpdSoftPair(const DpdSoftPair& other)
    : maxPairCutoff_(other.maxPairCutoff_),
      nAtomType_(other.nAtomType_),
      isInitialized_(other.isInitialized_)
   {
      int i,j;
      for (i = 0; i < nAtomType_; ++i) {
         for (j = 0; j < nAtomType_; ++j) {
            epsilon_[i][j] = other.epsilon_[i][j];
            sigma_[i][j]   = other.sigma_[i][j];
            sigmaSq_[i][j] = other.sigmaSq_[i][j];
            ce_[i][j]      = other.ce_[i][j];
            cf_[i][j]      = other.cf_[i][j];
         } 
      }
   }
   
   /* 
   * Assignment operator.
   */
   DpdSoftPair& DpdSoftPair::operator = (const DpdSoftPair& other)
   {
      maxPairCutoff_ = other.maxPairCutoff_;
      nAtomType_     = other.nAtomType_;
      isInitialized_ = other.isInitialized_;
      int i,j;
      for (i = 0; i < nAtomType_; ++i) {
         for (j = 0; j < nAtomType_; ++j) {
            epsilon_[i][j] = other.epsilon_[i][j];
            sigma_[i][j]   = other.sigma_[i][j];
            sigmaSq_[i][j] = other.sigmaSq_[i][j];
            ce_[i][j]      = other.ce_[i][j];
            cf_[i][j]      = other.cf_[i][j];
         } 
      }
      return *this;
   }
   
   /* 
   * Read potential parameters from file.
   */
   void DpdSoftPair::readParam(std::istream &in) 
   {
      // Preconditions
      if (nAtomType_ <= 0) {
         UTIL_THROW( "nAtomType must be set before readParam");
      }
   
      // Read parameters
      //readBegin(in, "DpdSoftPair");
      readCArray2D<double> (
                  in, "epsilon", epsilon_[0], nAtomType_, nAtomType_);
      readCArray2D<double>(
                  in, "sigma", sigma_[0], nAtomType_, nAtomType_);
      //readEnd(in);
   
      // Calculate sigmaSq_, ce_, cf_, and maxPairCutoff_
      int i, j;
      maxPairCutoff_ = 0.0;
      for (i = 0; i < nAtomType_; ++i) {
         for (j = 0; j < nAtomType_; ++j) {
            sigmaSq_[i][j]  = sigma_[i][j]*sigma_[i][j];
            if (sigma_[i][j] > maxPairCutoff_) {
               maxPairCutoff_ = sigma_[i][j];
            }
            cf_[i][j] = epsilon_[i][j]/sigmaSq_[i][j];
            ce_[i][j] = 0.5*cf_[i][j];
         } 
      }
  
      isInitialized_ = true; 
   }

   /* 
   * Set nAtomType
   */
   void DpdSoftPair::setNAtomType(int nAtomType) 
   {  
      if (nAtomType <= 0) {
         UTIL_THROW("nAtomType <= 0");
      }
      if (nAtomType > MaxAtomType) {
         UTIL_THROW("nAtomType > DpdSoftPair::MaxAtomType");
      }
      nAtomType_ = nAtomType;
   }
    
   /*
   * Reset epsilon_[i][j] after initialization
   */
   void DpdSoftPair::setEpsilon(int i, int j, double epsilon)
   {

      // Preconditions
      if (!isInitialized_) {
         UTIL_THROW("Cannot modify epsilon before DpdSoftPair is initialized");
      }
      if (i < 0 || i >= nAtomType_) {
         UTIL_THROW("Invalid atom type index i");
      }
      if (j < 0 || j >= nAtomType_) {
         UTIL_THROW("Invalid atom type index j");
      }

      epsilon_[i][j] = epsilon;
      if (j != i) {
         epsilon_[j][i] = epsilon;
      }
   } 

   /*
   * Reset sigma_[i][j] after initialization
   */
   void DpdSoftPair::setSigma(int i, int j, double sigma)
   {

      // Preconditions
      if (!isInitialized_) {
         UTIL_THROW("Cannot modify sigma before DpdSoftPair is initialized");
      }
      if (i < 0 || i >= nAtomType_) {
         UTIL_THROW("Invalid atom type index i");
      }
      if (j < 0 || j >= nAtomType_) {
         UTIL_THROW("Invalid atom type index j");
      }

      sigma_[i][j] = sigma;
      if (j != i) {
         sigma_[j][i] = sigma;
      }

   } 

   /* 
   * Get pair interaction strength.
   */
   double DpdSoftPair::epsilon(int i, int j) const
   { 
      assert(i >= 0 && i < nAtomType_); 
      assert(j >= 0 && j < nAtomType_); 
      return epsilon_[i][j]; 
   }

   /* 
   * Get pair interaction strength.
   */
   double DpdSoftPair::sigma(int i, int j) const
   {
      assert(i >= 0 && i < nAtomType_); 
      assert(j >= 0 && j < nAtomType_); 
      return sigma_[i][j]; 
   }

   /* 
   * Get maximum of pair cutoff distance, for all atom type pairs.
   */
   double DpdSoftPair::maxPairCutoff() const
   { return maxPairCutoff_; }

   /**
   * Return name string "DpdSoftPair" for this evaluator class.
   */
   std::string DpdSoftPair::className() const
   {  return std::string("DpdSoftPair"); }

} 
#endif