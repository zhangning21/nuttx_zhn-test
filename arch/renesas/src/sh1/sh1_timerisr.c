/****************************************************************************
 * arch/renesas/src/sh1/sh1_timerisr.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <time.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <arch/board/board.h>

#include "clock/clock.h"
#include "renesas_internal.h"
#include "chip.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The desired timer interrupt frequency is provided by the definition
 * CLK_TCK (see include/time.h).  CLK_TCK defines the desired number of
 * system clock ticks per second.  That value is a user configurable setting
 * that defaults to 100 (100 ticks per second = 10 MS interval).
 *
 * ITU1 operates in periodic timer mode.  TCNT counts up until it matches
 * the value of GRA0, then an interrupt is generated.  Two values must be
 * computed:
 *
 * (1) The divider that determines the rate at which TCNT increments, and
 * (2) The value of GRA0 that cause the interrupt to occur.
 *
 * These must be selected so that the frequency of interrupt generation is
 * CLK_TCK.  Ideally, we would like to use the full range of GRA0 for better
 * timing accuracy:
 */

#define DESIRED_GRA0     65535

/* The ideal divider would be one that generates exactly 65535 ticks in
 * 1/CLK_TCK seconds.  For example, if SH1_CLOCK is 10MHz and CLK_TCK is
 * 100, then the ideal divider must be less greater than or equal to:
 *
 *   (10,000,000 / CLK_TCK) / 65535 = 1.525
 *
 * The actual selected divider would then have to be 2, resulting in a
 * counting rate of 5,000,0000 and a GRA0 setting of 50,000.
 */

#define SYSCLKS_PER_TICK ((SH1_CLOCK + (CLK_TCK-1))/ CLK_TCK)
#define DESIRED_DIVIDER  ((SYSCLKS_PER_TICK + (DESIRED_GRA0-1))/ DESIRED_GRA0)

#if (DESIRED_DIVIDER <= 1)
#  define SYSCLK_DIVIDER 1
#  define SH1_ITUTCR_DIV SH1_ITUTCR_DIV1
#elif (DESIRED_DIVIDER <= 2)
#  define SYSCLK_DIVIDER 2
#  define SH1_ITUTCR_DIV SH1_ITUTCR_DIV2
#elif (DESIRED_DIVIDER <= 4)
#  define SYSCLK_DIVIDER 4
#  define SH1_ITUTCR_DIV SH1_ITUTCR_DIV4
#elif (DESIRED_DIVIDER <= 8)
#  define SYSCLK_DIVIDER 8
#  define SH1_ITUTCR_DIV SH1_ITUTCR_DIV8
#else
#  error "No divider is available for this system clock"
#endif

/* The TCNT will then increment at the following rate: */

#define TCNT_CLOCK       ((SH1_CLOCK + (SYSCLK_DIVIDER/2))/ SYSCLK_DIVIDER)

/* And the value of GRA0 that generates at CLK_TCK ticks per second is: */

#define TCNT_PER_TICK    ((TCNT_CLOCK + (CLK_TCK-1))/ CLK_TCK)

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  sh1_timerisr
 *
 * Description:
 *   The timer ISR will perform a variety of services for various portions
 *   of the systems.
 *
 ****************************************************************************/

static int sh1_timerisr(int irq, uint32_t *regs, void *arg)
{
  uint8_t reg8;

  /* Process timer interrupt */

  nxsched_process_timer();

  /* Clear ITU0 interrupt status flag */

  reg8 = getreg8(SH1_ITU0_TSR);
  reg8 &= ~SH1_ITUTSR_IMFA;
  putreg8(reg8, SH1_ITU0_TSR);

  return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  up_timer_initialize
 *
 * Description:
 *   This function is called during start-up to initialize
 *   the timer interrupt.
 *
 ****************************************************************************/

void up_timer_initialize(void)
{
  uint8_t reg8;

  /* Clear timer counter 0 */

  putreg16(0, SH1_ITU0_TCNT);

  /* Set the GRA0 match value.  The interrupt will be generated when TNCT
   * increments to this value
   */

  putreg16(TCNT_PER_TICK, SH1_ITU0_GRA);

  /* Set the timer control register.  TCNT cleared by FRA */

  putreg8(SH1_ITUTCR_CGRA | SH1_ITUTCR_DIV, SH1_ITU0_TCR);

  /* Set the timer I/O control register */

  putreg8(0, SH1_ITU0_TIOR); /* GRA used but with no inputs/output pins */

  /* Make sure that all status flags are clear */

  putreg8(0, SH1_ITU0_TSR);

  /* Attach the IMIA0 IRQ */

  irq_attach(SH1_SYSTIMER_IRQ, (xcpt_t)sh1_timerisr, NULL);

  /* Enable interrupts on GRA compare match */

  putreg8(SH1_ITUTIER_IMIEA, SH1_ITU0_TIER);

#ifdef CONFIG_ARCH_IRQPRIO
  /* Set the interrupt priority */

  up_prioritize_irq(SH1_SYSTIMER_IRQ, 7);  /* Set ITU priority midway */
#endif

  /* Start the timer */

  reg8  = getreg8(SH1_ITU_TSTR);
  reg8 |= SH1_ITUTSTR_STR0;     /* Enable TCNT0 */
  putreg8(reg8, SH1_ITU_TSTR);  /* TCNT0 is counting */
}
