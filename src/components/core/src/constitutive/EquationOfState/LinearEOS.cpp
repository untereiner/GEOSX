/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2018, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * LLNL-CODE-746361
 *
 * All rights reserved. See COPYRIGHT for details.
 *
 * This file is part of the GEOSX Simulation Framework.
 *
 * GEOSX is a free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License (as published by the
 * Free Software Foundation) version 2.1 dated February 1999.
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#include "LinearEOS.hpp"

namespace geosx
{
using namespace dataRepository;
using namespace cxx_utilities;
namespace constitutive
{


static inline void UpdateStatePoint( R2SymTensor const & D,
                                     R2Tensor const & Rot,
                                     localIndex const i,
                                     void * dataPtrs,
                                     integer const systemAssembleFlag )
{


}


LinearEOS::LinearEOS( std::string const & name, ManagedGroup * const parent ):
  ConstitutiveBase(name, parent ),
  m_densityRelation(ExponentApproximationType::Linear),
  m_viscosityRelation(ExponentApproximationType::Linear),
  m_porosityRelation(ExponentApproximationType::Linear)
{
  m_parameterData.RegisterViewWrapper( viewKeys.fluidBulkModulus.Key(), &m_fluidBulkModulus, 0 );
  m_parameterData.RegisterViewWrapper( viewKeys.solidBulkModulus.Key(), &m_solidBulkModulus, 0 );
  m_parameterData.RegisterViewWrapper( viewKeys.fluidViscosibility.Key(), &m_fluidViscosibility, 0 );
  m_parameterData.RegisterViewWrapper( viewKeys.referencePressure.Key(), &m_referencePressure, 0 );
  m_parameterData.RegisterViewWrapper( viewKeys.referenceDensity.Key(), &m_referenceDensity, 0 );
  m_parameterData.RegisterViewWrapper( viewKeys.referenceViscosity.Key(), &m_referenceViscosity, 0 );
}

LinearEOS::~LinearEOS()
{
  // TODO Auto-generated destructor stub
}

void LinearEOS::FillDocumentationNode()
{

  DocumentationNode * const docNode = this->getDocumentationNode();

  docNode->setName(this->CatalogName());
  docNode->setSchemaType("Node");
  docNode->setShortDescription("Slightly compressible single phase fluid equation of state");

  ManagedGroup * parameterData = this->GetGroup( groupKeys().ParameterData.Key() );
  DocumentationNode * const parameterDocNode = parameterData->getDocumentationNode();
  parameterDocNode->setSchemaType("Node");
  parameterDocNode->setShortDescription("Parameters for slightly compressible single phase fluid equation of state");

  parameterDocNode->AllocateChildNode( viewKeys.fluidBulkModulus.Key(),
                                       viewKeys.fluidBulkModulus.Key(),
                                       -1,
                                       "real64",
                                       "real64",
                                       "Fluid Bulk Modulus",
                                       "Fluid Bulk Modulus",
                                       "-1",
                                       "",
                                       1,
                                       1,
                                       0 );

  parameterDocNode->AllocateChildNode( viewKeys.solidBulkModulus.Key(),
                                       viewKeys.solidBulkModulus.Key(),
                                       -1,
                                       "real64",
                                       "real64",
                                       "Solid Bulk Modulus",
                                       "Solid Bulk Modulus",
                                       "-1",
                                       "",
                                       1,
                                       1,
                                       0 );

  parameterDocNode->AllocateChildNode( viewKeys.fluidViscosibility.Key(),
                                       viewKeys.fluidViscosibility.Key(),
                                       -1,
                                       "real64",
                                       "real64",
                                       "Fluid viscosity exponential coefficient",
                                       "Fluid viscosity exponential coefficient",
                                       "0",
                                       "",
                                       1,
                                       1,
                                       0 );

  parameterDocNode->AllocateChildNode( viewKeys.referencePressure.Key(),
                                       viewKeys.referencePressure.Key(),
                                       -1,
                                       "real64",
                                       "real64",
                                       "Reference pressure",
                                       "Reference pressure",
                                       "0",
                                       "",
                                       1,
                                       1,
                                       0 );

  parameterDocNode->AllocateChildNode( viewKeys.referenceDensity.Key(),
                                       viewKeys.referenceDensity.Key(),
                                       -1,
                                       "real64",
                                       "real64",
                                       "Reference fluid density",
                                       "Reference fluid density",
                                       "1000",
                                       "",
                                       1,
                                       1,
                                       0 );

  parameterDocNode->AllocateChildNode( viewKeys.referenceViscosity.Key(),
                                       viewKeys.referenceViscosity.Key(),
                                       -1,
                                       "real64",
                                       "real64",
                                       "Reference fluid viscosity",
                                       "Reference fluid viscosity",
                                       "0.001",
                                       "",
                                       1,
                                       1,
                                       0 );


  ManagedGroup * stateData = this->GetGroup( groupKeys().StateData.Key() );
  DocumentationNode * const stateDocNode = stateData->getDocumentationNode();
  stateDocNode->setSchemaType("Node");
  stateDocNode->setShortDescription("State for slightly compressible single phase fluid equation of state");
}

void LinearEOS::ReadXML_PostProcess()
{
  if( m_fluidBulkModulus <= 0.0 )
  {
    string const message = "An invalid value of fluid bulk modulus ("+std::to_string(m_fluidBulkModulus)+") is specified";
    GEOS_ERROR(message);
  }

  if( m_solidBulkModulus <= 0.0 )
  {
    string const message = "An invalid value of solid bulk modulus ("+std::to_string(m_solidBulkModulus)+") is specified";
    GEOS_ERROR(message);
  }

  if( m_fluidViscosibility < 0.0 )
  {
    string const message = "An invalid value of fluid viscosibility ("+std::to_string(m_fluidViscosibility)+") is specified";
    GEOS_ERROR(message);
  }

  if( m_referenceDensity <= 0.0 )
  {
     string const message = "An invalid value of reference density ("+std::to_string(m_referenceDensity)+") is specified";
     GEOS_ERROR(message);
  }

  if( m_referenceViscosity <= 0.0 )
  {
    string const message = "An invalid value of reference viscosity ("+std::to_string(m_referenceViscosity)+") is specified";
    GEOS_ERROR(message);
  }
}

ConstitutiveBase::UpdateFunctionPointer
LinearEOS::GetStateUpdateFunctionPointer()
{
  return UpdateStatePoint;
}

void LinearEOS::FluidPressureUpdate(real64 const &dens,
                                    localIndex const i,
                                    real64 &pres,
                                    real64 &dPres_dDens)
{
  m_densityRelation.Inverse(dens, pres, dPres_dDens);
}

void LinearEOS::FluidDensityUpdate(real64 const &pres,
                                   localIndex const i,
                                   real64 &dens,
                                   real64 &dDens_dPres)
{
  m_densityRelation.Compute(pres, dens, dDens_dPres);
}

void LinearEOS::FluidViscosityUpdate(real64 const &pres, localIndex const i, real64 &visc, real64 &dVisc_dPres)
{
  m_viscosityRelation.Compute(pres, visc, dVisc_dPres);
}

void LinearEOS::SimplePorosityUpdate(real64 const &pres, real64 const &poro_ref, localIndex const i, real64 &poro, real64 &dPoro_dPres)
{
  m_porosityRelation.Compute(pres, poro, dPoro_dPres);
  poro *= poro_ref;
  dPoro_dPres *= poro_ref;
}

void LinearEOS::FinalInitialization(ManagedGroup *const parent)
{
  m_densityRelation.SetCoefficients(m_referencePressure, m_referenceDensity, 1.0 / m_fluidBulkModulus);
  m_viscosityRelation.SetCoefficients(m_referencePressure, m_referenceViscosity, m_fluidViscosibility);
  m_porosityRelation.SetCoefficients(m_referencePressure, 1.0, 1.0 / m_solidBulkModulus);
}

void LinearEOS::GetStiffness( realT c[6][6]) const
{
  c[0][0] = m_fluidBulkModulus;
  c[0][1] = 0.0;
  c[0][2] = 0.0;
  c[0][3] = 0.0;
  c[0][4] = 0.0;
  c[0][5] = 0.0;

  c[1][0] = 0.0;
  c[1][1] = m_fluidBulkModulus;
  c[1][2] = 0.0;
  c[1][3] = 0.0;
  c[1][4] = 0.0;
  c[1][5] = 0.0;

  c[2][0] = 0.0;
  c[2][1] = 0.0;
  c[2][2] = m_fluidBulkModulus;
  c[2][3] = 0.0;
  c[2][4] = 0.0;
  c[2][5] = 0.0;

  c[3][0] = 0.0;
  c[3][1] = 0.0;
  c[3][2] = 0.0;
  c[3][3] = 0.0;
  c[3][4] = 0.0;
  c[3][5] = 0.0;

  c[4][0] = 0.0;
  c[4][1] = 0.0;
  c[4][2] = 0.0;
  c[4][3] = 0.0;
  c[4][4] = 0.0;
  c[4][5] = 0.0;

  c[5][0] = 0.0;
  c[5][1] = 0.0;
  c[5][2] = 0.0;
  c[5][3] = 0.0;
  c[5][4] = 0.0;
  c[5][5] = 0.0;
}


REGISTER_CATALOG_ENTRY( ConstitutiveBase, LinearEOS, std::string const &, ManagedGroup * const )
}
} /* namespace geosx */