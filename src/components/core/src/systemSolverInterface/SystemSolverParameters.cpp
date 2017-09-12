/*
 * SystemSolverParameters.cpp
 *
 *  Created on: Sep 12, 2017
 *      Author: settgast
 */

#include "SystemSolverParameters.hpp"

namespace geosx
{

SystemSolverParameters::SystemSolverParameters( std::string const & name,
                                                ManagedGroup * const parent ):
  ManagedGroup(name,parent)
{
  // TODO Auto-generated constructor stub

}

SystemSolverParameters::~SystemSolverParameters()
{
  // TODO Auto-generated destructor stub
}



void SystemSolverParameters::FillDocumentationNode( dataRepository::ManagedGroup * const  )
{
  cxx_utilities::DocumentationNode * const docNode = this->getDocumentationNode();

  docNode->setName("SystemSolverParamters");
  docNode->setSchemaType("Node");
  docNode->setShortDescription("Parameters for linear/non-linear system solver");


  docNode->AllocateChildNode( keys.verbosity.Key(),
                              keys.verbosity.Key(),
                              -1,
                              "int32",
                              "int32",
                              "verbosity level",
                              "verbosity level",
                              "0.5",
                              "",
                              0,
                              1,
                              0 );

//  real64 m_krylovTol;          // Solver convergence criteria
//  int32  m_numKrylovIter;
//  int32  m_kspace;             // Number of krylov vectors before GMRES restart
//  real64 m_ilut_fill;          // Fill factor for ILUT preconditioner
//  real64 m_ilut_drop;          // Drop tolerance for ILUT preconditioner
//  bool   m_useMLPrecond;       // Use ML preconditioner
//  bool   m_useInnerSolver;     // Use row scaling
//  int32  m_scalingOption;      // Use row scaling
//  bool   m_useBicgstab;        // Use bicgstab instead of gmres
//  bool   m_useDirectSolver;    // Use Direct solver
//  real64 m_KrylovResidualInit;
//  real64 m_KrylovResidualFinal;
//
//  bool   m_useNewtonSolve;    // Use Newton-Raphson iterations
//  real64 m_newtonTol;
//  int32  m_maxIterNewton;     // Maximum number of Newton-Raphson iterations
}

} /* namespace geosx */