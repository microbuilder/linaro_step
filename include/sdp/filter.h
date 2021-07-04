/*
 * Copyright (c) 2021 Linaro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SDP_FILTER_H__
#define SDP_FILTER_H__

#include <sdp/sdp.h>
#include <sdp/measurement/measurement.h>

/**
 * @defgroup FILTER Filter Definitions
 * @ingroup sdp_api
 * @brief API header file for the SDP filter engine.
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Logical operand used between the current and
 *        previous filter values in a filter chain.
 *
 * @note The first value in a filter chain MUST be either SDP_FILTER_OP_IS
 *       of SDP_FILTER_OP_NOT.
 */
enum sdp_filter_op {
	/**
	 * @brief Filter evaluation must be logically true. Solely for use as
	 *        the first operand in a filter chain.
	 *
	 * @note This is functionally identical to SDP_FILTER_AND_IS, with the
	 *       assumption that the previous value is true.
	 *
	 * @note The first value in a filter chain MUST be either SDP_FILTER_OP_IS
	 *       of SDP_FILTER_OP_NOT.
	 */
	SDP_FILTER_OP_IS        = 0,

	/**
	 * @brief Filter evaluation must be logically false. Solely for use as
	 *        the first operand in a filter chain.
	 *
	 * @note This is functionally identical to SDP_FILTER_AND_IS, with the
	 *       assumption that the previous value is true.
	 *
	 * @note The first value in a filter chain MUST be either SDP_FILTER_OP_IS
	 *       of SDP_FILTER_OP_NOT.
	 */
	SDP_FILTER_OP_NOT       = 1,

	/**
	 * @brief Previous operand AND current operand must resolve to being true,
	 *        where the current filter evaluation is logically true. Solely for
	 *        use in non-initial entries in a filter chain.
	 */
	SDP_FILTER_OP_AND       = 2,

	/**
	 * @brief Previous operand AND current operand must resolve to being true,
	 *        where the current filter evaluation is logically false. Solely for
	 *        use in non-initial entries in a filter chain.
	 */
	SDP_FILTER_OP_AND_NOT   = 3,

	/**
	 * @brief Previous operand OR current operand must resolve to being true,
	 *        where the current filter evaluation is logically true. Solely for
	 *        use in non-initial entries in a filter chain.
	 */
	SDP_FILTER_OP_OR        = 4,

	/**
	 * @brief Previous operand OR current operand must resolve to being true,
	 *        where the current filter evaluation is logically false. Solely for
	 *        use in non-initial entries in a filter chain.
	 */
	SDP_FILTER_OP_OR_NOT    = 5,

	/**
	 * @brief Exactly one of the previous operand OR current operand must
	 *        resolve to being true, where the current filter evaluation is
	 *        logically true. Solely for use in non-initial entries in a filter
	 *        chain.
	 */
	SDP_FILTER_OP_XOR       = 6,
};

/**
 * @brief An individual filter entry.
 */
struct sdp_filter {
	/**
	 * @brief The operand to apply between the current and previous sdp_filters.
	 */
	enum sdp_filter_op op;

	/**
	 * @brief The measurement's filter value must exactly match this value,
	 *        taking into account any bits excluded via ignore_mask.
	 */
	uint32_t match;

	/**
	 * @brief Any bits set to 1 in this mask field will be ignored when
	 *        determining if an exact match was found.
	 *
	 * @note This can be used to perfom and exact match only on the base and/or
	 *       extended data type fields, for example.
	 */
	uint32_t ignore_mask;
};

/**
 * @brief A filter chain.
 *
 * Set 'count' to 0 or 'chain' to NULL to indicate that this is a catch-all
 * filter that should match on any valid incoming measurement.
 *
 * @note Entries in a filter chain are evaluated in a strictly linear
 *       left-to-right (or top-to-bottom) manner, where the sum of the
 *       previous operands is evaluated against the current filter entry.
 *       There is currently no mechanism to override the order of evaluation
 *       via parentheses or operator precedence.
 */
struct sdp_filter_chain {
	/**
	 * @brief The number of filters supplied in 'chain'.
	 *
	 * If this value is 0, it will be interpretted as a catch-all indicator,
	 * matching on all measurements.
	 */
	uint32_t count;

	/**
	 * @brief Pointer to an array of 'count' len of individual filters.
	 *
	 * If this value is NULL, it will be interpretted as a catch-all indicator,
	 * matching on all measurements.
	 */
	struct sdp_filter *chain;
};

/**
 * @brief Prints the supplied filter chain using printk.
 *
 * @param fc The sdsp_filter_chain to print.
 */
void sdp_filt_print(struct sdp_filter_chain *fc);

/**
 * @brief Evaluates the supplied sdp_measurement against the sdp_filter_chain
 *        to determine if there is a match.
 *
 * @param fc		The sdp_filter_chain to evalute for a match.
 * @param mes 		The sdp_measurement to evaluate a match against.
 * @param match 	1 if the node's filter chain matches, otherwise 0.
 *
 * @return int 		Zero on normal execution, otherwise a negative error code.
 */
int sdp_filt_evaluate(struct sdp_filter_chain *fc, struct sdp_measurement *mes,
		      int *match);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* SDP_FILTER_H_ */
