#ifndef TOOLS_DDMD_CONFIGIO_TEST_H
#define TOOLS_DDMD_CONFIGIO_TEST_H

#include <tools/processor/Processor.h>
#include <tools/chemistry/Atom.h>
#include <tools/chemistry/Group.h>

#include <test/ParamFileTest.h>
#include <test/UnitTestRunner.h>

using namespace Util;
using namespace Tools;

class DdMdConfigReaderTest : public ParamFileTest
{

private:

   Processor processor_;

public:

   DdMdConfigReaderTest() 
    : processor_()
   {}

   virtual void setUp()
   { 
      setVerbose(2);
   }

   void testReadParam();
   void testReadConfig1();
   void testReadConfig2();

};

inline void DdMdConfigReaderTest::testReadParam()
{  
   printMethod(TEST_FUNC); 
   std::ifstream file;
   openInputFile("in/Processor", file);
   processor_.readParam(file); 
   file.close(); 
   if (verbose() > 0) {
      std::cout << "\n";
      processor_.writeParam(std::cout);
   }
}

inline void DdMdConfigReaderTest::testReadConfig1()
{
   printMethod(TEST_FUNC);

   std::ifstream file;
   openInputFile("in/Processor", file);
   processor_.readParam(file); 
   file.close(); 

   openInputFile("in/config", file);
   processor_.readConfig(file);
   file.close(); 

   TEST_ASSERT(processor_.atoms().size() == 40);
   TEST_ASSERT(processor_.bonds().size() == 35);
   // std::cout << "nAtom = " << processor_.atoms().size() << "\n";
   // std::cout << "nBond = " << processor_.bonds().size() << "\n";
   // std::cout << processor_.boundary() << "\n";

}

inline void DdMdConfigReaderTest::testReadConfig2()
{
   printMethod(TEST_FUNC);
   std::ifstream file;

   openInputFile("in/Processor.2", file);
   processor_.readParam(file); 
   file.close();

   processor_.setConfigReader("DdMdConfigReader_Molecule");
   openInputFile("in/config.2", file);
   processor_.readConfig(file);
   file.close();

   TEST_ASSERT(processor_.atoms().size() == 256);
   TEST_ASSERT(processor_.bonds().size() == 248);
   // std::cout << "nAtom = " << processor_.atoms().size() << "\n";
   // std::cout << "nBond = " << processor_.bonds().size() << "\n";
   // std::cout << processor_.boundary() << "\n";

}


TEST_BEGIN(DdMdConfigReaderTest)
TEST_ADD(DdMdConfigReaderTest, testReadParam)
TEST_ADD(DdMdConfigReaderTest, testReadConfig1)
TEST_ADD(DdMdConfigReaderTest, testReadConfig2)
TEST_END(DdMdConfigReaderTest)

#endif
