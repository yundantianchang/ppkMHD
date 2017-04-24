#ifndef ISENTROPIC_VORTEX_PARAMS_H_
#define ISENTROPIC_VORTEX_PARAMS_H_

#include "utils/config/ConfigMap.h"

/**
 * isentropic vortex advection test parameters.
 */
struct IsentropicVortexParams {

  // isentropic vortex ambient flow
  real_t rho_a;
  real_t p_a;
  real_t T_a;
  real_t u_a;
  real_t v_a;
  real_t w_a;

  // vortex center
  real_t vortex_x;
  real_t vortex_y;
  real_t vortex_z;
  
  IsentropicVortexParams(ConfigMap& configMap)
  {

    double xmin = configMap.getFloat("mesh", "xmin", 0.0);
    double ymin = configMap.getFloat("mesh", "ymin", 0.0);
    double zmin = configMap.getFloat("mesh", "zmin", 0.0);

    double xmax = configMap.getFloat("mesh", "xmax", 1.0);
    double ymax = configMap.getFloat("mesh", "ymax", 1.0);
    double zmax = configMap.getFloat("mesh", "zmax", 1.0);
    
    rho_a  = configMap.getFloat("isentropic_vortex","density_ambient", 1.0);
    p_a  = configMap.getFloat("isentropic_vortex","pressure_ambient", 1.0);
    T_a  = configMap.getFloat("isentropic_vortex","temperature_ambient", 1.0);
    u_a  = configMap.getFloat("isentropic_vortex","vx_ambient", 1.0);
    v_a  = configMap.getFloat("isentropic_vortex","vy_ambient", 1.0);
    w_a  = configMap.getFloat("isentropic_vortex","vz_ambient", 1.0);

    
    vortex_x = configMap.getFloat("isentropic_vortex","center_x", (xmin+xmax)/2);
    vortex_y = configMap.getFloat("isentropic_vortex","center_y", (ymin+ymax)/2);
    vortex_z = configMap.getFloat("isentropic_vortex","center_z", (zmin+zmax)/2);
    
  }

}; // struct IsentropicVortexParams

#endif // ISENTROPIC_VORTEX_PARAMS_H_
