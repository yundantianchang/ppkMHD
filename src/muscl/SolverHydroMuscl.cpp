#include <string>
#include <cstdio>
#include <cstdbool>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "muscl/SolverHydroMuscl.h"
#include "shared/HydroParams.h"

#include "shared/mpiBorderUtils.h"

namespace ppkMHD { namespace muscl {

// =======================================================
// =======================================================
// //////////////////////////////////////////////////
// Fill ghost cells according to border condition :
// absorbant, reflexive or periodic
// //////////////////////////////////////////////////
template<>
void SolverHydroMuscl<2>::make_boundaries(DataArray Udata)
{
  const int ghostWidth=params.ghostWidth;
  int nbIter = ghostWidth*std::max(isize,jsize);

#ifdef USE_MPI

  const int mx = params.mx;
  const int my = params.my;

  // ======
  // XDIR
  // ======
  // 1. copy boundary to MPI buffer
  

  // 2. send/recv buffer
  // 3. test if BC is BC_PERIODIC / BC_COPY then ... else ..
  if (params.myMpiPos[IX] == 0) {
    MakeBoundariesFunctor2D<FACE_XMIN> functor(params, Udata);
    Kokkos::parallel_for(nbIter, functor);
  } else {
  }
  
  if (params.myMpiPos[IX] == mx-1) {
    MakeBoundariesFunctor2D<FACE_XMAX> functor(params, Udata);
    Kokkos::parallel_for(nbIter, functor);
  } else {
  }
  params.communicator->synchronize();
  
  if (params.myMpiPos[IY] == 0) {
    MakeBoundariesFunctor2D<FACE_YMIN> functor(params, Udata);
    Kokkos::parallel_for(nbIter, functor);
  } else {
  }
  if (params.myMpiPos[IY] == my-1) {
    MakeBoundariesFunctor2D<FACE_YMAX> functor(params, Udata);
    Kokkos::parallel_for(nbIter, functor);
  } else {
  }
  params.communicator->synchronize();

#else

  bool mhd_enabled = false;
  make_boundaries_serial(Udata, mhd_enabled);
  
#endif // USE_MPI
  
} // SolverHydroMuscl<2>::make_boundaries

// =======================================================
// =======================================================
// //////////////////////////////////////////////////
// Fill ghost cells according to border condition :
// absorbant, reflexive or periodic
// //////////////////////////////////////////////////
template<>
void SolverHydroMuscl<3>::make_boundaries(DataArray Udata)
{

  bool mhd_enabled = false;
  make_boundaries_serial(Udata, mhd_enabled);

} // SolverHydroMuscl<3>::make_boundaries

// =======================================================
// =======================================================
template<int dim>
void SolverHydroMuscl<dim>::make_boundaries(DataArray Udata)
{

  // this routine is specialized for 2d / 3d
  
} // SolverHydroMuscl<dim>::make_boundaries


// =======================================================
// =======================================================
/**
 * Four quadrant 2D riemann problem.
 *
 * See article: Lax and Liu, "Solution of two-dimensional riemann
 * problems of gas dynamics by positive schemes",SIAM journal on
 * scientific computing, 1998, vol. 19, no2, pp. 319-340
 */
template<>
void SolverHydroMuscl<2>::init_four_quadrant(DataArray Udata)
{

  int configNumber = configMap.getInteger("riemann2d","config_number",0);
  real_t xt = configMap.getFloat("riemann2d","x",0.8);
  real_t yt = configMap.getFloat("riemann2d","y",0.8);

  HydroState2d U0, U1, U2, U3;
  getRiemannConfig2d(configNumber, U0, U1, U2, U3);
  
  primToCons_2D(U0, params.settings.gamma0);
  primToCons_2D(U1, params.settings.gamma0);
  primToCons_2D(U2, params.settings.gamma0);
  primToCons_2D(U3, params.settings.gamma0);

  InitFourQuadrantFunctor2D functor(params, Udata, configNumber,
				    U0, U1, U2, U3,
				    xt, yt);
  Kokkos::parallel_for(nbCells, functor);
  
} // SolverHydroMuscl<2>::init_four_quadrant

// =======================================================
// =======================================================
template<int dim>
void SolverHydroMuscl<dim>::init_four_quadrant(DataArray Udata)
{

  // specialized only for 2d
  std::cerr << "You shouldn't be here: four quadrant problem is not implemented in 3D !\n";
  
} // SolverHydroMuscl::init_four_quadrant

// =======================================================
// =======================================================
/**
 * Isentropic vortex advection test.
 * https://www.cfd-online.com/Wiki/2-D_vortex_in_isentropic_flow
 * https://hal.archives-ouvertes.fr/hal-01485587/document
 */
template<>
void SolverHydroMuscl<2>::init_isentropic_vortex(DataArray Udata)
{
  
  IsentropicVortexParams iparams(configMap);
  
  InitIsentropicVortexFunctor2D functor(params, iparams, Udata);
  Kokkos::parallel_for(nbCells, functor);
  
} // SolverHydroMuscl<2>::init_isentropic_vortex

// =======================================================
// =======================================================
template<int dim>
void SolverHydroMuscl<dim>::init_isentropic_vortex(DataArray Udata)
{

  // specialized only for 2d
  std::cerr << "You shouldn't be here: isentropic vortex is not implemented in 3D !\n";
  
} // SolverHydroMuscl::init_isentropic_vortex

// =======================================================
// =======================================================
template<>
void SolverHydroMuscl<2>::init(DataArray Udata)
{

  /*
   * initialize hydro array at t=0
   */
  if ( !m_problem_name.compare("implode") ) {

    init_implode(Udata);

  } else if ( !m_problem_name.compare("blast") ) {

    init_blast(Udata);

  } else if ( !m_problem_name.compare("four_quadrant") ) {

    init_four_quadrant(Udata);

  } else if ( !m_problem_name.compare("isentropic_vortex") ) {

    init_isentropic_vortex(Udata);

  } else {

    std::cout << "Problem : " << m_problem_name
	      << " is not recognized / implemented."
	      << std::endl;
    std::cout <<  "Use default - implode" << std::endl;
    init_implode(Udata);

  }

} // SolverHydroMuscl::init / 2d

// =======================================================
// =======================================================
template<>
void SolverHydroMuscl<3>::init(DataArray Udata)
{

  /*
   * initialize hydro array at t=0
   */
  if ( !m_problem_name.compare("implode") ) {

    init_implode(Udata);

  } else if ( !m_problem_name.compare("blast") ) {

    init_blast(Udata);

  } else {

    std::cout << "Problem : " << m_problem_name
	      << " is not recognized / implemented."
	      << std::endl;
    std::cout <<  "Use default - implode" << std::endl;
    init_implode(Udata);

  }

} // SolverHydroMuscl<3>::init

// =======================================================
// =======================================================
// ///////////////////////////////////////////
// Actual computation of Godunov scheme - 2d
// ///////////////////////////////////////////
template<>
void SolverHydroMuscl<2>::godunov_unsplit_impl(DataArray data_in, 
					       DataArray data_out, 
					       real_t dt)
{

  real_t dtdx;
  real_t dtdy;
  
  dtdx = dt / params.dx;
  dtdy = dt / params.dy;

  // fill ghost cell in data_in
  timers[TIMER_BOUNDARIES]->start();
  make_boundaries(data_in);
  timers[TIMER_BOUNDARIES]->stop();
    
  // copy data_in into data_out (not necessary)
  // data_out = data_in;
  Kokkos::deep_copy(data_out, data_in);
  
  // start main computation
  timers[TIMER_NUM_SCHEME]->start();

  // convert conservative variable into primitives ones for the entire domain
  convertToPrimitives(data_in);

  if (params.implementationVersion == 0) {
    
    // compute fluxes
    {
      ComputeAndStoreFluxesFunctor2D functor(params, Q,
					     Fluxes_x, Fluxes_y,
					     dtdx, dtdy);
      Kokkos::parallel_for(nbCells, functor);
      //save_data_debug(Fluxes_x, Uhost, m_times_saved, m_t, "flux_x");
      //save_data_debug(Fluxes_y, Uhost, m_times_saved, m_t, "flux_y");
    }

    // actual update
    {
      UpdateFunctor2D functor(params, data_out,
			      Fluxes_x, Fluxes_y);
      Kokkos::parallel_for(nbCells, functor);
    }
    
  } else if (params.implementationVersion == 1) {

    // call device functor to compute slopes
    ComputeSlopesFunctor2D computeSlopesFunctor(params, Q, Slopes_x, Slopes_y);
    Kokkos::parallel_for(nbCells, computeSlopesFunctor);

    // now trace along X axis
    {
      ComputeTraceAndFluxes_Functor2D<XDIR> functor(params, Q,
						    Slopes_x, Slopes_y,
						    Fluxes_x,
						    dtdx, dtdy);
      Kokkos::parallel_for(nbCells, functor);
    }
    
    // and update along X axis
    {
      UpdateDirFunctor2D<XDIR> functor(params, data_out, Fluxes_x);
      Kokkos::parallel_for(nbCells, functor);
    }
    
    // now trace along Y axis
    {
      ComputeTraceAndFluxes_Functor2D<YDIR> functor(params, Q,
						    Slopes_x, Slopes_y,
						    Fluxes_y,
						    dtdx, dtdy);
      Kokkos::parallel_for(nbCells, functor);
    }
    
    // and update along Y axis
    {
      UpdateDirFunctor2D<YDIR> functor(params, data_out, Fluxes_y);
      Kokkos::parallel_for(nbCells, functor);
    }
    
  } // end params.implementationVersion == 1
  
  timers[TIMER_NUM_SCHEME]->stop();
  
} // SolverHydroMuscl2D::godunov_unsplit_impl

// =======================================================
// =======================================================
// ///////////////////////////////////////////
// Actual computation of Godunov scheme - 3d
// ///////////////////////////////////////////
template<>
void SolverHydroMuscl<3>::godunov_unsplit_impl(DataArray data_in, 
					       DataArray data_out, 
					       real_t dt)
{
  real_t dtdx;
  real_t dtdy;
  real_t dtdz;
  
  dtdx = dt / params.dx;
  dtdy = dt / params.dy;
  dtdz = dt / params.dz;

  // fill ghost cell in data_in
  timers[TIMER_BOUNDARIES]->start();
  make_boundaries(data_in);
  timers[TIMER_BOUNDARIES]->stop();
    
  // copy data_in into data_out (not necessary)
  // data_out = data_in;
  Kokkos::deep_copy(data_out, data_in);
  
  // start main computation
  timers[TIMER_NUM_SCHEME]->start();

  // convert conservative variable into primitives ones for the entire domain
  convertToPrimitives(data_in);

  if (params.implementationVersion == 0) {
    
    // compute fluxes
    {
      ComputeAndStoreFluxesFunctor3D functor(params, Q,
					     Fluxes_x, Fluxes_y, Fluxes_z,
					     dtdx, dtdy, dtdz);
      Kokkos::parallel_for(nbCells, functor);
    }

    // actual update
    {
      UpdateFunctor3D functor(params, data_out,
			      Fluxes_x, Fluxes_y, Fluxes_z);
      Kokkos::parallel_for(nbCells, functor);
    }
    
  } else if (params.implementationVersion == 1) {

    // call device functor to compute slopes
    ComputeSlopesFunctor3D computeSlopesFunctor(params, Q,
						Slopes_x, Slopes_y, Slopes_z);
    Kokkos::parallel_for(nbCells, computeSlopesFunctor);

    // now trace along X axis
    {
      ComputeTraceAndFluxes_Functor3D<XDIR> functor(params, Q,
						    Slopes_x, Slopes_y, Slopes_z,
						    Fluxes_x,
						    dtdx, dtdy, dtdz);
      Kokkos::parallel_for(nbCells, functor);
    }
    
    // and update along X axis
    {
      UpdateDirFunctor3D<XDIR> functor(params, data_out, Fluxes_x);
      Kokkos::parallel_for(nbCells, functor);
    }

    // now trace along Y axis
    {
      ComputeTraceAndFluxes_Functor3D<YDIR> functor(params, Q,
						    Slopes_x, Slopes_y, Slopes_z,
						    Fluxes_y,
						    dtdx, dtdy, dtdz);
      Kokkos::parallel_for(nbCells, functor);
    }
    
    // and update along Y axis
    {
      UpdateDirFunctor3D<YDIR> functor(params, data_out, Fluxes_y);
      Kokkos::parallel_for(nbCells, functor);
    }

    // now trace along Z axis
    {
      ComputeTraceAndFluxes_Functor3D<ZDIR> functor(params, Q,
						    Slopes_x, Slopes_y, Slopes_z,
						    Fluxes_z,
						    dtdx, dtdy, dtdz);
      Kokkos::parallel_for(nbCells, functor);
    }
    
    // and update along Z axis
    {
      UpdateDirFunctor3D<ZDIR> functor(params, data_out, Fluxes_z);
      Kokkos::parallel_for(nbCells, functor);
    }

  } // end params.implementationVersion == 1
  
  timers[TIMER_NUM_SCHEME]->stop();

} // SolverHydroMuscl<3>::godunov_unsplit_impl

// =======================================================
// =======================================================
// ///////////////////////////////////////////
// Actual CPU computation of Godunov scheme
// ///////////////////////////////////////////
template<int dim>
void SolverHydroMuscl<dim>::godunov_unsplit_impl(DataArray data_in, 
						 DataArray data_out, 
						 real_t dt)
{

  // 2d / 3d implementation are specialized in implementation file
  
} // SolverHydroMuscl3D::godunov_unsplit_impl

} // namespace muscl

} // namespace ppkMHD
