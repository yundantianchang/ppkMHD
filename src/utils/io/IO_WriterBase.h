#ifndef IO_WRITER_BASE_H_
#define IO_WRITER_BASE_H_

#include <kokkos_shared.h>

namespace ppkMHD { namespace io {

/**
 * Base class exposing the public interface method save_data.
 *
 * \note should this class be templated by DataArray ?
 */
class IO_WriterBase {

public:
  IO_WriterBase(int nbvar) : nbvar(nbvar) {};
  virtual ~IO_WriterBase() {};

  virtual void save_data(DataArray2d             Udata,
			 DataArray2d::HostMirror Uhost,
			 int iStep) {};

  virtual void save_data(DataArray3d             Udata,
			 DataArray3d::HostMirror Uhost,
			 int iStep) {};

  void set_nbvar(int nbvar_) {nbvar = nbvar_;}

protected:
    int nbvar;
  
}; // class IO_WriterBase

} // namespace io

} // namespace ppkMHD

#endif // IO_WRITER_BASE_H_