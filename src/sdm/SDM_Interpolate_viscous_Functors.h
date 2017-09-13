#ifndef SDM_INTERPOLATE_VISCOUS_FUNCTORS_H_
#define SDM_INTERPOLATE_VISCOUS_FUNCTORS_H_

#include <limits> // for std::numeric_limits
#ifdef __CUDA_ARCH__
#include <math_constants.h> // for cuda math constants, e.g. CUDART_INF
#endif // __CUDA_ARCH__

#include "shared/kokkos_shared.h"
#include "sdm/SDMBaseFunctor.h"

#include "sdm/SDM_Geometry.h"
#include "sdm/sdm_shared.h" // for DofMap

#include "shared/EulerEquations.h"

namespace sdm {

/*************************************************/
/*************************************************/
/*************************************************/
/**
 * This functor takes as an input conservative variables
 * at solution points and perform interpolation of velocities
 * at flux points. What happends at cell borders is the subject
 * of an another functor : Average_velocity_at_cell_borders_functor
 *
 * It is essentially a wrapper arround interpolation method sol2flux_vector.
 *
 * \sa Interpolate_At_FluxPoints_Functor
 *
 */
template<int dim, int N, int dir>
class Interpolate_velocities_Sol2Flux_Functor : public SDMBaseFunctor<dim,N> {

public:
  using typename SDMBaseFunctor<dim,N>::DataArray;
  using typename SDMBaseFunctor<dim,N>::solution_values_t;
  using typename SDMBaseFunctor<dim,N>::flux_values_t;
  
  static constexpr auto dofMapS = DofMap<dim,N>;
  static constexpr auto dofMapF = DofMapFlux<dim,N,dir>;
  
  Interpolate_velocities_Sol2Flux_Functor(HydroParams         params,
					  SDM_Geometry<dim,N> sdm_geom,
					  DataArray           UdataSol,
					  DataArray           UdataFlux) :
    SDMBaseFunctor<dim,N>(params,sdm_geom),
    UdataSol(UdataSol),
    UdataFlux(UdataFlux)
  {};

  
  // =========================================================
  /*
   * 2D version.
   */
  // =========================================================
  //! functor for 2d 
  template<int dim_ = dim>
  KOKKOS_INLINE_FUNCTION
  void operator()(const typename Kokkos::Impl::enable_if<dim_==2, int>::type& index) const
  {

    const int isize = this->params.isize;
    const int jsize = this->params.jsize;

    // local cell index
    int i,j;
    index2coord(index,i,j,isize,jsize);

    solution_values_t sol;
    flux_values_t     flux;
    
    // loop over cell DoF's
    if (dir == IX) {

      for (int idy=0; idy<N; ++idy) {

	// for each velocity components
	for (int ivar = IU; ivar<IU+dim; ++ivar) {
	
	  // get solution values vector along X direction
	  for (int idx=0; idx<N; ++idx) {

	    // divide momentum by density to obtain velocity
	    sol[idx] =
	      UdataSol(i  ,j  , dofMapS(idx,idy,0,ivar)) /
	      UdataSol(i  ,j  , dofMapS(idx,idy,0,ID));

	  }
	  
	  // interpolate at flux points for this given variable
	  this->sol2flux_vector(sol, flux);
	  
	  // copy back interpolated value
	  for (int idx=0; idx<N+1; ++idx) {
	    
	    UdataFlux(i  ,j  , dofMapF(idx,idy,0,ivar)) = flux[idx];
	    
	  } // end for idx
	  
	} // end for ivar
	
      } // end for idy

    } // end for dir IX

    // loop over cell DoF's
    if (dir == IY) {

      for (int idx=0; idx<N; ++idx) {

	// for each variables
	for (int ivar = IU; ivar<IU+dim; ++ivar) {
	
	  // get solution values vector along Y direction
	  for (int idy=0; idy<N; ++idy) {
	  
	    sol[idy] =
	      UdataSol(i  ,j  , dofMapS(idx,idy,0,ivar)) /
	      UdataSol(i  ,j  , dofMapS(idx,idy,0,ID));
	    
	  }
	  
	  // interpolate at flux points for this given variable
	  this->sol2flux_vector(sol, flux);
	  
	  // copy back interpolated value
	  for (int idy=0; idy<N+1; ++idy) {
	    
	    UdataFlux(i  ,j  , dofMapF(idx,idy,0,ivar)) = flux[idy];
	    
	  }
	  
	} // end for ivar
	
      } // end for idx

    } // end for dir IY
    
  } // end operator () - 2d

  // =========================================================
  /*
   * 3D version.
   */
  // =========================================================
  //! functor for 3d 
  template<int dim_ = dim>
  KOKKOS_INLINE_FUNCTION
  void operator()(const typename Kokkos::Impl::enable_if<dim_==3, int>::type& index) const
  {

    const int isize = this->params.isize;
    const int jsize = this->params.jsize;
    const int ksize = this->params.ksize;

    // local cell index
    int i,j,k;
    index2coord(index,i,j,k,isize,jsize,ksize);

    solution_values_t sol;
    flux_values_t     flux;
    
    // loop over cell DoF's
    if (dir == IX) {

      for (int idz=0; idz<N; ++idz) {
	for (int idy=0; idy<N; ++idy) {
	  
	  // for each variables
	  for (int ivar = IU; ivar<IU+dim; ++ivar) {
	    
	    // get solution values vector along X direction
	    for (int idx=0; idx<N; ++idx) {
	      
	      sol[idx] =
		UdataSol(i,j,k, dofMapS(idx,idy,idz,ivar)) /
		UdataSol(i,j,k, dofMapS(idx,idy,idz,ID));
	      
	    }
	    
	    // interpolate at flux points for this given variable
	    this->sol2flux_vector(sol, flux);
	    	    
	    // copy back interpolated value
	    for (int idx=0; idx<N+1; ++idx) {
	      
	      UdataFlux(i,j,k, dofMapF(idx,idy,idz,ivar)) = flux[idx];
	      
	    }
	    
	  } // end for ivar
	  
	} // end for idy
      } // end for idz
      
    } // end for dir IX

    // loop over cell DoF's
    if (dir == IY) {

      for (int idz=0; idz<N; ++idz) {
	for (int idx=0; idx<N; ++idx) {
	  
	  // for each variables
	  for (int ivar = IU; ivar<IU+dim; ++ivar) {
	    
	    // get solution values vector along Y direction
	    for (int idy=0; idy<N; ++idy) {
	      
	      sol[idy] =
		UdataSol(i,j,k, dofMapS(idx,idy,idz,ivar)) /
		UdataSol(i,j,k, dofMapS(idx,idy,idz,ID));
	    
	    }
	    
	    // interpolate at flux points for this given variable
	    this->sol2flux_vector(sol, flux);
	    	    
	    // copy back interpolated value
	    for (int idy=0; idy<N+1; ++idy) {
	      
	      UdataFlux(i,j,k, dofMapF(idx,idy,idz,ivar)) = flux[idy];
	      
	    }
	  
	  } // end for ivar
	
	} // end for idx
      } // end for idz

    } // end for dir IY

    // loop over cell DoF's
    if (dir == IZ) {

      for (int idy=0; idy<N; ++idy) {
	for (int idx=0; idx<N; ++idx) {
	  
	  // for each variables
	  for (int ivar = IU; ivar<IU+dim; ++ivar) {
	    
	    // get solution values vector along Y direction
	    for (int idz=0; idz<N; ++idz) {
	      
	      sol[idz] =
		UdataSol(i,j,k, dofMapS(idx,idy,idz,ivar)) /
		UdataSol(i,j,k, dofMapS(idx,idy,idz,ID));
	      
	    }
	    
	    // interpolate at flux points for this given variable
	    this->sol2flux_vector(sol, flux);
	    
	    // copy back interpolated value
	    for (int idz=0; idz<N+1; ++idz) {
	      
	      UdataFlux(i,j,k, dofMapF(idx,idy,idz,ivar)) = flux[idz];
	      
	    }
	  
	  } // end for ivar
	
	} // end for idx
      } // end for idz

    } // end for dir IZ

  } // end operator () - 3d
  
  DataArray UdataSol, UdataFlux;

}; // class Interpolate_velocities_Sol2Flux_Functor


/*************************************************/
/*************************************************/
/*************************************************/
/**
 * This functor takes as an input a flux array, and perform
 * a simple average (half sum) at cell borders.
 *
 */
template<int dim, int N, int dir>
class Average_velocity_at_cell_borders_Functor : public SDMBaseFunctor<dim,N> {

public:
  using typename SDMBaseFunctor<dim,N>::DataArray;
  using typename SDMBaseFunctor<dim,N>::HydroState;
  
  static constexpr auto dofMapF = DofMapFlux<dim,N,dir>;

  /**
   * \param[in] params
   * \param[in] sdm_geom
   * \param[in,out] UdataFlux a flux array
   */
  Average_velocity_at_cell_borders_Functor(HydroParams         params,
					   SDM_Geometry<dim,N> sdm_geom,
					   DataArray           UdataFlux) :
    SDMBaseFunctor<dim,N>(params,sdm_geom),
    UdataFlux(UdataFlux)
  {};

  
  // =========================================================
  /*
   * 2D version.
   */
  // =========================================================
  //! functor for 2d 
  template<int dim_ = dim>
  KOKKOS_INLINE_FUNCTION
  void operator()(const typename Kokkos::Impl::enable_if<dim_==2, int>::type& index) const
  {

    const int isize = this->params.isize;
    const int jsize = this->params.jsize;

    // local cell index
    int i,j;
    index2coord(index,i,j,isize,jsize);

    real_t dataL, dataR, data_average;
    
    // =========================
    // ========= DIR X =========
    // =========================
    if (dir == IX) {

      // avoid ghost cells
      if (i>0 and i<isize) {

	// just deals with the left cell border
	for (int idy=0; idy<N; ++idy) {

	  for (int ivar : { IU,IV } ) {
	    dataL = UdataFlux(i-1,j, dofMapF(N,idy,0,ivar));
	    dataR = UdataFlux(i  ,j, dofMapF(0,idy,0,ivar));
	    data_average = 0.5*(dataL + dataR);
	    UdataFlux(i-1,j, dofMapF(N,idy,0,ivar)) = data_average;
	    UdataFlux(i  ,j, dofMapF(0,idy,0,ivar)) = data_average;
	  }
	  
	} // end for idy

      } // end ghost cells guard
      
    } // end dir IX
    
    // =========================
    // ========= DIR Y =========
    // =========================
    if (dir == IY) {

      // avoid ghost cells
      if (j>0 and j<jsize) {

	// just deals with the left cell border
	for (int idx=0; idx<N; ++idx) {

	  for (int ivar : { IU,IV } ) {
	    dataL = UdataFlux(i,j-1, dofMapF(idx,N,0,ivar));
	    dataR = UdataFlux(i,j  , dofMapF(idx,0,0,ivar));
	    data_average = 0.5*(dataL + dataR);
	    UdataFlux(i,j-1, dofMapF(idx,N,0,ivar)) = data_average;
	    UdataFlux(i,j  , dofMapF(idx,0,0,ivar)) = data_average;
	  }
	  
	} // end for idy

      } // end ghost cells guard
      
    } // end dir IY
    
  } // end operator() - 2d
  
  // =========================================================
  /*
   * 3D version.
   */
  // =========================================================
  //! functor for 3d 
  template<int dim_ = dim>
  KOKKOS_INLINE_FUNCTION
  void operator()(const typename Kokkos::Impl::enable_if<dim_==3, int>::type& index) const
  {

    const int isize = this->params.isize;
    const int jsize = this->params.jsize;
    const int ksize = this->params.ksize;

    // local cell index
    int i,j,k;
    index2coord(index,i,j,k,isize,jsize,ksize);

    real_t dataL, dataR, data_average;
    
    // =========================
    // ========= DIR X =========
    // =========================
    if (dir == IX) {

      // avoid ghost cells
      if (i>0 and i<isize) {

	// just deals with the left cell border
	for (int idz=0; idz<N; ++idz) {
	  for (int idy=0; idy<N; ++idy) {

	    for (int ivar : { IU,IV,IW } ) {
	      dataL = UdataFlux(i-1,j, k, dofMapF(N,idy,idz,ivar));
	      dataR = UdataFlux(i  ,j, k, dofMapF(0,idy,idz,ivar));
	      data_average = 0.5*(dataL + dataR);
	      UdataFlux(i-1,j, k, dofMapF(N,idy,idz,ivar)) = data_average;
	      UdataFlux(i  ,j, k, dofMapF(0,idy,idz,ivar)) = data_average;
	    } // end for ivar
	  
	  } // end for idy
	} // end for idz
	
      } // end ghost cells guard
      
    } // end dir IX

    // =========================
    // ========= DIR Y =========
    // =========================
    if (dir == IY) {

      // avoid ghost cells
      if (j>0 and j<jsize) {

	// just deals with the left cell border
	for (int idz=0; idz<N; ++idz) {
	  for (int idx=0; idx<N; ++idx) {

	    for (int ivar : { IU,IV,IW } ) {
	      dataL = UdataFlux(i,j-1, k, dofMapF(idx,N,idz,ivar));
	      dataR = UdataFlux(i,j  , k, dofMapF(idx,0,idz,ivar));
	      data_average = 0.5*(dataL + dataR);
	      UdataFlux(i,j-1, k, dofMapF(idx,N,idz,ivar)) = data_average;
	      UdataFlux(i,j  , k, dofMapF(idx,0,idz,ivar)) = data_average;
	    } // end for ivar
	  
	  } // end for idx
	} // end for idz
	
      } // end ghost cells guard
      
    } // end dir IY

    // =========================
    // ========= DIR Z =========
    // =========================
    if (dir == IZ) {

      // avoid ghost cells
      if (k>0 and k<ksize) {

	// just deals with the left cell border
	for (int idy=0; idy<N; ++idy) {
	  for (int idx=0; idx<N; ++idx) {

	    for (int ivar : { IU,IV,IW } ) {
	      dataL = UdataFlux(i,j, k-1, dofMapF(idx,idy,N,ivar));
	      dataR = UdataFlux(i,j, k  , dofMapF(idx,idy,0,ivar));
	      data_average = 0.5*(dataL + dataR);
	      UdataFlux(i,j, k-1, dofMapF(idx,idy,N,ivar)) = data_average;
	      UdataFlux(i,j, k  , dofMapF(idx,idy,0,ivar)) = data_average;
	    } // end for ivar
	  
	  } // end for idx
	} // end for idy
	
      } // end ghost cells guard
      
    } // end dir IZ

  } // end operator() - 3d
    
  DataArray UdataFlux;

}; // class Average_velocity_at_cell_borders_Functor

/*************************************************/
/*************************************************/
/*************************************************/
/**
 * This functor takes as an input a fluxes data array (only IU,IV, IW are used)
 * and perform interpolation at solution points.
 *
 * Its is essentially a wrapper arround interpolation method flux2sol_vector.
 *
 */
template<int dim, int N, int dir,
	 Interpolation_type_t itype=INTERPOLATE_DERIVATIVE>
class Interp_grad_velocity_at_SolutionPoints_Functor : public SDMBaseFunctor<dim,N> {
  
public:
  using typename SDMBaseFunctor<dim,N>::DataArray;
  using typename SDMBaseFunctor<dim,N>::solution_values_t;
  using typename SDMBaseFunctor<dim,N>::flux_values_t;

  using SDMBaseFunctor<dim,N>::IGU;
  using SDMBaseFunctor<dim,N>::IGV;
  using SDMBaseFunctor<dim,N>::IGW;
  using SDMBaseFunctor<dim,N>::IGT;

  static constexpr auto dofMapS = DofMap<dim,N>;
  static constexpr auto dofMapF = DofMapFlux<dim,N,dir>;
  
  Interp_grad_velocity_at_SolutionPoints_Functor(HydroParams         params,
						 SDM_Geometry<dim,N> sdm_geom,
						 DataArray           UdataFlux,
						 DataArray           UdataSol) :
    SDMBaseFunctor<dim,N>(params,sdm_geom),
    UdataFlux(UdataFlux),
    UdataSol(UdataSol)
  {};

  // =========================================================
  /*
   * 2D version.
   */
  // =========================================================
  //! functor for 2d 
  template<int dim_ = dim>
  KOKKOS_INLINE_FUNCTION
  void operator()(const typename Kokkos::Impl::enable_if<dim_==2, int>::type& index) const
  {

    const int isize = this->params.isize;
    const int jsize = this->params.jsize;

    // rescale factor for derivative
    real_t rescale = 1.0/this->params.dx;
    if (dir == IY)
      rescale = 1.0/this->params.dy;
    
    // local cell index
    int i,j;
    index2coord(index,i,j,isize,jsize);

    solution_values_t sol;
    flux_values_t     flux;
    
    Kokkos::Array<int,2> index_in  = { IU, IV};
    Kokkos::Array<int,2> index_out = {IGU,IGV};

    // loop over cell DoF's
    if (dir == IX) {

      for (int idy=0; idy<N; ++idy) {

	// for each variables
	for (int ivar = 0; ivar<dim; ++ivar ) {
	
	  // get values at flux point along X direction
	  for (int idx=0; idx<N+1; ++idx)
	    flux[idx] = UdataFlux(i  ,j  , dofMapF(idx,idy,0,index_in[ivar]));
	    
	  // interpolate at flux points for this given variable
	  if (itype==INTERPOLATE_SOLUTION)
	    this->flux2sol_vector(flux, sol);
	  else
	    this->flux2sol_derivative_vector(flux,sol,rescale);
	  
	  // copy back interpolated value
	  for (int idx=0; idx<N; ++idx)
	    UdataSol(i  ,j  , dofMapS(idx,idy,0,index_out[ivar])) = sol[idx];
	  
	} // end for ivar
	
      } // end for idy

    } // end for dir IX

    // loop over cell DoF's
    if (dir == IY) {

      for (int idx=0; idx<N; ++idx) {

	// for each variables
	for (int ivar=0; ivar<dim; ++ivar ) {
	
	  // get values at flux point along Y direction
	  for (int idy=0; idy<N+1; ++idy)
	    flux[idy] = UdataFlux(i  ,j  , dofMapF(idx,idy,0,index_in[ivar]));
	    	  
	  // interpolate at flux points for this given variable
	  if (itype==INTERPOLATE_SOLUTION)
	    this->flux2sol_vector(flux, sol);
	  else
	    this->flux2sol_derivative_vector(flux,sol,rescale);
	  
	  // copy back interpolated value
	  for (int idy=0; idy<N; ++idy)
	    UdataSol(i  ,j  , dofMapS(idx,idy,0,index_out[ivar])) += sol[idy];
	  
	} // end for ivar
	
      } // end for idx

    } // end for dir IY
    
  } // end operator () - 2d

  // =========================================================
  /*
   * 3D version.
   */
  // =========================================================
  //! functor for 3d 
  template<int dim_ = dim>
  KOKKOS_INLINE_FUNCTION
  void operator()(const typename Kokkos::Impl::enable_if<dim_==3, int>::type& index) const
  {

    const int isize = this->params.isize;
    const int jsize = this->params.jsize;
    const int ksize = this->params.ksize;

    // rescale factor for derivative
    real_t rescale = 1.0/this->params.dx;
    if (dir == IY)
      rescale = 1.0/this->params.dy;
    if (dir == IZ)
      rescale = 1.0/this->params.dz;

    // local cell index
    int i,j,k;
    index2coord(index,i,j,k,isize,jsize,ksize);

    solution_values_t sol;
    flux_values_t     flux;
    
    Kokkos::Array<int,3> index_in  = { IU, IV, IW};
    Kokkos::Array<int,3> index_out = {IGU,IGV,IGW};

    // loop over cell DoF's
    if (dir == IX) {

      for (int idz=0; idz<N; ++idz) {
	for (int idy=0; idy<N; ++idy) {
	  
	  // for each variables
	  for (int ivar = 0; ivar<dim; ++ivar) {
	    
	    // get values at flux point along X direction
	    for (int idx=0; idx<N+1; ++idx)
	      flux[idx] = UdataFlux(i,j,k, dofMapF(idx,idy,idz,index_in[ivar]));
	  
	    // interpolate at flux points for this given variable
	    if (itype==INTERPOLATE_SOLUTION)
	      this->flux2sol_vector(flux, sol);
	    else
	      this->flux2sol_derivative_vector(flux,sol,rescale);
	    
	    // copy back interpolated value
	    for (int idx=0; idx<N; ++idx)
	      UdataSol(i,j,k, dofMapS(idx,idy,idz,index_out[ivar])) += sol[idx];
	    
	  } // end for ivar
	  
	} // end for idy
      } // end for idz

    } // end for dir IX

    // loop over cell DoF's
    if (dir == IY) {

      for (int idz=0; idz<N; ++idz) {
	for (int idx=0; idx<N; ++idx) {

	  // for each variables
	  for (int ivar=0; ivar<dim; ++ivar) {
	    
	    // get values at flux point along Y direction
	    for (int idy=0; idy<N+1; ++idy)
	      flux[idy] = UdataFlux(i,j,k, dofMapF(idx,idy,idz,index_in[ivar]));
	    
	    // interpolate at flux points for this given variable
	    if (itype==INTERPOLATE_SOLUTION)
	      this->flux2sol_vector(flux, sol);
	    else
	      this->flux2sol_derivative_vector(flux,sol,rescale);
	    
	    // copy back interpolated value
	    for (int idy=0; idy<N; ++idy)
	      UdataSol(i,j,k, dofMapS(idx,idy,idz,index_out[ivar])) += sol[idy];
	    
	  } // end for ivar
	  
	} // end for idx
      } // end for idz

    } // end for dir IY

    // loop over cell DoF's
    if (dir == IZ) {

      for (int idy=0; idy<N; ++idy) {
	for (int idx=0; idx<N; ++idx) {

	  // for each variables
	  for (int ivar=0; ivar<dim; ++ivar) {
	    
	    // get values at flux point along Y direction
	    for (int idz=0; idz<N+1; ++idz)
	      flux[idz] = UdataFlux(i,j,k, dofMapF(idx,idy,idz,index_in[ivar]));
	    
	    // interpolate at flux points for this given variable
	    if (itype==INTERPOLATE_SOLUTION)
	      this->flux2sol_vector(flux, sol);
	    else
	      this->flux2sol_derivative_vector(flux,sol,rescale);
	    
	    // copy back interpolated value
	    for (int idz=0; idz<N; ++idz)
	      UdataSol(i,j,k, dofMapS(idx,idy,idz,index_out[ivar])) += sol[idz];
	    
	  } // end for ivar
	  
	} // end for idx
      } // end for idy

    } // end for dir IZ

  } // end operator () - 3d
  
  DataArray UdataFlux, UdataSol;

}; // Interp_grad_velocity_at_SolutionPoints_Functor

} // namespace sdm

#endif // SDM_INTERPOLATE_VISCOUS_FUNCTORS_H_
