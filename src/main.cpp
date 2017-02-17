/**
 * Hydro/MHD solver (Muscl-Hancock).
 *
 * \date April, 16 2016
 * \author P. Kestener
 */

#include <cstdlib>
#include <cstdio>

#include "kokkos_shared.h"

#include "real_type.h"   // choose between single and double precision
#include "HydroParams.h" // read parameter file

// solver
#include "HydroRun2D.h"
//#include "HydroRun3D.h"

// for timer

#ifdef CUDA
#include "CudaTimer.h"
#else
#include "Timer.h"
#endif

int main(int argc, char *argv[])
{

#ifdef CUDA
  // Initialize Host mirror device
  Kokkos::HostSpace::execution_space::initialize(1);
  const unsigned device_count = Kokkos::Cuda::detect_device_count();

  // Use the last device:
  Kokkos::Cuda::initialize( Kokkos::Cuda::SelectDevice(device_count-1) );
#else
  Kokkos::initialize(argc, argv);
#endif

  {
    std::cout << "##########################\n";
    std::cout << "KOKKOS CONFIG             \n";
    std::cout << "##########################\n";
    
    std::ostringstream msg;
    std::cout << "Kokkos configuration" << std::endl;
    if ( Kokkos::hwloc::available() ) {
      msg << "hwloc( NUMA[" << Kokkos::hwloc::get_available_numa_count()
          << "] x CORE["    << Kokkos::hwloc::get_available_cores_per_numa()
          << "] x HT["      << Kokkos::hwloc::get_available_threads_per_core()
          << "] )"
          << std::endl ;
    }
#if defined( CUDA )
    Kokkos::Cuda::print_configuration( msg );
#else
    Kokkos::OpenMP::print_configuration( msg );
#endif
    std::cout << msg.str();
    std::cout << "##########################\n";
  }

  if (argc != 2) {
    fprintf(stderr, "Error: wrong number of argument; input filename must be the only parameter on the command line\n");
    exit(EXIT_FAILURE);
  }

  // read parameter file and initialize parameter
  // parse parameters from input file
  std::string input_file = std::string(argv[1]);
  ConfigMap configMap(input_file);

  // test: create a HydroParams object
  HydroParams params = HydroParams();
  params.setup(configMap);
  
  // print parameters on screen
  params.print();

  // initialize workspace memory (U, U2, ...)
  HydroRun2D *hydro = new HydroRun2D(params, configMap);
  hydro->save_solution();
  
  // start computation
  std::cout << "Start computation....\n";
  hydro->timers[TIMER_TOTAL]->start();

  // Hydrodynamics solver loop
  while ( ! hydro->finished() ) {

    hydro->next_iteration();

  } // end solver loop

  // end of computation
  hydro->timers[TIMER_TOTAL]->stop();

  // print monitoring information
  {    
    real_t t_tot   = hydro->timers[TIMER_TOTAL]->elapsed();
    real_t t_comp  = hydro->timers[TIMER_NUM_SCHEME]->elapsed();
    real_t t_dt    = hydro->timers[TIMER_DT]->elapsed();
    real_t t_bound = hydro->timers[TIMER_BOUNDARIES]->elapsed();
    real_t t_io    = hydro->timers[TIMER_IO]->elapsed();
    printf("total       time : %5.3f secondes\n",t_tot);
    printf("godunov     time : %5.3f secondes %5.2f%%\n",t_comp,100*t_comp/t_tot);
    printf("compute dt  time : %5.3f secondes %5.2f%%\n",t_dt,100*t_dt/t_tot);
    printf("boundaries  time : %5.3f secondes %5.2f%%\n",t_bound,100*t_bound/t_tot);
    printf("io          time : %5.3f secondes %5.2f%%\n",t_io,100*t_io/t_tot);
    printf("Perf             : %10.2f number of Mcell-updates/s\n",hydro->m_iteration*hydro->m_nCells/t_tot*1e-6);
  }

  delete hydro;

#ifdef CUDA
  Kokkos::Cuda::finalize();
  Kokkos::HostSpace::execution_space::finalize();
#else
  Kokkos::finalize();
#endif
  
  return EXIT_SUCCESS;

} // end main
