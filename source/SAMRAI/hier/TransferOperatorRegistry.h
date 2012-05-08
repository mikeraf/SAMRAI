/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and COPYING.LESSER.
 *
 * Copyright:     (c) 1997-2012 Lawrence Livermore National Security, LLC
 * Description:   Singleton registry for all transfer operators.
 *
 ************************************************************************/

#ifndef included_hier_TransferOperatorRegistry
#define included_hier_TransferOperatorRegistry

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/hier/CoarsenOperator.h"
#include "SAMRAI/hier/RefineOperator.h"
#include "SAMRAI/hier/TimeInterpolateOperator.h"
#include "SAMRAI/hier/IntVector.h"
#include "SAMRAI/hier/Variable.h"
#include "SAMRAI/tbox/Boost.h"
#include "SAMRAI/tbox/Dimension.h"
#include "SAMRAI/tbox/Utilities.h"

#include <boost/shared_ptr.hpp>
#include <string>

BEGIN_BOOST_WARNING_SUPPRESSION
#include <boost/unordered_map.hpp>
END_BOOST_WARNING_SUPPRESSION

namespace SAMRAI {
namespace hier {

class BaseGridGeometry;

/*!
 * @brief Class TransferOperatorRegistry is intended to serve as the registry
 * for SAMRAI transfer operators.  It will be a singleton object held by class
 * hier::BaseGridGeometry.
 *
 * This hier::TransferOperatorRegistry class provides a lookup mechanism to
 * search for time interpolation and spatial coarsening/refining operators.
 * That is, algorithms and applications that manage communication on an
 * AMR hierarchy may query the hier::TransferOperatorRegistry object for
 * operators that may be applied to specific variables.
 *
 * Typically, the operators are assigned to the hier::TrnasferOperatorRegistry
 * object in the constructor of the geometry object that defines the mesh
 * coordinate system.
 *
 * @note
 * <ol>
 *   <li> Additional operators may be added to a transfer geometry object at
 *        any time during program execution. However, each operator must be
 *        added BEFORE it is requested or an unrecoverable assertion will be
 *        thrown and the program will abort.
 * </ol>
 *
 * See the time interpolation,spatial coarsening, and spatial refinement
 * operator base classes for more information about adding new operators for
 * either new patch data types or new operators for pre-existing patch data
 * types.
 *
 * @see hier::BaseGridGeometry
 * @see hier::RefineOperator
 * @see hier::CoarsenOperator
 * @see hier::TimeInterpolateOperator
 */
class TransferOperatorRegistry
{
public:
   /*!
    * @brief Set the state of the hier::TransferOperatorRegistry members.
    *
    * @param[in]  dim
    */
   explicit TransferOperatorRegistry(
      const tbox::Dimension& dim);

   /*!
    * @brief Destructor
    */
   virtual ~TransferOperatorRegistry();

   /*!
    * @brief Add a concrete spatial coarsening operator.
    *
    * @param[in]  var_type_name The type name of the variable with which
    *             coarsen_op is associated
    * @param[in]  coarsen_op The concrete coarsening operator to add to the
    *             lookup hash map.
    */
   void
   addCoarsenOperator(
      const char* var_type_name,
      const boost::shared_ptr<CoarsenOperator>& coarsen_op);

   /*!
    * @brief Add a concrete spatial refinement operator.
    *
    * @param[in]  var_type_name The type name of the variable with which
    *             refine_op is associated
    * @param[in]  refine_op The concrete refinement operator to add to the
    *             lookup hash map.
    */
   void
   addRefineOperator(
      const char* var_type_name,
      const boost::shared_ptr<RefineOperator>& refine_op);

   /*!
    * @brief Add a concrete time interpolation operator.
    *
    * @param[in]  var_type_name The type name of the variable with which
    *             time_op is associated
    * @param[in]  time_op The concrete time interpolation operator to add
    *             to the lookup hash map.
    */
   void
   addTimeInterpolateOperator(
      const char* var_type_name,
      const boost::shared_ptr<TimeInterpolateOperator>& time_op);

   /*!
    * @brief Lookup function for coarsening operator.
    *
    * Extract the spatial coarsening operator matching the request for
    * the given variable from the hash map.  If the operator is found, a
    * pointer to it will be returned.  Otherwise, an unrecoverable error
    * will result and the program will abort.
    *
    * @param[in]  grid_geometry The grid geometry with which the operator
    *                           being looked up is associated.
    * @param[in]  var The Variable for which the corresponding coarsening
    *                 operator should match.
    * @param[in]  op_name The string identifier of the coarsening operator.
    */
   boost::shared_ptr<CoarsenOperator>
   lookupCoarsenOperator(
      BaseGridGeometry& grid_geometry,
      const boost::shared_ptr<Variable>& var,
      const std::string& op_name);

   /*!
    * @brief Lookup function for refinement operator.
    *
    * Extract the spatial refinement operator matching the request for
    * the given variable from the hash map.  If the operator is found, a
    * pointer to it will be returned.  Otherwise, an unrecoverable
    * error will result and the program will abort.
    *
    * @param[in]  grid_geometry The grid geometry with which the operator
    *                           being looked up is associated.
    * @param[in]  var The Variable for which the corresponding refinement
    *                 operator should match.
    * @param[in]  op_name The string identifier of the refinement operator.
    */
   boost::shared_ptr<RefineOperator>
   lookupRefineOperator(
      BaseGridGeometry& grid_geometry,
      const boost::shared_ptr<Variable>& var,
      const std::string& op_name);

   /*!
    * @brief Lookup function for time interpolation operator.
    *
    * Extract the time interpolation operator matching the request for
    * the given variable from the hash map.  If the operator is found, a
    * pointer to it will be returned.  Otherwise, an unrecoverable
    * error will result and the program will abort.
    *
    * @param[in]  grid_geometry The grid geometry with which the operator
    *                           being looked up is associated.
    * @param[in]  var The Variable for which the corresponding time
    *                 interpolation operator should match.
    * @param[in]  op_name The string identifier of the time interpolation
    *                     operator.  \b Default: STD_LINEAR_TIME_INTERPOLATE
    */
   boost::shared_ptr<TimeInterpolateOperator>
   lookupTimeInterpolateOperator(
      BaseGridGeometry& grid_geometry,
      const boost::shared_ptr<Variable>& var,
      const std::string& op_name =
         "STD_LINEAR_TIME_INTERPOLATE");

   /*!
    * @brief Get the max stencil width of all transfer operators.
    *
    * The max stencil width computed from all registered (constructed)
    * transfer operators.  This function is simply returns the max
    * from the registered refine, coarsen and time-refine operators.
    *
    * @return The max stencil width computed from all registered
    * operators.
    *
    * @see hier::BaseGridGeometry::getMaxTransferOpStencilWidth().
    * @see hier::RefineOperator::getMaxRefineOpStencilWidth().
    * @see hier::CoarsenOperator::getMaxCoarsenOpStencilWidth().
    */
   IntVector
   getMaxTransferOpStencilWidth();

   /*!
    * @brief Set a minimum value on the value returned by
    * getMaxTransferOpStencilWidth().
    *
    * This method allows users to specify a minimum value returned by
    * getMaxTransferOpStencilWidth().  The default minimum is zero.
    * This value can be used as a substitute for data that is not yet
    * registered with the Geometry and therefore cannot be reflected
    * in getMaxTransferOpStencilWidth().
    *
    * @param[in]  min_value The minimum value to set.
    */
   void
   setMinTransferOpStencilWidth(
      const IntVector& min_value)
   {
      TBOX_ASSERT_OBJDIM_EQUALITY2(d_min_stencil_width, min_value);
      d_min_stencil_width = min_value;
   }

   /*!
    * @brief Print class data representation.
    *
    * @param[in]  os The std::ostream to print to.
    */
   void
   printClassData(
      std::ostream& os) const;

private:
   /*
    * The hash maps of spatial coarsening operators is maintained to lookup
    * operators for specific variables as requested by algorithms and/or
    * applications using the hier::BaseGridGeometry holding this object.
    * Standard concrete coarsening operators can be found in the patchdata
    * package.  Additional operators may be added to this hash map at any time
    * (see addCoarsenOperator() function).
    */
   boost::unordered_map<std::string, boost::unordered_map<std::string,
      boost::shared_ptr<CoarsenOperator> > > d_coarsen_operators;

   /*
    * The hash map of spatial refinement operators is maintained to lookup
    * operators for specific variables as requested by algorithms and/or
    * applications using the hier::BaseGridGeometry holding this object.
    * Standard concrete refinement operators can be found in the patchdata
    * package.  Additional operators may be added to this hash map at any time
    * (see addRefineOperator() function).
    */
   boost::unordered_map<std::string, boost::unordered_map<std::string,
      boost::shared_ptr<RefineOperator> > > d_refine_operators;

   /*
    * The hash map of time interpolation operators is maintained to lookup
    * operators for specific variables as requested by algorithms and/or
    * applications using the hier::BaseGridGeometry holding this object.
    * Standard concrete time interpolation operators can be found in the
    * patchdata package.  Additional operators may be added to this hash map at
    * any time (see addTimeInterpolateOperator() function).
    */
   boost::unordered_map<std::string, boost::unordered_map<std::string,
      boost::shared_ptr<TimeInterpolateOperator> > > d_time_operators;

   /*!
    * @brief Value set by setMinTransferOpStencilWidth().
    */
   IntVector d_min_stencil_width;

   /*!
    * @brief true if a call to getMaxTransferOpStencilWidth has been made.
    */
   bool d_max_op_stencil_width_req;

};

}
}

#endif
