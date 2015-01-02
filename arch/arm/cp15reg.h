#ifdef __ASM__
#define MIDR_get(reg)     mrc p15,0,reg,c0,c0,0
#define CTR_get(reg)     mrc p15,0,reg,c0,c0,1
#define TCMTR_get(reg)     mrc p15,0,reg,c0,c0,2
#define TLBTR_get(reg)     mrc p15,0,reg,c0,c0,3
#define MPIDR_get(reg)     mrc p15,0,reg,c0,c0,5
#define REVIDR_get(reg)     mrc p15,0,reg,c0,c0,6
#define ID_PFR0_get(reg)     mrc p15,0,reg,c0,c1,0
#define ID_PFR1_get(reg)     mrc p15,0,reg,c0,c1,1
#define ID_DFR0_get(reg)     mrc p15,0,reg,c0,c1,2
#define ID_AFR0_get(reg)     mrc p15,0,reg,c0,c1,3
#define ID_MMFR0_get(reg)     mrc p15,0,reg,c0,c1,4
#define ID_MMFR1_get(reg)     mrc p15,0,reg,c0,c1,5
#define ID_MMFR2_get(reg)     mrc p15,0,reg,c0,c1,6
#define ID_MMFR3_get(reg)     mrc p15,0,reg,c0,c1,7
#define ID_ISAR0_get(reg)     mrc p15,0,reg,c0,c2,0
#define ID_ISAR1_get(reg)     mrc p15,0,reg,c0,c2,1
#define ID_ISAR2_get(reg)     mrc p15,0,reg,c0,c2,2
#define ID_ISAR3_get(reg)     mrc p15,0,reg,c0,c2,3
#define ID_ISAR4_get(reg)     mrc p15,0,reg,c0,c2,4
#define ID_ISAR5_get(reg)     mrc p15,0,reg,c0,c2,5
#define CCSIDR_get(reg)     mrc p15,1,reg,c0,c0,0
#define CLIDR_get(reg)     mrc p15,1,reg,c0,c0,1
#define AIDR_get(reg)     mrc p15,1,reg,c0,c0,7
#define CSSELR_get(reg)     mrc p15,2,reg,c0,c0,0
#define CSSELR_set(reg)     mcr p15,2,reg,c0,c0,0
#define VPIDR_get(reg)     mrc p15,4,reg,c0,c0,0
#define VPIDR_set(reg)     mcr p15,4,reg,c0,c0,0
#define VMPIDR_get(reg)     mrc p15,4,reg,c0,c0,5
#define VMPIDR_set(reg)     mcr p15,4,reg,c0,c0,5
#define SCTLR_get(reg)     mrc p15,0,reg,c1,c0,0
#define SCTLR_set(reg)     mcr p15,0,reg,c1,c0,0
#define ACTLR_get(reg)     mrc p15,0,reg,c1,c0,1
#define ACTLR_set(reg)     mcr p15,0,reg,c1,c0,1
#define CPACR_get(reg)     mrc p15,0,reg,c1,c0,2
#define CPACR_set(reg)     mcr p15,0,reg,c1,c0,2
#define SCR_get(reg)     mrc p15,0,reg,c1,c1,0
#define SCR_set(reg)     mcr p15,0,reg,c1,c1,0
#define SDER_get(reg)     mrc p15,0,reg,c1,c1,1
#define SDER_set(reg)     mcr p15,0,reg,c1,c1,1
#define NSACR_get(reg)     mrc p15,0,reg,c1,c1,2
#define NSACR_set(reg)     mcr p15,0,reg,c1,c1,2
#define TTBR0_get(reg)     mrc p15,0,reg,c2,c0,0
#define TTBR0_set(reg)     mcr p15,0,reg,c2,c0,0
#define TTBR1_get(reg)     mrc p15,0,reg,c2,c0,1
#define TTBR1_set(reg)     mcr p15,0,reg,c2,c0,1
#define TTBCR_get(reg)     mrc p15,0,reg,c2,c0,2
#define TTBCR_set(reg)     mcr p15,0,reg,c2,c0,2
#define VTCR_get(reg)     mrc p15,0,reg,c2,c1,2
#define VTCR_set(reg)     mcr p15,0,reg,c2,c1,2
#define DACR_get(reg)     mrc p15,0,reg,c3,c0,0
#define DACR_set(reg)     mcr p15,0,reg,c3,c0,0
#define DFSR_get(reg)     mrc p15,0,reg,c5,c0,0
#define DFSR_set(reg)     mcr p15,0,reg,c5,c0,0
#define IFSR_get(reg)     mrc p15,0,reg,c5,c0,1
#define IFSR_set(reg)     mcr p15,0,reg,c5,c0,1
#define ADFSR_get(reg)     mrc p15,0,reg,c5,c1,0
#define ADFSR_set(reg)     mcr p15,0,reg,c5,c1,0
#define AIFSR_get(reg)     mrc p15,0,reg,c5,c1,1
#define AIFSR_set(reg)     mcr p15,0,reg,c5,c1,1
#define DFAR_get(reg)     mrc p15,0,reg,c6,c0,0
#define DFAR_set(reg)     mcr p15,0,reg,c6,c0,0
#define IFAR_get(reg)     mrc p15,0,reg,c6,c0,2
#define IFAR_set(reg)     mcr p15,0,reg,c6,c0,2
#define ICIALLUIS_set(reg)     mcr p15,0,reg,c7,c1,0
#define BPIALLIS_set(reg)     mcr p15,0,reg,c7,c1,6
#define PAR_get(reg)     mrc p15,0,reg,c7,c4,0
#define PAR_set(reg)     mcr p15,0,reg,c7,c4,0
#define ICIALLU_set(reg)     mcr p15,0,reg,c7,c5,0
#define ICIMVAU_set(reg)     mcr p15,0,reg,c7,c5,1
#define CP15ISB_set(reg)     mcr p15,0,reg,c7,c5,4
#define BPIALL_set(reg)     mcr p15,0,reg,c7,c5,6
#define BPIMVA_set(reg)     mcr p15,0,reg,c7,c5,7
#define DCIMVAC_set(reg)     mcr p15,0,reg,c7,c6,1
#define DCISW_set(reg)     mcr p15,0,reg,c7,c6,2
#define ATS1CPR_set(reg)     mcr p15,0,reg,c7,c8,0
#define ATS1CPW_set(reg)     mcr p15,0,reg,c7,c8,1
#define ATS1CUR_set(reg)     mcr p15,0,reg,c7,c8,2
#define ATS1CUW_set(reg)     mcr p15,0,reg,c7,c8,3
#define ATS12NSOPR_set(reg)     mcr p15,0,reg,c7,c8,4
#define ATS12NSOPW_set(reg)     mcr p15,0,reg,c7,c8,5
#define ATS12NSOUR_set(reg)     mcr p15,0,reg,c7,c8,6
#define ATS12NSOUW_set(reg)     mcr p15,0,reg,c7,c8,7
#define DCCMVAC_set(reg)     mcr p15,0,reg,c7,c10,1
#define DCCSW_set(reg)     mcr p15,0,reg,c7,c10,2
#define CP15DSB_set(reg)     mcr p15,0,reg,c7,c10,4
#define CP15DMB_set(reg)     mcr p15,0,reg,c7,c10,5
#define DCCMVAU_set(reg)     mcr p15,0,reg,c7,c11,1
#define DCCIMVAC_set(reg)     mcr p15,0,reg,c7,c14,1
#define DCCISW_set(reg)     mcr p15,0,reg,c7,c14,2
#define ATS1HR_set(reg)     mcr p15,4,reg,c7,c8,0
#define ATS1HW_set(reg)     mcr p15,4,reg,c7,c8,1
#define TLBIALLIS_set(reg)     mcr p15,0,reg,c8,c3,0
#define TLBIMVAIS_set(reg)     mcr p15,0,reg,c8,c3,1
#define TLBIASIDIS_set(reg)     mcr p15,0,reg,c8,c3,2
#define TLBIMVAAIS_set(reg)     mcr p15,0,reg,c8,c3,3
#define ITLBIALL_set(reg)     mcr p15,0,reg,c8,c5,0
#define ITLBIMVA_set(reg)     mcr p15,0,reg,c8,c5,1
#define ITLBIASID_set(reg)     mcr p15,0,reg,c8,c5,2
#define DTLBIALL_set(reg)     mcr p15,0,reg,c8,c6,0
#define DTLBIMVA_set(reg)     mcr p15,0,reg,c8,c6,1
#define DTLBIASID_set(reg)     mcr p15,0,reg,c8,c6,2
#define TLBIALL_set(reg)     mcr p15,0,reg,c8,c7,0
#define TLBIMVA_set(reg)     mcr p15,0,reg,c8,c7,1
#define TLBIASID_set(reg)     mcr p15,0,reg,c8,c7,2
#define TLBIMVAA_set(reg)     mcr p15,0,reg,c8,c7,3
#define TLBIALLHIS_set(reg)     mcr p15,4,reg,c8,c3,0
#define TLBIMVAHIS_set(reg)     mcr p15,4,reg,c8,c3,1
#define TLBIALLNSNHIS_set(reg)     mcr p15,4,reg,c8,c3,4
#define TLBIALLH_set(reg)     mcr p15,4,reg,c8,c7,0
#define TLBIMVAH_set(reg)     mcr p15,4,reg,c8,c7,1
#define TLBIALLNSNH_set(reg)     mcr p15,4,reg,c8,c7,4
#define PRRR_get(reg)     mrc p15,0,reg,c10,c2,0
#define PRRR_set(reg)     mcr p15,0,reg,c10,c2,0
#define MAIR0_get(reg)     mrc p15,0,reg,c10,c2,0
#define MAIR0_set(reg)     mcr p15,0,reg,c10,c2,0
#define NMRR_get(reg)     mrc p15,0,reg,c10,c2,1
#define NMRR_set(reg)     mcr p15,0,reg,c10,c2,1
#define MAIR1_get(reg)     mrc p15,0,reg,c10,c2,1
#define MAIR1_set(reg)     mcr p15,0,reg,c10,c2,1
#define AMAIR0_get(reg)     mrc p15,0,reg,c10,c3,0
#define AMAIR0_set(reg)     mcr p15,0,reg,c10,c3,0
#define AMAIR1_get(reg)     mrc p15,0,reg,c10,c3,1
#define AMAIR1_set(reg)     mcr p15,0,reg,c10,c3,1
#define VBAR_get(reg)     mrc p15,0,reg,c12,c0,0
#define VBAR_set(reg)     mcr p15,0,reg,c12,c0,0
#define MVBAR_get(reg)     mrc p15,0,reg,c12,c0,1
#define MVBAR_set(reg)     mcr p15,0,reg,c12,c0,1
#define ISR_get(reg)     mrc p15,0,reg,c12,c1,0
#define FCSEIDR_get(reg)     mrc p15,0,reg,c13,c0,0
#define FCSEIDR_set(reg)     mcr p15,0,reg,c13,c0,0
#define CONTEXTIDR_get(reg)     mrc p15,0,reg,c13,c0,1
#define CONTEXTIDR_set(reg)     mcr p15,0,reg,c13,c0,1
#define TPIDRURW_get(reg)     mrc p15,0,reg,c13,c0,2
#define TPIDRURW_set(reg)     mcr p15,0,reg,c13,c0,2
#define TPIDRURO_get(reg)     mrc p15,0,reg,c13,c0,3
#define TPIDRURO_set(reg)     mcr p15,0,reg,c13,c0,3
#define TPIDRPRW_get(reg)     mrc p15,0,reg,c13,c0,4
#define TPIDRPRW_set(reg)     mcr p15,0,reg,c13,c0,4
#else
#define MIDR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c0,0":"=r"(_reg_)::"memory");_reg_;})
#define CTR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c0,1":"=r"(_reg_)::"memory");_reg_;})
#define TCMTR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c0,2":"=r"(_reg_)::"memory");_reg_;})
#define TLBTR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c0,3":"=r"(_reg_)::"memory");_reg_;})
#define MPIDR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c0,5":"=r"(_reg_)::"memory");_reg_;})
#define REVIDR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c0,6":"=r"(_reg_)::"memory");_reg_;})
#define ID_PFR0_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c1,0":"=r"(_reg_)::"memory");_reg_;})
#define ID_PFR1_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c1,1":"=r"(_reg_)::"memory");_reg_;})
#define ID_DFR0_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c1,2":"=r"(_reg_)::"memory");_reg_;})
#define ID_AFR0_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c1,3":"=r"(_reg_)::"memory");_reg_;})
#define ID_MMFR0_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c1,4":"=r"(_reg_)::"memory");_reg_;})
#define ID_MMFR1_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c1,5":"=r"(_reg_)::"memory");_reg_;})
#define ID_MMFR2_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c1,6":"=r"(_reg_)::"memory");_reg_;})
#define ID_MMFR3_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c1,7":"=r"(_reg_)::"memory");_reg_;})
#define ID_ISAR0_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c2,0":"=r"(_reg_)::"memory");_reg_;})
#define ID_ISAR1_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c2,1":"=r"(_reg_)::"memory");_reg_;})
#define ID_ISAR2_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c2,2":"=r"(_reg_)::"memory");_reg_;})
#define ID_ISAR3_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c2,3":"=r"(_reg_)::"memory");_reg_;})
#define ID_ISAR4_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c2,4":"=r"(_reg_)::"memory");_reg_;})
#define ID_ISAR5_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c0,c2,5":"=r"(_reg_)::"memory");_reg_;})
#define CCSIDR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,1,%0,c0,c0,0":"=r"(_reg_)::"memory");_reg_;})
#define CLIDR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,1,%0,c0,c0,1":"=r"(_reg_)::"memory");_reg_;})
#define AIDR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,1,%0,c0,c0,7":"=r"(_reg_)::"memory");_reg_;})
#define CSSELR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,2,%0,c0,c0,0":"=r"(_reg_)::"memory");_reg_;})
#define CSSELR_set(reg)     do {__asm__ volatile ("mcr p15,2,%0,c0,c0,0"::"r"(reg):"memory");} while (0)
#define VPIDR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,4,%0,c0,c0,0":"=r"(_reg_)::"memory");_reg_;})
#define VPIDR_set(reg)     do {__asm__ volatile ("mcr p15,4,%0,c0,c0,0"::"r"(reg):"memory");} while (0)
#define VMPIDR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,4,%0,c0,c0,5":"=r"(_reg_)::"memory");_reg_;})
#define VMPIDR_set(reg)     do {__asm__ volatile ("mcr p15,4,%0,c0,c0,5"::"r"(reg):"memory");} while (0)
#define SCTLR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c1,c0,0":"=r"(_reg_)::"memory");_reg_;})
#define SCTLR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c1,c0,0"::"r"(reg):"memory");} while (0)
#define ACTLR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c1,c0,1":"=r"(_reg_)::"memory");_reg_;})
#define ACTLR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c1,c0,1"::"r"(reg):"memory");} while (0)
#define CPACR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c1,c0,2":"=r"(_reg_)::"memory");_reg_;})
#define CPACR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c1,c0,2"::"r"(reg):"memory");} while (0)
#define SCR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c1,c1,0":"=r"(_reg_)::"memory");_reg_;})
#define SCR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c1,c1,0"::"r"(reg):"memory");} while (0)
#define SDER_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c1,c1,1":"=r"(_reg_)::"memory");_reg_;})
#define SDER_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c1,c1,1"::"r"(reg):"memory");} while (0)
#define NSACR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c1,c1,2":"=r"(_reg_)::"memory");_reg_;})
#define NSACR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c1,c1,2"::"r"(reg):"memory");} while (0)
#define TTBR0_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c2,c0,0":"=r"(_reg_)::"memory");_reg_;})
#define TTBR0_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c2,c0,0"::"r"(reg):"memory");} while (0)
#define TTBR1_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c2,c0,1":"=r"(_reg_)::"memory");_reg_;})
#define TTBR1_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c2,c0,1"::"r"(reg):"memory");} while (0)
#define TTBCR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c2,c0,2":"=r"(_reg_)::"memory");_reg_;})
#define TTBCR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c2,c0,2"::"r"(reg):"memory");} while (0)
#define VTCR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c2,c1,2":"=r"(_reg_)::"memory");_reg_;})
#define VTCR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c2,c1,2"::"r"(reg):"memory");} while (0)
#define DACR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c3,c0,0":"=r"(_reg_)::"memory");_reg_;})
#define DACR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c3,c0,0"::"r"(reg):"memory");} while (0)
#define DFSR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c5,c0,0":"=r"(_reg_)::"memory");_reg_;})
#define DFSR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c5,c0,0"::"r"(reg):"memory");} while (0)
#define IFSR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c5,c0,1":"=r"(_reg_)::"memory");_reg_;})
#define IFSR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c5,c0,1"::"r"(reg):"memory");} while (0)
#define ADFSR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c5,c1,0":"=r"(_reg_)::"memory");_reg_;})
#define ADFSR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c5,c1,0"::"r"(reg):"memory");} while (0)
#define AIFSR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c5,c1,1":"=r"(_reg_)::"memory");_reg_;})
#define AIFSR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c5,c1,1"::"r"(reg):"memory");} while (0)
#define DFAR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c6,c0,0":"=r"(_reg_)::"memory");_reg_;})
#define DFAR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c6,c0,0"::"r"(reg):"memory");} while (0)
#define IFAR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c6,c0,2":"=r"(_reg_)::"memory");_reg_;})
#define IFAR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c6,c0,2"::"r"(reg):"memory");} while (0)
#define ICIALLUIS_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c1,0"::"r"(reg):"memory");} while (0)
#define BPIALLIS_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c1,6"::"r"(reg):"memory");} while (0)
#define PAR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c7,c4,0":"=r"(_reg_)::"memory");_reg_;})
#define PAR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c4,0"::"r"(reg):"memory");} while(0)
#define ICIALLU_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c5,0"::"r"(reg):"memory");} while(0)
#define ICIMVAU_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c5,1"::"r"(reg):"memory");} while(0)
#define CP15ISB_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c5,4"::"r"(reg):"memory");} while(0)
#define BPIALL_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c5,6"::"r"(reg):"memory");} while(0)
#define BPIMVA_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c5,7"::"r"(reg):"memory");} while(0)
#define DCIMVAC_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c6,1"::"r"(reg):"memory");} while(0)
#define DCISW_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c6,2"::"r"(reg):"memory");} while(0)
#define ATS1CPR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c8,0"::"r"(reg):"memory");} while(0)
#define ATS1CPW_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c8,1"::"r"(reg):"memory");} while(0)
#define ATS1CUR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c8,2"::"r"(reg):"memory");} while(0)
#define ATS1CUW_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c8,3"::"r"(reg):"memory");} while(0)
#define ATS12NSOPR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c8,4"::"r"(reg):"memory");} while(0)
#define ATS12NSOPW_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c8,5"::"r"(reg):"memory");} while(0)
#define ATS12NSOUR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c8,6"::"r"(reg):"memory");} while(0)
#define ATS12NSOUW_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c8,7"::"r"(reg):"memory");} while(0)
#define DCCMVAC_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c10,1"::"r"(reg):"memory");} while(0)
#define DCCSW_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c10,2"::"r"(reg):"memory");} while(0)
#define CP15DSB_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c10,4"::"r"(reg):"memory");} while(0)
#define CP15DMB_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c10,5"::"r"(reg):"memory");} while(0)
#define DCCMVAU_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c11,1"::"r"(reg):"memory");} while(0)
#define DCCIMVAC_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c14,1"::"r"(reg):"memory");} while(0)
#define DCCISW_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c7,c14,2"::"r"(reg):"memory");} while(0)
#define ATS1HR_set(reg)     do {__asm__ volatile ("mcr p15,4,%0,c7,c8,0"::"r"(reg):"memory");} while(0)
#define ATS1HW_set(reg)     do {__asm__ volatile ("mcr p15,4,%0,c7,c8,1"::"r"(reg):"memory");} while(0)
#define TLBIALLIS_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c8,c3,0"::"r"(reg):"memory");} while(0)
#define TLBIMVAIS_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c8,c3,1"::"r"(reg):"memory");} while(0)
#define TLBIASIDIS_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c8,c3,2"::"r"(reg):"memory");} while(0)
#define TLBIMVAAIS_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c8,c3,3"::"r"(reg):"memory");} while(0)
#define ITLBIALL_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c8,c5,0"::"r"(reg):"memory");} while(0)
#define ITLBIMVA_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c8,c5,1"::"r"(reg):"memory");} while(0)
#define ITLBIASID_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c8,c5,2"::"r"(reg):"memory");} while(0)
#define DTLBIALL_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c8,c6,0"::"r"(reg):"memory");} while(0)
#define DTLBIMVA_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c8,c6,1"::"r"(reg):"memory");} while(0)
#define DTLBIASID_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c8,c6,2"::"r"(reg):"memory");} while(0)
#define TLBIALL_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c8,c7,0"::"r"(reg):"memory");} while(0)
#define TLBIMVA_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c8,c7,1"::"r"(reg):"memory");} while(0)
#define TLBIASID_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c8,c7,2"::"r"(reg):"memory");} while(0)
#define TLBIMVAA_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c8,c7,3"::"r"(reg):"memory");} while(0)
#define TLBIALLHIS_set(reg)     do {__asm__ volatile ("mcr p15,4,%0,c8,c3,0"::"r"(reg):"memory");} while(0)
#define TLBIMVAHIS_set(reg)     do {__asm__ volatile ("mcr p15,4,%0,c8,c3,1"::"r"(reg):"memory");} while(0)
#define TLBIALLNSNHIS_set(reg)     do {__asm__ volatile ("mcr p15,4,%0,c8,c3,4"::"r"(reg):"memory");} while(0)
#define TLBIALLH_set(reg)     do {__asm__ volatile ("mcr p15,4,%0,c8,c7,0"::"r"(reg):"memory");} while(0)
#define TLBIMVAH_set(reg)     do {__asm__ volatile ("mcr p15,4,%0,c8,c7,1"::"r"(reg):"memory");} while (0)
#define TLBIALLNSNH_set(reg)     do {__asm__ volatile ("mcr p15,4,%0,c8,c7,4"::"r"(reg):"memory");} while (0)
#define PRRR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c10,c2,0":"=r"(_reg_)::"memory");_reg_;})
#define PRRR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c10,c2,0"::"r"(reg):"memory");} while (0)
#define MAIR0_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c10,c2,0":"=r"(_reg_)::"memory");_reg_;})
#define MAIR0_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c10,c2,0"::"r"(reg):"memory");} while (0)
#define NMRR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c10,c2,1":"=r"(_reg_)::"memory");_reg_;})
#define NMRR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c10,c2,1"::"r"(reg):"memory");} while (0)
#define MAIR1_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c10,c2,1":"=r"(_reg_)::"memory");_reg_;})
#define MAIR1_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c10,c2,1"::"r"(reg):"memory");} while (0)
#define AMAIR0_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c10,c3,0":"=r"(_reg_)::"memory");_reg_;})
#define AMAIR0_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c10,c3,0"::"r"(reg):"memory");} while (0)
#define AMAIR1_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c10,c3,1":"=r"(_reg_)::"memory");_reg_;})
#define AMAIR1_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c10,c3,1"::"r"(reg):"memory");} while (0)
#define VBAR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c12,c0,0":"=r"(_reg_)::"memory");_reg_;})
#define VBAR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c12,c0,0"::"r"(reg):"memory");} while (0)
#define MVBAR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c12,c0,1":"=r"(_reg_)::"memory");_reg_;})
#define MVBAR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c12,c0,1"::"r"(reg):"memory");} while (0)
#define ISR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c12,c1,0":"=r"(_reg_)::"memory");_reg_;})
#define FCSEIDR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c13,c0,0":"=r"(_reg_)::"memory");_reg_;})
#define FCSEIDR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c13,c0,0"::"r"(reg):"memory");} while (0)
#define CONTEXTIDR_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c13,c0,1":"=r"(_reg_)::"memory");_reg_;})
#define CONTEXTIDR_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c13,c0,1"::"r"(reg):"memory");} while (0)
#define TPIDRURW_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c13,c0,2":"=r"(_reg_)::"memory");_reg_;})
#define TPIDRURW_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c13,c0,2"::"r"(reg):"memory");} while (0)
#define TPIDRURO_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c13,c0,3":"=r"(_reg_)::"memory");_reg_;})
#define TPIDRURO_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c13,c0,3"::"r"(reg):"memory");} while (0)
#define TPIDRPRW_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c13,c0,4":"=r"(_reg_)::"memory");_reg_;})
#define TPIDRPRW_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c13,c0,4"::"r"(reg):"memory");} while (0)
#define CNTFRQ_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c14,c0,0":"=r"(_reg_)::"memory");_reg_;})
#define CNTFRQ_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c14,c0,0"::"r"(reg):"memory");} while (0)
#define CNTKCTL_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c14,c1,0":"=r"(_reg_)::"memory");_reg_;})
#define CNTKCTL_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c14,c1,0"::"r"(reg):"memory");} while (0)
#define CNTP_TVAL_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c14,c2,0":"=r"(_reg_)::"memory");_reg_;})
#define CNTP_TVAL_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c14,c2,0"::"r"(reg):"memory");} while (0)
#define CNTP_CTL_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c14,c2,1":"=r"(_reg_)::"memory");_reg_;})
#define CNTP_CTL_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c14,c2,1"::"r"(reg):"memory");} while (0)
#define CNTV_TVAL_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c14,c3,0":"=r"(_reg_)::"memory");_reg_;})
#define CNTV_TVAL_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c14,c3,0"::"r"(reg):"memory");} while (0)
#define CNTV_CTL_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,0,%0,c14,c3,1":"=r"(_reg_)::"memory");_reg_;})
#define CNTV_CTL_set(reg)     do {__asm__ volatile ("mcr p15,0,%0,c14,c3,1"::"r"(reg):"memory");} while (0)
#define CNTHCTL_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,4,%0,c14,c1,0":"=r"(_reg_)::"memory");_reg_;})
#define CNTHCTL_set(reg)     do {__asm__ volatile ("mcr p15,4,%0,c14,c1,0"::"r"(reg):"memory");} while (0)
#define CNTHP_TVAL_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,4,%0,c14,c2,0":"=r"(_reg_)::"memory");_reg_;})
#define CNTHP_TVAL_set(reg)     do {__asm__ volatile ("mcr p15,4,%0,c14,c2,0"::"r"(reg):"memory");} while (0)
#define CNTHP_CTL_get()     ({uint32_t _reg_;__asm__ volatile ("mrc p15,4,%0,c14,c2,1":"=r"(_reg_)::"memory");_reg_;})
#define CNTHP_CTL_set(reg)     do {__asm__ volatile ("mcr p15,4,%0,c14,c2,1"::"r"(reg):"memory");} while (0)
#define CNTPCT_get()     ({uint32_t _regl_, _regh_;__asm__ volatile ("mrrc p15,0,%0,%1,c14":"=r"(_regl_),"=r"(_regh_)::"memory"); \
										(((uint64_t)(_regh_)<<32)|(uint64_t)(_regl_));})
#define CNTVCT_get()     ({uint32_t _regl_, _regh_;__asm__ volatile ("mrrc p15,1,%0,%1,c14":"=r"(_regl_),"=r"(_regh_)::"memory"); \
										(((uint64_t)(_regh_)<<32)|(uint64_t)(_regl_));})
#define CNTP_CVAL_get()     ({uint32_t _regl_, _regh_;__asm__ volatile ("mrrc p15,2,%0,%1,c14":"=r"(_regl_),"=r"(_regh_)::"memory"); \
										(((uint64_t)(_regh_)<<32)|(uint64_t)(_regl_));})
#define CNTP_CVAL_set(reg)     do {__asm__ volatile ("mcrr p15,2,%0,%1,c14"::"r"((uint32_t)reg),"r"((uint32_t)(reg>>32)):"memory");} while (0)
#define CNTV_CVAL_get()     ({uint32_t _regl_, _regh_;__asm__ volatile ("mrrc p15,3,%0,%1,c14":"=r"(_regl_),"=r"(_regh_)::"memory"); \
										(((uint64_t)(_regh_)<<32)|(uint64_t)(_regl_));})
#define CNTV_CVAL_set(reg)     do {__asm__ volatile ("mcrr p15,3,%0,%1,c14"::"r"((uint32_t)reg),"r"((uint32_t)(reg>>32)):"memory");} while (0)
#define CNTVOFF_get()     ({uint32_t _regl_, _regh_;__asm__ volatile ("mrrc p15,4,%0,%1,c14":"=r"(_regl_),"=r"(_regh_)::"memory"); \
										(((uint64_t)(_regh_)<<32)|(uint64_t)(_regl_));})
#define CNTVOFF_set(reg)     do {__asm__ volatile ("mrrc p15,4,%0,%1,c14"::"r"((uint32_t)reg),"r"((uint32_t)(reg>>32)):"memory");} while (0)
#define CNTHP_CVAL_get()     ({uint32_t _regl_, _regh_;__asm__ volatile ("mrrc p15,6,%0,%1,c14":"=r"(_regl_),"=r"(_regh_)::"memory"); \
										(((uint64_t)(_regh_)<<32)|(uint64_t)(_regl_));})
#define CNTHP_CVAL_set(reg)     do {__asm__ volatile ("mrrc p15,6,%0,%1,c14"::"r"((uint32_t)reg),"r"((uint32_t)(reg>>32)):"memory");} while (0)

#endif

/* SCTLRの各ビット */
#define	SCTLR_TE	(0x01<<30)		/* thumb exception enable */
#define	SCTLR_I		(0x01<<12)		/* instruction cache enable */
#define	SCTLR_Z		(0x01<<11)		/* brunch prediction enable */
#define	SCTLR_C		(0x01<<2)		/* data cache */
#define	SCTLR_M		(0x01<<0)		/* MMU enable */
