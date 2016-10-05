/*
 * ProblemManager.hpp
 *
 *  Created on: Jul 21, 2016
 *      Author: rrsettgast
 */

#ifndef COMPONENTS_CORE_SRC_MANAGERS_PROBLEMMANAGER_HPP_
#define COMPONENTS_CORE_SRC_MANAGERS_PROBLEMMANAGER_HPP_

#ifndef USE_PYTHON
#define USE_PYTHON 0
#endif

#if USE_PYTHON==1
// Note: the python header must be included first to avoid conflicting definitions of _posix_c_source
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <numpy/arrayobject.h>
#endif
#include "pugixml/src/pugixml.hpp"

#include "ObjectManagerBase.hpp"
#include "PhysicsSolvers/PhysicsSolverManager.hpp"
#include "DocumentationNode.hpp"

namespace geosx
{

class DomainPartition;

class ProblemManager : public ObjectManagerBase
{
public:
  explicit ProblemManager( const std::string& name,
                           ObjectManagerBase * const parent );

  explicit ProblemManager( const std::string& name,
                           ObjectManagerBase * const parent,
                           cxx_utilities::DocumentationNode * docNode );
  ~ProblemManager();

  /**
   * @name Static Factory Catalog Functions
   */
  ///@{
  static std::string CatalogName() { return "ProblemManager"; }
  string getName() const override final
  {
    return ProblemManager::CatalogName();
  }
  ///@}



  virtual void FillDocumentationNode( dataRepository::ManagedGroup * const group ) override;

  virtual void BuildDataStructure( dataRepository::ManagedGroup * const ) override;

  void ParseCommandLineInput( int const& argc, char* const argv[]);

  void InitializePythonInterpreter();

  void ClosePythonInterpreter();

  void ParseInputFile();

  void InitializeObjects();

  void RunSimulation();

  void ApplySchedulerEvent();

  DomainPartition & getDomainPartition();
  DomainPartition const & getDomainPartition() const;

  pugi::xml_document xmlDocument;
  pugi::xml_parse_result xmlResult;
  pugi::xml_node xmlProblemNode;

private:
  PhysicsSolverManager * m_physicsSolverManager;
};

} /* namespace geosx */

#endif /* COMPONENTS_CORE_SRC_MANAGERS_PROBLEMMANAGER_HPP_ */
