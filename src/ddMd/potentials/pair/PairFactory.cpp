#ifndef PAIR_FACTORY_CPP
#define PAIR_FACTORY_CPP

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010, David Morse (morse@cems.umn.edu)
* Distributed under the terms of the GNU General Public License.
*/

#include <ddMd/potentials/pair/PairFactory.h>
#include <ddMd/system/System.h>

// PairPotential interface and implementation classes
#include <ddMd/potentials/pair/PairPotential.h>
#include <ddMd/potentials/pair/PairPotentialImpl.h>

// Pair interaction classes
#include <ddMd/potentials/pair/DpdSoftPair.h>
#include <ddMd/potentials/pair/LJPair.h>

namespace DdMd
{

   /**
   * Default constructor.
   */
   PairFactory::PairFactory(System& system)
    : Factory<PairPotential>(),
      systemPtr_(&system)
   {}

   /*
   * Return a pointer to a new PairPotential, if possible.
   */
   PairPotential* 
   PairFactory::factory(const std::string& name) const
   {
      PairPotential* ptr = 0;

      // Try subfactories first.
      ptr = trySubfactories(name);
      if (ptr) return ptr;

      if (name == "LJPair") {
         ptr = new PairPotentialImpl<LJPair>(*systemPtr_);
      } else
      if (name == "DpdSoftPair") {
         ptr = new PairPotentialImpl<DpdSoftPair>(*systemPtr_);
      } 
      return ptr;
   }

}
#endif