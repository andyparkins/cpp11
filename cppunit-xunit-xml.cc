// ----------------------------------------------------------------------------
// Project: template
/// @file   cppunit-xunit-xml.cc
/// @author Andy Parkins
//
// Version Control
//    $Author$
//      $Date$
//        $Id$
//
// Legal
//    Copyright 2013  FussyLogic Ltd
//
// ----------------------------------------------------------------------------

#ifdef UNITTESTALL
// Module include
//#include "cppunit-xunit-xml.h"

// -------------- Includes
// --- C
// --- C++
#include <string>
#include <iostream>
// --- OS
// --- Project libs
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/XmlOutputterHook.h>
#include <cppunit/tools/XmlElement.h>
#include <cppunit/tools/XmlDocument.h>
#include <cppunit/tools/StringTools.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestFailure.h>


// -------------- Namespace
using namespace std;


// -------------- Macros


// -------------- Module Globals


// -------------- World Globals (need "extern"s in header)


// -------------- Class member static initialisers


// -------------- Class member definitions


// -------------- Function definitions


//
// Class:	JUnitXmlOutputter
// Description:
//
class JUnitXmlOutputter : public CppUnit::XmlOutputter
{
  public:
	using CppUnit::XmlOutputter::XmlOutputter;

	void setName(const string &s) { m_name = s; }

	void setRootNode() override {
		CppUnit::XmlElement *rootNode = new CppUnit::XmlElement( "testsuite" );
		m_xml->setRootElement( rootNode );

		fillFailedTestsMap( m_failedTests );

		for (auto &hook : m_hooks)
			hook->beginDocument(m_xml);

		addTests( rootNode );
		addStatistics(rootNode);

		for (auto &hook : m_hooks)
			hook->endDocument(m_xml);
	}

	void addStatistics(CppUnit::XmlElement *rootNode) override {
		int countErrors = 0;
		int countFailures = 0;
		for (auto &failedTest : m_failedTests ) {
			if (failedTest.second->isError()) 
				++countErrors;
			else
				++countFailures;
		}

		char buf[100];
		sprintf(buf, "%d", countErrors);
		rootNode->addAttribute("errors", buf);
		sprintf(buf, "%d", countFailures);
		rootNode->addAttribute("failures", buf);
		sprintf(buf, "%d", m_result->tests().size());
		rootNode->addAttribute("tests", buf);

		rootNode->addAttribute("name", m_name);

		for (auto &hook : m_hooks)
			hook->statisticsAdded(m_xml, rootNode);
	}

	// Replaced these with addTests
	void addFailedTests(FailedTests &, CppUnit::XmlElement *) override {}
	void addSuccessfulTests(FailedTests &, CppUnit::XmlElement *) override {}

	void addTests(CppUnit::XmlElement *rootNode) {
		const CppUnit::TestResultCollector::Tests &tests = m_result->tests();
		for ( unsigned int n = 0; n < tests.size(); ++n ) {
			CppUnit::XmlElement *testElement = addTest(tests[n], n, rootNode);
			if (m_failedTests.find(tests[n]) != m_failedTests.end()) {
				CppUnit::TestFailure *failure = m_failedTests[tests[n]];
				addFailedTest(tests[n], failure, n, testElement);
			} else {
				addSuccessfulTest(tests[n], n, testElement);
			}
		}
	}

	CppUnit::XmlElement *addTest(CppUnit::Test *test, int testNumber,
			CppUnit::XmlElement *rootNode) {
		CppUnit::XmlElement *testElement = new CppUnit::XmlElement( "testcase" );
		rootNode->addElement( testElement );
		testElement->addAttribute( "id", testNumber );
		std::string::size_type n = test->getName().find("::");
		if (n != std::string::npos) {
			testElement->addAttribute( "class", std::string(test->getName(), 0, n) );
			testElement->addAttribute( "name",  std::string(test->getName(), n+2) );
		} else {
			testElement->addAttribute( "class", "" );
			testElement->addAttribute( "name",  test->getName() );
		}

		return testElement;
	}

	void addFailedTest(CppUnit::Test *test, CppUnit::TestFailure *failure,
			int testNumber, CppUnit::XmlElement *testElement) {
		CppUnit::Exception *thrownException = failure->thrownException();

		CppUnit::XmlElement *failureElement = new CppUnit::XmlElement( "failure",
				thrownException->what() );
		testElement->addElement( failureElement );

		failureElement->addAttribute( "type", failure->isError() ? "error" : "assertion" );
		failureElement->addAttribute( "message", thrownException->message().shortDescription() );

		if (failure->sourceLine().isValid())
			addFailureLocation( failure, failureElement );

		for (auto &hook : m_hooks)
			hook->failTestAdded(m_xml, testElement, test, failure);
	}

	void addFailureLocation( CppUnit::TestFailure *failure,
				CppUnit::XmlElement *testElement ) override {
		CppUnit::XmlElement *locationNode = new CppUnit::XmlElement( "location" );
		testElement->addElement( locationNode );
		CppUnit::SourceLine sourceLine = failure->sourceLine();
		locationNode->addElement( new CppUnit::XmlElement( "file", sourceLine.fileName() ) );
		locationNode->addElement( new CppUnit::XmlElement( "line", sourceLine.lineNumber() ) );
	}

	void addSuccessfulTest (CppUnit::Test *test, int testNumber,
			CppUnit::XmlElement *testElement) {
		for (auto &hook : m_hooks)
			hook->successfulTestAdded(m_xml, testElement, test);
	}

  protected:
	string m_name;
	FailedTests m_failedTests;
};

// -------------- main()

int main(int argc, char *argv[])
{
	// --- Boilerplate cppunit code
	// Set up runner to run all test in registry
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( CppUnit::TestFactoryRegistry::getRegistry().makeTest() );
	// Redirect output to clog,
	JUnitXmlOutputter *outputter = new JUnitXmlOutputter(
			&runner.result(), std::clog);
	outputter->setName(argv[0]);
	runner.setOutputter(outputter);
	// Run all and give success indiciation
	return runner.run() ? 0 : 1;
}
#endif
