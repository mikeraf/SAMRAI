/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and LICENSE.
 *
 * Copyright:     (c) 1997-2020 Lawrence Livermore National Security, LLC
 * Description:   hier
 *
 ************************************************************************/

#ifndef included_pdat_EdgeOverlap
#define included_pdat_EdgeOverlap

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/hier/Box.h"
#include "SAMRAI/hier/BoxContainer.h"
#include "SAMRAI/hier/BoxOverlap.h"
#include "SAMRAI/hier/IntVector.h"

#include <memory>

namespace SAMRAI {
namespace pdat {

/**
 * Class EdgeOverlap represents the intersection between two edge
 * centered geometry boxes.  It is a subclass of hier::BoxOverlap and records
 * the portions of index space that needs to be copied between two objects
 * with edge centered geometry.
 *
 * @see hier::BoxOverlap
 * @see EdgeOverlap
 */

class EdgeOverlap:public hier::BoxOverlap
{
public:
   /**
    * The constructor takes the list of boxes and the transformation from
    * source to destination index spaces.  This information is used later
    * in the generation of communication schedules.
    */
   EdgeOverlap(
      const std::vector<hier::BoxContainer>& boxes,
      const hier::Transformation& transformation);

   /**
    * The virtual destructor does nothing interesting except deallocate
    * box data.
    */
   virtual ~EdgeOverlap();

   /**
    * Return whether there is an empty intersection between the two
    * edge centered boxes.  This method over-rides the virtual function
    * in the hier::BoxOverlap base class.
    */
   virtual bool
   isOverlapEmpty() const;

   /**
    * Return the list of boxes (in edge centered index space) that
    * constitute the intersection.  The boxes are given in the
    * destination coordinate space and must be shifted by
    * -(getSourceOffset()) to lie in the source index space.  The axis
    * argument represents which axis is desired: X=0, Y=1, and Z=2.
    *
    * @pre (axis >= 0) && (axis < d_dst_boxes.size())
    */
   virtual const hier::BoxContainer&
   getDestinationBoxContainer(
      const int axis) const;

   /*!
    * @brief Get a BoxContainer representing the source boxes of the overlap.
    *
    * The src_boxes container will be filled with edge-centered source
    * boxes of the overlap in the source coordinate space.  The given
    * axis direction is the axis in destination space on input and
    * in source space on output.
    *
    * @param[out] src_boxes
    * @param[in,out] axis_direction
    *
    * @pre src_boxes.empty()
    * @pre axis_direction >= 0 && axis_direction < d_dst_boxes.size()
    * @post axis_direction >= 0 && axis_direction < d_dst_boxes.size()
    */
   virtual void
   getSourceBoxContainer(
      hier::BoxContainer& src_boxes,
      int& axis_direction) const;

   /**
    * Return the offset between the destination and source index spaces.
    * The destination index space is the source index space shifted
    * by this amount.
    */
   virtual const hier::IntVector&
   getSourceOffset() const;

   virtual const hier::Transformation&
   getTransformation() const;

private:
   bool d_is_overlap_empty;
   hier::Transformation d_transformation;
   std::vector<hier::BoxContainer> d_dst_boxes;
};

}
}
#endif
