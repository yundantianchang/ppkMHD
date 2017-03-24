#ifndef MOOD_QUADRATURE_RULES_H_
#define MOOD_QUADRATURE_RULES_H_

#include <array>

#include "shared/real_type.h"

#include "mood/Stencil.h"

namespace mood {

/**
 * Quadrature rules: map stencil id to the required number of quadrature
 * points.
 *
 * In 3D, a tensor product must be done.
 */
constexpr std::array<int,STENCIL_TOTAL_NUMBER> QUADRATURE_NUM_POINTS =
  {1,  /* STENCIL_2D_DEGREE1 */
   2,  /* STENCIL_2D_DEGREE2 */
   2,  /* STENCIL_2D_DEGREE3 */
   2,  /* STENCIL_2D_DEGREE3_V2 */
   3,  /* STENCIL_2D_DEGREE4 */
   3,  /* STENCIL_2D_DEGREE5 */
   1,  /* STENCIL_3D_DEGREE1 */
   2,  /* STENCIL_3D_DEGREE2 */
   2,  /* STENCIL_3D_DEGREE3 */
   2,  /* STENCIL_3D_DEGREE3_V2 */
   3,  /* STENCIL_3D_DEGREE4 */
   3,  /* STENCIL_3D_DEGREE5 */
   3   /* STENCIL_3D_DEGREE5_V2 */
  };

/**
 * Quadrature weights when using 1 point (Gauss-Legendre).
 */
constexpr std::array<real_t,1> QUADRATURE_WEIGHTS_N1 =
  {
    1.0
  };

/**
 * Quadrature weights when using 2 points (Gauss-Legendre).
 */
constexpr std::array<real_t,2> QUADRATURE_WEIGHTS_N2 =
  {
    0.5, 0.5
  };

/**
 * Quadrature weights when using 3 points (Gauss-Legendre).
 */
constexpr std::array<real_t,3> QUADRATURE_WEIGHTS_N3 =
  {
    5.0/18, 8.0/18, 5.0/18
  };

// ============================================================================
// ============================================================================
// ============================================================================
//                       2D
// ============================================================================

/**
 * X,Y coordinates of quadrature points relative to cell center (0,0)
 * when 1 point is enougth.
 *
 * Left interface along X axis.
 */
constexpr std::array<std::array<real_t,2>,1> QUADRATURE_LOCATION_2D_N1_X_M =
  {
    {-0.5, 0.0}
  };

/**
 * X,Y coordinates of quadrature points relative to cell center (0,0)
 * when 1 point is enougth.
 *
 * Right interface along X axis.
 */
constexpr std::array<std::array<real_t,2>,1> QUADRATURE_LOCATION_2D_N1_X_P =
  {
    { 0.5, 0.0}
  };
  
/**
 * X,Y coordinates of quadrature points relative to cell center (0,0)
 * when 1 point is enougth.
 *
 * Left interface along Y axis.
 */
constexpr std::array<std::array<real_t,2>,1> QUADRATURE_LOCATION_2D_N1_Y_M =
  {
    { 0.0,-0.5}
  };
    
/**
 * X,Y coordinates of quadrature points relative to cell center (0,0)
 * when 1 point is enougth.
 *
 * Right interface along Y axis.
 */
constexpr std::array<std::array<real_t,2>,1> QUADRATURE_LOCATION_2D_N1_Y_P =
  {
    { 0.0, 0.5}
  };
    
/**
 * X,Y coordinates of Gauss-Legendre quadrature points relative to cell center (0,0)
 * when 2 points are enougth.
 */
constexpr std::array<std::array<real_t,2>,2> QUADRATURE_LOCATION_2D_N2_X_M =
  {
    {-0.5, -0.5/sqrt(3.0)},
    {-0.5,  0.5/sqrt(3.0)},
  };

constexpr std::array<std::array<real_t,2>,2> QUADRATURE_LOCATION_2D_N2_X_P =
  {
    { 0.5, -0.5/sqrt(3.0)},
    { 0.5,  0.5/sqrt(3.0)},
  };

constexpr std::array<std::array<real_t,2>,2> QUADRATURE_LOCATION_2D_N2_Y_M =
  {
    {-0.5/sqrt(3.0), -0.5},
    { 0.5/sqrt(3.0), -0.5},
  };

constexpr std::array<std::array<real_t,2>,2> QUADRATURE_LOCATION_2D_N2_Y_P =
  {
    {-0.5/sqrt(3.0),  0.5},
    { 0.5/sqrt(3.0),  0.5},
  };

/**
 * X,Y coordinates of Gauss-Legendre quadrature points relative to cell center (0,0)
 * when 3 points are enougth.
 */
constexpr std::array<std::array<real_t,2>,3> QUADRATURE_LOCATION_2D_N3_X_M =
  {
    {-0.5, -0.5*sqrt(3.0/5.0)},
    {-0.5,  0.0},
    {-0.5,  0.5*sqrt(3.0/5.0)},
  };

constexpr std::array<std::array<real_t,2>,3> QUADRATURE_LOCATION_2D_N3_X_P =
  {
    { 0.5, -0.5*sqrt(3.0/5.0)},
    { 0.5,  0.0},
    { 0.5,  0.5*sqrt(3.0/5.0)},
  };

constexpr std::array<std::array<real_t,2>,3> QUADRATURE_LOCATION_2D_N3_Y_M =
  {
    {-0.5*sqrt(3.0/5.0), -0.5},
    { 0.0,               -0.5},
    { 0.5*sqrt(3.0/5.0), -0.5},
  };

constexpr std::array<std::array<real_t,2>,3> QUADRATURE_LOCATION_2D_N3_Y_P =
  {
    {-0.5*sqrt(3.0/5.0),  0.5},
    { 0.0,                0.5},
    { 0.5*sqrt(3.0/5.0),  0.5},
  };

// ============================================================================
// ============================================================================
// ============================================================================
//                       3D
// ============================================================================

/**
 * X,Y,Z coordinates of quadrature points relative to cell center (0,0,0)
 * when 1 point is enougth.
 *
 * Left interface along X axis.
 */
constexpr std::array<std::array<real_t,3>,1> QUADRATURE_LOCATION_3D_N1_X_M =
  {
    {-0.5, 0.0, 0.0}
  };

/**
 * X,Y,Z coordinates of quadrature points relative to cell center (0,0,0)
 * when 1 point is enougth.
 *
 * Right interface along X axis.
 */
constexpr std::array<std::array<real_t,3>,1> QUADRATURE_LOCATION_3D_N1_X_P =
  {
    { 0.5, 0.0, 0.0}
  };

/**
 * X,Y,Z coordinates of quadrature points relative to cell center (0,0,0)
 * when 1 point is enougth.
 *
 * Left interface along Y axis.
 */
constexpr std::array<std::array<real_t,3>,1> QUADRATURE_LOCATION_3D_N1_Y_M =
  {
    { 0.0,-0.5, 0.0}
  };

/**
 * X,Y,Z coordinates of quadrature points relative to cell center (0,0,0)
 * when 1 point is enougth.
 *
 * Right interface along Y axis.
 */
constexpr std::array<std::array<real_t,3>,1> QUADRATURE_LOCATION_3D_N1_Y_P =
  {
    { 0.0, 0.5, 0.0}
  };

/**
 * X,Y,Z coordinates of quadrature points relative to cell center (0,0,0)
 * when 1 point is enougth.
 *
 * Left interface along Z axis.
 */
constexpr std::array<std::array<real_t,3>,1> QUADRATURE_LOCATION_3D_N1_Z_M =
  {
    { 0.0, 0.0,-0.5}
  };

/**
 * X,Y,Z coordinates of quadrature points relative to cell center (0,0,0)
 * when 1 point is enougth.
 *
 * Right interface along Z axis.
 */
constexpr std::array<std::array<real_t,3>,1> QUADRATURE_LOCATION_3D_N1_Z_P =
  {
    { 0.0, 0.0, 0.5}
  };

/**
 * X,Y,Z coordinates of Gauss-Legendre quadrature points relative to cell center (0,0,0)
 * when 2 points are enougth.
 */
constexpr std::array<std::array<real_t,3>,4> QUADRATURE_LOCATION_3D_N2_X_M =
  {
    {-0.5, -0.5/sqrt(3.0), -0.5/sqrt(3.0)},
    {-0.5,  0.5/sqrt(3.0), -0.5/sqrt(3.0)},
    {-0.5, -0.5/sqrt(3.0),  0.5/sqrt(3.0)},
    {-0.5,  0.5/sqrt(3.0),  0.5/sqrt(3.0)},
  };

constexpr std::array<std::array<real_t,3>,4> QUADRATURE_LOCATION_3D_N2_X_P =
  {
    { 0.5, -0.5/sqrt(3.0), -0.5/sqrt(3.0)},
    { 0.5,  0.5/sqrt(3.0), -0.5/sqrt(3.0)},
    { 0.5, -0.5/sqrt(3.0),  0.5/sqrt(3.0)},
    { 0.5,  0.5/sqrt(3.0),  0.5/sqrt(3.0)},
  };

constexpr std::array<std::array<real_t,3>,4> QUADRATURE_LOCATION_3D_N2_Y_M =
  {
    {-0.5/sqrt(3.0), -0.5, -0.5/sqrt(3.0)},
    { 0.5/sqrt(3.0), -0.5, -0.5/sqrt(3.0)},
    {-0.5/sqrt(3.0), -0.5,  0.5/sqrt(3.0)},
    { 0.5/sqrt(3.0), -0.5,  0.5/sqrt(3.0)},
  };

constexpr std::array<std::array<real_t,3>,4> QUADRATURE_LOCATION_3D_N2_Y_P =
  {
    {-0.5/sqrt(3.0),  0.5, -0.5/sqrt(3.0)},
    { 0.5/sqrt(3.0),  0.5, -0.5/sqrt(3.0)},
    {-0.5/sqrt(3.0),  0.5,  0.5/sqrt(3.0)},
    { 0.5/sqrt(3.0),  0.5,  0.5/sqrt(3.0)},
  };

constexpr std::array<std::array<real_t,3>,4> QUADRATURE_LOCATION_3D_N2_Z_M =
  {
    {-0.5/sqrt(3.0), -0.5/sqrt(3.0), -0.5},
    { 0.5/sqrt(3.0), -0.5/sqrt(3.0), -0.5},
    {-0.5/sqrt(3.0),  0.5/sqrt(3.0), -0.5},
    { 0.5/sqrt(3.0),  0.5/sqrt(3.0), -0.5},
  };

constexpr std::array<std::array<real_t,3>,4> QUADRATURE_LOCATION_3D_N2_Z_P =
  {
    {-0.5/sqrt(3.0), -0.5/sqrt(3.0),  0.5},
    { 0.5/sqrt(3.0), -0.5/sqrt(3.0),  0.5},
    {-0.5/sqrt(3.0),  0.5/sqrt(3.0),  0.5},
    { 0.5/sqrt(3.0),  0.5/sqrt(3.0),  0.5},
  };

/**
 * X,Y,Z coordinates of Gauss-Legendre quadrature points relative to cell center (0,0,0)
 * when 2 points are enougth.
 */
constexpr std::array<std::array<real_t,3>,9> QUADRATURE_LOCATION_3D_N3_X_M =
  {
    {-0.5, -0.5*sqrt(3.0/5.0), -0.5*sqrt(3.0/5.0)},
    {-0.5,  0.0,               -0.5*sqrt(3.0/5.0)},
    {-0.5,  0.5*sqrt(3.0/5.0), -0.5*sqrt(3.0/5.0)},
    {-0.5, -0.5*sqrt(3.0/5.0),  0.0},
    {-0.5,  0.0,                0.0},
    {-0.5,  0.5*sqrt(3.0/5.0),  0.0},
    {-0.5, -0.5*sqrt(3.0/5.0),  0.5*sqrt(3.0/5.0)},
    {-0.5,  0.0,                0.5*sqrt(3.0/5.0)},
    {-0.5,  0.5*sqrt(3.0/5.0),  0.5*sqrt(3.0/5.0)},
  };

constexpr std::array<std::array<real_t,3>,9> QUADRATURE_LOCATION_3D_N3_X_P =
  {
    { 0.5, -0.5*sqrt(3.0/5.0), -0.5*sqrt(3.0/5.0)},
    { 0.5,  0.0,               -0.5*sqrt(3.0/5.0)},
    { 0.5,  0.5*sqrt(3.0/5.0), -0.5*sqrt(3.0/5.0)},
    { 0.5, -0.5*sqrt(3.0/5.0),  0.0},
    { 0.5,  0.0,                0.0},
    { 0.5,  0.5*sqrt(3.0/5.0),  0.0},
    { 0.5, -0.5*sqrt(3.0/5.0),  0.5*sqrt(3.0/5.0)},
    { 0.5,  0.0,                0.5*sqrt(3.0/5.0)},
    { 0.5,  0.5*sqrt(3.0/5.0),  0.5*sqrt(3.0/5.0)},
  };

constexpr std::array<std::array<real_t,3>,9> QUADRATURE_LOCATION_3D_N3_Y_M =
  {
    {-0.5*sqrt(3.0/5.0), -0.5, -0.5*sqrt(3.0/5.0)},
    { 0.0,               -0.5, -0.5*sqrt(3.0/5.0)},
    { 0.5*sqrt(3.0/5.0), -0.5, -0.5*sqrt(3.0/5.0)},
    {-0.5*sqrt(3.0/5.0), -0.5,  0.0},
    { 0.0,               -0.5,  0.0},
    { 0.5*sqrt(3.0/5.0), -0.5,  0.0},
    {-0.5*sqrt(3.0/5.0), -0.5,  0.5*sqrt(3.0/5.0)},
    { 0.0,               -0.5,  0.5*sqrt(3.0/5.0)},
    { 0.5*sqrt(3.0/5.0), -0.5,  0.5*sqrt(3.0/5.0)},
  };

constexpr std::array<std::array<real_t,3>,9> QUADRATURE_LOCATION_3D_N3_Y_P =
  {
    {-0.5*sqrt(3.0/5.0),  0.5, -0.5*sqrt(3.0/5.0)},
    { 0.0,                0.5, -0.5*sqrt(3.0/5.0)},
    { 0.5*sqrt(3.0/5.0),  0.5, -0.5*sqrt(3.0/5.0)},
    {-0.5*sqrt(3.0/5.0),  0.5,  0.0},
    { 0.0,                0.5,  0.0},
    { 0.5*sqrt(3.0/5.0),  0.5,  0.0},
    {-0.5*sqrt(3.0/5.0),  0.5,  0.5*sqrt(3.0/5.0)},
    { 0.0,                0.5,  0.5*sqrt(3.0/5.0)},
    { 0.5*sqrt(3.0/5.0),  0.5,  0.5*sqrt(3.0/5.0)},
  };

constexpr std::array<std::array<real_t,3>,9> QUADRATURE_LOCATION_3D_N3_Z_M =
  {
    {-0.5*sqrt(3.0/5.0), -0.5*sqrt(3.0/5.0), -0.5},
    { 0.0,               -0.5*sqrt(3.0/5.0), -0.5},
    { 0.5*sqrt(3.0/5.0), -0.5*sqrt(3.0/5.0), -0.5},
    {-0.5*sqrt(3.0/5.0),  0.0,               -0.5},
    { 0.0,                0.0,               -0.5},
    { 0.5*sqrt(3.0/5.0),  0.0,               -0.5},
    {-0.5*sqrt(3.0/5.0),  0.5*sqrt(3.0/5.0), -0.5},
    { 0.0,                0.5*sqrt(3.0/5.0), -0.5},
    { 0.5*sqrt(3.0/5.0),  0.5*sqrt(3.0/5.0), -0.5},
  };

constexpr std::array<std::array<real_t,3>,9> QUADRATURE_LOCATION_3D_N3_Z_P =
  {
    {-0.5*sqrt(3.0/5.0), -0.5*sqrt(3.0/5.0),  0.5},
    { 0.0,               -0.5*sqrt(3.0/5.0),  0.5},
    { 0.5*sqrt(3.0/5.0), -0.5*sqrt(3.0/5.0),  0.5},
    {-0.5*sqrt(3.0/5.0),  0.0,                0.5},
    { 0.0,                0.0,                0.5},
    { 0.5*sqrt(3.0/5.0),  0.0,                0.5},
    {-0.5*sqrt(3.0/5.0),  0.5*sqrt(3.0/5.0),  0.5},
    { 0.0,                0.5*sqrt(3.0/5.0),  0.5},
    { 0.5*sqrt(3.0/5.0),  0.5*sqrt(3.0/5.0),  0.5},
  };

} // namespace mood

#endif // MOOD_QUADRATURE_RULES_H_
