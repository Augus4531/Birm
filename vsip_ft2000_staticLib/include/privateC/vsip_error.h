#ifndef _vsip_error_h
#define _vsip_error_h 1
/*
 * vsip_public_error.h
 *
 * This file contains public VSIP error type definitions.
*/

#ifdef VSIP_DEVELOPMENT_MODE
/* public enumerated errors						*/
typedef enum {
		vsip_invalid_structure_object   = 1 << 0,
		vsip_bad_public_array_pointer	= 1 << 1,
		vsip_bad_private_array_storage	= 1 << 2,
		vsip_non_zero_binding_counter	= 1 << 3,
		vsip_view_outside_data_array	= 1 << 4,
		vsip_view_index_exceeds_length	= 1 << 5,
		vsip_nonconformal_view_lengths	= 1 << 6,
		vsip_clobbered_source_operand	= 1 << 7
  } vsip_error;

vsip_error vsip_set_error_mask(vsip_error mask);
void vsip_put_error_mask(vsip_error mask);
int vsip_check_error(vsip_error mask);

#define VSIP_SET_ERROR_MASK(t, m) vsip_error t = vsip_set_error_mask((vsip_error)(m));
#define VSIP_PUT_ERROR_MASK(m) vsip_put_error_mask((vsip_error)(m));
#else
#define VSIP_SET_ERROR_MASK(t, m)
#define VSIP_PUT_ERROR_MASK(m)
#endif /* VSIP_DEVELOPMENT_MODE */

#endif /* _vsip_public_error_h */
