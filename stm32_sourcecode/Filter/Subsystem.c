/*
 * File: Subsystem.c
 *
 * Code generated for Simulink model 'Subsystem'.
 *
 * Model version                  : 1.16
 * Simulink Coder version         : 9.1 (R2019a) 23-Nov-2018
 * C/C++ source code generated on : Sat Oct  3 22:40:52 2020
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "Subsystem.h"
#ifndef UCHAR_MAX
#include <limits.h>
#endif

#if ( UCHAR_MAX != (0xFFU) ) || ( SCHAR_MAX != (0x7F) )
#error Code was generated for compiler with different sized uchar/char. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( USHRT_MAX != (0xFFFFU) ) || ( SHRT_MAX != (0x7FFF) )
#error Code was generated for compiler with different sized ushort/short. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( UINT_MAX != (0xFFFFFFFFU) ) || ( INT_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized uint/int. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( ULONG_MAX != (0xFFFFFFFFU) ) || ( LONG_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized ulong/long. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

/* Skipping ulong_long/long_long check: insufficient preprocessor integer range. */

/* Block signals and states (default storage) */
DW rtDW;

/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Real-time model */
RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;

/* Model step function */
void Subsystem_step(void)
{
  int64_T acc1;
  int32_T j;

  /* DiscreteFir: '<S1>/LOWPASS Filter' incorporates:
   *  Inport: '<Root>/In1'
   */
  /* Consume delay line and beginning of input samples */
  acc1 = (uint64_T)rtU.In1 * rtConstP.LOWPASSFilter_Coefficients[0];
  for (j = 0; j < 26; j++) {
    acc1 += (uint64_T)rtConstP.LOWPASSFilter_Coefficients[1 + j] *
      rtDW.LOWPASSFilter_states[j];
  }

  /* Update delay line for next frame */
  for (j = 24; j >= 0; j--) {
    rtDW.LOWPASSFilter_states[1 + j] = rtDW.LOWPASSFilter_states[j];
  }

  rtDW.LOWPASSFilter_states[0] = rtU.In1;

  /* Outport: '<Root>/Out1' incorporates:
   *  DiscreteFir: '<S1>/LOWPASS Filter'
   */
  rtY.Out1 = acc1>>32;
}

/* Model initialize function */
void Subsystem_initialize(void)
{
  /* Registration code */

  /* external outputs */
  rtY.Out1 = (0ULL);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
