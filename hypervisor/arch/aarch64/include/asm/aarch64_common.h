#ifndef _MINOS_AARCH64_COMMON_H_
#define _MINOS_AARCH64_COMMON_H_

/**
 * SPSR_ELn Saved Program Status Register
 * Holds the saved processor state when an exception is taken to this mode or
 * Exception level
 * • M[3:2] holds the Exception Level.
 * • M[1] is unused and is RES 0 for all non-reserved values.
 * • M[0] is used to select the SP:
 * — 0 means the SP is always SP0.
 * — 1 means the exception SP is determined by the EL.
 */
#define AARCH64_SPSR_EL3h	0b1101
#define AARCH64_SPSR_EL3t	0b1100
#define AARCH64_SPSR_EL2h	0b1001
#define AARCH64_SPSR_EL2t	0b1000
#define AARCH64_SPSR_EL1h	0b0101
#define AARCH64_SPSR_EL1t	0b0100
#define AARCH64_SPSR_EL0t	0b0000
/* Execution state that the exception was taken from. */
#define AARCH64_SPSR_RW		(1 << 4)
/* FIQ mask bit. */
#define AARCH64_SPSR_F		(1 << 6)
/* IRQ mask bit. */
#define AARCH64_SPSR_I		(1 << 7)
/* SError interrupt mask bit. */
#define AARCH64_SPSR_A		(1 << 8)
/* Process state D mask. */
#define AARCH64_SPSR_D		(1 << 9)
/* Illegal Execution state bit. Shows the value of PSTATE.IL immediately before
the exception was taken */
#define AARCH64_SPSR_IL		(1 << 20)
/* Software step. Shows the value of PSTATE.SS immediately before the exception
 * was taken. */
#define AARCH64_SPSR_SS		(1 << 21)
/* V condition flag */
#define AARCH64_SPSR_V		(1 << 28)
/* C condition flag */
#define AARCH64_SPSR_C		(1 << 29)
/* Z condition flag */
#define AARCH64_SPSR_Z		(1 << 30)
/* N condition flag */
#define AARCH64_SPSR_N		(1 << 31)

/* AArch32 mode that an exception was taken from */
#define AARCH32_USER		0b0000
#define AARCH32_FIQ		0b0001
#define AARCH32_IRQ		0b0010
#define AARCH32_SVC		0b0011
#define AARCH32_MON		0b0110
#define AARCH32_ABT		0b0111
#define AARCH32_HYP		0b1010
#define AARCH32_UND		0b1011
#define AARCH32_SYSTEM		0b1111

#define MODE_EL3		(0x3UL)
#define MODE_EL2		(0x2UL)
#define MODE_EL1		(0x1UL)
#define MODE_EL0		(0x0UL)
#define MODE_EL_SHIFT		(0x2UL)
#define MODE_EL_MASK		(0x3UL)
#define GET_EL(mode)		(((mode) >> MODE_EL_SHIFT) & MODE_EL_MASK)

/**
 * MPIDR_EL1, Multiprocessor Affinity Register
 * In a multiprocessor system, provides an additional PE identification
 * mechanism for scheduling purposes
 */
#define MPIDR_EL1_AFF3_LSB	32
#define MPIDR_EL1_U		(1 << 30)
#define MPIDR_EL1_MT		(1 << 24)
#define MPIDR_EL1_AFF2_LSB	16
#define MPIDR_EL1_AFF1_LSB	8
#define MPIDR_EL1_AFF0_LSB	0
#define MPIDR_EL1_AFF_WIDTH	8
#define MIPIDR_AFF_SHIFT	2

/**
 * DCZID_ELn Data Cache Zero ID Register
 * Indicates the block size that is written with byte values of 0 by the Data
 * Cache Zero by virtual address (DCZVA) system instruction.
 */
#define DCZID_EL0_BS_LSB	0
#define DCZID_EL0_BS_WIDTH	4
#define DCZID_EL0_DZP_LSB	5
#define DCZID_EL0_DZP		(1 << 5)

/**
 * SCTLR_ELn System Control Register
 * Controls architectural features, for example the MMU, caches and alignment
 * checking.
 */
#define SCTLR_EL1_UCI		(1 << 26)
#define SCTLR_ELx_EE		(1 << 25)
#define SCTLR_EL1_E0E		(1 << 24)
#define SCTLR_ELx_WXN		(1 << 19)
#define SCTLR_EL1_nTWE		(1 << 18)
#define SCTLR_EL1_nTWI		(1 << 16)
#define SCTLR_EL1_UCT		(1 << 15)
#define SCTLR_EL1_DZE		(1 << 14)
#define SCTLR_ELx_I		(1 << 12)
#define SCTLR_EL1_UMA		(1 << 9)
#define SCTLR_EL1_SED		(1 << 8)
#define SCTLR_EL1_ITD		(1 << 7)
#define SCTLR_EL1_THEE		(1 << 6)
#define SCTLR_EL1_CP15BEN	(1 << 5)
#define SCTLR_EL1_SA0		(1 << 4)
#define SCTLR_ELx_SA		(1 << 3)
#define SCTLR_ELx_C		(1 << 2)
#define SCTLR_ELx_A		(1 << 1)
#define SCTLR_ELx_M		(1 << 0)

#define SCTLR_ELx_C_BIT		(2)
#define SCTLR_ELx_A_BIT		(1)
#define SCTLR_ELx_M_BIT		(0)

/**
 * CPACR_ELn Coprocessor Access Control Register
 * Controls access to trace, floating-point, and SIMD functionality.
 */
#define CPACR_EL1_TTA		(1 << 28)
#define CPACR_EL1_FPEN		(3 << 20)

/**
 * Architectural Feature Trap Register
 */
#define CPTR_ELx_TCPAC		(1 << 31)
#define CPTR_ELx_TTA		(1 << 20)
#define CPTR_ELx_TFP		(1 << 10)

/**
 * SCR_ELn Secure Configuration Register
 * Controls Secure state and trapping of exceptions to EL3.
 */
#define SCR_EL3_TWE		(1 << 13)
#define SCR_EL3_TWI		(1 << 12)
#define SCR_EL3_ST		(1 << 11)
#define SCR_EL3_RW		(1 << 10)
#define SCR_EL3_SIF		(1 << 9)
#define SCR_EL3_HCE		(1 << 8)
#define SCR_EL3_SMD		(1 << 7)
#define SCR_EL3_EA		(1 << 3)
#define SCR_EL3_FIQ		(1 << 2)
#define SCR_EL3_IRQ		(1 << 1)
#define SCR_EL3_NS		(1 << 0)

/**
 * HCR_ELn Hypervisor Configuration Register
 * Controls virtualization settings and trapping of exceptions to EL2.
 */
#define HCR_EL2_VM		(1ul << 0)
#define HCR_EL2_SWIO		(1ul << 1)
#define HCR_EL2_PTW		(1ul << 2)
#define HCR_EL2_FMO		(1ul << 3)
#define HCR_EL2_IMO		(1ul << 4)
#define HCR_EL2_AMO		(1ul << 5)
#define HCR_EL2_VF		(1ul << 6)
#define HCR_EL2_VI		(1ul << 7)
#define HCR_EL2_VSE		(1ul << 8)
#define HCR_EL2_FB		(1ul << 9)
#define HCR_EL2_BSU_IS		(1ul << 10)
#define HCR_EL2_BSU_OS		(2ul << 10)
#define HCR_EL2_BSU_FS		(3ul << 10)
#define HCR_EL2_DC		(1ul << 12)
#define HCR_EL2_TWI		(1ul << 13)
#define HCR_EL2_TWE		(1ul << 14)
#define HCR_EL2_TID0		(1ul << 15)
#define HCR_EL2_TID1		(1ul << 16)
#define HCR_EL2_TID2		(1ul << 17)
#define HCR_EL2_TID3		(1ul << 18)
#define HCR_EL2_TSC		(1ul << 19)
#define HCR_EL2_TIDCP		(1ul << 20)
#define HCR_EL2_TACR		(1ul << 21)
#define HCR_EL2_TSW		(1ul << 22)
#define HCR_EL2_TPC		(1ul << 23)
#define HCR_EL2_TPU		(1ul << 24)
#define HCR_EL2_TTLB		(1ul << 25)
#define HCR_EL2_TVM		(1ul << 26)
#define HCR_EL2_TGE		(1ul << 27)
#define HCR_EL2_TDZ		(1ul << 28)
#define HCR_EL2_HVC		(1ul << 29)
#define HCR_EL2_TRVM		(1ul << 30)
#define HCR_EL2_RW		(1ul << 31)
#define HCR_EL2_CD		(1ul << 32)
#define HCR_EL2_ID		(1ul << 33)

#define LOUIS_SHIFT		(21)
#define LOC_SHIFT		(24)
#define CLIDR_FIELD_WIDTH	(3)

#define LEVEL_SHIFT		(1)

#endif
