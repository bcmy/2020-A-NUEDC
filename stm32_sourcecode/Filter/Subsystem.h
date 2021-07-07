/*
 * File: Subsystem.h
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

#ifndef RTW_HEADER_Subsystem_h_
#define RTW_HEADER_Subsystem_h_
#ifndef Subsystem_COMMON_INCLUDES_
# define Subsystem_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* Subsystem_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  uint32_T LOWPASSFilter_states[26];   /* '<S1>/LOWPASS Filter' */
} DW;

/* Constant parameters (default storage) */
typedef struct {
  /* Computed Parameter: LOWPASSFilter_Coefficients
   * Referenced by: '<S1>/LOWPASS Filter'
   */
  uint32_T LOWPASSFilter_Coefficients[27];
} ConstP;

/* External inputs (root inport signals with default storage) */
typedef struct {
  uint32_T In1;                        /* '<Root>/In1' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  uint64_T Out1;                       /* '<Root>/Out1' */
} ExtY;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T * volatile errorStatus;
};

/* Block signals and states (default storage) */
extern DW rtDW;

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY rtY;

/* Constant parameters (default storage) */
extern const ConstP rtConstP;

/* Model entry point functions */
extern void Subsystem_initialize(void);
extern void Subsystem_step(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('simu2016a/Subsystem')    - opens subsystem simu2016a/Subsystem
 * hilite_system('simu2016a/Subsystem/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'simu2016a'
 * '<S1>'   : 'simu2016a/Subsystem'
 */
#endif                                 /* RTW_HEADER_Subsystem_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
