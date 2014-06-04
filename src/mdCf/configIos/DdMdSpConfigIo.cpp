#ifndef DDMD_SP_DDMD_CONFIG_IO_CPP
#define DDMD_SP_DDMD_CONFIG_IO_CPP

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2012, David Morse (morse012@umn.edu)
* Distributed under the terms of the GNU General Public License.
*/

#include "DdMdSpConfigIo.h"

#include <ddMd/sp/chemistry/SpAtom.h>
#include <ddMd/sp/chemistry/SpGroup.h>
#include <ddMd/sp/chemistry/SpSpecies.h>
//#include <ddMd/sp/chemistry/MaskPolicy.h>
#include <ddMd/sp/storage/SpConfiguration.h>

#include <util/space/Vector.h>
#include <util/format/Int.h>
#include <util/format/Dbl.h>

namespace MdCf
{

   using namespace Util;

   /*
   * Constructor.
   */
   DdMdSpConfigIo::DdMdSpConfigIo(SpConfiguration& configuration, bool hasMolecules)
    : SpConfigIo(configuration),
      hasMolecules_(hasMolecules)
   {  setClassName("DdMdSpConfigIo"); }

   /*
   * Read a configuration file.
   */
   void DdMdSpConfigIo::readConfig(std::ifstream& file)
   {
      // Precondition
      if (!file.is_open()) {  
            UTIL_THROW("Error: File is not open"); 
      }

      // Read and broadcast boundary
      file >> Label("BOUNDARY");
      file >> configuration().boundary();

      // Read and distribute atoms

      // Read atoms
      SpAtom* atomPtr;
      int atomCapacity = configuration().atoms().capacity(); // Maximum allowed id + 1
      int nAtom;          
      file >> Label("ATOMS");
      file >> Label("nAtom") >> nAtom;
      for (int i = 0; i < nAtom; ++i) {

         // Get pointer to new atom 
         atomPtr = configuration().atoms().newPtr();
 
         file >> atomPtr->id >> atomPtr->typeId;
         if (atomPtr->id < 0 || atomPtr->id >= atomCapacity) {
            UTIL_THROW("Invalid atom id");
         }
         if (hasMolecules_) {
            file >> atomPtr->speciesId 
                 >> atomPtr->moleculeId 
                 >> atomPtr->atomId;
            if (atomPtr->speciesId < 0) {
               UTIL_THROW("Invalid species id");
            }
            if (atomPtr->moleculeId < 0) {
               UTIL_THROW("Invalid molecule id");
            }
            if (atomPtr->atomId < 0) {
               UTIL_THROW("Invalid atom id");
            }
         }
         file >> atomPtr->position;
         file >> atomPtr->velocity;

         configuration().atoms().add();
      }

      // Read Covalent SpGroups
      #ifdef INTER_BOND
      if (configuration().bonds().capacity()) {
         readGroups(file, "BONDS", "nBond", configuration().bonds());
         //if (maskPolicy == MaskBonded) {
         //   setAtomMasks();
         //}
      }
      #endif

      // Optionally add atoms to species
      if (configuration().nSpecies() > 0) {
         if (!hasMolecules_) {
            UTIL_THROW("No atom context info in chosen SpConfigIo");
         }
         int speciesId;
         SpAtomStorage::Iterator iter;
         configuration().atoms().begin(iter);
         for ( ; iter.notEnd(); ++iter) {
            speciesId = iter->speciesId;
            configuration().species(speciesId).addAtom(*iter);
         }
         for (int i = 0; configuration().nSpecies(); ++i) {
            configuration().species(i).isValid();
         }
      }
   }

   /* 
   * Write the configuration file.
   */
   void DdMdSpConfigIo::writeConfig(std::ofstream& file)
   {
      // Precondition
      if (!file.is_open()) {  
         UTIL_THROW("Error: File is not open"); 
      }

      // Write Boundary dimensions
      file << "BOUNDARY" << std::endl << std::endl;
      file << configuration().boundary() << std::endl;
      file << std::endl;

      // Atoms
      int nAtom = configuration().atoms().size();
      file << "ATOMS" << std::endl;
      file << "nAtom" << Int(nAtom, 10) << std::endl;

      // Write atoms
      Vector r;
      SpAtomStorage::Iterator iter;
      configuration().atoms().begin(iter);
      for (; iter.notEnd(); ++iter) {
         file << Int(iter->id, 10) 
              << Int(iter->typeId, 6);
         r = iter->position;
         if (hasMolecules_) {
            file << Int(iter->speciesId, 6) 
                 << Int(iter->moleculeId, 10)
                 << Int(iter->atomId, 6);
         }
         file << "\n" << r 
              << "\n" << iter->velocity << "\n";
      }

      // Write the groups
      #ifdef INTER_BOND
      if (configuration().bonds().capacity()) {
         writeGroups(file, "BONDS", "nBond", configuration().bonds());
      }
      #endif

   }
 
}
#endif
