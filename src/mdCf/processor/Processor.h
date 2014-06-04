#ifndef DDMD_SP_PROCESSOR_H
#define DDMD_SP_PROCESSOR_H

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2012, David Morse (morse012@umn.edu)
* Distributed under the terms of the GNU General Public License.
*/

#include <ddMd/sp/storage/SpConfiguration.h>              // base class
#include <ddMd/sp/configIos/SpConfigIoFactory.h>   // member 
#include <ddMd/sp/analyzers/SpAnalyzerManager.h>   // member 
#include <util/misc/FileMaster.h>             // member 


namespace MdCf 
{

   class SpConfigIo;

   using namespace Util;

   /**
   * A post-processor for analyzing outputs of MD simulations.
   *
   * \ingroup MdCf_Module
   */
   class Processor : public SpConfiguration
   {

   public:

      /**
      * Constructor
      */
      Processor();

      /**
      * Destructor
      */
      ~Processor();

      using ParamComposite::readParam;

      /**
      * Open, read, and close parameter file.
      */
      void readParam(const char* filename);

      /**
      * Read parameters.
      */
      void readParameters(std::istream& in);

      /**
      * Set SpConfigIo style.
      */
      void setSpConfigIo(std::string configIoName);

      /**
      * Return the SpConfigIo (create default if necessary).
      */
      SpConfigIo& configIo();
   
      /**
      * Read a single configuration file.
      */
      void readConfig(std::ifstream& in);

      /**
      * Open, read and close a configuration file.
      */
      void readConfig(const char* filename);
   
      /**
      * Open, read and close a configuration file.
      */
      void readConfig(const std::string& filename);
   
      /**
      * Write a single configuration file.
      */
      void writeConfig(std::ofstream& out);

      /**
      * Open, write and close a configuration file.
      */
      void writeConfig(const std::string& filename);

      /**
      * Read and analyze a sequence of numbered configuration files.
      *
      * This function reads and analyzes a sequence of configuration files 
      * that were generated by running a previous simulation. The function 
      * reads files with names of the form inputPrefix() + n for integer 
      * suffixes min <= n <= max. 
      *
      * \param min  integer suffix of first configuration file name
      * \param max  integer suffix of last configuration file name
      * \param fileBaseName root name for dump files (without integer suffix)
      */  
      void analyzeDumps(int min, int max, std::string fileBaseName);

      /**
      * Analyze a trajectory file.
      */
      void analyzeTrajectory(std::string& filename);

      /**
      * Return true if the FileMaster is active.
      */
      bool hasFileMaster() const;

      /**
      * Return FileMaster if active, or throw Exception.
      */
      FileMaster& fileMaster();

   private:

      /// Pointer to current SpConfigIo object.
      SpConfigIo* configIoPtr_;

      /// Factory for generating SpConfigIo at run time.
      SpConfigIoFactory configIoFactory_;

      /// Manager for analyzers
      SpAnalyzerManager analyzerManager_;

      /// FileMaster (optionally activated)
      FileMaster fileMaster_;

      /// String identifier for SpConfigIo class name
      std::string configIoName_;

      /// Name of configuration or trajectory input file
      std::string configFileName_;

   };

}
#endif
