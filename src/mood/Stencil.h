#ifndef MOOD_STENCIL_H_
#define MOOD_STENCIL_H_

#include "shared/kokkos_shared.h"

namespace mood {

#ifndef STRINGIFY
#define STRINGIFY( value ) # value
#endif

/**
 * List of implemented stencil for MOOD numerical schemes, order is the order of
 * the multivariate polynomial used to interpolate value in the given stencil.
 *
 * Please note that, for a given order, the miminal stencil points need to compute
 * the polynomial coefficient (using least squares minimization) is the binomial 
 * number: (dim+order)! / dim! / order!
 * 
 * So the stencil size must be "significantly" larger than this number to estimate
 * the polynomial coefficients in a robust manner.
 * 
 * In 2D, the binomial coefficient (2+order)! / 2! / order! reduces to
 * (order+1)(order+2)/2
 *
 * - 2D - order 1 (minimum number of points is 2*3/2 = 3 )
 *  x
 * xox
 *  x
 *
 * - 2D - order 2 (minimum number of points is 3*4/2 = 6)
 * xxx
 * xox
 * xxx
 *
 * - 2D - order 3 (minimum number of points is 4*5/2 = 10)
 *   x
 *  xxx
 * xxoxx
 *  xxx
 *   x
 *
 * - 2D - order 3 v2 (minimum number of points is 4*5/2 = 10)
 *   
 * xxxx
 * xxox
 * xxxx
 * xxxx
 *
 * - 2D - order 4 (minimum number of points is 5*6/2 = 15)
 *   
 * xxxxx
 * xxxxx
 * xxoxx
 * xxxxx
 * xxxxx
 *
 * - 2D - order 5 (minimum number of points is 6*7/2 = 21)
 *   
 * xxxxxx
 * xxxxxx
 * xxxoxx
 * xxxxxx
 * xxxxxx
 * xxxxxx
 *
 * ---------------------------------------------------------
 *
 * In 3D, the binomial coefficient (3+order)! / 3! / order! reduces to
 * (order+1)(order+2)(order+3)/6
 *
 * - 3D - order 1 (minimum number of points is 2*3*4/6 =  4)
 * - 3D - order 2 (minimum number of points is 3*4*5/6 = 10)
 * - 3D - order 3 (minimum number of points is 4*5*6/6 = 20)
 * - 3D - order 4 (minimum number of points is 5*6*7/6 = 35)
 * - 3D - order 5 (minimum number of points is 6*7*8/6 = 56)
 *
 *
 */
enum STENCIL_ID {
  STENCIL_2D_ORDER1,
  STENCIL_2D_ORDER2,
  STENCIL_2D_ORDER3,
  STENCIL_2D_ORDER3_V2,
  STENCIL_2D_ORDER4,
  STENCIL_2D_ORDER5,
  STENCIL_3D_ORDER1,
  STENCIL_3D_ORDER2,
  STENCIL_3D_ORDER3,
  STENCIL_3D_ORDER3_V2,
  STENCIL_3D_ORDER4,
  STENCIL_3D_ORDER5,
  STENCIL_3D_ORDER5_V2,
};

/**
 * Return how many cell is made the stencil.
 *
 * \param[in] stencilId a valid value from enum STENCIL_ID
 *
 * \return number of cells contained in stencil.
 */
unsigned int get_stencil_size(STENCIL_ID stencilId);

/**
 *
 */
struct Stencil {

  using StencilArray     = Kokkos::View<int*[3], DEVICE>;
  using StencilArrayHost = StencilArray::HostMirror;
  
  //! number identify stencil (values from enum STENCIL_ID)
  STENCIL_ID stencilId;

  //! stencil size, is usefull for allocating offsets arrays
  unsigned int stencilSize;

  StencilArray offsets;

  StencilArrayHost offsets_h;

  bool is3dStencil;
  
  //! default constructor
  Stencil(STENCIL_ID stencilId) : stencilId(stencilId) {

    stencilSize = get_stencil_size(stencilId);

    is3dStencil = (stencilId >= STENCIL_3D_ORDER1) ? true : false;
    
    offsets   = StencilArray    ("offsets",   stencilSize);

    offsets_h = StencilArrayHost("offsets_h", stencilSize);

    init_stencil();
    
  }; // Stencil

  void init_stencil();

  void print_stencil();

  static STENCIL_ID select_stencil(unsigned int dim, unsigned int order);
  
}; // Stencil


} // namespace mood

#endif // MOOD_STENCIL_H_